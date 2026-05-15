// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static aom_image_t *create_test_image(const uint8_t *data, size_t size, int use_border) {
    if (size < 4) return NULL;
    
    // Extract parameters from fuzzer input
    aom_img_fmt_t fmt = (aom_img_fmt_t)(data[0] % 20); // Limited to valid enum range
    unsigned int d_w = (data[1] << 8) | data[2];
    unsigned int d_h = (data[3] << 8) | data[4];
    
    // Limit dimensions to reasonable values for fuzzing
    d_w = d_w % 1024 + 1;
    d_h = d_h % 1024 + 1;
    
    unsigned int align = (data[5] % 64) + 1; // Alignment must be at least 1
    unsigned int border = (data[6] % 32); // Border can be 0
    unsigned int size_align = (data[7] % 64) + 1;
    unsigned int stride_align = (data[8] % 64) + 1;
    
    aom_image_t *img = NULL;
    
    if (use_border == 0) {
        img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
    } else if (use_border == 1) {
        img = aom_img_alloc_with_border(NULL, fmt, d_w, d_h, align, size_align, border);
    } else {
        // For wrap, we need to allocate buffer first
        // Calculate approximate buffer size
        size_t buffer_size = d_w * d_h * 4; // Conservative estimate
        if (buffer_size > 1024 * 1024) buffer_size = 1024 * 1024; // Cap size
        
        unsigned char *img_data = (unsigned char*)malloc(buffer_size);
        if (img_data) {
            // Fill with fuzzer data (cyclic)
            for (size_t i = 0; i < buffer_size; i++) {
                img_data[i] = data[i % size];
            }
            img = aom_img_wrap(NULL, fmt, d_w, d_h, stride_align, img_data);
            // Note: img_data must be freed separately if wrap fails
            if (!img) {
                free(img_data);
            }
        }
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    // Test different creation methods based on input
    int method = Data[0] % 4;
    aom_image_t *img = NULL;
    
    switch (method) {
        case 0:
            img = create_test_image(Data, Size, 0); // alloc
            break;
        case 1:
            img = create_test_image(Data, Size, 1); // alloc_with_border
            break;
        case 2:
            img = create_test_image(Data, Size, 2); // wrap
            break;
        case 3:
            // Test with pre-allocated descriptor
            aom_image_t pre_alloc;
            unsigned int d_w = (Data[1] << 8) | Data[2];
            unsigned int d_h = (Data[3] << 8) | Data[4];
            d_w = d_w % 1024 + 1;
            d_h = d_h % 1024 + 1;
            aom_img_fmt_t fmt = (aom_img_fmt_t)(Data[5] % 20);
            unsigned int align = (Data[6] % 64) + 1;
            
            img = aom_img_alloc(&pre_alloc, fmt, d_w, d_h, align);
            break;
    }
    
    if (!img) return 0;
    
    // Test aom_img_flip
    aom_img_flip(img);
    
    // Test aom_img_set_rect with various parameters
    if (Size >= 15) {
        unsigned int x = (Data[9] << 8) | Data[10];
        unsigned int y = (Data[11] << 8) | Data[12];
        unsigned int w = (Data[13] << 8) | Data[14];
        unsigned int h = (Data[15] % 256) + 1;
        unsigned int border = (Data[16] % 32);
        
        // Ensure coordinates are within bounds
        x = x % (img->w > 0 ? img->w : 1);
        y = y % (img->h > 0 ? img->h : 1);
        w = w % ((img->w > x ? img->w - x : 1));
        h = h % ((img->h > y ? img->h - y : 1));
        
        if (w > 0 && h > 0) {
            aom_img_set_rect(img, x, y, w, h, border);
        }
    }
    
    // Test flip again after set_rect
    aom_img_flip(img);
    
    // Cleanup
    aom_img_free(img);
    
    return 0;
}