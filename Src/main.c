/**
******************************************************************************
* @file    main.c
* @author  GPM Application Team
*
******************************************************************************
* @attention
*
* Copyright (c) 2023 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

#include "main.h"
#include "FreeRTOS.h"
#include "app/app.h"
#include "app/app_uart_handlers.h"
#include "bsp/platform.h"
#include "stm32n6570_discovery.h"
#include "sysobj/inc/sysobj_uart.h"
#include "task.h"

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* NVIC for USART1 to catch RXNE/IDLE/Error */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

static StaticTask_t main_thread;
static StackType_t main_thread_stack[configMINIMAL_STACK_SIZE * 2];

static int main_freertos(void);
static void main_thread_fct(void *arg);

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void) {
  BSP_EarlyPlatformInit();
  return main_freertos();
}

static int main_freertos() {
  TaskHandle_t hdl;
  hdl = xTaskCreateStatic(
      main_thread_fct, "main", sizeof(main_thread_stack) / sizeof(StackType_t),
      NULL, tskIDLE_PRIORITY + 1, main_thread_stack, &main_thread);
  configASSERT(hdl != NULL);

  vTaskStartScheduler();
  configASSERT(0);
  return -1;
}

static void main_thread_fct(void *arg) {
  uint32_t preemptPriority;
  uint32_t subPriority;
  IRQn_Type i;

  BSP_PlatformInit();
  
  HAL_NVIC_GetPriority(SysTick_IRQn, HAL_NVIC_GetPriorityGrouping(),
                       &preemptPriority, &subPriority);
  for (i = PVD_PVM_IRQn; i <= LTDC_UP_ERR_IRQn; i++)
    HAL_NVIC_SetPriority(i, preemptPriority, subPriority);

  /* Ensure USB has high priority to avoid UVC timeouts under heavy AI load */
  HAL_NVIC_SetPriority(USB1_OTG_HS_IRQn, preemptPriority - 3, subPriority);

  sysobj_uart_init(&huart1);

  /* Use larger stack for UART task just in case */
  xTaskCreate(sysobj_uart_task_func, "sysobj_uart",
              configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 2, NULL);

  app_run();
  app_uart_set_ready();  /* BOOT → ON; broadcasts GET_STATE notification */
  vTaskDelete(NULL);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
  UNUSED(file);
  UNUSED(line);
  __BKPT(0);
  while (1) {
  }
}
#endif

__attribute__((section(".keep_me"))) void app_clean_invalidate_dbg() {
  SCB_CleanInvalidateDCache();
}
