// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
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

/* Dummy callback function for put_frame callback */
static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    /* Empty callback for fuzzing */
    (void)user_priv;
    (void)img;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_stream_info_t si;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    vpx_codec_err_t err;
    
    /* Initialize configuration with default values */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 640;
    cfg.h = 480;
    
    /* Initialize stream info structure */
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    /* Get VP8 decoder interface */
    iface = vpx_codec_vp8_dx();
    if (!iface) {
        return 0;
    }
    
    /* Initialize decoder context */
    err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        /* If initialization fails, we can't proceed further */
        return 0;
    }
    
    /* Try to register put_frame callback */
    vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
    
    /* Decode the input data */
    vpx_codec_decode(&ctx, Data, (unsigned int)Size, NULL, 0);
    
    /* Try to get stream information */
    vpx_codec_get_stream_info(&ctx, &si);
    
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
    
    /* Clean up decoder context */
    vpx_codec_destroy(&ctx);
    
    return 0;
}