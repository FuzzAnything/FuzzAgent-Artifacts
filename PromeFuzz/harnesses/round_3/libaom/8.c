// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static aom_img_fmt_t get_random_format(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset >= size) return AOM_IMG_FMT_I420;
    
    static const aom_img_fmt_t formats[] = {
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
    
    uint8_t idx = data[*offset] % (sizeof(formats) / sizeof(formats[0]));
    (*offset)++;
    return formats[idx];
}

static unsigned int get_limited_value(const uint8_t *data, size_t *offset, size_t size, 
                                      unsigned int max_val, unsigned int min_val) {
    if (*offset + 2 > size) return min_val;
    
    unsigned int val = (data[*offset] << 8) | data[*offset + 1];
    (*offset) += 2;
    
    if (val > max_val) val = max_val;
    if (val < min_val) val = min_val;
    return val;
}

static unsigned char *allocate_image_buffer(aom_img_fmt_t fmt, unsigned int d_w, 
                                           unsigned int d_h, unsigned int stride_align,
                                           size_t *alloc_size) {
    int x_chroma_shift = 0;
    int y_chroma_shift = 0;
    int bps = 8;
    
    switch (fmt) {
        case AOM_IMG_FMT_YV12:
        case AOM_IMG_FMT_I420:
        case AOM_IMG_FMT_AOMYV12:
        case AOM_IMG_FMT_AOMI420:
        case AOM_IMG_FMT_NV12:
            x_chroma_shift = 1;
            y_chroma_shift = 1;
            break;
        case AOM_IMG_FMT_I422:
            x_chroma_shift = 1;
            y_chroma_shift = 0;
            break;
        case AOM_IMG_FMT_I444:
            x_chroma_shift = 0;
            y_chroma_shift = 0;
            break;
        case AOM_IMG_FMT_I42016:
        case AOM_IMG_FMT_YV1216:
            x_chroma_shift = 1;
            y_chroma_shift = 1;
            bps = 16;
            break;
        case AOM_IMG_FMT_I42216:
            x_chroma_shift = 1;
            y_chroma_shift = 0;
            bps = 16;
            break;
        case AOM_IMG_FMT_I44416:
            x_chroma_shift = 0;
            y_chroma_shift = 0;
            bps = 16;
            break;
        default:
            x_chroma_shift = 1;
            y_chroma_shift = 1;
    }
    
    unsigned int align_x = (1 << x_chroma_shift) - 1;
    unsigned int w = (d_w + align_x) & ~align_x;
    unsigned int align_y = (1 << y_chroma_shift) - 1;
    unsigned int h = (d_h + align_y) & ~align_y;
    
    size_t s;
    if (fmt & AOM_IMG_FMT_PLANAR) {
        s = w;
    } else {
        s = (size_t)bps * w / 8;
    }
    
    if (fmt & AOM_IMG_FMT_HIGHBITDEPTH) {
        s = s * 2;
    }
    
    if (stride_align > 0) {
        s = (s + stride_align - 1) & ~((size_t)stride_align - 1);
    }
    
    if (fmt & AOM_IMG_FMT_HIGHBITDEPTH) {
        s = s / 2;
    }
    
    if (fmt & AOM_IMG_FMT_PLANAR) {
        *alloc_size = (size_t)h * s * bps / 8;
    } else {
        *alloc_size = (size_t)h * s;
    }
    
    if (*alloc_size == 0) *alloc_size = 1;
    
    return (unsigned char *)malloc(*alloc_size);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    
    size_t offset = 0;
    uint8_t function_selector = data[offset] % 6;
    offset++;
    
    switch (function_selector) {
        case 0: {
            aom_img_fmt_t fmt = get_random_format(data, &offset, size);
            unsigned int d_w = get_limited_value(data, &offset, size, 0x08000000, 1);
            unsigned int d_h = get_limited_value(data, &offset, size, 0x08000000, 1);
            unsigned int align = get_limited_value(data, &offset, size, 65536, 1);
            unsigned int size_align = get_limited_value(data, &offset, size, 65536, 1);
            unsigned int border = get_limited_value(data, &offset, size, 65536, 0);
            
            aom_image_t *img = NULL;
            if (data[offset % size] & 0x01) {
                img = (aom_image_t *)malloc(sizeof(aom_image_t));
                if (img) {
                    aom_img_alloc_with_border(img, fmt, d_w, d_h, align, size_align, border);
                    aom_img_free(img);
                    free(img);
                }
            } else {
                img = aom_img_alloc_with_border(NULL, fmt, d_w, d_h, align, size_align, border);
                if (img) aom_img_free(img);
            }
            break;
        }
        
        case 1: {
            aom_img_fmt_t fmt = get_random_format(data, &offset, size);
            unsigned int d_w = get_limited_value(data, &offset, size, 0x08000000, 1);
            unsigned int d_h = get_limited_value(data, &offset, size, 0x08000000, 1);
            unsigned int align = get_limited_value(data, &offset, size, 65536, 1);
            
            aom_image_t *img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
            if (img) {
                aom_img_flip(img);
                aom_img_free(img);
            }
            break;
        }
        
        case 2: {
            aom_codec_ctx_t ctx = {0};
            aom_codec_iter_t iter = NULL;
            aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
            (void)frame;
            break;
        }
        
        case 3: {
            aom_img_fmt_t fmt = get_random_format(data, &offset, size);
            unsigned int d_w = get_limited_value(data, &offset, size, 0x08000000, 1);
            unsigned int d_h = get_limited_value(data, &offset, size, 0x08000000, 1);
            unsigned int stride_align = get_limited_value(data, &offset, size, 65536, 1);
            
            size_t alloc_size;
            unsigned char *img_data = allocate_image_buffer(fmt, d_w, d_h, stride_align, &alloc_size);
            
            if (img_data) {
                aom_image_t *img = NULL;
                if (data[offset % size] & 0x01) {
                    img = (aom_image_t *)malloc(sizeof(aom_image_t));
                    if (img) {
                        aom_img_wrap(img, fmt, d_w, d_h, stride_align, img_data);
                        aom_img_free(img);
                        free(img);
                    }
                } else {
                    img = aom_img_wrap(NULL, fmt, d_w, d_h, stride_align, img_data);
                    if (img) aom_img_free(img);
                }
                free(img_data);
            }
            break;
        }
        
        case 4: {
            aom_img_fmt_t fmt = get_random_format(data, &offset, size);
            unsigned int d_w = get_limited_value(data, &offset, size, 0x08000000, 1);
            unsigned int d_h = get_limited_value(data, &offset, size, 0x08000000, 1);
            unsigned int align = get_limited_value(data, &offset, size, 65536, 1);
            
            aom_image_t *img = NULL;
            if (data[offset % size] & 0x01) {
                img = (aom_image_t *)malloc(sizeof(aom_image_t));
                if (img) {
                    aom_img_alloc(img, fmt, d_w, d_h, align);
                    aom_img_free(img);
                    free(img);
                }
            } else {
                img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
                if (img) aom_img_free(img);
            }
            break;
        }
        
        case 5: {
            aom_img_free(NULL);
            
            aom_img_fmt_t fmt = get_random_format(data, &offset, size);
            unsigned int d_w = get_limited_value(data, &offset, size, 0x08000000, 1);
            unsigned int d_h = get_limited_value(data, &offset, size, 0x08000000, 1);
            unsigned int align = get_limited_value(data, &offset, size, 65536, 1);
            
            aom_image_t *img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
            if (img) {
                aom_img_free(img);
            }
            break;
        }
    }
    
    return 0;
}