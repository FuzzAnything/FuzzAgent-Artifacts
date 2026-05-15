// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_init_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (size < sizeof(vpx_codec_enc_cfg_t)) {
        // Use default values if not enough data
        cfg->g_w = 64;
        cfg->g_h = 64;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->rc_target_bitrate = 256;
        cfg->g_threads = 1;
        cfg->g_profile = 0;
        cfg->rc_min_quantizer = 0;
        cfg->rc_max_quantizer = 63;
        cfg->g_pass = VPX_RC_ONE_PASS;
        cfg->g_lag_in_frames = 0;
        cfg->kf_mode = VPX_KF_AUTO;
        cfg->kf_min_dist = 0;
        cfg->kf_max_dist = 9999;
    } else {
        // Use fuzzer data to initialize config
        memcpy(cfg, data, sizeof(vpx_codec_enc_cfg_t));
        
        // Ensure some basic constraints
        if (cfg->g_w == 0) cfg->g_w = 64;
        if (cfg->g_h == 0) cfg->g_h = 64;
        if (cfg->g_timebase.den == 0) cfg->g_timebase.den = 30;
        if (cfg->g_timebase.num == 0) cfg->g_timebase.num = 1;
        if (cfg->g_threads > 16) cfg->g_threads = 16;
        if (cfg->rc_max_quantizer > 63) cfg->rc_max_quantizer = 63;
        if (cfg->rc_min_quantizer > cfg->rc_max_quantizer) 
            cfg->rc_min_quantizer = cfg->rc_max_quantizer;
    }
}

static void fuzz_init_rational(vpx_rational_t *dsf, const uint8_t *data, size_t offset) {
    if (offset + sizeof(vpx_rational_t) <= 256) { // Arbitrary limit
        dsf->num = 1;
        dsf->den = 1;
    } else {
        dsf->num = 1;
        dsf->den = 1;
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;
    
    // Test 1: vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) return 0;
    
    // Modify config with fuzzer data
    fuzz_init_config(&cfg, Data, Size);
    
    // Test 2: vpx_codec_enc_init_ver (single encoder)
    vpx_codec_ctx_t ctx;
    err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    if (err == VPX_CODEC_OK) {
        // Test 5: vpx_codec_get_cx_data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Just iterate through packets
        }
        
        // Test 6: vpx_codec_enc_config_set
        if (Size > sizeof(vpx_codec_enc_cfg_t)) {
            // Create a slightly modified config
            vpx_codec_enc_cfg_t new_cfg = cfg;
            new_cfg.rc_target_bitrate = (cfg.rc_target_bitrate + 100) % 2000;
            new_cfg.g_w = (cfg.g_w + 16) % 256;
            new_cfg.g_h = (cfg.g_h + 16) % 256;
            vpx_codec_enc_config_set(&ctx, &new_cfg);
        }
        
        vpx_codec_destroy(&ctx);
    }
    
    // Test 3: vpx_codec_enc_init_multi_ver (multiple encoders)
    if (Size > 100) { // Ensure enough data for multi-encoder test
        const int num_enc = 2;
        vpx_codec_ctx_t ctx_array[num_enc];
        vpx_codec_enc_cfg_t cfg_array[num_enc];
        vpx_rational_t dsf_array[num_enc];
        
        for (int i = 0; i < num_enc; i++) {
            vpx_codec_enc_config_default(iface, &cfg_array[i], 0);
            fuzz_init_config(&cfg_array[i], Data + i * 20, Size - i * 20);
            fuzz_init_rational(&dsf_array[i], Data, i * sizeof(vpx_rational_t));
            
            // Adjust config for multiple encoders
            cfg_array[i].g_w = 64 + i * 32;
            cfg_array[i].g_h = 64 + i * 32;
            cfg_array[i].rc_target_bitrate = 256 + i * 128;
        }
        
        err = vpx_codec_enc_init_multi_ver(ctx_array, iface, cfg_array, 
                                          num_enc, 0, dsf_array, 
                                          VPX_ENCODER_ABI_VERSION);
        
        if (err == VPX_CODEC_OK) {
            for (int i = 0; i < num_enc; i++) {
                vpx_codec_destroy(&ctx_array[i]);
            }
        }
    }
    
    return 0;
}