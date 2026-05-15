// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Declare the decoder interface functions that are in the header but might need explicit declaration
extern vpx_codec_iface_t vpx_codec_vp9_dx_algo;
extern vpx_codec_iface_t vpx_codec_vp8_dx_algo;

// Dummy callback functions with correct signatures
static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *valid, 
                               const vpx_image_rect_t *update) {
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
}

static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize decoder context
    vpx_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));

    // Get decoder interface - use direct access to algo structures
    vpx_codec_iface_t *iface = &vpx_codec_vp9_dx_algo;
    
    // Initialize decoder configuration
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    // Use fuzz data to set configuration parameters
    if (Size >= 4) {
        cfg.threads = (Data[0] % 8) + 1;  // 1-8 threads
        cfg.w = (Data[1] % 64) * 16;      // Width multiple of 16
        cfg.h = (Data[2] % 64) * 16;      // Height multiple of 16
    } else {
        cfg.threads = 1;
        cfg.w = 320;
        cfg.h = 240;
    }

    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&codec, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        iface = &vpx_codec_vp8_dx_algo;
        err = vpx_codec_dec_init_ver(&codec, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            return 0;
        }
    }

    // Register callbacks
    vpx_codec_register_put_slice_cb(&codec, dummy_put_slice_cb, NULL);
    vpx_codec_register_put_frame_cb(&codec, dummy_put_frame_cb, NULL);

    // Decode the input data
    size_t data_offset = Size >= 4 ? 3 : 0;
    size_t data_size = Size - data_offset;
    
    if (data_size > 0) {
        // First decode with actual data
        err = vpx_codec_decode(&codec, Data + data_offset, (unsigned int)data_size, NULL, 0);
        
        // Then flush the decoder
        vpx_codec_decode(&codec, NULL, 0, NULL, 0);
    } else {
        // Just flush if no data
        vpx_codec_decode(&codec, NULL, 0, NULL, 0);
    }

    // Get decoded frames
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&codec, &iter)) != NULL) {
        // Access some image properties to explore more states
        (void)img->fmt;
        (void)img->w;
        (void)img->h;
        (void)img->d_w;
        (void)img->d_h;
        if (img->planes[0]) {
            (void)img->planes[0][0];
        }
        (void)img->stride[0];
    }

    // Test various control operations using vpx_codec_control_ directly
    // We'll use arbitrary control IDs from the fuzz data
    if (Size >= 8) {
        // Use different control IDs based on input data
        int ctrl_id = Data[3] % 256;  // Use full byte range
        
        // Try different types of arguments based on fuzz data
        int int_value = (Size >= 12) ? *((int*)(Data + 4)) : 0;
        void *ptr_value = (Size >= 16) ? (void*)(Data + 8) : NULL;
        
        // Try calling vpx_codec_control_ with different argument types
        // We'll try a few different calls to explore different code paths
        vpx_codec_control_(&codec, ctrl_id, int_value);
        vpx_codec_control_(&codec, ctrl_id, ptr_value);
        
        // Try with multiple arguments if we have enough data
        if (Size >= 20) {
            int arg1 = *((int*)(Data + 12));
            int arg2 = *((int*)(Data + 16));
            vpx_codec_control_(&codec, ctrl_id, arg1, arg2);
        }
    }

    // Clean up
    vpx_codec_destroy(&codec);

    return 0;
}