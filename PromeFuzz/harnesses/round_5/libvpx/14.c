// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"

static void initialize_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Initialize with reasonable defaults
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 256;
    cfg->g_profile = 0;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;
    cfg->g_lag_in_frames = 0;
    cfg->rc_end_usage = VPX_VBR;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 56;
    
    // Use fuzz data to modify some parameters
    if (size > 0) {
        cfg->g_w = 16 + (data[0] % 128);  // 16-143
        cfg->g_h = 16 + (data[1 % size] % 128);
        cfg->rc_target_bitrate = 64 + (data[2 % size] * 4);
        cfg->g_profile = data[3 % size] % 4;
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Test vpx_codec_vp9_cx
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;
    
    // Initialize encoder context
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    // Get default configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Modify config with fuzz data
    initialize_config(&cfg, Data, Size);
    
    // Test vpx_codec_enc_init_ver
    vpx_codec_err_t init_result = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    if (init_result == VPX_CODEC_OK) {
        // Test vpx_codec_get_global_headers
        vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec);
        (void)global_headers; // Use result to avoid unused variable warning
        
        // Test vpx_codec_enc_config_set with modified config
        if (Size > 8) {
            cfg.rc_target_bitrate = 128 + (Data[4] * 2);
            cfg.g_w = 32 + (Data[5] % 96);
            cfg.g_h = 32 + (Data[6] % 96);
            vpx_codec_enc_config_set(&codec, &cfg);
        }
        
        // Test vpx_codec_get_cx_data (even though we haven't encoded anything)
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Just iterate through packets
            (void)pkt;
        }
        
        // Clean up
        vpx_codec_destroy(&codec);
    }
    
    // Test vpx_codec_enc_init_multi_ver (VP8 only)
    // First get VP8 interface
    vpx_codec_iface_t *vp8_iface = vpx_codec_vp8_cx();
    if (vp8_iface && Size > 16) {
        const int num_enc = 2;
        vpx_codec_ctx_t ctx_array[2];
        vpx_codec_enc_cfg_t cfg_array[2];
        vpx_rational_t dsf[2] = {{1, 1}, {2, 2}};
        
        // Initialize configs
        for (int i = 0; i < num_enc; i++) {
            if (vpx_codec_enc_config_default(vp8_iface, &cfg_array[i], 0) == VPX_CODEC_OK) {
                cfg_array[i].g_w = 32 + (Data[7 + i] % 64);
                cfg_array[i].g_h = 32 + (Data[8 + i] % 64);
                cfg_array[i].rc_target_bitrate = 128 + (Data[9 + i] * 2);
            }
        }
        
        // Test multi-encoder initialization
        vpx_codec_err_t multi_result = vpx_codec_enc_init_multi_ver(
            ctx_array, vp8_iface, cfg_array, num_enc, 0, dsf, VPX_ENCODER_ABI_VERSION);
        
        if (multi_result == VPX_CODEC_OK) {
            // Clean up multi encoders
            for (int i = 0; i < num_enc; i++) {
                vpx_codec_destroy(&ctx_array[i]);
            }
        }
    }
    
    return 0;
}