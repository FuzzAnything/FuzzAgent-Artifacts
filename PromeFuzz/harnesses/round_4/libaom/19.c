// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
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

static aom_image_t *create_test_image(const uint8_t *data, size_t size) {
    if (size < 10) return NULL;
    
    // Extract parameters from fuzzer data
    aom_img_fmt_t fmt = (aom_img_fmt_t)(data[0] % 16);
    unsigned int d_w = (data[1] << 8) | data[2];
    unsigned int d_h = (data[3] << 8) | data[4];
    unsigned int align = 1 << (data[5] % 8); // Power of two alignment
    
    // Limit dimensions to avoid excessive memory allocation
    if (d_w > 4096) d_w = 4096;
    if (d_h > 4096) d_h = 4096;
    if (align > 256) align = 256;
    
    return aom_img_alloc(NULL, fmt, d_w, d_h, align);
}

static void test_image_functions(aom_image_t *img, const uint8_t *data, size_t size) {
    if (!img) return;
    
    // Test aom_img_flip
    aom_img_flip(img);
    
    // Test aom_img_plane_width and aom_img_plane_height for each plane
    for (int plane = 0; plane < 3; plane++) {
        int width = aom_img_plane_width(img, plane);
        int height = aom_img_plane_height(img, plane);
        (void)width; (void)height; // Prevent unused variable warnings
    }
    
    // Test aom_img_set_rect with various parameters
    if (size >= 20) {
        unsigned int x = (data[6] << 8) | data[7];
        unsigned int y = (data[8] << 8) | data[9];
        unsigned int w = (data[10] << 8) | data[11];
        unsigned int h = (data[12] << 8) | data[13];
        unsigned int border = data[14];
        
        // Ensure rectangle is within reasonable bounds
        if (w > 4096) w = 4096;
        if (h > 4096) h = 4096;
        if (border > 100) border = 100;
        
        aom_img_set_rect(img, x, y, w, h, border);
    }
}

static void test_decoder_functions(const uint8_t *data, size_t size) {
    // Create a dummy decoder context
    aom_codec_ctx_t ctx = {0};
    aom_codec_iter_t iter = NULL;
    
    // Test aom_codec_get_frame with NULL context (should return NULL)
    aom_image_t *frame = aom_codec_get_frame(NULL, &iter);
    (void)frame;
    
    // Test with initialized context (even though it's not properly set up)
    frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test image allocation and related functions
    aom_image_t *img = create_test_image(Data, Size);
    if (img) {
        test_image_functions(img, Data, Size);
        aom_img_free(img);
    }
    
    // Test decoder functions
    test_decoder_functions(Data, Size);
    
    return 0;
}