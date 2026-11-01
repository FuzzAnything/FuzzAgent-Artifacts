// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_ENABLEAUTOALTREF__value at vp8cx.h:986:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

static int fuzz_vp8_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t img;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    int ret;
    unsigned char *img_data = NULL;

    if (size < 2) return 0;

    ret = vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    ret = vpx_codec_enc_init(&ctx, vpx_codec_vp8_cx(), &cfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    // Set various control parameters based on input data
    int val = (int)data[0] & 7; // CPUUSED 0-8
    ret = vpx_codec_control(&ctx, VP8E_SET_CPUUSED, val);
    (void)ret;

    val = (int)data[1] & 1; // Enable/Disable
    ret = vpx_codec_control(&ctx, VP8E_SET_ENABLEAUTOALTREF, val);
    (void)ret;

    val = (int)data[1] % 101; // 0-100
    ret = vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, val);
    (void)ret;

    val = (int)data[1] % 101; // 0-100
    ret = vpx_codec_control(&ctx, VP8E_SET_GF_CBR_BOOST_PCT, val);
    (void)ret;

    // Prepare a dummy frame
    int stride = cfg.g_w;
    int uv_stride = stride / 2;
    int frame_size = stride * cfg.g_h + 2 * uv_stride * (cfg.g_h / 2);
    img_data = (unsigned char *)malloc(frame_size);
    if (!img_data) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    vpx_img_wrap(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, stride, img_data);
    memset(img.planes[0], 128, stride * cfg.g_h);
    memset(img.planes[1], 128, uv_stride * (cfg.g_h / 2));
    memset(img.planes[2], 128, uv_stride * (cfg.g_h / 2));

    ret = vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
    if (ret != VPX_CODEC_OK) {
        free(img_data);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process encoded frame data if needed
            (void)pkt->data.frame;
        }
    }

    free(img_data);
    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz VP8 encoder
    return fuzz_vp8_encoder(Data, Size);
}