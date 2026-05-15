// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

static int get_frame_buffer(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    static uint8_t buffer[1024 * 1024]; // 1MB buffer
    if (min_size > sizeof(buffer)) {
        return -1;
    }
    fb->data = buffer;
    fb->size = sizeof(buffer);
    fb->priv = NULL;
    return 0;
}

static int release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    // No-op for fuzzing
    return 0;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use first byte to control fuzzing behavior
    uint8_t mode = Data[0];
    const uint8_t *stream_data = Data + 1;
    size_t stream_size = Size - 1;

    // Test aom_codec_peek_stream_info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.is_annexb = mode & 1; // Initialize is_annexb based on mode
    
    aom_codec_err_t err = aom_codec_peek_stream_info(
        aom_codec_av1_dx(), 
        stream_data, 
        stream_size, 
        &si
    );

    // Test decoder initialization and operations
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;
    
    aom_codec_flags_t flags = 0;
    
    // Initialize decoder
    err = aom_codec_dec_init(&ctx, aom_codec_av1_dx(), &cfg, flags);
    if (err == AOM_CODEC_OK) {
        // Test setting frame buffer functions
        if (mode & 4) {
            aom_codec_set_frame_buffer_functions(
                &ctx, 
                get_frame_buffer, 
                release_frame_buffer, 
                NULL
            );
        }
        
        // Test decode if we have stream data
        if (stream_size > 0) {
            aom_codec_decode(&ctx, stream_data, stream_size, NULL);
            
            // Test get stream info after decode
            aom_codec_stream_info_t si2;
            memset(&si2, 0, sizeof(si2));
            si2.is_annexb = mode & 1;
            aom_codec_get_stream_info(&ctx, &si2);
        }
        
        // Clean up
        aom_codec_destroy(&ctx);
    }
    
    return 0;
}