// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"

/* Declare the decoder interface functions */
extern vpx_codec_iface_t *vpx_codec_vp8_dx(void);
extern vpx_codec_iface_t *vpx_codec_vp9_dx(void);

/* Dummy callback function for vpx_codec_register_put_frame_cb */
static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    /* Do nothing - just a placeholder callback */
    (void)user_priv;
    (void)img;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_stream_info_t si;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    vpx_codec_err_t res;
    int i;
    
    /* Use a small portion of input for configuration if available */
    if (Size < 1) return 0;
    
    /* Initialize configuration with fuzzed data */
    cfg.threads = (Data[0] % 8) + 1;  /* 1-8 threads */
    cfg.w = 128 + (Data[0] % 384);    /* 128-511 width */
    cfg.h = 128 + ((Data[0] >> 4) % 384); /* 128-511 height */
    
    /* Try both VP8 and VP9 decoders */
    for (i = 0; i < 2; i++) {
        /* Get decoder interface */
        if (i == 0) {
            iface = vpx_codec_vp8_dx();
        } else {
            iface = vpx_codec_vp9_dx();
        }
        
        if (!iface) continue;
        
        /* Initialize decoder with version check */
        res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
        if (res != VPX_CODEC_OK) continue;
        
        /* Try to register put frame callback */
        vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
        
        /* Decode the input data */
        res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
        
        /* Try to get stream info */
        memset(&si, 0, sizeof(si));
        si.sz = sizeof(si);
        vpx_codec_get_stream_info(&ctx, &si);
        
        /* Iterate over decoded frames */
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            /* Access some image properties to explore state */
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
            (void)img->d_w;
            (void)img->d_h;
            (void)img->r_w;
            (void)img->r_h;
            (void)img->x_chroma_shift;
            (void)img->y_chroma_shift;
            (void)img->bps;
            (void)img->cs;
            (void)img->range;
            (void)img->bit_depth;
        }
        
        /* Try various control codes */
        int ctrl_id;
        int int_val;
        void *ptr_val;
        
        /* Try a few common control IDs */
        for (ctrl_id = 0; ctrl_id < 32; ctrl_id++) {
            /* Different control IDs expect different argument types */
            switch (ctrl_id % 4) {
                case 0:
                    int_val = (int)Data[0];
                    vpx_codec_control_(&ctx, ctrl_id, int_val);
                    break;
                case 1:
                    int_val = (int)Data[0];
                    vpx_codec_control_(&ctx, ctrl_id, &int_val);
                    break;
                case 2:
                    ptr_val = (void *)Data;
                    vpx_codec_control_(&ctx, ctrl_id, ptr_val);
                    break;
                case 3:
                    vpx_codec_control_(&ctx, ctrl_id);
                    break;
            }
        }
        
        /* Flush decoder */
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        
        /* Clean up */
        vpx_codec_destroy(&ctx);
        
        /* Try a different configuration for the second iteration */
        if (Size > 1) {
            cfg.threads = (Data[1] % 16) + 1;
            cfg.w = 64 + (Data[1] % 192);
            cfg.h = 64 + ((Data[1] >> 4) % 192);
        }
    }
    
    return 0;
}