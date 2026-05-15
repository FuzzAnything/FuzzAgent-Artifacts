// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include "aom/aom_decoder.h"

static aom_img_fmt_t get_format_from_byte(uint8_t byte) {
    switch (byte % 12) {
        case 0: return AOM_IMG_FMT_YV12;
        case 1: return AOM_IMG_FMT_I420;
        case 2: return AOM_IMG_FMT_AOMYV12;
        case 3: return AOM_IMG_FMT_AOMI420;
        case 4: return AOM_IMG_FMT_I422;
        case 5: return AOM_IMG_FMT_I444;
        case 6: return AOM_IMG_FMT_NV12;
        case 7: return AOM_IMG_FMT_I42016;
        case 8: return AOM_IMG_FMT_YV1216;
        case 9: return AOM_IMG_FMT_I42216;
        case 10: return AOM_IMG_FMT_I44416;
        default: return AOM_IMG_FMT_I420;
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;

    // Extract parameters from fuzzer input
    unsigned int width = ((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]) % 1024 + 1;
    unsigned int height = ((Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7]) % 1024 + 1;
    aom_img_fmt_t fmt = get_format_from_byte(Data[8]);
    unsigned int align = (Data[9] % 256) + 1;
    int plane = Data[10] % 3;
    unsigned int border = Data[11] % 32;

    // Test aom_img_alloc
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, align);
    if (!img) return 0;

    // Test aom_img_plane_width
    int plane_width = aom_img_plane_width(img, plane);

    // Test aom_img_plane_height
    int plane_height = aom_img_plane_height(img, plane);

    // Test aom_img_flip
    aom_img_flip(img);

    // Test aom_img_set_rect with various rectangle parameters
    // Avoid division by zero by ensuring width/2 is at least 1
    unsigned int half_width = width / 2;
    if (half_width == 0) half_width = 1;
    
    unsigned int half_height = height / 2;
    if (half_height == 0) half_height = 1;
    
    unsigned int rect_x = (Data[0] % half_width) & ~1;  // Ensure even for chroma
    unsigned int rect_y = (Data[1] % half_height) & ~1;
    
    // Ensure rect_x and rect_y don't exceed image bounds
    if (rect_x >= width) rect_x = width > 2 ? width - 2 : 0;
    if (rect_y >= height) rect_y = height > 2 ? height - 2 : 0;
    
    // Calculate maximum width and height for rectangle
    unsigned int max_w = width - rect_x;
    unsigned int max_h = height - rect_y;
    
    // Ensure max_w and max_h are at least 1
    if (max_w == 0) max_w = 1;
    if (max_h == 0) max_h = 1;
    
    unsigned int rect_w = (Data[2] % max_w) + 1;
    unsigned int rect_h = (Data[3] % max_h) + 1;
    
    // Ensure rect_w and rect_h are even for 4:2:0 formats
    if (fmt == AOM_IMG_FMT_I420 || fmt == AOM_IMG_FMT_YV12 || 
        fmt == AOM_IMG_FMT_AOMI420 || fmt == AOM_IMG_FMT_AOMYV12) {
        rect_w &= ~1;
        rect_h &= ~1;
        if (rect_w == 0) rect_w = 2;
        if (rect_h == 0) rect_h = 2;
    }
    
    int set_rect_result = aom_img_set_rect(img, rect_x, rect_y, rect_w, rect_h, border);

    // Test aom_codec_get_frame with a dummy context
    aom_codec_ctx_t ctx = {0};
    aom_codec_iter_t iter = NULL;
    aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame;  // Suppress unused variable warning

    // Clean up
    aom_img_free(img);

    return 0;
}