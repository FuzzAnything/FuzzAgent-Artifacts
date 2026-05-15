// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void use_string(const char *str) {
    if (str && str[0]) {
        volatile char c = str[0];
        (void)c;
    }
}

static void test_error_detail(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Set up some minimal context structure
    ctx.err = AOM_CODEC_ERROR;  // Set an error state
    ctx.err_detail = NULL;
    
    // Use fuzz data to influence context state
    if (size > 0) {
        ctx.err = (aom_codec_err_t)(data[0] % 10);  // Use first byte for error code
        if (size > 1 && (data[1] % 2)) {
            // Simulate having error detail sometimes
            static const char dummy_detail[] = "Fuzzed error detail";
            ctx.err_detail = dummy_detail;
        }
    }
    
    const char *detail = aom_codec_error_detail(&ctx);
    use_string(detail);
}

int LLVMFuzzerTestOneInput_10(const uint8_t *data, size_t size) {
    // Test aom_codec_build_config
    const char *build_config = aom_codec_build_config();
    use_string(build_config);
    
    // Test aom_codec_version_str
    const char *version_str = aom_codec_version_str();
    use_string(version_str);
    
    // Test aom_codec_version
    int version = aom_codec_version();
    volatile int v = version;
    (void)v;
    
    // Test aom_codec_version_extra_str
    const char *extra_str = aom_codec_version_extra_str();
    use_string(extra_str);
    
    // Test aom_codec_err_to_string with various error codes
    if (size > 0) {
        // Use fuzz data to test different error codes
        for (int i = 0; i < 10 && i < (int)size; i++) {
            aom_codec_err_t err = (aom_codec_err_t)(data[i] % 10);
            const char *err_str = aom_codec_err_to_string(err);
            use_string(err_str);
        }
        
        // Also test some known error codes
        const char *known_errors[] = {
            aom_codec_err_to_string(AOM_CODEC_OK),
            aom_codec_err_to_string(AOM_CODEC_ERROR),
            aom_codec_err_to_string(AOM_CODEC_MEM_ERROR),
            aom_codec_err_to_string(AOM_CODEC_ABI_MISMATCH),
            aom_codec_err_to_string(AOM_CODEC_INCAPABLE),
            aom_codec_err_to_string(AOM_CODEC_UNSUP_BITSTREAM),
            aom_codec_err_to_string(AOM_CODEC_UNSUP_FEATURE),
            aom_codec_err_to_string(AOM_CODEC_CORRUPT_FRAME),
            aom_codec_err_to_string(AOM_CODEC_INVALID_PARAM),
            aom_codec_err_to_string(AOM_CODEC_LIST_END)
        };
        
        for (int i = 0; i < 10; i++) {
            use_string(known_errors[i]);
        }
    }
    
    // Test aom_codec_error_detail with fuzzed context
    test_error_detail(data, size);
    
    return 0;
}