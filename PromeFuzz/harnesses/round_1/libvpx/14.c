// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx_decoder.h"
#include "vpx/vp8dx.h"

/* Dummy callback functions for slice and frame notifications */
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

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_stream_info_t si;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    vpx_codec_err_t res;
    vpx_codec_flags_t flags = 0;
    
    /* Use VP8 decoder interface */
    iface = vpx_codec_vp8_dx();
    if (!iface)
        return 0;
    
    /* Initialize configuration with default values */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;
    
    /* Initialize decoder context */
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, flags, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK)
        return 0;
    
    /* Register slice callback (if supported) */
    vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, NULL);
    
    /* Register frame callback (if supported) */
    vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
    
    /* Decode the input data */
    if (Size > 0) {
        vpx_codec_decode(&ctx, Data, (unsigned int)Size, NULL, 0);
    }
    
    /* Flush the decoder */
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    
    /* Try to get stream information */
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    vpx_codec_get_stream_info(&ctx, &si);
    
    /* Iterate over decoded frames */
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