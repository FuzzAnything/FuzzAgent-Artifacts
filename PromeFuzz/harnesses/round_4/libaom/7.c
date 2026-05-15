// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_error_functions(const aom_codec_ctx_t *ctx) {
    // Test aom_codec_error with provided context
    const char *error_str = aom_codec_error(ctx);
    (void)error_str; // Use to avoid unused variable warning
    
    // Test aom_codec_error_detail with provided context
    const char *error_detail = aom_codec_error_detail(ctx);
    (void)error_detail; // Use to avoid unused variable warning
}

static void test_version_functions(void) {
    // Test aom_codec_version
    int version = aom_codec_version();
    (void)version; // Use to avoid unused variable warning
    
    // Test aom_codec_version_str
    const char *version_str = aom_codec_version_str();
    (void)version_str; // Use to avoid unused variable warning
    
    // Test aom_codec_version_extra_str
    const char *version_extra_str = aom_codec_version_extra_str();
    (void)version_extra_str; // Use to avoid unused variable warning
    
    // Test aom_codec_build_config
    const char *build_config = aom_codec_build_config();
    (void)build_config; // Use to avoid unused variable warning
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Test version functions (don't require context)
    test_version_functions();
    
    // Test error functions with NULL context
    test_error_functions(NULL);
    
    // Create a dummy context structure with fuzzer-provided data
    // We'll use the fuzzer input to partially initialize the structure
    aom_codec_ctx_t ctx;
    
    // Initialize context fields with fuzzer data
    // We need to be careful about pointer fields - we'll set them to NULL
    // or to point to valid memory if we can allocate it
    
    // Clear the structure first
    memset(&ctx, 0, sizeof(ctx));
    
    // Use fuzzer data to set some fields if we have enough data
    if (Size >= sizeof(ctx)) {
        // Copy as much data as we can into the context
        memcpy(&ctx, Data, Size < sizeof(ctx) ? Size : sizeof(ctx));
    }
    
    // Ensure pointer fields are either NULL or point to valid memory
    // We'll set them to NULL for safety
    ctx.name = NULL;
    ctx.iface = NULL;
    ctx.config.raw = NULL;
    ctx.priv = NULL;
    
    // Set err to a random value from fuzzer data if available
    if (Size >= sizeof(ctx.err) + offsetof(aom_codec_ctx_t, err)) {
        memcpy(&ctx.err, Data + offsetof(aom_codec_ctx_t, err), sizeof(ctx.err));
    }
    
    // Set err_detail to NULL since we don't have valid string
    ctx.err_detail = NULL;
    
    // Set init_flags to a random value from fuzzer data if available
    if (Size >= sizeof(ctx.init_flags) + offsetof(aom_codec_ctx_t, init_flags)) {
        memcpy(&ctx.init_flags, Data + offsetof(aom_codec_ctx_t, init_flags), sizeof(ctx.init_flags));
    }
    
    // Test error functions with our partially initialized context
    test_error_functions(&ctx);
    
    // Create another test with all NULL pointers
    aom_codec_ctx_t null_ctx;
    memset(&null_ctx, 0, sizeof(null_ctx));
    test_error_functions(&null_ctx);
    
    // Test with a context where err_detail points to something
    aom_codec_ctx_t ctx_with_detail;
    memset(&ctx_with_detail, 0, sizeof(ctx_with_detail));
    
    // Set err_detail to point to a static string or NULL
    if (Size > 0 && Data[0] % 2 == 0) {
        // Use a static string for error detail
        ctx_with_detail.err_detail = "Test error detail";
    }
    
    test_error_functions(&ctx_with_detail);
    
    return 0;
}