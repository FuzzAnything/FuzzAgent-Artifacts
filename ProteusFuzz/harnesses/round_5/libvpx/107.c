// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_MAX_INTER_BITRATE_PCT__value at vp8cx.h:1014:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

static int fuzz_vp8_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_iface = vpx_codec_vp8_cx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_img_fmt_t img_fmt = VPX_IMG_FMT_I420;

    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    vpx_image_t img;
    vpx_img_alloc(&img, img_fmt, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 0, img.stride[0] * cfg.g_h);
    memset(img.planes[1], 128, img.stride[1] * (cfg.g_h / 2));
    memset(img.planes[2], 128, img.stride[2] * (cfg.g_h / 2));

    int ret = 0;

    // Fuzz VP8E_SET_SCREEN_CONTENT_MODE
    if (size > 0) {
        int mode = data[0] % 2;
        if (vpx_codec_control(&ctx, VP8E_SET_SCREEN_CONTENT_MODE, mode) != VPX_CODEC_OK) {
            ret = 1;
        }
    }

    // Fuzz VP8E_SET_ARNR_STRENGTH
    if (size > 1) {
        int strength = data[1] % 8;
        if (vpx_codec_control(&ctx, VP8E_SET_ARNR_STRENGTH, strength) != VPX_CODEC_OK) {
            ret = 1;
        }
    }

    // Fuzz VP8E_SET_MAX_INTRA_BITRATE_PCT
    if (size > 2) {
        int pct = data[2] % 100;
        if (vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, pct) != VPX_CODEC_OK) {
            ret = 1;
        }
    }

    // Fuzz VP8E_SET_NOISE_SENSITIVITY
    if (size > 3) {
        int noise = data[3] % 16;
        if (vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, noise) != VPX_CODEC_OK) {
            ret = 1;
        }
    }

    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);
    return ret;
}

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_img_fmt_t img_fmt = VPX_IMG_FMT_I420;

    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    vpx_image_t img;
    vpx_img_alloc(&img, img_fmt, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 0, img.stride[0] * cfg.g_h);
    memset(img.planes[1], 128, img.stride[1] * (cfg.g_h / 2));
    memset(img.planes[2], 128, img.stride[2] * (cfg.g_h / 2));

    int ret = 0;

    // Fuzz VP9E_SET_MAX_INTER_BITRATE_PCT
    if (size > 0) {
        int pct = data[0] % 100;
        if (vpx_codec_control(&ctx, VP9E_SET_MAX_INTER_BITRATE_PCT, pct) != VPX_CODEC_OK) {
            ret = 1;
        }
    }

    // Fuzz VP9E_SET_NOISE_SENSITIVITY
    if (size > 1) {
        int noise = data[1] % 16;
        if (vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, noise) != VPX_CODEC_OK) {
            ret = 1;
        }
    }

    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);
    return ret;
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Alternate between VP8 and VP9 based on input
    if (Data[0] % 2 == 0) {
        return fuzz_vp8_encoder(Data, Size);
    } else {
        return fuzz_vp9_encoder(Data, Size);
    }
}