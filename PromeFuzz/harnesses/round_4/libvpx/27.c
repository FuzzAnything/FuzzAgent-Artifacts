// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_set_frame_buffer_functions at vpx_decoder.c:173:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

/* Static callback functions to avoid name conflicts */
static void put_frame_callback(void* user_priv, const vpx_image_t* img) {
    /* Dummy callback - do nothing */
    (void)user_priv;
    (void)img;
}

static int get_frame_buffer_callback(void* cb_priv, size_t min_size, 
                                     vpx_codec_frame_buffer_t* fb) {
    /* Dummy callback - return failure to use default allocation */
    (void)cb_priv;
    (void)min_size;
    (void)fb;
    return -1;
}

static int release_frame_buffer_callback(void* cb_priv, 
                                         vpx_codec_frame_buffer_t* fb) {
    /* Dummy callback */
    (void)cb_priv;
    (void)fb;
    return 0;
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_stream_info_t si;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t* img;
    
    /* Initialize structures */
    memset(&ctx, 0, sizeof(ctx));
    memset(&si, 0, sizeof(si));
    memset(&cfg, 0, sizeof(cfg));
    
    /* Set stream info size */
    si.sz = sizeof(vpx_codec_stream_info_t);
    
    /* Parse configuration from fuzzer input if we have enough data */
    if (Size >= sizeof(cfg)) {
        memcpy(&cfg, Data, sizeof(cfg));
        Data += sizeof(cfg);
        Size -= sizeof(cfg);
        
        /* Sanitize values to avoid unreasonable allocations */
        if (cfg.threads > 64) cfg.threads = 64;
        if (cfg.w > 16384) cfg.w = 16384;
        if (cfg.h > 16384) cfg.h = 16384;
    } else {
        /* Use default configuration */
        cfg.threads = 1;
        cfg.w = 320;
        cfg.h = 240;
    }
    
    /* Try both VP8 and VP9 codecs */
    const vpx_codec_iface_t* codecs[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    for (int codec_idx = 0; codec_idx < 2; codec_idx++) {
        /* Initialize decoder */
        vpx_codec_err_t init_err = vpx_codec_dec_init_ver(
            &ctx, 
            (vpx_codec_iface_t*)codecs[codec_idx], 
            &cfg, 
            0, 
            VPX_DECODER_ABI_VERSION
        );
        
        if (init_err != VPX_CODEC_OK) {
            /* Failed to initialize, try next codec */
            continue;
        }
        
        /* Try to register put frame callback */
        vpx_codec_register_put_frame_cb(&ctx, put_frame_callback, NULL);
        
        /* Try to set frame buffer functions */
        vpx_codec_set_frame_buffer_functions(
            &ctx, 
            get_frame_buffer_callback, 
            release_frame_buffer_callback, 
            NULL
        );
        
        /* Decode the input data */
        vpx_codec_decode(&ctx, Data, (unsigned int)Size, NULL, 0);
        
        /* Try to get stream info */
        vpx_codec_get_stream_info(&ctx, &si);
        
        /* Iterate through decoded frames */
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            /* Access some image properties to explore state */
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
            (void)img->d_w;
            (void)img->d_h;
            (void)img->planes[0];
            (void)img->stride[0];
        }
        
        /* Clean up decoder */
        vpx_codec_destroy(&ctx);
        
        /* Break after first successful initialization to save cycles */
        break;
    }
    
    return 0;
}