// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (size < sizeof(vpx_codec_enc_cfg_t)) {
        // Use minimal valid configuration if data is insufficient
        cfg->g_w = 64;
        cfg->g_h = 64;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->rc_target_bitrate = 256;
        cfg->g_error_resilient = 0;
        cfg->g_pass = VPX_RC_ONE_PASS;
        cfg->g_lag_in_frames = 0;
        cfg->rc_min_quantizer = 2;
        cfg->rc_max_quantizer = 56;
        cfg->kf_mode = VPX_KF_AUTO;
        cfg->kf_min_dist = 0;
        cfg->kf_max_dist = 9999;
        cfg->g_profile = 0;
        cfg->g_threads = 1;
        cfg->g_bit_depth = VPX_BITS_8;
        cfg->g_input_bit_depth = 8;
        return;
    }
    
    // Copy configuration from fuzzer data
    memcpy(cfg, data, sizeof(vpx_codec_enc_cfg_t));
    
    // Ensure basic validity
    if (cfg->g_w == 0) cfg->g_w = 64;
    if (cfg->g_h == 0) cfg->g_h = 64;
    if (cfg->g_timebase.den == 0) cfg->g_timebase.den = 30;
    if (cfg->g_timebase.num == 0) cfg->g_timebase.num = 1;
    if (cfg->rc_target_bitrate == 0) cfg->rc_target_bitrate = 256;
    if (cfg->rc_min_quantizer > 63) cfg->rc_min_quantizer = 2;
    if (cfg->rc_max_quantizer > 63) cfg->rc_max_quantizer = 56;
    if (cfg->kf_min_dist > 9999) cfg->kf_min_dist = 0;
    if (cfg->kf_max_dist > 9999) cfg->kf_max_dist = 9999;
    if (cfg->g_threads > 64) cfg->g_threads = 1;
}

static void init_rational(vpx_rational_t *rat, const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + sizeof(vpx_rational_t) <= size) {
        memcpy(rat, data + *offset, sizeof(vpx_rational_t));
        *offset += sizeof(vpx_rational_t);
    } else {
        rat->num = 1;
        rat->den = 1;
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Get VP9 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;
    
    // Initialize encoder configuration
    vpx_codec_enc_cfg_t cfg;
    size_t offset = 0;
    init_config(&cfg, Data, Size);
    offset += sizeof(vpx_codec_enc_cfg_t);
    
    // Initialize encoder context
    vpx_codec_ctx_t ctx;
    vpx_codec_err_t err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Try with minimal configuration
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 256;
        err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) return 0;
    }
    
    // Test vpx_codec_control_ with various control IDs
    if (Size >= offset + 4) {
        uint32_t ctrl_id = *(uint32_t*)(Data + offset);
        offset += 4;
        
        // Test a few common control IDs
        int value = 0;
        if (Size >= offset + 4) {
            value = *(int*)(Data + offset);
            offset += 4;
        }
        
        // Try different control IDs based on fuzzer input
        switch (ctrl_id % 10) {
            case 0:
                vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, value % 9);
                break;
            case 1:
                vpx_codec_control_(&ctx, VP8E_SET_ENABLEAUTOALTREF, value & 1);
                break;
            case 2:
                vpx_codec_control_(&ctx, VP8E_SET_ARNR_MAXFRAMES, value % 5);
                break;
            case 3:
                vpx_codec_control_(&ctx, VP8E_SET_ARNR_STRENGTH, value % 7);
                break;
            case 4:
                vpx_codec_control_(&ctx, VP8E_SET_ARNR_TYPE, value & 1);
                break;
            case 5:
                vpx_codec_control_(&ctx, VP9E_SET_TILE_COLUMNS, value % 6);
                break;
            case 6:
                vpx_codec_control_(&ctx, VP9E_SET_TILE_ROWS, value % 6);
                break;
            case 7:
                vpx_codec_control_(&ctx, VP9E_SET_ROW_MT, value & 1);
                break;
            case 8:
                vpx_codec_control_(&ctx, VP9E_SET_LOSSLESS, value & 1);
                break;
            case 9:
                vpx_codec_control_(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, value & 1);
                break;
        }
    }
    
    // Test vpx_codec_get_cx_data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Just iterate through packets
    }
    
    // Test vpx_codec_enc_config_set with modified configuration
    if (Size >= offset + sizeof(vpx_codec_enc_cfg_t)) {
        vpx_codec_enc_cfg_t new_cfg;
        memcpy(&new_cfg, Data + offset, sizeof(vpx_codec_enc_cfg_t));
        offset += sizeof(vpx_codec_enc_cfg_t);
        
        // Ensure basic validity
        if (new_cfg.g_w == 0) new_cfg.g_w = cfg.g_w;
        if (new_cfg.g_h == 0) new_cfg.g_h = cfg.g_h;
        if (new_cfg.g_timebase.den == 0) new_cfg.g_timebase.den = cfg.g_timebase.den;
        if (new_cfg.g_timebase.num == 0) new_cfg.g_timebase.num = cfg.g_timebase.num;
        
        vpx_codec_enc_config_set(&ctx, &new_cfg);
    }
    
    // Test vpx_codec_enc_init_multi_ver for VP8 (if available)
    // Note: This is only supported for VP8, not VP9
    // We'll test it with minimal configuration
    
    // Clean up
    vpx_codec_destroy(&ctx);
    
    return 0;
}