/**
 ******************************************************************************
 * @file    app_uart_handlers.c
 * @brief   App-layer UART state machine and message handler overrides.
 *
 *          All sysobj_uart_handle_*() overrides live here.  main.c is kept
 *          thin (HAL MSP, FreeRTOS bootstrap, app_run()).
 ******************************************************************************
 */

#include "app/app_uart_handlers.h"

#include "sysobj_uart.h"
#include "sysobj_params.h"
#include "svc/face_pipeline.h"
#include "svc/embedding_store.h"
#include "svc/app_stats.h"
#include "app/app_config.h"
#include "app/app_pipeline.h"

#include "stm32n6570_discovery.h"

/* ---------------------------------------------------------------------------
 * App-layer UART state machine
 * BOOT(0) → ON(1) ↔ CONFIG(2)
 * ------------------------------------------------------------------------- */
#define APP_UART_STATUS_WRONG_STATE 0x10U

static volatile uint8_t s_uart_state = APP_UART_STATE_BOOT;

/* Send [status, current_state] with the given msg_type/subtype. */
static void app_uart_send_state(uint8_t dst_id, uint8_t msg_type,
                                uint8_t msg_subtype, uint8_t status)
{
  uint8_t payload[2] = { status, s_uart_state };
  sysobj_uart_msg_t msg = {
    .src_id      = 0x02,
    .dst_id      = dst_id,
    .is_ack      = 0,
    .need_ack    = 0,
    .msg_type    = msg_type,
    .msg_subtype = msg_subtype,
    .data        = payload,
    .data_len    = sizeof(payload),
  };
  sysobj_uart_send(&msg);
}

void app_uart_set_ready(void)
{
  s_uart_state = APP_UART_STATE_ON;
  app_uart_send_state(0xFF, SYSOBJ_UART_MSG_TYPE_MANAGE,
                      SYSOBJ_UART_MANAGE_SUBTYPE_GET_STATE, 0);
}

/* ---------------------------------------------------------------------------
 * MANAGE handlers
 * ------------------------------------------------------------------------- */

void sysobj_uart_handle_manage_set_led(uint8_t led_id, uint8_t state)
{
  if (led_id == 1) {
    if (state) BSP_LED_On(LED_GREEN); else BSP_LED_Off(LED_GREEN);
  } else if (led_id == 2) {
    if (state) BSP_LED_On(LED_RED); else BSP_LED_Off(LED_RED);
  }
}

void sysobj_uart_handle_manage_get_state(uint8_t src_id)
{
  app_uart_send_state(src_id, SYSOBJ_UART_MSG_TYPE_MANAGE,
                      SYSOBJ_UART_MANAGE_SUBTYPE_GET_STATE, 0);
}

void sysobj_uart_handle_manage_telemetry(uint8_t src_id)
{
  stat_info_t copy;
  stat_info_copy(&copy);

  float cpu_load_last;
  app_stats_cpuload_get(&cpu_load_last, NULL, NULL);

  uint8_t payload[4];
  payload[0] = (uint8_t)(cpu_load_last);

  uint16_t inf_time = (uint16_t)copy.nn_inference_time.last;
  payload[1] = (uint8_t)(inf_time & 0xFF);
  payload[2] = (uint8_t)((inf_time >> 8) & 0xFF);

  payload[3] = (uint8_t)(copy.nb_detect & 0xFF);

  sysobj_uart_msg_t msg = {
    .src_id      = 0x02,
    .dst_id      = src_id,
    .is_ack      = 0,
    .need_ack    = 0,
    .msg_type    = SYSOBJ_UART_MSG_TYPE_MANAGE,
    .msg_subtype = SYSOBJ_UART_MANAGE_SUBTYPE_TELEMETRY,
    .data        = payload,
    .data_len    = sizeof(payload),
  };
  sysobj_uart_send(&msg);
}

/* ---------------------------------------------------------------------------
 * CONFIG handlers — state transitions
 * ------------------------------------------------------------------------- */

void sysobj_uart_handle_config_enter_config(uint8_t src_id)
{
  uint8_t status;
  if (s_uart_state == APP_UART_STATE_ON) {
    s_uart_state = APP_UART_STATE_CONFIG;
    status = 0;
  } else {
    status = APP_UART_STATUS_WRONG_STATE;
  }
  app_uart_send_state(src_id, SYSOBJ_UART_MSG_TYPE_CONFIG,
                      SYSOBJ_UART_CONFIG_SUBTYPE_ENTER_CONFIG, status);
}

void sysobj_uart_handle_config_exit_config(uint8_t src_id)
{
  uint8_t status;
  if (s_uart_state == APP_UART_STATE_CONFIG) {
    s_uart_state = APP_UART_STATE_ON;
    status = 0;
  } else {
    status = APP_UART_STATUS_WRONG_STATE;
  }
  app_uart_send_state(src_id, SYSOBJ_UART_MSG_TYPE_CONFIG,
                      SYSOBJ_UART_CONFIG_SUBTYPE_EXIT_CONFIG, status);
}

