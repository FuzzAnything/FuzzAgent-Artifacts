// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"

static void fuzz_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    // Use fuzz data to modify some configuration parameters
    cfg->g_w = 64 + (data[0] % 256);  // Width between 64-319
    cfg->g_h = 64 + (data[1] % 256);  // Height between 64-319
    cfg->rc_target_bitrate = 100 + (data[2] % 1000);  // Bitrate between 100-1099
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30 + (data[3] % 60);  // Framerate between 30-89
    
    // Set some reasonable bounds
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
    cfg->g_error_resilient = 0;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    
    // Test vpx_codec_vp8_cx and vpx_codec_vp9_cx
    vpx_codec_iface_t *vp8_iface = vpx_codec_vp8_cx();
    vpx_codec_iface_t *vp9_iface = vpx_codec_vp9_cx();
    
    if (!vp8_iface || !vp9_iface) return 0;
    
    // Test vpx_codec_get_caps
    vpx_codec_caps_t vp8_caps = vpx_codec_get_caps(vp8_iface);
    vpx_codec_caps_t vp9_caps = vpx_codec_get_caps(vp9_iface);
    (void)vp8_caps;  // Mark as used
    (void)vp9_caps;  // Mark as used
    
    // Test single encoder initialization for VP8
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    
    // Get default config for VP8
    if (vpx_codec_enc_config_default(vp8_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Fuzz the configuration
    fuzz_config(&cfg, data, size);
    
    // Initialize VP8 encoder
    vpx_codec_err_t err = vpx_codec_enc_init_ver(&ctx, vp8_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
    }
    
    // Test single encoder initialization for VP9
    if (vpx_codec_enc_config_default(vp9_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    fuzz_config(&cfg, data + 4, size > 4 ? size - 4 : 0);
    
    err = vpx_codec_enc_init_ver(&ctx, vp9_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
    }
    
    // Test multi-encoder initialization (VP8 only as per documentation)
    if (size >= 8) {
        const int num_enc = 1 + (data[4] % 3);  // 1-3 encoders
        vpx_codec_ctx_t *ctx_array = (vpx_codec_ctx_t*)malloc(num_enc * sizeof(vpx_codec_ctx_t));
        vpx_codec_enc_cfg_t *cfg_array = (vpx_codec_enc_cfg_t*)malloc(num_enc * sizeof(vpx_codec_enc_cfg_t));
        vpx_rational_t *dsf_array = (vpx_rational_t*)malloc(num_enc * sizeof(vpx_rational_t));
        
        if (ctx_array && cfg_array && dsf_array) {
            for (int i = 0; i < num_enc; i++) {
                if (vpx_codec_enc_config_default(vp8_iface, &cfg_array[i], 0) != VPX_CODEC_OK) {
                    free(ctx_array);
                    free(cfg_array);
                    free(dsf_array);
                    return 0;
                }
                fuzz_config(&cfg_array[i], data + i * 2, size > i * 2 ? size - i * 2 : 0);
                dsf_array[i].num = 1;
                dsf_array[i].den = 1 + (i % 3);  // 1, 2, or 3
            }
            
            err = vpx_codec_enc_init_multi_ver(ctx_array, vp8_iface, cfg_array, num_enc, 0, dsf_array, VPX_ENCODER_ABI_VERSION);
            
            // Clean up initialized contexts
            for (int i = 0; i < num_enc; i++) {
                if (ctx_array[i].err == VPX_CODEC_OK) {
                    vpx_codec_destroy(&ctx_array[i]);
                }
            }
            
            free(ctx_array);
            free(cfg_array);
            free(dsf_array);
        } else {
            free(ctx_array);
            free(cfg_array);
            free(dsf_array);
        }
    }
    
    return 0;
}