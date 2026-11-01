// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_SHARPNESS__value at vp8cx.h:990:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>

static int fuzz_vp8_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_iface = vpx_codec_vp8_cx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret = vpx_codec_enc_config_default(codec_iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    ret = vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    // Attempt to set screen content mode if data is sufficient
    if (size >= 2) {
        int mode = (data[0] % 2);
        vpx_codec_control(&ctx, VP8E_SET_SCREEN_CONTENT_MODE, mode);
    }

    // Attempt to set ARNR strength
    if (size >= 3) {
        int strength = (data[1] % 10) + 1;
        vpx_codec_control(&ctx, VP8E_SET_ARNR_STRENGTH, strength);
    }

    // Attempt to set sharpness
    if (size >= 4) {
        int sharpness = (data[2] % 8);
        vpx_codec_control(&ctx, VP8E_SET_SHARPNESS, sharpness);
    }

    // Encode a dummy frame
    vpx_img_fmt_t img_fmt = VPX_IMG_FMT_I420;
    vpx_image_t img;
    vpx_img_alloc(&img, img_fmt, cfg.g_w, cfg.g_h, 1);

    // Fill image with fuzz data if available
    if (size > 0) {
        // Use stride[0] for the Y plane stride
        int y_stride = img.stride[0];
        size_t copy_size = (size < (size_t)y_stride) ? size : (size_t)y_stride;
        memcpy(img.planes[0], data, copy_size);
    }

    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);
    return 0;
}

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret = vpx_codec_enc_config_default(codec_iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    ret = vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    // Attempt to set disable loop filter
    if (size >= 1) {
        int disable = (data[0] % 2);
        vpx_codec_control(&ctx, VP9E_SET_DISABLE_LOOPFILTER, disable);
    }

    // Attempt to set frame parallel decoding
    if (size >= 2) {
        int parallel = (data[1] % 2);
        vpx_codec_control(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, parallel);
    }

    // Attempt to set noise sensitivity
    if (size >= 3) {
        int sensitivity = (data[2] % 100);
        vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, sensitivity);
    }

    // Encode a dummy frame
    vpx_img_fmt_t img_fmt = VPX_IMG_FMT_I420;
    vpx_image_t img;
    vpx_img_alloc(&img, img_fmt, cfg.g_w, cfg.g_h, 1);

    if (size > 0) {
        // Use stride[0] for the Y plane stride
        int y_stride = img.stride[0];
        size_t copy_size = (size < (size_t)y_stride) ? size : (size_t)y_stride;
        memcpy(img.planes[0], data, copy_size);
    }

    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);
    return 0;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz VP8 encoder
    fuzz_vp8_encoder(Data, Size);

    // Fuzz VP9 encoder
    fuzz_vp9_encoder(Data, Size);

    return 0;
}