// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* 
 * Include the necessary libvpx headers. 
 */
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/* 
 * Fuzz driver for libvpx focusing on vpx_codec_control_.
 * We initialize a VP9 encoder context and then attempt to send various control
 * commands, including those that might correspond to the missing macros.
 */

static int init_encoder(vpx_codec_ctx_t *ctx, vpx_codec_enc_cfg_t *cfg) {
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 1;

    if (vpx_codec_enc_config_default(cfg, iface, 0) != VPX_CODEC_OK) return 1;
    if (vpx_codec_enc_init(ctx, iface, cfg, 0) != VPX_CODEC_OK) return 1;
    return 0;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx = {0};
    vpx_codec_enc_cfg_t cfg = {0};

    // Initialize configuration with some defaults
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;
    cfg.g_lag_in_frames = 0;

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Use the input data to determine the control ID and arguments
    int ctrl_id = (int)Data[0];
    
    // Map raw byte to known libvpx control IDs to increase coverage of valid paths
    // while still allowing fuzzing of invalid IDs.
    switch (ctrl_id % 10) {
        case 0: ctrl_id = VP9E_SET_DISABLE_LOOPFILTER; break;
        case 1: ctrl_id = VP9E_GET_LOOPFILTER_LEVEL; break;
        case 2: ctrl_id = VP9E_SET_RENDER_SIZE; break;
        case 3: ctrl_id = VP9E_SET_SVC; break;
        case 4: ctrl_id = VP9_SET_SKIP_LOOP_FILTER; break;
        // Using VP8E equivalents or generic valid control IDs where VP9 specific ones are ambiguous or missing in this context
        case 5: ctrl_id = VP8E_SET_CPUUSED; break; 
        case 6: ctrl_id = VP8E_SET_TUNING; break; // Replaced VP8E_SET_QMIN
        case 7: ctrl_id = VP8E_SET_CPUUSED; break; // Replaced VP8E_SET_QMAX with valid ID
        case 8: ctrl_id = VP8E_SET_CQ_LEVEL; break;
        case 9: ctrl_id = VP8E_SET_CPUUSED; break; // Replaced VPX_CODEC_SET_CPUUSED
        default: ctrl_id = ctrl_id; // Keep raw value to test invalid IDs
    }

    // Prepare arguments based on remaining data
    int arg1 = 0;
    int arg2 = 0;
    int arg_count = 0;

    if (Size > 1) {
        arg1 = (int)Data[1];
        arg_count = 1;
    }
    if (Size > 2) {
        arg2 = (int)Data[2];
        arg_count = 2;
    }

    // Fuzzing strategy: Call vpx_codec_control_ with different argument counts
    vpx_codec_err_t res = VPX_CODEC_ERROR;

    // Try calling with 0 extra args (some controls take no args)
    if (arg_count == 0) {
        res = vpx_codec_control_(&ctx, ctrl_id);
    } 
    else if (arg_count >= 1) {
        // Try with 1 arg
        res = vpx_codec_control_(&ctx, ctrl_id, arg1);
        
        // If we have a second arg, try calling with 2 args (for controls like SET_RENDER_SIZE)
        if (arg_count >= 2) {
            res = vpx_codec_control_(&ctx, ctrl_id, arg1, arg2);
        }
    }

    cleanup_encoder(&ctx);

    return 0;
}