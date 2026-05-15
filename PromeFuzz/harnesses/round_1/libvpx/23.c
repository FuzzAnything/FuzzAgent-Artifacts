// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

static vpx_image_t* create_test_image(const uint8_t* data, size_t size, int use_wrap, unsigned char** allocated_buffer) {
    if (size < 16) return NULL;
    
    // Extract parameters from fuzz data
    vpx_img_fmt_t fmt = (vpx_img_fmt_t)(data[0] % 16);
    unsigned int d_w = (data[1] << 8) | data[2];
    unsigned int d_h = (data[3] << 8) | data[4];
    unsigned int align = 1 << (data[5] % 8); // 1,2,4,8,16,32,64,128
    unsigned int stride_align = align;
    
    // Limit dimensions to avoid excessive memory usage
    if (d_w > 1024) d_w = 1024;
    if (d_h > 1024) d_h = 1024;
    if (align > 128) align = 128;
    if (stride_align > 128) stride_align = 128;
    
    vpx_image_t* img = NULL;
    
    if (use_wrap) {
        // Calculate approximate buffer size needed
        size_t buffer_size = d_w * d_h * 3; // Conservative estimate
        if (buffer_size > size - 16) buffer_size = size - 16;
        if (buffer_size == 0) return NULL;
        
        unsigned char* img_data = (unsigned char*)malloc(buffer_size);
        if (!img_data) return NULL;
        
        // Fill with fuzz data
        size_t copy_size = buffer_size < (size - 16) ? buffer_size : (size - 16);
        memcpy(img_data, data + 16, copy_size);
        
        img = vpx_img_wrap(NULL, fmt, d_w, d_h, stride_align, img_data);
        
        // If wrap failed, free the buffer
        if (!img) {
            free(img_data);
        } else {
            // Store the allocated buffer pointer for later cleanup
            *allocated_buffer = img_data;
        }
    } else {
        img = vpx_img_alloc(NULL, fmt, d_w, d_h, align);
        
        // Fill image data with fuzz data if allocation succeeded
        if (img && img->img_data && size > 16) {
            size_t copy_size = img->w * img->h;
            if (copy_size > size - 16) copy_size = size - 16;
            memcpy(img->img_data, data + 16, copy_size);
        }
    }
    
    return img;
}

static vpx_codec_ctx_t* create_test_encoder(const uint8_t* data, size_t size) {
    if (size < 24) return NULL;
    
    vpx_codec_ctx_t* ctx = (vpx_codec_ctx_t*)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    
    // Initialize with some default values
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));
    
    // Use VP8 encoder interface
    ctx->iface = vpx_codec_vp8_cx();
    if (!ctx->iface) {
        free(ctx);
        return NULL;
    }
    
    // Set some basic configuration from fuzz data
    ctx->err = VPX_CODEC_OK;
    ctx->name = "VP8 Encoder";
    
    return ctx;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;
    
    // Test vpx_img_alloc and vpx_img_wrap
    int use_wrap = Data[6] % 2;
    unsigned char* allocated_buffer = NULL;
    vpx_image_t* img = create_test_image(Data, Size, use_wrap, &allocated_buffer);
    
    if (img) {
        // Test vpx_img_set_rect with random coordinates from fuzz data
        if (Size >= 40) {
            unsigned int x = (Data[20] << 8) | Data[21];
            unsigned int y = (Data[22] << 8) | Data[23];
            unsigned int w = (Data[24] << 8) | Data[25];
            unsigned int h = (Data[26] << 8) | Data[27];
            
            // Ensure rectangle is within bounds
            if (x < img->w && y < img->h) {
                if (w > img->w - x) w = img->w - x;
                if (h > img->h - y) h = img->h - y;
                vpx_img_set_rect(img, x, y, w, h);
            }
        }
        
        // Test vpx_img_flip
        if (Data[7] % 2) {
            vpx_img_flip(img);
        }
        
        // Test encoder functions if we have enough data
        if (Size >= 48) {
            vpx_codec_ctx_t* encoder = create_test_encoder(Data, Size);
            if (encoder) {
                // Extract encoding parameters from fuzz data
                vpx_codec_pts_t pts = (Data[28] << 24) | (Data[29] << 16) | 
                                      (Data[30] << 8) | Data[31];
                unsigned long duration = (Data[32] << 24) | (Data[33] << 16) | 
                                        (Data[34] << 8) | Data[35];
                vpx_enc_frame_flags_t flags = (Data[36] << 24) | (Data[37] << 16) | 
                                             (Data[38] << 8) | Data[39];
                vpx_enc_deadline_t deadline = (Data[40] << 24) | (Data[41] << 16) | 
                                             (Data[42] << 8) | Data[43];
                
                // Try encoding with image
                vpx_codec_encode(encoder, img, pts, duration, flags, deadline);
                
                // Try encoding with NULL image (flush)
                if (Data[8] % 2) {
                    vpx_codec_encode(encoder, NULL, pts + 1, duration, flags, deadline);
                }
                
                free(encoder);
            }
        }
        
        // Clean up image
        vpx_img_free(img);
        
        // Free the allocated buffer if we used vpx_img_wrap
        if (allocated_buffer) {
            free(allocated_buffer);
        }
    }
    
    // Test vpx_img_free with NULL (should be safe)
    if (Data[9] % 2) {
        vpx_img_free(NULL);
    }
    
    return 0;
}