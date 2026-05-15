// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
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
#include <stdio.h>

#include "aom_image.h"
#include "aom_decoder.h"

static uint32_t read_uint32(const uint8_t **data, size_t *size) {
    if (*size < 4) {
        *size = 0;
        return 0;
    }
    uint32_t val;
    memcpy(&val, *data, 4);
    *data += 4;
    *size -= 4;
    return val;
}

static uint8_t read_uint8(const uint8_t **data, size_t *size) {
    if (*size < 1) {
        *size = 0;
        return 0;
    }
    uint8_t val = **data;
    *data += 1;
    *size -= 1;
    return val;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *data, size_t size) {
    if (size < 20) return 0;
    
    const uint8_t *ptr = data;
    size_t remaining = size;
    
    // Read parameters for aom_img_alloc
    aom_img_fmt_t fmt = read_uint8(&ptr, &remaining) % 16; // Limited to available formats
    unsigned int d_w = read_uint32(&ptr, &remaining) % 1024 + 1; // Ensure non-zero
    unsigned int d_h = read_uint32(&ptr, &remaining) % 1024 + 1;
    unsigned int align = (read_uint32(&ptr, &remaining) % 256) + 1; // Ensure non-zero
    
    // Allocate image
    aom_image_t *img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
    if (!img) return 0;
    
    // Read parameters for aom_img_set_rect
    unsigned int x = read_uint32(&ptr, &remaining) % (d_w + 1);
    unsigned int y = read_uint32(&ptr, &remaining) % (d_h + 1);
    unsigned int w = read_uint32(&ptr, &remaining) % (d_w - x + 1);
    unsigned int h = read_uint32(&ptr, &remaining) % (d_h - y + 1);
    unsigned int border = read_uint32(&ptr, &remaining) % 32;
    
    // Call aom_img_set_rect
    aom_img_set_rect(img, x, y, w, h, border);
    
    // Call aom_img_plane_height and aom_img_plane_width for each plane
    for (int plane = 0; plane < 3; plane++) {
        if (img->planes[plane]) {
            int height = aom_img_plane_height(img, plane);
            int width = aom_img_plane_width(img, plane);
            (void)height; // Avoid unused variable warning
            (void)width;
        }
    }
    
    // Call aom_img_flip
    aom_img_flip(img);
    
    // Try to create a dummy decoder context for aom_codec_get_frame
    // We'll create a minimal context but won't actually decode
    aom_codec_ctx_t ctx = {0};
    aom_codec_iter_t iter = NULL;
    
    // This should return NULL since we don't have valid decoded data
    aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame; // Avoid unused variable warning
    
    // Clean up
    aom_img_free(img);
    
    return 0;
}