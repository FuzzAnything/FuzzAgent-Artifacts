// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

/*
 * Helper function to initialize the encoder context.
 * Returns a valid aom_codec_ctx_t pointer on success, NULL on failure.
 */
static aom_codec_ctx_t *init_encoder_context(void) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Set minimal configuration to allow initialization
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Cleanup function to destroy the encoder context.
 */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

/*
 * Fuzzing entry point.
 */
int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context();
    if (!ctx) return 0;

    // Extract parameters from input data
    uint32_t param1 = *(const uint32_t *)Data;
    uint32_t param2 = *(const uint32_t *)(Data + 4);
    uint32_t param3 = *(const uint32_t *)(Data + 8);
    uint32_t param4 = *(const uint32_t *)(Data + 12);
    uint32_t param5 = *(const uint32_t *)(Data + 16);
    uint32_t param6 = *(const uint32_t *)(Data + 20);

    // Fuzz AV1E_SET_ENABLE_CDEF (0/1 expected, but fuzz with any value)
    aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, param1);

    // Fuzz AV1E_SET_ENABLE_INTRABC (0/1 expected)
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, param2);

    // Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING (0/1 expected)
    aom_codec_control(ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, param3);

    // Fuzz AV1E_SET_TILE_ROWS (unsigned int, valid range 0-7)
    aom_codec_control(ctx, AV1E_SET_TILE_ROWS, param4);

    // Fuzz AV1E_SET_ENABLE_KEYFRAME_FILTERING (0/1 expected)
    aom_codec_control(ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, param5);

    // Fuzz AV1E_SET_QM_U (int, valid range -128 to 127)
    int qm_u = (int)(param6 & 0xFF) - 128;
    aom_codec_control(ctx, AV1E_SET_QM_U, qm_u);

    // Cleanup
    cleanup_encoder_context(ctx);

    return 0;
}