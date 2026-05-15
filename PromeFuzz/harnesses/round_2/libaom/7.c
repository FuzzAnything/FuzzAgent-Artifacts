// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_decoder.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Get AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    // Test aom_codec_get_caps
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps; // Use result to avoid unused variable warning

    // Test aom_codec_iface_name
    const char *name = aom_codec_iface_name(iface);
    (void)name; // Use result to avoid unused variable warning

    // Test aom_codec_peek_stream_info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    // Initialize is_annexb as required by the API
    si.is_annexb = (Size > 0) ? (Data[0] & 1) : 0;
    aom_codec_peek_stream_info(iface, Data, Size, &si);

    // Test decoder initialization and decoding
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    // Set some configuration values based on fuzzer input
    if (Size > 0) {
        cfg.w = (Data[0] << 8) | (Size > 1 ? Data[1] : 0);
        cfg.h = (Size > 2 ? Data[2] << 8 : 0) | (Size > 3 ? Data[3] : 0);
        cfg.threads = Size > 4 ? Data[4] : 1;
    } else {
        cfg.w = 320;
        cfg.h = 240;
        cfg.threads = 1;
    }
    
    // Set flags based on input
    aom_codec_flags_t flags = 0;
    if (Size > 5) {
        flags = Data[5] & 0xFF;
    }

    // Initialize decoder
    aom_codec_err_t init_result = aom_codec_dec_init(&ctx, iface, &cfg, flags);
    
    if (init_result == AOM_CODEC_OK) {
        // Test decoding with the fuzzer input
        aom_codec_decode(&ctx, Data, Size, NULL);
        
        // Try decoding with different user_priv values
        static int user_priv_counter = 0;
        user_priv_counter++;
        aom_codec_decode(&ctx, Data, Size, (void *)(intptr_t)user_priv_counter);
        
        // Try decoding with empty data
        if (Size > 0) {
            aom_codec_decode(&ctx, NULL, 0, NULL);
        }
        
        // Destroy decoder context
        aom_codec_destroy(&ctx);
    }

    // Test with NULL config
    aom_codec_ctx_t ctx2;
    aom_codec_err_t init_result2 = aom_codec_dec_init(&ctx2, iface, NULL, 0);
    if (init_result2 == AOM_CODEC_OK) {
        aom_codec_decode(&ctx2, Data, Size, NULL);
        aom_codec_destroy(&ctx2);
    }

    // Test aom_codec_get_caps with NULL pointer
    aom_codec_caps_t null_caps = aom_codec_get_caps(NULL);
    (void)null_caps;

    // Test aom_codec_iface_name with NULL pointer
    const char *null_name = aom_codec_iface_name(NULL);
    (void)null_name;

    return 0;
}

#ifdef __cplusplus
}
#endif