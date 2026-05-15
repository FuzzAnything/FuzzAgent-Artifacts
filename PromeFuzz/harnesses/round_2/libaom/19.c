// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom_codec.h"
#include "aomdx.h"
#include "aom_decoder.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Initialize decoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // Initialize decoder with default config
    aom_codec_err_t init_result = aom_codec_dec_init(&ctx, iface, NULL, 0);
    if (init_result != AOM_CODEC_OK) {
        // If initialization fails, just return - decoder may not be usable
        return 0;
    }
    
    // Try to decode the fuzzer input
    aom_codec_decode(&ctx, Data, Size, NULL);
    
    // Try to get stream info after decoding
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_get_stream_info(&ctx, &si);
    
    // Try some common decoder controls to explore more states
    int value;
    
    // AOMD_GET_FRAME_CORRUPTED control
    value = 0;
    aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &value);
    
    // AOMD_GET_LAST_REF_UPDATES control
    value = 0;
    aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, &value);
    
    // Try decoding again with different parameters
    if (Size > 0) {
        // Try decoding in chunks if we have enough data
        size_t chunk_size = Size / 2;
        if (chunk_size > 0) {
            aom_codec_decode(&ctx, Data, chunk_size, NULL);
            aom_codec_decode(&ctx, Data + chunk_size, Size - chunk_size, NULL);
        }
        
        // Try getting stream info again
        aom_codec_get_stream_info(&ctx, &si);
    }
    
    // Clean up
    aom_codec_destroy(&ctx);
    
    return 0;
}

#ifdef __cplusplus
}
#endif