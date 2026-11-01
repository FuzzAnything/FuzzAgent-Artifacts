// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
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
#include "vpx/vpx_image.h"

/*
 * Fuzz driver for libvpx encoder initialization and control.
 * Targets: vpx_codec_enc_init_ver, vpx_codec_control_
 * Note: VP9E_SET_* macros are used via vpx_codec_control.
 */

static int fuzz_init_encoder(const uint8_t *data, size_t size, vpx_codec_ctx_t *ctx) {
    vpx_codec_iface_t *iface = NULL;
    
    // Try to get VP9 encoder interface
    // Note: In some builds, the function might be named differently or not exported directly
    // We rely on the standard vpx_codec_vp9_enc() if available, otherwise fallback
    #ifdef vpx_codec_vp9_enc
    iface = vpx_codec_vp9_enc();
    #endif
    
    if (!iface) {
        // Fallback to VP8 if VP9 is not available or not compiled in
        #ifdef vpx_codec_vp8_enc
        iface = vpx_codec_vp8_enc();
        #endif
        if (!iface) {
            return 0;
        }
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters safely
    if (size > 0) {
        cfg.g_w = data[0] ? data[0] : 16;
        cfg.g_h = (size > 1) ? data[1] : 16;
        if (cfg.g_w < 16) cfg.g_w = 16;
        if (cfg.g_h < 16) cfg.g_h = 16;
        
        cfg.g_threads = (size > 2) ? data[2] : 1;
        cfg.g_profile = (size > 3) ? data[3] % 4 : 0;
        cfg.g_bit_depth = (size > 4) ? data[4] : 8;
        if (cfg.g_bit_depth != 8 && cfg.g_bit_depth != 10 && cfg.g_bit_depth != 12) {
            cfg.g_bit_depth = 8;
        }
        
        cfg.rc_target_bitrate = (size > 5) ? (data[5] * 100) : 1000;
        cfg.rc_min_quantizer = (size > 6) ? data[6] : 0;
        cfg.rc_max_quantizer = (size > 7) ? data[7] : 63;
        if (cfg.rc_min_quantizer > cfg.rc_max_quantizer) {
            cfg.rc_min_quantizer = 0;
            cfg.rc_max_quantizer = 63;
        }

        cfg.kf_mode = (size > 8) ? (enum vpx_kf_mode)(data[8] % 3) : VPX_KF_DISABLED;
        cfg.kf_min_dist = (size > 9) ? data[9] : 0;
        cfg.kf_max_dist = (size > 10) ? data[10] : 0;
        
        cfg.g_pass = (size > 11) ? (enum vpx_enc_pass)(data[11] % 3) : VPX_RC_ONE_PASS;
    }

    vpx_codec_flags_t flags = 0;
    if (size > 12) {
        flags = (vpx_codec_flags_t)data[12];
    }

    vpx_codec_err_t err = vpx_codec_enc_init_ver(ctx, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    return 1;
}

static int fuzz_control_encoder(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    // Try various control IDs that are safe to call on an initialized encoder
    // VP9E_SET_TILE_COLUMNS, VP9E_SET_DISABLE_LOOPFILTER, VP9E_SET_TUNE_CONTENT
    // Note: We must ensure the codec is VP9 for VP9 specific controls, but 
    // vpx_codec_control_ will return INCAPABLE if the codec doesn't support it.
    
    int ctrl_id = -1;
    int val = 0;

    if (size > 0) {
        // Select a control ID based on input
        int choice = data[0] % 5;
        switch (choice) {
            case 0: // VP9E_SET_TILE_COLUMNS
                ctrl_id = VP9E_SET_TILE_COLUMNS;
                val = (size > 1) ? data[1] : 1;
                break;
            case 1: // VP9E_SET_DISABLE_LOOPFILTER
                ctrl_id = VP9E_SET_DISABLE_LOOPFILTER;
                val = (size > 1) ? (data[1] % 2) : 0;
                break;
            case 2: // VP9E_SET_TUNE_CONTENT
                ctrl_id = VP9E_SET_TUNE_CONTENT;
                val = (size > 1) ? (data[1] % 3) : 0; // VP9E_CONTENT_DEFAULT, SCREEN, FILM
                break;
            case 3: // VP8E_SET_CPUUSED (Generic fallback)
                ctrl_id = VP8E_SET_CPUUSED;
                val = (size > 1) ? (data[1] % 8) : 0;
                break;
            case 4: // VP8E_SET_STATIC_THRESHOLD (Fixed from VP9E_SET_STATIC_THRESHOLD)
                ctrl_id = VP8E_SET_STATIC_THRESHOLD;
                val = (size > 1) ? data[1] : 0;
                break;
            default:
                return 0;
        }
    } else {
        // Default control if no data
        ctrl_id = VP8E_SET_CPUUSED;
        val = 0;
    }

    vpx_codec_err_t err = vpx_codec_control_(ctx, ctrl_id, val);
    // We don't return error here, we just let the fuzzer explore the state
    (void)err;
    return 1;
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Step 1: Initialize the encoder
    if (!fuzz_init_encoder(Data, Size, &ctx)) {
        // Initialization failed, nothing to do
        return 0;
    }

    // Step 2: Try to invoke control functions to explore states
    // We use a subset of data for control fuzzing to ensure we have enough for init
    fuzz_control_encoder(&ctx, Data, Size);

    // Step 3: Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}