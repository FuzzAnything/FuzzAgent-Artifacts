// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_RENDER_SIZE__value at vp8cx.h:1062:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// VP9E_SET_COLOR_SPACE__value at vp8cx.h:1050:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t ecfg = {0};
    vpx_codec_err_t err;
    const vpx_codec_iface_t *encoder = NULL;

    // Try to get the VP9 encoder interface
    encoder = vpx_codec_vp9_cx();
    if (!encoder) {
        return 0;
    }

    // Initialize encoder configuration
    if (vpx_codec_enc_config_default(encoder, &ecfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Configure basic settings from input with bounds checks
    ecfg.g_w = (Data[0] % 512) + 16;
    ecfg.g_h = (Data[1] % 512) + 16;
    ecfg.g_timebase.num = (Data[2] % 100) + 1;
    ecfg.g_timebase.den = (Data[3] % 100) + 1;
    
    // Ensure we have enough data before accessing index 4
    if (Size >= 5) {
        ecfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    } else {
        ecfg.rc_target_bitrate = 100;
    }
    
    ecfg.g_error_resilient = 1;

    // Initialize encoder
    err = vpx_codec_enc_init(&ctx, encoder, &ecfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Extract values from input data for controls with bounds checks
    uint32_t val1 = 0;
    uint32_t val2 = 0;
    uint32_t val3 = 0;
    uint32_t val4 = 0;
    uint32_t val5 = 0;
    int val6_w = 16;
    int val6_h = 16;

    if (Size >= 8) {
        val1 = *(const uint32_t *)(Data + 4);
        val2 = *(const uint32_t *)(Data + 8);
    }
    if (Size >= 12) {
        val3 = *(const uint32_t *)(Data + 12);
    }
    if (Size >= 16) {
        val4 = *(const uint32_t *)(Data + 16);
    }
    if (Size >= 20) {
        val5 = *(const uint32_t *)(Data + 20);
    }
    if (Size >= 28) {
        val6_w = *(const int32_t *)(Data + 24);
        val6_h = *(const int32_t *)(Data + 28);
    }

    // Fuzz VP9E_SET_COLOR_RANGE (expects int: 0 or 1)
    vpx_codec_control(&ctx, VP9E_SET_COLOR_RANGE, val1 & 0x1);

    // Fuzz VP9E_SET_COLOR_SPACE (expects int: color space enum)
    vpx_codec_control(&ctx, VP9E_SET_COLOR_SPACE, val2 & 0x7);

    // Fuzz VP9E_SET_POSTENCODE_DROP (expects int: 0 or 1)
    vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, val3 & 0x1);

    // Fuzz VP9E_SET_FRAME_PERIODIC_BOOST (expects int: 0 or 1)
    vpx_codec_control(&ctx, VP9E_SET_FRAME_PERIODIC_BOOST, val4 & 0x1);

    // Fuzz VP9E_SET_TARGET_LEVEL (expects int: level enum)
    vpx_codec_control(&ctx, VP9E_SET_TARGET_LEVEL, val5 & 0x7);

    // Fuzz VP9E_SET_RENDER_SIZE (expects int* containing width and height)
    // We need to pass a pointer to an array of two ints
    int render_size[2];
    render_size[0] = val6_w > 0 ? val6_w : 16;
    render_size[1] = val6_h > 0 ? val6_h : 16;
    vpx_codec_control(&ctx, VP9E_SET_RENDER_SIZE, render_size);

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}