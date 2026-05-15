// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
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
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
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

static void init_random_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Initialize with some reasonable defaults
    cfg->g_w = 320;
    cfg->g_h = 240;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 256;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 56;
    cfg->g_error_resilient = 0;
    cfg->g_lag_in_frames = 0;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->rc_end_usage = VPX_VBR;
    
    // Use fuzzer data to modify some parameters
    if (size > 0) {
        size_t offset = 0;
        
        if (size >= sizeof(unsigned int)) {
            cfg->g_w = *(const unsigned int*)(data + offset) % 1920 + 16;
            offset += sizeof(unsigned int);
        }
        
        if (size - offset >= sizeof(unsigned int)) {
            cfg->g_h = *(const unsigned int*)(data + offset) % 1080 + 16;
            offset += sizeof(unsigned int);
        }
        
        if (size - offset >= sizeof(unsigned int)) {
            cfg->rc_target_bitrate = *(const unsigned int*)(data + offset) % 10000 + 100;
            offset += sizeof(unsigned int);
        }
        
        if (size - offset >= sizeof(unsigned int)) {
            cfg->g_threads = *(const unsigned int*)(data + offset) % 8;
            offset += sizeof(unsigned int);
        }
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    vpx_codec_iface_t *iface_vp8 = NULL;
    vpx_codec_iface_t *iface_vp9 = NULL;
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    
    // Test vpx_codec_vp8_cx
    iface_vp8 = vpx_codec_vp8_cx();
    if (!iface_vp8) {
        return 0;
    }
    
    // Test vpx_codec_vp9_cx
    iface_vp9 = vpx_codec_vp9_cx();
    if (!iface_vp9) {
        return 0;
    }
    
    // Test vpx_codec_get_caps for both interfaces
    vpx_codec_caps_t caps_vp8 = vpx_codec_get_caps(iface_vp8);
    vpx_codec_caps_t caps_vp9 = vpx_codec_get_caps(iface_vp9);
    (void)caps_vp8;
    (void)caps_vp9;
    
    // Test vpx_codec_enc_config_default for VP8
    res = vpx_codec_enc_config_default(iface_vp8, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    // Modify config with fuzzer data
    init_random_config(&cfg, Data, Size);
    
    // Test vpx_codec_enc_init_ver for VP8
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    res = vpx_codec_enc_init_ver(&codec_ctx, iface_vp8, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res == VPX_CODEC_OK) {
        vpx_codec_destroy(&codec_ctx);
    }
    
    // Test vpx_codec_enc_config_default for VP9
    res = vpx_codec_enc_config_default(iface_vp9, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    // Modify config with fuzzer data again
    init_random_config(&cfg, Data, Size);
    
    // Test vpx_codec_enc_init_ver for VP9
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    res = vpx_codec_enc_init_ver(&codec_ctx, iface_vp9, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res == VPX_CODEC_OK) {
        vpx_codec_destroy(&codec_ctx);
    }
    
    // Test vpx_codec_enc_init_multi_ver for VP8 (multi-encoder only supported by VP8)
    if (Size > 0) {
        const int num_enc = (Data[0] % 3) + 1; // 1-3 encoders
        vpx_codec_ctx_t ctx_array[3];
        vpx_codec_enc_cfg_t cfg_array[3];
        vpx_rational_t dsf_array[3];
        
        for (int i = 0; i < num_enc; i++) {
            memset(&ctx_array[i], 0, sizeof(vpx_codec_ctx_t));
            res = vpx_codec_enc_config_default(iface_vp8, &cfg_array[i], 0);
            if (res != VPX_CODEC_OK) {
                continue;
            }
            init_random_config(&cfg_array[i], Data, Size);
            
            // Set down-sampling factors
            dsf_array[i].num = 1;
            dsf_array[i].den = (i + 1);
        }
        
        res = vpx_codec_enc_init_multi_ver(ctx_array, iface_vp8, cfg_array, num_enc, 0, dsf_array, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            for (int i = 0; i < num_enc; i++) {
                vpx_codec_destroy(&ctx_array[i]);
            }
        }
    }
    
    return 0;
}