// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
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

static void init_random_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size, size_t *offset) {
    if (size == 0) return;
    
    // Use data to initialize some fields pseudo-randomly
    size_t local_offset = *offset;
    
    cfg->g_w = 64 + (data[local_offset % size] % 1920);
    local_offset++;
    cfg->g_h = 64 + (data[local_offset % size] % 1080);
    local_offset++;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30 + (data[local_offset % size] % 60);
    local_offset++;
    cfg->rc_target_bitrate = 100 + (data[local_offset % size] % 5000);
    local_offset++;
    cfg->rc_min_quantizer = data[local_offset % size] % 64;
    local_offset++;
    cfg->rc_max_quantizer = 10 + (data[local_offset % size] % 54);
    local_offset++;
    cfg->g_threads = data[local_offset % size] % 8;
    local_offset++;
    cfg->g_error_resilient = data[local_offset % size] & 1;
    local_offset++;
    cfg->g_lag_in_frames = data[local_offset % size] % 16;
    local_offset++;
    cfg->kf_mode = data[local_offset % size] % 3;
    local_offset++;
    cfg->kf_min_dist = data[local_offset % size] % 100;
    local_offset++;
    cfg->kf_max_dist = 100 + (data[local_offset % size] % 900);
    local_offset++;
    
    *offset = local_offset;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t data_offset = 0;
    
    // Test vpx_codec_vp8_cx and vpx_codec_vp9_cx
    vpx_codec_iface_t *vp8_iface = vpx_codec_vp8_cx();
    vpx_codec_iface_t *vp9_iface = vpx_codec_vp9_cx();
    
    if (!vp8_iface || !vp9_iface) {
        return 0;
    }
    
    // Test vpx_codec_get_caps
    vpx_codec_caps_t vp8_caps = vpx_codec_get_caps(vp8_iface);
    vpx_codec_caps_t vp9_caps = vpx_codec_get_caps(vp9_iface);
    
    // Test vpx_codec_enc_config_default for both codecs
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;
    
    // Test VP8
    err = vpx_codec_enc_config_default(vp8_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        // Try with different usage values based on input data
        unsigned int usage = (Size > 0) ? Data[data_offset % Size] % 4 : 0;
        data_offset++;
        vpx_codec_enc_config_default(vp8_iface, &cfg, usage);
    }
    
    // Test VP9
    err = vpx_codec_enc_config_default(vp9_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        unsigned int usage = (Size > 0) ? Data[data_offset % Size] % 4 : 0;
        data_offset++;
        vpx_codec_enc_config_default(vp9_iface, &cfg, usage);
    }
    
    // Test vpx_codec_enc_init_ver with both codecs
    vpx_codec_ctx_t ctx;
    
    // Initialize with VP8
    init_random_config(&cfg, Data, Size, &data_offset);
    err = vpx_codec_enc_init_ver(&ctx, vp8_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        // Try different flags based on input data
        vpx_codec_flags_t flags = 0;
        if (Size > 0) {
            flags = Data[data_offset % Size] & (VPX_CODEC_USE_PSNR | VPX_CODEC_USE_OUTPUT_PARTITION);
        }
        data_offset++;
        
        // Re-initialize with different flags
        vpx_codec_ctx_t ctx2;
        vpx_codec_enc_init_ver(&ctx2, vp8_iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
        vpx_codec_destroy(&ctx2);
        
        vpx_codec_destroy(&ctx);
    }
    
    // Initialize with VP9
    init_random_config(&cfg, Data, Size, &data_offset);
    err = vpx_codec_enc_init_ver(&ctx, vp9_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
    }
    
    // Test vpx_codec_enc_init_multi_ver (VP8 only as per documentation)
    if (Size > 10) {
        int num_enc = 1 + (Data[data_offset % Size] % 3); // 1-3 encoders
        data_offset++;
        
        vpx_codec_ctx_t multi_ctx[3];
        vpx_codec_enc_cfg_t multi_cfg[3];
        vpx_rational_t dsf[3];
        
        for (int i = 0; i < num_enc; i++) {
            init_random_config(&multi_cfg[i], Data, Size, &data_offset);
            
            // Set downsampling factors
            dsf[i].num = 1 + (Data[data_offset % Size] % 4);
            data_offset++;
            dsf[i].den = 1 + (Data[data_offset % Size] % 4);
            data_offset++;
            
            // Adjust dimensions for downsampling
            if (i > 0) {
                multi_cfg[i].g_w = multi_cfg[0].g_w * dsf[i].den / dsf[i].num;
                multi_cfg[i].g_h = multi_cfg[0].g_h * dsf[i].den / dsf[i].num;
            }
        }
        
        err = vpx_codec_enc_init_multi_ver(multi_ctx, vp8_iface, multi_cfg, num_enc, 0, dsf, VPX_ENCODER_ABI_VERSION);
        
        if (err == VPX_CODEC_OK) {
            for (int i = 0; i < num_enc; i++) {
                vpx_codec_destroy(&multi_ctx[i]);
            }
        }
    }
    
    return 0;
}