// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"
#include "vpx/vp8.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy callback functions */
static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *valid,
                               const vpx_image_rect_t *update) {
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Initialize decoder context */
    vpx_codec_ctx_t ctx;
    const vpx_codec_iface_t *iface = NULL;
    
    /* Choose codec based on first byte */
    if (Data[0] & 0x01) {
        iface = vpx_codec_vp8_dx();
    } else {
        iface = vpx_codec_vp9_dx();
    }
    
    /* Prepare configuration */
    vpx_codec_dec_cfg_t cfg;
    cfg.threads = (Data[0] >> 1) & 0x07;  /* Use 0-7 threads */
    cfg.w = 640;
    cfg.h = 480;
    
    /* Initialize decoder with different flags */
    vpx_codec_flags_t flags = 0;
    if (Data[0] & 0x10) {
        flags |= VPX_CODEC_USE_INPUT_FRAGMENTS;
    }
    if (Data[0] & 0x20) {
        flags |= VPX_CODEC_USE_POSTPROC;
    }
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, flags, 
                                                 VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        /* Initialization failed, cleanup and return */
        vpx_codec_destroy(&ctx);
        return 0;
    }
    
    /* Register callbacks if enabled */
    if (Data[0] & 0x40) {
        (void)vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
    }
    if (Data[0] & 0x80) {
        (void)vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, NULL);
    }
    
    /* Decode the input data */
    size_t data_offset = 1;
    size_t remaining = Size - data_offset;
    
    if (remaining > 0) {
        /* Use different deadlines based on input */
        long deadline = 0;
        if (Data[0] & 0x02) {
            deadline = 1000;  /* 1ms */
        }
        
        /* Try decoding with the remaining data */
        err = vpx_codec_decode(&ctx, Data + data_offset, (unsigned int)remaining, 
                              NULL, deadline);
        
        /* Try to get decoded frames */
        vpx_codec_iter_t iter = NULL;
        const vpx_image_t *img;
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
        
        /* Try some control operations */
        if (remaining >= 2) {
            /* Use second byte as control ID */
            int ctrl_id = Data[1];
            
            /* Try different argument types based on control ID */
            if (ctrl_id & 0x01) {
                int int_arg = (int)Data[1];
                (void)vpx_codec_control_(&ctx, ctrl_id, int_arg);
            } else {
                void *ptr_arg = NULL;
                (void)vpx_codec_control_(&ctx, ctrl_id, ptr_arg);
            }
        }
        
        /* Try flushing the decoder */
        (void)vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        
        /* Get any remaining frames after flush */
        iter = NULL;
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            (void)img;
        }
    }
    
    /* Cleanup */
    vpx_codec_destroy(&ctx);
    
    return 0;
}