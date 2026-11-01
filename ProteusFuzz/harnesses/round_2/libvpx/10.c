// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8cx.h"

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t raw;
    vpx_codec_err_t res;
    vpx_codec_iter_t iter = NULL;
    vpx_codec_cx_pkt_t *pkt;
    int ret = 0;
    const vpx_codec_iface_t *codec;

    if (size < 15) {
        return 0;
    }

    codec = vpx_codec_vp9_cx();
    if (codec == NULL) {
        return 0;
    }

    if (vpx_codec_enc_config_default(&cfg, codec, 0) != 0) {
        return 0;
    }

    cfg.g_w = (data[0] % 128) + 16;
    cfg.g_h = (data[1] % 128) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (data[2] % 60) + 30;
    cfg.rc_target_bitrate = (data[3] * 256 + data[4]) + 100;
    cfg.rc_min_quantizer = (data[5] % 63) + 1;
    cfg.rc_max_quantizer = (data[6] % 63) + 1;

    if (vpx_codec_enc_init(&ctx, codec, &cfg, 0) != 0) {
        return 0;
    }

    if (vpx_img_alloc(&raw, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16) != 0) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    memset(raw.planes[0], data[7], cfg.g_w * cfg.g_h);
    memset(raw.planes[1], data[8], (cfg.g_w / 2) * (cfg.g_h / 2));
    memset(raw.planes[2], data[9], (cfg.g_w / 2) * (cfg.g_h / 2));

    vpx_codec_control(&ctx, VP9E_SET_FRAME_PERIODIC_BOOST, data[10]);
    vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, data[11]);
    vpx_codec_control(&ctx, VP8E_SET_STATIC_THRESHOLD, data[12]);
    vpx_codec_control(&ctx, VP9E_SET_TUNE_CONTENT, data[13] % 3);
    vpx_codec_control(&ctx, VP9E_SET_TILE_COLUMNS, data[14] % 8);

    res = vpx_codec_encode(&ctx, &raw, 0, 1, 0, 0);
    if (res != 0) {
        vpx_img_free(&raw);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            if (pkt->data.frame.sz > 0) {
                ret = 1;
            }
        }
    }

    vpx_img_free(&raw);
    vpx_codec_destroy(&ctx);
    return ret;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 15) {
        return 0;
    }

    return fuzz_vp9_encoder(Data, Size);
}