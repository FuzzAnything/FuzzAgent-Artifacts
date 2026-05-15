// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

static void test_aom_codec_err_to_string(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use data to generate error codes
    for (size_t i = 0; i < size && i < 10; i++) {
        aom_codec_err_t err = (aom_codec_err_t)(data[i] % 20); // Limit to reasonable range
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str; // Use result to avoid compiler warnings
    }
}

static void test_aom_codec_destroy(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_err_t err = aom_codec_destroy(ctx);
        (void)err;
    }
    
    // Test with NULL context
    aom_codec_destroy(NULL);
}

static void test_aom_codec_decode(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || size == 0) return;
    
    // Try decoding with different chunk sizes
    size_t chunk_size = size > 100 ? 100 : size;
    for (size_t offset = 0; offset < size; offset += chunk_size) {
        size_t remaining = size - offset;
        size_t decode_size = remaining < chunk_size ? remaining : chunk_size;
        
        aom_codec_err_t err = aom_codec_decode(ctx, data + offset, decode_size, NULL);
        (void)err;
        
        // Break early if we've processed enough
        if (offset > 1000) break;
    }
}

static void test_aom_codec_set_option(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || size < 2) return;
    
    // Create null-terminated strings from data
    char key[256] = {0};
    char value[256] = {0};
    
    size_t key_len = data[0] % 255;
    if (key_len == 0) key_len = 1;
    if (key_len > size - 1) key_len = size - 1;
    
    size_t value_len = data[1] % 255;
    if (value_len == 0) value_len = 1;
    if (value_len > size - key_len - 1) value_len = size - key_len - 1;
    
    if (key_len > 255) key_len = 255;
    if (value_len > 255) value_len = 255;
    
    memcpy(key, data + 2, key_len);
    memcpy(value, data + 2 + key_len, value_len);
    key[key_len] = '\0';
    value[value_len] = '\0';
    
    aom_codec_err_t err = aom_codec_set_option(ctx, key, value);
    (void)err;
    
    // Test with NULL context
    aom_codec_set_option(NULL, "test", "value");
    
    // Test with NULL name or value
    if (size > 10) {
        aom_codec_set_option(ctx, NULL, "value");
        aom_codec_set_option(ctx, "test", NULL);
    }
}

static void test_aom_codec_get_stream_info(aom_codec_ctx_t *ctx) {
    if (!ctx) return;
    
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    
    aom_codec_err_t err = aom_codec_get_stream_info(ctx, &si);
    (void)err;
    
    // Test with NULL parameters
    aom_codec_get_stream_info(NULL, &si);
    aom_codec_get_stream_info(ctx, NULL);
}

static void test_aom_codec_dec_init_ver(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return;
    
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return;
    
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    // Use data to influence configuration
    if (size >= sizeof(cfg)) {
        memcpy(&cfg, data, sizeof(cfg) > size ? size : sizeof(cfg));
    }
    
    // Limit values to reasonable ranges
    cfg.threads = cfg.threads % 16;
    if (cfg.threads == 0) cfg.threads = 1;
    
    // Test different initialization scenarios
    aom_codec_err_t err;
    
    // Test with valid parameters
    err = aom_codec_dec_init_ver(ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    (void)err;
    
    // Clean up after each initialization attempt
    aom_codec_destroy(ctx);
    
    // Test with NULL config
    if (size > 0) {
        err = aom_codec_dec_init_ver(ctx, iface, NULL, 0, AOM_DECODER_ABI_VERSION);
        (void)err;
        aom_codec_destroy(ctx);
    }
    
    // Test with invalid ABI version
    if (size > 1) {
        err = aom_codec_dec_init_ver(ctx, iface, &cfg, 0, data[0]);
        (void)err;
        aom_codec_destroy(ctx);
    }
    
    // Test with NULL context
    aom_codec_dec_init_ver(NULL, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // Test with NULL interface
    err = aom_codec_dec_init_ver(ctx, NULL, &cfg, 0, AOM_DECODER_ABI_VERSION);
    (void)err;
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Test error code to string conversion
    test_aom_codec_err_to_string(data, size);
    
    // Initialize codec context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Test decoder initialization with cleanup
    test_aom_codec_dec_init_ver(&ctx, data, size);
    
    // Re-initialize for other tests
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface) {
        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        
        aom_codec_err_t err = aom_codec_dec_init_ver(&ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
        if (err == AOM_CODEC_OK) {
            // Test set option
            test_aom_codec_set_option(&ctx, data, size);
            
            // Test decode
            test_aom_codec_decode(&ctx, data, size);
            
            // Test get stream info
            test_aom_codec_get_stream_info(&ctx);
            
            // Clean up
            aom_codec_destroy(&ctx);
        }
    }
    
    // Test destroy with potentially uninitialized context
    test_aom_codec_destroy(&ctx);
    
    return 0;
}