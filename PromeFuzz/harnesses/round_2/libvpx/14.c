// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx_codec.h"
#include "vpx_decoder.h"
#include "vpx/vp8dx.h"

/* Static callback function for vpx_codec_register_put_frame_cb */
static void put_frame_callback(void* user_priv, const vpx_image_t* img) {
    /* Empty callback - just to exercise the API */
    (void)user_priv;
    (void)img;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_stream_info_t si;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    vpx_codec_err_t err;
    int i;
    
    /* Initialize configuration with fuzzed data */
    cfg.threads = (Size > 0) ? (Data[0] % 8) + 1 : 1;
    cfg.w = (Size > 1) ? (Data[1] * 16) : 320;
    cfg.h = (Size > 2) ? (Data[2] * 16) : 240;
    
    /* Initialize stream info structure */
    si.sz = sizeof(si);
    si.w = 0;
    si.h = 0;
    si.is_kf = 0;
    
    /* Try to get decoder interface - use the correct function names */
    /* First try VP8 decoder */
    iface = vpx_codec_vp8_dx();
    if (!iface) {
        /* Try VP9 decoder if VP8 not available */
        iface = vpx_codec_vp9_dx();
    }
    
    if (!iface) {
        /* If still no interface, try to use vpx_codec_dec_init with NULL iface */
        /* This will fail but we want to test error paths */
        err = vpx_codec_dec_init_ver(&ctx, NULL, &cfg, 0, VPX_DECODER_ABI_VERSION);
        return 0;
    }
    
    /* Initialize decoder with version check */
    err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        /* Try with NULL configuration */
        err = vpx_codec_dec_init_ver(&ctx, iface, NULL, 0, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            return 0; /* Failed to initialize decoder */
        }
    }
    
    /* Register put frame callback (may fail if not supported) */
    vpx_codec_register_put_frame_cb(&ctx, put_frame_callback, NULL);
    
    /* Decode the input data */
    if (Size > 0) {
        vpx_codec_decode(&ctx, Data, (unsigned int)Size, NULL, 0);
    } else {
        /* Flush decoder with NULL input */
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    }
    
    /* Get stream information */
    vpx_codec_get_stream_info(&ctx, &si);
    
    /* Iterate through decoded frames */
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Access some image properties to exercise code */
        (void)img->fmt;
        (void)img->w;
        (void)img->h;
        (void)img->d_w;
        (void)img->d_h;
        (void)img->r_w;
        (void)img->r_h;
        (void)img->bit_depth;
        (void)img->cs;
        (void)img->range;
        (void)img->x_chroma_shift;
        (void)img->y_chroma_shift;
        (void)img->bps;
    }
    
    /* Try various control codes */
    for (i = 0; i < 10; i++) {
        vpx_codec_control_(&ctx, i, NULL);
    }
    
    /* Decode again with different parameters */
    if (Size > 10) {
        /* Use fragment of data */
        size_t half_size = Size / 2;
        vpx_codec_decode(&ctx, Data, (unsigned int)half_size, NULL, 0);
        vpx_codec_decode(&ctx, Data + half_size, (unsigned int)(Size - half_size), NULL, 0);
        
        /* Flush */
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        
        /* Iterate frames again */
        iter = NULL;
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            (void)img;
        }
    }
    
    /* Clean up */
    vpx_codec_destroy(&ctx);
    
    return 0;
}