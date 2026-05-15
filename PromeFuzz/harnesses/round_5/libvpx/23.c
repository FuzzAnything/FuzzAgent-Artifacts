// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_set_frame_buffer_functions at vpx_decoder.c:173:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx_decoder.h"

/* Static callback functions for fuzzing */
static void put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* rect, const vpx_image_rect_t* rect2) {
    /* Minimal implementation - just to satisfy function pointer */
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

static void put_frame_callback(void* user_priv, const vpx_image_t* img) {
    /* Minimal implementation - just to satisfy function pointer */
    (void)user_priv;
    (void)img;
}

static int get_frame_buffer_callback(void* cb_priv, size_t min_size,
                                     vpx_codec_frame_buffer_t* fb) {
    /* Minimal implementation - allocate buffer if needed */
    (void)cb_priv;
    
    if (min_size == 0) {
        return -1;
    }
    
    /* Try to allocate buffer - may fail under fuzzing */
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) {
        return -1;
    }
    
    fb->size = min_size;
    return 0;
}

static int release_frame_buffer_callback(void* cb_priv,
                                         vpx_codec_frame_buffer_t* fb) {
    /* Minimal implementation - free buffer */
    (void)cb_priv;
    
    if (fb && fb->data) {
        free(fb->data);
        fb->data = NULL;
    }
    return 0;
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    vpx_codec_err_t res;
    
    /* Skip if input is too small */
    if (size < 4) {
        return 0;
    }
    
    /* Initialize configuration with values from fuzz input */
    cfg.threads = (data[0] % 8) + 1;  /* 1-8 threads */
    cfg.w = (data[1] % 256) * 16;     /* Width in blocks of 16 */
    cfg.h = (data[2] % 256) * 16;     /* Height in blocks of 16 */
    
    /* Try VP8 decoder first */
    extern vpx_codec_iface_t vpx_codec_vp8_dx_algo;
    res = vpx_codec_dec_init_ver(&ctx, &vpx_codec_vp8_dx_algo, &cfg, 0, VPX_DECODER_ABI_VERSION);
    
    /* If VP8 fails, try VP9 */
    if (res != VPX_CODEC_OK) {
        extern vpx_codec_iface_t vpx_codec_vp9_dx_algo;
        res = vpx_codec_dec_init_ver(&ctx, &vpx_codec_vp9_dx_algo, &cfg, 0, VPX_DECODER_ABI_VERSION);
    }
    
    /* If initialization succeeded, proceed with fuzzing */
    if (res == VPX_CODEC_OK) {
        /* Register callbacks based on input data */
        if (size > 10 && (data[3] & 0x01)) {
            vpx_codec_register_put_slice_cb(&ctx, put_slice_callback, NULL);
        }
        
        if (size > 10 && (data[3] & 0x02)) {
            vpx_codec_register_put_frame_cb(&ctx, put_frame_callback, NULL);
        }
        
        /* Set frame buffer functions based on input data */
        if (size > 10 && (data[3] & 0x04)) {
            vpx_codec_set_frame_buffer_functions(&ctx, get_frame_buffer_callback,
                                               release_frame_buffer_callback, NULL);
        }
        
        /* Decode the input data */
        vpx_codec_decode(&ctx, data + 4, size - 4, NULL, 0);
        
        /* Try to get decoded frames */
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            /* Access some image properties to explore more states */
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
            (void)img->d_w;
            (void)img->d_h;
            (void)img->r_w;
            (void)img->r_h;
            (void)img->x_chroma_shift;
            (void)img->y_chroma_shift;
            (void)img->bps;
            (void)img->cs;
            (void)img->range;
            (void)img->bit_depth;
        }
        
        /* Clean up */
        vpx_codec_destroy(&ctx);
    }
    
    return 0;
}