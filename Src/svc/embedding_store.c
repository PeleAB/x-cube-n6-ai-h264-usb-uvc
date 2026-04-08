/**
 ******************************************************************************
 * @file    embedding_store.c
 * @brief   Face embedding bank with NOR flash persistence.
 ******************************************************************************
 */
#include "svc/embedding_store.h"

#include <math.h>
#include <string.h>

#include "stm32n6570_discovery_xspi.h"
#include "FreeRTOS.h"
#include "semphr.h"

/* ---------------------------------------------------------------------------
 * Flash sector layout
 * -------------------------------------------------------------------------- */
#define EMBED_MAGIC       0xFACE0001UL
#define EMBED_XSPI_INST   PARAM_XSPI_INST   /* same XSPI as param store */

/** Packed structure written/read as a single block to/from flash. */
typedef struct {
    uint32_t magic;                                    /*   4 bytes */
    uint32_t count;                                    /*   4 bytes */
    float    target[FR_EMBEDDING_DIM];                 /* 512 bytes */
} __attribute__((packed)) embed_flash_t;               /* 520 bytes total */

/* ---------------------------------------------------------------------------
 * SRAM state
 * -------------------------------------------------------------------------- */
static embed_flash_t  s_flash;                  /* shadow of flash content   */
static float          s_samples[EMBED_MAX_SAMPLES][FR_EMBEDDING_DIM]; /* accum */
static uint32_t       s_sample_count;           /* samples added since reset  */
static bool           s_has_target;             /* valid target in s_flash    */

static SemaphoreHandle_t  s_mutex;
static StaticSemaphore_t  s_mutex_buf;

/* ---------------------------------------------------------------------------
 * Internal helpers
 * -------------------------------------------------------------------------- */

static void l2_normalize(float *v, uint32_t len)
{
    float norm_sq = 0.0f;
    for (uint32_t i = 0; i < len; i++)
        norm_sq += v[i] * v[i];
    if (norm_sq < 1e-12f)
        return;
    float inv_norm = 1.0f / sqrtf(norm_sq);
    for (uint32_t i = 0; i < len; i++)
        v[i] *= inv_norm;
}

static float cosine_similarity(const float *a, const float *b, uint32_t len)
{
    float dot = 0.0f, na = 0.0f, nb = 0.0f;
    for (uint32_t i = 0; i < len; i++) {
        dot += a[i] * b[i];
        na  += a[i] * a[i];
        nb  += b[i] * b[i];
    }
    if (na == 0.0f || nb == 0.0f)
        return 0.0f;
    return dot / sqrtf(na * nb);
}

/* ---------------------------------------------------------------------------
 * Public API
 * -------------------------------------------------------------------------- */

void embedding_store_init(void)
{
    s_mutex = xSemaphoreCreateMutexStatic(&s_mutex_buf);

    memset(&s_flash, 0, sizeof(s_flash));
    memset(s_samples, 0, sizeof(s_samples));
    s_sample_count = 0;
    s_has_target   = false;

    /* Load from memory-mapped NOR flash (XSPI2 MMP at 0x70000000). */
    embed_flash_t flash_buf;
    const uint8_t *mmp_src = (const uint8_t *)(XSPI2_BASE + EMBED_FLASH_BASE);
    memcpy(&flash_buf, mmp_src, sizeof(embed_flash_t));
    if (flash_buf.magic == EMBED_MAGIC && flash_buf.count > 0) {
        s_flash      = flash_buf;
        s_has_target = true;
    }
}

void embedding_store_add(const float *embedding)
{
    xSemaphoreTake(s_mutex, portMAX_DELAY);

    uint32_t slot = s_sample_count % EMBED_MAX_SAMPLES;
    memcpy(s_samples[slot], embedding, FR_EMBEDDING_DIM * sizeof(float));
    l2_normalize(s_samples[slot], FR_EMBEDDING_DIM);
    s_sample_count++;

    xSemaphoreGive(s_mutex);
}

int embedding_store_commit(void)
{
    xSemaphoreTake(s_mutex, portMAX_DELAY);

    uint32_t n = (s_sample_count < EMBED_MAX_SAMPLES) ? s_sample_count : EMBED_MAX_SAMPLES;
    if (n == 0) {
        xSemaphoreGive(s_mutex);
        return -1;
    }

    /* Average all accumulated samples */
    float avg[FR_EMBEDDING_DIM];
    memset(avg, 0, sizeof(avg));
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < FR_EMBEDDING_DIM; j++)
            avg[j] += s_samples[i][j];
    }
    for (uint32_t j = 0; j < FR_EMBEDDING_DIM; j++)
        avg[j] /= (float)n;

    l2_normalize(avg, FR_EMBEDDING_DIM);

    /* Update SRAM shadow */
    s_flash.magic = EMBED_MAGIC;
    s_flash.count = n;
    memcpy(s_flash.target, avg, sizeof(avg));
    s_has_target = true;

    /* Persist to flash: erase then write */
    int rc = 0;
    if (BSP_XSPI_NOR_Erase_Block(EMBED_XSPI_INST, EMBED_FLASH_BASE,
                                  BSP_XSPI_NOR_ERASE_4K) != BSP_ERROR_NONE) {
        rc = -1;
    } else if (BSP_XSPI_NOR_Write(EMBED_XSPI_INST,
                                   (uint8_t *)&s_flash,
                                   EMBED_FLASH_BASE,
                                   sizeof(embed_flash_t)) != BSP_ERROR_NONE) {
        rc = -1;
    }

    xSemaphoreGive(s_mutex);
    return rc;
}

void embedding_store_clear(void)
{
    xSemaphoreTake(s_mutex, portMAX_DELAY);

    memset(&s_flash, 0, sizeof(s_flash));
    memset(s_samples, 0, sizeof(s_samples));
    s_sample_count = 0;
    s_has_target   = false;

    BSP_XSPI_NOR_Erase_Block(EMBED_XSPI_INST, EMBED_FLASH_BASE, BSP_XSPI_NOR_ERASE_4K);

    xSemaphoreGive(s_mutex);
}

bool embedding_store_match(const float *embedding, float *out_similarity)
{
    xSemaphoreTake(s_mutex, portMAX_DELAY);

    if (!s_has_target) {
        if (out_similarity) *out_similarity = 0.0f;
        xSemaphoreGive(s_mutex);
        return false;
    }

    /* Copy target out of the packed struct to a stack-aligned buffer.
     * Taking &s_flash.target directly gives an unaligned float* on Cortex-M55,
     * which triggers -Waddress-of-packed-member and can cause a fault. */
    float target_aligned[FR_EMBEDDING_DIM];
    memcpy(target_aligned, s_flash.target, sizeof(target_aligned));
    float sim = cosine_similarity(embedding, target_aligned, FR_EMBEDDING_DIM);
    if (out_similarity) *out_similarity = sim;

    xSemaphoreGive(s_mutex);
    return sim >= FACE_SIM_THRESHOLD;
}

uint32_t embedding_store_sample_count(void)
{
    return s_sample_count;
}

bool embedding_store_has_target(void)
{
    return s_has_target;
}
