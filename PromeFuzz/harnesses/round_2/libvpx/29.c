// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_set_frame_buffer_functions at vpx_decoder.c:173:17 in vpx_decoder.h
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
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Frame buffer callback functions */
static int get_frame_buffer(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    fb->priv = NULL;
    return 0;
}

static int release_frame_buffer(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    free(fb->data);
    return 0;
}

/* Put frame callback function */
static void put_frame_callback(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
    /* Do nothing - just a stub callback */
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_err_t res;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    
    /* Initialize with VP9 decoder interface */
    iface = vpx_codec_vp9_dx();
    if (!iface) return 0;
    
    /* Clear context */
    memset(&ctx, 0, sizeof(ctx));
    
    /* Set up configuration from fuzz data if we have enough bytes */
    if (Size >= sizeof(cfg)) {
        memcpy(&cfg, Data, sizeof(cfg));
        Data += sizeof(cfg);
        Size -= sizeof(cfg);
    } else {
        /* Default configuration */
        cfg.threads = 1;
        cfg.w = 640;
        cfg.h = 480;
    }
    
    /* Initialize decoder with version check */
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        /* Initialization failed - try with NULL config */
        res = vpx_codec_dec_init_ver(&ctx, iface, NULL, 0, VPX_DECODER_ABI_VERSION);
        if (res != VPX_CODEC_OK) {
            return 0;
        }
    }
    
    /* Try to set frame buffer functions (optional) */
    if (Size > 0) {
        vpx_codec_set_frame_buffer_functions(&ctx, get_frame_buffer, 
                                           release_frame_buffer, NULL);
    }
    
    /* Try to register put frame callback (optional) */
    if (Size > 1) {
        vpx_codec_register_put_frame_cb(&ctx, put_frame_callback, NULL);
    }
    
    /* Decode the input data */
    if (Size > 0) {
        vpx_codec_decode(&ctx, Data, Size, NULL, 0);
        
        /* Also try to decode with NULL to flush */
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    } else {
        /* Try decoding with NULL data to flush */
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    }
    
    /* Iterate through decoded frames */
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Access some image properties to explore more states */
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