/**
 ******************************************************************************
 * @file    app_pipeline.h
 * @author  GPM Application Team
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifndef APP_PIPELINE_H
#define APP_PIPELINE_H


#include <stdint.h>

void app_pipeline_init(void);
void app_pipeline_start(void);

/**
 * @brief  Early flash-dependent initialisation — called from BSP_PlatformInit
 *         while XSPI2 is still in indirect mode (before EnableMemoryMappedMode).
 *
 * Initialises sysobj_params, embedding_store, and seeds the active-model cache.
 * Must run before BSP_XSPI_NOR_EnableMemoryMappedMode(0) because the CPU cannot
 * perform indirect BSP_XSPI_NOR_Read once MMP mode is active.
 */
void app_flash_early_init(void);

/** Update the SRAM-cached active model (called from UART param handler). */
void app_pipeline_set_active_model(uint32_t mode);

#endif

