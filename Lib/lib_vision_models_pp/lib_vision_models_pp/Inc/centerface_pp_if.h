/**
 ******************************************************************************
 * @file    centerface_pp_if.h
 * @brief   CenterFace detection post-processing interface.
 *
 *          CenterFace produces 4 output tensors from a 128x128 input:
 *            pHeatmap : (32x32x1)  face-centre heatmap
 *            pScale   : (32x32x2)  predicted face width/height (log scale)
 *            pOffset  : (32x32x2)  sub-pixel centre offset
 *            pLms     : (32x32x10) 5 facial landmark offsets (x,y pairs)
 *
 *          All coordinates in pOutput are normalized to [0, 1] relative to
 *          the 128x128 input resolution.
 *
 *          Ported from STM32N6-FaceRecognition reference application.
 *          Renamed types to avoid conflict with palm-detection pd_model_pp_if.h.
 ******************************************************************************
 */
#ifndef CENTERFACE_PP_IF_H
#define CENTERFACE_PP_IF_H

#include "arm_math.h"
#include "pd_pp_output_if.h"   /* pd_pp_box_t, pd_pp_point_t, pd_pp_out_t */

#ifdef __cplusplus
extern "C" {
#endif

/** CenterFace 4-tensor input */
typedef struct {
    float32_t *pScale;    /*!< (32x32x2) log-scale face w/h */
    float32_t *pLms;      /*!< (32x32x10) 5 landmark (x,y) offsets */
    float32_t *pHeatmap;  /*!< (32x32x1) centre heatmap */
    float32_t *pOffset;   /*!< (32x32x2) sub-pixel offset */
} centerface_pp_in_t;

/** Static parameters (set once at init) */
typedef struct {
    uint32_t  width;           /*!< Input image width  (128) */
    uint32_t  height;          /*!< Input image height (128) */
    uint32_t  nb_keypoints;    /*!< Number of facial landmarks (5) */
    float32_t conf_threshold;  /*!< Minimum heatmap score to keep a box */
    float32_t iou_threshold;   /*!< NMS IoU threshold */
    uint32_t  max_boxes_limit; /*!< Maximum number of output boxes */
} centerface_pp_static_param_t;

/**
 * @brief Reset post-processing state (no-op, provided for symmetry).
 */
int32_t centerface_pp_reset(centerface_pp_static_param_t *params);

/**
 * @brief Run CenterFace decode + NMS.
 *
 * @param pInput   Pointers to the 4 model output tensors.
 * @param pOutput  pOutData must point to a caller-allocated pd_pp_box_t array
 *                 (at least max_boxes_limit entries); each pKps must point to
 *                 a caller-allocated pd_pp_point_t array (nb_keypoints entries).
 * @param params   Static configuration.
 * @return AI_PD_POSTPROCESS_ERROR_NO (0) on success.
 */
int32_t centerface_pp_process(centerface_pp_in_t *pInput,
                              pd_pp_out_t *pOutput,
                              centerface_pp_static_param_t *params);

#ifdef __cplusplus
}
#endif

#endif /* CENTERFACE_PP_IF_H */
