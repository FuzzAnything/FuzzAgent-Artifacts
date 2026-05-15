// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"

static aom_image_t *create_test_image(const uint8_t *data, size_t size) {
    if (size < 4) return NULL;
    
    // Extract parameters from fuzzer data
    unsigned int width = (data[0] << 8) | data[1];
    unsigned int height = (data[2] << 8) | data[3];
    unsigned int align = (data[0] & 0x3F) + 1;  // 1-64 alignment
    
    // Limit dimensions to reasonable values for fuzzing
    width = width % 1024 + 1;
    height = height % 1024 + 1;
    align = align % 64 + 1;
    
    // Select a format based on fuzzer data
    aom_img_fmt_t formats[] = {
        AOM_IMG_FMT_YV12,
        AOM_IMG_FMT_I420,
        AOM_IMG_FMT_AOMYV12,
        AOM_IMG_FMT_AOMI420,
        AOM_IMG_FMT_I422,
        AOM_IMG_FMT_I444,
        AOM_IMG_FMT_NV12,
        AOM_IMG_FMT_I42016,
        AOM_IMG_FMT_YV1216,
        AOM_IMG_FMT_I42216,
        AOM_IMG_FMT_I44416
    };
    int fmt_index = data[0] % (sizeof(formats) / sizeof(formats[0]));
    
    return aom_img_alloc(NULL, formats[fmt_index], width, height, align);
}

static void test_image_functions(aom_image_t *img, const uint8_t *data, size_t size) {
    if (!img) return;
    
    // Test aom_img_plane_width and aom_img_plane_height
    for (int plane = 0; plane < 3; plane++) {
        int width = aom_img_plane_width(img, plane);
        int height = aom_img_plane_height(img, plane);
        (void)width;
        (void)height;
    }
    
    // Test aom_img_flip
    aom_img_flip(img);
    
    // Test aom_img_set_rect if we have enough data
    if (size >= 20) {
        unsigned int x = (data[4] << 8) | data[5];
        unsigned int y = (data[6] << 8) | data[7];
        unsigned int w = (data[8] << 8) | data[9];
        unsigned int h = (data[10] << 8) | data[11];
        unsigned int border = data[12] % 32;
        
        // Ensure coordinates are within bounds
        x = x % (img->w + 1);
        y = y % (img->h + 1);
        w = w % (img->w - x + 1);
        h = h % (img->h - y + 1);
        
        aom_img_set_rect(img, x, y, w, h, border);
    }
}

static void test_decoder_functions(const uint8_t *data, size_t size) {
    // Create a dummy decoder context
    aom_codec_ctx_t ctx = {0};
    aom_codec_iter_t iter = NULL;
    
    // Test aom_codec_get_frame with NULL context (should handle gracefully)
    aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame;
    
    // Test with valid-looking iterator
    iter = data;  // Cast fuzzer data as iterator
    frame = aom_codec_get_frame(&ctx, (aom_codec_iter_t*)&iter);
    (void)frame;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test image functions
    aom_image_t *img = create_test_image(Data, Size);
    if (img) {
        test_image_functions(img, Data, Size);
        aom_img_free(img);
    }
    
    // Test decoder functions
    test_decoder_functions(Data, Size);
    
    return 0;
}