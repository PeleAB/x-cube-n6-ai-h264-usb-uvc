/**
 ******************************************************************************
 * @file    crop_img.h
 * @brief   Image crop, resize, alignment and format conversion utilities
 *          used by the face recognition pipeline.
 *
 *          Ported from STM32N6-FaceRecognition reference application.
 ******************************************************************************
 */
#ifndef CROP_IMG_H
#define CROP_IMG_H

#include "arm_math.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Convert HWC RGB888 image to CHW float32 (no normalization).
 */
void img_rgb_to_chw_float(uint8_t *src_image, float32_t *dst_img,
                          uint32_t src_stride, uint16_t width,
                          uint16_t height);

/**
 * @brief Convert HWC RGB888 image to CHW float32 with normalization:
 *        out = (pixel / 127.5) - 1.0  (range [-1, 1]).
 *        Used to prepare MobileFaceNet input.
 */
void img_rgb_to_chw_float_norm(uint8_t *src_image, float32_t *dst_img,
                               uint32_t src_stride, uint16_t width,
                               uint16_t height);

/**
 * @brief Crop and resize a region from src to dst (nearest-neighbour, any bpp).
 */
void img_crop_resize(uint8_t *src_image, uint8_t *dst_img,
                     uint16_t src_width, uint16_t src_height,
                     uint16_t dst_width, uint16_t dst_height,
                     uint16_t bpp, int x0, int y0,
                     int crop_width, int crop_height);

/**
 * @brief Crop and align a face from an HWC source image using eye keypoints
 *        for rotation correction.  Any bpp (use 3 for RGB888, 4 for ARGB8888).
 *
 * @param x_center, y_center  Face centre in source pixel coords
 * @param width, height       Face bounding box size in source pixels
 * @param left_eye_x/y, right_eye_x/y  Eye keypoints in source pixel coords
 */
void img_crop_align(uint8_t *src_image, uint8_t *dst_img,
                    uint16_t src_width, uint16_t src_height,
                    uint16_t dst_width, uint16_t dst_height,
                    uint16_t bpp, float x_center, float y_center,
                    float width, float height,
                    float left_eye_x, float left_eye_y,
                    float right_eye_x, float right_eye_y);

/**
 * @brief Crop and align a face from an ARGB8888 source (PIPE1 display buffer)
 *        to an RGB888 destination, with eye-keypoint rotation correction.
 *
 *        ARGB8888 memory layout (little-endian): B G R A per pixel.
 *        Output is R G B (3 bytes/pixel) for model input.
 */
void img_crop_align_argb_to_rgb888(const uint8_t *src_image,
                                   uint8_t *dst_img,
                                   uint16_t src_width, uint16_t src_height,
                                   uint16_t dst_width, uint16_t dst_height,
                                   float x_center, float y_center,
                                   float width, float height,
                                   float left_eye_x, float left_eye_y,
                                   float right_eye_x, float right_eye_y);

/**
 * @brief Crop and align a face from an RGB565 source (display buffer) to an
 *        RGB888 destination, with eye-keypoint rotation correction.
 *
 * @param src_stride  Source row stride in pixels (usually src_width for packed)
 */
void img_crop_align565_to_888(uint8_t *src_image, uint16_t src_stride,
                              uint8_t *dst_img,
                              uint16_t src_width, uint16_t src_height,
                              uint16_t dst_width, uint16_t dst_height,
                              float x_center, float y_center,
                              float width, float height,
                              float left_eye_x, float left_eye_y,
                              float right_eye_x, float right_eye_y);

#ifdef __cplusplus
}
#endif

#endif /* CROP_IMG_H */
