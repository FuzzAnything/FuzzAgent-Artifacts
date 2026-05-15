// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_set_frame_buffer_functions at vpx_decoder.c:173:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_codec.h"

/* Declare decoder interface functions that are missing from headers */
extern const vpx_codec_iface_t *vpx_codec_vp8_dx(void);
extern const vpx_codec_iface_t *vpx_codec_vp9_dx(void);

/* Callback function for vpx_codec_register_put_frame_cb */
static void put_frame_callback(void* user_priv, const vpx_image_t* img) {
    /* Empty callback for fuzzing */
    (void)user_priv;
    (void)img;
}

/* Callback functions for vpx_codec_set_frame_buffer_functions */
static int get_frame_buffer_callback(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    /* Return failure for fuzzing */
    (void)cb_priv;
    (void)min_size;
    (void)fb;
    return -1;
}

static int release_frame_buffer_callback(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    /* Return failure for fuzzing */
    (void)cb_priv;
    (void)fb;
    return -1;
}

int LLVMFuzzerTestOneInput_25(const uint8_t* Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    const vpx_codec_iface_t* iface = NULL;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_stream_info_t si;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t* img;
    vpx_codec_err_t res;
    
    /* Initialize configuration with fuzzed data */
    memset(&cfg, 0, sizeof(cfg));
    if (Size >= sizeof(cfg)) {
        memcpy(&cfg, Data, sizeof(cfg) > Size ? Size : sizeof(cfg));
    }
    
    /* Initialize stream info */
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    if (Size >= sizeof(si)) {
        memcpy(&si, Data, sizeof(si) > Size ? Size : sizeof(si));
    }
    
    /* Try both VP8 and VP9 decoders */
    const vpx_codec_iface_t* decoders[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    for (size_t i = 0; i < sizeof(decoders)/sizeof(decoders[0]); i++) {
        iface = decoders[i];
        
        /* Initialize decoder */
        memset(&ctx, 0, sizeof(ctx));
        res = vpx_codec_dec_init_ver(&ctx, (vpx_codec_iface_t*)iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
        if (res != VPX_CODEC_OK) {
            continue; /* Try next decoder */
        }
        
        /* Try to set frame buffer functions */
        vpx_codec_set_frame_buffer_functions(&ctx, get_frame_buffer_callback, 
                                           release_frame_buffer_callback, NULL);
        
        /* Try to register put frame callback */
        vpx_codec_register_put_frame_cb(&ctx, put_frame_callback, NULL);
        
        /* Decode the input data */
        vpx_codec_decode(&ctx, Data, Size, NULL, 0);
        
        /* Try to get stream info */
        vpx_codec_get_stream_info(&ctx, &si);
        
        /* Iterate through decoded frames */
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            /* Access some image properties to explore more states */
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
        }
        
        /* Decode again with NULL data to flush */
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        
        /* Get stream info again after flush */
        vpx_codec_get_stream_info(&ctx, &si);
        
        /* Clean up */
        vpx_codec_destroy(&ctx);
    }
    
    return 0;
}