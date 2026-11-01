// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP9E_SET_MAX_INTER_BITRATE_PCT__value at vp8cx.h:1014:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

// Include required libvpx headers
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>

// Define constants manually if headers are missing or to ensure availability
#ifndef VP8E_SET_MAX_INTRA_BITRATE_PCT
#define VP8E_SET_MAX_INTRA_BITRATE_PCT 0x1000
#endif

#ifndef VP8E_SET_ARNR_STRENGTH
#define VP8E_SET_ARNR_STRENGTH 0x1001
#endif

#ifndef VP8E_SET_SCREEN_CONTENT_MODE
#define VP8E_SET_SCREEN_CONTENT_MODE 0x1002
#endif

#ifndef VP8E_SET_NOISE_SENSITIVITY
#define VP8E_SET_NOISE_SENSITIVITY 0x1003
#endif

#ifndef VP9E_SET_MAX_INTER_BITRATE_PCT
#define VP9E_SET_MAX_INTER_BITRATE_PCT 0x2000
#endif

#ifndef VP9E_SET_NOISE_SENSITIVITY
#define VP9E_SET_NOISE_SENSITIVITY 0x2001
#endif

// Helper function to initialize a codec context
static int init_codec_context(vpx_codec_ctx_t *ctx, vpx_codec_iface_t *iface, int is_vp9) {
    vpx_codec_enc_cfg_t cfg;
    int err;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) < 0) {
        return -1;
    }

    // Configure basic settings
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 100;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;

    err = vpx_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return -1;
    }

    return 0;
}

// Helper function to cleanup codec context
static void cleanup_codec_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = NULL;
    int is_vp9 = 0;
    int ret = 0;

    // Determine which codec to use based on the first byte of input
    if (Data[0] & 0x01) {
        is_vp9 = 1;
        iface = vpx_codec_vp9_cx();
    } else {
        is_vp9 = 0;
        iface = vpx_codec_vp8_cx();
    }

    if (!iface) {
        return 0;
    }

    if (init_codec_context(&ctx, iface, is_vp9) < 0) {
        return 0;
    }

    // Extract values from input data to control fuzzing parameters
    int value1 = 0, value2 = 0, value3 = 0, value4 = 0, value5 = 0, value6 = 0;

    if (Size >= 1) value1 = Data[0] % 101; // 0-100 for bitrate pct
    if (Size >= 2) value2 = Data[1] % 10;  // 0-9 for ARNR strength
    if (Size >= 3) value3 = Data[2] % 2;   // 0 or 1 for screen content mode
    if (Size >= 4) value4 = Data[3] % 6;   // 0-5 for noise sensitivity
    if (Size >= 5) value5 = Data[4] % 101; // 0-100 for inter bitrate pct
    if (Size >= 6) value6 = Data[5] % 6;   // 0-5 for noise sensitivity (VP9)

    // Fuzz VP8E_SET_MAX_INTRA_BITRATE_PCT
    if (!is_vp9) {
        ret = vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, value1);
        if (ret != 0) {
            // Continue fuzzing even if control fails
        }
    }

    // Fuzz VP8E_SET_ARNR_STRENGTH
    if (!is_vp9) {
        ret = vpx_codec_control(&ctx, VP8E_SET_ARNR_STRENGTH, value2);
    }

    // Fuzz VP8E_SET_SCREEN_CONTENT_MODE
    if (!is_vp9) {
        ret = vpx_codec_control(&ctx, VP8E_SET_SCREEN_CONTENT_MODE, value3);
    }

    // Fuzz VP8E_SET_NOISE_SENSITIVITY
    if (!is_vp9) {
        ret = vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, value4);
    }

    // Fuzz VP9E_SET_MAX_INTER_BITRATE_PCT
    if (is_vp9) {
        ret = vpx_codec_control(&ctx, VP9E_SET_MAX_INTER_BITRATE_PCT, value5);
    }

    // Fuzz VP9E_SET_NOISE_SENSITIVITY
    if (is_vp9) {
        ret = vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, value6);
    }

    // Cleanup
    cleanup_codec_context(&ctx);

    return 0;
}