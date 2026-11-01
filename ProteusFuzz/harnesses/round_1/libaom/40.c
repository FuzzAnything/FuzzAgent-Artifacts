// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 3) {
        return 0;
    }

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (ctx == NULL) {
        return 0;
    }

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        cleanup_codec(ctx);
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Use fuzzing data to set some configuration parameters
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzzing different control commands
    int8_t val = (int8_t)(Data[2] % 256);
    int ret = 0;

    // AV1E_SET_DELTALF_MODE
    ret = aom_codec_control(ctx, AV1E_SET_DELTALF_MODE, val);
    (void)ret;

    // AV1E_SET_ENABLE_GLOBAL_MOTION
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val);
    (void)ret;

    // AV1E_SET_ENABLE_TPL_MODEL
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TPL_MODEL, val);
    (void)ret;

    // AV1E_SET_CDF_UPDATE_MODE
    ret = aom_codec_control(ctx, AV1E_SET_CDF_UPDATE_MODE, val);
    (void)ret;

    // AOME_SET_CQ_LEVEL
    ret = aom_codec_control(ctx, AOME_SET_CQ_LEVEL, (int32_t)val);
    (void)ret;

    // AV1E_SET_MODE_COST_UPD_FREQ
    ret = aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, (int32_t)val);
    (void)ret;

    cleanup_codec(ctx);
    return 0;
}