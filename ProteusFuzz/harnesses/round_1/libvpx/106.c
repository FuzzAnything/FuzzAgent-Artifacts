// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_GET_LEVEL__value at vp8cx.h:1068:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "vp8cx.h"
#include "vp8dx.h"

/* 
 * Fuzz driver for libvpx VP9 encoder.
 * Targets: VP9E_GET_LEVEL, VP9E_SET_NOISE_SENSITIVITY, VP9E_SET_FRAME_PERIODIC_BOOST,
 *          VP9E_SET_ROW_MT, VP9E_SET_POSTENCODE_DROP, VP9E_SET_SVC_INTER_LAYER_PRED.
 */

static int try_encode_frame(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data) return -1;

    vpx_image_t img;
    /* Initialize image with I420 format, 128x128 resolution */
    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, 128, 128, 1) != 0) {
        return -1;
    }

    /* Fill Y plane with fuzz data */
    /* Use img.d_w and img.d_h instead of d_y */
    size_t y_size = img.d_h * img.stride[0];
    size_t copy_size = (size < y_size) ? size : y_size;
    memcpy(img.planes[VPX_PLANE_Y], data, copy_size);
    
    /* Fill UV planes with derived data or zeros */
    /* Use img.d_uv or calculate based on height/2 */
    size_t uv_size = (img.d_h / 2) * img.stride[1];
    if (uv_size > 0) {
        memset(img.planes[VPX_PLANE_U], 0, uv_size);
        memset(img.planes[VPX_PLANE_V], 0, uv_size);
    }

    /* Use 0 for flags instead of VPX_EFLAG_NONE which might not be defined in older versions */
    vpx_codec_encode(ctx, &img, 0, 1, 0, 0);

    vpx_img_free(&img);
    return 0;
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    /* Get VP9 encoder interface */
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    /* Use input data to configure encoder settings */
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Size > 1 ? (Data[1] % 1024) + 16 : cfg.g_w);
    
    if (Size > 2) {
        cfg.g_timebase.num = (Data[2] % 100) + 1;
        cfg.g_timebase.den = (Data[2] % 60) + 1;
    }
    if (Size > 3) {
        cfg.g_threads = (Data[3] % 4) + 1;
    }
    if (Size > 4) {
        cfg.rc_target_bitrate = (Data[4] % 1000) + 1;
    }
    if (Size > 5) {
        cfg.rc_min_quantizer = Data[5] % 64;
        cfg.rc_max_quantizer = (Data[5] + 10) % 64;
        if (cfg.rc_max_quantizer < cfg.rc_min_quantizer) {
            cfg.rc_max_quantizer = cfg.rc_min_quantizer + 1;
        }
    }
    if (Size > 6) {
        cfg.kf_mode = (Data[6] % 2) ? VPX_KF_AUTO : VPX_KF_DISABLED;
    }
    if (Size > 7) {
        cfg.kf_max_dist = (Data[7] % 256) + 1;
    }

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    int val = (Size > 8) ? (Data[8] % 100) : 0;
    int val2 = (Size > 9) ? (Data[9] % 100) : 0;
    int val3 = (Size > 10) ? (Data[10] % 100) : 0;
    int val4 = (Size > 11) ? (Data[11] % 100) : 0;
    int val5 = (Size > 12) ? (Data[12] % 100) : 0;
    int val6 = (Size > 13) ? (Data[13] % 100) : 0;

    /* Try to set Noise Sensitivity */
    vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, val);
    
    /* Try to set Frame Periodic Boost */
    vpx_codec_control(&ctx, VP9E_SET_FRAME_PERIODIC_BOOST, val2);
    
    /* Try to set Row Multi-Threading */
    vpx_codec_control(&ctx, VP9E_SET_ROW_MT, val3);
    
    /* Try to set Post-Encode Drop */
    vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, val4);
    
    /* Try to set SVC Inter-Layer Prediction */
    vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, val5);
    
    /* Try to get Level (Requires a pointer to int) */
    int level_out = 0;
    vpx_codec_control(&ctx, VP9E_GET_LEVEL, &level_out);

    /* Encode a frame to exercise the state */
    try_encode_frame(&ctx, Data, Size);

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}