// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_version_extra_str at vpx_codec.c:28:13 in vpx_codec.h
// vpx_codec_build_config at vpx_config.c:10:13 in vpx_codec.h
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_version_str at vpx_codec.c:26:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx_codec.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Call simple functions that don't require context
    const char *version_extra = vpx_codec_version_extra_str();
    const char *build_config = vpx_codec_build_config();
    int version = vpx_codec_version();
    const char *version_str = vpx_codec_version_str();
    
    // Use the version information (even though we don't do anything with it,
    // calling these functions exercises the library)
    (void)version_extra;
    (void)build_config;
    (void)version;
    (void)version_str;
    
    // Test vpx_codec_err_to_string with various error codes
    // Use the input data to generate different error codes
    if (Size > 0) {
        // Create error code from input data
        vpx_codec_err_t err_code = (vpx_codec_err_t)(Data[0] % 256);
        const char *err_str = vpx_codec_err_to_string(err_code);
        (void)err_str;
        
        // Test with more error codes if we have enough data
        for (size_t i = 1; i < Size && i < 10; i++) {
            err_code = (vpx_codec_err_t)(Data[i] % 256);
            err_str = vpx_codec_err_to_string(err_code);
            (void)err_str;
        }
    }
    
    // Test vpx_codec_error_detail with a dummy context
    // We need to create a vpx_codec_ctx_t structure
    static vpx_codec_ctx_t ctx;
    
    // Initialize context fields with dummy values
    ctx.name = NULL;
    ctx.iface = NULL;
    ctx.err = VPX_CODEC_OK;  // Default to OK
    
    // Use input data to set error details if we have enough data
    if (Size > 0) {
        // Set error code based on input
        ctx.err = (vpx_codec_err_t)(Data[0] % 256);
        
        // If we have more data, create a string for error details
        if (Size > 1) {
            // Create a buffer for error details
            char *detail_buf = (char *)malloc(Size);
            if (detail_buf) {
                memcpy(detail_buf, Data, Size - 1);
                detail_buf[Size - 1] = '\0';  // Ensure null termination
                ctx.err_detail = detail_buf;
                
                // Call the target function
                const char *error_detail = vpx_codec_error_detail(&ctx);
                (void)error_detail;
                
                free(detail_buf);
                ctx.err_detail = NULL;
            }
        } else {
            // Call with minimal context
            const char *error_detail = vpx_codec_error_detail(&ctx);
            (void)error_detail;
        }
    }
    
    // Test with NULL context
    const char *null_detail = vpx_codec_error_detail(NULL);
    (void)null_detail;
    
    // Test with various error states in context
    for (int i = 0; i < 5 && i < (int)Size; i++) {
        ctx.err = (vpx_codec_err_t)((Data[i] + i) % 256);
        const char *error_detail = vpx_codec_error_detail(&ctx);
        (void)error_detail;
    }
    
    return 0;
}