// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
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

static uint32_t read_uint32(const uint8_t **data, size_t *size) {
    if (*size < 4) {
        *size = 0;
        return 0;
    }
    uint32_t val = 0;
    for (int i = 0; i < 4; i++) {
        val = (val << 8) | (*data)[i];
    }
    *data += 4;
    *size -= 4;
    return val;
}

static uint8_t read_uint8(const uint8_t **data, size_t *size) {
    if (*size < 1) {
        *size = 0;
        return 0;
    }
    uint8_t val = (*data)[0];
    *data += 1;
    *size -= 1;
    return val;
}

static aom_img_fmt_t get_random_format(uint8_t fmt_byte) {
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
    return formats[fmt_byte % (sizeof(formats)/sizeof(formats[0]))];
}

int LLVMFuzzerTestOneInput_9(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    const uint8_t *original_data = data;
    size_t original_size = size;
    
    // Test aom_codec_get_frame with a dummy context
    {
        aom_codec_ctx_t ctx = {0};
        aom_codec_iter_t iter = NULL;
        aom_image_t *img = aom_codec_get_frame(&ctx, &iter);
        // Just call it, don't use the result since context is invalid
    }
    
    // Test aom_img_alloc
    {
        uint32_t d_w = read_uint32(&data, &size) % 1024 + 1;
        uint32_t d_h = read_uint32(&data, &size) % 1024 + 1;
        uint32_t align = read_uint32(&data, &size) % 256 + 1;
        aom_img_fmt_t fmt = get_random_format(read_uint8(&data, &size));
        
        aom_image_t *img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
        if (img) {
            aom_img_flip(img);
            aom_img_free(img);
        }
    }
    
    // Test aom_img_alloc_with_border
    {
        uint32_t d_w = read_uint32(&data, &size) % 1024 + 1;
        uint32_t d_h = read_uint32(&data, &size) % 1024 + 1;
        uint32_t align = read_uint32(&data, &size) % 256 + 1;
        uint32_t size_align = read_uint32(&data, &size) % 256 + 1;
        uint32_t border = read_uint32(&data, &size) % 256;
        aom_img_fmt_t fmt = get_random_format(read_uint8(&data, &size));
        
        aom_image_t *img = aom_img_alloc_with_border(NULL, fmt, d_w, d_h, align, size_align, border);
        if (img) {
            aom_img_flip(img);
            aom_img_free(img);
        }
    }
    
    // Test aom_img_wrap
    {
        uint32_t d_w = read_uint32(&data, &size) % 1024 + 1;
        uint32_t d_h = read_uint32(&data, &size) % 1024 + 1;
        uint32_t stride_align = read_uint32(&data, &size) % 256 + 1;
        aom_img_fmt_t fmt = get_random_format(read_uint8(&data, &size));
        
        // Calculate approximate buffer size needed
        size_t buffer_size = 0;
        int bps = 8;
        if (fmt & AOM_IMG_FMT_HIGHBITDEPTH) bps = 16;
        
        unsigned int x_chroma_shift = 0;
        unsigned int y_chroma_shift = 0;
        
        // Get chroma shifts based on format
        switch (fmt & ~(AOM_IMG_FMT_PLANAR | AOM_IMG_FMT_UV_FLIP | AOM_IMG_FMT_HIGHBITDEPTH)) {
            case 1: // YV12
            case 2: // I420
            case 3: // AOMYV12
            case 4: // AOMI420
            case 7: // NV12
                x_chroma_shift = 1;
                y_chroma_shift = 1;
                break;
            case 5: // I422
                x_chroma_shift = 1;
                y_chroma_shift = 0;
                break;
            case 6: // I444
                x_chroma_shift = 0;
                y_chroma_shift = 0;
                break;
        }
        
        unsigned int align_x = (1 << x_chroma_shift) - 1;
        unsigned int w = (d_w + align_x) & ~align_x;
        unsigned int align_y = (1 << y_chroma_shift) - 1;
        unsigned int h = (d_h + align_y) & ~align_y;
        
        uint64_t s = (fmt & AOM_IMG_FMT_PLANAR) ? w : (uint64_t)bps * w / 8;
        s = (fmt & AOM_IMG_FMT_HIGHBITDEPTH) ? s * 2 : s;
        s = (s + stride_align - 1) & ~((uint64_t)stride_align - 1);
        s = (fmt & AOM_IMG_FMT_HIGHBITDEPTH) ? s / 2 : s;
        
        if (fmt & AOM_IMG_FMT_PLANAR) {
            buffer_size = (uint64_t)h * s * bps / 8;
        } else {
            buffer_size = (uint64_t)h * s;
        }
        
        // Add some extra space for safety
        buffer_size += 1024;
        
        if (buffer_size > 0 && buffer_size < 100 * 1024 * 1024) { // Limit to 100MB
            unsigned char *img_data = (unsigned char *)malloc(buffer_size);
            if (img_data) {
                // Fill with some data from fuzzer input
                size_t copy_size = size < buffer_size ? size : buffer_size;
                if (copy_size > 0) {
                    memcpy(img_data, data, copy_size);
                }
                
                aom_image_t *img = aom_img_wrap(NULL, fmt, d_w, d_h, stride_align, img_data);
                if (img) {
                    aom_img_flip(img);
                    aom_img_free(img);
                }
                free(img_data);
            }
        }
    }
    
    // Test aom_img_flip with various image allocations
    {
        // Small image allocation test
        aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 16);
        if (img) {
            aom_img_flip(img);
            aom_img_free(img);
        }
        
        // Test with border
        img = aom_img_alloc_with_border(NULL, AOM_IMG_FMT_I420, 128, 128, 16, 16, 8);
        if (img) {
            aom_img_flip(img);
            aom_img_free(img);
        }
    }
    
    // Test aom_img_free with NULL
    aom_img_free(NULL);
    
    return 0;
}