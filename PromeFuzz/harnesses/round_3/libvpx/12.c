// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Initialize with default values
    cfg->g_w = 320;
    cfg->g_h = 240;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_profile = 0;
    cfg->g_bit_depth = VPX_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->rc_target_bitrate = 256;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 56;
    cfg->g_error_resilient = 0;
    cfg->g_lag_in_frames = 0;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 9999;
    cfg->rc_end_usage = VPX_VBR;
    
    // Use fuzzer data to modify some parameters
    if (size > 0) {
        cfg->g_w = 64 + (data[0] % 256);
        cfg->g_h = 64 + (data[1] % 256);
        cfg->rc_target_bitrate = 100 + (data[2] % 1000);
        cfg->g_threads = data[3] % 4;
        cfg->rc_min_quantizer = data[4] % 64;
        cfg->rc_max_quantizer = cfg->rc_min_quantizer + (data[5] % (64 - cfg->rc_min_quantizer));
    }
}

static void initialize_rational(vpx_rational_t *rat, const uint8_t *data, size_t offset) {
    if (offset + 1 < 256) {
        rat->num = 1 + (data[offset] % 10);
        rat->den = 1 + (data[offset + 1] % 10);
    } else {
        rat->num = 1;
        rat->den = 1;
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    
    // Initialize encoder configuration
    initialize_config(&cfg, Data, Size);
    
    // Test vpx_codec_enc_init_ver
    vpx_codec_err_t init_result = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    if (init_result == VPX_CODEC_OK) {
        // Test vpx_codec_enc_config_set with modified configuration
        if (Size > 10) {
            cfg.g_w = 128 + (Data[6] % 128);
            cfg.g_h = 128 + (Data[7] % 128);
            cfg.rc_target_bitrate = 200 + (Data[8] % 800);
            vpx_codec_enc_config_set(&ctx, &cfg);
        }
        
        // Test vpx_codec_control_ with various control IDs
        if (Size > 0) {
            // Use fuzzer data to select control ID
            int ctrl_id = Data[0] % 100;
            int value = Data[1] % 256;
            
            // Try a few common control IDs
            switch (ctrl_id % 5) {
                case 0:
                    vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, value % 9);
                    break;
                case 1:
                    vpx_codec_control_(&ctx, VP8E_SET_ENABLEAUTOALTREF, value & 1);
                    break;
                case 2:
                    vpx_codec_control_(&ctx, VP9E_SET_ROW_MT, value & 1);
                    break;
                case 3:
                    vpx_codec_control_(&ctx, VP8E_SET_STATIC_THRESHOLD, value % 1000);
                    break;
                case 4:
                    vpx_codec_control_(&ctx, VP9E_SET_TILE_COLUMNS, value % 6);
                    break;
            }
        }
        
        // Test vpx_codec_get_cx_data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Just iterate through packets
        }
        
        // Test multi-encoder initialization if we have enough data
        if (Size > 50) {
            const int num_enc = 2;
            vpx_codec_ctx_t multi_ctx[num_enc];
            vpx_codec_enc_cfg_t multi_cfg[num_enc];
            vpx_rational_t dsf[num_enc];
            
            for (int i = 0; i < num_enc; i++) {
                initialize_config(&multi_cfg[i], Data + i * 10, Size - i * 10);
                initialize_rational(&dsf[i], Data, i * 2);
                multi_cfg[i].g_w = 160 + (Data[i * 3] % 160);
                multi_cfg[i].g_h = 120 + (Data[i * 3 + 1] % 120);
            }
            
            vpx_codec_enc_init_multi_ver(multi_ctx, iface, multi_cfg, num_enc, 0, dsf, VPX_ENCODER_ABI_VERSION);
            
            // Clean up multi encoders
            for (int i = 0; i < num_enc; i++) {
                if (multi_ctx[i].err == VPX_CODEC_OK) {
                    vpx_codec_destroy(&multi_ctx[i]);
                }
            }
        }
        
        // Clean up
        vpx_codec_destroy(&ctx);
    }
    
    return 0;
}