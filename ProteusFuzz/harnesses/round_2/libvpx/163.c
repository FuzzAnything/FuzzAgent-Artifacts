// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
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
 * Fuzz driver for libvpx targeting control commands.
 */

static vpx_codec_ctx_t codec_ctx;
static vpx_codec_enc_cfg_t cfg;
static int initialized = 0;

static int init_encoder(void) {
    vpx_codec_iface_t *iface;
    vpx_codec_err_t err;

    iface = vpx_codec_vp9_cx();
    if (!iface) return -1;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 0;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;

    err = vpx_codec_enc_init(&codec_ctx, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return -1;
    }

    initialized = 1;
    return 0;
}

static void cleanup_encoder(void) {
    if (initialized) {
        vpx_codec_destroy(&codec_ctx);
        initialized = 0;
    }
}

int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize if not already done
    if (!initialized) {
        if (init_encoder() != 0) return 0;
    }

    // Parse input to determine control ID and arguments
    int ctrl_id = (int)Data[0];
    int arg1 = 0;
    int arg2 = 0;
    int arg3 = 0;
    int arg4 = 0;
    int arg5 = 0;

    if (Size > 1) arg1 = (int)Data[1];
    if (Size > 2) arg2 = (int)Data[2];
    if (Size > 3) arg3 = (int)Data[3];
    if (Size > 4) arg4 = (int)Data[4];
    if (Size > 5) arg5 = (int)Data[5];

    vpx_codec_err_t res;

    // Use vpx_codec_control_ directly to allow dynamic ctrl_id fuzzing.
    
    // Pattern 1: Single int argument
    res = vpx_codec_control_(&codec_ctx, ctrl_id, arg1);
    
    // Pattern 2: Two int arguments
    if (Size > 2) {
        res = vpx_codec_control_(&codec_ctx, ctrl_id, arg1, arg2);
    }
    
    // Pattern 3: Three int arguments
    if (Size > 3) {
        res = vpx_codec_control_(&codec_ctx, ctrl_id, arg1, arg2, arg3);
    }

    // Try to encode a frame to ensure the context remains valid and triggers more code paths
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Fill with fuzz data safely
    // Ensure we don't read out of bounds from Data
    uint8_t fill_y = (Size > 0) ? Data[0] : 0;
    uint8_t fill_u = (Size > 1) ? Data[1] : 0;
    uint8_t fill_v = (Size > 2) ? Data[2] : 0;

    memset(img.planes[0], fill_y, img.stride[0] * img.d_h);
    memset(img.planes[1], fill_u, img.stride[1] * img.d_h / 2);
    memset(img.planes[2], fill_v, img.stride[2] * img.d_h / 2);

    // Fixed: Use 0 instead of undefined VPX_EFLAG_NONE
    vpx_codec_err_t encode_res = vpx_codec_encode(&codec_ctx, &img, 0, 1, 0, 0);
    
    // Cleanup image
    vpx_img_free(&img);

    // If the context is in a bad state, reset it
    if (res != VPX_CODEC_OK && encode_res != VPX_CODEC_OK) {
        cleanup_encoder();
        if (init_encoder() != 0) return 0;
    }

    return 0;
}