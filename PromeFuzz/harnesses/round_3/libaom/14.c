// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include "aom/aom_decoder.h"

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Use first few bytes to seed parameters for image creation
    if (Size < 10) return 0;
    
    // Extract parameters from fuzzer input
    unsigned int d_w = (Data[0] << 8) | Data[1];
    unsigned int d_h = (Data[2] << 8) | Data[3];
    unsigned int align = (Data[4] << 8) | Data[5];
    aom_img_fmt_t fmt = (aom_img_fmt_t)(Data[6] % 19); // 19 formats in enum
    int plane = Data[7] % 3; // 3 planes max
    unsigned int x = (Data[8] << 8) | Data[9];
    unsigned int y = (Data[10] << 8) | Data[11];
    unsigned int w = (Data[12] << 8) | Data[13];
    unsigned int h = (Data[14] << 8) | Data[15];
    unsigned int border = (Data[16] << 8) | Data[17];
    
    // Limit dimensions to reasonable values to avoid excessive memory allocation
    d_w = d_w % 1024 + 1;
    d_h = d_h % 1024 + 1;
    align = align % 256 + 1;
    x = x % d_w;
    y = y % d_h;
    w = w % (d_w - x + 1) + 1;
    h = h % (d_h - y + 1) + 1;
    border = border % 32;
    
    // Test aom_img_alloc
    aom_image_t *img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
    if (!img) return 0;
    
    // Test aom_img_plane_width
    int plane_width = aom_img_plane_width(img, plane);
    (void)plane_width; // Use result to avoid compiler warnings
    
    // Test aom_img_plane_height
    int plane_height = aom_img_plane_height(img, plane);
    (void)plane_height;
    
    // Test aom_img_set_rect
    int set_rect_result = aom_img_set_rect(img, x, y, w, h, border);
    (void)set_rect_result;
    
    // Test aom_img_flip
    aom_img_flip(img);
    
    // Test aom_codec_get_frame with dummy context
    // Create a minimal codec context structure
    aom_codec_ctx_t ctx = {0};
    aom_codec_iter_t iter = NULL;
    
    // This will likely return NULL since we don't have valid decoded data,
    // but we're testing the function call itself
    aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame;
    
    // Cleanup
    aom_img_free(img);
    
    return 0;
}