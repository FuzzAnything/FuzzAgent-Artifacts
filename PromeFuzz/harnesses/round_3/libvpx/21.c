// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"

#ifdef __cplusplus
extern "C" {
#endif

static void fuzz_init_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Initialize with some default values
    memset(cfg, 0, sizeof(*cfg));
    
    // Use fuzzer data to populate some fields
    if (size >= sizeof(*cfg)) {
        memcpy(cfg, data, sizeof(*cfg));
    } else if (size > 0) {
        memcpy(cfg, data, size);
    }
    
    // Ensure some basic constraints to avoid obvious crashes
    if (cfg->g_w == 0) cfg->g_w = 320;
    if (cfg->g_h == 0) cfg->g_h = 240;
    if (cfg->g_timebase.den == 0) {
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
    }
    if (cfg->rc_target_bitrate == 0) cfg->rc_target_bitrate = 256;
    
    // Clear potentially dangerous pointers
    cfg->rc_twopass_stats_in.buf = NULL;
    cfg->rc_twopass_stats_in.sz = 0;
    cfg->rc_firstpass_mb_stats_in.buf = NULL;
    cfg->rc_firstpass_mb_stats_in.sz = 0;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Use first byte to decide which test to run
    uint8_t test_type = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    // Test vpx_codec_get_caps
    if (test_type & 0x01) {
        vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_cx();
        vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_cx();
        
        vpx_codec_caps_t caps_vp8 = vpx_codec_get_caps(iface_vp8);
        vpx_codec_caps_t caps_vp9 = vpx_codec_get_caps(iface_vp9);
        
        (void)caps_vp8;
        (void)caps_vp9;
    }
    
    // Test vpx_codec_enc_config_default
    if (test_type & 0x02) {
        vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_cx();
        vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_cx();
        
        vpx_codec_enc_cfg_t cfg_vp8, cfg_vp9;
        
        vpx_codec_err_t err_vp8 = vpx_codec_enc_config_default(iface_vp8, &cfg_vp8, 0);
        vpx_codec_err_t err_vp9 = vpx_codec_enc_config_default(iface_vp9, &cfg_vp9, 0);
        
        (void)err_vp8;
        (void)err_vp9;
    }
    
    // Test vpx_codec_enc_init_ver (single encoder)
    if (test_type & 0x04) {
        vpx_codec_iface_t *iface = NULL;
        if (test_type & 0x08) {
            iface = vpx_codec_vp9_cx();
        } else {
            iface = vpx_codec_vp8_cx();
        }
        
        vpx_codec_ctx_t ctx;
        vpx_codec_enc_cfg_t cfg;
        
        // Get default config
        if (vpx_codec_enc_config_default(iface, &cfg, 0) == VPX_CODEC_OK) {
            // Modify config with fuzzer data
            fuzz_init_config(&cfg, fuzz_data, fuzz_size);
            
            // Initialize encoder
            vpx_codec_err_t err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
            
            if (err == VPX_CODEC_OK) {
                // Clean up
                vpx_codec_destroy(&ctx);
            }
        }
    }
    
    // Test vpx_codec_enc_init_multi_ver (multiple encoders)
    if (test_type & 0x10 && fuzz_size > sizeof(vpx_codec_enc_cfg_t)) {
        vpx_codec_iface_t *iface = vpx_codec_vp8_cx();  // Multi-encoder only supported for VP8
        
        // Test with up to 2 encoders
        int num_enc = 2;
        vpx_codec_ctx_t ctx[2];
        vpx_codec_enc_cfg_t cfg[2];
        vpx_rational_t dsf[2];
        
        // Initialize downsampling factors
        for (int i = 0; i < num_enc; i++) {
            dsf[i].num = 1;
            dsf[i].den = 1;
        }
        
        // Get default configs and modify with fuzzer data
        for (int i = 0; i < num_enc; i++) {
            if (vpx_codec_enc_config_default(iface, &cfg[i], 0) == VPX_CODEC_OK) {
                // Use different parts of fuzzer data for each config
                size_t offset = i * sizeof(vpx_codec_enc_cfg_t);
                if (offset < fuzz_size) {
                    size_t copy_size = sizeof(vpx_codec_enc_cfg_t);
                    if (offset + copy_size > fuzz_size) {
                        copy_size = fuzz_size - offset;
                    }
                    memcpy(&cfg[i], fuzz_data + offset, copy_size);
                    
                    // Ensure basic constraints
                    if (cfg[i].g_w == 0) cfg[i].g_w = 320;
                    if (cfg[i].g_h == 0) cfg[i].g_h = 240;
                    if (cfg[i].g_timebase.den == 0) {
                        cfg[i].g_timebase.num = 1;
                        cfg[i].g_timebase.den = 30;
                    }
                    if (cfg[i].rc_target_bitrate == 0) cfg[i].rc_target_bitrate = 256;
                    
                    // Clear potentially dangerous pointers
                    cfg[i].rc_twopass_stats_in.buf = NULL;
                    cfg[i].rc_twopass_stats_in.sz = 0;
                    cfg[i].rc_firstpass_mb_stats_in.buf = NULL;
                    cfg[i].rc_firstpass_mb_stats_in.sz = 0;
                }
            }
        }
        
        // Initialize multiple encoders
        vpx_codec_err_t err = vpx_codec_enc_init_multi_ver(ctx, iface, cfg, num_enc, 0, dsf, VPX_ENCODER_ABI_VERSION);
        
        if (err == VPX_CODEC_OK) {
            // Clean up
            for (int i = 0; i < num_enc; i++) {
                vpx_codec_destroy(&ctx[i]);
            }
        }
    }
    
    return 0;
}

#ifdef __cplusplus
}
#endif