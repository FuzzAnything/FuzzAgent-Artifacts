// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_decoder.h"

static void dummy_put_slice_cb(void *user_priv,
                               const vpx_image_t *img,
                               const vpx_image_rect_t *rect,
                               const vpx_image_rect_t *valid_rect) {
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)valid_rect;
}

static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = NULL;
    
    extern vpx_codec_iface_t vpx_codec_vp8_dx_algo;
    extern vpx_codec_iface_t vpx_codec_vp9_dx_algo;
    
    uint8_t config_byte = data[0];
    if (config_byte & 1) {
        iface = &vpx_codec_vp8_dx_algo;
    } else {
        iface = &vpx_codec_vp9_dx_algo;
    }
    
    if (!iface) {
        return 0;
    }

    vpx_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 640;
    cfg.h = 480;
    
    if (size > 1) {
        cfg.threads = (data[1] % 8) + 1;
    }
    if (size > 2) {
        cfg.w = (data[2] % 1024) + 1;
    }
    if (size > 3) {
        cfg.h = (data[3] % 1024) + 1;
    }

    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    void *user_priv = NULL;
    if (size > 4) {
        user_priv = (void *)(uintptr_t)data[4];
    }
    
    if (size > 5 && (data[5] & 1)) {
        vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, user_priv);
    }
    
    if (size > 5 && (data[5] & 2)) {
        vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, user_priv);
    }

    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    if (size > 5 && (data[5] & 4)) {
        vpx_codec_get_stream_info(&ctx, &si);
    }

    long deadline = 0;
    if (size > 6) {
        deadline = (data[6] & 1) ? 0 : 1000;
    }
    
    if (size > 10) {
        unsigned int data_sz = (unsigned int)(size - 10);
        if (data_sz > size) {
            data_sz = (unsigned int)size;
        }
        
        vpx_codec_decode(&ctx, data + 10, data_sz, user_priv, deadline);
        
        if (size > 7 && (data[7] & 1)) {
            vpx_codec_decode(&ctx, NULL, 0, user_priv, deadline);
        }
    }

    if (size > 8 && (data[8] & 1)) {
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *img;
        int frame_count = 0;
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL && frame_count < 10) {
            frame_count++;
            (void)img;
        }
    }

    if (size > 9 && (data[9] & 1)) {
        vpx_codec_get_stream_info(&ctx, &si);
    }

    vpx_codec_destroy(&ctx);
    return 0;
}