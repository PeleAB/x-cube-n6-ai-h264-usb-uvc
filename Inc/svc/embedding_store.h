/**
 ******************************************************************************
 * @file    embedding_store.h
 * @brief   Face embedding bank with NOR flash persistence.
 *
 * Maintains a running accumulator of face embeddings (up to
 * EMBED_MAX_SAMPLES) in SRAM and writes a single averaged target embedding
 * to a dedicated NOR flash sector at EMBED_FLASH_BASE (0x07FFC000).
 *
 * Flash layout (one 4 KB sector):
 *   Offset 0-3:   magic    (uint32_t = 0xFACE0001)
 *   Offset 4-7:   count    (uint32_t, number of samples averaged)
 *   Offset 8-519: target   (FR_EMBEDDING_DIM × float32 = 512 bytes)
 *   Total: 520 bytes
 *
 * Thread safety:
 *   A FreeRTOS mutex protects all SRAM state.  Flash write/erase is deferred
 *   to nn_thread via face_pipeline_set_commit_pending() /
 *   face_pipeline_set_clear_pending() so that it never races with NPU
 *   model-weight reads through the AXI MMP window.
 ******************************************************************************
 */
#ifndef EMBEDDING_STORE_H
#define EMBEDDING_STORE_H

#include <stdbool.h>
#include <stdint.h>
#include "app/app_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Maximum number of raw enrollment samples kept in RAM */
#define EMBED_MAX_SAMPLES  10

/**
 * @brief Initialize the embedding store.
 *
 * Reads the NOR flash sector.  If the magic number matches, loads the stored
 * target embedding into SRAM.  On first boot / corruption, starts with an
 * empty bank.  Must be called after BSP_PlatformInit and the FreeRTOS
 * scheduler is running (creates a mutex).
 */
void embedding_store_init(void);

/**
 * @brief Add one enrollment sample to the in-SRAM accumulator.
 *
 * The sample is L2-normalised before storage.  When EMBED_MAX_SAMPLES is
 * reached, the oldest sample is discarded (ring buffer).
 *
 * @param embedding  Pointer to FR_EMBEDDING_DIM floats.
 */
void embedding_store_add(const float *embedding);

/**
 * @brief Compute averaged target from accumulated samples and persist to flash.
 *
 * Averages all samples in the accumulator, L2-normalises the result, stores
 * it as the active target, then erases + writes the flash sector.
 *
 * Must only be called from nn_thread (via face_pipeline_set_commit_pending()),
 * never directly from the UART handler.
 *
 * @return 0 on success, -1 on flash error.
 */
int embedding_store_commit(void);

/**
 * @brief Clear all in-SRAM samples and erase the flash sector.
 *
 * Must only be called from nn_thread (via face_pipeline_set_clear_pending()),
 * never directly from the UART handler.
 *
 * After this call, embedding_store_match() always returns false.
 */
void embedding_store_clear(void);

/**
 * @brief Compare a query embedding against the stored target.
 *
 * @param embedding      Pointer to FR_EMBEDDING_DIM floats (query).
 * @param out_similarity Receives the cosine similarity in [0, 1].
 * @return true  if a target is enrolled AND similarity >= FACE_SIM_THRESHOLD.
 * @return false if no target is enrolled or similarity is below threshold.
 */
bool embedding_store_match(const float *embedding, float *out_similarity);

/**
 * @brief Number of raw samples currently in the in-SRAM accumulator.
 */
uint32_t embedding_store_sample_count(void);

/**
 * @brief True if a committed target embedding is loaded (from flash or commit).
 */
bool embedding_store_has_target(void);

#ifdef __cplusplus
}
#endif

#endif /* EMBEDDING_STORE_H */
