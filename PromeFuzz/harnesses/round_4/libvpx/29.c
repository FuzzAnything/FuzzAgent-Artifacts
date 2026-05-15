// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"
#include "vpx/vp8.h"

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

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t res;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_stream_info_t si;
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *img;
    
    /* Try VP9 decoder first */
    iface = vpx_codec_vp9_dx();
    if (iface == NULL) {
        /* Fall back to VP8 decoder */
        iface = vpx_codec_vp8_dx();
        if (iface == NULL) {
            return 0;
        }
    }
    
    /* Initialize decoder configuration with fuzzed data */
    cfg.threads = (Size > 0) ? (Data[0] % 8) + 1 : 1;
    cfg.w = (Size > 2) ? ((Data[1] << 8) | Data[2]) : 320;
    cfg.h = (Size > 4) ? ((Data[3] << 8) | Data[4]) : 240;
    
    /* Initialize decoder context */
    res = vpx_codec_dec_init_ver(&codec_ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    /* Register callbacks if supported */
    vpx_codec_register_put_slice_cb(&codec_ctx, dummy_put_slice_cb, NULL);
    vpx_codec_register_put_frame_cb(&codec_ctx, dummy_put_frame_cb, NULL);
    
    /* Get stream info (may fail if no data parsed yet) */
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    vpx_codec_get_stream_info(&codec_ctx, &si);
    
    /* Decode the fuzzed input data */
    if (Size > 0) {
        /* Decode with the provided data */
        res = vpx_codec_decode(&codec_ctx, Data, (unsigned int)Size, NULL, 0);
        
        /* Flush decoder */
        vpx_codec_decode(&codec_ctx, NULL, 0, NULL, 0);
    } else {
        /* Decode with empty data to flush */
        vpx_codec_decode(&codec_ctx, NULL, 0, NULL, 0);
    }
    
    /* Iterate through available decoded frames */
    while ((img = vpx_codec_get_frame(&codec_ctx, &iter)) != NULL) {
        /* Access image properties to explore state */
        (void)img->fmt;
        (void)img->w;
        (void)img->h;
        (void)img->d_w;
        (void)img->d_h;
        (void)img->planes[0];
        (void)img->stride[0];
    }
    
    /* Get stream info again after decoding */
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    vpx_codec_get_stream_info(&codec_ctx, &si);
    
    /* Clean up */
    vpx_codec_destroy(&codec_ctx);
    
    return 0;
}