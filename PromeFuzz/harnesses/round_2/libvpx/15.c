// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_preview_frame at vpx_encoder.c:314:20 in vpx_encoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize decoder context
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Choose codec interface based on first byte
    vpx_codec_iface_t *iface = NULL;
    if (Data[0] & 1) {
        // VP9 decoder interface
        extern vpx_codec_iface_t vpx_codec_vp9_dx_algo;
        iface = &vpx_codec_vp9_dx_algo;
    } else {
        // VP8 decoder interface
        extern vpx_codec_iface_t vpx_codec_vp8_dx_algo;
        iface = &vpx_codec_vp8_dx_algo;
    }
    
    // Initialize decoder configuration
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = (Data[0] & 0x0F) + 1; // 1-16 threads
    cfg.w = 320;
    cfg.h = 240;
    
    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Try to decode the input data
    if (Size > 1) {
        const uint8_t *decode_data = Data + 1;
        size_t decode_size = Size - 1;
        
        // Decode in fragments if data suggests it
        if (decode_size > 100) {
            // Split into two fragments
            size_t first_frag = decode_size / 2;
            vpx_codec_decode(&ctx, decode_data, (unsigned int)first_frag, NULL, 0);
            vpx_codec_decode(&ctx, decode_data + first_frag, 
                           (unsigned int)(decode_size - first_frag), NULL, 0);
        } else {
            vpx_codec_decode(&ctx, decode_data, (unsigned int)decode_size, NULL, 0);
        }
        
        // Flush decoder
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    }
    
    // Try to get preview frame (should return NULL for decoder)
    const vpx_image_t *preview = vpx_codec_get_preview_frame(&ctx);
    (void)preview; // Preview may be NULL, that's expected
    
    // Iterate through decoded frames
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *frame;
    while ((frame = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        // Access some frame properties to explore more states
        (void)frame->fmt;
        (void)frame->w;
        (void)frame->h;
        (void)frame->d_w;
        (void)frame->d_h;
        (void)frame->r_w;
        (void)frame->r_h;
        (void)frame->x_chroma_shift;
        (void)frame->y_chroma_shift;
        (void)frame->cs;
        (void)frame->range;
        (void)frame->bit_depth;
        (void)frame->bps;
        
        // Try to access plane data (but don't write to it)
        if (frame->planes[0]) {
            (void)frame->planes[0][0];
        }
        if (frame->planes[1]) {
            (void)frame->planes[1][0];
        }
        if (frame->planes[2]) {
            (void)frame->planes[2][0];
        }
        if (frame->planes[3]) {
            (void)frame->planes[3][0];
        }
    }
    
    // Try various control operations using generic control IDs
    // Use some common control IDs that should be available
    int int_val = 0;
    
    // Try VP8 decoder controls (if available)
    vpx_codec_control_(&ctx, 256, &int_val);  // VP8D_GET_LAST_REF_UPDATES
    vpx_codec_control_(&ctx, 257, &int_val);  // VP8D_GET_FRAME_CORRUPTED
    
    // For VP9 decoder controls
    if (Data[0] & 1) {
        vpx_codec_control_(&ctx, 9, &int_val);   // VP9D_GET_DISPLAY_SIZE
        vpx_codec_control_(&ctx, 10, &int_val);  // VP9D_GET_FRAME_SIZE
    }
    
    // Try some common codec controls
    vpx_codec_control_(&ctx, 8, &int_val);   // VP8_COPY_REFERENCE
    vpx_codec_control_(&ctx, 1, &int_val);   // VP8_SET_REFERENCE
    
    // Clean up
    vpx_codec_destroy(&ctx);
    
    return 0;
}