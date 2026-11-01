// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP9E_ENABLE_MOTION_VECTOR_UNIT_TEST__value at vp8cx.h:1074:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP8E_SET_SHARPNESS__value at vp8cx.h:990:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx = {0};
    // Fixed: Use vpx_codec_vp8_cx() instead of non-existent vpx_codec_vp8_cx_iface()
    vpx_codec_iface_t *encoder = vpx_codec_vp8_cx();
    if (!encoder) {
        return 0;
    }

    vpx_codec_enc_cfg_t cfg = {0};
    if (vpx_codec_enc_config_default(encoder, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input data
    if (Size > 0) cfg.g_w = Data[0] + 16;
    if (Size > 1) cfg.g_h = Data[1] + 16;
    if (Size > 2) cfg.g_timebase.num = Data[2] + 1;
    if (Size > 3) cfg.g_timebase.den = Data[3] + 1;
    if (Size > 4) cfg.rc_target_bitrate = Data[4] + 1;
    if (Size > 5) cfg.g_lag_in_frames = Data[5];
    if (Size > 6) cfg.g_error_resilient = Data[6] % 2;

    if (vpx_codec_enc_init(&ctx, encoder, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy image buffer
    int img_size = (cfg.g_w * cfg.g_h * 3) / 2; // I420 approx
    uint8_t *img_data = malloc(img_size);
    if (!img_data) {
        cleanup_encoder(&ctx);
        return 0;
    }
    memset(img_data, 0, img_size);
    if (Size > 0) {
        size_t copy_len = (Size < img_size) ? Size : img_size;
        memcpy(img_data, Data, copy_len);
    }

    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    vpx_image_t img = {0};
    img.d_w = cfg.g_w;
    img.d_h = cfg.g_h;
    img.w = img.d_w;
    img.h = img.d_h;
    img.stride[0] = img.w;
    img.stride[1] = img.w / 2;
    img.stride[2] = img.w / 2;
    img.planes[0] = img_data;
    img.planes[1] = img_data + img.w * img.h;
    img.planes[2] = img_data + img.w * img.h + (img.w / 2) * (img.h / 2);
    img.fmt = fmt;

    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    // Attempt to invoke control functions with fuzzed values
    // Note: Some of these constants might not be defined or valid for VP8, 
    // which is acceptable for fuzzing to trigger error handling paths.
    
    // VP8E_SET_NOISE_SENSITIVITY
    int noise_val = (Size > 7) ? Data[7] : 0;
    vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, noise_val);

    // VP9E_ENABLE_MOTION_VECTOR_UNIT_TEST
    int mv_test_val = (Size > 8) ? Data[8] : 0;
    vpx_codec_control(&ctx, VP9E_ENABLE_MOTION_VECTOR_UNIT_TEST, mv_test_val);

    // VP9E_SET_NOISE_SENSITIVITY
    int vp9_noise_val = (Size > 9) ? Data[9] : 0;
    vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, vp9_noise_val);

    // VP8E_SET_SHARPNESS
    int sharpness_val = (Size > 10) ? Data[10] : 0;
    vpx_codec_control(&ctx, VP8E_SET_SHARPNESS, sharpness_val);

    // VP8E_SET_ARNR_STRENGTH
    int arnr_val = (Size > 11) ? Data[11] : 0;
    vpx_codec_control(&ctx, VP8E_SET_ARNR_STRENGTH, arnr_val);

    // VP8E_SET_SCREEN_CONTENT_MODE
    int screen_mode_val = (Size > 12) ? Data[12] : 0;
    vpx_codec_control(&ctx, VP8E_SET_SCREEN_CONTENT_MODE, screen_mode_val);

    // Drain the encoder
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process frame data if needed, here just ignore
        }
    }

    free(img_data);
    cleanup_encoder(&ctx);
    return 0;
}