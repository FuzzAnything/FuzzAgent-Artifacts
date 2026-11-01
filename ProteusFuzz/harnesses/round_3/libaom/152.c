// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_S_FRAME_MODE
 * - AV1E_SET_ENABLE_INTERINTER_WEDGE
 * - AV1E_SET_ENABLE_REF_FRAME_MVS
 * - AV1E_SET_TARGET_SEQ_LEVEL_IDX
 * - AV1E_SET_ENABLE_FILTER_INTRA
 * - AV1E_SET_ALLOW_WARPED_MOTION
 */

static int try_init_encoder(aom_codec_ctx_t *ctx, const aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static int run_control_tests(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int32_t val;

    /* Helper to safely read int32_t from fuzz input */
    #define READ_INT32(idx) \
        ((idx + 4 <= size) ? \
         ((int32_t)(data[idx] | (data[idx+1] << 8) | (data[idx+2] << 16) | (data[idx+3] << 24))) : \
         0)

    /* AV1E_SET_S_FRAME_MODE */
    if (size >= 4) {
        val = READ_INT32(0);
        if (aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, val) != AOM_CODEC_OK) {
            ret = -1;
        }
    }

    /* AV1E_SET_ENABLE_INTERINTER_WEDGE */
    if (size >= 8) {
        val = READ_INT32(4);
        if (aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val) != AOM_CODEC_OK) {
            ret = -1;
        }
    }

    /* AV1E_SET_ENABLE_REF_FRAME_MVS */
    if (size >= 12) {
        val = READ_INT32(8);
        if (aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val) != AOM_CODEC_OK) {
            ret = -1;
        }
    }

    /* AV1E_SET_TARGET_SEQ_LEVEL_IDX */
    if (size >= 16) {
        val = READ_INT32(12);
        if (aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val) != AOM_CODEC_OK) {
            ret = -1;
        }
    }

    /* AV1E_SET_ENABLE_FILTER_INTRA */
    if (size >= 20) {
        val = READ_INT32(16);
        if (aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, val) != AOM_CODEC_OK) {
            ret = -1;
        }
    }

    /* AV1E_SET_ALLOW_WARPED_MOTION */
    if (size >= 24) {
        val = READ_INT32(20);
        if (aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, val) != AOM_CODEC_OK) {
            ret = -1;
        }
    }

    /* Try with some fixed values to explore more states */
    int fixed_vals[] = {0, 1, 2, -1, 100, 255, 1000};
    for (int i = 0; i < 7; i++) {
        (void)aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, fixed_vals[i]);
        (void)aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, fixed_vals[i]);
        (void)aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, fixed_vals[i]);
        (void)aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, fixed_vals[i]);
        (void)aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, fixed_vals[i]);
        (void)aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, fixed_vals[i]);
    }

    return ret;
}

int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    /* Initialize default config */
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Use fuzz data to tweak some config fields safely */
    if (Size > 0) cfg.g_w = (Data[0] % 100) + 16;
    if (Size > 1) cfg.g_h = (Data[1] % 100) + 16;
    if (Size > 2) cfg.g_timebase.num = (Data[2] % 100) + 1;
    if (Size > 3) cfg.g_timebase.den = (Data[3] % 100) + 1;
    if (Size > 4) cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    if (Size > 5) cfg.g_threads = (Data[5] % 8) + 1;
    if (Size > 6) cfg.g_error_resilient = (Data[6] % 2);
    if (Size > 7) cfg.g_lag_in_frames = (Data[7] % 20);

    if (try_init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    /* Run control tests */
    (void)run_control_tests(&ctx, Data, Size);

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}