// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx_decoder.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_image.h"

/* Static callback functions to avoid name conflicts */
static void put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* valid, const vpx_image_rect_t* update) {
    /* Empty callback for fuzzing */
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
}

static void put_frame_callback(void* user_priv, const vpx_image_t* img) {
    /* Empty callback for fuzzing */
    (void)user_priv;
    (void)img;
}

/* Helper function to extract configuration values from fuzzer input */
static void extract_config(const uint8_t* data, size_t size, 
                          vpx_codec_dec_cfg_t* cfg, const uint8_t** frame_data,
                          size_t* frame_size) {
    /* Default configuration */
    cfg->threads = 1;
    cfg->w = 0;
    cfg->h = 0;
    
    /* Use first bytes for configuration if available */
    if (size >= 1) {
        cfg->threads = (data[0] & 0x0F) + 1; /* 1-16 threads */
    }
    if (size >= 5) {
        cfg->w = (data[1] << 8) | data[2];
        cfg->h = (data[3] << 8) | data[4];
        *frame_data = data + 5;
        *frame_size = size - 5;
    } else {
        *frame_data = data;
        *frame_size = size;
    }
    
    /* Limit dimensions to reasonable values */
    if (cfg->w > 4096) cfg->w = 4096;
    if (cfg->h > 4096) cfg->h = 4096;
}

int LLVMFuzzerTestOneInput_18(const uint8_t* data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_dec_cfg_t cfg;
    const uint8_t* frame_data;
    size_t frame_size;
    vpx_codec_stream_info_t si;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t* img;
    vpx_codec_err_t err;
    
    /* Need at least 1 byte for configuration */
    if (size < 1) {
        return 0;
    }
    
    /* Extract configuration from input */
    extract_config(data, size, &cfg, &frame_data, &frame_size);
    
    /* Initialize stream info structure */
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    /* Try VP9 decoder first */
    err = vpx_codec_dec_init_ver(&ctx, &vpx_codec_vp9_dx_algo, &cfg, 0, VPX_DECODER_ABI_VERSION);
    
    /* If VP9 fails, try VP8 */
    if (err != VPX_CODEC_OK) {
        err = vpx_codec_dec_init_ver(&ctx, &vpx_codec_vp8_dx_algo, &cfg, 0, VPX_DECODER_ABI_VERSION);
    }
    
    /* If initialization failed, return early */
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    /* Register callbacks if supported */
    vpx_codec_register_put_slice_cb(&ctx, put_slice_callback, NULL);
    vpx_codec_register_put_frame_cb(&ctx, put_frame_callback, NULL);
    
    /* Decode the frame data */
    if (frame_size > 0) {
        vpx_codec_decode(&ctx, frame_data, (unsigned int)frame_size, NULL, 0);
    }
    
    /* Flush the decoder */
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    
    /* Get stream information */
    vpx_codec_get_stream_info(&ctx, &si);
    
    /* Iterate through decoded frames */
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Access image properties to explore more states */
        (void)img->fmt;
        (void)img->w;
        (void)img->h;
        (void)img->d_w;
        (void)img->d_h;
        (void)img->bit_depth;
        (void)img->cs;
        (void)img->range;
    }
    
    /* Clean up */
    vpx_codec_destroy(&ctx);
    
    return 0;
}