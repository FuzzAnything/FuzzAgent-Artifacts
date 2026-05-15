// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom_codec.h"
#include "aom_decoder.h"

// Forward declaration for the AV1 decoder interface getter
const aom_codec_iface_t *aom_codec_av1_dx(void);

static void fuzz_decoder_init(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    // Get the AV1 decoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return;
    }
    
    aom_codec_dec_cfg_t cfg = {0};
    aom_codec_flags_t flags = 0;
    
    // Use fuzz data to influence initialization parameters
    if (size > 0) {
        cfg.threads = (data[0] % 8) + 1;  // 1-8 threads
        flags = (data[0] >> 4) & 0xF;     // Use some bits for flags
    }
    
    // Try initialization with version checking
    aom_codec_err_t err = aom_codec_dec_init_ver(ctx, iface, &cfg, flags, AOM_DECODER_ABI_VERSION);
    
    // Convert error to string (for coverage, ignore result)
    const char *err_str = aom_codec_err_to_string(err);
    (void)err_str;
}

static void fuzz_set_options(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create null-terminated strings from fuzz data
    char key[32] = {0};
    char value[32] = {0};
    
    size_t key_len = (data[0] % 31) + 1;
    size_t val_len = (data[1] % 31) + 1;
    
    if (key_len > 31) key_len = 31;
    if (val_len > 31) val_len = 31;
    
    for (size_t i = 0; i < key_len && i + 2 < size; i++) {
        key[i] = data[2 + i] ? data[2 + i] : 'A';
    }
    key[key_len] = '\0';
    
    for (size_t i = 0; i < val_len && i + 2 + key_len < size; i++) {
        value[i] = data[2 + key_len + i] ? data[2 + key_len + i] : '1';
    }
    value[val_len] = '\0';
    
    aom_codec_err_t err = aom_codec_set_option(ctx, key, value);
    (void)err;
}

static void fuzz_decode_and_get_info(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Try decoding with the fuzz data
    aom_codec_err_t decode_err = aom_codec_decode(ctx, data, size, NULL);
    (void)decode_err;
    
    // Try to get stream information
    aom_codec_stream_info_t si = {0};
    aom_codec_err_t info_err = aom_codec_get_stream_info(ctx, &si);
    (void)info_err;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    aom_codec_ctx_t ctx = {0};
    
    // Initialize decoder with fuzz-influenced parameters
    fuzz_decoder_init(&ctx, data, size);
    
    // Only proceed if initialization succeeded
    if (ctx.iface != NULL) {
        // Try setting options with remaining data
        size_t offset = (size > 0) ? 1 : 0;
        if (size > offset) {
            fuzz_set_options(&ctx, data + offset, size - offset);
        }
        
        // Try decoding with all data (including invalid data)
        fuzz_decode_and_get_info(&ctx, data, size);
        
        // Clean up
        aom_codec_err_t destroy_err = aom_codec_destroy(&ctx);
        (void)destroy_err;
    } else {
        // Test error string conversion for initialization failure
        const char *err_str = aom_codec_err_to_string(AOM_CODEC_MEM_ERROR);
        (void)err_str;
    }
    
    return 0;
}