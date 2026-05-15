// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"

// Declare the decoder interface functions that are defined in the library
// These are typically defined in vpx_decoder.h but we need to declare them
extern vpx_codec_iface_t *vpx_codec_vp8_dx(void);
extern vpx_codec_iface_t *vpx_codec_vp9_dx(void);

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Initialize decoder context
    vpx_codec_ctx_t ctx;
    
    // Get decoder interface - try both VP8 and VP9 decoders
    vpx_codec_iface_t *iface = NULL;
    
    // First try VP8 decoder
    iface = vpx_codec_vp8_dx();
    if (!iface) {
        // If VP8 decoder is not available, try VP9 decoder
        iface = vpx_codec_vp9_dx();
    }
    
    if (!iface) {
        // No decoder interface available
        return 0;
    }
    
    // Call vpx_codec_iface_name
    const char *name = vpx_codec_iface_name(iface);
    (void)name; // Use the result to avoid unused variable warning
    
    // Initialize decoder configuration
    vpx_codec_dec_cfg_t cfg;
    cfg.threads = 1;
    cfg.w = 640;
    cfg.h = 480;
    
    // Initialize decoder with version check
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (err == VPX_CODEC_OK) {
        // Decode the input data
        err = vpx_codec_decode(&ctx, Data, (unsigned int)Size, NULL, 0);
        
        if (err == VPX_CODEC_OK) {
            // Get decoded frames
            vpx_codec_iter_t iter = NULL;
            vpx_image_t *img;
            
            while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
                // Access image data to explore more states
                if (img->planes[0] && img->stride[0] > 0 && img->h > 0) {
                    // Try to access a small portion of the image data
                    volatile uint8_t dummy = 0;
                    for (unsigned int i = 0; i < img->h && i < 10; i++) {
                        if (i * img->stride[0] < 100) {
                            dummy = img->planes[0][i * img->stride[0]];
                        }
                    }
                    (void)dummy;
                }
            }
        }
        
        // Destroy the decoder
        vpx_codec_destroy(&ctx);
    }
    
    return 0;
}