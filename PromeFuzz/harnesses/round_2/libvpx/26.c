// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"

static vpx_image_t* create_test_image(vpx_img_fmt_t fmt, unsigned int w, unsigned int h, unsigned int align) {
    return vpx_img_alloc(NULL, fmt, w, h, align);
}

static vpx_codec_ctx_t* create_test_encoder(void) {
    static vpx_codec_ctx_t ctx;
    const vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    
    if (!iface) return NULL;
    
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return NULL;
    }
    
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return NULL;
    }
    
    return &ctx;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // For vpx_img_set_rect
    unsigned int x = *(unsigned int*)(Data + offset); offset += sizeof(unsigned int);
    unsigned int y = *(unsigned int*)(Data + offset); offset += sizeof(unsigned int);
    unsigned int w = *(unsigned int*)(Data + offset); offset += sizeof(unsigned int);
    unsigned int h = *(unsigned int*)(Data + offset); offset += sizeof(unsigned int);
    
    // For image creation
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    unsigned int d_w = 64;
    unsigned int d_h = 64;
    unsigned int align = 16;
    
    // Test vpx_img_alloc and vpx_img_set_rect
    vpx_image_t* img1 = create_test_image(fmt, d_w, d_h, align);
    if (img1) {
        vpx_img_set_rect(img1, x % d_w, y % d_h, w % d_w, h % d_h);
        vpx_img_flip(img1);
        vpx_img_free(img1);
    }
    
    // Test vpx_img_wrap
    if (Size - offset > d_w * d_h) {
        vpx_image_t img2;
        unsigned char* img_data = (unsigned char*)(Data + offset);
        vpx_image_t* wrapped = vpx_img_wrap(&img2, fmt, d_w, d_h, align, img_data);
        if (wrapped) {
            vpx_img_set_rect(wrapped, x % d_w, y % d_h, w % d_w, h % d_h);
            vpx_img_flip(wrapped);
            vpx_img_free(wrapped);
        }
    }
    
    // Test vpx_codec_encode
    vpx_codec_ctx_t* encoder = create_test_encoder();
    if (encoder) {
        vpx_image_t* img3 = create_test_image(fmt, d_w, d_h, align);
        if (img3) {
            // Fill with some data
            if (Size > offset) {
                size_t copy_size = d_w * d_h;
                if (copy_size > Size - offset) copy_size = Size - offset;
                for (int plane = 0; plane < 3; plane++) {
                    if (img3->planes[plane]) {
                        size_t plane_size = (plane == 0) ? d_w * d_h : (d_w * d_h) / 4;
                        if (plane_size > copy_size) plane_size = copy_size;
                        memcpy(img3->planes[plane], Data + offset, plane_size);
                    }
                }
            }
            
            vpx_codec_encode(encoder, img3, 0, 1, 0, VPX_DL_REALTIME);
            vpx_codec_encode(encoder, NULL, 1, 1, 0, VPX_DL_REALTIME);
            vpx_img_free(img3);
        }
        vpx_codec_destroy(encoder);
    }
    
    return 0;
}