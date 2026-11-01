// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_SVC_GF_TEMPORAL_REF__value at vp8cx.h:1082:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) {
        return 0;
    }

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;

    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Set basic configuration
    cfg.g_w = 160;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;

    if (vpx_codec_enc_init(&codec, codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Prepare input frame
    vpx_image_t img;
    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
        vpx_codec_destroy(&codec);
        return 0;
    }

    // Calculate plane size for chroma planes (YUV420)
    size_t plane_size = (cfg.g_w * cfg.g_h) / 2;

    // Fill image with fuzzing data if available
    if (size > 0 && data != NULL) {
        size_t max_copy = size < (cfg.g_w * cfg.g_h) ? size : (cfg.g_w * cfg.g_h);
        memcpy(img.planes[0], data, max_copy);
        // Fill other planes with zeros or derived data to avoid uninitialized memory
        memset(img.planes[1], 0, plane_size);
        memset(img.planes[2], 0, plane_size);
    } else {
        memset(img.planes[0], 0, cfg.g_w * cfg.g_h);
        memset(img.planes[1], 0, plane_size);
        memset(img.planes[2], 0, plane_size);
    }

    // Try to set various encoder controls using vpx_codec_control
    // These correspond to the requested macros, which are typically used as:
    // vpx_codec_control(&codec, VP9E_SET_XXX, value)

    // VP9E_SET_MAX_GF_INTERVAL
    if (size > 0) {
        int max_gf_interval = data[0] % 32 + 1; // 1 to 32
        vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);
    }

    // VP9E_SET_POSTENCODE_DROP
    if (size > 1) {
        int postencode_drop = (data[1] % 2); // 0 or 1
        vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, postencode_drop);
    }

    // VP9E_SET_ROW_MT
    if (size > 2) {
        int row_mt = (data[2] % 2); // 0 or 1
        vpx_codec_control(&codec, VP9E_SET_ROW_MT, row_mt);
    }

    // VP9E_SET_SVC_GF_TEMPORAL_REF
    if (size > 3) {
        int svc_gf_temp_ref = (data[3] % 2); // 0 or 1
        vpx_codec_control(&codec, VP9E_SET_SVC_GF_TEMPORAL_REF, svc_gf_temp_ref);
    }

    // VP9E_SET_SVC_INTER_LAYER_PRED
    if (size > 4) {
        int svc_inter_layer_pred = (data[4] % 2); // 0 or 1
        vpx_codec_control(&codec, VP9E_SET_SVC_INTER_LAYER_PRED, svc_inter_layer_pred);
    }

    // VP9E_SET_TARGET_LEVEL
    if (size > 5) {
        int target_level = data[5] % 4; // 0 to 3 (assuming valid levels)
        vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, target_level);
    }

    // Encode a frame
    vpx_codec_err_t res = vpx_codec_encode(&codec, &img, 0, 1, 0, 0);
    if (res != VPX_CODEC_OK) {
        // Non-fatal, just continue
    }

    // Cleanup
    vpx_img_free(&img);
    vpx_codec_destroy(&codec);

    return 0;
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Run the fuzzing logic
    fuzz_vp9_encoder(Data, Size);

    return 0;
}