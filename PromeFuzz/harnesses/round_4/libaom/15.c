// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_aom_img_flip(const uint8_t *data, size_t size) {
    if (size < sizeof(aom_image_t)) return;
    
    aom_image_t img;
    memcpy(&img, data, sizeof(aom_image_t));
    
    // Set some reasonable values to avoid undefined behavior
    img.w = img.w & 0x0FFFFFFF;  // Limit to reasonable size
    img.h = img.h & 0x0FFFFFFF;
    img.d_w = img.d_w & 0x0FFFFFFF;
    img.d_h = img.d_h & 0x0FFFFFFF;
    img.r_w = img.r_w & 0x0FFFFFFF;
    img.r_h = img.r_h & 0x0FFFFFFF;
    
    // Ensure planes are valid or NULL
    for (int i = 0; i < 3; i++) {
        if (img.planes[i] != NULL) {
            img.planes[i] = (unsigned char *)&img;  // Point to safe memory
        }
    }
    
    aom_img_flip(&img);
}

static void fuzz_aom_img_alloc(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    // Extract parameters from fuzzer input
    aom_img_fmt_t fmt = (aom_img_fmt_t)(data[0] % 16);  // Limited to valid enum range
    unsigned int d_w = (data[1] << 8) | data[2];
    unsigned int d_h = (data[3] << 8) | data[4 % size];
    unsigned int align = 1 << (data[5 % size] % 8);  // Power of 2 alignment
    
    // Limit dimensions to safe values
    d_w = d_w & 0x0FFFFFFF;
    d_h = d_h & 0x0FFFFFFF;
    if (align == 0) align = 1;
    align = align & 0xFFFF;  // Limit to max 65536
    
    aom_image_t *img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
    if (img) {
        aom_img_free(img);
    }
}

static void fuzz_aom_img_wrap(const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    // Extract parameters from fuzzer input
    aom_img_fmt_t fmt = (aom_img_fmt_t)(data[0] % 16);
    unsigned int d_w = (data[1] << 8) | data[2];
    unsigned int d_h = (data[3] << 8) | data[4 % size];
    unsigned int stride_align = 1 << (data[5 % size] % 8);
    
    // Limit dimensions
    d_w = d_w & 0x0FFFFFFF;
    d_h = d_h & 0x0FFFFFFF;
    if (stride_align == 0) stride_align = 1;
    stride_align = stride_align & 0xFFFF;
    
    // Create a dummy buffer for wrapping
    size_t buffer_size = d_w * d_h * 4;  // Conservative estimate
    if (buffer_size > 1024 * 1024) buffer_size = 1024 * 1024;  // Limit to 1MB
    
    unsigned char *img_data = (unsigned char *)malloc(buffer_size);
    if (!img_data) return;
    
    // Fill with fuzzer data
    size_t copy_size = size < buffer_size ? size : buffer_size;
    memcpy(img_data, data, copy_size);
    
    aom_image_t *img = aom_img_wrap(NULL, fmt, d_w, d_h, stride_align, img_data);
    if (img) {
        aom_img_free(img);
    }
    free(img_data);
}

static void fuzz_aom_img_alloc_with_border(const uint8_t *data, size_t size) {
    if (size < 6) return;
    
    aom_img_fmt_t fmt = (aom_img_fmt_t)(data[0] % 16);
    unsigned int d_w = (data[1] << 8) | data[2];
    unsigned int d_h = (data[3] << 8) | data[4 % size];
    unsigned int align = 1 << (data[5 % size] % 8);
    unsigned int size_align = 1 << (data[6 % size] % 8);
    unsigned int border = data[7 % size];
    
    // Limit values
    d_w = d_w & 0x0FFFFFFF;
    d_h = d_h & 0x0FFFFFFF;
    if (align == 0) align = 1;
    align = align & 0xFFFF;
    if (size_align == 0) size_align = 1;
    size_align = size_align & 0xFFFF;
    border = border & 0xFFFF;
    
    aom_image_t *img = aom_img_alloc_with_border(NULL, fmt, d_w, d_h, align, size_align, border);
    if (img) {
        aom_img_free(img);
    }
}

static void fuzz_aom_img_set_rect(const uint8_t *data, size_t size) {
    if (size < sizeof(aom_image_t) + 5) return;
    
    // First create an image
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int d_w = 64;
    unsigned int d_h = 64;
    unsigned int align = 16;
    
    aom_image_t *img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
    if (!img) return;
    
    // Extract rectangle parameters from fuzzer input
    unsigned int x = data[0] % (d_w + 1);
    unsigned int y = data[1] % (d_h + 1);
    unsigned int w = data[2] % (d_w - x + 1);
    unsigned int h = data[3] % (d_h - y + 1);
    unsigned int border = data[4] % 32;
    
    if (w == 0) w = 1;
    if (h == 0) h = 1;
    
    aom_img_set_rect(img, x, y, w, h, border);
    aom_img_free(img);
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to select which function to test
    uint8_t selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size > 1 ? Size - 1 : 0;
    
    switch (selector % 5) {
        case 0:
            fuzz_aom_img_flip(remaining_data, remaining_size);
            break;
        case 1:
            fuzz_aom_img_alloc(remaining_data, remaining_size);
            break;
        case 2:
            fuzz_aom_img_wrap(remaining_data, remaining_size);
            break;
        case 3:
            fuzz_aom_img_alloc_with_border(remaining_data, remaining_size);
            break;
        case 4:
            fuzz_aom_img_set_rect(remaining_data, remaining_size);
            break;
    }
    
    return 0;
}