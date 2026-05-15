// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"

/* Dummy callback functions */
static void dummy_put_slice_cb(void *user_priv,
                               const vpx_image_t *img,
                               const vpx_image_rect_t *rect1,
                               const vpx_image_rect_t *rect2) {
    (void)user_priv;
    (void)img;
    (void)rect1;
    (void)rect2;
}

static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    
    if (!iface) {
        return 0;
    }
    
    /* Initialize decoder with default configuration */
    vpx_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    /* Try to register callbacks if supported */
    vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, NULL);
    vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
    
    /* Decode the input data */
    if (Size > 0) {
        vpx_codec_decode(&ctx, Data, (unsigned int)Size, NULL, 0);
    }
    
    /* Flush the decoder */
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    
    /* Iterate through decoded frames */
    vpx_codec_iter_t iter = NULL;
    while (vpx_codec_get_frame(&ctx, &iter) != NULL) {
        /* Just iterate through frames */
    }
    
    /* Clean up */
    vpx_codec_destroy(&ctx);
    
    return 0;
}