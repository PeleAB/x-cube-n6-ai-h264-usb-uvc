/**
 ******************************************************************************
 * @file    centerface_pp.c
 * @brief   CenterFace detection post-processing.
 *
 *          Ported from STM32N6-FaceRecognition reference (pd_pp_model.c).
 *          Renamed types to avoid conflict with the palm-detection
 *          pd_model_pp_if.h already present in this library.
 ******************************************************************************
 */
#include "centerface_pp_if.h"

/* Do NOT include vision_models_pp.h here: it redeclares qsort with an
 * int32_t comparator, which conflicts with stdlib.h's int comparator on
 * GCC 13 for ARM (int32_t = long int ≠ int → hard error). */
#include <stdlib.h>  /* qsort */
#include <math.h>    /* expf */

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/* CenterFace uses stride-4 grid: 128/4 = 32 */
#define CF_GRID  32

/* -------------------------------------------------------------------------
 * Internal helpers
 * ---------------------------------------------------------------------- */

static int pp_nms_comparator(const void *a, const void *b)
{
    const pd_pp_box_t *ba = (const pd_pp_box_t *)a;
    const pd_pp_box_t *bb = (const pd_pp_box_t *)b;
    if (ba->prob < bb->prob) return  1;
    if (ba->prob > bb->prob) return -1;
    return 0;
}

static float iou(const pd_pp_box_t *a, const pd_pp_box_t *b)
{
    float ax0 = a->x_center - a->width  * 0.5f;
    float ay0 = a->y_center - a->height * 0.5f;
    float ax1 = a->x_center + a->width  * 0.5f;
    float ay1 = a->y_center + a->height * 0.5f;

    float bx0 = b->x_center - b->width  * 0.5f;
    float by0 = b->y_center - b->height * 0.5f;
    float bx1 = b->x_center + b->width  * 0.5f;
    float by1 = b->y_center + b->height * 0.5f;

    float ix = MAX(MIN(ax1, bx1) - MAX(ax0, bx0), 0.0f);
    float iy = MAX(MIN(ay1, by1) - MAX(ay0, by0), 0.0f);
    float inter = ix * iy;
    float area_a = (ax1 - ax0) * (ay1 - ay0);
    float area_b = (bx1 - bx0) * (by1 - by0);
    float uni = area_a + area_b - inter;
    return (uni <= 0.0f) ? 0.0f : inter / uni;
}

/* Decode heatmap/scale/offset/landmarks into pd_pp_box_t entries */
static uint32_t cf_decode(const centerface_pp_in_t *pInput,
                          pd_pp_out_t *pOutput,
                          const centerface_pp_static_param_t *params)
{
    float W  = (float)params->width;
    float H  = (float)params->height;
    uint32_t nb_kps = params->nb_keypoints;
    uint32_t box_nb = 0;
    pd_pp_box_t *boxes = pOutput->pOutData;

    for (int y = 0; y < CF_GRID; y++) {
        for (int x = 0; x < CF_GRID; x++) {
            int idx = y * CF_GRID + x;
            float score = pInput->pHeatmap[idx];
            if (score <= params->conf_threshold)
                continue;

            /* Face size: exp(scale) * 4 (stride) */
            float s_h = expf(pInput->pScale[idx * 2 + 0]) * 4.0f;
            float s_w = expf(pInput->pScale[idx * 2 + 1]) * 4.0f;

            /* Sub-pixel offset */
            float o_y = pInput->pOffset[idx * 2 + 0];
            float o_x = pInput->pOffset[idx * 2 + 1];

            /* Top-left in pixel space */
            float x1 = ((float)x + o_x + 0.5f) * 4.0f - s_w * 0.5f;
            float y1 = ((float)y + o_y + 0.5f) * 4.0f - s_h * 0.5f;
            if (x1 < 0.0f) x1 = 0.0f;
            if (y1 < 0.0f) y1 = 0.0f;

            pd_pp_box_t *box = &boxes[box_nb];
            box->prob     = score;
            box->x_center = (x1 + s_w * 0.5f) / W;
            box->y_center = (y1 + s_h * 0.5f) / H;
            box->width    = s_w / W;
            box->height   = s_h / H;

            /* Facial landmarks */
            if (box->pKps) {
                for (uint32_t k = 0; k < nb_kps; k++) {
                    float lm_y = pInput->pLms[idx * nb_kps * 2 + k * 2 + 0];
                    float lm_x = pInput->pLms[idx * nb_kps * 2 + k * 2 + 1];
                    box->pKps[k].x = (lm_x * s_w + x1) / W;
                    box->pKps[k].y = (lm_y * s_h + y1) / H;
                }
            }

            box_nb++;
            if (box_nb >= params->max_boxes_limit) {
                pOutput->box_nb = box_nb;
                return box_nb;
            }
        }
    }

    pOutput->box_nb = box_nb;
    return box_nb;
}

/* NMS: filter overlapping boxes */
static void cf_nms(pd_pp_out_t *pOutput, const centerface_pp_static_param_t *params)
{
    pd_pp_box_t *boxes = pOutput->pOutData;
    uint32_t n = pOutput->box_nb;
    uint32_t kept = 0;

    qsort(boxes, n, sizeof(pd_pp_box_t), pp_nms_comparator);

    for (uint32_t i = 0; i < n; i++) {
        int suppress = 0;
        for (uint32_t j = 0; j < kept; j++) {
            if (iou(&boxes[i], &boxes[j]) >= params->iou_threshold) {
                suppress = 1;
                break;
            }
        }
        if (!suppress)
            boxes[kept++] = boxes[i];
    }

    pOutput->box_nb = kept;
}

/* -------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------- */

int32_t centerface_pp_reset(centerface_pp_static_param_t *params)
{
    (void)params;
    return AI_PD_POSTPROCESS_ERROR_NO;
}

int32_t centerface_pp_process(centerface_pp_in_t *pInput,
                              pd_pp_out_t *pOutput,
                              centerface_pp_static_param_t *params)
{
    cf_decode(pInput, pOutput, params);
    cf_nms(pOutput, params);
    return AI_PD_POSTPROCESS_ERROR_NO;
}
