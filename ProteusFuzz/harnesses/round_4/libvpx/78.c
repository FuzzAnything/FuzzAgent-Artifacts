// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// VP9E_GET_LEVEL__value at vp8cx.h:1068:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/*
 * Helper function to initialize a VP9 encoder context.
 * Returns NULL on failure.
 */
static vpx_codec_ctx_t* init_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface;
    vpx_codec_err_t res;
    vpx_codec_ctx_t *ctx_ptr = NULL;

    // Initialize configuration
    if (vpx_codec_enc_config_default(&vpx_codec_vp9_cx_algo, &cfg, 0) != VPX_CODEC_OK) {
        return NULL;
    }

    // Fuzz some configuration parameters based on input data
    if (size > 0) cfg.g_w = 16 + (data[0] % 384);
    if (size > 1) cfg.g_h = 16 + (data[1] % 384);
    if (size > 2) cfg.g_timebase.num = 1 + (data[2] % 100);
    if (size > 3) cfg.g_timebase.den = 1 + (data[3] % 100);
    if (size > 4) cfg.g_lag_in_frames = (data[4] % 256);
    if (size > 5) cfg.rc_target_bitrate = 100 + (data[5] % 10000);

    // Get the interface
    iface = &vpx_codec_vp9_cx_algo;
    if (!iface) return NULL;

    // Allocate context dynamically to return pointer
    ctx_ptr = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx_ptr) return NULL;
    memset(ctx_ptr, 0, sizeof(vpx_codec_ctx_t));

    // Initialize the codec
    res = vpx_codec_enc_init(ctx_ptr, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        free(ctx_ptr);
        return NULL;
    }

    return ctx_ptr;
}

/*
 * Helper function to initialize a VP8 encoder context.
 * Returns NULL on failure.
 */
static vpx_codec_ctx_t* init_vp8_encoder(const uint8_t *data, size_t size) {
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface;
    vpx_codec_err_t res;
    vpx_codec_ctx_t *ctx_ptr = NULL;

    // Initialize configuration
    if (vpx_codec_enc_config_default(&vpx_codec_vp8_cx_algo, &cfg, 0) != VPX_CODEC_OK) {
        return NULL;
    }

    // Fuzz some configuration parameters
    if (size > 0) cfg.g_w = 16 + (data[0] % 384);
    if (size > 1) cfg.g_h = 16 + (data[1] % 384);
    if (size > 2) cfg.g_timebase.num = 1 + (data[2] % 100);
    if (size > 3) cfg.g_timebase.den = 1 + (data[3] % 100);

    iface = &vpx_codec_vp8_cx_algo;
    if (!iface) return NULL;

    ctx_ptr = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx_ptr) return NULL;
    memset(ctx_ptr, 0, sizeof(vpx_codec_ctx_t));

    res = vpx_codec_enc_init(ctx_ptr, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        free(ctx_ptr);
        return NULL;
    }

    return ctx_ptr;
}

/*
 * Helper to perform a dummy encode frame to ensure state is valid before control
 */
static int perform_dummy_encode(vpx_codec_ctx_t *ctx) {
    vpx_image_t img;
    vpx_codec_cx_pkt_t pkt;
    vpx_codec_err_t res;

    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, 16, 16, 1) != 0) return 0;

    // Fill image with zeros
    memset(img.planes[0], 0, img.stride[0] * img.d_h);
    memset(img.planes[1], 0, img.stride[1] * img.d_h / 2);
    memset(img.planes[2], 0, img.stride[2] * img.d_h / 2);

    // Encode one frame
    res = vpx_codec_encode(ctx, &img, 0, 1, 0, 0);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(&img);
        return 0;
    }

    // Drain packets
    while (vpx_codec_get_cx_data(ctx, &pkt) == VPX_CODEC_OK) {
        if (pkt.kind == VPX_CODEC_CX_FRAME_PKT) {
            // Do nothing, just consume
        }
    }

    vpx_img_free(&img);
    return 1;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Select codec based on first byte
    int is_vp9 = (Data[0] % 2) == 0;
    vpx_codec_ctx_t *ctx = NULL;

    if (is_vp9) {
        ctx = init_vp9_encoder(Data, Size);
    } else {
        ctx = init_vp8_encoder(Data, Size);
    }

    if (!ctx) return 0;

    // Perform a dummy encode to ensure context is in a valid state for controls
    if (!perform_dummy_encode(ctx)) {
        vpx_codec_destroy(ctx);
        free(ctx);
        return 0;
    }

    // Fuzz control parameters based on input data
    int disable_loopfilter = (Size > 1) ? (Data[1] % 2) : 0;
    int frame_parallel_decoding = (Size > 2) ? (Data[2] % 2) : 0;
    int noise_sensitivity = (Size > 3) ? (Data[3] % 10) : 0;
    int screen_content_mode = (Size > 4) ? (Data[4] % 2) : 0;

    // 1. VP9E_SET_DISABLE_LOOPFILTER (if VP9)
    if (is_vp9) {
        vpx_codec_control(ctx, VP9E_SET_DISABLE_LOOPFILTER, disable_loopfilter);
    }

    // 2. VP9E_SET_FRAME_PARALLEL_DECODING (if VP9)
    if (is_vp9) {
        vpx_codec_control(ctx, VP9E_SET_FRAME_PARALLEL_DECODING, frame_parallel_decoding);
    }

    // 3. VP9E_GET_LEVEL (if VP9) - returns value via pointer
    if (is_vp9) {
        int level;
        vpx_codec_control(ctx, VP9E_GET_LEVEL, &level);
    }

    // 4. VP9E_SET_NOISE_SENSITIVITY (if VP9)
    if (is_vp9) {
        vpx_codec_control(ctx, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    }

    // 6. VP8E_SET_SCREEN_CONTENT_MODE (if VP8)
    if (!is_vp9) {
        vpx_codec_control(ctx, VP8E_SET_SCREEN_CONTENT_MODE, screen_content_mode);
    }

    // Cleanup
    vpx_codec_destroy(ctx);
    free(ctx);

    return 0;
}