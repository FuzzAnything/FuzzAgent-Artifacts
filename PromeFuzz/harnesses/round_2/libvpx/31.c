// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
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
#include "vpx/vp8dx.h"

static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
}

static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *rect,
                               const vpx_image_rect_t *valid_rect) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)valid_rect;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t codec;
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    
    // Try VP9 decoder as well
    vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_dx();
    
    // Initialize decoder with default configuration
    vpx_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 320;
    cfg.h = 240;
    
    // Try initialization with VP8 decoder
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(&codec, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_result == VPX_CODEC_OK) {
        // Try registering callbacks
        vpx_codec_register_put_frame_cb(&codec, dummy_put_frame_cb, NULL);
        vpx_codec_register_put_slice_cb(&codec, dummy_put_slice_cb, NULL);
        
        // Decode the input data
        vpx_codec_decode(&codec, Data, Size, NULL, 0);
        
        // Iterate over decoded frames
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&codec, &iter)) != NULL) {
            // Access some image properties to explore more states
            (void)img->w;
            (void)img->h;
            (void)img->fmt;
        }
        
        // Try some control codes for VP8
        int value;
        vpx_codec_control(&codec, VP8D_GET_LAST_REF_UPDATES, &value);
        vpx_codec_control(&codec, VP8D_GET_FRAME_CORRUPTED, &value);
        
        // Clean up
        vpx_codec_destroy(&codec);
    }
    
    // Try with VP9 decoder if VP8 failed
    if (init_result != VPX_CODEC_OK && iface_vp9) {
        init_result = vpx_codec_dec_init_ver(&codec, iface_vp9, &cfg, 0, VPX_DECODER_ABI_VERSION);
        
        if (init_result == VPX_CODEC_OK) {
            // Try registering callbacks
            vpx_codec_register_put_frame_cb(&codec, dummy_put_frame_cb, NULL);
            vpx_codec_register_put_slice_cb(&codec, dummy_put_slice_cb, NULL);
            
            // Decode the input data
            vpx_codec_decode(&codec, Data, Size, NULL, 0);
            
            // Iterate over decoded frames
            vpx_codec_iter_t iter = NULL;
            vpx_image_t *img;
            while ((img = vpx_codec_get_frame(&codec, &iter)) != NULL) {
                // Access some image properties to explore more states
                (void)img->w;
                (void)img->h;
                (void)img->fmt;
            }
            
            // Clean up - no VP9 control codes since we don't have the header
            vpx_codec_destroy(&codec);
        }
    }
    
    return 0;
}