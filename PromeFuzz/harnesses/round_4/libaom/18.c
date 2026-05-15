// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
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

static aom_image_t* create_fuzzed_image(const uint8_t *data, size_t size) {
    if (size < 16) return NULL;
    
    // Extract parameters from fuzzer input
    uint32_t fmt_val = *(const uint32_t*)(data);
    uint32_t width = *(const uint32_t*)(data + 4);
    uint32_t height = *(const uint32_t*)(data + 8);
    uint32_t align = *(const uint32_t*)(data + 12);
    
    // Clamp values to reasonable ranges
    aom_img_fmt_t fmt = (aom_img_fmt_t)(fmt_val % 20); // More than enough for defined formats
    width = width % 1025;  // Reasonable width
    height = height % 1025; // Reasonable height
    align = (align % 64) + 1; // Alignment 1-64
    
    // Allocate image
    return aom_img_alloc(NULL, fmt, width, height, align);
}

static void fuzz_image_functions(aom_image_t *img, const uint8_t *data, size_t size) {
    if (!img) return;
    
    // Extract rectangle parameters if enough data
    if (size >= 36) {
        uint32_t x = *(const uint32_t*)(data + 16);
        uint32_t y = *(const uint32_t*)(data + 20);
        uint32_t w = *(const uint32_t*)(data + 24);
        uint32_t h = *(const uint32_t*)(data + 28);
        uint32_t border = *(const uint32_t*)(data + 32);
        
        // Clamp values
        x = x % (img->w + 1);
        y = y % (img->h + 1);
        w = w % (img->w - x + 1);
        h = h % (img->h - y + 1);
        border = border % 33; // Reasonable border
        
        aom_img_set_rect(img, x, y, w, h, border);
    }
    
    // Flip the image
    aom_img_flip(img);
    
    // Get plane dimensions
    for (int plane = 0; plane < 3; plane++) {
        aom_img_plane_width(img, plane);
        aom_img_plane_height(img, plane);
    }
    
    // Flip back to original state
    aom_img_flip(img);
}

static void fuzz_decoder_functions(const uint8_t *data, size_t size) {
    // Write data to dummy file for decoder
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
    
    // Create a minimal decoder context
    aom_codec_ctx_t ctx = {0};
    aom_codec_iter_t iter = NULL;
    
    // Try to get frames (will likely fail but exercise the API)
    aom_codec_get_frame(&ctx, &iter);
    aom_codec_get_frame(&ctx, &iter);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create and fuzz image functions
    aom_image_t *img = create_fuzzed_image(Data, Size);
    if (img) {
        fuzz_image_functions(img, Data, Size);
        aom_img_free(img);
    }
    
    // Fuzz decoder functions
    fuzz_decoder_functions(Data, Size);
    
    return 0;
}