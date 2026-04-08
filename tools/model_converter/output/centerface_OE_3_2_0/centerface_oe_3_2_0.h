/**
  ******************************************************************************
  * @file    centerface_oe_3_2_0.h
  * @author  STEdgeAI
  * @date    2026-03-10 12:08:06
  * @brief   Minimal description of the generated c-implemention of the network
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */
#ifndef LL_ATON_CENTERFACE_OE_3_2_0_H
#define LL_ATON_CENTERFACE_OE_3_2_0_H

/******************************************************************************/
#define LL_ATON_CENTERFACE_OE_3_2_0_C_MODEL_NAME        "centerface_oe_3_2_0"
#define LL_ATON_CENTERFACE_OE_3_2_0_ORIGIN_MODEL_NAME   "centerface_OE_3_2_0"

/************************** USER ALLOCATED IOs ********************************/
// No user allocated inputs
// No user allocated outputs

/************************** INPUTS ********************************************/
#define LL_ATON_CENTERFACE_OE_3_2_0_IN_NUM        (1)    // Total number of input buffers
// Input buffer 1 -- Input_0_out_0
#define LL_ATON_CENTERFACE_OE_3_2_0_IN_1_ALIGNMENT   (32)
#define LL_ATON_CENTERFACE_OE_3_2_0_IN_1_SIZE_BYTES  (196608)

/************************** OUTPUTS *******************************************/
#define LL_ATON_CENTERFACE_OE_3_2_0_OUT_NUM        (4)    // Total number of output buffers
// Output buffer 1 -- Transpose_287_out_0
#define LL_ATON_CENTERFACE_OE_3_2_0_OUT_1_ALIGNMENT   (32)
#define LL_ATON_CENTERFACE_OE_3_2_0_OUT_1_SIZE_BYTES  (2048)
// Output buffer 2 -- Transpose_293_out_0
#define LL_ATON_CENTERFACE_OE_3_2_0_OUT_2_ALIGNMENT   (32)
#define LL_ATON_CENTERFACE_OE_3_2_0_OUT_2_SIZE_BYTES  (10240)
// Output buffer 3 -- Transpose_284_out_0
#define LL_ATON_CENTERFACE_OE_3_2_0_OUT_3_ALIGNMENT   (32)
#define LL_ATON_CENTERFACE_OE_3_2_0_OUT_3_SIZE_BYTES  (1024)
// Output buffer 4 -- Transpose_290_out_0
#define LL_ATON_CENTERFACE_OE_3_2_0_OUT_4_ALIGNMENT   (32)
#define LL_ATON_CENTERFACE_OE_3_2_0_OUT_4_SIZE_BYTES  (2048)

#endif /* LL_ATON_CENTERFACE_OE_3_2_0_H */
