// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"

// Declare the AV1 encoder and decoder interface functions
extern aom_codec_iface_t aom_codec_av1_cx_algo;
extern aom_codec_iface_t aom_codec_av1_dx_algo;

static void init_encoder_config(aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Initialize with default values
    memset(cfg, 0, sizeof(*cfg));
    
    // Set some basic values to avoid crashes
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 1000;
    cfg->g_profile = 0;
    cfg->g_bit_depth = AOM_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->g_threads = 1;
    cfg->g_pass = AOM_RC_ONE_PASS;
    cfg->rc_end_usage = AOM_VBR;
    cfg->kf_mode = AOM_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 9999;
    
    // Use fuzzer data to modify some parameters if we have enough data
    if (size >= 8) {
        cfg->g_w = 16 + (data[0] % 128);  // Keep it reasonable
        cfg->g_h = 16 + (data[1] % 128);
        cfg->rc_target_bitrate = 100 + (data[2] % 9900);
        cfg->rc_min_quantizer = data[3] % 64;
        cfg->rc_max_quantizer = 32 + (data[4] % 32);
        cfg->g_threads = 1 + (data[5] % 4);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    // Test aom_codec_err_to_string with various error codes
    const char *err_str = aom_codec_err_to_string(AOM_CODEC_OK);
    err_str = aom_codec_err_to_string(AOM_CODEC_ERROR);
    err_str = aom_codec_err_to_string(AOM_CODEC_MEM_ERROR);
    err_str = aom_codec_err_to_string(AOM_CODEC_INVALID_PARAM);
    
    // Try to create a codec context for encoder
    aom_codec_ctx_t encoder_ctx;
    aom_codec_iface_t *encoder_iface = &aom_codec_av1_cx_algo;
    
    if (encoder_iface) {
        aom_codec_enc_cfg_t cfg;
        init_encoder_config(&cfg, data, size);
        
        // Try to initialize encoder with version check
        aom_codec_err_t init_result = aom_codec_enc_init_ver(
            &encoder_ctx,
            encoder_iface,
            &cfg,
            0,  // flags
            AOM_ENCODER_ABI_VERSION
        );
        
        // Test error functions after init attempt
        const char *error_msg = aom_codec_error(&encoder_ctx);
        const char *error_detail = aom_codec_error_detail(&encoder_ctx);
        
        // If init succeeded, try to decode some data
        if (init_result == AOM_CODEC_OK) {
            // Create a decoder context
            aom_codec_ctx_t decoder_ctx;
            aom_codec_iface_t *decoder_iface = &aom_codec_av1_dx_algo;
            
            if (decoder_iface) {
                // Initialize decoder
                aom_codec_err_t dec_init_result = aom_codec_dec_init(
                    &decoder_ctx,
                    decoder_iface,
                    NULL,
                    0
                );
                
                if (dec_init_result == AOM_CODEC_OK) {
                    // Try to decode the fuzzer input
                    aom_codec_err_t decode_result = aom_codec_decode(
                        &decoder_ctx,
                        data,
                        size > 1024 ? 1024 : size,  // Limit size to avoid timeouts
                        NULL
                    );
                    
                    // Test error functions after decode
                    error_msg = aom_codec_error(&decoder_ctx);
                    error_detail = aom_codec_error_detail(&decoder_ctx);
                    
                    // Destroy decoder
                    aom_codec_err_t destroy_result = aom_codec_destroy(&decoder_ctx);
                    err_str = aom_codec_err_to_string(destroy_result);
                }
            }
            
            // Destroy encoder
            aom_codec_err_t destroy_result = aom_codec_destroy(&encoder_ctx);
            err_str = aom_codec_err_to_string(destroy_result);
        } else {
            // Test error functions on failed init
            error_msg = aom_codec_error(&encoder_ctx);
            error_detail = aom_codec_error_detail(&encoder_ctx);
            err_str = aom_codec_err_to_string(init_result);
        }
    }
    
    // Test with NULL contexts
    const char *null_error = aom_codec_error(NULL);
    const char *null_detail = aom_codec_error_detail(NULL);
    
    // Test aom_codec_destroy with NULL
    aom_codec_err_t null_destroy = aom_codec_destroy(NULL);
    err_str = aom_codec_err_to_string(null_destroy);
    
    // Create a dummy codec context and try to destroy it without proper init
    aom_codec_ctx_t dummy_ctx;
    memset(&dummy_ctx, 0, sizeof(dummy_ctx));
    aom_codec_err_t dummy_destroy = aom_codec_destroy(&dummy_ctx);
    err_str = aom_codec_err_to_string(dummy_destroy);
    
    return 0;
}