// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
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

static vpx_codec_ctx_t* init_encoder() {
    static vpx_codec_ctx_t ctx;
    static vpx_codec_enc_cfg_t cfg;
    
    // Get VP8 encoder interface
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    if (!iface) return NULL;
    
    // Initialize config with default values
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return NULL;
    }
    
    // Set some reasonable defaults for fuzzing
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_pass = VPX_RC_ONE_PASS;
    
    // Initialize encoder
    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return NULL;
    }
    
    return &ctx;
}

static void cleanup_encoder(vpx_codec_ctx_t* ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

static vpx_image_t* create_test_image(const uint8_t* data, size_t size, 
                                      vpx_img_fmt_t fmt, unsigned int width, 
                                      unsigned int height, unsigned int align) {
    if (size < width * height * 3) {
        return NULL;
    }
    
    vpx_image_t* img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (!img) return NULL;
    
    // Fill image planes with fuzzer data
    size_t offset = 0;
    for (int plane = 0; plane < 3; plane++) {
        if (img->planes[plane]) {
            size_t plane_size = img->stride[plane] * height;
            size_t copy_size = (size - offset < plane_size) ? size - offset : plane_size;
            if (copy_size > 0) {
                memcpy(img->planes[plane], data + offset, copy_size);
                offset += copy_size;
            }
        }
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_21(const uint8_t* Data, size_t Size) {
    if (Size < 32) return 0;
    
    // Parse parameters from fuzzer input
    size_t offset = 0;
    
    // Extract image format (use modulo to ensure valid value)
    vpx_img_fmt_t fmt = (vpx_img_fmt_t)(Data[offset++] % 10);
    
    // Extract dimensions (ensure reasonable bounds for fuzzing)
    unsigned int width = (Data[offset++] % 128) + 1;
    unsigned int height = (Data[offset++] % 128) + 1;
    unsigned int align = (Data[offset++] % 32) + 1;
    
    // Extract rectangle parameters
    unsigned int rect_x = Data[offset++] % width;
    unsigned int rect_y = Data[offset++] % height;
    unsigned int rect_w = (Data[offset++] % (width - rect_x)) + 1;
    unsigned int rect_h = (Data[offset++] % (height - rect_y)) + 1;
    
    // Extract encoder parameters
    vpx_codec_pts_t pts = (vpx_codec_pts_t)(Data[offset++] * 1000);
    unsigned long duration = Data[offset++] * 1000;
    vpx_enc_frame_flags_t flags = (vpx_enc_frame_flags_t)(Data[offset++] % 4);
    vpx_enc_deadline_t deadline = Data[offset++] % 3; // 0, 1, or 2
    
    // Initialize encoder
    vpx_codec_ctx_t* encoder = init_encoder();
    if (!encoder) return 0;
    
    // Test vpx_img_alloc
    vpx_image_t* img1 = create_test_image(Data + offset, Size - offset, 
                                         fmt, width, height, align);
    if (!img1) {
        cleanup_encoder(encoder);
        return 0;
    }
    
    // Test vpx_img_set_rect
    if (rect_x < width && rect_y < height && 
        rect_w > 0 && rect_h > 0 &&
        rect_x + rect_w <= width && 
        rect_y + rect_h <= height) {
        vpx_img_set_rect(img1, rect_x, rect_y, rect_w, rect_h);
    }
    
    // Test vpx_img_flip
    vpx_img_flip(img1);
    
    // Test vpx_codec_encode with image
    vpx_codec_encode(encoder, img1, pts, duration, flags, deadline);
    
    // Test vpx_codec_encode with NULL (flush)
    vpx_codec_encode(encoder, NULL, pts + 1000, duration, flags, deadline);
    
    // Test vpx_img_wrap with existing buffer
    unsigned char* img_data = (unsigned char*)malloc(width * height * 3);
    if (img_data) {
        size_t copy_size = (Size - offset < width * height * 3) ? 
                          Size - offset : width * height * 3;
        if (copy_size > 0) {
            memcpy(img_data, Data + offset, copy_size);
        }
        
        vpx_image_t* img2 = vpx_img_wrap(NULL, fmt, width, height, align, img_data);
        if (img2) {
            // Test vpx_img_set_rect on wrapped image
            if (rect_x < width && rect_y < height && 
                rect_w > 0 && rect_h > 0 &&
                rect_x + rect_w <= width && 
                rect_y + rect_h <= height) {
                vpx_img_set_rect(img2, rect_x, rect_y, rect_w, rect_h);
            }
            
            // Test vpx_img_flip on wrapped image
            vpx_img_flip(img2);
            
            // Test encode with wrapped image
            vpx_codec_encode(encoder, img2, pts + 2000, duration, flags, deadline);
            
            // Free wrapped image (doesn't free img_data)
            vpx_img_free(img2);
        }
        free(img_data);
    }
    
    // Cleanup
    vpx_img_free(img1);
    cleanup_encoder(encoder);
    
    return 0;
}