/* ---------------------------------------------------------------------------
 * CONFIG handlers — persistent parameter read/write
 * ------------------------------------------------------------------------- */

void sysobj_uart_handle_config_param_read(uint8_t src_id, uint16_t param_id)
{
  if (s_uart_state != APP_UART_STATE_CONFIG) {
    uint8_t payload[17] = {0};
    payload[0] = APP_UART_STATUS_WRONG_STATE;
    payload[1] = (uint8_t)(param_id & 0xFFU);
    payload[2] = (uint8_t)((param_id >> 8) & 0xFFU);
    sysobj_uart_msg_t err = {
      .src_id = 0x02, .dst_id = src_id, .is_ack = 0, .need_ack = 0,
      .msg_type    = SYSOBJ_UART_MSG_TYPE_CONFIG,
      .msg_subtype = SYSOBJ_UART_CONFIG_SUBTYPE_PARAM_READ,
      .data = payload, .data_len = sizeof(payload),
    };
    sysobj_uart_send(&err);
    return;
  }

  uint64_t value       = 0;
  bool     was_default = false;
  params_status_t st   = sysobj_params_read(param_id, &value, &was_default);

  uint8_t type = (uint8_t)PARAM_TYPE_U32;

  uint8_t payload[17];
  payload[0] = (uint8_t)st;
  payload[1] = (uint8_t)(param_id & 0xFFU);
  payload[2] = (uint8_t)((param_id >> 8) & 0xFFU);
  payload[3] = type;
  for (int i = 0; i < 8; i++)
    payload[4 + i] = (uint8_t)((value >> (8 * i)) & 0xFFU);

  uint8_t crc_input[11];
  crc_input[0] = payload[1]; crc_input[1] = payload[2];
  crc_input[2] = type;
  for (int i = 0; i < 8; i++) crc_input[3 + i] = payload[4 + i];
  uint32_t crc = sysobj_uart_calculate_crc32(crc_input, sizeof(crc_input));
  payload[12] = (uint8_t)(crc & 0xFFU);
  payload[13] = (uint8_t)((crc >> 8) & 0xFFU);
  payload[14] = (uint8_t)((crc >> 16) & 0xFFU);
  payload[15] = (uint8_t)((crc >> 24) & 0xFFU);
  payload[16] = was_default ? 1U : 0U;

  sysobj_uart_msg_t msg = {
    .src_id      = 0x02,
    .dst_id      = src_id,
    .is_ack      = 0,
    .need_ack    = 0,
    .msg_type    = SYSOBJ_UART_MSG_TYPE_CONFIG,
    .msg_subtype = SYSOBJ_UART_CONFIG_SUBTYPE_PARAM_READ,
    .data        = payload,
    .data_len    = sizeof(payload),
  };
  sysobj_uart_send(&msg);
}

void sysobj_uart_handle_config_param_write(uint8_t src_id, uint16_t param_id,
                                           uint8_t type, uint64_t value)
{
  if (s_uart_state != APP_UART_STATE_CONFIG) {
    uint8_t payload[3] = {
      APP_UART_STATUS_WRONG_STATE,
      (uint8_t)(param_id & 0xFFU),
      (uint8_t)((param_id >> 8) & 0xFFU),
    };
    sysobj_uart_msg_t err = {
      .src_id = 0x02, .dst_id = src_id, .is_ack = 0, .need_ack = 0,
      .msg_type    = SYSOBJ_UART_MSG_TYPE_CONFIG,
      .msg_subtype = SYSOBJ_UART_CONFIG_SUBTYPE_PARAM_WRITE,
      .data = payload, .data_len = sizeof(payload),
    };
    sysobj_uart_send(&err);
    return;
  }

  (void)type;
  params_status_t st = sysobj_params_write(param_id, value);

  /* Update SRAM cache so nn/dp threads see the change immediately
   * without reading flash (XSPI is in MMP mode at runtime). */
  if (st == PARAMS_OK && param_id == PARAM_ACTIVE_MODEL)
    app_pipeline_set_active_model((uint32_t)value);

  uint8_t payload[3];
  payload[0] = (uint8_t)st;
  payload[1] = (uint8_t)(param_id & 0xFFU);
  payload[2] = (uint8_t)((param_id >> 8) & 0xFFU);

  sysobj_uart_msg_t msg = {
    .src_id      = 0x02,
    .dst_id      = src_id,
    .is_ack      = 0,
    .need_ack    = 0,
    .msg_type    = SYSOBJ_UART_MSG_TYPE_CONFIG,
    .msg_subtype = SYSOBJ_UART_CONFIG_SUBTYPE_PARAM_WRITE,
    .data        = payload,
    .data_len    = sizeof(payload),
  };
  sysobj_uart_send(&msg);
}

