// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_set_frame_buffer_functions at vpx_decoder.c:173:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

/* Frame buffer callback functions */
static int get_frame_buffer(void* priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    (void)priv;
    fb->data = (uint8_t*)malloc(min_size);
    if (!fb->data) return -1;
    fb->size = min_size;
    fb->priv = fb->data; /* Store pointer for freeing */
    return 0;
}

static int release_frame_buffer(void* priv, vpx_codec_frame_buffer_t* fb) {
    (void)priv;
    free(fb->priv);
    return 0;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    vpx_codec_ctx_t ctx;
    const vpx_codec_iface_t *iface = NULL;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_err_t res;
    
    /* Choose codec interface based on input - safely */
    uint8_t codec_choice = Data[0] % 2;
    if (codec_choice == 0) {
        iface = vpx_codec_vp8_dx();
    } else {
        iface = vpx_codec_vp9_dx();
    }
    
    if (!iface) return 0;
    
    /* Configure decoder with safe bounds checking */
    cfg.threads = 1; /* Default value */
    cfg.w = 16; /* Default minimum */
    cfg.h = 16; /* Default minimum */
    
    /* Only read Data[1], Data[2], Data[3] if they exist */
    if (Size > 1) {
        cfg.threads = (Data[1] % 8) + 1;
    }
    if (Size > 2) {
        cfg.w = (Data[2] % 256) * 16 + 16;
    }
    if (Size > 3) {
        cfg.h = (Data[3] % 256) * 16 + 16;
    }
    
    /* Initialize decoder with different flags - safely */
    uint8_t flags_choice = 0;
    if (Size > 4) {
        flags_choice = Data[4] % 4;
    }
    vpx_codec_flags_t flags = 0;
    if (flags_choice & 0x1) flags |= VPX_CODEC_USE_INPUT_FRAGMENTS;
    if (flags_choice & 0x2) flags |= VPX_CODEC_USE_POSTPROC;
    
    /* Try initialization with and without config - safely */
    const vpx_codec_dec_cfg_t *cfg_ptr = &cfg; /* Default to using config */
    if (Size > 5) {
        cfg_ptr = (Data[5] % 2) ? &cfg : NULL;
    }
    
    res = vpx_codec_dec_init_ver(&ctx, iface, cfg_ptr, flags, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        /* Initialization failed, cleanup and return */
        vpx_codec_destroy(&ctx);
        return 0;
    }
    
    /* Set frame buffer functions based on input - safely */
    if (Size > 6 && (Data[6] % 2)) {
        vpx_codec_set_frame_buffer_functions(&ctx, get_frame_buffer, 
                                           release_frame_buffer, NULL);
    }
    
    /* Decode the input data (skip first 7 bytes used for configuration if available) */
    size_t data_offset = (Size > 7) ? 7 : 0;
    size_t data_sz = Size - data_offset;
    const uint8_t *decode_data = (data_sz > 0) ? Data + data_offset : NULL;
    
    /* Try different deadline values - safely */
    long deadline = 0; /* Default to unlimited */
    if (Size > 0) {
        deadline = (Data[0] % 3) * 1000;
    }
    
    /* Decode with the data */
    res = vpx_codec_decode(&ctx, decode_data, (unsigned int)data_sz, NULL, deadline);
    
    /* Try control API with different control IDs - safely */
    int control_id;
    int int_arg;
    
    /* Try a few common control IDs - safely */
    uint8_t control_choice = 0;
    if (Size > 0) {
        control_choice = Data[0] % 5;
    }
    
    switch (control_choice) {
        case 0:
            control_id = VP8D_GET_LAST_REF_UPDATES;
            vpx_codec_control_(&ctx, control_id, &int_arg);
            break;
        case 1:
            control_id = VP8D_GET_FRAME_CORRUPTED;
            vpx_codec_control_(&ctx, control_id, &int_arg);
            break;
        case 2:
            /* Use VP8 control for VP9 as well - will test error handling */
            control_id = VP8D_GET_LAST_REF_UPDATES;
            vpx_codec_control_(&ctx, control_id, &int_arg);
            break;
        case 3:
            control_id = VP8D_GET_FRAME_CORRUPTED;
            vpx_codec_control_(&ctx, control_id, &int_arg);
            break;
        case 4:
            /* Try an invalid control ID to test error handling */
            control_id = 9999;
            vpx_codec_control_(&ctx, control_id, &int_arg);
            break;
    }
    
    /* Get decoded frames */
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
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
    
    /* Try decoding with NULL data to flush - safely */
    if (Size > 1 && (Data[1] % 2)) {
        vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        
        /* Get any remaining frames after flush */
        iter = NULL;
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            (void)img;
        }
    }
    
    /* Try control API again after decode - safely */
    if (Size > 2 && (Data[2] % 2)) {
        control_id = (codec_choice == 0) ? VP8D_GET_LAST_REF_UPDATES : VP8D_GET_FRAME_CORRUPTED;
        vpx_codec_control_(&ctx, control_id, &int_arg);
    }
    
    /* Cleanup */
    vpx_codec_destroy(&ctx);
    
    return 0;
}