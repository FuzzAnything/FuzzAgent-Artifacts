// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_LOSSLESS__value at vp8cx.h:1022:1 in vp8cx.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// VP9D_SET_ROW_MT__value at vp8dx.h:216:1 in vp8dx.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static int fuzz_encoder(const uint8_t *data, size_t size) {
    const vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;

    res = vpx_codec_enc_config_default(codec_iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return 0;

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    res = vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz VP9E_SET_LOSSLESS
    int lossless_val = (size > 0) ? (data[0] % 2) : 0;
    res = vpx_codec_control(&ctx, VP9E_SET_LOSSLESS, lossless_val);
    (void)res;

    // Fuzz VP9E_SET_MAX_GF_INTERVAL
    int max_gf_interval = (size > 1) ? (data[1] % 8 + 1) : 1;
    res = vpx_codec_control(&ctx, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);
    (void)res;

    // Fuzz VP9E_SET_POSTENCODE_DROP
    int postencode_drop = (size > 2) ? (data[2] % 2) : 0;
    res = vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, postencode_drop);
    (void)res;

    // Fuzz VP9E_SET_ROW_MT
    int row_mt = (size > 3) ? (data[3] % 2) : 0;
    res = vpx_codec_control(&ctx, VP9E_SET_ROW_MT, row_mt);
    (void)res;

    // Fuzz VP9E_SET_FRAME_PERIODIC_BOOST
    int frame_periodic_boost = (size > 4) ? (data[4] % 2) : 0;
    res = vpx_codec_control(&ctx, VP9E_SET_FRAME_PERIODIC_BOOST, frame_periodic_boost);
    (void)res;

    // Encode a dummy frame
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 0, img.stride[0] * cfg.g_h);
    memset(img.planes[1], 128, img.stride[1] * (cfg.g_h / 2));
    memset(img.planes[2], 128, img.stride[2] * (cfg.g_h / 2));

    vpx_codec_frame_flags_t flags = 0;
    res = vpx_codec_encode(&ctx, &img, 0, 1, flags, VPX_DL_REALTIME);
    (void)res;

    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);
    return 0;
}

static int fuzz_decoder(const uint8_t *data, size_t size) {
    const vpx_codec_iface_t *codec_iface = vpx_codec_vp9_dx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_err_t res;

    res = vpx_codec_dec_init(&ctx, codec_iface, NULL, 0);
    if (res != VPX_CODEC_OK) return 0;

    // Fuzz VP9D_SET_ROW_MT
    int row_mt = (size > 0) ? (data[0] % 2) : 0;
    res = vpx_codec_control(&ctx, VP9D_SET_ROW_MT, row_mt);
    (void)res;

    // Attempt to decode dummy data (no actual data provided, just testing control)
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *frame = vpx_codec_get_frame(&ctx, &iter);
    (void)frame;

    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Choose between encoder and decoder fuzzing based on first byte
    if (Data[0] & 1) {
        fuzz_encoder(Data, Size);
    } else {
        fuzz_decoder(Data, Size);
    }

    return 0;
}