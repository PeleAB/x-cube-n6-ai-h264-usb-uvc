#include "ll_aton_NN_interface.h"
#include "ll_aton.h"
#include "ll_aton_ec_trace.h"

#if 0
// Workaround: the tracer does not know the target at this moment
// and cannot call the functions since are used in static code
#define ATON_LIB_PHYSICAL_TO_VIRTUAL_ADDR(address) LL_Address_Physical2Virtual(address)
#define ATON_LIB_VIRTUAL_TO_PHYSICAL_ADDR(address) LL_Address_Virtual2Physical(address)
#else
#define ATON_LIB_PHYSICAL_TO_VIRTUAL_ADDR(address) (address)
#define ATON_LIB_VIRTUAL_TO_PHYSICAL_ADDR(address) (address)
#endif


// MCU cache line size: 32 (bytes)
// NPU cache line size: 64 (bytes)
// MCU+NPU cache line size equal to 64 bytes (power of 2 not less than 8)
unsigned int cache_line_size = 64;

mpool_reloc_info_t mpool_reloc_info[] = {
  {"AXISRAM6", "_mem_pool_AXISRAM6_centerface_oe_3_2_0", 0x34350000, 1, 0},
  {"AXISRAM5", "_mem_pool_AXISRAM5_centerface_oe_3_2_0", 0x342e0000, 1, 0},
  {"AXISRAM4", "_mem_pool_AXISRAM4_centerface_oe_3_2_0", 0x34270000, 1, 0},
  {"AXISRAM3", "_mem_pool_AXISRAM3_centerface_oe_3_2_0", 0x34200000, 1, 0},
  {"AXISRAM2", "_mem_pool_AXISRAM2_centerface_oe_3_2_0", 0x34100000, 1, 0},
  {"AXISRAM1", "_mem_pool_AXISRAM1_centerface_oe_3_2_0", 0x34080000, 1, 0},
  {"AXIFLEXMEM", "_mem_pool_AXIFLEXMEM_centerface_oe_3_2_0", 0x34000000, 1, 0},
  {"xSPI1", "_mem_pool_xSPI1_centerface_oe_3_2_0", 0x90000000, 1, 0},
  {"xSPI2", "_mem_pool_xSPI2_centerface_oe_3_2_0", 0x71000000, 1, 0},
  {"AXISRAM2_AXISRAM3_AXISRAM4_AXISRAM5_AXISRAM6", "_mem_pool_AXISRAM2_AXISRAM3_AXISRAM4_AXISRAM5_AXISRAM6_centerface_oe_3_2_0", 0x34100000, 1, 0},
  {NULL, NULL, 0, 0, 0}
};


static const uint64_t __ec_params_memory_pool_base[32] = {
0x0249E00000000000ULL,0x0000000011983C49ULL,0xE04000000000E040ULL,0x0000E04000000000ULL,0x00000000E0400000ULL,0xE04000000000E040ULL,0x0000E08000000000ULL,0x00000000E0800000ULL,
0xE09CFDE5FFB6E080ULL,0x00C3E5670A5C00BAULL,0x00000000E5210A00ULL,0xE34000000000E300ULL,0x0000E38000000000ULL,0x10000000E6401000ULL,0xE5C010000000E600ULL,0x0000E4C000000000ULL,
0x0C000200E5801000ULL,0xE58E0A9D025BE567ULL,0x0000E12E187E00FCULL,0xE9110444D9F02800ULL,0xE20020000000F6D7ULL,0x0000D18030000000ULL,0x30000000D1403000ULL,0xD0C030000000D100ULL,
0x0000E30020000000ULL,0x20000000E3402000ULL,0xB4296690FCB8E340ULL,0xFD28EF1018000000ULL,0x10000000B56C605BULL,0x03B80C000000FC40ULL,0xCE0D0C0BC3020100ULL,0x666423DB59181716ULL,
};

