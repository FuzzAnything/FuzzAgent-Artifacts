// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_version_str at vpx_codec.c:26:13 in vpx_codec.h
// vpx_codec_version_extra_str at vpx_codec.c:28:13 in vpx_codec.h
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_build_config at vpx_config.c:10:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Call simple version functions that don't require context
    const char *version_str = vpx_codec_version_str();
    const char *extra_str = vpx_codec_version_extra_str();
    int version = vpx_codec_version();
    const char *build_config = vpx_codec_build_config();
    
    // Use the version information to avoid compiler warnings about unused variables
    if (version_str && extra_str && build_config) {
        // Do nothing, just reference them
        (void)version_str;
        (void)extra_str;
        (void)version;
        (void)build_config;
    }
    
    // Test vpx_codec_err_to_string with various error codes
    // Use fuzzer input to select error codes
    if (Size > 0) {
        // Use first byte to select error code
        uint8_t err_selector = Data[0];
        
        // Map selector to various error codes (including invalid ones)
        vpx_codec_err_t err;
        switch (err_selector % 8) {
            case 0: err = VPX_CODEC_OK; break;
            case 1: err = VPX_CODEC_ERROR; break;
            case 2: err = VPX_CODEC_MEM_ERROR; break;
            case 3: err = VPX_CODEC_ABI_MISMATCH; break;
            case 4: err = VPX_CODEC_INCAPABLE; break;
            case 5: err = VPX_CODEC_UNSUP_BITSTREAM; break;
            case 6: err = VPX_CODEC_UNSUP_FEATURE; break;
            case 7: err = (vpx_codec_err_t)0xFFFFFFFF; break; // Invalid error code
            default: err = VPX_CODEC_OK; break;
        }
        
        const char *err_string = vpx_codec_err_to_string(err);
        (void)err_string;
    }
    
    // Test vpx_codec_error_detail - but we need a valid context
    // Instead of creating a fake context, we'll skip this test for now
    // since we don't have a way to properly initialize a codec context
    // without calling actual codec initialization functions
    
    return 0;
}