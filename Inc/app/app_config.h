/**
 ******************************************************************************
 * @file    app_config.h
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
#ifndef APP_CONFIG
#define APP_CONFIG

#include "postprocess_conf.h"
#include "sysobj_params.h"

/* ---------------------------------------------------------------------------
 * Persistent parameter store configuration
 * 3 × 4 KB sectors placed in the free gap between the face_recognition model
 * (chip offset 0x01000000, ~1.3 MB) and the face_detection model (chip offset
 * 0x02000000).  The XSPI2 MMP window covers chip offsets 0x00000000–0x03FFFFFF
 * (64 MB); placing params at the end of the 32 MB mark keeps them well inside
 * the window and well clear of all three model binaries.
 *   Bank 0: 0x01FFD000   Bank 1: 0x01FFE000   Bank 2: 0x01FFF000
 * NOTE: if models grow past 0x01FFC000 these offsets must be moved further up
 * (but must stay below 0x02000000 to avoid the face_detection binary).
 * ------------------------------------------------------------------------- */
#define PARAM_FLASH_BASE  0x01FFD000UL
#define PARAM_XSPI_INST   0U

/**
 * @brief Application parameter IDs.
 *
 * Each entry corresponds to one row in s_param_table (defined in
 * app_pipeline.c).  Add new IDs here and a matching row in the table.
 * Maximum: PARAMS_MAX_ENTRIES (15).
 */
typedef enum {
  PARAM_CONF_THRESHOLD = 0, /*!< uint32, default=50,  range [0,  100] – detection confidence % */
  PARAM_BRIGHTNESS     = 1, /*!< uint32, default=80,  range [0,  100] – display brightness %   */
  PARAM_TARGET_FPS     = 2, /*!< uint32, default=30,  range [1,   60] – target frame rate       */
  PARAM_ACTIVE_MODEL   = 3, /*!< uint32, default=0,   range [0,   15] – active model index      */
  PARAM_ID_COUNT       = 4,
} app_param_id_t;

#ifndef USE_DCACHE
#define USE_DCACHE
#endif

/* Define sensor info */
#define SENSOR_IMX335_WIDTH 2592
#define SENSOR_IMX335_HEIGHT 1944
#define SENSOR_IMX335_FLIP CMW_MIRRORFLIP_MIRROR

#define SENSOR_VD66GY_WIDTH 1120
#define SENSOR_VD66GY_HEIGHT 720
#define SENSOR_VD66GY_FLIP CMW_MIRRORFLIP_FLIP

#define SENSOR_VD55G1_WIDTH 800
#define SENSOR_VD55G1_HEIGHT 600
#define SENSOR_VD55G1_FLIP CMW_MIRRORFLIP_FLIP

/* Define venc info per sensor */
#define VENC_IMX335_WIDTH 1280
#define VENC_IMX335_HEIGHT 720

#define VENC_VD66GY_WIDTH 1120
#define VENC_VD66GY_HEIGHT 720

#define VENC_VD55G1_WIDTH 640
#define VENC_VD55G1_HEIGHT 480

#define CAMERA_FPS 30

#define CAPTURE_FORMAT DCMIPP_PIXEL_PACKER_FORMAT_ARGB8888
#define CAPTURE_BPP 4

/* Model Related Info */
#define NN_WIDTH 224
#define NN_HEIGHT 224
#define NN_FORMAT DCMIPP_PIXEL_PACKER_FORMAT_RGB888_YUV444_1
#define NN_BPP 3

/* Delay display by CAPTURE_DELAY frame number */
#define CAPTURE_DELAY 1

/* ---------------------------------------------------------------------------
 * Face Recognition pipeline
 * ------------------------------------------------------------------------- */

/* CenterFace detection input resolution (DCMIPP PIPE2 in FR mode) */
#define FR_DET_WIDTH   128
#define FR_DET_HEIGHT  128

/* MobileFaceNet recognition input resolution (cropped + aligned from PIPE1) */
#define FR_REC_WIDTH   112
#define FR_REC_HEIGHT  112

/* Embedding dimension (MobileFaceNet output) */
#define FR_EMBEDDING_DIM  128

/* NOR flash sector reserved for the face embedding bank (one 4 KB sector,
 * immediately below the 3-sector param region — see above). */
#define EMBED_FLASH_BASE  0x01FFC000UL

/* Default cosine-similarity threshold for face verification */
#define FACE_SIM_THRESHOLD     0.55f

/* CenterFace post-processing thresholds */
#define AI_PD_CONF_THRESHOLD   0.5f
#define AI_PD_IOU_THRESHOLD    0.3f
#define AI_PD_MAX_BOXES        10

/* Pipeline mode values stored in PARAM_ACTIVE_MODEL */
#define PIPELINE_MODE_PERSON_DETECTION  0U
#define PIPELINE_MODE_FACE_RECOGNITION  1U

#endif

