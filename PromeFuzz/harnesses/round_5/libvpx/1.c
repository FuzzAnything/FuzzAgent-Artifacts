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

// Declare the decoder interface functions that are missing from headers
// These are typically defined in the library but not in the headers we have
extern const vpx_codec_iface_t *vpx_codec_vp8_dx(void);
extern const vpx_codec_iface_t *vpx_codec_vp9_dx(void);

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Initialize decoder context
    vpx_codec_ctx_t codec_ctx;
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    
    // Try both VP8 and VP9 decoder interfaces if available
    const vpx_codec_iface_t* interfaces[2] = {NULL, NULL};
    
    // Get VP8 interface if available
    if (vpx_codec_vp8_dx) {
        interfaces[0] = vpx_codec_vp8_dx();
    }
    
    // Get VP9 interface if available  
    if (vpx_codec_vp9_dx) {
        interfaces[1] = vpx_codec_vp9_dx();
    }
    
    for (int i = 0; i < 2; i++) {
        if (!interfaces[i]) {
            continue;
        }
        
        vpx_codec_iface_t* iface = (vpx_codec_iface_t*)interfaces[i];
        
        // 1. Get interface name
        const char* name = vpx_codec_iface_name(iface);
        (void)name; // Use name to avoid unused variable warning
        
        // 2. Initialize decoder with default configuration
        vpx_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        cfg.w = 640;
        cfg.h = 480;
        
        vpx_codec_err_t init_err = vpx_codec_dec_init_ver(&codec_ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
        if (init_err != VPX_CODEC_OK) {
            continue; // Try next interface if initialization fails
        }
        
        // 3. Decode the input data
        // Pass the fuzzer input as compressed video data
        vpx_codec_err_t decode_err = vpx_codec_decode(&codec_ctx, Data, (unsigned int)Size, NULL, 0);
        (void)decode_err; // Ignore decode error for fuzzing
        
        // 4. Get decoded frames
        vpx_codec_iter_t iter = NULL;
        vpx_image_t* img;
        while ((img = vpx_codec_get_frame(&codec_ctx, &iter)) != NULL) {
            // Access some fields to exercise the code
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
            (void)img->planes[0];
        }
        
        // 5. Destroy the codec instance
        vpx_codec_err_t destroy_err = vpx_codec_destroy(&codec_ctx);
        (void)destroy_err; // Ignore destroy error for fuzzing
        
        // Break after first successful initialization
        if (init_err == VPX_CODEC_OK) {
            break;
        }
    }
    
    return 0;
}