// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_version_str at vpx_codec.c:26:13 in vpx_codec.h
// vpx_codec_version_extra_str at vpx_codec.c:28:13 in vpx_codec.h
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_build_config at vpx_config.c:10:13 in vpx_codec.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"

// Define version macros if they're not already defined
#ifndef VERSION_MAJOR
#define VERSION_MAJOR(x) (((x) >> 16) & 0xff)
#endif

#ifndef VERSION_MINOR
#define VERSION_MINOR(x) (((x) >> 8) & 0xff)
#endif

#ifndef VERSION_PATCH
#define VERSION_PATCH(x) ((x) & 0xff)
#endif

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Call simple functions that don't require setup
    const char *version_str = vpx_codec_version_str();
    const char *extra_str = vpx_codec_version_extra_str();
    int version = vpx_codec_version();
    const char *build_config = vpx_codec_build_config();
    
    // Test vpx_codec_err_to_string with various error codes
    const char *err_str;
    err_str = vpx_codec_err_to_string(VPX_CODEC_OK);
    err_str = vpx_codec_err_to_string(VPX_CODEC_ERROR);
    err_str = vpx_codec_err_to_string(VPX_CODEC_MEM_ERROR);
    err_str = vpx_codec_err_to_string(VPX_CODEC_ABI_MISMATCH);
    err_str = vpx_codec_err_to_string(VPX_CODEC_INCAPABLE);
    err_str = vpx_codec_err_to_string(VPX_CODEC_UNSUP_BITSTREAM);
    err_str = vpx_codec_err_to_string(VPX_CODEC_UNSUP_FEATURE);
    err_str = vpx_codec_err_to_string(VPX_CODEC_CORRUPT_FRAME);
    err_str = vpx_codec_err_to_string(VPX_CODEC_INVALID_PARAM);
    err_str = vpx_codec_err_to_string(VPX_CODEC_LIST_END);
    
    // Also test some random error codes based on input data
    if (Size > 0) {
        for (size_t i = 0; i < Size && i < 10; i++) {
            err_str = vpx_codec_err_to_string((vpx_codec_err_t)Data[i]);
        }
    }
    
    // Test vpx_codec_error_detail - we need a codec context
    vpx_codec_ctx_t codec_ctx;
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    
    // Initialize the context with some dummy values to potentially trigger errors
    codec_ctx.err = VPX_CODEC_ERROR;
    if (Size > 0) {
        // Use first byte to set error code for testing
        codec_ctx.err = (vpx_codec_err_t)(Data[0] % (VPX_CODEC_LIST_END + 1));
    }
    
    // Call error_detail with our context
    const char *error_detail = vpx_codec_error_detail(&codec_ctx);
    
    // Also test with NULL context
    error_detail = vpx_codec_error_detail(NULL);
    
    // Use the version macros to decode the packed version integer
    int major = VERSION_MAJOR(version);
    int minor = VERSION_MINOR(version);
    int patch = VERSION_PATCH(version);
    
    // Prevent unused variable warnings
    (void)version_str;
    (void)extra_str;
    (void)build_config;
    (void)err_str;
    (void)error_detail;
    (void)major;
    (void)minor;
    (void)patch;
    
    return 0;
}