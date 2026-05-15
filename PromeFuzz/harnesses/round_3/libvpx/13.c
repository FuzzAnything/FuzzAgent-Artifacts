// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static vpx_image_t* create_test_image(vpx_img_fmt_t fmt, unsigned int width, unsigned int height, unsigned int align) {
    // Limit dimensions to reasonable values to avoid excessive memory usage
    if (width > 1024) width = 1024;
    if (height > 1024) height = 1024;
    if (align > 256) align = 256;
    
    return vpx_img_alloc(NULL, fmt, width, height, align);
}

static void fuzz_vpx_img_set_rect(const uint8_t* data, size_t size) {
    if (size < 16) return;
    
    // Parse parameters from fuzzer input
    unsigned int x = ((unsigned int)data[0] << 24) | ((unsigned int)data[1] << 16) | 
                     ((unsigned int)data[2] << 8) | data[3];
    unsigned int y = ((unsigned int)data[4] << 24) | ((unsigned int)data[5] << 16) | 
                     ((unsigned int)data[6] << 8) | data[7];
    unsigned int w = ((unsigned int)data[8] << 24) | ((unsigned int)data[9] << 16) | 
                     ((unsigned int)data[10] << 8) | data[11];
    unsigned int h = ((unsigned int)data[12] << 24) | ((unsigned int)data[13] << 16) | 
                     ((unsigned int)data[14] << 8) | data[15];
    
    // Create test image with reasonable parameters
    vpx_image_t* img = create_test_image(VPX_IMG_FMT_I420, 640, 480, 16);
    if (!img) return;
    
    // Test the target function
    vpx_img_set_rect(img, x, y, w, h);
    
    // Clean up
    vpx_img_free(img);
}

static void fuzz_vpx_img_wrap(const uint8_t* data, size_t size) {
    if (size < 20) return;
    
    // Parse parameters from fuzzer input
    vpx_img_fmt_t fmt = (vpx_img_fmt_t)(data[0] % 16); // Limit to valid format range
    unsigned int d_w = ((unsigned int)data[1] << 8) | data[2];
    unsigned int d_h = ((unsigned int)data[3] << 8) | data[4];
    unsigned int stride_align = data[5] % 256;
    
    // Limit dimensions
    if (d_w > 1024) d_w = 1024;
    if (d_h > 1024) d_h = 1024;
    
    // Calculate approximate buffer size needed
    size_t buffer_size = 0;
    switch (fmt) {
        case VPX_IMG_FMT_I420:
        case VPX_IMG_FMT_YV12:
            buffer_size = d_w * d_h * 3 / 2;
            break;
        case VPX_IMG_FMT_I422:
            buffer_size = d_w * d_h * 2;
            break;
        case VPX_IMG_FMT_I444:
            buffer_size = d_w * d_h * 3;
            break;
        default:
            buffer_size = d_w * d_h * 4; // Conservative estimate
    }
    
    // Allocate buffer
    unsigned char* img_data = (unsigned char*)malloc(buffer_size);
    if (!img_data) return;
    
    // Fill buffer with fuzzer data (cyclic)
    for (size_t i = 0; i < buffer_size; i++) {
        img_data[i] = data[i % size];
    }
    
    // Test the target function
    vpx_image_t* img = vpx_img_wrap(NULL, fmt, d_w, d_h, stride_align, img_data);
    
    if (img) {
        // Test other operations on the wrapped image
        vpx_img_flip(img);
        vpx_img_set_rect(img, 0, 0, d_w/2, d_h/2);
        vpx_img_free(img);
    }
    
    free(img_data);
}

