// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_encoder.h"

/* 
 * Helper to safely initialize encoder config with fuzzed data.
 * We ensure critical fields (width, height, bitrate) are non-zero to avoid
 * immediate invalid parameter errors, while allowing other fields to be fuzzed.
 */
static void init_enc_cfg(const uint8_t *data, size_t size, vpx_codec_enc_cfg_t *cfg) {
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    // Use fuzzed data for initialization if available
    if (size > 0) {
        memcpy(cfg, data, size < sizeof(vpx_codec_enc_cfg_t) ? size : sizeof(vpx_codec_enc_cfg_t));
    }

    // Enforce minimum valid values for critical parameters to reach deeper logic
    if (cfg->g_w == 0) cfg->g_w = 16;
    if (cfg->g_h == 0) cfg->g_h = 16;
    if (cfg->g_w > 4096) cfg->g_w = 4096;
    if (cfg->g_h > 4096) cfg->g_h = 4096;
    
    if (cfg->g_bit_depth == 0) cfg->g_bit_depth = VPX_BITS_8;
    if (cfg->g_input_bit_depth == 0) cfg->g_input_bit_depth = 8;
    
    // Ensure timebase is valid
    if (cfg->g_timebase.den == 0) {
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
    }

    if (cfg->rc_target_bitrate == 0) cfg->rc_target_bitrate = 100;
    
    // Ensure quantizers are within reasonable bounds
    if (cfg->rc_min_quantizer > 255) cfg->rc_min_quantizer = 0;
    if (cfg->rc_max_quantizer > 255) cfg->rc_max_quantizer = 255;
    if (cfg->rc_min_quantizer > cfg->rc_max_quantizer) {
        unsigned int tmp = cfg->rc_min_quantizer;
        cfg->rc_min_quantizer = cfg->rc_max_quantizer;
        cfg->rc_max_quantizer = tmp;
    }
}

static vpx_codec_iface_t *get_codec_iface(const uint8_t *data, size_t size) {
    // Fuzz selection between VP8 and VP9 encoders
    // VP8: 0, VP9: 1
    int sel = (size > 0) ? (data[0] % 2) : 0;
    
    if (sel == 0) {
        return vpx_codec_vp8_cx();
    } else {
        return vpx_codec_vp9_cx();
    }
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t res;

    // 1. Prepare environment
    init_enc_cfg(Data, Size, &cfg);
    iface = get_codec_iface(Data, Size);

    if (iface == NULL) {
        return 0;
    }

    // 2. Initialize encoder
    // Using VPX_CODEC_USE_PSNR to test flags
    vpx_codec_flags_t flags = 0;
    if (Size > 1 && (Data[1] & 1)) {
        flags |= VPX_CODEC_USE_PSNR;
    }
    // Removed VPX_CODEC_USE_FRAME_CONTEXTS as it is not a valid flag constant
    // It was likely a confusion with VPX_CODEC_CX_FRAME_PKT which is a packet type, not a flag.

    res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);

    if (res != VPX_CODEC_OK) {
        // If init fails, we don't need to destroy, but we can still test control on null/invalid ctx
        // However, vpx_codec_control_ expects a valid ctx.
        return 0;
    }

    // 3. Explore program states via vpx_codec_control_
    // We try various control IDs based on fuzzed data to trigger different handlers
    
    // VP8E_SET_CPUUSED (0)
    if (Size > 3) {
        int cpuused = Data[3] % 9; // 0-8
        vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, cpuused);
    }

    // VP8E_SET_TUNING (1)
    if (Size > 4) {
        int tuning = Data[4] % 10; // 0-9
        vpx_codec_control_(&ctx, VP8E_SET_TUNING, tuning);
    }

    // VP9E_SET_TUNE_CONTENT (2) - Note: Enum values might differ, but we try common ones
    // VP9E_SET_TUNE_CONTENT is typically passed as an int (enum value)
    if (Size > 5) {
        int content_type = Data[5] % 4; // 0=VP9E_TUNE_CONTENT_DEFAULT, 1=VP9E_TUNE_CONTENT_SCREEN, etc.
        vpx_codec_control_(&ctx, VP9E_SET_TUNE_CONTENT, content_type);
    }

    // Try to trigger VP8D_GET_LAST_REF_UPDATES (Decoder control, might fail on encoder)
    // This is a decoder control, but we try it on encoder to test error handling paths
    if (Size > 6) {
        vpx_codec_control_(&ctx, VP8D_GET_LAST_REF_UPDATES, 0);
    }

    // Try random control IDs to test the dispatch logic and error handling
    if (Size > 7) {
        int random_ctrl = Data[7];
        vpx_codec_control_(&ctx, random_ctrl, 0);
    }

    // 4. Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}