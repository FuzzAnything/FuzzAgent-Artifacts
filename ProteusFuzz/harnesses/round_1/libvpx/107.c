// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// VP9D_GET_BIT_DEPTH__value at vp8dx.h:204:1 in vp8dx.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_ENABLE_MOTION_VECTOR_UNIT_TEST__value at vp8cx.h:1074:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_SHARPNESS__value at vp8cx.h:990:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>

// Define constants if not present in headers or to ensure compatibility
#ifndef VP8E_SET_SHARPNESS
#define VP8E_SET_SHARPNESS 0
#endif
#ifndef VP8E_SET_SCREEN_CONTENT_MODE
#define VP8E_SET_SCREEN_CONTENT_MODE 0
#endif
#ifndef VP8E_SET_MAX_INTRA_BITRATE_PCT
#define VP8E_SET_MAX_INTRA_BITRATE_PCT 0
#endif
#ifndef VP8E_SET_ARNR_STRENGTH
#define VP8E_SET_ARNR_STRENGTH 0
#endif
#ifndef VP9D_GET_BIT_DEPTH
#define VP9D_GET_BIT_DEPTH 0
#endif
#ifndef VP9E_ENABLE_MOTION_VECTOR_UNIT_TEST
#define VP9E_ENABLE_MOTION_VECTOR_UNIT_TEST 0
#endif

// Define VPX_EFLAG_NONE if missing (usually 0)
#ifndef VPX_EFLAG_NONE
#define VPX_EFLAG_NONE 0
#endif

static int fuzz_vp8_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t codec;
    vpx_codec_iface_t *iface;
    vpx_image_t img;
    vpx_codec_enc_cfg_t cfg;
    int ret;

    iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (vpx_codec_enc_init(&codec, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, 16, 16, 1) != 0) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    // Initialize image data with fuzz input
    if (size > 0) {
        size_t copy_size = (size > 16 * 16) ? 16 * 16 : size;
        memcpy(img.planes[0], data, copy_size);
        memset(img.planes[1], 0, 8 * 8);
        memset(img.planes[2], 0, 8 * 8);
    }

    // Fuzz VP8E_SET_SHARPNESS
    if (size > 0) {
        int sharpness = data[0] % 7; // Range 0-6
        vpx_codec_control(&codec, VP8E_SET_SHARPNESS, sharpness);
    }

    // Fuzz VP8E_SET_SCREEN_CONTENT_MODE
    if (size > 1) {
        int sc_mode = (data[1] % 2) ? 1 : 0;
        vpx_codec_control(&codec, VP8E_SET_SCREEN_CONTENT_MODE, sc_mode);
    }

    // Fuzz VP8E_SET_MAX_INTRA_BITRATE_PCT
    if (size > 2) {
        int pct = data[2] % 101; // Range 0-100
        vpx_codec_control(&codec, VP8E_SET_MAX_INTRA_BITRATE_PCT, pct);
    }

    // Fuzz VP8E_SET_ARNR_STRENGTH
    if (size > 3) {
        int arnr = data[3] % 5; // Range 0-4
        vpx_codec_control(&codec, VP8E_SET_ARNR_STRENGTH, arnr);
    }

    vpx_codec_encode(&codec, &img, 0, 1, 0, VPX_EFLAG_NONE);

    vpx_img_free(&img);
    vpx_codec_destroy(&codec);

    return 0;
}

static int fuzz_vp9_decoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t codec;
    vpx_codec_iface_t *iface;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_err_t ret;

    iface = vpx_codec_vp9_dx();
    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));

    if (vpx_codec_dec_init(&codec, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz VP9D_GET_BIT_DEPTH
    int bit_depth = 8;
    ret = vpx_codec_control(&codec, VP9D_GET_BIT_DEPTH, &bit_depth);
    (void)ret; // Use the result implicitly

    // Attempt to decode a frame if data is provided
    if (size > 0) {
        vpx_codec_decode(&codec, data, size, NULL, 0);
    }

    vpx_codec_destroy(&codec);
    return 0;
}

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t codec;
    vpx_codec_iface_t *iface;
    vpx_image_t img;
    vpx_codec_enc_cfg_t cfg;
    int ret;

    iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (vpx_codec_enc_init(&codec, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, 16, 16, 1) != 0) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    if (size > 0) {
        size_t copy_size = (size > 16 * 16) ? 16 * 16 : size;
        memcpy(img.planes[0], data, copy_size);
        memset(img.planes[1], 0, 8 * 8);
        memset(img.planes[2], 0, 8 * 8);
    }

    // Fuzz VP9E_ENABLE_MOTION_VECTOR_UNIT_TEST
    if (size > 0) {
        int enable = data[0] % 2;
        vpx_codec_control(&codec, VP9E_ENABLE_MOTION_VECTOR_UNIT_TEST, enable);
    }

    vpx_codec_encode(&codec, &img, 0, 1, 0, VPX_EFLAG_NONE);

    vpx_img_free(&img);
    vpx_codec_destroy(&codec);

    return 0;
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Rotate through different fuzzing modes based on the first byte
    int mode = Data[0] % 3;

    switch (mode) {
        case 0:
            fuzz_vp8_encoder(Data, Size);
            break;
        case 1:
            fuzz_vp9_decoder(Data, Size);
            break;
        case 2:
            fuzz_vp9_encoder(Data, Size);
            break;
    }

    return 0;
}