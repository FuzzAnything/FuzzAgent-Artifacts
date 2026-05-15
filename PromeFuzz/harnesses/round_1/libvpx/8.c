// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Initialize config with reasonable defaults
    vpx_codec_enc_config_default(vpx_codec_vp9_cx(), cfg, 0);
    
    // Use fuzzer data to modify some parameters
    if (size > 0) {
        cfg->g_w = 64 + (data[0] % 192);  // 64-256 width
        cfg->g_h = 64 + (data[1] % 192);  // 64-256 height
        cfg->rc_target_bitrate = 100 + (data[2] % 900);  // 100-1000 kbps
        cfg->rc_min_quantizer = data[3] % 64;
        cfg->rc_max_quantizer = 10 + (data[4] % 54);
        cfg->g_threads = 1 + (data[5] % 4);
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->g_pass = VPX_RC_ONE_PASS;
        cfg->g_lag_in_frames = data[6] % 17;
        cfg->kf_mode = data[7] % 3;
        cfg->kf_min_dist = data[8] % 100;
        cfg->kf_max_dist = 100 + (data[9] % 900);
    }
}

static void init_rational(vpx_rational_t *rat, const uint8_t *data, size_t offset) {
    if (offset + 1 < 256) {
        rat->num = 1 + (data[offset] % 10);
        rat->den = 1 + (data[offset + 1] % 10);
    } else {
        rat->num = 1;
        rat->den = 1;
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;
    
    // Test vpx_codec_enc_init_ver
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    init_config(&cfg, Data, Size);
    
    vpx_codec_err_t err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Test vpx_codec_control_ with various control IDs
    int ctrl_id = Data[0] % 100;  // Sample some control IDs
    int int_arg = (int)Data[1];
    void *ptr_arg = NULL;
    
    // Try different control types based on fuzzer data
    switch (ctrl_id % 4) {
        case 0:
            vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, int_arg);
            break;
        case 1:
            vpx_codec_control_(&ctx, VP8E_SET_NOISE_SENSITIVITY, int_arg % 7);
            break;
        case 2:
            vpx_codec_control_(&ctx, VP8E_SET_STATIC_THRESHOLD, int_arg % 1000);
            break;
        case 3:
            vpx_codec_control_(&ctx, VP9E_SET_TILE_COLUMNS, int_arg % 6);
            break;
    }
    
    // Test vpx_codec_enc_config_set with modified config
    if (Size > 20) {
        cfg.rc_target_bitrate = 200 + (Data[10] % 800);
        cfg.g_w = 128 + (Data[11] % 128);
        cfg.g_h = 128 + (Data[12] % 128);
        vpx_codec_enc_config_set(&ctx, &cfg);
    }
    
    // Test vpx_codec_get_cx_data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Just iterate through packets
    }
    
    // Test vpx_codec_enc_init_multi_ver if we have enough data
    if (Size > 100) {
        vpx_codec_ctx_t multi_ctx[2];
        vpx_codec_enc_cfg_t multi_cfg[2];
        vpx_rational_t dsf[2];
        
        for (int i = 0; i < 2; i++) {
            init_config(&multi_cfg[i], Data + i * 20, Size - i * 20);
            init_rational(&dsf[i], Data + 50 + i * 2, i * 2);
            dsf[i].num = 1;
            dsf[i].den = 1;
        }
        
        err = vpx_codec_enc_init_multi_ver(multi_ctx, iface, multi_cfg, 2, 0, dsf, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
            // Test control on multi encoder
            vpx_codec_control_(&multi_ctx[0], VP8E_SET_CPUUSED, Data[15] % 10);
            
            // Clean up multi encoders
            for (int i = 0; i < 2; i++) {
                vpx_codec_destroy(&multi_ctx[i]);
            }
        }
    }
    
    // Test more control operations
    if (Size > 30) {
        vpx_codec_control_(&ctx, VP8E_SET_ENABLEAUTOALTREF, Data[20] & 1);
        vpx_codec_control_(&ctx, VP8E_SET_ARNR_MAXFRAMES, Data[21] % 10);
        vpx_codec_control_(&ctx, VP8E_SET_ARNR_STRENGTH, Data[22] % 7);
        vpx_codec_control_(&ctx, VP8E_SET_ARNR_TYPE, Data[23] % 3);
    }
    
    // Test with different flags
    if (Size > 40) {
        vpx_codec_ctx_t ctx2;
        vpx_codec_enc_cfg_t cfg2;
        init_config(&cfg2, Data + 30, Size - 30);
        
        vpx_codec_flags_t flags = 0;
        if (Data[31] & 1) flags |= VPX_CODEC_USE_PSNR;
        if (Data[31] & 2) flags |= VPX_CODEC_USE_OUTPUT_PARTITION;
        
        err = vpx_codec_enc_init_ver(&ctx2, iface, &cfg2, flags, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
            vpx_codec_control_(&ctx2, VP9E_SET_ROW_MT, Data[32] & 1);
            vpx_codec_destroy(&ctx2);
        }
    }
    
    vpx_codec_destroy(&ctx);
    return 0;
}