// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_GET_LEVEL__value at vp8cx.h:1068:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>

static int init_encoder(vpx_codec_ctx_t *codec, vpx_codec_iface_t *iface) {
    vpx_codec_enc_cfg_t cfg;
    int ret;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if ((ret = vpx_codec_enc_init(codec, iface, &cfg, 0)) != VPX_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(vpx_codec_ctx_t *codec) {
    vpx_codec_destroy(codec);
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t codec;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    int ret;
    int value;

    if (iface == NULL || Size == 0) {
        return 0;
    }

    if (init_encoder(&codec, iface) != 0) {
        return 0;
    }

    /* Extract a value from the input to use for control calls */
    if (Size >= 4) {
        value = ((int)Data[0] << 24) | ((int)Data[1] << 16) | 
                ((int)Data[2] << 8) | (int)Data[3];
    } else {
        value = 0;
    }

    /* Attempt to call vpx_codec_control with IDs corresponding to the target macros */
    /* Note: These IDs are standard VP9 encoder control IDs in libvpx */
    
    /* VP9E_SET_FRAME_PARALLEL_DECODING */
    ret = vpx_codec_control(&codec, VP9E_SET_FRAME_PARALLEL_DECODING, value);
    (void)ret;

    /* VP9E_SET_POSTENCODE_DROP */
    ret = vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, value);
    (void)ret;

    /* VP9E_SET_NOISE_SENSITIVITY */
    ret = vpx_codec_control(&codec, VP9E_SET_NOISE_SENSITIVITY, value);
    (void)ret;

    /* VP9E_SET_SVC_INTER_LAYER_PRED */
    ret = vpx_codec_control(&codec, VP9E_SET_SVC_INTER_LAYER_PRED, value);
    (void)ret;

    /* VP9E_GET_LEVEL */
    ret = vpx_codec_control(&codec, VP9E_GET_LEVEL, &value);
    (void)ret;

    /* VP9E_SET_ROW_MT (if available in this build) */
    /* This ID might not exist in all versions, but we try to call it if defined */
    /* Since the macro name suggests it exists, we attempt the call */
    ret = vpx_codec_control(&codec, VP9E_SET_ROW_MT, value);
    (void)ret;

    /* Encode a dummy frame to exercise the encoder further */
    vpx_image_t img;
    vpx_codec_enc_cfg_t cfg;
    
    if (vpx_codec_enc_config_default(iface, &cfg, 0) == VPX_CODEC_OK) {
        cfg.g_w = 32;
        cfg.g_h = 32;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        
        if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != NULL) {
            /* Fill image with fuzzer data if available */
            if (Size > 0) {
                size_t total_pixels = (size_t)cfg.g_w * cfg.g_h;
                size_t bytes_to_write = (Size < total_pixels) ? Size : total_pixels;
                
                for (size_t i = 0; i < bytes_to_write; i++) {
                    img.planes[0][i] = Data[i % Size];
                }
            }
            
            vpx_codec_encode(&codec, &img, 0, 1, 0, 0);
            vpx_img_free(&img);
        }
    }

    cleanup_encoder(&codec);

    return 0;
}