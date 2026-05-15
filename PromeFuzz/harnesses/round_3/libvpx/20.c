// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx_decoder.h"
#include "vpx/vp8dx.h"
#include "vpx/vp8.h"

/* Dummy callback functions for testing */
static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *rect, const vpx_image_rect_t *clipped_rect) {
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)clipped_rect;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *data, size_t size) {
    /* Initialize decoder contexts for both VP8 and VP9 */
    vpx_codec_ctx_t decoder_vp8, decoder_vp9;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_err_t res;
    
    /* Use some fuzzed data to initialize configuration */
    if (size >= sizeof(cfg)) {
        memcpy(&cfg, data, sizeof(cfg));
        /* Ensure reasonable thread count */
        cfg.threads = cfg.threads % 16;
        /* Ensure reasonable dimensions */
        cfg.w = cfg.w % 4096;
        cfg.h = cfg.h % 4096;
    } else {
        /* Default configuration */
        cfg.threads = 1;
        cfg.w = 0;
        cfg.h = 0;
    }
    
    /* Initialize VP8 decoder */
    res = vpx_codec_dec_init(&decoder_vp8, &vpx_codec_vp8_dx_algo, &cfg, 0);
    if (res == VPX_CODEC_OK) {
        /* Try to register callbacks */
        vpx_codec_register_put_frame_cb(&decoder_vp8, dummy_put_frame_cb, NULL);
        vpx_codec_register_put_slice_cb(&decoder_vp8, dummy_put_slice_cb, NULL);
        
        /* Decode the input data */
        vpx_codec_decode(&decoder_vp8, data, (unsigned int)(size > 0xFFFFFFFF ? 0xFFFFFFFF : size), NULL, 0);
        
        /* Get stream information */
        vpx_codec_stream_info_t si;
        si.sz = sizeof(si);
        vpx_codec_get_stream_info(&decoder_vp8, &si);
        
        /* Iterate over decoded frames */
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&decoder_vp8, &iter)) != NULL) {
            /* Access some image properties to explore more states */
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
        }
        
        /* Clean up */
        vpx_codec_destroy(&decoder_vp8);
    }
    
    /* Initialize VP9 decoder */
    res = vpx_codec_dec_init(&decoder_vp9, &vpx_codec_vp9_dx_algo, &cfg, 0);
    if (res == VPX_CODEC_OK) {
        /* Try to register callbacks */
        vpx_codec_register_put_frame_cb(&decoder_vp9, dummy_put_frame_cb, NULL);
        vpx_codec_register_put_slice_cb(&decoder_vp9, dummy_put_slice_cb, NULL);
        
        /* Decode the input data */
        vpx_codec_decode(&decoder_vp9, data, (unsigned int)(size > 0xFFFFFFFF ? 0xFFFFFFFF : size), NULL, 0);
        
        /* Get stream information */
        vpx_codec_stream_info_t si;
        si.sz = sizeof(si);
        vpx_codec_get_stream_info(&decoder_vp9, &si);
        
        /* Iterate over decoded frames */
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&decoder_vp9, &iter)) != NULL) {
            /* Access some image properties to explore more states */
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
        }
        
        /* Clean up */
        vpx_codec_destroy(&decoder_vp9);
    }
    
    return 0;
}