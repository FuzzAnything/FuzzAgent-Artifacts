// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_version_str at vpx_codec.c:26:13 in vpx_codec.h
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_build_config at vpx_config.c:10:13 in vpx_codec.h
// vpx_codec_version_extra_str at vpx_codec.c:28:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_vpx_codec_err_to_string(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use the first byte to generate an error code
    vpx_codec_err_t err = (vpx_codec_err_t)data[0];
    const char *err_str = vpx_codec_err_to_string(err);
    (void)err_str; // Use the result to avoid compiler warnings
}

static void fuzz_vpx_codec_error_detail(const uint8_t *data, size_t size) {
    // Create a dummy codec context structure
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Initialize some fields with fuzzer data
    if (size > 0) {
        // Use first byte for error code
        ctx.err = (vpx_codec_err_t)data[0];
    }
    
    if (size > 1) {
        // Use second byte for init_flags
        ctx.init_flags = (vpx_codec_flags_t)data[1];
    }
    
    // We cannot allocate vpx_codec_priv_t since it's opaque/incomplete
    // Just set priv to NULL and let the function handle it
    ctx.priv = NULL;
    
    // Call the target function
    const char *detail = vpx_codec_error_detail(&ctx);
    (void)detail;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *data, size_t size) {
    // Call functions that don't require input
    const char *version_str = vpx_codec_version_str();
    (void)version_str;
    
    int version = vpx_codec_version();
    (void)version;
    
    const char *build_config = vpx_codec_build_config();
    (void)build_config;
    
    const char *version_extra = vpx_codec_version_extra_str();
    (void)version_extra;
    
    // Call functions that use fuzzer input
    fuzz_vpx_codec_err_to_string(data, size);
    fuzz_vpx_codec_error_detail(data, size);
    
    return 0;
}