/* ---------------------------------------------------------------------------
 * CONFIG handlers — model select
 * ------------------------------------------------------------------------- */

void sysobj_uart_handle_config_model_select(uint8_t src_id, uint16_t model_id)
{
  if (s_uart_state != APP_UART_STATE_CONFIG) {
    uint8_t payload[3] = {
      APP_UART_STATUS_WRONG_STATE,
      (uint8_t)(model_id & 0xFFU),
      (uint8_t)((model_id >> 8) & 0xFFU),
    };
    sysobj_uart_msg_t err = {
      .src_id = 0x02, .dst_id = src_id, .is_ack = 0, .need_ack = 0,
      .msg_type    = SYSOBJ_UART_MSG_TYPE_CONFIG,
      .msg_subtype = SYSOBJ_UART_CONFIG_SUBTYPE_MODEL_SELECT,
      .data = payload, .data_len = sizeof(payload),
    };
    sysobj_uart_send(&err);
    return;
  }

  params_status_t st = sysobj_params_write(PARAM_ACTIVE_MODEL, (uint64_t)model_id);

  uint8_t payload[3];
  payload[0] = (uint8_t)st;
  payload[1] = (uint8_t)(model_id & 0xFFU);
  payload[2] = (uint8_t)((model_id >> 8) & 0xFFU);

  sysobj_uart_msg_t msg = {
    .src_id      = 0x02,
    .dst_id      = src_id,
    .is_ack      = 0,
    .need_ack    = 0,
    .msg_type    = SYSOBJ_UART_MSG_TYPE_CONFIG,
    .msg_subtype = SYSOBJ_UART_CONFIG_SUBTYPE_MODEL_SELECT,
    .data        = payload,
    .data_len    = sizeof(payload),
  };
  sysobj_uart_send(&msg);
}

/* ---------------------------------------------------------------------------
 * CONFIG handlers — face recognition enrollment
 * ------------------------------------------------------------------------- */

void sysobj_uart_handle_config_enroll(uint8_t src_id)
{
  uint8_t status;
  if (s_uart_state != APP_UART_STATE_CONFIG) {
    status = APP_UART_STATUS_WRONG_STATE;
  } else {
    face_pipeline_set_enroll_pending();
    status = 0;
  }
  uint8_t payload[1] = { status };
  sysobj_uart_msg_t msg = {
    .src_id      = 0x02,
    .dst_id      = src_id,
    .is_ack      = 0,
    .need_ack    = 0,
    .msg_type    = SYSOBJ_UART_MSG_TYPE_CONFIG,
    .msg_subtype = SYSOBJ_UART_CONFIG_SUBTYPE_ENROLL,
    .data        = payload,
    .data_len    = sizeof(payload),
  };
  sysobj_uart_send(&msg);
}

void sysobj_uart_handle_config_commit_enroll(uint8_t src_id)
{
  uint8_t status;
  if (s_uart_state != APP_UART_STATE_CONFIG) {
    status = APP_UART_STATUS_WRONG_STATE;
  } else {
    face_pipeline_set_commit_pending();
    status = 0;
  }
  uint32_t count = embedding_store_sample_count();
  uint8_t payload[5];
  payload[0] = status;
  payload[1] = (uint8_t)(count & 0xFFU);
  payload[2] = (uint8_t)((count >>  8) & 0xFFU);
  payload[3] = (uint8_t)((count >> 16) & 0xFFU);
  payload[4] = (uint8_t)((count >> 24) & 0xFFU);
  sysobj_uart_msg_t msg = {
    .src_id      = 0x02,
    .dst_id      = src_id,
    .is_ack      = 0,
    .need_ack    = 0,
    .msg_type    = SYSOBJ_UART_MSG_TYPE_CONFIG,
    .msg_subtype = SYSOBJ_UART_CONFIG_SUBTYPE_COMMIT_ENROLL,
    .data        = payload,
    .data_len    = sizeof(payload),
  };
  sysobj_uart_send(&msg);
}

void sysobj_uart_handle_config_clear_embeddings(uint8_t src_id)
{
  uint8_t status;
  if (s_uart_state != APP_UART_STATE_CONFIG) {
    status = APP_UART_STATUS_WRONG_STATE;
  } else {
    /* Defer to nn_thread — NOR erase must not race with NPU MMP reads */
    face_pipeline_set_clear_pending();
    status = 0;
  }
  uint8_t payload[1] = { status };
  sysobj_uart_msg_t msg = {
    .src_id      = 0x02,
    .dst_id      = src_id,
    .is_ack      = 0,
    .need_ack    = 0,
    .msg_type    = SYSOBJ_UART_MSG_TYPE_CONFIG,
    .msg_subtype = SYSOBJ_UART_CONFIG_SUBTYPE_CLEAR_EMBEDDINGS,
    .data        = payload,
    .data_len    = sizeof(payload),
  };
  sysobj_uart_send(&msg);
}
