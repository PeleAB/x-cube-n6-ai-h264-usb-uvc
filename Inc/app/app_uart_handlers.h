/**
 ******************************************************************************
 * @file    app_uart_handlers.h
 * @brief   App-layer UART state machine and message handler overrides.
 *
 *          Owns the UART state (BOOT → ON ↔ CONFIG) and provides all
 *          sysobj_uart_handle_*() overrides.  main.c is responsible only for
 *          HAL/FreeRTOS init and calling app_run(); all UART business logic
 *          lives here.
 ******************************************************************************
 */
#ifndef APP_UART_HANDLERS_H
#define APP_UART_HANDLERS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define APP_UART_STATE_BOOT   0U
#define APP_UART_STATE_ON     1U
#define APP_UART_STATE_CONFIG 2U

/**
 * @brief Transition from BOOT to ON and broadcast unsolicited GET_STATE.
 *
 * Must be called once from the main thread after app_run() returns, i.e.
 * after all subsystems (camera, pipeline, UART) are fully initialised.
 */
void app_uart_set_ready(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_UART_HANDLERS_H */
