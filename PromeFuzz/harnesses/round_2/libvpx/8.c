// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"

#define MAX_ENCODERS 4
#define MAX_WIDTH 4096
#define MAX_HEIGHT 4096

static void init_config_with_fuzz(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Initialize with some reasonable defaults
    cfg->g_w = 320;
    cfg->g_h = 240;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 256;
    cfg->g_threads = 0;
    cfg->g_profile = 0;
    cfg->g_bit_depth = VPX_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->rc_end_usage = VPX_VBR;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 9999;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 56;
    
    // Use fuzz data to modify some parameters if we have enough data
    if (size >= 16) {
        // Use first bytes to set width/height (clamped to reasonable values)
        cfg->g_w = (data[0] | (data[1] << 8)) % MAX_WIDTH;
        if (cfg->g_w < 16) cfg->g_w = 16;
        
        cfg->g_h = (data[2] | (data[3] << 8)) % MAX_HEIGHT;
        if (cfg->g_h < 16) cfg->g_h = 16;
        
        cfg->rc_target_bitrate = (data[4] | (data[5] << 8)) % 10000;
        if (cfg->rc_target_bitrate < 100) cfg->rc_target_bitrate = 100;
        
        cfg->g_threads = data[6] % 8;
        cfg->g_profile = data[7] % 4;
        
        // Use more data for other parameters if available
        if (size >= 32) {
            cfg->rc_min_quantizer = data[8] % 64;
            cfg->rc_max_quantizer = data[9] % 64;
            if (cfg->rc_min_quantizer > cfg->rc_max_quantizer) {
                unsigned int tmp = cfg->rc_min_quantizer;
                cfg->rc_min_quantizer = cfg->rc_max_quantizer;
                cfg->rc_max_quantizer = tmp;
            }
            
            cfg->g_lag_in_frames = data[10] % 16;
            cfg->kf_min_dist = data[11] | (data[12] << 8);
            cfg->kf_max_dist = data[13] | (data[14] << 8);
            
            // Set some flags based on fuzz data
            cfg->g_error_resilient = data[15] & 0x3;
        }
    }
}

static int is_vp8_interface(vpx_codec_iface_t *iface) {
    // Get VP8 interface pointer for comparison
    vpx_codec_iface_t *vp8_iface = vpx_codec_vp8_cx();
    return (iface == vp8_iface);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }
    
    // Use fuzz data to decide which codec to test
    vpx_codec_iface_t *iface;
    if (data[0] % 2 == 0) {
        iface = vpx_codec_vp8_cx();
    } else {
        iface = vpx_codec_vp9_cx();
    }
    
    if (!iface) {
        return 0;
    }
    
    // Test 1: Get default configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    // Modify configuration with fuzz data
    init_config_with_fuzz(&cfg, data, size);
    
    // Test 2: Initialize single encoder
    vpx_codec_ctx_t codec;
    res = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    if (res == VPX_CODEC_OK) {
        // Test 3: Destroy the encoder
        vpx_codec_destroy(&codec);
    }
    
    // Test 4: Test multi-encoder initialization (VP8 only)
    if (is_vp8_interface(iface)) {
        // Limit number of encoders to avoid excessive memory usage
        int num_enc = (size > 1 ? data[1] % MAX_ENCODERS : 1);
        if (num_enc < 1) num_enc = 1;
        if (num_enc > MAX_ENCODERS) num_enc = MAX_ENCODERS;
        
        vpx_codec_ctx_t ctx[MAX_ENCODERS];
        vpx_codec_enc_cfg_t cfgs[MAX_ENCODERS];
        vpx_rational_t dsf[MAX_ENCODERS];
        
        // Initialize configurations and downsampling factors
        for (int i = 0; i < num_enc; i++) {
            vpx_codec_enc_config_default(iface, &cfgs[i], 0);
            init_config_with_fuzz(&cfgs[i], data + i * 8, size > i * 8 ? size - i * 8 : 0);
            
            // Set downsampling factors
            dsf[i].num = 1;
            dsf[i].den = 1;
            if (size > i * 2 + 2) {
                dsf[i].num = 1 + (data[i * 2 + 2] % 4);
                dsf[i].den = 1 + (data[i * 2 + 3] % 4);
                if (dsf[i].den == 0) dsf[i].den = 1;
            }
        }
        
        // Initialize multiple encoders
        res = vpx_codec_enc_init_multi_ver(ctx, iface, cfgs, num_enc, 0, dsf, VPX_ENCODER_ABI_VERSION);
        
        // Destroy all initialized encoders
        for (int i = 0; i < num_enc; i++) {
            if (ctx[i].err == VPX_CODEC_OK || ctx[i].priv != NULL) {
                vpx_codec_destroy(&ctx[i]);
            }
        }
    }
    
    // Test 5: Test with different flags
    if (size > 2) {
        vpx_codec_flags_t flags = 0;
        // Set some flags based on fuzz data
        if (data[2] & 0x01) flags |= VPX_CODEC_USE_PSNR;
        if (data[2] & 0x02) flags |= VPX_CODEC_USE_OUTPUT_PARTITION;
        
        vpx_codec_ctx_t codec2;
        res = vpx_codec_enc_init_ver(&codec2, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
        
        if (res == VPX_CODEC_OK) {
            vpx_codec_destroy(&codec2);
        }
    }
    
    return 0;
}