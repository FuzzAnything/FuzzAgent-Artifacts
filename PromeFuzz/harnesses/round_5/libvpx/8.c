// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"

// Declare the decoder interface variables as extern (they are defined in the library)
extern const vpx_codec_iface_t vpx_codec_vp8_dx_algo;
extern const vpx_codec_iface_t vpx_codec_vp9_dx_algo;

static void fuzz_init_config(vpx_codec_dec_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (size >= sizeof(vpx_codec_dec_cfg_t)) {
        memcpy(cfg, data, sizeof(vpx_codec_dec_cfg_t));
    } else {
        cfg->threads = 1;
        cfg->w = 320;
        cfg->h = 240;
    }
}

static const vpx_codec_iface_t* fuzz_get_codec_iface(const uint8_t *data, size_t size) {
    if (size == 0) return &vpx_codec_vp8_dx_algo;
    return (data[0] % 2) ? &vpx_codec_vp9_dx_algo : &vpx_codec_vp8_dx_algo;
}

static vpx_codec_flags_t fuzz_get_flags(const uint8_t *data, size_t size) {
    if (size < 2) return 0;
    return (vpx_codec_flags_t)data[1];
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_iter_t iter = NULL;
    
    // Initialize configuration with fuzzed data
    fuzz_init_config(&cfg, Data, Size);
    
    // Get codec interface based on fuzzed data
    const vpx_codec_iface_t *iface = fuzz_get_codec_iface(Data, Size);
    
    // Get flags based on fuzzed data
    vpx_codec_flags_t flags = fuzz_get_flags(Data, Size);
    
    // Test vpx_codec_dec_init_ver via macro
    vpx_codec_err_t init_err = vpx_codec_dec_init(&ctx, iface, &cfg, flags);
    
    // Test vpx_codec_error
    const char *error_str = vpx_codec_error(&ctx);
    (void)error_str; // Use result to avoid unused variable warning
    
    // Test vpx_codec_err_to_string
    const char *err_to_str = vpx_codec_err_to_string(init_err);
    (void)err_to_str;
    
    // Test vpx_codec_error_detail
    const char *error_detail = vpx_codec_error_detail(&ctx);
    (void)error_detail;
    
    // Test vpx_codec_get_frame multiple times with different iterator states
    for (int i = 0; i < 3; i++) {
        vpx_image_t *frame = vpx_codec_get_frame(&ctx, &iter);
        (void)frame;
        
        // Reset iterator for next iteration to test different states
        if (i == 1) {
            iter = NULL;
        }
    }
    
    // Test vpx_codec_destroy
    vpx_codec_err_t destroy_err = vpx_codec_destroy(&ctx);
    (void)destroy_err;
    
    // Test with NULL config
    if (Size > 10) {
        vpx_codec_ctx_t ctx2;
        vpx_codec_err_t init_err2 = vpx_codec_dec_init(&ctx2, iface, NULL, flags);
        (void)init_err2;
        
        const char *error_str2 = vpx_codec_error(&ctx2);
        (void)error_str2;
        
        vpx_codec_err_t destroy_err2 = vpx_codec_destroy(&ctx2);
        (void)destroy_err2;
    }
    
    // Test error functions with NULL context
    if (Size > 20) {
        const char *null_error = vpx_codec_error(NULL);
        (void)null_error;
        
        const char *null_detail = vpx_codec_error_detail(NULL);
        (void)null_detail;
        
        vpx_codec_err_t null_destroy = vpx_codec_destroy(NULL);
        (void)null_destroy;
    }
    
    return 0;
}