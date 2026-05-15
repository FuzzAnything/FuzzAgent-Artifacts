// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_version_extra_str at vpx_codec.c:28:13 in vpx_codec.h
// vpx_codec_build_config at vpx_config.c:10:13 in vpx_codec.h
// vpx_codec_version_str at vpx_codec.c:26:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // 1. Call version-related functions (no setup needed)
    int version = vpx_codec_version();
    (void)version; // Use to avoid unused variable warning
    
    const char *version_extra = vpx_codec_version_extra_str();
    if (version_extra) {
        // Touch the string to ensure it's accessed
        volatile char c = version_extra[0];
        (void)c;
    }
    
    const char *build_config = vpx_codec_build_config();
    if (build_config) {
        volatile char c = build_config[0];
        (void)c;
    }
    
    const char *version_str = vpx_codec_version_str();
    if (version_str) {
        volatile char c = version_str[0];
        (void)c;
    }
    
    // 2. Test vpx_codec_err_to_string with various error codes
    // Use fuzzer input to generate error codes
    if (Size > 0) {
        // Use first byte to generate error code
        vpx_codec_err_t err_code = (vpx_codec_err_t)(Data[0] % 256);
        const char *err_str = vpx_codec_err_to_string(err_code);
        if (err_str) {
            volatile char c = err_str[0];
            (void)c;
        }
    }
    
    // 3. Test vpx_codec_error_detail with a codec context
    // First create a dummy context structure
    vpx_codec_ctx_t ctx;
    
    // Initialize context fields with fuzzer data
    memset(&ctx, 0, sizeof(ctx));
    
    // Use fuzzer data to populate context fields if available
    if (Size >= sizeof(ctx)) {
        // Copy fuzzer data into context (but be careful with pointers)
        memcpy(&ctx, Data, sizeof(ctx) > Size ? Size : sizeof(ctx));
    } else if (Size > 0) {
        // Partial initialization
        memcpy(&ctx, Data, Size);
    }
    
    // Set some reasonable values to avoid completely invalid pointers
    ctx.name = "dummy";
    ctx.err = VPX_CODEC_OK;
    ctx.err_detail = NULL;
    ctx.init_flags = 0;
    ctx.config.raw = NULL;
    ctx.priv = NULL;
    ctx.iface = NULL;
    
    // Call vpx_codec_error_detail
    const char *error_detail = vpx_codec_error_detail(&ctx);
    if (error_detail) {
        volatile char c = error_detail[0];
        (void)c;
    }
    
    // 4. Test with different error states
    // Create another context with different error values
    if (Size > 1) {
        vpx_codec_ctx_t ctx2;
        memset(&ctx2, 0, sizeof(ctx2));
        
        // Use second byte for error code
        ctx2.err = (vpx_codec_err_t)(Data[1 % Size] % 256);
        ctx2.name = "dummy2";
        ctx2.err_detail = "test detail";
        
        const char *error_detail2 = vpx_codec_error_detail(&ctx2);
        if (error_detail2) {
            volatile char c = error_detail2[0];
            (void)c;
        }
    }
    
    // 5. Test error string conversion with more values from fuzzer input
    if (Size > 2) {
        for (size_t i = 0; i < Size && i < 10; i++) {
            vpx_codec_err_t err = (vpx_codec_err_t)(Data[i] % 256);
            const char *str = vpx_codec_err_to_string(err);
            if (str) {
                volatile char c = str[0];
                (void)c;
            }
        }
    }
    
    return 0;
}