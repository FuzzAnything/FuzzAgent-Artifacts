// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) return 0;

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;

    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 0;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&codec, codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    // Fill image with fuzz data if available
    if (size > 0) {
        memcpy(img->planes[0], data, size < img->d_w * img->d_h ? size : img->d_w * img->d_h);
    }

    vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, 2);
    vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, 1);
    vpx_codec_control(&codec, VP9E_SET_QUANTIZER_ONE_PASS, 50);
    vpx_codec_control(&codec, VP9E_SET_ROW_MT, 1);
    vpx_codec_control(&codec, VP9E_SET_FRAME_PERIODIC_BOOST, 1);

    int64_t ts = 0;
    vpx_codec_encode(&codec, img, ts, 1, 0, 0);

    vpx_codec_iter_t iter = NULL;
    vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process encoded frame if needed
        }
    }

    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    return 0;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    return fuzz_vp9_encoder(Data, Size);
}