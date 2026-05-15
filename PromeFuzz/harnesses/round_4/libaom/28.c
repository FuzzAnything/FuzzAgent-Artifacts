// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom_codec.h"
#include "aom_encoder.h"

static void init_random_config(aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Initialize with some defaults
    memset(cfg, 0, sizeof(*cfg));
    
    // Use fuzzer data to set some fields
    if (size > 0) {
        cfg->g_w = (data[0] << 8) | (size > 1 ? data[1] : 0);
        cfg->g_h = (size > 2 ? data[2] << 8 : 0) | (size > 3 ? data[3] : 0);
        cfg->g_timebase.num = size > 4 ? data[4] : 1;
        cfg->g_timebase.den = size > 5 ? data[5] : 30;
        cfg->rc_target_bitrate = size > 6 ? data[6] * 100 : 1000;
        cfg->g_pass = size > 7 ? (data[7] % 4) : AOM_RC_ONE_PASS;
        cfg->g_lag_in_frames = size > 8 ? data[8] : 0;
        cfg->rc_min_quantizer = size > 9 ? data[9] % 64 : 10;
        cfg->rc_max_quantizer = size > 10 ? data[10] % 64 : 50;
    } else {
        cfg->g_w = 640;
        cfg->g_h = 480;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->rc_target_bitrate = 1000;
        cfg->g_pass = AOM_RC_ONE_PASS;
        cfg->g_lag_in_frames = 0;
        cfg->rc_min_quantizer = 10;
        cfg->rc_max_quantizer = 50;
    }
    
    cfg->g_bit_depth = AOM_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->g_error_resilient = 0;
    cfg->rc_end_usage = AOM_VBR;
    cfg->kf_mode = AOM_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 9999;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    // Test aom_codec_err_to_string with various error codes
    for (int i = 0; i < 10; i++) {
        aom_codec_err_t err = (aom_codec_err_t)(data[0] + i);
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str; // Use result to avoid unused variable warning
    }
    
    // Test with NULL context
    const char *error_null = aom_codec_error(NULL);
    const char *detail_null = aom_codec_error_detail(NULL);
    (void)error_null;
    (void)detail_null;
    
    // Create and initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    
    // Initialize configuration
    init_random_config(&cfg, data, size);
    
    // Try to initialize encoder with a dummy interface pointer
    // We'll use NULL for iface to test error cases
    aom_codec_err_t init_result = aom_codec_enc_init_ver(&ctx, NULL, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    
    // Test error functions with the context
    const char *error_msg = aom_codec_error(&ctx);
    const char *detail_msg = aom_codec_error_detail(&ctx);
    (void)error_msg;
    (void)detail_msg;
    
    // Test aom_codec_set_option if initialization succeeded (or even if it didn't)
    // Create some option strings from fuzzer data
    char key[64];
    char value[64];
    
    // Use fuzzer data to create key-value pairs
    size_t key_len = size > 0 ? (data[0] % 63) + 1 : 10;
    size_t val_len = size > 1 ? (data[1] % 63) + 1 : 10;
    
    for (size_t i = 0; i < key_len && i + 2 < size; i++) {
        key[i] = data[i + 2] ? data[i + 2] : 'A';
    }
    key[key_len < 63 ? key_len : 62] = '\0';
    
    for (size_t i = 0; i < val_len && i + 2 + key_len < size; i++) {
        value[i] = data[i + 2 + key_len] ? data[i + 2 + key_len] : 'B';
    }
    value[val_len < 63 ? val_len : 62] = '\0';
    
    // Try setting the option (works even if init failed)
    aom_codec_set_option(&ctx, key, value);
    
    // Test with empty strings
    aom_codec_set_option(&ctx, "", "");
    
    // Test with NULL values
    aom_codec_set_option(&ctx, key, NULL);
    aom_codec_set_option(&ctx, NULL, value);
    aom_codec_set_option(&ctx, NULL, NULL);
    
    // Test error functions again
    error_msg = aom_codec_error(&ctx);
    detail_msg = aom_codec_error_detail(&ctx);
    (void)error_msg;
    (void)detail_msg;
    
    // Test aom_codec_err_to_string with the context's error
    const char *ctx_err_str = aom_codec_err_to_string(ctx.err);
    (void)ctx_err_str;
    
    // Destroy the codec instance
    aom_codec_err_t destroy_result = aom_codec_destroy(&ctx);
    (void)destroy_result;
    
    // Test destroying again (should fail)
    aom_codec_destroy(&ctx);
    
    // Test with NULL context for destroy
    aom_codec_destroy(NULL);
    
    return 0;
}