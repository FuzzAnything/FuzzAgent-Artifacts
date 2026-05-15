// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_get_preview_frame at vpx_encoder.c:314:20 in vpx_encoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_err_t err;
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *img;
    
    // Initialize configuration with fuzzed data
    cfg.threads = (Size > 0) ? (Data[0] % 8) + 1 : 1;
    cfg.w = (Size > 1) ? (Data[1] * 16) : 320;
    cfg.h = (Size > 2) ? (Data[2] * 16) : 240;
    
    // Initialize decoder with version check
    err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Try with NULL config
        err = vpx_codec_dec_init_ver(&ctx, iface, NULL, 0, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            return 0;
        }
    }
    
    // Try to get preview frame (should be NULL for decoder)
    img = vpx_codec_get_preview_frame(&ctx);
    
    // Decode the input data
    if (Size > 0) {
        // Try different deadline values
        long deadlines[] = {0, 1000, 10000};
        for (int i = 0; i < 3; i++) {
            err = vpx_codec_decode(&ctx, Data, (unsigned int)Size, NULL, deadlines[i]);
            
            // Get error details if decode failed
            if (err != VPX_CODEC_OK) {
                const char *detail = vpx_codec_error_detail(&ctx);
                (void)detail; // Use detail to avoid unused variable warning
            }
            
            // Iterate through decoded frames
            while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
                // Access some image properties to explore more states
                (void)img->fmt;
                (void)img->w;
                (void)img->h;
                (void)img->d_w;
                (void)img->d_h;
                (void)img->bit_depth;
                (void)img->cs;
                (void)img->range;
            }
            iter = NULL;
            
            // Try decoding with NULL data to flush
            if (i == 1) {
                vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
                while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
                    (void)img;
                }
                iter = NULL;
            }
        }
    }
    
    // Try decoding fragmented input
    if (Size > 10) {
        // Split data into fragments
        size_t fragment_size = Size / 3;
        if (fragment_size > 0) {
            vpx_codec_decode(&ctx, Data, (unsigned int)fragment_size, NULL, 0);
            vpx_codec_decode(&ctx, Data + fragment_size, (unsigned int)fragment_size, NULL, 0);
            vpx_codec_decode(&ctx, Data + 2 * fragment_size, (unsigned int)(Size - 2 * fragment_size), NULL, 0);
            
            // Flush
            vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
            
            // Get all frames
            while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
                (void)img;
            }
            iter = NULL;
        }
    }
    
    // Get error details one more time
    const char *detail = vpx_codec_error_detail(&ctx);
    (void)detail;
    
    // Destroy the codec instance
    err = vpx_codec_destroy(&ctx);
    (void)err;
    
    return 0;
}