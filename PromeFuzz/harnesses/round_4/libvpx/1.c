// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"

// Declare the decoder interface functions that are typically available
extern vpx_codec_iface_t* vpx_codec_vp8_dx(void);
extern vpx_codec_iface_t* vpx_codec_vp9_dx(void);

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Try to get decoder interfaces
    vpx_codec_iface_t *iface = NULL;
    
    // First try VP8 decoder
    iface = vpx_codec_vp8_dx();
    
    // If VP8 not available, try VP9
    if (!iface) {
        iface = vpx_codec_vp9_dx();
    }
    
    // If no decoder interface is available, we can't proceed
    if (!iface) {
        // Still test vpx_codec_iface_name with NULL
        const char *name = vpx_codec_iface_name(NULL);
        (void)name;
        return 0;
    }
    
    // Call vpx_codec_iface_name
    const char *name = vpx_codec_iface_name(iface);
    (void)name;
    
    // Initialize decoder context
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg = {0};
    
    // Use fuzzer data to set some configuration parameters
    if (Size >= 2) {
        cfg.threads = Data[0] % 8 + 1;
        cfg.w = (Data[1] % 64 + 1) * 16;
        cfg.h = ((Data[1] >> 4) % 64 + 1) * 16;
    } else {
        cfg.threads = 1;
        cfg.w = 320;
        cfg.h = 240;
    }
    
    // Initialize decoder with version check
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Try with NULL config
        err = vpx_codec_dec_init_ver(&ctx, iface, NULL, 0, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            // Test vpx_codec_destroy with uninitialized context
            vpx_codec_destroy(&ctx);
            return 0;
        }
    }
    
    // Decode the input data
    if (Size > 0) {
        // First decode with the provided data
        err = vpx_codec_decode(&ctx, Data, (unsigned int)Size, NULL, 0);
        (void)err;
        
        // Then try to flush the decoder
        err = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        (void)err;
        
        // Try fragmented input if we have enough data
        if (Size >= 2) {
            size_t split = Size / 2;
            err = vpx_codec_decode(&ctx, Data, (unsigned int)split, NULL, 0);
            (void)err;
            err = vpx_codec_decode(&ctx, Data + split, (unsigned int)(Size - split), NULL, 0);
            (void)err;
            err = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
            (void)err;
        }
    } else {
        // Try decode with NULL data (flush)
        err = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        (void)err;
    }
    
    // Get decoded frames
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        // Access some image properties to explore more states
        (void)img->fmt;
        (void)img->w;
        (void)img->h;
        (void)img->d_w;
        (void)img->d_h;
        (void)img->r_w;
        (void)img->r_h;
        (void)img->bit_depth;
        (void)img->cs;
        (void)img->range;
    }
    
    // Destroy the decoder context
    err = vpx_codec_destroy(&ctx);
    (void)err;
    
    return 0;
}