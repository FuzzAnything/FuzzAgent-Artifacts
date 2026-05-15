// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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

static aom_image_t* create_test_image(const uint8_t *data, size_t size) {
    if (size < 5) return NULL;
    
    // Extract parameters from fuzzer data
    aom_img_fmt_t fmt = (aom_img_fmt_t)(data[0] % 20);  // Limited to valid enum range
    unsigned int d_w = (unsigned int)(data[1] % 256) + 1;
    unsigned int d_h = (unsigned int)(data[2] % 256) + 1;
    unsigned int align = (unsigned int)(data[3] % 32) + 1;
    
    // Create image
    aom_image_t *img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
    if (!img) return NULL;
    
    // Initialize some fields with fuzzer data
    if (size > 10) {
        img->cp = (aom_color_primaries_t)(data[4] % 24);
        img->tc = (aom_transfer_characteristics_t)(data[5] % 20);
        img->mc = (aom_matrix_coefficients_t)(data[6] % 16);
        img->monochrome = data[7] % 2;
        img->csp = (aom_chroma_sample_position_t)(data[8] % 4);
        img->range = (aom_color_range_t)(data[9] % 2);
        img->bit_depth = (unsigned int)(data[10] % 3) * 4 + 8;  // 8, 12, or 16
    }
    
    return img;
}

static void test_image_functions(aom_image_t *img, const uint8_t *data, size_t size) {
    if (!img) return;
    
    // Test aom_img_plane_height for all planes
    for (int plane = 0; plane < 3; plane++) {
        int height = aom_img_plane_height(img, plane);
        (void)height;  // Use result to avoid unused variable warning
    }
    
    // Test aom_img_plane_width for all planes
    for (int plane = 0; plane < 3; plane++) {
        int width = aom_img_plane_width(img, plane);
        (void)width;  // Use result to avoid unused variable warning
    }
    
    // Test aom_img_set_rect if we have enough data
    if (size >= 15) {
        unsigned int x = (unsigned int)data[0] % (img->w + 1);
        unsigned int y = (unsigned int)data[1] % (img->h + 1);
        unsigned int w = (unsigned int)data[2] % (img->w - x + 1);
        unsigned int h = (unsigned int)data[3] % (img->h - y + 1);
        unsigned int border = (unsigned int)data[4] % 32;
        
        int result = aom_img_set_rect(img, x, y, w, h, border);
        (void)result;  // Use result to avoid unused variable warning
    }
    
    // Test aom_img_flip
    aom_img_flip(img);
}

static void test_codec_get_frame(const uint8_t *data, size_t size) {
    // Create a minimal codec context
    aom_codec_ctx_t ctx = {0};
    aom_codec_iter_t iter = NULL;
    
    // Test aom_codec_get_frame - should return NULL since no decode was performed
    aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame;  // Use result to avoid unused variable warning
    
    // Try again with iterator still NULL
    frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame;  // Use result to avoid unused variable warning
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test aom_img_alloc and related image functions
    aom_image_t *img = create_test_image(Data, Size);
    if (img) {
        test_image_functions(img, Data, Size);
        aom_img_free(img);
    }
    
    // Test aom_codec_get_frame
    test_codec_get_frame(Data, Size);
    
    // Test aom_img_alloc with non-NULL img parameter
    if (Size >= 5) {
        aom_image_t local_img;
        aom_img_fmt_t fmt = (aom_img_fmt_t)(Data[0] % 20);
        unsigned int d_w = (unsigned int)(Data[1] % 256) + 1;
        unsigned int d_h = (unsigned int)(Data[2] % 256) + 1;
        unsigned int align = (unsigned int)(Data[3] % 32) + 1;
        
        aom_image_t *result = aom_img_alloc(&local_img, fmt, d_w, d_h, align);
        if (result == &local_img) {
            // Test functions on this image too
            test_image_functions(&local_img, Data, Size);
            aom_img_free(&local_img);
        }
    }
    
    return 0;
}