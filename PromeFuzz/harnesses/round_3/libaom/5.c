// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
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

static aom_img_fmt_t get_valid_format(uint8_t fmt_byte) {
    const aom_img_fmt_t formats[] = {
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
    return formats[fmt_byte % (sizeof(formats)/sizeof(formats[0]))];
}

static void test_image_functions(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    uint8_t fmt_byte = data[0];
    unsigned int d_w = (data[1] << 8) | data[2];
    unsigned int d_h = (data[3] << 8) | data[4];
    unsigned int align = data[5] ? data[5] : 1;
    int plane = data[6] % 3;
    unsigned int x = (data[7] << 8) | data[8];
    unsigned int y = (data[9] << 8) | data[10];
    unsigned int w = (data[11] << 8) | data[12];
    unsigned int h = (data[13] << 8) | data[14];
    unsigned int border = data[15];
    
    if (d_w > 0x08000000) d_w = 256;
    if (d_h > 0x08000000) d_h = 256;
    if (align > 65536) align = 16;
    
    aom_img_fmt_t fmt = get_valid_format(fmt_byte);
    aom_image_t *img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
    
    if (img) {
        int width = aom_img_plane_width(img, plane);
        int height = aom_img_plane_height(img, plane);
        
        (void)width;
        (void)height;
        
        aom_img_flip(img);
        
        int set_rect_result = aom_img_set_rect(img, x, y, w, h, border);
        (void)set_rect_result;
        
        aom_img_free(img);
    }
}

static void test_codec_get_frame(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx = {0};
    aom_codec_iter_t iter = NULL;
    
    aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame;
    
    if (size > 0) {
        iter = (aom_codec_iter_t)data;
        frame = aom_codec_get_frame(&ctx, &iter);
        (void)frame;
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_image_functions(Data, Size);
    
    if (Size > 1) {
        test_codec_get_frame(Data + 1, Size - 1);
    }
    
    return 0;
}