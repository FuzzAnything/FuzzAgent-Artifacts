// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_decoder.h"

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Get the AV1 decoder interface
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
    if (Size > 0) {
        aom_codec_stream_info_t si;
        // Initialize required field
        si.is_annexb = (Data[0] & 1); // Use first byte to determine annexb flag
        aom_codec_peek_stream_info(iface, Data, Size, &si);
    }

    // Test decoder initialization and decoding
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg = {0};
    
    // Use some data to influence initialization parameters
    if (Size >= 1) {
        cfg.threads = (Data[0] % 8) + 1; // 1-8 threads
    }

    // Initialize decoder
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (init_result == AOM_CODEC_OK) {
        // Test decoding with the input data
        if (Size > 0) {
            aom_codec_decode(&ctx, Data, Size, NULL);
        }
        
        // Clean up
        aom_codec_destroy(&ctx);
    }

    return 0;
}