void trace_ec__ec_blob_centerface_oe_3_2_0_1(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_1");
  ec_trace_start_epoch(1);
  {
  }
  {
  }
  ec_trace_end_epoch(1);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_1");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_3(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_3");
  ec_trace_start_epoch(3);
  {
  }
  {
  }
  ec_trace_end_epoch(3);
  ec_trace_start_epoch(4);
  {
    /* Unit= 28 [NULL_UNIT 0] */
    /* kind=Identity node=Identity_inserted_id568 */
    /* node=Identity_inserted_id568 satisfies input and output adjacency (DMA->DMA) and can be omitted */

    /* Dma inputs units to cycle: */
    /* Unit= 3 [STREAM_ENG_V2 3] */
    /* Emit conf for STREAM_ENG_V2 node=Identity_inserted_id568 input ports=0 range=1[196608,245760] */

    static const LL_Streng_TensorInitTypeDef Identity_inserted_id568_dma_init_in_0_4 = {
      /* from memory with batch=1 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Transpose_4_out_0_inserted_in568 */
      .offset_start = 196608,
      .offset_end = 212992,
      .offset_limit = 245824,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 16384,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 3,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(3, &Identity_inserted_id568_dma_init_in_0_4, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 49152 */

    /* Dma output units from cycle: */
    /* Unit= 6 [STREAM_ENG_V2 6] */
    /* Emit conf for STREAM_ENG_V2 node=Identity_inserted_id568 output ports=0 range=1[245760,294912] */

    static const LL_Streng_TensorInitTypeDef Identity_inserted_id568_dma_init_out_0_4 = {
      /* to memory canonical from batch=1 */
      .dir = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Transpose_4_out_0_inserted_out568 */
      .offset_start = 245760,
      .offset_limit = 294976,
      .frame_count = 0,
      .fwidth = 128,
      .fheight = 128,
      .batch_depth = 1,
      .batch_offset = 3,
      .frame_offset = 1,
      .line_offset = 0,
      .loop_offset = 49152,
      .frame_loop_cnt = 3,
      .frame_tot_cnt = 3,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(6, &Identity_inserted_id568_dma_init_out_0_4, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 49152 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_4[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 6, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 3, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Identity_inserted_id568 OUT: in unit=STREAM_ENG_V2 6 in port=0 out unit=STREAM_ENG_V2 3 out port=0 */
    };


    /* epoch=4 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_4, 1);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_4_all_units[] = {
      { {STRENG, 6} }, /* STREAM_ENG_V2 */
      { {STRENG, 3} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_4_all_units, 2);

  }

  ec_trace_wait_epoch_end(0x40);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_4[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 6, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 3, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Identity_inserted_id568 OUT: in unit=STREAM_ENG_V2 6 in port=0 out unit=STREAM_ENG_V2 3 out port=0 */
    };


    /* epoch=4 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_4, 1);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_4_all_units[] = {
      { {STRENG, 6} }, /* STREAM_ENG_V2 */
      { {STRENG, 3} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_4_all_units, 2);

  }
  ec_trace_end_epoch(4);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_3");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_29(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_29");
  ec_trace_start_epoch(29);
  {
    /* Unit= 18 [ARITH_ACC_V2 0] */
    /* kind=Add node=Add_42 */
    static const LL_Arithacc_InitTypeDef Add_42_init29 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 1,
      .By_shift = 0,
      .C_shift = 3,
      .fWidth = 32,
      .fHeight = 32,
      .fChannels = 24,
      .batchDepth = 24,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 27613,
      .B_scalar = 16384,
      .C_scalar = -25887,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(0, &Add_42_init29);


    /* Dma inputs units to cycle: */
    /* Unit= 6 [STREAM_ENG_V2 6] */
    /* Emit conf for STREAM_ENG_V2 node=Add_42 input ports=0 range=1[196608,221184] */

    static const LL_Streng_TensorInitTypeDef Add_42_dma_init_in_0_29 = {
      /* from memory with batch=24 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_27_out_0 */
      .offset_start = 196608,
      .offset_end = 221184,
      .offset_limit = 221248,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 24576,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(6, &Add_42_dma_init_in_0_29, 1);

    /* Unit= 2 [STREAM_ENG_V2 2] */
    /* Emit conf for STREAM_ENG_V2 node=Add_42 input ports=1 range=1[98304,122880] */

    static const LL_Streng_TensorInitTypeDef Add_42_dma_init_in_1_29 = {
      /* from memory with batch=24 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_39_out_0 */
      .offset_start = 98304,
      .offset_end = 122880,
      .offset_limit = 122944,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 24576,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(2, &Add_42_dma_init_in_1_29, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 49152 */

    /* Dma output units from cycle: */
    /* Unit= 3 [STREAM_ENG_V2 3] */
    /* Emit conf for STREAM_ENG_V2 node=Add_42 output ports=0 range=1[196608,221184] */

    static const LL_Streng_TensorInitTypeDef Add_42_dma_init_out_0_29 = {
      /* to memory with batch=24 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_42_out_0 */
      .offset_start = 196608,
      .offset_end = 221184,
      .offset_limit = 221248,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 24576,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(3, &Add_42_dma_init_out_0_29, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 24576 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_29[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 6, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_42 IN: in unit=ARITH_ACC_V2 0 in port=0 out unit=STREAM_ENG_V2 6 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_42 IN: in unit=ARITH_ACC_V2 0 in port=1 out unit=STREAM_ENG_V2 2 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 3, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_42 OUT: in unit=STREAM_ENG_V2 3 in port=0 out unit=ARITH_ACC_V2 0 out port=0 */
    };


    /* epoch=29 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_29, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_29_all_units[] = {
      { {STRENG, 3} }, /* STREAM_ENG_V2 */
      { {ARITH, 0} }, /* ARITH_ACC_V2 */
      { {STRENG, 2} }, /* STREAM_ENG_V2 */
      { {STRENG, 6} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_29_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x8);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_29[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 6, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_42 IN: in unit=ARITH_ACC_V2 0 in port=0 out unit=STREAM_ENG_V2 6 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_42 IN: in unit=ARITH_ACC_V2 0 in port=1 out unit=STREAM_ENG_V2 2 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 3, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_42 OUT: in unit=STREAM_ENG_V2 3 in port=0 out unit=ARITH_ACC_V2 0 out port=0 */
    };


    /* epoch=29 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_29, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_29_all_units[] = {
      { {STRENG, 3} }, /* STREAM_ENG_V2 */
      { {ARITH, 0} }, /* ARITH_ACC_V2 */
      { {STRENG, 2} }, /* STREAM_ENG_V2 */
      { {STRENG, 6} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_29_all_units, 4);

  }
  ec_trace_end_epoch(29);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_29");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_50(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_50");
  ec_trace_start_epoch(50);
  {
    /* Unit= 19 [ARITH_ACC_V2 1] */
    /* kind=Add node=Add_69 */
    static const LL_Arithacc_InitTypeDef Add_69_init50 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 1,
      .By_shift = 0,
      .C_shift = 3,
      .fWidth = 16,
      .fHeight = 16,
      .fChannels = 32,
      .batchDepth = 32,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 23248,
      .B_scalar = 16384,
      .C_scalar = 26154,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(1, &Add_69_init50);


    /* Dma inputs units to cycle: */
    /* Unit= 1 [STREAM_ENG_V2 1] */
    /* Emit conf for STREAM_ENG_V2 node=Add_69 input ports=0 range=1[442368,450560] */

    static const LL_Streng_TensorInitTypeDef Add_69_dma_init_in_0_50 = {
      /* from memory with batch=32 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_54_out_0 */
      .offset_start = 442368,
      .offset_end = 450560,
      .offset_limit = 450624,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 8192,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(1, &Add_69_dma_init_in_0_50, 1);

    /* Unit= 9 [STREAM_ENG_V2 9] */
    /* Emit conf for STREAM_ENG_V2 node=Add_69 input ports=1 range=1[0,8192] */

    static const LL_Streng_TensorInitTypeDef Add_69_dma_init_in_1_50 = {
      /* from memory with batch=32 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_66_out_0 */
      .offset_start = 0,
      .offset_end = 8192,
      .offset_limit = 8256,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 8192,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(9, &Add_69_dma_init_in_1_50, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 16384 */

    /* Dma output units from cycle: */
    /* Unit= 0 [STREAM_ENG_V2 0] */
    /* Emit conf for STREAM_ENG_V2 node=Add_69 output ports=0 range=1[442368,450560] */

    static const LL_Streng_TensorInitTypeDef Add_69_dma_init_out_0_50 = {
      /* to memory with batch=32 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_69_out_0 */
      .offset_start = 442368,
      .offset_end = 450560,
      .offset_limit = 450624,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 8192,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(0, &Add_69_dma_init_out_0_50, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 8192 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_50[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 1, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 1, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_69 IN: in unit=ARITH_ACC_V2 1 in port=0 out unit=STREAM_ENG_V2 1 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 1, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 9, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_69 IN: in unit=ARITH_ACC_V2 1 in port=1 out unit=STREAM_ENG_V2 9 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 1, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_69 OUT: in unit=STREAM_ENG_V2 0 in port=0 out unit=ARITH_ACC_V2 1 out port=0 */
    };


    /* epoch=50 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_50, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_50_all_units[] = {
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
      { {ARITH, 1} }, /* ARITH_ACC_V2 */
      { {STRENG, 1} }, /* STREAM_ENG_V2 */
      { {STRENG, 9} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_50_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x1);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_50[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 1, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 1, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_69 IN: in unit=ARITH_ACC_V2 1 in port=0 out unit=STREAM_ENG_V2 1 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 1, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 9, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_69 IN: in unit=ARITH_ACC_V2 1 in port=1 out unit=STREAM_ENG_V2 9 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 1, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_69 OUT: in unit=STREAM_ENG_V2 0 in port=0 out unit=ARITH_ACC_V2 1 out port=0 */
    };


    /* epoch=50 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_50, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_50_all_units[] = {
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
      { {ARITH, 1} }, /* ARITH_ACC_V2 */
      { {STRENG, 1} }, /* STREAM_ENG_V2 */
      { {STRENG, 9} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_50_all_units, 4);

  }
  ec_trace_end_epoch(50);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_50");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_60(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_60");
  ec_trace_start_epoch(60);
  {
    /* Unit= 20 [ARITH_ACC_V2 2] */
    /* kind=Add node=Add_84 */
    static const LL_Arithacc_InitTypeDef Add_84_init60 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 1,
      .By_shift = 0,
      .C_shift = 1,
      .fWidth = 16,
      .fHeight = 16,
      .fChannels = 32,
      .batchDepth = 32,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 19135,
      .B_scalar = 16384,
      .C_scalar = 28702,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(2, &Add_84_init60);


    /* Dma inputs units to cycle: */
    /* Unit= 6 [STREAM_ENG_V2 6] */
    /* Emit conf for STREAM_ENG_V2 node=Add_84 input ports=0 range=1[442368,450560] */

    static const LL_Streng_TensorInitTypeDef Add_84_dma_init_in_0_60 = {
      /* from memory with batch=32 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_69_out_0 */
      .offset_start = 442368,
      .offset_end = 450560,
      .offset_limit = 450624,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 8192,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(6, &Add_84_dma_init_in_0_60, 1);

    /* Unit= 2 [STREAM_ENG_V2 2] */
    /* Emit conf for STREAM_ENG_V2 node=Add_84 input ports=1 range=1[0,8192] */

    static const LL_Streng_TensorInitTypeDef Add_84_dma_init_in_1_60 = {
      /* from memory with batch=32 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_81_out_0 */
      .offset_start = 0,
      .offset_end = 8192,
      .offset_limit = 8256,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 8192,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(2, &Add_84_dma_init_in_1_60, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 16384 */

    /* Dma output units from cycle: */
    /* Unit= 9 [STREAM_ENG_V2 9] */
    /* Emit conf for STREAM_ENG_V2 node=Add_84 output ports=0 range=1[57344,65536] */

    static const LL_Streng_TensorInitTypeDef Add_84_dma_init_out_0_60 = {
      /* to memory with batch=32 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_84_out_0 */
      .offset_start = 57344,
      .offset_end = 65536,
      .offset_limit = 65600,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 8192,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(9, &Add_84_dma_init_out_0_60, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 8192 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_60[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 2, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 6, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_84 IN: in unit=ARITH_ACC_V2 2 in port=0 out unit=STREAM_ENG_V2 6 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 2, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_84 IN: in unit=ARITH_ACC_V2 2 in port=1 out unit=STREAM_ENG_V2 2 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 9, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_84 OUT: in unit=STREAM_ENG_V2 9 in port=0 out unit=ARITH_ACC_V2 2 out port=0 */
    };


    /* epoch=60 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_60, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_60_all_units[] = {
      { {STRENG, 9} }, /* STREAM_ENG_V2 */
      { {ARITH, 2} }, /* ARITH_ACC_V2 */
      { {STRENG, 2} }, /* STREAM_ENG_V2 */
      { {STRENG, 6} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_60_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x200);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_60[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 2, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 6, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_84 IN: in unit=ARITH_ACC_V2 2 in port=0 out unit=STREAM_ENG_V2 6 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 2, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_84 IN: in unit=ARITH_ACC_V2 2 in port=1 out unit=STREAM_ENG_V2 2 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 9, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_84 OUT: in unit=STREAM_ENG_V2 9 in port=0 out unit=ARITH_ACC_V2 2 out port=0 */
    };


    /* epoch=60 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_60, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_60_all_units[] = {
      { {STRENG, 9} }, /* STREAM_ENG_V2 */
      { {ARITH, 2} }, /* ARITH_ACC_V2 */
      { {STRENG, 2} }, /* STREAM_ENG_V2 */
      { {STRENG, 6} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_60_all_units, 4);

  }
  ec_trace_end_epoch(60);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_60");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_81(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_81");
  ec_trace_start_epoch(81);
  {
    /* Unit= 21 [ARITH_ACC_V2 3] */
    /* kind=Add node=Add_111 */
    static const LL_Arithacc_InitTypeDef Add_111_init81 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 1,
      .By_shift = 0,
      .C_shift = 2,
      .fWidth = 8,
      .fHeight = 8,
      .fChannels = 64,
      .batchDepth = 64,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 28959,
      .B_scalar = 16384,
      .C_scalar = -21719,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(3, &Add_111_init81);


    /* Dma inputs units to cycle: */
    /* Unit= 7 [STREAM_ENG_V2 7] */
    /* Emit conf for STREAM_ENG_V2 node=Add_111 input ports=0 range=1[391168,395264] */

    static const LL_Streng_TensorInitTypeDef Add_111_dma_init_in_0_81 = {
      /* from memory with batch=64 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_96_out_0 */
      .offset_start = 391168,
      .offset_end = 395264,
      .offset_limit = 395328,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 4096,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(7, &Add_111_dma_init_in_0_81, 1);

    /* Unit= 5 [STREAM_ENG_V2 5] */
    /* Emit conf for STREAM_ENG_V2 node=Add_111 input ports=1 range=1[387072,391168] */

    static const LL_Streng_TensorInitTypeDef Add_111_dma_init_in_1_81 = {
      /* from memory with batch=64 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_108_out_0 */
      .offset_start = 387072,
      .offset_end = 391168,
      .offset_limit = 391232,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 4096,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(5, &Add_111_dma_init_in_1_81, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 8192 */

    /* Dma output units from cycle: */
    /* Unit= 1 [STREAM_ENG_V2 1] */
    /* Emit conf for STREAM_ENG_V2 node=Add_111 output ports=0 range=1[391168,395264] */

    static const LL_Streng_TensorInitTypeDef Add_111_dma_init_out_0_81 = {
      /* to memory with batch=64 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_111_out_0 */
      .offset_start = 391168,
      .offset_end = 395264,
      .offset_limit = 395328,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 4096,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(1, &Add_111_dma_init_out_0_81, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 4096 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_81[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 3, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 7, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_111 IN: in unit=ARITH_ACC_V2 3 in port=0 out unit=STREAM_ENG_V2 7 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 3, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 5, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_111 IN: in unit=ARITH_ACC_V2 3 in port=1 out unit=STREAM_ENG_V2 5 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 1, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 3, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_111 OUT: in unit=STREAM_ENG_V2 1 in port=0 out unit=ARITH_ACC_V2 3 out port=0 */
    };


    /* epoch=81 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_81, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_81_all_units[] = {
      { {STRENG, 1} }, /* STREAM_ENG_V2 */
      { {ARITH, 3} }, /* ARITH_ACC_V2 */
      { {STRENG, 5} }, /* STREAM_ENG_V2 */
      { {STRENG, 7} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_81_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x2);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_81[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 3, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 7, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_111 IN: in unit=ARITH_ACC_V2 3 in port=0 out unit=STREAM_ENG_V2 7 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 3, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 5, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_111 IN: in unit=ARITH_ACC_V2 3 in port=1 out unit=STREAM_ENG_V2 5 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 1, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 3, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_111 OUT: in unit=STREAM_ENG_V2 1 in port=0 out unit=ARITH_ACC_V2 3 out port=0 */
    };


    /* epoch=81 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_81, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_81_all_units[] = {
      { {STRENG, 1} }, /* STREAM_ENG_V2 */
      { {ARITH, 3} }, /* ARITH_ACC_V2 */
      { {STRENG, 5} }, /* STREAM_ENG_V2 */
      { {STRENG, 7} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_81_all_units, 4);

  }
  ec_trace_end_epoch(81);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_81");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_91(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_91");
  ec_trace_start_epoch(91);
  {
    /* Unit= 18 [ARITH_ACC_V2 0] */
    /* kind=Add node=Add_126 */
    static const LL_Arithacc_InitTypeDef Add_126_init91 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 1,
      .By_shift = 0,
      .C_shift = 1,
      .fWidth = 8,
      .fHeight = 8,
      .fChannels = 64,
      .batchDepth = 64,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 27126,
      .B_scalar = 16384,
      .C_scalar = -20344,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(0, &Add_126_init91);


    /* Dma inputs units to cycle: */
    /* Unit= 1 [STREAM_ENG_V2 1] */
    /* Emit conf for STREAM_ENG_V2 node=Add_126 input ports=0 range=1[391168,395264] */

    static const LL_Streng_TensorInitTypeDef Add_126_dma_init_in_0_91 = {
      /* from memory with batch=64 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_111_out_0 */
      .offset_start = 391168,
      .offset_end = 395264,
      .offset_limit = 395328,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 4096,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(1, &Add_126_dma_init_in_0_91, 1);

    /* Unit= 2 [STREAM_ENG_V2 2] */
    /* Emit conf for STREAM_ENG_V2 node=Add_126 input ports=1 range=1[387072,391168] */

    static const LL_Streng_TensorInitTypeDef Add_126_dma_init_in_1_91 = {
      /* from memory with batch=64 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_123_out_0 */
      .offset_start = 387072,
      .offset_end = 391168,
      .offset_limit = 391232,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 4096,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(2, &Add_126_dma_init_in_1_91, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 8192 */

    /* Dma output units from cycle: */
    /* Unit= 6 [STREAM_ENG_V2 6] */
    /* Emit conf for STREAM_ENG_V2 node=Add_126 output ports=0 range=1[395264,399360] */

    static const LL_Streng_TensorInitTypeDef Add_126_dma_init_out_0_91 = {
      /* to memory with batch=64 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_126_out_0 */
      .offset_start = 395264,
      .offset_end = 399360,
      .offset_limit = 399424,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 4096,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(6, &Add_126_dma_init_out_0_91, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 4096 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_91[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 1, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_126 IN: in unit=ARITH_ACC_V2 0 in port=0 out unit=STREAM_ENG_V2 1 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_126 IN: in unit=ARITH_ACC_V2 0 in port=1 out unit=STREAM_ENG_V2 2 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 6, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_126 OUT: in unit=STREAM_ENG_V2 6 in port=0 out unit=ARITH_ACC_V2 0 out port=0 */
    };


    /* epoch=91 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_91, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_91_all_units[] = {
      { {STRENG, 6} }, /* STREAM_ENG_V2 */
      { {ARITH, 0} }, /* ARITH_ACC_V2 */
      { {STRENG, 1} }, /* STREAM_ENG_V2 */
      { {STRENG, 2} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_91_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x40);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_91[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 1, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_126 IN: in unit=ARITH_ACC_V2 0 in port=0 out unit=STREAM_ENG_V2 1 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_126 IN: in unit=ARITH_ACC_V2 0 in port=1 out unit=STREAM_ENG_V2 2 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 6, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_126 OUT: in unit=STREAM_ENG_V2 6 in port=0 out unit=ARITH_ACC_V2 0 out port=0 */
    };


    /* epoch=91 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_91, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_91_all_units[] = {
      { {STRENG, 6} }, /* STREAM_ENG_V2 */
      { {ARITH, 0} }, /* ARITH_ACC_V2 */
      { {STRENG, 1} }, /* STREAM_ENG_V2 */
      { {STRENG, 2} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_91_all_units, 4);

  }
  ec_trace_end_epoch(91);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_91");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_101(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_101");
  ec_trace_start_epoch(101);
  {
    /* Unit= 19 [ARITH_ACC_V2 1] */
    /* kind=Add node=Add_141 */
    static const LL_Arithacc_InitTypeDef Add_141_init101 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 1,
      .By_shift = 0,
      .C_shift = 3,
      .fWidth = 8,
      .fHeight = 8,
      .fChannels = 64,
      .batchDepth = 64,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 30447,
      .B_scalar = 16384,
      .C_scalar = 20932,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(1, &Add_141_init101);


    /* Dma inputs units to cycle: */
    /* Unit= 8 [STREAM_ENG_V2 8] */
    /* Emit conf for STREAM_ENG_V2 node=Add_141 input ports=0 range=1[395264,399360] */

    static const LL_Streng_TensorInitTypeDef Add_141_dma_init_in_0_101 = {
      /* from memory with batch=64 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_126_out_0 */
      .offset_start = 395264,
      .offset_end = 399360,
      .offset_limit = 399424,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 4096,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(8, &Add_141_dma_init_in_0_101, 1);

    /* Unit= 0 [STREAM_ENG_V2 0] */
    /* Emit conf for STREAM_ENG_V2 node=Add_141 input ports=1 range=1[387072,391168] */

    static const LL_Streng_TensorInitTypeDef Add_141_dma_init_in_1_101 = {
      /* from memory with batch=64 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_138_out_0 */
      .offset_start = 387072,
      .offset_end = 391168,
      .offset_limit = 391232,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 4096,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(0, &Add_141_dma_init_in_1_101, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 8192 */

    /* Dma output units from cycle: */
    /* Unit= 5 [STREAM_ENG_V2 5] */
    /* Emit conf for STREAM_ENG_V2 node=Add_141 output ports=0 range=1[395264,399360] */

    static const LL_Streng_TensorInitTypeDef Add_141_dma_init_out_0_101 = {
      /* to memory with batch=64 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_141_out_0 */
      .offset_start = 395264,
      .offset_end = 399360,
      .offset_limit = 399424,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 4096,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(5, &Add_141_dma_init_out_0_101, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 4096 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_101[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 1, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 8, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_141 IN: in unit=ARITH_ACC_V2 1 in port=0 out unit=STREAM_ENG_V2 8 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 1, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_141 IN: in unit=ARITH_ACC_V2 1 in port=1 out unit=STREAM_ENG_V2 0 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 5, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 1, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_141 OUT: in unit=STREAM_ENG_V2 5 in port=0 out unit=ARITH_ACC_V2 1 out port=0 */
    };


    /* epoch=101 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_101, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_101_all_units[] = {
      { {STRENG, 5} }, /* STREAM_ENG_V2 */
      { {ARITH, 1} }, /* ARITH_ACC_V2 */
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
      { {STRENG, 8} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_101_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x20);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_101[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 1, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 8, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_141 IN: in unit=ARITH_ACC_V2 1 in port=0 out unit=STREAM_ENG_V2 8 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 1, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_141 IN: in unit=ARITH_ACC_V2 1 in port=1 out unit=STREAM_ENG_V2 0 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 5, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 1, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_141 OUT: in unit=STREAM_ENG_V2 5 in port=0 out unit=ARITH_ACC_V2 1 out port=0 */
    };


    /* epoch=101 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_101, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_101_all_units[] = {
      { {STRENG, 5} }, /* STREAM_ENG_V2 */
      { {ARITH, 1} }, /* ARITH_ACC_V2 */
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
      { {STRENG, 8} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_101_all_units, 4);

  }
  ec_trace_end_epoch(101);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_101");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_119(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_119");
  ec_trace_start_epoch(119);
  {
    /* Unit= 20 [ARITH_ACC_V2 2] */
    /* kind=Add node=Add_168 */
    static const LL_Arithacc_InitTypeDef Add_168_init119 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 1,
      .By_shift = 0,
      .C_shift = 3,
      .fWidth = 8,
      .fHeight = 8,
      .fChannels = 96,
      .batchDepth = 96,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 27516,
      .B_scalar = 16384,
      .C_scalar = -27516,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(2, &Add_168_init119);


    /* Dma inputs units to cycle: */
    /* Unit= 2 [STREAM_ENG_V2 2] */
    /* Emit conf for STREAM_ENG_V2 node=Add_168 input ports=0 range=1[356352,362496] */

    static const LL_Streng_TensorInitTypeDef Add_168_dma_init_in_0_119 = {
      /* from memory with batch=96 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_153_out_0 */
      .offset_start = 356352,
      .offset_end = 362496,
      .offset_limit = 362560,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 6144,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(2, &Add_168_dma_init_in_0_119, 1);

    /* Unit= 9 [STREAM_ENG_V2 9] */
    /* Emit conf for STREAM_ENG_V2 node=Add_168 input ports=1 range=1[387072,393216] */

    static const LL_Streng_TensorInitTypeDef Add_168_dma_init_in_1_119 = {
      /* from memory with batch=96 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_165_out_0 */
      .offset_start = 387072,
      .offset_end = 393216,
      .offset_limit = 393280,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 6144,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(9, &Add_168_dma_init_in_1_119, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 12288 */

    /* Dma output units from cycle: */
    /* Unit= 4 [STREAM_ENG_V2 4] */
    /* Emit conf for STREAM_ENG_V2 node=Add_168 output ports=0 range=1[356352,362496] */

    static const LL_Streng_TensorInitTypeDef Add_168_dma_init_out_0_119 = {
      /* to memory with batch=96 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_168_out_0 */
      .offset_start = 356352,
      .offset_end = 362496,
      .offset_limit = 362560,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 6144,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(4, &Add_168_dma_init_out_0_119, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 6144 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_119[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 2, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_168 IN: in unit=ARITH_ACC_V2 2 in port=0 out unit=STREAM_ENG_V2 2 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 2, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 9, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_168 IN: in unit=ARITH_ACC_V2 2 in port=1 out unit=STREAM_ENG_V2 9 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 4, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_168 OUT: in unit=STREAM_ENG_V2 4 in port=0 out unit=ARITH_ACC_V2 2 out port=0 */
    };


    /* epoch=119 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_119, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_119_all_units[] = {
      { {STRENG, 4} }, /* STREAM_ENG_V2 */
      { {ARITH, 2} }, /* ARITH_ACC_V2 */
      { {STRENG, 2} }, /* STREAM_ENG_V2 */
      { {STRENG, 9} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_119_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x10);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_119[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 2, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_168 IN: in unit=ARITH_ACC_V2 2 in port=0 out unit=STREAM_ENG_V2 2 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 2, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 9, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_168 IN: in unit=ARITH_ACC_V2 2 in port=1 out unit=STREAM_ENG_V2 9 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 4, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_168 OUT: in unit=STREAM_ENG_V2 4 in port=0 out unit=ARITH_ACC_V2 2 out port=0 */
    };


    /* epoch=119 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_119, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_119_all_units[] = {
      { {STRENG, 4} }, /* STREAM_ENG_V2 */
      { {ARITH, 2} }, /* ARITH_ACC_V2 */
      { {STRENG, 2} }, /* STREAM_ENG_V2 */
      { {STRENG, 9} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_119_all_units, 4);

  }
  ec_trace_end_epoch(119);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_119");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_129(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_129");
  ec_trace_start_epoch(129);
  {
    /* Unit= 21 [ARITH_ACC_V2 3] */
    /* kind=Add node=Add_183 */
    static const LL_Arithacc_InitTypeDef Add_183_init129 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 1,
      .By_shift = 0,
      .C_shift = 0,
      .fWidth = 8,
      .fHeight = 8,
      .fChannels = 96,
      .batchDepth = 96,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 20789,
      .B_scalar = 16384,
      .C_scalar = -20789,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(3, &Add_183_init129);


    /* Dma inputs units to cycle: */
    /* Unit= 8 [STREAM_ENG_V2 8] */
    /* Emit conf for STREAM_ENG_V2 node=Add_183 input ports=0 range=1[356352,362496] */

    static const LL_Streng_TensorInitTypeDef Add_183_dma_init_in_0_129 = {
      /* from memory with batch=96 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_168_out_0 */
      .offset_start = 356352,
      .offset_end = 362496,
      .offset_limit = 362560,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 6144,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(8, &Add_183_dma_init_in_0_129, 1);

    /* Unit= 0 [STREAM_ENG_V2 0] */
    /* Emit conf for STREAM_ENG_V2 node=Add_183 input ports=1 range=1[387072,393216] */

    static const LL_Streng_TensorInitTypeDef Add_183_dma_init_in_1_129 = {
      /* from memory with batch=96 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_180_out_0 */
      .offset_start = 387072,
      .offset_end = 393216,
      .offset_limit = 393280,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 6144,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(0, &Add_183_dma_init_in_1_129, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 12288 */

    /* Dma output units from cycle: */
    /* Unit= 3 [STREAM_ENG_V2 3] */
    /* Emit conf for STREAM_ENG_V2 node=Add_183 output ports=0 range=1[393216,399360] */

    static const LL_Streng_TensorInitTypeDef Add_183_dma_init_out_0_129 = {
      /* to memory with batch=96 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_183_out_0 */
      .offset_start = 393216,
      .offset_end = 399360,
      .offset_limit = 399424,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 6144,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(3, &Add_183_dma_init_out_0_129, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 6144 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_129[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 3, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 8, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_183 IN: in unit=ARITH_ACC_V2 3 in port=0 out unit=STREAM_ENG_V2 8 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 3, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_183 IN: in unit=ARITH_ACC_V2 3 in port=1 out unit=STREAM_ENG_V2 0 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 3, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 3, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_183 OUT: in unit=STREAM_ENG_V2 3 in port=0 out unit=ARITH_ACC_V2 3 out port=0 */
    };


    /* epoch=129 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_129, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_129_all_units[] = {
      { {STRENG, 3} }, /* STREAM_ENG_V2 */
      { {ARITH, 3} }, /* ARITH_ACC_V2 */
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
      { {STRENG, 8} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_129_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x8);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_129[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 3, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 8, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_183 IN: in unit=ARITH_ACC_V2 3 in port=0 out unit=STREAM_ENG_V2 8 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 3, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_183 IN: in unit=ARITH_ACC_V2 3 in port=1 out unit=STREAM_ENG_V2 0 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 3, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 3, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_183 OUT: in unit=STREAM_ENG_V2 3 in port=0 out unit=ARITH_ACC_V2 3 out port=0 */
    };


    /* epoch=129 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_129, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_129_all_units[] = {
      { {STRENG, 3} }, /* STREAM_ENG_V2 */
      { {ARITH, 3} }, /* ARITH_ACC_V2 */
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
      { {STRENG, 8} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_129_all_units, 4);

  }
  ec_trace_end_epoch(129);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_129");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_150(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_150");
  ec_trace_start_epoch(150);
  {
    /* Unit= 18 [ARITH_ACC_V2 0] */
    /* kind=Add node=Add_210 */
    static const LL_Arithacc_InitTypeDef Add_210_init150 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 1,
      .By_shift = 0,
      .C_shift = 2,
      .fWidth = 4,
      .fHeight = 4,
      .fChannels = 160,
      .batchDepth = 160,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 27897,
      .B_scalar = 16384,
      .C_scalar = 17436,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(0, &Add_210_init150);


    /* Dma inputs units to cycle: */
    /* Unit= 4 [STREAM_ENG_V2 4] */
    /* Emit conf for STREAM_ENG_V2 node=Add_210 input ports=0 range=1[387072,389632] */

    static const LL_Streng_TensorInitTypeDef Add_210_dma_init_in_0_150 = {
      /* from memory with batch=160 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_195_out_0 */
      .offset_start = 387072,
      .offset_end = 389632,
      .offset_limit = 389696,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 2560,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(4, &Add_210_dma_init_in_0_150, 1);

    /* Unit= 9 [STREAM_ENG_V2 9] */
    /* Emit conf for STREAM_ENG_V2 node=Add_210 input ports=1 range=1[0,2560] */

    static const LL_Streng_TensorInitTypeDef Add_210_dma_init_in_1_150 = {
      /* from memory with batch=160 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_207_out_0 */
      .offset_start = 0,
      .offset_end = 2560,
      .offset_limit = 2624,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 2560,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(9, &Add_210_dma_init_in_1_150, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 5120 */

    /* Dma output units from cycle: */
    /* Unit= 0 [STREAM_ENG_V2 0] */
    /* Emit conf for STREAM_ENG_V2 node=Add_210 output ports=0 range=1[389632,392192] */

    static const LL_Streng_TensorInitTypeDef Add_210_dma_init_out_0_150 = {
      /* to memory with batch=160 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_210_out_0 */
      .offset_start = 389632,
      .offset_end = 392192,
      .offset_limit = 392256,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 2560,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(0, &Add_210_dma_init_out_0_150, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 2560 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_150[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 4, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_210 IN: in unit=ARITH_ACC_V2 0 in port=0 out unit=STREAM_ENG_V2 4 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 9, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_210 IN: in unit=ARITH_ACC_V2 0 in port=1 out unit=STREAM_ENG_V2 9 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_210 OUT: in unit=STREAM_ENG_V2 0 in port=0 out unit=ARITH_ACC_V2 0 out port=0 */
    };


    /* epoch=150 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_150, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_150_all_units[] = {
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
      { {ARITH, 0} }, /* ARITH_ACC_V2 */
      { {STRENG, 4} }, /* STREAM_ENG_V2 */
      { {STRENG, 9} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_150_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x1);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_150[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 4, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_210 IN: in unit=ARITH_ACC_V2 0 in port=0 out unit=STREAM_ENG_V2 4 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 9, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_210 IN: in unit=ARITH_ACC_V2 0 in port=1 out unit=STREAM_ENG_V2 9 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_210 OUT: in unit=STREAM_ENG_V2 0 in port=0 out unit=ARITH_ACC_V2 0 out port=0 */
    };


    /* epoch=150 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_150, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_150_all_units[] = {
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
      { {ARITH, 0} }, /* ARITH_ACC_V2 */
      { {STRENG, 4} }, /* STREAM_ENG_V2 */
      { {STRENG, 9} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_150_all_units, 4);

  }
  ec_trace_end_epoch(150);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_150");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_160(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_160");
  ec_trace_start_epoch(160);
  {
    /* Unit= 19 [ARITH_ACC_V2 1] */
    /* kind=Add node=Add_225 */
    static const LL_Arithacc_InitTypeDef Add_225_init160 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 1,
      .By_shift = 0,
      .C_shift = 0,
      .fWidth = 4,
      .fHeight = 4,
      .fChannels = 160,
      .batchDepth = 160,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 23338,
      .B_scalar = 16384,
      .C_scalar = -23338,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(1, &Add_225_init160);


    /* Dma inputs units to cycle: */
    /* Unit= 3 [STREAM_ENG_V2 3] */
    /* Emit conf for STREAM_ENG_V2 node=Add_225 input ports=0 range=1[389632,392192] */

    static const LL_Streng_TensorInitTypeDef Add_225_dma_init_in_0_160 = {
      /* from memory with batch=160 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_210_out_0 */
      .offset_start = 389632,
      .offset_end = 392192,
      .offset_limit = 392256,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 2560,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(3, &Add_225_dma_init_in_0_160, 1);

    /* Unit= 8 [STREAM_ENG_V2 8] */
    /* Emit conf for STREAM_ENG_V2 node=Add_225 input ports=1 range=1[0,2560] */

    static const LL_Streng_TensorInitTypeDef Add_225_dma_init_in_1_160 = {
      /* from memory with batch=160 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_222_out_0 */
      .offset_start = 0,
      .offset_end = 2560,
      .offset_limit = 2624,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 2560,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(8, &Add_225_dma_init_in_1_160, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 5120 */

    /* Dma output units from cycle: */
    /* Unit= 5 [STREAM_ENG_V2 5] */
    /* Emit conf for STREAM_ENG_V2 node=Add_225 output ports=0 range=1[389632,392192] */

    static const LL_Streng_TensorInitTypeDef Add_225_dma_init_out_0_160 = {
      /* to memory with batch=160 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_225_out_0 */
      .offset_start = 389632,
      .offset_end = 392192,
      .offset_limit = 392256,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 2560,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(5, &Add_225_dma_init_out_0_160, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 2560 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_160[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 1, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 3, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_225 IN: in unit=ARITH_ACC_V2 1 in port=0 out unit=STREAM_ENG_V2 3 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 1, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 8, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_225 IN: in unit=ARITH_ACC_V2 1 in port=1 out unit=STREAM_ENG_V2 8 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 5, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 1, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_225 OUT: in unit=STREAM_ENG_V2 5 in port=0 out unit=ARITH_ACC_V2 1 out port=0 */
    };


    /* epoch=160 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_160, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_160_all_units[] = {
      { {STRENG, 5} }, /* STREAM_ENG_V2 */
      { {ARITH, 1} }, /* ARITH_ACC_V2 */
      { {STRENG, 3} }, /* STREAM_ENG_V2 */
      { {STRENG, 8} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_160_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x20);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_160[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 1, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 3, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_225 IN: in unit=ARITH_ACC_V2 1 in port=0 out unit=STREAM_ENG_V2 3 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 1, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 8, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_225 IN: in unit=ARITH_ACC_V2 1 in port=1 out unit=STREAM_ENG_V2 8 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 5, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 1, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_225 OUT: in unit=STREAM_ENG_V2 5 in port=0 out unit=ARITH_ACC_V2 1 out port=0 */
    };


    /* epoch=160 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_160, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_160_all_units[] = {
      { {STRENG, 5} }, /* STREAM_ENG_V2 */
      { {ARITH, 1} }, /* ARITH_ACC_V2 */
      { {STRENG, 3} }, /* STREAM_ENG_V2 */
      { {STRENG, 8} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_160_all_units, 4);

  }
  ec_trace_end_epoch(160);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_160");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_175(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_175");
  ec_trace_start_epoch(175);
  {
    /* Unit= 20 [ARITH_ACC_V2 2] */
    /* kind=Add node=Add_251 */
    static const LL_Arithacc_InitTypeDef Add_251_init175 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 0,
      .By_shift = 1,
      .C_shift = 6,
      .fWidth = 8,
      .fHeight = 8,
      .fChannels = 24,
      .batchDepth = 24,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 16384,
      .B_scalar = 31627,
      .C_scalar = 31627,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(2, &Add_251_init175);


    /* Dma inputs units to cycle: */
    /* Unit= 5 [STREAM_ENG_V2 5] */
    /* Emit conf for STREAM_ENG_V2 node=Add_251 input ports=0 range=1[393216,394752] */

    static const LL_Streng_TensorInitTypeDef Add_251_dma_init_in_0_175 = {
      /* from memory with batch=24 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* ConvTranspose_244_expanded_conv_6_out_7 */
      .offset_start = 393216,
      .offset_end = 394752,
      .offset_limit = 394816,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 1536,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(5, &Add_251_dma_init_in_0_175, 1);

    /* Unit= 8 [STREAM_ENG_V2 8] */
    /* Emit conf for STREAM_ENG_V2 node=Add_251 input ports=1 range=1[399360,400896] */

    static const LL_Streng_TensorInitTypeDef Add_251_dma_init_in_1_175 = {
      /* from memory with batch=24 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Relu_250_out_0 */
      .offset_start = 399360,
      .offset_end = 400896,
      .offset_limit = 400960,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 1536,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(8, &Add_251_dma_init_in_1_175, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 3072 */

    /* Dma output units from cycle: */
    /* Unit= 0 [STREAM_ENG_V2 0] */
    /* Emit conf for STREAM_ENG_V2 node=Add_251 output ports=0 range=1[393216,394752] */

    static const LL_Streng_TensorInitTypeDef Add_251_dma_init_out_0_175 = {
      /* to memory with batch=24 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_251_out_0 */
      .offset_start = 393216,
      .offset_end = 394752,
      .offset_limit = 394816,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 1536,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(0, &Add_251_dma_init_out_0_175, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 1536 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_175[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 2, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 5, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_251 IN: in unit=ARITH_ACC_V2 2 in port=0 out unit=STREAM_ENG_V2 5 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 2, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 8, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_251 IN: in unit=ARITH_ACC_V2 2 in port=1 out unit=STREAM_ENG_V2 8 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_251 OUT: in unit=STREAM_ENG_V2 0 in port=0 out unit=ARITH_ACC_V2 2 out port=0 */
    };


    /* epoch=175 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_175, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_175_all_units[] = {
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
      { {ARITH, 2} }, /* ARITH_ACC_V2 */
      { {STRENG, 5} }, /* STREAM_ENG_V2 */
      { {STRENG, 8} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_175_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x1);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_175[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 2, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 5, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_251 IN: in unit=ARITH_ACC_V2 2 in port=0 out unit=STREAM_ENG_V2 5 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 2, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 8, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_251 IN: in unit=ARITH_ACC_V2 2 in port=1 out unit=STREAM_ENG_V2 8 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_251 OUT: in unit=STREAM_ENG_V2 0 in port=0 out unit=ARITH_ACC_V2 2 out port=0 */
    };


    /* epoch=175 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_175, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_175_all_units[] = {
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
      { {ARITH, 2} }, /* ARITH_ACC_V2 */
      { {STRENG, 5} }, /* STREAM_ENG_V2 */
      { {STRENG, 8} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_175_all_units, 4);

  }
  ec_trace_end_epoch(175);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_175");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_181(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_181");
  ec_trace_start_epoch(181);
  {
    /* Unit= 21 [ARITH_ACC_V2 3] */
    /* kind=Add node=Add_261 */
    static const LL_Arithacc_InitTypeDef Add_261_init181 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 0,
      .By_shift = 2,
      .C_shift = 5,
      .fWidth = 16,
      .fHeight = 16,
      .fChannels = 24,
      .batchDepth = 24,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 16384,
      .B_scalar = 26774,
      .C_scalar = 26774,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(3, &Add_261_init181);


    /* Dma inputs units to cycle: */
    /* Unit= 9 [STREAM_ENG_V2 9] */
    /* Emit conf for STREAM_ENG_V2 node=Add_261 input ports=0 range=1[387072,393216] */

    static const LL_Streng_TensorInitTypeDef Add_261_dma_init_in_0_181 = {
      /* from memory with batch=24 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* ConvTranspose_254_expanded_conv_14_out_15 */
      .offset_start = 387072,
      .offset_end = 393216,
      .offset_limit = 393280,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 6144,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(9, &Add_261_dma_init_in_0_181, 1);

    /* Unit= 0 [STREAM_ENG_V2 0] */
    /* Emit conf for STREAM_ENG_V2 node=Add_261 input ports=1 range=1[380928,387072] */

    static const LL_Streng_TensorInitTypeDef Add_261_dma_init_in_1_181 = {
      /* from memory with batch=24 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Relu_260_out_0 */
      .offset_start = 380928,
      .offset_end = 387072,
      .offset_limit = 387136,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 6144,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(0, &Add_261_dma_init_in_1_181, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 12288 */

    /* Dma output units from cycle: */
    /* Unit= 3 [STREAM_ENG_V2 3] */
    /* Emit conf for STREAM_ENG_V2 node=Add_261 output ports=0 range=1[387072,393216] */

    static const LL_Streng_TensorInitTypeDef Add_261_dma_init_out_0_181 = {
      /* to memory with batch=24 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_261_out_0 */
      .offset_start = 387072,
      .offset_end = 393216,
      .offset_limit = 393280,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 6144,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(3, &Add_261_dma_init_out_0_181, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 6144 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_181[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 3, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 9, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_261 IN: in unit=ARITH_ACC_V2 3 in port=0 out unit=STREAM_ENG_V2 9 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 3, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_261 IN: in unit=ARITH_ACC_V2 3 in port=1 out unit=STREAM_ENG_V2 0 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 3, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 3, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_261 OUT: in unit=STREAM_ENG_V2 3 in port=0 out unit=ARITH_ACC_V2 3 out port=0 */
    };


    /* epoch=181 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_181, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_181_all_units[] = {
      { {STRENG, 3} }, /* STREAM_ENG_V2 */
      { {ARITH, 3} }, /* ARITH_ACC_V2 */
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
      { {STRENG, 9} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_181_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x8);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_181[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 3, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 9, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_261 IN: in unit=ARITH_ACC_V2 3 in port=0 out unit=STREAM_ENG_V2 9 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 3, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_261 IN: in unit=ARITH_ACC_V2 3 in port=1 out unit=STREAM_ENG_V2 0 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 3, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 3, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_261 OUT: in unit=STREAM_ENG_V2 3 in port=0 out unit=ARITH_ACC_V2 3 out port=0 */
    };


    /* epoch=181 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_181, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_181_all_units[] = {
      { {STRENG, 3} }, /* STREAM_ENG_V2 */
      { {ARITH, 3} }, /* ARITH_ACC_V2 */
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
      { {STRENG, 9} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_181_all_units, 4);

  }
  ec_trace_end_epoch(181);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_181");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_187(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_187");
  ec_trace_start_epoch(187);
  {
    /* Unit= 18 [ARITH_ACC_V2 0] */
    /* kind=Add node=Add_271 */
    static const LL_Arithacc_InitTypeDef Add_271_init187 = {
      .rounding_x = 0,
      .saturation_x = 0,
      .round_mode_x = 0,
      .inbytes_x = 1,
      .outbytes_x = 1,
      .shift_x = 0,
      .rounding_y = 0,
      .saturation_y = 0,
      .round_mode_y = 0,
      .inbytes_y = 1,
      .outbytes_y = 1,
      .combinebc = 0,
      .clipout = 0,
      .shift_y = 0,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .shift_o = 14,
      .scalar = 1,
      .dualinput = 1,
      .operation = ARITH_AFFINE,
      .bcast = ARITH_BCAST_NONE,
      .Ax_shift = 0,
      .By_shift = 2,
      .C_shift = 5,
      .fWidth = 32,
      .fHeight = 32,
      .fChannels = 24,
      .batchDepth = 24,
      .clipmin = 0,
      .clipmax = 0,
      .A_scalar = 16384,
      .B_scalar = 25326,
      .C_scalar = 25326,
      .A_vector = {0},
      .B_vector = {0},
      .C_vector = {0},
      .vec_precision = {8, 8, 8},
    };

    /* Unit=ARITH_ACC_V2 */
    LL_Arithacc_Init(0, &Add_271_init187);


    /* Dma inputs units to cycle: */
    /* Unit= 1 [STREAM_ENG_V2 1] */
    /* Emit conf for STREAM_ENG_V2 node=Add_271 input ports=0 range=1[196608,221184] */

    static const LL_Streng_TensorInitTypeDef Add_271_dma_init_in_0_187 = {
      /* from memory with batch=24 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* ConvTranspose_264_expanded_conv_22_out_23 */
      .offset_start = 196608,
      .offset_end = 221184,
      .offset_limit = 221248,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 24576,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(1, &Add_271_dma_init_in_0_187, 1);

    /* Unit= 2 [STREAM_ENG_V2 2] */
    /* Emit conf for STREAM_ENG_V2 node=Add_271 input ports=1 range=2[32768,57344] */

    static const LL_Streng_TensorInitTypeDef Add_271_dma_init_in_1_187 = {
      /* from memory with batch=24 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x34270000UL) /* Equivalent hex address = 0x34270000UL */}, /* Relu_270_out_0 */
      .offset_start = 32768,
      .offset_end = 57344,
      .offset_limit = 57408,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 24576,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(2, &Add_271_dma_init_in_1_187, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 24576 */
    /* npuRAM4 -> 24576 */

    /* Dma output units from cycle: */
    /* Unit= 6 [STREAM_ENG_V2 6] */
    /* Emit conf for STREAM_ENG_V2 node=Add_271 output ports=0 range=1[196608,221184] */

    static const LL_Streng_TensorInitTypeDef Add_271_dma_init_out_0_187 = {
      /* to memory with batch=24 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Add_271_out_0 */
      .offset_start = 196608,
      .offset_end = 221184,
      .offset_limit = 221248,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 24576,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(6, &Add_271_dma_init_out_0_187, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 24576 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_187[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 1, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_271 IN: in unit=ARITH_ACC_V2 0 in port=0 out unit=STREAM_ENG_V2 1 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_271 IN: in unit=ARITH_ACC_V2 0 in port=1 out unit=STREAM_ENG_V2 2 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 6, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_271 OUT: in unit=STREAM_ENG_V2 6 in port=0 out unit=ARITH_ACC_V2 0 out port=0 */
    };


    /* epoch=187 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_187, 3);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_187_all_units[] = {
      { {STRENG, 6} }, /* STREAM_ENG_V2 */
      { {ARITH, 0} }, /* ARITH_ACC_V2 */
      { {STRENG, 1} }, /* STREAM_ENG_V2 */
      { {STRENG, 2} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_187_all_units, 4);

  }

  ec_trace_wait_epoch_end(0x40);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_187[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 1, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_271 IN: in unit=ARITH_ACC_V2 0 in port=0 out unit=STREAM_ENG_V2 1 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ARITH, 0, 1), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 2, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_271 IN: in unit=ARITH_ACC_V2 0 in port=1 out unit=STREAM_ENG_V2 2 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 6, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ARITH, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Add_271 OUT: in unit=STREAM_ENG_V2 6 in port=0 out unit=ARITH_ACC_V2 0 out port=0 */
    };


    /* epoch=187 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_187, 3);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_187_all_units[] = {
      { {STRENG, 6} }, /* STREAM_ENG_V2 */
      { {ARITH, 0} }, /* ARITH_ACC_V2 */
      { {STRENG, 1} }, /* STREAM_ENG_V2 */
      { {STRENG, 2} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_187_all_units, 4);

  }
  ec_trace_end_epoch(187);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_187");
}

void trace_ec__ec_blob_centerface_oe_3_2_0_203(void) {
  ec_trace_start_blob("_ec_blob_centerface_oe_3_2_0_203");
  ec_trace_start_epoch(203);
  {
    /* Unit= 16 [ACTIV_ACC_V2 0] */
    /* kind=Sigmoid node=Sigmoid_282 */
    static const LL_Activacc_InitTypeDef Sigmoid_282_init203 = {
      .rounding_f = 0,
      .saturation_f = 0,
      .round_mode_f = 0,
      .inbytes_f = 1,
      .outbytes_f = 1,
      .rounding_o = 1,
      .saturation_o = 1,
      .round_mode_o = 1,
      .relu_mode_o = 0,
      .outbytes_o = 1,
      .signedop = 1,
      .shift_f = 0,
      .shift_o = 15,
      .parameter = 32640,
      .parameter_2 = 0,
      .ROM0_vector = {((unsigned char *)(ATON_LIB_VIRTUAL_TO_PHYSICAL_ADDR(((uintptr_t)(__ec_params_memory_pool_base)) + 240))) /* Equivalent hex offset = 0xf0 */},
      .ROM1_vector = {((unsigned char *)(ATON_LIB_VIRTUAL_TO_PHYSICAL_ADDR(((uintptr_t)(__ec_params_memory_pool_base)) + 0))) /* Equivalent hex offset = 0x0 */},
      .ROM0_nbytes = 16,
      .ROM1_nbytes = 240,
      .shift_b = 3,
      .shift_c = 9,
      .shift_norm = 8,
      .bwidth = 4,
      .fsub = 0,
      .operation = ACTIV_FUNC,
    };

    /* Unit=ACTIV_ACC_V2 */
    LL_Activacc_Init(0, &Sigmoid_282_init203);


    /* Dma inputs units to cycle: */
    /* Unit= 0 [STREAM_ENG_V2 0] */
    /* Emit conf for STREAM_ENG_V2 node=Sigmoid_282 input ports=0 range=1[102400,103424] */

    static const LL_Streng_TensorInitTypeDef Sigmoid_282_dma_init_in_0_203 = {
      /* from memory with batch=1 */
      .dir = 0,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Conv2D_279_out_0 */
      .offset_start = 102400,
      .offset_end = 103424,
      .offset_limit = 103488,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 1024,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(0, &Sigmoid_282_dma_init_in_0_203, 1);


    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 1024 */

    /* Dma output units from cycle: */
    /* Unit= 6 [STREAM_ENG_V2 6] */
    /* Emit conf for STREAM_ENG_V2 node=Sigmoid_282 output ports=0 range=1[110592,111616] */

    static const LL_Streng_TensorInitTypeDef Sigmoid_282_dma_init_out_0_203 = {
      /* to memory with batch=1 */
      .dir = 1,
      .raw = 1,
      .noblk = 0,
      .align_right = 0,
      .nbits_unsigned = 0,
      .addr_base = {(unsigned char *)(0x342e0000UL) /* Equivalent hex address = 0x342e0000UL */}, /* Sigmoid_282_out_0 */
      .offset_start = 110592,
      .offset_end = 111616,
      .offset_limit = 111680,
      .frame_count = 0,
      .fwidth = 0,
      .fheight = 0,
      .batch_depth = 0,
      .batch_offset = 0,
      .frame_offset = 1024,
      .line_offset = 0,
      .loop_offset = 0,
      .frame_loop_cnt = 0,
      .frame_tot_cnt = 1,
      .nbits_in = 8,
      .nbits_out = 8,
    };

    /* Unit=STREAM_ENG_V2 */
    LL_Streng_TensorInit(6, &Sigmoid_282_dma_init_out_0_203, 1);


    /* Dma output bandwidth to memory pools: */
    /* npuRAM5 <- 1024 */

    static const LL_Switch_InitTypeDef STREAM_SWITCH_0_init_in_203[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ACTIV, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Sigmoid_282 IN: in unit=ACTIV_ACC_V2 0 in port=0 out unit=STREAM_ENG_V2 0 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 6, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ACTIV, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Sigmoid_282 OUT: in unit=STREAM_ENG_V2 6 in port=0 out unit=ACTIV_ACC_V2 0 out port=0 */
    };


    /* epoch=203 */
    LL_Switch_Init(STREAM_SWITCH_0_init_in_203, 2);

    static const LL_ATON_EnableUnits_InitTypeDef Enable_epoch_203_all_units[] = {
      { {STRENG, 6} }, /* STREAM_ENG_V2 */
      { {ACTIV, 0} }, /* ACTIV_ACC_V2 */
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_EnableUnits_Init(Enable_epoch_203_all_units, 3);

  }

  ec_trace_wait_epoch_end(0x40);

  {
    static const LL_Switch_DeinitTypeDef STREAM_SWITCH_0_deinit_in_203[] = {
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, ACTIV, 0, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, STRENG, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Sigmoid_282 IN: in unit=ACTIV_ACC_V2 0 in port=0 out unit=STREAM_ENG_V2 0 out port=0 */
      { LL_Switch_Init_Dest() = ATONN_DSTPORT(STRSWITCH, 0, STRENG, 6, 0), LL_Switch_Init_Source(0) = ATONN_SRCPORT(STRSWITCH, 0, ACTIV, 0, 0), LL_Switch_Init_Context(0) = 1, LL_Switch_Init_Frames(0) = 0, }, /* Sigmoid_282 OUT: in unit=STREAM_ENG_V2 6 in port=0 out unit=ACTIV_ACC_V2 0 out port=0 */
    };


    /* epoch=203 */
    LL_Switch_Deinit(STREAM_SWITCH_0_deinit_in_203, 2);

    static const LL_ATON_DisableUnits_InitTypeDef Disable_epoch_203_all_units[] = {
      { {STRENG, 6} }, /* STREAM_ENG_V2 */
      { {ACTIV, 0} }, /* ACTIV_ACC_V2 */
      { {STRENG, 0} }, /* STREAM_ENG_V2 */
    };


    LL_ATON_DisableUnits_Init(Disable_epoch_203_all_units, 3);

  }
  ec_trace_end_epoch(203);
  ec_trace_start_epoch(204);
  {
    /* Dma input bandwidth from memory pools: */
    /* npuRAM5 -> 0 */

  }
  {
  }
  ec_trace_end_epoch(204);
  ec_trace_end_blob("_ec_blob_centerface_oe_3_2_0_203");
}


int main () {
  ec_trace_init("centerface_oe_3_2_0_ecblobs.h", "centerface_oe_3_2_0", false, 0, false);
  trace_ec__ec_blob_centerface_oe_3_2_0_1();
  trace_ec__ec_blob_centerface_oe_3_2_0_3();
  trace_ec__ec_blob_centerface_oe_3_2_0_29();
  trace_ec__ec_blob_centerface_oe_3_2_0_50();
  trace_ec__ec_blob_centerface_oe_3_2_0_60();
  trace_ec__ec_blob_centerface_oe_3_2_0_81();
  trace_ec__ec_blob_centerface_oe_3_2_0_91();
  trace_ec__ec_blob_centerface_oe_3_2_0_101();
  trace_ec__ec_blob_centerface_oe_3_2_0_119();
  trace_ec__ec_blob_centerface_oe_3_2_0_129();
  trace_ec__ec_blob_centerface_oe_3_2_0_150();
  trace_ec__ec_blob_centerface_oe_3_2_0_160();
  trace_ec__ec_blob_centerface_oe_3_2_0_175();
  trace_ec__ec_blob_centerface_oe_3_2_0_181();
  trace_ec__ec_blob_centerface_oe_3_2_0_187();
  trace_ec__ec_blob_centerface_oe_3_2_0_203();
  ec_trace_all_blobs_done();
}
