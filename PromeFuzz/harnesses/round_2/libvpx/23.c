// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
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

/* Static callback functions for put_frame and put_slice */
static void put_frame_callback(void* user_priv, const vpx_image_t* img) {
    /* Empty callback for fuzzing */
    (void)user_priv;
    (void)img;
}

static void put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* rect,
                               const vpx_image_rect_t* valid_rect) {
    /* Empty callback for fuzzing */
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)valid_rect;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t codec;
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    vpx_codec_dec_cfg_t cfg = {0};
    vpx_codec_err_t res;
    
    /* Initialize decoder with default configuration */
    res = vpx_codec_dec_init_ver(&codec, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    /* Register callbacks if supported */
    vpx_codec_register_put_frame_cb(&codec, put_frame_callback, NULL);
    vpx_codec_register_put_slice_cb(&codec, put_slice_callback, NULL);
    
    /* Try various control operations */
    int ctrl_val;
    vpx_codec_control_(&codec, VP8D_GET_LAST_REF_UPDATES, &ctrl_val);
    vpx_codec_control_(&codec, VP8D_GET_FRAME_CORRUPTED, &ctrl_val);
    
    /* Decode the input data */
    if (Size > 0) {
        vpx_codec_decode(&codec, Data, (unsigned int)Size, NULL, 0);
    }
    
    /* Flush the decoder */
    vpx_codec_decode(&codec, NULL, 0, NULL, 0);
    
    /* Iterate through decoded frames */
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&codec, &iter)) != NULL) {
        /* Access some image properties to explore state */
        (void)img->fmt;
        (void)img->w;
        (void)img->h;
    }
    
    /* Clean up */
    vpx_codec_destroy(&codec);
    
    return 0;
}