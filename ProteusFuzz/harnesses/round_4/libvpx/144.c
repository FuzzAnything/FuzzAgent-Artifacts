// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static void init_encoder_cfg(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!cfg) return;
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    // Initialize with safe defaults
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_bit_depth = VPX_BITS_8;
    cfg->g_input_bit_depth = VPX_BITS_8;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_profile = 0;
    cfg->rc_end_usage = VPX_VBR;
    cfg->rc_target_bitrate = 100;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 63;
    cfg->kf_mode = VPX_KF_DISABLED;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 0;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    
    // Use fuzzing data to perturb values safely, checking bounds
    if (size > 0) {
        cfg->g_w = (data[0] % 1024) + 16;
        if (size > 1) {
            cfg->g_h = (data[1] % 1024) + 16;
        }
        if (size > 2) {
            cfg->rc_target_bitrate = (data[2] % 10000) + 1;
        }
        if (size > 3) {
            cfg->g_threads = (data[3] % 4) + 1;
        }
        if (size > 4) {
            cfg->g_profile = data[4] % 4;
        }
        if (size > 5) {
            cfg->rc_min_quantizer = data[5] % 63;
        }
        if (size > 6) {
            cfg->rc_max_quantizer = data[6] % 63 + 10;
        }
    }
}

static void init_decoder_cfg(vpx_codec_dec_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!cfg) return;
    memset(cfg, 0, sizeof(vpx_codec_dec_cfg_t));
    
    cfg->threads = 1;
    cfg->w = 64;
    cfg->h = 64;
    
    if (size > 0) {
        cfg->threads = (data[0] % 4) + 1;
        if (size > 1) {
            cfg->w = (data[1] % 1024) + 16;
        }
        if (size > 2) {
            cfg->h = (data[2] % 1024) + 16;
        }
    }
}

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Test vpx_codec_version
    int version = vpx_codec_version();
    (void)version;

    // 2. Test vpx_codec_err_to_string with various error codes
    vpx_codec_err_t err_codes[] = {
        VPX_CODEC_OK,
        VPX_CODEC_MEM_ERROR,
        VPX_CODEC_ERROR,
        VPX_CODEC_INCAPABLE,
        VPX_CODEC_ABI_MISMATCH,
        -1,
        -999
    };
    size_t num_errs = sizeof(err_codes) / sizeof(err_codes[0]);
    
    for (size_t i = 0; i < num_errs; i++) {
        const char *str = vpx_codec_err_to_string(err_codes[i]);
        (void)str;
    }
    
    // Perturb error code from input
    vpx_codec_err_t fuzzed_err = (vpx_codec_err_t)(Data[0] * 100 - 500);
    const char *fuzzed_str = vpx_codec_err_to_string(fuzzed_err);
    (void)fuzzed_str;

    // 3. Test Encoder Initialization
    vpx_codec_ctx_t enc_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));
    
    vpx_codec_enc_cfg_t enc_cfg;
    init_encoder_cfg(&enc_cfg, Data, Size);
    
    const vpx_codec_iface_t *enc_iface = &vpx_codec_vp8_cx;
    
    if (enc_iface) {
        vpx_codec_err_t ret = vpx_codec_enc_init_ver(&enc_ctx, enc_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
        
        // Test error retrieval functions
        const char *err_msg = vpx_codec_error(&enc_ctx);
        const char *err_detail = vpx_codec_error_detail(&enc_ctx);
        (void)err_msg;
        (void)err_detail;
        
        if (ret == VPX_CODEC_OK) {
            // If init succeeded, destroy it
            vpx_codec_destroy(&enc_ctx);
        }
    }
    
    // Test with invalid ABI version
    if (enc_iface) {
        vpx_codec_err_t ret_bad = vpx_codec_enc_init_ver(&enc_ctx, enc_iface, &enc_cfg, 0, 0);
        (void)ret_bad;
        const char *err_bad = vpx_codec_error(&enc_ctx);
        const char *detail_bad = vpx_codec_error_detail(&enc_ctx);
        (void)err_bad;
        (void)detail_bad;
    }

    // 4. Test Decoder Initialization
    vpx_codec_ctx_t dec_ctx;
    memset(&dec_ctx, 0, sizeof(dec_ctx));
    
    vpx_codec_dec_cfg_t dec_cfg;
    init_decoder_cfg(&dec_cfg, Data, Size);
    
    const vpx_codec_iface_t *dec_iface = &vpx_codec_vp8_dx;
    
    if (dec_iface) {
        vpx_codec_err_t ret = vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        
        const char *dec_err_msg = vpx_codec_error(&dec_ctx);
        const char *dec_err_detail = vpx_codec_error_detail(&dec_ctx);
        (void)dec_err_msg;
        (void)dec_err_detail;
        
        if (ret == VPX_CODEC_OK) {
            vpx_codec_destroy(&dec_ctx);
        }
    }
    
    // Test with invalid ABI version for decoder
    if (dec_iface) {
        vpx_codec_err_t ret_bad = vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 0, 0);
        (void)ret_bad;
        const char *dec_err_bad = vpx_codec_error(&dec_ctx);
        const char *dec_detail_bad = vpx_codec_error_detail(&dec_ctx);
        (void)dec_err_bad;
        (void)dec_detail_bad;
    }

    // 5. Test error functions with NULL context
    const char *null_err = vpx_codec_error(NULL);
    const char *null_detail = vpx_codec_error_detail(NULL);
    (void)null_err;
    (void)null_detail;

    return 0;
}