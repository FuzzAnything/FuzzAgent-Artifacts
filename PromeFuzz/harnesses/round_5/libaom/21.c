// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static aom_img_fmt_t get_format_from_byte(uint8_t byte) {
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
    return formats[byte % (sizeof(formats) / sizeof(formats[0]))];
}

static unsigned int get_limited_value(uint8_t byte, unsigned int max) {
    if (max == 0) return 0;
    return (byte % (max + 1));
}

static unsigned int get_dimension(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 > size) {
        *offset = size;
        return 1;
    }
    unsigned int val = *(unsigned int*)(data + *offset);
    *offset += 4;
    return val % 0x08000000;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    uint8_t mode = Data[offset++];
    
    aom_image_t *img = NULL;
    aom_img_fmt_t fmt = get_format_from_byte(Data[offset++ % Size]);
    
    unsigned int d_w = get_dimension(Data, &offset, Size);
    unsigned int d_h = get_dimension(Data, &offset, Size);
    unsigned int align = get_dimension(Data, &offset, Size);
    align = align % 65536;
    if (align == 0) align = 1;
    
    switch (mode % 4) {
        case 0: {
            img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
            break;
        }
        case 1: {
            unsigned int size_align = get_dimension(Data, &offset, Size);
            unsigned int border = get_dimension(Data, &offset, Size);
            size_align = size_align % 65536;
            border = border % 65536;
            img = aom_img_alloc_with_border(NULL, fmt, d_w, d_h, align, size_align, border);
            break;
        }
        case 2: {
            unsigned int stride_align = get_dimension(Data, &offset, Size);
            stride_align = stride_align % 65536;
            if (stride_align == 0) stride_align = 1;
            
            aom_image_t temp_img;
            aom_img_wrap(&temp_img, fmt, d_w, d_h, stride_align, (unsigned char*)1);
            
            unsigned int align_x = (1 << temp_img.x_chroma_shift) - 1;
            unsigned int align_y = (1 << temp_img.y_chroma_shift) - 1;
            unsigned int w = (d_w + align_x) & ~align_x;
            unsigned int h = (d_h + align_y) & ~align_y;
            
            uint64_t s = (fmt & AOM_IMG_FMT_PLANAR) ? w : (uint64_t)temp_img.bps * w / 8;
            s = (fmt & AOM_IMG_FMT_HIGHBITDEPTH) ? s * 2 : s;
            s = (s + stride_align - 1) & ~((uint64_t)stride_align - 1);
            s = (fmt & AOM_IMG_FMT_HIGHBITDEPTH) ? s / 2 : s;
            
            uint64_t alloc_size = (fmt & AOM_IMG_FMT_PLANAR) ? 
                (uint64_t)h * s * temp_img.bps / 8 : (uint64_t)h * s;
            
            if (alloc_size > 0 && alloc_size < 100 * 1024 * 1024) {
                unsigned char *img_data = (unsigned char*)malloc(alloc_size);
                if (img_data) {
                    size_t copy_size = (Size - offset) < alloc_size ? (Size - offset) : alloc_size;
                    if (copy_size > 0) {
                        memcpy(img_data, Data + offset, copy_size);
                    }
                    img = aom_img_wrap(NULL, fmt, d_w, d_h, stride_align, img_data);
                }
            }
            break;
        }
        case 3: {
            img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
            if (img) {
                aom_img_flip(img);
            }
            break;
        }
    }
    
    if (img) {
        if (mode % 2 == 0) {
            unsigned int x = get_dimension(Data, &offset, Size) % (d_w + 1);
            unsigned int y = get_dimension(Data, &offset, Size) % (d_h + 1);
            unsigned int w = get_dimension(Data, &offset, Size) % (d_w - x + 1);
            unsigned int h = get_dimension(Data, &offset, Size) % (d_h - y + 1);
            unsigned int border = get_dimension(Data, &offset, Size) % 65536;
            aom_img_set_rect(img, x, y, w, h, border);
        }
        
        if (mode % 3 == 0) {
            aom_img_flip(img);
        }
        
        aom_img_free(img);
    }
    
    return 0;
}