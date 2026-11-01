// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

static vpx_codec_enc_cfg_t init_config(const uint8_t *data, size_t size, vpx_codec_iface_t *iface) {
    vpx_codec_enc_cfg_t cfg;
    
    // Initialize config using the correct API with the interface
    if (iface) {
        vpx_codec_enc_config_default(iface, &cfg, 0);
    } else {
        // Fallback to zero initialization if interface is not available
        memset(&cfg, 0, sizeof(cfg));
    }

    // Use fuzzing data to randomize configuration parameters safely
    if (size > 0) cfg.g_w = (data[0] % 256) + 16; // Ensure minimum width
    if (size > 1) cfg.g_h = (data[1] % 256) + 16; // Ensure minimum height
    if (size > 2) cfg.g_threads = (data[2] % 4) + 1;
    if (size > 3) cfg.g_profile = data[3] % 4;
    if (size > 4) cfg.g_bit_depth = (data[4] % 3) + 1; // 8, 10, or 12
    if (size > 5) cfg.g_input_bit_depth = (data[5] % 3) + 1;
    
    // Timebase
    if (size > 6) cfg.g_timebase.num = (data[6] % 100) + 1;
    if (size > 7) cfg.g_timebase.den = (data[7] % 100) + 1;

    // Rate control
    if (size > 8) cfg.rc_end_usage = (data[8] % 4); // VPX_VBR, VPX_CBR, VPX_CQ, VPX_Q
    if (size > 9) cfg.rc_target_bitrate = (data[9] % 1000) + 100;
    if (size > 10) cfg.rc_min_quantizer = data[10] % 64;
    if (size > 11) cfg.rc_max_quantizer = (data[11] % 64) + 64;
    
    // Keyframe settings
    if (size > 12) cfg.kf_mode = (data[12] % 3); // VPX_KF_FIXED, VPX_KF_AUTO, VPX_KF_DISABLED
    if (size > 13) cfg.kf_min_dist = (data[13] % 30) + 1;
    if (size > 14) cfg.kf_max_dist = (data[14] % 30) + 1;

    return cfg;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Get interfaces
    vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_cx();
    vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_cx();

    // Test vpx_codec_get_caps
    if (iface_vp8) {
        vpx_codec_get_caps(iface_vp8);
        vpx_codec_iface_name(iface_vp8);
    }
    if (iface_vp9) {
        vpx_codec_get_caps(iface_vp9);
        vpx_codec_iface_name(iface_vp9);
    }
    // Test with NULL
    vpx_codec_get_caps(NULL);
    vpx_codec_iface_name(NULL);

    // Flags
    vpx_codec_flags_t flags = 0;
    if (Size > 15) flags = Data[15];

    // Test vpx_codec_enc_init_ver with VP8
    vpx_codec_ctx_t ctx_vp8;
    memset(&ctx_vp8, 0, sizeof(ctx_vp8));
    if (iface_vp8) {
        vpx_codec_enc_cfg_t cfg_vp8 = init_config(Data, Size, iface_vp8);
        vpx_codec_err_t err = vpx_codec_enc_init_ver(&ctx_vp8, iface_vp8, &cfg_vp8, flags, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx_vp8);
        }
    }

    // Test vpx_codec_enc_init_ver with VP9
    vpx_codec_ctx_t ctx_vp9;
    memset(&ctx_vp9, 0, sizeof(ctx_vp9));
    if (iface_vp9) {
        vpx_codec_enc_cfg_t cfg_vp9 = init_config(Data, Size, iface_vp9);
        vpx_codec_err_t err = vpx_codec_enc_init_ver(&ctx_vp9, iface_vp9, &cfg_vp9, flags, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx_vp9);
        }
    }

    // Test vpx_codec_enc_init_multi_ver (VP8 only)
    if (iface_vp8) {
        int num_enc = 1;
        if (Size > 16) num_enc = (Data[16] % 16) + 1; // 1 to 16
        
        // Allocate arrays
        vpx_codec_ctx_t *ctx_multi = (vpx_codec_ctx_t *)malloc(num_enc * sizeof(vpx_codec_ctx_t));
        vpx_codec_enc_cfg_t *cfg_multi = (vpx_codec_enc_cfg_t *)malloc(num_enc * sizeof(vpx_codec_enc_cfg_t));
        vpx_rational_t *dsf_multi = (vpx_rational_t *)malloc(num_enc * sizeof(vpx_rational_t));

        if (ctx_multi && cfg_multi && dsf_multi) {
            memset(ctx_multi, 0, num_enc * sizeof(vpx_codec_ctx_t));
            for (int i = 0; i < num_enc; i++) {
                cfg_multi[i] = init_config(Data, Size, iface_vp8);
                dsf_multi[i].num = 1;
                dsf_multi[i].den = 1;
                if (Size > 17 + i) {
                    dsf_multi[i].num = (Data[17 + i] % 10) + 1;
                    dsf_multi[i].den = (Data[17 + i] % 10) + 1;
                }
            }

            vpx_codec_err_t err = vpx_codec_enc_init_multi_ver(ctx_multi, iface_vp8, cfg_multi, num_enc, flags, dsf_multi, VPX_ENCODER_ABI_VERSION);
            
            if (err == VPX_CODEC_OK) {
                for (int i = 0; i < num_enc; i++) {
                    vpx_codec_destroy(&ctx_multi[i]);
                }
            }
        }

        free(ctx_multi);
        free(cfg_multi);
        free(dsf_multi);
    }

    return 0;
}