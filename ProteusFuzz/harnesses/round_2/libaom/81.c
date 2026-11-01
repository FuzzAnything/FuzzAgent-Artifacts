// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

/*
 * Fuzz driver for libaom encoder control functions.
 * Targets:
 * - AV1E_SET_MV_COST_UPD_FREQ
 * - AV1E_SET_DV_COST_UPD_FREQ
 * - AV1E_SET_RTC_EXTERNAL_RC
 * - AV1E_SET_TIER_MASK
 * - AOME_SET_ENABLEAUTOBWDREF
 * - AV1E_SET_COEFF_COST_UPD_FREQ
 */

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    struct aom_codec_enc_cfg cfg;
    aom_codec_err_t ret;

    if (!ctx || !data) return 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    /* Use fuzzing data to configure width/height if possible, with safety checks */
    if (size >= 8) {
        uint32_t w = *(const uint32_t *)(data);
        uint32_t h = *(const uint32_t *)(data + 4);
        if (w > 0 && w < 16384 && h > 0 && h < 16384) {
            cfg.g_w = w;
            cfg.g_h = h;
        } else {
            cfg.g_w = 64;
            cfg.g_h = 64;
        }
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }
    cfg.g_threads = 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int invoke_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t ret;
    ret = aom_codec_control(ctx, control_id, value);
    return (ret == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int success = 0;

    if (Size < 1) return 0;

    memset(&ctx, 0, sizeof(ctx));

    /* Initialize encoder with fuzz data */
    if (!init_encoder(&ctx, Data, Size)) {
        return 0;
    }

    /* Extract values from input to diversify control arguments */
    int val1 = (Size > 0) ? (int)Data[0] : 0;
    int val2 = (Size > 1) ? (int)Data[1] : 0;
    int val3 = (Size > 2) ? (int)Data[2] : 0;
    int val4 = (Size > 3) ? (int)Data[3] : 0;
    int val5 = (Size > 4) ? (int)Data[4] : 0;
    int val6 = (Size > 5) ? (int)Data[5] : 0;

    /*
     * Invoke the target control functions.
     */

    /* 1. AV1E_SET_MV_COST_UPD_FREQ */
    if (invoke_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, val1)) {
        success = 1;
    }

    /* 2. AV1E_SET_DV_COST_UPD_FREQ */
    if (invoke_control(&ctx, AV1E_SET_DV_COST_UPD_FREQ, val2)) {
        success = 1;
    }

    /* 3. AV1E_SET_RTC_EXTERNAL_RC */
    if (invoke_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, (val3 % 2))) {
        success = 1;
    }

    /* 4. AV1E_SET_TIER_MASK */
    if (invoke_control(&ctx, AV1E_SET_TIER_MASK, val4)) {
        success = 1;
    }

    /* 5. AOME_SET_ENABLEAUTOBWDREF */
    if (invoke_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, (val5 % 2))) {
        success = 1;
    }

    /* 6. AV1E_SET_COEFF_COST_UPD_FREQ */
    if (invoke_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val6)) {
        success = 1;
    }

    /* Cleanup */
    cleanup_encoder(&ctx);

    return 0;
}