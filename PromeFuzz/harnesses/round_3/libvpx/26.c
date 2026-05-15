// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"

static void init_config_from_fuzz(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Initialize with some reasonable defaults
    cfg->g_w = 640;
    cfg->g_h = 480;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 256;
    cfg->g_threads = 1;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->rc_end_usage = VPX_VBR;
    cfg->g_error_resilient = 0;
    cfg->g_lag_in_frames = 0;
    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 9999;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 56;
    
    // Use fuzz data to modify some parameters if we have enough data
    if (size >= 16) {
        cfg->g_w = (data[0] << 8) | data[1];
        cfg->g_h = (data[2] << 8) | data[3];
        if (cfg->g_w == 0) cfg->g_w = 1;
        if (cfg->g_h == 0) cfg->g_h = 1;
        if (cfg->g_w > 4096) cfg->g_w = 4096;
        if (cfg->g_h > 4096) cfg->g_h = 4096;
        
        cfg->rc_target_bitrate = (data[4] << 8) | data[5];
        cfg->g_threads = data[6] % 8;
        cfg->rc_min_quantizer = data[7] % 64;
        cfg->rc_max_quantizer = data[8] % 64;
        if (cfg->rc_max_quantizer < cfg->rc_min_quantizer) {
            cfg->rc_max_quantizer = cfg->rc_min_quantizer;
        }
        
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 1 + (data[9] % 120);
        cfg->g_lag_in_frames = data[10] % 16;
        cfg->kf_min_dist = data[11] % 100;
        cfg->kf_max_dist = data[12] % 1000;
        if (cfg->kf_max_dist < cfg->kf_min_dist) {
            cfg->kf_max_dist = cfg->kf_min_dist;
        }
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }
    
    // Test vpx_codec_vp8_cx and vpx_codec_vp9_cx
    vpx_codec_iface_t *vp8_iface = vpx_codec_vp8_cx();
    vpx_codec_iface_t *vp9_iface = vpx_codec_vp9_cx();
    
    if (!vp8_iface || !vp9_iface) {
        return 0;
    }
    
    // Test vpx_codec_get_caps
    vpx_codec_caps_t caps_vp8 = vpx_codec_get_caps(vp8_iface);
    vpx_codec_caps_t caps_vp9 = vpx_codec_get_caps(vp9_iface);
    
    // Test vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg_vp8, cfg_vp9;
    vpx_codec_err_t err;
    
    err = vpx_codec_enc_config_default(vp8_iface, &cfg_vp8, 0);
    if (err != VPX_CODEC_OK) {
        // Handle error
    }
    
    err = vpx_codec_enc_config_default(vp9_iface, &cfg_vp9, 0);
    if (err != VPX_CODEC_OK) {
        // Handle error
    }
    
    // Modify configs with fuzz data
    init_config_from_fuzz(&cfg_vp8, data, size);
    init_config_from_fuzz(&cfg_vp9, data, size);
    
    // Test vpx_codec_enc_init_ver for VP8
    vpx_codec_ctx_t ctx_vp8;
    vpx_codec_flags_t flags = 0;
    
    if (size > 0 && (data[0] & 1)) {
        flags |= VPX_CODEC_USE_PSNR;
    }
    if (size > 1 && (data[1] & 1)) {
        flags |= VPX_CODEC_USE_OUTPUT_PARTITION;
    }
    
    err = vpx_codec_enc_init_ver(&ctx_vp8, vp8_iface, &cfg_vp8, flags, VPX_ENCODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        // Clean up if initialization succeeded
        vpx_codec_destroy(&ctx_vp8);
    }
    
    // Test vpx_codec_enc_init_ver for VP9
    vpx_codec_ctx_t ctx_vp9;
    err = vpx_codec_enc_init_ver(&ctx_vp9, vp9_iface, &cfg_vp9, flags, VPX_ENCODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx_vp9);
    }
    
    // Test vpx_codec_enc_init_multi_ver for VP8 (only supported by VP8)
    if (size >= 4) {
        int num_enc = 1 + (data[2] % 4); // 1-4 encoders
        vpx_codec_ctx_t *multi_ctx = calloc(num_enc, sizeof(vpx_codec_ctx_t));
        vpx_codec_enc_cfg_t *multi_cfg = calloc(num_enc, sizeof(vpx_codec_enc_cfg_t));
        vpx_rational_t *dsf = calloc(num_enc, sizeof(vpx_rational_t));
        
        if (multi_ctx && multi_cfg && dsf) {
            for (int i = 0; i < num_enc; i++) {
                err = vpx_codec_enc_config_default(vp8_iface, &multi_cfg[i], 0);
                if (err != VPX_CODEC_OK) {
                    break;
                }
                init_config_from_fuzz(&multi_cfg[i], data + (i * 8) % size, 
                                     size - (i * 8) % size);
                
                // Set downsampling factors
                dsf[i].num = 1;
                dsf[i].den = 1 + (i % 4);
            }
            
            if (err == VPX_CODEC_OK) {
                err = vpx_codec_enc_init_multi_ver(multi_ctx, vp8_iface, multi_cfg, 
                                                   num_enc, flags, dsf, VPX_ENCODER_ABI_VERSION);
                
                // Clean up if initialization succeeded
                if (err == VPX_CODEC_OK) {
                    for (int i = 0; i < num_enc; i++) {
                        vpx_codec_destroy(&multi_ctx[i]);
                    }
                }
            }
        }
        
        free(multi_ctx);
        free(multi_cfg);
        free(dsf);
    }
    
    return 0;
}