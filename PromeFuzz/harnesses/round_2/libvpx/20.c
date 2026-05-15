// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

static vpx_image_t *create_fuzzed_image(const uint8_t *data, size_t size, int use_wrap) {
    if (size < 8) return NULL;
    
    // Extract parameters from fuzzer data
    unsigned int width = (data[0] << 8) | data[1];
    unsigned int height = (data[2] << 8) | data[3];
    vpx_img_fmt_t fmt = (vpx_img_fmt_t)(data[4] % 8); // Limit to valid formats
    unsigned int align = (data[5] % 64) + 1; // Alignment must be at least 1
    
    // Ensure dimensions are reasonable for fuzzing
    if (width == 0) width = 1;
    if (height == 0) height = 1;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    vpx_image_t *img = NULL;
    
    if (use_wrap) {
        // For vpx_img_wrap, we need to allocate our own buffer
        // Calculate approximate buffer size needed
        size_t buffer_size = width * height * 3; // Conservative estimate
        if (buffer_size > size - 6 || buffer_size == 0) {
            buffer_size = 1024; // Fallback size
        }
        
        unsigned char *img_data = (unsigned char *)malloc(buffer_size);
        if (!img_data) return NULL;
        
        // Fill with fuzzer data if available
        size_t copy_size = (size - 6 > buffer_size) ? buffer_size : size - 6;
        if (copy_size > 0) {
            memcpy(img_data, data + 6, copy_size);
        }
        
        img = vpx_img_wrap(NULL, fmt, width, height, align, img_data);
        
        // If wrap failed, free our buffer
        if (!img) {
            free(img_data);
        } else {
            // Mark that we allocated the buffer
            img->img_data_owner = 1;
        }
    } else {
        // Use vpx_img_alloc
        img = vpx_img_alloc(NULL, fmt, width, height, align);
        
        // If allocation succeeded, fill with fuzzer data
        if (img && img->img_data && size > 6) {
            size_t copy_size = (size - 6 > (size_t)(img->w * img->h * 3)) ? 
                              (size_t)(img->w * img->h * 3) : size - 6;
            if (copy_size > 0) {
                memcpy(img->img_data, data + 6, copy_size);
            }
        }
    }
    
    return img;
}

static void fuzz_vpx_img_set_rect(vpx_image_t *img, const uint8_t *data, size_t size) {
    if (!img || size < 4) return;
    
    // Extract rectangle parameters from fuzzer data
    unsigned int x = (data[0] << 8) | data[1];
    unsigned int y = (data[2] << 8) | data[3];
    unsigned int w = 0, h = 0;
    
    if (size >= 8) {
        w = (data[4] << 8) | data[5];
        h = (data[6] << 8) | data[7];
    } else {
        // Use some reasonable defaults if not enough data
        w = img->w / 2;
        h = img->h / 2;
    }
    
    // Call the target function
    vpx_img_set_rect(img, x, y, w, h);
}

static void fuzz_vpx_codec_encode(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    // Initialize encoder context
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    // Get default configuration for VP8
    vpx_codec_err_t res = vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0);
    if (res != VPX_CODEC_OK) return;
    
    // Set some parameters from fuzzer data
    cfg.g_w = (data[0] << 8) | data[1];
    cfg.g_h = (data[2] << 8) | data[3];
    if (cfg.g_w == 0) cfg.g_w = 16;
    if (cfg.g_h == 0) cfg.g_h = 16;
    if (cfg.g_w > 4096) cfg.g_w = 4096;
    if (cfg.g_h > 4096) cfg.g_h = 4096;
    
    // Initialize encoder
    res = vpx_codec_enc_init(&codec, vpx_codec_vp8_cx(), &cfg, 0);
    if (res != VPX_CODEC_OK) return;
    
    // Create an image for encoding
    vpx_image_t *img = create_fuzzed_image(data + 4, size - 4, 0);
    if (img) {
        // Extract encoding parameters from fuzzer data
        vpx_codec_pts_t pts = (int64_t)((data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7]);
        unsigned long duration = (data[8] << 24) | (data[9] << 16) | (data[10] << 8) | data[11];
        vpx_enc_frame_flags_t flags = (data[12] % 4); // Limited to valid flag values
        vpx_enc_deadline_t deadline = (data[13] % 4) * 1000; // 0, 1000, 2000, 3000
        
        // Encode the image
        vpx_codec_encode(&codec, img, pts, duration, flags, deadline);
        
        // Also test flushing by encoding with NULL image
        vpx_codec_encode(&codec, NULL, pts + 1, duration, flags, deadline);
        
        vpx_img_free(img);
    } else {
        // Test encoding with NULL image (flushing)
        vpx_codec_pts_t pts = 0;
        unsigned long duration = 1;
        vpx_codec_encode(&codec, NULL, pts, duration, 0, 0);
    }
    
    // Destroy the encoder
    vpx_codec_destroy(&codec);
}

int LLVMFuzzerTestOneInput_20(const uint8_t *data, size_t size) {
    if (size < 2) return 0;
    
    // Use first byte to determine which functions to test
    uint8_t test_case = data[0];
    
    // Test vpx_img_alloc and related functions
    if (test_case % 2 == 0) {
        // Test with vpx_img_alloc
        vpx_image_t *img1 = create_fuzzed_image(data + 1, size - 1, 0);
        if (img1) {
            // Test vpx_img_set_rect
            fuzz_vpx_img_set_rect(img1, data + 1, size - 1);
            
            // Test vpx_img_flip
            vpx_img_flip(img1);
            
            // Test vpx_img_set_rect again after flip
            fuzz_vpx_img_set_rect(img1, data + 1, size - 1);
            
            vpx_img_free(img1);
        }
    }
    
    // Test vpx_img_wrap and related functions
    if (test_case % 3 == 0) {
        // Test with vpx_img_wrap
        vpx_image_t *img2 = create_fuzzed_image(data + 1, size - 1, 1);
        if (img2) {
            // Test vpx_img_set_rect
            fuzz_vpx_img_set_rect(img2, data + 1, size - 1);
            
            // Test vpx_img_flip
            vpx_img_flip(img2);
            
            vpx_img_free(img2);
        }
    }
    
    // Test vpx_codec_encode
    if (test_case % 5 == 0) {
        fuzz_vpx_codec_encode(data + 1, size - 1);
    }
    
    return 0;
}