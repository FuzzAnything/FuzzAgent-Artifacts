// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_external_partition.h"

/*
 * Helper function to initialize an encoder context.
 * Returns NULL on failure.
 */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to tweak config slightly, but keep it valid
    cfg.g_w = (size > 0) ? (data[0] % 128 + 16) : 128;
    cfg.g_h = (size > 1) ? (data[1] % 128 + 16) : 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Helper function to initialize a decoder context.
 * Returns NULL on failure.
 */
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t err;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    err = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Helper to cleanup encoder context
 */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

/*
 * Helper to cleanup decoder context
 */
static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use first byte to decide encoder vs decoder
    int use_encoder = (Data[0] & 1);

    if (use_encoder) {
        aom_codec_ctx_t *ctx = init_encoder(Data, Size);
        if (!ctx) return 0;

        // Try AOME_SET_CQ_LEVEL
        int cq_level = (Size > 1) ? (Data[1] % 64) : 0;
        aom_codec_control(ctx, AOME_SET_CQ_LEVEL, cq_level);

        // Try AOME_GET_LAST_QUANTIZER
        int last_quant;
        aom_codec_control(ctx, AOME_GET_LAST_QUANTIZER, &last_quant);

        // Try AOME_GET_LAST_QUANTIZER_64
        int64_t last_quant_64;
        aom_codec_control(ctx, AOME_GET_LAST_QUANTIZER_64, &last_quant_64);

        // Try AV1E_SET_QUANTIZER_ONE_PASS
        int quantizer = (Size > 2) ? (Data[2] % 64) : 0;
        aom_codec_control(ctx, AV1E_SET_QUANTIZER_ONE_PASS, quantizer);

        cleanup_encoder(ctx);
    } else {
        aom_codec_ctx_t *ctx = init_decoder(Data, Size);
        if (!ctx) return 0;

        // Try AOMD_GET_LAST_QUANTIZER
        int last_quant;
        aom_codec_control(ctx, AOMD_GET_LAST_QUANTIZER, &last_quant);

        // Try AOMD_GET_BASE_Q_IDX
        int base_q_idx;
        aom_codec_control(ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);

        cleanup_decoder(ctx);
    }

    return 0;
}