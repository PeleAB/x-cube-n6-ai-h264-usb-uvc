/**
 ******************************************************************************
 * @file    face_pipeline.h
 * @brief   Two-model sequential face detection + recognition pipeline.
 *
 *          Owns CenterFace and MobileFaceNet inference directly via LL_ATON.
 *          Called from nn_thread when PIPELINE_MODE_FACE_RECOGNITION is active.
 ******************************************************************************
 */
#ifndef FACE_PIPELINE_H
#define FACE_PIPELINE_H

#include <stdint.h>
#include "app/face_result.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief One-time initialisation.
 *
 * Queries buffer addresses from the CenterFace and MobileFaceNet models and
 * initialises the CenterFace post-processing parameters.  Must be called after
 * the LL_ATON runtime is ready (i.e. after nn_service_init / LL_ATON_RT_RuntimeInit).
 */
void face_pipeline_init(void);

/**
 * @brief Run detection + recognition for one frame.
 *
 * @param nn_input_128x128  128×128 RGB888 HWC buffer from DCMIPP PIPE2.
 * @param display_argb8888  1280×720 ARGB8888 buffer from DCMIPP PIPE1 (read-only).
 * @param disp_width        Display buffer width in pixels.
 * @param disp_height       Display buffer height in pixels.
 * @param out               Result written here; num_faces=0 if nothing detected.
 */
void face_pipeline_run(const uint8_t *nn_input_128x128,
                       const uint8_t *display_argb8888,
                       uint16_t disp_width, uint16_t disp_height,
                       face_pipeline_result_t *out);

/**
 * @brief Request enrollment of the next detected face.
 *
 * Thread-safe (atomic flag).  On the next call to face_pipeline_run() that
 * detects at least one face, embedding_store_add() will be called for the
 * highest-confidence face and recognized will be set to 2.
 */
void face_pipeline_set_enroll_pending(void);

/**
 * @brief Request embedding_store_commit() on the next face_pipeline_run() call.
 *
 * Commit is deferred so it runs on nn_thread (not UART handler) where the NPU
 * is guaranteed idle and MMP access to flash is safe.
 */
void face_pipeline_set_commit_pending(void);

/**
 * @brief Request embedding_store_clear() on the next face_pipeline_run() call.
 *
 * Deferred for the same reason as commit: the NOR flash erase must not race
 * with NPU model-weight reads via MMP.  If both clear and commit are pending,
 * clear wins and the commit is cancelled.
 */
void face_pipeline_set_clear_pending(void);

#ifdef __cplusplus
}
#endif

#endif /* FACE_PIPELINE_H */
