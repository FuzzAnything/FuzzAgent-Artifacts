// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_version_extra_str at vpx_codec.c:28:13 in vpx_codec.h
// vpx_codec_build_config at vpx_config.c:10:13 in vpx_codec.h
// vpx_codec_version_str at vpx_codec.c:26:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Always call version functions - they're safe and don't require setup
    int version = vpx_codec_version();
    const char *version_extra = vpx_codec_version_extra_str();
    const char *build_config = vpx_codec_build_config();
    const char *version_str = vpx_codec_version_str();
    
    // Use the version number to decode major/minor/patch
    // This explores program states based on version macros
    int major = (version >> 16) & 0xff;
    int minor = (version >> 8) & 0xff;
    int patch = version & 0xff;
    
    // Test vpx_codec_err_to_string with various error codes
    // Use fuzzer data to generate different error codes
    if (Size > 0) {
        // Use first byte to generate error codes within reasonable range
        vpx_codec_err_t err = (vpx_codec_err_t)(Data[0] % 20); // Limit to reasonable range
        const char *err_str = vpx_codec_err_to_string(err);
        
        // Also test with some known error codes
        const char *known_errs[] = {
            vpx_codec_err_to_string(VPX_CODEC_OK),
            vpx_codec_err_to_string(VPX_CODEC_ERROR),
            vpx_codec_err_to_string(VPX_CODEC_MEM_ERROR),
            vpx_codec_err_to_string(VPX_CODEC_ABI_MISMATCH),
            vpx_codec_err_to_string(VPX_CODEC_INCAPABLE),
            vpx_codec_err_to_string(VPX_CODEC_UNSUP_BITSTREAM),
            vpx_codec_err_to_string(VPX_CODEC_UNSUP_FEATURE),
            vpx_codec_err_to_string(VPX_CODEC_CORRUPT_FRAME),
            vpx_codec_err_to_string(VPX_CODEC_INVALID_PARAM),
            vpx_codec_err_to_string(VPX_CODEC_LIST_END)
        };
    }
    
    // Test vpx_codec_error_detail with a dummy context
    // We need to create a minimal context to pass to the function
    if (Size > sizeof(vpx_codec_ctx_t)) {
        // Create a context on the stack
        vpx_codec_ctx_t ctx;
        
        // Initialize with some data from fuzzer input
        // We'll copy what we can from the fuzzer data
        size_t copy_size = Size < sizeof(ctx) ? Size : sizeof(ctx);
        memcpy(&ctx, Data, copy_size);
        
        // Ensure the context has some reasonable values to avoid crashes
        ctx.name = "dummy";
        ctx.err = VPX_CODEC_OK;
        ctx.err_detail = NULL;
        ctx.init_flags = 0;
        ctx.config.raw = NULL;
        ctx.priv = NULL;
        
        // Call the target function
        const char *error_detail = vpx_codec_error_detail(&ctx);
        
        // Test with different error states
        ctx.err = VPX_CODEC_ERROR;
        error_detail = vpx_codec_error_detail(&ctx);
        
        ctx.err = VPX_CODEC_MEM_ERROR;
        error_detail = vpx_codec_error_detail(&ctx);
        
        ctx.err = VPX_CODEC_INVALID_PARAM;
        error_detail = vpx_codec_error_detail(&ctx);
    }
    
    // Create more diverse test cases by using different parts of the input
    if (Size > 1) {
        // Use second byte for another error code test
        vpx_codec_err_t err2 = (vpx_codec_err_t)(Data[1] % 30);
        const char *err_str2 = vpx_codec_err_to_string(err2);
    }
    
    // Test with NULL context for error_detail (should return NULL)
    const char *null_detail = vpx_codec_error_detail(NULL);
    
    return 0;
}