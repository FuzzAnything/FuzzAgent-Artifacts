// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_set_frame_buffer_functions at vpx_decoder.c:173:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_codec.h"

/* Declare the decoder interface functions that are missing */
extern vpx_codec_iface_t *vpx_codec_vp8_dx(void);
extern vpx_codec_iface_t *vpx_codec_vp9_dx(void);

/* Dummy callback functions for frame buffer management */
static int get_frame_buffer_cb(void *cb_priv, size_t min_size, 
                               vpx_codec_frame_buffer_t *fb) {
    /* Allocate minimal frame buffer */
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    /* Use cb_priv as simple counter */
    if (cb_priv) {
        int *counter = (int*)cb_priv;
        fb->priv = counter;
    }
    return 0;
}

static int release_frame_buffer_cb(void *cb_priv, 
                                   vpx_codec_frame_buffer_t *fb) {
    if (fb && fb->data) {
        free(fb->data);
    }
    return 0;
}

/* Dummy put_frame callback */
static void put_frame_cb(void *user_priv, const vpx_image_t *img) {
    /* Do nothing, just accept the frame */
    (void)user_priv;
    (void)img;
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    const vpx_codec_iface_t *iface = NULL;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_stream_info_t si;
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *img;
    int use_callbacks = 0;
    int cb_priv_data = 0;
    
    /* Initialize configuration with default values */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 320;
    cfg.h = 240;
    
    /* Initialize stream info */
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    /* Choose codec interface based on input data */
    if (Size > 0) {
        /* Use first byte to determine codec */
        if ((Data[0] & 0x01) == 0) {
            /* VP8 decoder */
            iface = vpx_codec_vp8_dx();
        } else {
            /* VP9 decoder */
            iface = vpx_codec_vp9_dx();
        }
        use_callbacks = (Data[0] & 0x02) != 0;
    } else {
        /* Default to VP8 if no data */
        iface = vpx_codec_vp8_dx();
    }
    
    if (!iface) {
        return 0;
    }
    
    /* Initialize decoder */
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, 
                                                 VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    /* Optionally set frame buffer callbacks */
    if (use_callbacks) {
        vpx_codec_set_frame_buffer_functions(&ctx, get_frame_buffer_cb, 
                                             release_frame_buffer_cb, 
                                             &cb_priv_data);
    }
    
    /* Register put_frame callback */
    vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, NULL);
    
    /* Decode the input data */
    if (Size > 0) {
        /* Skip first byte used for configuration */
        const uint8_t *video_data = Data + 1;
        size_t video_size = Size - 1;
        
        if (video_size > 0) {
            vpx_codec_decode(&ctx, video_data, (unsigned int)video_size, 
                            NULL, 0);
        }
    }
    
    /* Flush decoder */
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    
    /* Try to get stream info */
    vpx_codec_get_stream_info(&ctx, &si);
    
    /* Iterate through decoded frames */
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Access some image properties to explore state */
        (void)img->fmt;
        (void)img->w;
        (void)img->h;
        (void)img->d_w;
        (void)img->d_h;
        (void)img->planes[0];
        (void)img->stride[0];
    }
    
    /* Clean up */
    vpx_codec_destroy(&ctx);
    
    return 0;
}