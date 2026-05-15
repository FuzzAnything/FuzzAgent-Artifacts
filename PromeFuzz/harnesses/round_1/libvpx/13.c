// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_version_extra_str at vpx_codec.c:28:13 in vpx_codec.h
// vpx_codec_build_config at vpx_config.c:10:13 in vpx_codec.h
// vpx_codec_version_str at vpx_codec.c:26:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Call version functions - these don't require any setup
    int version = vpx_codec_version();
    const char *version_extra = vpx_codec_version_extra_str();
    const char *build_config = vpx_codec_build_config();
    const char *version_str = vpx_codec_version_str();
    
    // Use fuzzer data to generate error codes for testing vpx_codec_err_to_string
    // We'll use the first few bytes to create error codes
    if (Size > 0) {
        // Create error codes from the data
        for (size_t i = 0; i < Size && i < 10; i++) {
            vpx_codec_err_t err = (vpx_codec_err_t)Data[i];
            const char *err_str = vpx_codec_err_to_string(err);
            (void)err_str; // Avoid unused variable warning
        }
    }
    
    // Test vpx_codec_error_detail - this requires a codec context
    // We'll create a minimal context and try to trigger errors
    vpx_codec_ctx_t ctx;
    
    // Initialize context with some values from fuzzer data
    ctx.name = NULL;
    ctx.iface = NULL;
    ctx.err = VPX_CODEC_OK;
    ctx.err_detail = NULL;
    ctx.init_flags = 0;
    ctx.config.raw = NULL;
    ctx.priv = NULL;
    
    // Use fuzzer data to set error code and detail if we have enough data
    if (Size >= 2) {
        ctx.err = (vpx_codec_err_t)Data[0];
        // Create a temporary string from the data for error detail
        // Note: In real usage, this would be set by the library internally
        // We're just testing that the function can be called safely
        static char temp_detail[256];
        size_t copy_len = Size - 1 < 255 ? Size - 1 : 255;
        for (size_t i = 0; i < copy_len; i++) {
            temp_detail[i] = Data[i + 1];
        }
        temp_detail[copy_len] = '\0';
        ctx.err_detail = temp_detail;
    }
    
    // Call vpx_codec_error_detail
    const char *error_detail = vpx_codec_error_detail(&ctx);
    (void)error_detail; // Avoid unused variable warning
    
    // Try with a NULL context
    if (Size > 0 && (Data[0] & 1)) {
        error_detail = vpx_codec_error_detail(NULL);
        (void)error_detail;
    }
    
    // Test more error codes with vpx_codec_err_to_string
    // Use various values from the fuzzer input
    if (Size > 0) {
        // Test some known error codes
        const vpx_codec_err_t known_errors[] = {
            VPX_CODEC_OK,
            VPX_CODEC_ERROR,
            VPX_CODEC_MEM_ERROR,
            VPX_CODEC_ABI_MISMATCH,
            VPX_CODEC_INCAPABLE,
            VPX_CODEC_UNSUP_BITSTREAM,
            VPX_CODEC_UNSUP_FEATURE,
            VPX_CODEC_CORRUPT_FRAME,
            VPX_CODEC_INVALID_PARAM,
            VPX_CODEC_LIST_END
        };
        
        for (size_t i = 0; i < sizeof(known_errors)/sizeof(known_errors[0]); i++) {
            const char *err_str = vpx_codec_err_to_string(known_errors[i]);
            (void)err_str;
        }
        
        // Test some random values from fuzzer data
        for (size_t i = 0; i < Size && i < 20; i++) {
            vpx_codec_err_t random_err = (vpx_codec_err_t)(Data[i] * 17);
            const char *err_str = vpx_codec_err_to_string(random_err);
            (void)err_str;
        }
    }
    
    return 0;
}