// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_MV_COST_UPD_FREQ
 * - AV1E_SET_QM_MIN
 * - AV1E_SET_ENABLE_PALETTE
 * - AV1E_SET_ENABLE_OVERLAY
 * - AV1E_SET_MODE_COST_UPD_FREQ
 * - AV1E_SET_ENABLE_GLOBAL_MOTION
 */

static aom_codec_ctx_t *init_encoder(aom_codec_iface_t *iface, int width, int height) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = width > 0 ? width : 64;
    cfg.g_h = height > 0 ? height : 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 0;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static void fuzz_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    (void)res; /* Ignore return for fuzzing state exploration */
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    /* Extract parameters from input safely */
    int width = (Data[0] % 256) + 1;
    int height = (Data[1] % 256) + 1;
    int value = 0;

    /* Safely read value if enough data is available */
    if (Size >= 6) {
        uint32_t val32;
        memcpy(&val32, Data + 2, sizeof(val32));
        value = (int32_t)val32;
    } else if (Size >= 3) {
        value = (int32_t)Data[2];
    }

    /* Get AV1 encoder interface */
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t *ctx = init_encoder(iface, width, height);
    if (!ctx) return 0;

    /* Fuzz various control commands */
    /* AV1E_SET_MV_COST_UPD_FREQ */
    fuzz_control(ctx, AV1E_SET_MV_COST_UPD_FREQ, value);

    /* AV1E_SET_QM_MIN */
    fuzz_control(ctx, AV1E_SET_QM_MIN, value);

    /* AV1E_SET_ENABLE_PALETTE */
    fuzz_control(ctx, AV1E_SET_ENABLE_PALETTE, value);

    /* AV1E_SET_ENABLE_OVERLAY */
    fuzz_control(ctx, AV1E_SET_ENABLE_OVERLAY, value);

    /* AV1E_SET_MODE_COST_UPD_FREQ */
    fuzz_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, value);

    /* AV1E_SET_ENABLE_GLOBAL_MOTION */
    fuzz_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, value);

    cleanup_encoder(ctx);
    return 0;
}