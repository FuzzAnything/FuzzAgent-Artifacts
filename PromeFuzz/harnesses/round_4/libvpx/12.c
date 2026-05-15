// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_version_str at vpx_codec.c:26:13 in vpx_codec.h
// vpx_codec_version_extra_str at vpx_codec.c:28:13 in vpx_codec.h
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_build_config at vpx_config.c:10:13 in vpx_codec.h
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

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Call version functions that don't require initialization
    const char *version_str = vpx_codec_version_str();
    const char *extra_str = vpx_codec_version_extra_str();
    int version = vpx_codec_version();
    const char *build_config = vpx_codec_build_config();
    
    // Use volatile to prevent optimization
    volatile const char *v1 = version_str;
    volatile const char *v2 = extra_str;
    volatile int v3 = version;
    volatile const char *v4 = build_config;
    
    // Test vpx_codec_err_to_string with various error codes
    // Use fuzz data to select error codes
    if (Size > 0) {
        // Create error codes from fuzz data
        for (size_t i = 0; i < Size && i < 10; i++) {
            vpx_codec_err_t err = (vpx_codec_err_t)Data[i];
            const char *err_str = vpx_codec_err_to_string(err);
            volatile const char *v5 = err_str;
        }
    }
    
    // Test vpx_codec_error_detail with a dummy context
    // We need to create a vpx_codec_ctx_t structure
    static vpx_codec_ctx_t ctx;
    
    // Initialize context with fuzz data if available
    if (Size >= sizeof(vpx_codec_ctx_t)) {
        // Copy fuzz data into context (this may create invalid pointers,
        // but that's acceptable for fuzzing)
        memcpy(&ctx, Data, sizeof(vpx_codec_ctx_t));
        
        // Create a dummy private structure - we can't use vpx_codec_priv_t
        // directly since it's opaque, so we'll allocate some memory
        static uint8_t dummy_priv[256];
        if (Size >= sizeof(vpx_codec_ctx_t) + sizeof(dummy_priv)) {
            memcpy(dummy_priv, Data + sizeof(vpx_codec_ctx_t), sizeof(dummy_priv));
        } else if (Size > sizeof(vpx_codec_ctx_t)) {
            memcpy(dummy_priv, Data + sizeof(vpx_codec_ctx_t), 
                   Size - sizeof(vpx_codec_ctx_t));
        }
        ctx.priv = (vpx_codec_priv_t*)dummy_priv;
    } else {
        // Minimal initialization
        ctx.name = NULL;
        ctx.iface = NULL;
        ctx.err = (vpx_codec_err_t)0;
        ctx.err_detail = NULL;
        ctx.init_flags = 0;
        ctx.config.raw = NULL;
        
        // Create a dummy private structure
        static uint8_t dummy_priv[256];
        if (Size > 0) {
            memcpy(dummy_priv, Data, Size < sizeof(dummy_priv) ? Size : sizeof(dummy_priv));
        }
        ctx.priv = (vpx_codec_priv_t*)dummy_priv;
    }
    
    // Call error_detail function
    const char *error_detail = vpx_codec_error_detail(&ctx);
    volatile const char *v6 = error_detail;
    
    // Additional test: use version macros to decode version
    if (Size > 0) {
        volatile int major = (version >> 16) & 0xff;
        volatile int minor = (version >> 8) & 0xff;
        volatile int patch = version & 0xff;
    }
    
    return 0;
}