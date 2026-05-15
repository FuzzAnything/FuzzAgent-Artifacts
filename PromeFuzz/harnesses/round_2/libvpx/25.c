// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"

/* Declare decoder interface functions */
extern const vpx_codec_iface_t *vpx_codec_vp8_dx(void);
extern const vpx_codec_iface_t *vpx_codec_vp9_dx(void);

/* Static callback functions */
static void put_frame_callback(void *user_priv, const vpx_image_t *img) {
    /* Do nothing - just a stub for fuzzing */
    (void)user_priv;
    (void)img;
}

static void put_slice_callback(void *user_priv, const vpx_image_t *img, 
                               const vpx_image_rect_t *valid, const vpx_image_rect_t *update) {
    /* Do nothing - just a stub for fuzzing */
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    const vpx_codec_iface_t *iface = NULL;
    vpx_codec_dec_cfg_t cfg = {0};
    vpx_codec_err_t res;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    
    /* Use first few bytes to determine decoder type and configuration */
    if (Size < 4) return 0;
    
    /* Choose decoder interface based on input */
    uint8_t decoder_choice = Data[0] % 2;
    if (decoder_choice == 0) {
        /* VP8 decoder */
        iface = vpx_codec_vp8_dx();
    } else {
        /* VP9 decoder */
        iface = vpx_codec_vp9_dx();
    }
    
    if (iface == NULL) {
        /* Failed to get decoder interface */
        return 0;
    }
    
    /* Configure decoder with fuzzed parameters */
    cfg.threads = (Data[1] % 8) + 1;  /* 1-8 threads */
    cfg.w = (Data[2] % 640) + 1;      /* 1-640 width */
    cfg.h = (Data[3] % 480) + 1;      /* 1-480 height */
    
    /* Initialize decoder */
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        /* Initialization failed - skip further processing */
        return 0;
    }
    
    /* Register callbacks with fuzzed user data */
    if (Size > 10) {
        void *user_priv = (void *)(Data + 4);
        vpx_codec_register_put_frame_cb(&ctx, put_frame_callback, user_priv);
        vpx_codec_register_put_slice_cb(&ctx, put_slice_callback, user_priv);
    }
    
    /* Decode the input data (skip first 4 bytes used for config) */
    const uint8_t *decode_data = Data + 4;
    size_t decode_size = (Size > 4) ? Size - 4 : 0;
    
    if (decode_size > 0) {
        /* First decode pass */
        res = vpx_codec_decode(&ctx, decode_data, (unsigned int)decode_size, NULL, 0);
        
        /* Iterate through decoded frames */
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            /* Access some image properties to explore more states */
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
        }
        
        /* Second decode pass with NULL data to flush */
        res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        
        /* Iterate again after flush */
        iter = NULL;
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
        }
    }
    
    /* Try various control operations */
    if (Size > 20) {
        int ctrl_id = Data[4] % 10;  /* Simulate different control IDs */
        int int_arg = (int)Data[5];
        void *ptr_arg = (void *)(Data + 6);
        
        /* Try control with integer argument */
        vpx_codec_control_(&ctx, ctrl_id, int_arg);
        
        /* Try control with pointer argument */
        vpx_codec_control_(&ctx, ctrl_id, ptr_arg);
        
        /* Try control with multiple arguments */
        vpx_codec_control_(&ctx, ctrl_id, int_arg, ptr_arg);
    }
    
    /* Cleanup */
    vpx_codec_destroy(&ctx);
    
    return 0;
}