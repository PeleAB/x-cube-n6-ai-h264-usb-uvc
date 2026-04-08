/**
 ******************************************************************************
 * @file    face_result.h
 * @brief   Shared data structure for face detection + recognition pipeline output.
 *
 *          face_pipeline_result_t is written by nn_thread (via face_pipeline_run)
 *          and read by dp_thread (via app_display_render_faces).  It travels
 *          through the existing nn_output_queue buffers (5880 bytes each),
 *          which are large enough to hold the ~260-byte struct.
 ******************************************************************************
 */
#ifndef FACE_RESULT_H
#define FACE_RESULT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum number of faces processed per frame */
#define FACE_RESULT_MAX_FACES  4

/* Keypoint indices (CenterFace outputs 5 landmarks) */
#define FACE_KP_LEFT_EYE    0
#define FACE_KP_RIGHT_EYE   1
#define FACE_KP_NOSE        2
#define FACE_KP_LEFT_MOUTH  3
#define FACE_KP_RIGHT_MOUTH 4
#define FACE_KP_COUNT       5

/**
 * @brief Per-face detection + recognition result.
 *
 * Bounding box and keypoints are in display (1280×720 or sensor-specific)
 * pixel space — already scaled from the 128×128 CenterFace output.
 */
typedef struct {
    uint16_t box_x;           /*!< Top-left X, pixels */
    uint16_t box_y;           /*!< Top-left Y, pixels */
    uint16_t box_w;           /*!< Width, pixels */
    uint16_t box_h;           /*!< Height, pixels */
    float    detection_score; /*!< CenterFace confidence [0,1] */
    uint16_t kp_x[FACE_KP_COUNT]; /*!< Keypoint X coords, pixels */
    uint16_t kp_y[FACE_KP_COUNT]; /*!< Keypoint Y coords, pixels */
    uint8_t  recognized;      /*!< 0=unknown, 1=matched, 2=just enrolled */
    uint8_t  _pad[3];
    float    similarity;      /*!< Cosine similarity vs stored target [0,1] */
} face_entry_t;               /* 64 bytes */

/**
 * @brief Full per-frame face pipeline result — placed in nn_output_queue buffer.
 */
typedef struct {
    uint8_t      num_faces;                        /*!< Number of valid entries */
    uint8_t      _pad[3];
    face_entry_t faces[FACE_RESULT_MAX_FACES];     /*!< Per-face data */
} face_pipeline_result_t;                          /* 4 + 4×64 = 260 bytes */

#ifdef __cplusplus
}
#endif

#endif /* FACE_RESULT_H */
