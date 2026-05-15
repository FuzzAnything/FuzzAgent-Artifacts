// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Initialize decoder interface - using VP8 decoder
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    
    // Call vpx_codec_iface_name (handles null pointer gracefully)
    const char *name = vpx_codec_iface_name(iface);
    (void)name; // Avoid unused variable warning
    
    // Initialize decoder context
    vpx_codec_ctx_t codec;
    vpx_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1; // Use single thread for deterministic fuzzing
    
    // Call vpx_codec_dec_init_ver via the macro as specified
    vpx_codec_err_t init_result = vpx_codec_dec_init(&codec, iface, &cfg, 0);
    
    if (init_result == VPX_CODEC_OK) {
        // Call vpx_codec_decode with the fuzzer input
        vpx_codec_decode(&codec, Data, (unsigned int)Size, NULL, 0);
        
        // Call vpx_codec_get_frame to iterate over decoded frames
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&codec, &iter)) != NULL) {
            // Access some image properties to explore more states
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
            (void)img->d_w;
            (void)img->d_h;
            (void)img->planes[0];
            (void)img->stride[0];
        }
        
        // Also try decoding with NULL data to flush the decoder
        vpx_codec_decode(&codec, NULL, 0, NULL, 0);
        
        // Get any remaining frames after flush
        iter = NULL;
        while ((img = vpx_codec_get_frame(&codec, &iter)) != NULL) {
            (void)img;
        }
        
        // Call vpx_codec_destroy to clean up
        vpx_codec_destroy(&codec);
    } else {
        // If initialization failed, still try to destroy (should handle error)
        vpx_codec_destroy(&codec);
    }
    
    return 0;
}