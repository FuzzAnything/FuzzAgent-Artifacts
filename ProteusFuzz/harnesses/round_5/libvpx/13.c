// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR__value at vp8cx.h:1093:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
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
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    if (size < 16) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface;
    vpx_image_t img;
    vpx_codec_cx_pkt_t *pkt;
    vpx_codec_err_t err;
    int svc_inter_layer_pred = (data[0] % 2);
    int noise_sensitivity = (data[1] % 100);
    int tune_content = (data[2] % 3);
    int disable_overshoot = (data[3] % 2);

    iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }

    err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;
    cfg.g_lag_in_frames = 0;

    err = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Invoke VP9E_SET_SVC_INTER_LAYER_PRED
    err = vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, svc_inter_layer_pred);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Invoke VP9E_SET_NOISE_SENSITIVITY
    err = vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Invoke VP9E_SET_TUNE_CONTENT
    err = vpx_codec_control(&ctx, VP9E_SET_TUNE_CONTENT, tune_content);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Invoke VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR
    err = vpx_codec_control(&ctx, VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR, disable_overshoot);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Prepare a dummy image for encoding
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, 128, 128, 1);
    memset(img.planes[0], data[4], img.stride[0] * 128);
    memset(img.planes[1], data[5], img.stride[1] * 64);
    memset(img.planes[2], data[6], img.stride[2] * 64);

    err = vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(&img);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Get encoded packets
    while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process encoded data if needed
        }
        // vpx_codec_cx_pkt_free is not a valid API; packets are managed internally or via vpx_codec_destroy
    }

    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    fuzz_vp9_encoder(Data, Size);
    return 0;
}