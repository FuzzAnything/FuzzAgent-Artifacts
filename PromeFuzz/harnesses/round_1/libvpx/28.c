// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

static vpx_codec_ctx_t* init_encoder(const uint8_t* data, size_t size) {
    static vpx_codec_ctx_t ctx;
    static int initialized = 0;
    
    if (!initialized) {
        vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
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
        
        initialized = 1;
    }
    
    return &ctx;
}

static void cleanup_encoder() {
    static vpx_codec_ctx_t ctx;
    vpx_codec_destroy(&ctx);
}

static size_t calculate_image_size(vpx_image_t* img) {
    if (!img || !img->img_data) return 0;
    
    size_t total_size = 0;
    for (int plane = 0; plane < 4; ++plane) {
        if (img->planes[plane]) {
            unsigned int plane_height = img->h;
            if (plane > 0) {
                plane_height = (plane_height + img->y_chroma_shift) >> img->y_chroma_shift;
            }
            total_size += img->stride[plane] * plane_height;
        }
    }
    return total_size;
}

int LLVMFuzzerTestOneInput_28(const uint8_t* Data, size_t Size) {
    if (Size < 16) return 0;
    
    vpx_codec_ctx_t* encoder = init_encoder(Data, Size);
    if (!encoder) return 0;
    
    size_t offset = 0;
    
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    if (Size >= 1) {
        uint8_t fmt_byte = Data[offset++];
        fmt = (fmt_byte % 8) + 2;
    }
    
    unsigned int width = 64;
    unsigned int height = 64;
    if (Size - offset >= 2) {
        width = (Data[offset++] % 128) + 1;
        height = (Data[offset++] % 128) + 1;
    }
    
    unsigned int align = 16;
    if (Size - offset >= 1) {
        align = 1 << (Data[offset++] % 8);
        if (align == 0) align = 1;
        if (align > 65536) align = 65536;
    }
    
    vpx_image_t* img_alloc = vpx_img_alloc(NULL, fmt, width, height, align);
    if (img_alloc) {
        if (img_alloc->img_data) {
            size_t image_size = calculate_image_size(img_alloc);
            if (image_size > 0 && Size > offset) {
                size_t copy_size = Size - offset;
                if (copy_size > image_size) copy_size = image_size;
                memcpy(img_alloc->img_data, Data + offset, copy_size);
                offset += copy_size;
            }
        }
        
        if (Size - offset >= 4) {
            unsigned int x = width > 0 ? Data[offset] % width : 0;
            unsigned int y = height > 0 ? Data[offset + 1] % height : 0;
            unsigned int w = (width > x) ? Data[offset + 2] % (width - x) : 0;
            unsigned int h = (height > y) ? Data[offset + 3] % (height - y) : 0;
            
            vpx_img_set_rect(img_alloc, x, y, w, h);
            offset += 4;
        }
        
        vpx_img_flip(img_alloc);
        
        if (encoder) {
            vpx_codec_pts_t pts = 0;
            unsigned long duration = 1;
            vpx_enc_frame_flags_t flags = 0;
            vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
            
            if (Size - offset >= 8) {
                pts = (vpx_codec_pts_t)(*(uint64_t*)(Data + offset));
                offset += 8;
            }
            
            vpx_codec_encode(encoder, img_alloc, pts, duration, flags, deadline);
            
            vpx_codec_encode(encoder, NULL, pts + 1, duration, flags, deadline);
        }
        
        vpx_img_free(img_alloc);
    }
    
    if (Size - offset >= 8) {
        unsigned int stride_align = 16;
        if (Size - offset >= 9) {
            stride_align = 1 << (Data[offset + 8] % 8);
            if (stride_align == 0) stride_align = 1;
            if (stride_align > 65536) stride_align = 65536;
        }
        
        size_t buffer_size = width * height * 3;
        if (buffer_size > 0 && buffer_size <= 1024 * 1024) {
            unsigned char* img_data = (unsigned char*)malloc(buffer_size);
            if (img_data) {
                size_t copy_size = Size - offset;
                if (copy_size > buffer_size) copy_size = buffer_size;
                if (copy_size > 0) {
                    memcpy(img_data, Data + offset, copy_size);
                }
                
                vpx_image_t img_wrap;
                vpx_img_wrap(&img_wrap, fmt, width, height, stride_align, img_data);
                
                if (Size - offset >= 12) {
                    unsigned int x = width > 0 ? Data[offset + 4] % width : 0;
                    unsigned int y = height > 0 ? Data[offset + 5] % height : 0;
                    unsigned int w = (width > x) ? Data[offset + 6] % (width - x) : 0;
                    unsigned int h = (height > y) ? Data[offset + 7] % (height - y) : 0;
                    
                    vpx_img_set_rect(&img_wrap, x, y, w, h);
                }
                
                vpx_img_flip(&img_wrap);
                
                vpx_img_free(&img_wrap);
                free(img_data);
            }
        }
    }
    
    cleanup_encoder();
    return 0;
}