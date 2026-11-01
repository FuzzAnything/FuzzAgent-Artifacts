// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

/* 
 * Fuzz driver for libvpx encoder initialization and control APIs.
 * Targets: vpx_codec_enc_init_ver, vpx_codec_control_
 * Note: VP8E_SET_... and VP9E_SET_... are control IDs passed to vpx_codec_control.
 */

static void init_cfg_from_fuzz(vpx_codec_enc_cfg_t *cfg, const uint8_t *Data, size_t Size) {
    memset(cfg, 0, sizeof(*cfg));
    
    /* Use vpx_codec_enc_config_default to set sane defaults based on codec */
    /* We will try both VP8 and VP9 interfaces later, so we initialize generically here */
    /* However, vpx_codec_enc_config_default requires an iface. 
       We will call it inside the fuzzer loop after selecting the interface. */
    
    /* Fuzz some fields directly if we have enough data */
    if (Size > 0) cfg->g_w = Data[0] ? Data[0] : 16;
    if (Size > 1) cfg->g_h = Data[1] ? Data[1] : 16;
    if (Size > 2) cfg->g_threads = Data[2] % 16;
    if (Size > 3) cfg->g_profile = Data[3] % 4;
    if (Size > 4) cfg->g_bit_depth = (vpx_bit_depth_t)(Data[4] % 3); /* 8, 10, 12 */
    if (Size > 5) cfg->rc_target_bitrate = Data[5] * 100 + 100;
    if (Size > 6) cfg->rc_min_quantizer = Data[6] % 64;
    if (Size > 7) cfg->rc_max_quantizer = (Data[7] % 63) + cfg->rc_min_quantizer + 1;
    if (Size > 8) cfg->kf_mode = (enum vpx_kf_mode)(Data[8] % 3);
    if (Size > 9) cfg->kf_min_dist = Data[9] * 10;
    if (Size > 10) cfg->kf_max_dist = (Data[10] * 10) + cfg->kf_min_dist;
    
    /* Timebase */
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = (Size > 11) ? (Data[11] * 10 + 10) : 30;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t res;
    int ctrl_id;
    int arg_val;
    
    /* Select interface based on first byte */
    /* 0: VP8, 1: VP9, others: try VP8 */
    if (Data[0] == 1) {
        iface = vpx_codec_vp9_cx();
    } else {
        iface = vpx_codec_vp8_cx();
    }

    if (!iface) return 0;

    /* Initialize config with defaults for the selected interface */
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    /* Overwrite with fuzzed values */
    init_cfg_from_fuzz(&cfg, Data, Size);

    /* Ensure minimum resolution */
    if (cfg.g_w < 2) cfg.g_w = 16;
    if (cfg.g_h < 2) cfg.g_h = 16;

    /* Initialize the encoder */
    res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    if (res != VPX_CODEC_OK) {
        /* Initialization failed, nothing to clean up per libvpx docs */
        return 0;
    }

    /* 
     * Fuzz control commands.
     * We extract control IDs and arguments from the fuzz input.
     * We focus on the valid control IDs mentioned in the prompt:
     * VP8E_SET_TUNING, VP9E_SET_TUNE_CONTENT, VP8E_SET_SCALEMODE, VP8E_SET_CPUUSED
     */
    
    /* Try to invoke vpx_codec_control_ with various IDs */
    /* We use a loop to try multiple commands if data permits */
    size_t idx = 0;
    while (idx < Size) {
        /* Pick a control ID based on fuzz data */
        /* Map fuzz byte to specific control IDs */
        int selector = Data[idx] % 5;
        
        switch (selector) {
            case 0:
                ctrl_id = VP8E_SET_TUNING;
                /* Tuning value: 0-4 */
                if (idx + 1 < Size) {
                    arg_val = Data[idx + 1] % 5;
                    idx++;
                } else {
                    arg_val = 0;
                }
                res = vpx_codec_control_(&ctx, ctrl_id, arg_val);
                break;
                
            case 1:
                ctrl_id = VP9E_SET_TUNE_CONTENT;
                /* Content type: 0-3 (e.g., VP9E_CONTENT_DEFAULT, SCREEN, etc.) */
                if (idx + 1 < Size) {
                    arg_val = Data[idx + 1] % 4;
                    idx++;
                } else {
                    arg_val = 0;
                }
                res = vpx_codec_control_(&ctx, ctrl_id, arg_val);
                break;
                
            case 2:
                ctrl_id = VP8E_SET_SCALEMODE;
                /* Scaling mode: VPX_SCALING_MODE enum */
                if (idx + 1 < Size) {
                    arg_val = Data[idx + 1] % 4;
                    idx++;
                } else {
                    arg_val = 0;
                }
                res = vpx_codec_control_(&ctx, ctrl_id, arg_val);
                break;
                
            case 3:
                ctrl_id = VP8E_SET_CPUUSED;
                /* CPU usage: 0-8 */
                if (idx + 1 < Size) {
                    arg_val = Data[idx + 1] % 9;
                    idx++;
                } else {
                    arg_val = 0;
                }
                res = vpx_codec_control_(&ctx, ctrl_id, arg_val);
                break;
                
            default:
                /* Random invalid control ID to test error handling */
                ctrl_id = (int)Data[idx] + 0x1000;
                arg_val = 0;
                res = vpx_codec_control_(&ctx, ctrl_id, arg_val);
                break;
        }
        
        idx++;
        
        /* If a control call fails with an error that might leave state inconsistent 
           (though libvpx is generally robust), we could break or continue. 
           Here we continue to explore more states. */
        if (res == VPX_CODEC_ERROR || res == VPX_CODEC_INVALID_PARAM) {
            /* Continue to next command */
        }
    }

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}