// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

static vpx_image_t* create_test_image(const uint8_t *data, size_t size, vpx_img_fmt_t fmt) {
    if (size < 12) return NULL;
    
    // Extract dimensions from fuzzer data
    unsigned int width = (data[0] << 8) | data[1];
    unsigned int height = (data[2] << 8) | data[3];
    unsigned int align = (data[4] & 0x3F) + 1; // 1-64
    
    // Limit dimensions to reasonable values for fuzzing
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Create image using vpx_img_alloc
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (!img) return NULL;
    
    // Fill image planes with fuzzer data
    size_t offset = 5;
    for (int plane = 0; plane < 3; plane++) {
        if (img->planes[plane]) {
            int stride = img->stride[plane];
            int plane_height = (plane == 0) ? height : (height >> img->y_chroma_shift);
            
            for (int y = 0; y < plane_height && offset < size; y++) {
                int row_size = stride < (int)(size - offset) ? stride : (int)(size - offset);
                if (row_size > 0) {
                    memcpy(img->planes[plane] + y * stride, data + offset, row_size);
                    offset += row_size;
                }
            }
        }
    }
    
    return img;
}

static vpx_codec_ctx_t* init_test_encoder(vpx_img_fmt_t fmt, unsigned int width, unsigned int height) {
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return NULL;
    
    vpx_codec_ctx_t *codec = (vpx_codec_ctx_t*)malloc(sizeof(vpx_codec_ctx_t));
    if (!codec) return NULL;
    
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(codec);
        return NULL;
    }
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (vpx_codec_enc_init(codec, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(codec);
        return NULL;
    }
    
    return codec;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    // Use fuzzer data to determine test parameters
    uint8_t test_case = Data[0] % 6;
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    uint8_t fmt_selector = Data[1] % 4;
    
    switch (fmt_selector) {
        case 0: fmt = VPX_IMG_FMT_I420; break;
        case 1: fmt = VPX_IMG_FMT_I422; break;
        case 2: fmt = VPX_IMG_FMT_I444; break;
        case 3: fmt = VPX_IMG_FMT_NV12; break;
    }
    
    // Test different API functions based on test_case
    switch (test_case) {
        case 0: {
            // Test vpx_img_alloc and vpx_img_free
            unsigned int width = (Data[2] << 8) | Data[3];
            unsigned int height = (Data[4] << 8) | Data[5];
            unsigned int align = (Data[6] & 0x3F) + 1;
            
            if (width > 0 && height > 0 && width < 4096 && height < 4096) {
                vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
                if (img) {
                    vpx_img_free(img);
                }
            }
            break;
        }
        
        case 1: {
            // Test vpx_img_wrap and vpx_img_set_rect
            unsigned int width = (Data[2] << 8) | Data[3];
            unsigned int height = (Data[4] << 8) | Data[5];
            unsigned int stride_align = (Data[6] & 0x3F) + 1;
            
            if (width > 0 && height > 0 && width < 4096 && height < 4096) {
                // Calculate required buffer size
                size_t buffer_size = width * height * 3 / 2; // Approximate for YUV
                unsigned char *img_data = (unsigned char*)malloc(buffer_size);
                if (img_data) {
                    // Fill with fuzzer data
                    size_t copy_size = Size - 7 < buffer_size ? Size - 7 : buffer_size;
                    if (copy_size > 0) {
                        memcpy(img_data, Data + 7, copy_size);
                    }
                    
                    vpx_image_t *img = vpx_img_wrap(NULL, fmt, width, height, stride_align, img_data);
                    if (img) {
                        // Test vpx_img_set_rect with fuzzer-provided coordinates
                        unsigned int x = (Data[7] << 8) | Data[8];
                        unsigned int y = (Data[9] << 8) | Data[10];
                        unsigned int w = (Data[11] << 8) | Data[12];
                        unsigned int h = (Data[13] << 8) | Data[14];
                        
                        x = x % (width > 0 ? width : 1);
                        y = y % (height > 0 ? height : 1);
                        w = w % (width - x);
                        h = h % (height - y);
                        
                        if (w > 0 && h > 0) {
                            vpx_img_set_rect(img, x, y, w, h);
                        }
                        
                        vpx_img_free(img);
                    }
                    free(img_data);
                }
            }
            break;
        }
        
        case 2: {
            // Test vpx_img_flip
            vpx_image_t *img = create_test_image(Data + 1, Size - 1, fmt);
            if (img) {
                vpx_img_flip(img);
                vpx_img_free(img);
            }
            break;
        }
        
        case 3: {
            // Test vpx_codec_encode with image
            vpx_image_t *img = create_test_image(Data + 1, Size - 1, fmt);
            if (img && img->w > 0 && img->h > 0) {
                vpx_codec_ctx_t *codec = init_test_encoder(fmt, img->w, img->h);
                if (codec) {
                    vpx_codec_pts_t pts = (Data[5] << 24) | (Data[6] << 16) | (Data[7] << 8) | Data[8];
                    unsigned long duration = (Data[9] << 24) | (Data[10] << 16) | (Data[11] << 8) | Data[12];
                    vpx_enc_frame_flags_t flags = Data[13];
                    vpx_enc_deadline_t deadline = Data[14] % 3; // 0, 1, or 2
                    
                    vpx_codec_encode(codec, img, pts, duration, flags, deadline);
                    
                    vpx_codec_destroy(codec);
                    free(codec);
                }
                vpx_img_free(img);
            }
            break;
        }
        
        case 4: {
            // Test vpx_codec_encode flush (NULL img)
            unsigned int width = (Data[1] << 8) | Data[2];
            unsigned int height = (Data[3] << 8) | Data[4];
            
            if (width > 0 && height > 0 && width < 4096 && height < 4096) {
                vpx_codec_ctx_t *codec = init_test_encoder(fmt, width, height);
                if (codec) {
                    vpx_codec_pts_t pts = (Data[5] << 24) | (Data[6] << 16) | (Data[7] << 8) | Data[8];
                    unsigned long duration = (Data[9] << 24) | (Data[10] << 16) | (Data[11] << 8) | Data[12];
                    vpx_enc_frame_flags_t flags = Data[13];
                    vpx_enc_deadline_t deadline = Data[14] % 3;
                    
                    // Encode with NULL to flush
                    vpx_codec_encode(codec, NULL, pts, duration, flags, deadline);
                    
                    vpx_codec_destroy(codec);
                    free(codec);
                }
            }
            break;
        }
        
        case 5: {
            // Combined test: create image, set rect, flip, encode
            vpx_image_t *img = create_test_image(Data + 1, Size - 1, fmt);
            if (img && img->w > 0 && img->h > 0) {
                // Set rectangle
                unsigned int x = (Data[5] << 8) | Data[6];
                unsigned int y = (Data[7] << 8) | Data[8];
                unsigned int w = (Data[9] << 8) | Data[10];
                unsigned int h = (Data[11] << 8) | Data[12];
                
                x = x % (img->w > 0 ? img->w : 1);
                y = y % (img->h > 0 ? img->h : 1);
                w = w % (img->w - x);
                h = h % (img->h - y);
                
                if (w > 0 && h > 0) {
                    vpx_img_set_rect(img, x, y, w, h);
                }
                
                // Flip image
                vpx_img_flip(img);
                
                // Encode
                vpx_codec_ctx_t *codec = init_test_encoder(fmt, img->w, img->h);
                if (codec) {
                    vpx_codec_pts_t pts = (Data[13] << 24) | (Data[14] << 16) | (Data[15] << 8) | Data[16];
                    unsigned long duration = 1;
                    vpx_enc_frame_flags_t flags = 0;
                    vpx_enc_deadline_t deadline = 0;
                    
                    if (Size > 20) {
                        duration = (Data[17] << 8) | Data[18];
                        flags = Data[19];
                        deadline = Data[20] % 3;
                    }
                    
                    vpx_codec_encode(codec, img, pts, duration, flags, deadline);
                    
                    vpx_codec_destroy(codec);
                    free(codec);
                }
                
                vpx_img_free(img);
            }
            break;
        }
    }
    
    return 0;
}