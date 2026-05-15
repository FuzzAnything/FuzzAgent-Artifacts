// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

static vpx_codec_ctx_t* init_encoder() {
    static vpx_codec_ctx_t codec;
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t cfg;
    
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return NULL;
    }
    
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (vpx_codec_enc_init(&codec, iface, &cfg, 0) != VPX_CODEC_OK) {
        return NULL;
    }
    
    return &codec;
}

static vpx_image_t* create_test_image(vpx_img_fmt_t fmt, unsigned int w, unsigned int h) {
    vpx_image_t* img = vpx_img_alloc(NULL, fmt, w, h, 1);
    if (!img) return NULL;
    
    img->cs = VPX_CS_BT_601;
    img->range = VPX_CR_STUDIO_RANGE;
    
    if (img->img_data) {
        size_t data_size = 0;
        if (fmt & VPX_IMG_FMT_PLANAR) {
            for (int plane = 0; plane < 3; plane++) {
                unsigned int plane_w = (plane == 0) ? w : (w >> img->x_chroma_shift);
                unsigned int plane_h = (plane == 0) ? h : (h >> img->y_chroma_shift);
                data_size += plane_w * plane_h;
            }
        } else {
            data_size = w * h * 3;
        }
        
        if (fmt & VPX_IMG_FMT_HIGHBITDEPTH) {
            data_size *= 2;
        }
        
        for (size_t i = 0; i < data_size && i < 1024; i++) {
            img->img_data[i] = (unsigned char)(i % 256);
        }
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    uint32_t seed = *(uint32_t*)Data;
    Data += 4;
    Size -= 4;
    
    srand(seed);
    
    vpx_img_fmt_t formats[] = {
        VPX_IMG_FMT_YV12,
        VPX_IMG_FMT_I420,
        VPX_IMG_FMT_I422,
        VPX_IMG_FMT_I444,
        VPX_IMG_FMT_NV12,
        VPX_IMG_FMT_I42016,
        VPX_IMG_FMT_I42216,
        VPX_IMG_FMT_I44416
    };
    
    vpx_img_fmt_t fmt = formats[seed % (sizeof(formats)/sizeof(formats[0]))];
    
    unsigned int w = 64 + (seed % 256);
    unsigned int h = 64 + ((seed >> 8) % 256);
    
    vpx_image_t* img1 = create_test_image(fmt, w, h);
    if (!img1) return 0;
    
    unsigned int x = seed % (w + 100);
    unsigned int y = (seed >> 8) % (h + 100);
    unsigned int rect_w = (seed >> 16) % (w + 100);
    unsigned int rect_h = (seed >> 24) % (h + 100);
    
    vpx_img_set_rect(img1, x, y, rect_w, rect_h);
    
    vpx_img_flip(img1);
    
    vpx_codec_ctx_t* encoder = init_encoder();
    if (encoder) {
        vpx_codec_pts_t pts = seed;
        unsigned long duration = 1;
        vpx_enc_frame_flags_t flags = 0;
        vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
        
        vpx_codec_encode(encoder, img1, pts, duration, flags, deadline);
        
        vpx_codec_encode(encoder, NULL, pts + 1, duration, flags, deadline);
        
        vpx_codec_destroy(encoder);
    }
    
    vpx_image_t img2;
    unsigned int stride_align = 1 + (seed % 32);
    
    size_t buffer_size = w * h * 3;
    if (fmt & VPX_IMG_FMT_HIGHBITDEPTH) {
        buffer_size *= 2;
    }
    
    unsigned char* img_data = (unsigned char*)malloc(buffer_size);
    if (img_data) {
        for (size_t i = 0; i < buffer_size && i < Size; i++) {
            img_data[i] = Data[i % Size];
        }
        
        vpx_img_wrap(&img2, fmt, w, h, stride_align, img_data);
        
        unsigned int x2 = (seed >> 4) % (w + 50);
        unsigned int y2 = (seed >> 12) % (h + 50);
        unsigned int rect_w2 = (seed >> 20) % (w + 50);
        unsigned int rect_h2 = ((seed >> 28) | (seed << 4)) % (h + 50);
        
        vpx_img_set_rect(&img2, x2, y2, rect_w2, rect_h2);
        
        vpx_img_flip(&img2);
        
        free(img_data);
    }
    
    vpx_image_t* img3 = vpx_img_alloc(NULL, fmt, w/2, h/2, 16);
    if (img3) {
        unsigned int x3 = seed % (w/2 + 20);
        unsigned int y3 = (seed >> 8) % (h/2 + 20);
        unsigned int rect_w3 = (seed >> 16) % (w/2 + 20);
        unsigned int rect_h3 = (seed >> 24) % (h/2 + 20);
        
        vpx_img_set_rect(img3, x3, y3, rect_w3, rect_h3);
        
        vpx_img_flip(img3);
        
        vpx_img_free(img3);
    }
    
    vpx_img_free(img1);
    
    return 0;
}