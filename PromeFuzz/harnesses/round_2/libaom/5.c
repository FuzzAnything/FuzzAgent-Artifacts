// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "aom/aom_image.h"
#include "aom/aom_decoder.h"

static aom_img_fmt_t get_random_fmt(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset >= size) return AOM_IMG_FMT_I420;
    uint8_t fmt_val = data[*offset] % 15;
    (*offset)++;
    
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
    
    if (fmt_val < sizeof(formats)/sizeof(formats[0])) {
        return formats[fmt_val];
    }
    return AOM_IMG_FMT_I420;
}

static unsigned int get_uint(const uint8_t *data, size_t *offset, size_t size, 
                             unsigned int max_val, unsigned int default_val) {
    if (*offset + 4 > size) return default_val;
    
    unsigned int val = 0;
    for (int i = 0; i < 4; i++) {
        val = (val << 8) | data[*offset];
        (*offset)++;
    }
    
    if (max_val > 0) {
        val = val % (max_val + 1);
    }
    return val;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    // Test aom_img_alloc
    {
        aom_img_fmt_t fmt = get_random_fmt(Data, &offset, Size);
        unsigned int d_w = get_uint(Data, &offset, Size, 0x08000000, 64);
        unsigned int d_h = get_uint(Data, &offset, Size, 0x08000000, 64);
        unsigned int align = get_uint(Data, &offset, Size, 65536, 16);
        
        aom_image_t *img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
        if (img) {
            // Test aom_img_flip on the allocated image
            aom_img_flip(img);
            
            // Test aom_img_free
            aom_img_free(img);
        }
    }
    
    // Test aom_img_alloc_with_border
    {
        aom_img_fmt_t fmt = get_random_fmt(Data, &offset, Size);
        unsigned int d_w = get_uint(Data, &offset, Size, 0x08000000, 128);
        unsigned int d_h = get_uint(Data, &offset, Size, 0x08000000, 128);
        unsigned int align = get_uint(Data, &offset, Size, 65536, 32);
        unsigned int size_align = get_uint(Data, &offset, Size, 65536, 1);
        unsigned int border = get_uint(Data, &offset, Size, 65536, 8);
        
        aom_image_t *img = aom_img_alloc_with_border(NULL, fmt, d_w, d_h, align, size_align, border);
        if (img) {
            aom_img_flip(img);
            aom_img_free(img);
        }
    }
    
    // Test aom_img_wrap
    {
        aom_img_fmt_t fmt = get_random_fmt(Data, &offset, Size);
        unsigned int d_w = get_uint(Data, &offset, Size, 0x08000000, 96);
        unsigned int d_h = get_uint(Data, &offset, Size, 0x08000000, 96);
        unsigned int stride_align = get_uint(Data, &offset, Size, 65536, 64);
        
        // Calculate approximate buffer size needed
        size_t buffer_size = 0;
        if (fmt & AOM_IMG_FMT_HIGHBITDEPTH) {
            buffer_size = d_w * d_h * 2 * 3; // Conservative estimate for planar high bit depth
        } else if (fmt & AOM_IMG_FMT_PLANAR) {
            buffer_size = d_w * d_h * 3; // Conservative estimate for planar formats
        } else {
            buffer_size = d_w * d_h * 4; // Conservative estimate for packed formats
        }
        
        if (buffer_size > 0 && buffer_size < 100 * 1024 * 1024) { // Limit to 100MB
            unsigned char *img_data = (unsigned char *)malloc(buffer_size);
            if (img_data) {
                // Initialize with some data
                for (size_t i = 0; i < buffer_size && i < Size - offset; i++) {
                    img_data[i] = Data[offset + i];
                }
                
                aom_image_t *img = aom_img_wrap(NULL, fmt, d_w, d_h, stride_align, img_data);
                if (img) {
                    aom_img_flip(img);
                    aom_img_free(img);
                    // Free the image data buffer after aom_img_free
                    free(img_data);
                } else {
                    // If aom_img_wrap failed, free the buffer
                    free(img_data);
                }
            }
        }
    }
    
    // Test aom_codec_get_frame with a dummy context
    {
        aom_codec_ctx_t ctx = {0};
        aom_codec_iter_t iter = NULL;
        
        // This should return NULL since we don't have a valid decoder context
        aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
        (void)frame; // Suppress unused variable warning
    }
    
    return 0;
}