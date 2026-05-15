// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"

static void init_encoder_config(aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Initialize with some sane defaults
    cfg->g_usage = 0;
    cfg->g_threads = 1;
    cfg->g_profile = 0;
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_limit = 30;
    cfg->g_forced_max_frame_width = 0;
    cfg->g_forced_max_frame_height = 0;
    cfg->g_bit_depth = AOM_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 0;
    cfg->g_pass = AOM_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    
    cfg->rc_dropframe_thresh = 0;
    cfg->rc_resize_mode = 0;
    cfg->rc_resize_denominator = 8;
    cfg->rc_resize_kf_denominator = 8;
    cfg->rc_superres_mode = 0; // AOM_SUPERRES_NONE
    cfg->rc_superres_denominator = 8;
    cfg->rc_superres_kf_denominator = 8;
    cfg->rc_superres_qthresh = 0;
    cfg->rc_superres_kf_qthresh = 0;
    cfg->rc_end_usage = AOM_VBR;
    cfg->rc_target_bitrate = 256;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 56;
    cfg->rc_undershoot_pct = 50;
    cfg->rc_overshoot_pct = 50;
    cfg->rc_buf_sz = 1000;
    cfg->rc_buf_initial_sz = 500;
    cfg->rc_buf_optimal_sz = 600;
    cfg->rc_2pass_vbr_bias_pct = 50;
    cfg->rc_2pass_vbr_minsection_pct = 0;
    cfg->rc_2pass_vbr_maxsection_pct = 2000;
    cfg->fwd_kf_enabled = 0;
    cfg->kf_mode = AOM_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 9999;
    cfg->sframe_dist = 0;
    cfg->sframe_mode = 0;
    cfg->large_scale_tile = 0;
    cfg->monochrome = 0;
    cfg->full_still_picture_hdr = 0;
    cfg->save_as_annexb = 0;
    cfg->tile_width_count = 0;
    cfg->tile_height_count = 0;
    cfg->use_fixed_qp_offsets = 0;
    
    // Use fuzzer data to modify some parameters
    if (size > 0) {
        size_t offset = 0;
        
        if (size >= sizeof(unsigned int)) {
            cfg->g_w = *(const unsigned int*)(data + offset) % 1024 + 16;
            offset += sizeof(unsigned int);
        }
        if (size - offset >= sizeof(unsigned int)) {
            cfg->g_h = *(const unsigned int*)(data + offset) % 1024 + 16;
            offset += sizeof(unsigned int);
        }
        if (size - offset >= sizeof(unsigned int)) {
            cfg->rc_target_bitrate = *(const unsigned int*)(data + offset) % 2000000;
            offset += sizeof(unsigned int);
        }
        if (size - offset >= sizeof(unsigned int)) {
            cfg->g_threads = (*(const unsigned int*)(data + offset) % 16) + 1;
            offset += sizeof(unsigned int);
        }
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Test encoder initialization and destruction
    aom_codec_ctx_t encoder_ctx;
    aom_codec_enc_cfg_t cfg;
    
    // Initialize encoder config with fuzzer data
    init_encoder_config(&cfg, data, size);
    
    // Initialize encoder with version check
    aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(&encoder_ctx, NULL, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    
    // Test error handling functions
    if (enc_init_result != AOM_CODEC_OK) {
        const char *error_str = aom_codec_error(&encoder_ctx);
        const char *error_detail = aom_codec_error_detail(&encoder_ctx);
        const char *err_to_str = aom_codec_err_to_string(enc_init_result);
        
        // Just call them to ensure they don't crash
        (void)error_str;
        (void)error_detail;
        (void)err_to_str;
    } else {
        // If encoder initialized successfully, test destruction
        aom_codec_err_t destroy_result = aom_codec_destroy(&encoder_ctx);
        
        // Test error conversion for destruction result
        const char *destroy_err_str = aom_codec_err_to_string(destroy_result);
        (void)destroy_err_str;
    }
    
    // Test decoder
    aom_codec_ctx_t decoder_ctx;
    
    // Initialize decoder
    aom_codec_err_t dec_init_result = aom_codec_dec_init(&decoder_ctx, NULL, NULL, 0);
    
    if (dec_init_result == AOM_CODEC_OK) {
        // Try to decode the fuzzer input
        aom_codec_err_t decode_result = aom_codec_decode(&decoder_ctx, data, size, NULL);
        
        // Test error handling for decode
        if (decode_result != AOM_CODEC_OK) {
            const char *decode_error = aom_codec_error(&decoder_ctx);
            const char *decode_error_detail = aom_codec_error_detail(&decoder_ctx);
            const char *decode_err_str = aom_codec_err_to_string(decode_result);
            
            (void)decode_error;
            (void)decode_error_detail;
            (void)decode_err_str;
        }
        
        // Destroy decoder
        aom_codec_destroy(&decoder_ctx);
    } else {
        // Test error handling for failed decoder init
        const char *dec_init_error = aom_codec_error(&decoder_ctx);
        const char *dec_init_err_str = aom_codec_err_to_string(dec_init_result);
        
        (void)dec_init_error;
        (void)dec_init_err_str;
    }
    
    // Test error functions with NULL context
    const char *null_error = aom_codec_error(NULL);
    const char *null_error_detail = aom_codec_error_detail(NULL);
    
    (void)null_error;
    (void)null_error_detail;
    
    // Test error string conversion for various error codes
    const char *err_strings[] = {
        aom_codec_err_to_string(AOM_CODEC_OK),
        aom_codec_err_to_string(AOM_CODEC_ERROR),
        aom_codec_err_to_string(AOM_CODEC_MEM_ERROR),
        aom_codec_err_to_string(AOM_CODEC_ABI_MISMATCH),
        aom_codec_err_to_string(AOM_CODEC_INCAPABLE),
        aom_codec_err_to_string(AOM_CODEC_UNSUP_BITSTREAM),
        aom_codec_err_to_string(AOM_CODEC_UNSUP_FEATURE),
        aom_codec_err_to_string(AOM_CODEC_CORRUPT_FRAME),
        aom_codec_err_to_string(AOM_CODEC_INVALID_PARAM),
        aom_codec_err_to_string(999) // Test invalid error code
    };
    
    (void)err_strings;
    
    return 0;
}