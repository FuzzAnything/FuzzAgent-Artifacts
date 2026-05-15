// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include "aom/aom_decoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static aom_image_t *create_test_image(unsigned int width, unsigned int height, 
                                       aom_img_fmt_t fmt, unsigned int align) {
    if (width == 0 || height == 0 || width > 0x08000000 || height > 0x08000000 || align > 65536) {
        return NULL;
    }
    
    // Create image on heap
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, align);
    if (!img) {
        return NULL;
    }
    
    // Fill image data with some pattern to make it non-empty
    for (int plane = 0; plane < 3; plane++) {
        if (img->planes[plane]) {
            int plane_h = aom_img_plane_height(img, plane);
            int plane_w = aom_img_plane_width(img, plane);
            int stride = img->stride[plane];
            
            for (int y = 0; y < plane_h; y++) {
                memset(img->planes[plane] + y * stride, (plane + y) & 0xFF, plane_w);
            }
        }
    }
    
    return img;
}

static void test_image_functions(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    // Extract parameters from fuzzer input
    unsigned int width = (data[0] << 8) | data[1];
    unsigned int height = (data[2] << 8) | data[3];
    aom_img_fmt_t fmt = (aom_img_fmt_t)(data[4] % 8);  // Limit to valid formats
    unsigned int align = 1 << (data[5] % 5);  // Alignment as power of 2
    
    // Create test image
    aom_image_t *img = create_test_image(width, height, fmt, align);
    if (!img) return;
    
    // Test aom_img_plane_height and aom_img_plane_width
    for (int plane = 0; plane < 3; plane++) {
        (void)aom_img_plane_height(img, plane);
        (void)aom_img_plane_width(img, plane);
    }
    
    // Test aom_img_set_rect if we have enough data
    if (size >= 26) {
        unsigned int x = (data[6] << 8) | data[7];
        unsigned int y = (data[8] << 8) | data[9];
        unsigned int w = (data[10] << 8) | data[11];
        unsigned int h = (data[12] << 8) | data[13];
        unsigned int border = data[14];
        
        (void)aom_img_set_rect(img, x, y, w, h, border);
    }
    
    // Test aom_img_flip
    aom_img_flip(img);
    
    // Clean up
    aom_img_free(img);
}

static void test_codec_get_frame(const uint8_t *data, size_t size) {
    // Create a dummy codec context structure
    aom_codec_ctx_t ctx = {0};
    aom_codec_iter_t iter = NULL;
    
    // Initialize minimal context to avoid null pointer dereference
    ctx.name = "dummy";
    ctx.err = AOM_CODEC_OK;
    
    // Call aom_codec_get_frame - will return NULL but should not crash
    aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame;
    
    // Call again with iterator set
    iter = data;  // Use fuzzer data as iterator (won't be valid but shouldn't crash)
    frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *data, size_t size) {
    // Test image-related functions
    test_image_functions(data, size);
    
    // Test codec get frame function
    test_codec_get_frame(data, size);
    
    // Also test aom_img_alloc directly with various parameters
    if (size >= 8) {
        unsigned int width = (data[0] << 8) | data[1];
        unsigned int height = (data[2] << 8) | data[3];
        aom_img_fmt_t fmt = (aom_img_fmt_t)(data[4] % 8);
        unsigned int align = 1 << (data[5] % 5);
        
        // Test allocating on heap
        aom_image_t *img1 = aom_img_alloc(NULL, fmt, width, height, align);
        if (img1) {
            aom_img_free(img1);
        }
        
        // Test allocating with pre-allocated structure
        aom_image_t img2;
        aom_image_t *img2_ptr = aom_img_alloc(&img2, fmt, width, height, align);
        if (img2_ptr == &img2) {
            // The function should return our pointer
            aom_img_free(&img2);
        }
    }
    
    return 0;
}