static void fuzz_vpx_codec_encode(const uint8_t* data, size_t size) {
    if (size < 32) return;
    
    // Parse parameters from fuzzer input
    vpx_codec_pts_t pts = ((int64_t)data[0] << 56) | ((int64_t)data[1] << 48) | 
                          ((int64_t)data[2] << 40) | ((int64_t)data[3] << 32) |
                          ((int64_t)data[4] << 24) | ((int64_t)data[5] << 16) | 
                          ((int64_t)data[6] << 8) | data[7];
    unsigned long duration = ((unsigned long)data[8] << 24) | ((unsigned long)data[9] << 16) | 
                             ((unsigned long)data[10] << 8) | data[11];
    vpx_enc_frame_flags_t flags = (vpx_enc_frame_flags_t)(data[12] % 256);
    vpx_enc_deadline_t deadline = ((unsigned long)data[13] << 24) | ((unsigned long)data[14] << 16) | 
                                  ((unsigned long)data[15] << 8) | data[16];
    
    // Initialize codec context
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Create test image
    vpx_image_t* img = create_test_image(VPX_IMG_FMT_I420, 320, 240, 16);
    if (!img) return;
    
    // Fill image data with fuzzer input
    for (int plane = 0; plane < 3; plane++) {
        if (img->planes[plane]) {
            size_t plane_size = img->stride[plane] * (img->h >> (plane ? 1 : 0));
            for (size_t i = 0; i < plane_size && i < size; i++) {
                img->planes[plane][i] = data[(i + plane * 100) % size];
            }
        }
    }
    
    // Test encoding with image
    vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
    
    // Test flushing (NULL image)
    vpx_codec_encode(&ctx, NULL, pts + 1, duration, flags, deadline);
    
    // Clean up
    vpx_img_free(img);
}

static void fuzz_vpx_img_alloc(const uint8_t* data, size_t size) {
    if (size < 8) return;
    
    // Parse parameters from fuzzer input
    vpx_img_fmt_t fmt = (vpx_img_fmt_t)(data[0] % 16); // Limit to valid format range
    unsigned int d_w = ((unsigned int)data[1] << 8) | data[2];
    unsigned int d_h = ((unsigned int)data[3] << 8) | data[4];
    unsigned int align = data[5] % 256;
    
    // Limit dimensions
    if (d_w > 1024) d_w = 1024;
    if (d_h > 1024) d_h = 1024;
    if (align > 256) align = 256;
    
    // Test the target function
    vpx_image_t* img = vpx_img_alloc(NULL, fmt, d_w, d_h, align);
    
    if (img) {
        // Test other operations on the allocated image
        vpx_img_flip(img);
        vpx_img_set_rect(img, d_w/4, d_h/4, d_w/2, d_h/2);
        vpx_img_free(img);
    }
}

static void fuzz_vpx_img_flip(const uint8_t* data, size_t size) {
    // Create test image
    vpx_image_t* img = create_test_image(VPX_IMG_FMT_I420, 640, 480, 16);
    if (!img) return;
    
    // Fill image data with fuzzer input
    for (int plane = 0; plane < 3; plane++) {
        if (img->planes[plane]) {
            size_t plane_size = img->stride[plane] * (img->h >> (plane ? 1 : 0));
            for (size_t i = 0; i < plane_size && i < size; i++) {
                img->planes[plane][i] = data[i % size];
            }
        }
    }
    
    // Test the target function
    vpx_img_flip(img);
    
    // Test flipping multiple times
    vpx_img_flip(img);
    vpx_img_flip(img);
    
    // Clean up
    vpx_img_free(img);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Use first byte to determine which function to test
    uint8_t selector = data[0];
    
    switch (selector % 6) {
        case 0:
            fuzz_vpx_img_set_rect(data + 1, size - 1);
            break;
        case 1:
            fuzz_vpx_img_wrap(data + 1, size - 1);
            break;
        case 2:
            fuzz_vpx_codec_encode(data + 1, size - 1);
            break;
        case 3:
            fuzz_vpx_img_alloc(data + 1, size - 1);
            break;
        case 4:
            fuzz_vpx_img_flip(data + 1, size - 1);
            break;
        case 5:
            // Test multiple functions in sequence
            if (size > 50) {
                fuzz_vpx_img_alloc(data, size / 3);
                fuzz_vpx_img_set_rect(data + size / 3, size / 3);
                fuzz_vpx_img_flip(data + 2 * size / 3, size / 3);
            }
            break;
    }
    
    return 0;
}