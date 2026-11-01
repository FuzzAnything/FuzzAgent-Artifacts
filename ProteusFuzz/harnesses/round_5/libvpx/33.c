// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_MIN_GF_INTERVAL__value at vp8cx.h:1052:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_LOSSLESS__value at vp8cx.h:1022:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vp8cx.h"
#include "vp8dx.h"

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;

    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input data
    if (size > 0) cfg.g_w = (data[0] % 256) + 16;
    if (size > 1) cfg.g_h = (data[1] % 256) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 255) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 255) + 1;
    if (size > 4) cfg.rc_target_bitrate = (data[4] % 1000) + 1;
    if (size > 5) cfg.g_error_resilient = (data[5] % 3);

    if (vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Prepare dummy frame
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzzed data if available
    if (size > 0) {
        size_t plane_size = img->d_w * img->d_h;
        size_t copy_size = (size < plane_size) ? size : plane_size;
        memcpy(img->planes[0], data, copy_size);
        if (size > plane_size) {
            memcpy(img->planes[1], data + plane_size, size - plane_size);
        }
    }

    vpx_codec_control(&ctx, VP9E_SET_MAX_GF_INTERVAL, (data[0] % 100) + 1);
    vpx_codec_control(&ctx, VP9E_SET_MIN_GF_INTERVAL, (data[1] % 100) + 1);
    vpx_codec_control(&ctx, VP9E_SET_FRAME_PERIODIC_BOOST, (data[2] % 256));
    vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, (data[3] % 2));
    vpx_codec_control(&ctx, VP9E_SET_TARGET_LEVEL, (data[4] % 6));
    vpx_codec_control(&ctx, VP9E_SET_LOSSLESS, (data[5] % 2));

    vpx_codec_encode(&ctx, img, 0, 1, 0, 0);

    vpx_img_free(img);
    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;
    fuzz_vp9_encoder(Data, Size);
    return 0;
}