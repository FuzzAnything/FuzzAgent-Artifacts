// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static aom_codec_ctx_t *create_codec_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    const aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    unsigned int width, height;

    if (size < 4) return NULL;

    width = data[0] | (data[1] << 8);
    height = data[2] | (data[3] << 8);

    if (width == 0 || height == 0 || width > 8192 || height > 8192) {
        return NULL;
    }

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

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_codec_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = create_codec_context(Data, Size);
    if (!ctx) return 0;

    int value;
    aom_codec_err_t res;

    // Fuzz AV1E_SET_ENABLE_FILTER_INTRA
    if (Size > 0) {
        value = Data[0] % 2;
        res = aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, value);
        (void)res;
    }

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    if (Size > 1) {
        value = Data[1] % 2;
        res = aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, value);
        (void)res;
    }

    // Fuzz AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC
    if (Size > 2) {
        int out_val = 0;
        res = aom_codec_control(ctx, AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC, &out_val);
        (void)res;
    }

    // Fuzz AV1E_SET_FRAME_PERIODIC_BOOST
    if (Size > 3) {
        value = Data[3] % 100;
        res = aom_codec_control(ctx, AV1E_SET_FRAME_PERIODIC_BOOST, value);
        (void)res;
    }

    // Fuzz AV1E_SET_ENABLE_PALETTE
    if (Size > 4) {
        value = Data[4] % 2;
        res = aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, value);
        (void)res;
    }

    // Fuzz AV1E_SET_QM_MAX
    if (Size > 5) {
        value = Data[5] % 64;
        res = aom_codec_control(ctx, AV1E_SET_QM_MAX, value);
        (void)res;
    }

    cleanup_codec_context(ctx);
    return 0;
}