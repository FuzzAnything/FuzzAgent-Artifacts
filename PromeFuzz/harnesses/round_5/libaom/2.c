// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"

static void test_aom_codec_err_to_string(const uint8_t *data, size_t size) {
    if (size > 0) {
        // Use first byte as error code
        aom_codec_err_t err = (aom_codec_err_t)data[0];
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str; // Prevent unused variable warning
    }
}

static void test_aom_codec_decode(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx = {0};
    
    if (size > 0) {
        // Create a dummy context structure
        ctx.err = AOM_CODEC_OK;
        
        // Try to decode with the dummy context
        // This will test the function's handling of invalid contexts
        aom_codec_decode(&ctx, data, size, NULL);
    }
}

static void test_aom_codec_set_option(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx = {0};
    
    if (size > 2) {
        // Create null-terminated strings from input data
        // Use first half for key, second half for value
        size_t key_len = size / 2;
        size_t val_len = size - key_len;
        
        char *key = (char *)malloc(key_len + 1);
        char *val = (char *)malloc(val_len + 1);
        
        if (key && val) {
            memcpy(key, data, key_len);
            key[key_len] = '\0';
            memcpy(val, data + key_len, val_len);
            val[val_len] = '\0';
            
            // Test with dummy context
            aom_codec_set_option(&ctx, key, val);
            
            free(key);
            free(val);
        }
    }
}

static void test_aom_codec_control(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx = {0};
    
    if (size >= sizeof(int)) {
        // Extract control ID from first sizeof(int) bytes
        int ctrl_id = 0;
        memcpy(&ctrl_id, data, sizeof(int));
        
        // Use remaining data as control argument if available
        // Check if we have enough data for the argument
        if (size >= 2 * sizeof(int)) {
            int arg = 0;
            memcpy(&arg, data + sizeof(int), sizeof(int));
            aom_codec_control(&ctx, ctrl_id, arg);
        } else {
            aom_codec_control(&ctx, ctrl_id, 0);
        }
    }
}

static void test_aom_codec_get_stream_info(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx = {0};
    aom_codec_stream_info_t si = {0};
    
    if (size > 0) {
        // Test with dummy context
        aom_codec_get_stream_info(&ctx, &si);
    }
}

static void test_aom_codec_destroy(const uint8_t *data, size_t size) {
    (void)data;
    (void)size;
    
    // Test with null pointer
    aom_codec_destroy(NULL);
    
    // Test with uninitialized context
    aom_codec_ctx_t ctx = {0};
    aom_codec_destroy(&ctx);
}

int LLVMFuzzerTestOneInput_2(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    // Test each API function with the fuzzer input
    test_aom_codec_err_to_string(data, size);
    test_aom_codec_decode(data, size);
    test_aom_codec_set_option(data, size);
    test_aom_codec_control(data, size);
    test_aom_codec_get_stream_info(data, size);
    test_aom_codec_destroy(data, size);
    
    return 0;
}