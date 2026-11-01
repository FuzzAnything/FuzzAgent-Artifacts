// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t **ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_ctx_t *c = NULL;
    aom_codec_err_t res;

    if (!ctx) return 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    c = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!c) return 0;
    memset(c, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(c);
        return 0;
    }

    // Use fuzz input to configure width/height within reasonable bounds
    if (size >= 2) {
        uint16_t w = data[0] & 0xFF;
        uint16_t h = data[1] & 0xFF;
        if (w < 2) w = 2;
        if (h < 2) h = 2;
        cfg.g_w = w;
        cfg.g_h = h;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;

    res = aom_codec_enc_init(c, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(c);
        return 0;
    }

    *ctx = c;
    return 1;
}

int LLVMFuzzerTestOneInput_255(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_err_t res;

    if (Size < 1) return 0;

    if (!init_encoder(&ctx, Data, Size)) {
        return 0;
    }

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
    if (Size >= 2) {
        int32_t val = (int32_t)(Data[0] | (Data[1] << 8));
        res = aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, val);
        (void)res;
    }

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    if (Size >= 3) {
        int32_t val = (int32_t)(Data[2] | (Data[3] << 8));
        res = aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, val);
        (void)res;
    }

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
    if (Size >= 4) {
        int32_t val = (int32_t)(Data[4] | (Data[5] << 8));
        res = aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, val);
        (void)res;
    }

    // Fuzz AV1E_SET_GF_CBR_BOOST_PCT
    if (Size >= 6) {
        int32_t val = (int32_t)(Data[6] | (Data[7] << 8));
        res = aom_codec_control(ctx, AV1E_SET_GF_CBR_BOOST_PCT, val);
        (void)res;
    }

    // Fuzz AV1E_ENABLE_RATE_GUIDE_DELTAQ
    if (Size >= 8) {
        int32_t val = Data[8] ? 1 : 0;
        res = aom_codec_control(ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, val);
        (void)res;
    }

    // Fuzz AV1E_SET_SVC_FRAME_DROP_MODE
    if (Size >= 9) {
        int32_t val = Data[9] % 3; // Assume valid range 0-2
        res = aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val);
        (void)res;
    }

    // Try to encode a dummy frame to ensure state is exercised
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
    memset(img.planes[0], 0, 64 * 64);
    memset(img.planes[1], 128, 32 * 32);
    memset(img.planes[2], 128, 32 * 32);
    // aom_img_alloc sets strides correctly, so no need to manually set plane_stride

    res = aom_codec_encode(ctx, &img, 0, 1, 0);
    (void)res;

    aom_img_free(&img);
    cleanup_codec(ctx);

    return 0;
}