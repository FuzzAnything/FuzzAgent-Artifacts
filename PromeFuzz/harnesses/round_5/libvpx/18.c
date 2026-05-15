// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"

static vpx_image_t *create_test_image(int width, int height, vpx_img_fmt_t fmt, const uint8_t *data, size_t size) {
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    unsigned int plane;
    for (plane = 0; plane < 3; ++plane) {
        unsigned int plane_width = (plane ? (width + img->x_chroma_shift) >> img->x_chroma_shift : width);
        unsigned int plane_height = (plane ? (height + img->y_chroma_shift) >> img->y_chroma_shift : height);
        unsigned int stride = plane_width;
        
        for (unsigned int y = 0; y < plane_height; ++y) {
            size_t offset = (y * stride) % size;
            size_t copy_size = stride < (size - offset) ? stride : (size - offset);
            if (copy_size > 0) {
                memcpy(img->planes[plane] + y * img->stride[plane], data + offset, copy_size);
            }
        }
    }
    
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    img->bit_depth = (fmt & VPX_IMG_FMT_HIGHBITDEPTH) ? 10 : 8;
    img->range = VPX_CR_FULL_RANGE;
    img->cs = VPX_CS_BT_709;
    
    return img;
}

static void free_test_image(vpx_image_t *img) {
    if (img) vpx_img_free(img);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    
    // Get VP9 encoder interface
    extern vpx_codec_iface_t vpx_codec_vp9_cx_algo;
    vpx_codec_iface_t *encoder_iface = &vpx_codec_vp9_cx_algo;
    
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_lag_in_frames = 0;
    
    if (vpx_codec_enc_init(&encoder, encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    
    // Get VP9 decoder interface
    extern vpx_codec_iface_t vpx_codec_vp9_dx_algo;
    vpx_codec_iface_t *decoder_iface = &vpx_codec_vp9_dx_algo;
    
    if (vpx_codec_dec_init(&decoder, decoder_iface, NULL, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        return 0;
    }
    
    // Test vpx_codec_get_global_headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    (void)global_headers;
    
    // Test vpx_codec_encode with test image
    if (Size > 100) {
        vpx_image_t *img = create_test_image(64, 64, VPX_IMG_FMT_I420, Data, Size);
        if (img) {
            vpx_codec_encode(&encoder, img, 0, 1, 0, 0);
            free_test_image(img);
        }
        
        // Flush encoder
        vpx_codec_encode(&encoder, NULL, 0, 1, 0, 0);
        
        // Test vpx_codec_get_cx_data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
            (void)pkt;
        }
    }
    
    // Test vpx_codec_set_cx_data_buf
    if (Size > 50) {
        vpx_fixed_buf_t buf;
        buf.buf = malloc(Size);
        if (buf.buf) {
            buf.sz = Size;
            memcpy(buf.buf, Data, Size);
            
            unsigned int pad_before = Data[0] % 16;
            unsigned int pad_after = Data[1] % 16;
            vpx_codec_set_cx_data_buf(&encoder, &buf, pad_before, pad_after);
            
            free(buf.buf);
        }
        
        // Test with NULL buffer
        vpx_codec_set_cx_data_buf(&encoder, NULL, 0, 0);
    }
    
    // Test vpx_codec_decode
    if (Size > 0) {
        size_t decode_size = Size > 1024 ? 1024 : Size;
        vpx_codec_decode(&decoder, Data, decode_size, NULL, 0);
        
        // Flush decoder
        vpx_codec_decode(&decoder, NULL, 0, NULL, 0);
        
        // Test vpx_codec_error_detail
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail;
        
        error_detail = vpx_codec_error_detail(&encoder);
        (void)error_detail;
    }
    
    // Cleanup
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    
    return 0;
}