// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_ENABLE_FLIP_IDTX
 * - AV1E_SET_INTER_DCT_ONLY
 * - AV1E_SET_NOISE_SENSITIVITY
 * - AV1E_SET_REDUCED_TX_TYPE_SET
 * - AV1E_SET_S_FRAME_MODE
 * - AV1E_SET_ENABLE_DIST_WTD_COMP
 */

static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Use fuzzing data to set some config values */
    if (size > 0) cfg.g_w = (data[0] % 32) * 16 + 16;
    if (size > 1) cfg.g_h = (data[1] % 32) * 16 + 16;
    if (size > 2) cfg.g_threads = data[2] % 4 + 1;
    if (size > 3) cfg.g_timebase.num = data[3] % 100 + 1;
    if (size > 4) cfg.g_timebase.den = (data[4] % 1000) + 1;
    if (size > 5) cfg.rc_target_bitrate = (data[5] % 100) * 1000 + 1000;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_231(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    /*
     * aom_codec_control returns aom_codec_err_t and takes a variable argument.
     * We call it directly without a function pointer to avoid type mismatch.
     */

    /* 1. AV1E_SET_ENABLE_FLIP_IDTX (likely int: 0 or 1) */
    int val_flip_idtx = (Size > 0) ? (Data[0] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, val_flip_idtx);

    /* 2. AV1E_SET_INTER_DCT_ONLY (likely int: 0 or 1) */
    int val_inter_dct = (Size > 1) ? (Data[1] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val_inter_dct);

    /* 3. AV1E_SET_NOISE_SENSITIVITY (likely int: 0-63) */
    int val_noise = (Size > 2) ? (Data[2] % 64) : 0;
    aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, val_noise);

    /* 4. AV1E_SET_REDUCED_TX_TYPE_SET (likely int: 0 or 1) */
    int val_reduced_tx = (Size > 3) ? (Data[3] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val_reduced_tx);

    /* 5. AV1E_SET_S_FRAME_MODE (likely int: 0-2) */
    int val_s_frame = (Size > 4) ? (Data[4] % 3) : 0;
    aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, val_s_frame);

    /* 6. AV1E_SET_ENABLE_DIST_WTD_COMP (likely int: 0 or 1) */
    int val_dist_wtd = (Size > 5) ? (Data[5] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val_dist_wtd);

    /*
     * To explore more states, we can also try invalid values or large values
     * by using different bytes from the input.
     */
    if (Size > 6) {
        int val_large = Data[6] * 256 + (Size > 7 ? Data[7] : 0);
        aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, val_large);
    }

    if (Size > 8) {
        int val_neg = (int8_t)Data[8];
        aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, val_neg);
    }

    cleanup_encoder_context(ctx);
    return 0;
}