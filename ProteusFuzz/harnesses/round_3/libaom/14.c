// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

static int get_fmt_from_data(const uint8_t *data, size_t size) {
    if (size < 1) return AOM_IMG_FMT_I420;
    uint8_t val = data[0];
    switch (val % 11) {
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

static unsigned int get_val_from_data(const uint8_t *data, size_t size, unsigned int max_val, unsigned int default_val) {
    if (size < 2) return default_val;
    unsigned int val = (data[0] << 8) | data[1];
    if (val == 0 || val > max_val) return default_val;
    return val;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_image_t img;
    aom_image_t *img_ptr = NULL;
    aom_img_fmt_t fmt = get_fmt_from_data(Data, Size);
    
    // Ensure dimensions are within limits (2^27)
    unsigned int d_w = get_val_from_data(Data, Size, 0x08000000, 32);
    unsigned int d_h = get_val_from_data(Data + 2, Size > 2 ? Size - 2 : 0, 0x08000000, 32);
    unsigned int align = get_val_from_data(Data + 4, Size > 4 ? Size - 4 : 0, 65536, 32);
    unsigned int size_align = get_val_from_data(Data + 6, Size > 6 ? Size - 6 : 0, 65536, 16);
    unsigned int border = get_val_from_data(Data + 8, Size > 8 ? Size - 8 : 0, 65536, 4);
    unsigned int stride_align = get_val_from_data(Data + 10, Size > 10 ? Size - 10 : 0, 65536, 32);

    // Test aom_img_alloc_with_border
    // Case 1: Heap allocation (NULL img)
    img_ptr = aom_img_alloc_with_border(NULL, fmt, d_w, d_h, align, size_align, border);
    if (img_ptr) {
        // Test aom_img_flip
        aom_img_flip(img_ptr);
        // Test aom_img_flip again to restore
        aom_img_flip(img_ptr);

        // Test aom_img_set_rect
        unsigned int x = get_val_from_data(Data + 12, Size > 12 ? Size - 12 : 0, d_w, 0);
        unsigned int y = get_val_from_data(Data + 14, Size > 14 ? Size - 14 : 0, d_h, 0);
        unsigned int w = get_val_from_data(Data + 16, Size > 16 ? Size - 16 : 0, d_w - x, 16);
        unsigned int h = get_val_from_data(Data + 18, Size > 18 ? Size - 18 : 0, d_h - y, 16);
        
        // Ensure w and h are valid relative to x, y
        if (x + w > d_w) w = d_w - x;
        if (y + h > d_h) h = d_h - y;
        if (w > 0 && h > 0) {
            aom_img_set_rect(img_ptr, x, y, w, h, border);
        }

        // Test aom_img_remove_metadata
        aom_img_remove_metadata(img_ptr);

        // Free the allocated image
        aom_img_free(img_ptr);
        img_ptr = NULL;
    }

    // Case 2: Reuse pre-allocated struct
    memset(&img, 0, sizeof(img));
    img_ptr = aom_img_alloc_with_border(&img, fmt, d_w, d_h, align, size_align, border);
    if (img_ptr) {
        aom_img_flip(img_ptr);
        aom_img_set_rect(img_ptr, 0, 0, d_w, d_h, border);
        aom_img_remove_metadata(img_ptr);
        aom_img_free(img_ptr);
        img_ptr = NULL;
    }

    // Test aom_img_wrap
    // We need to allocate a buffer large enough for the image
    // First, try to get size requirements by wrapping with a dummy pointer (as per docs)
    // However, aom_img_wrap with dummy pointer doesn't return size directly.
    // We will estimate a safe size based on max dimensions and high bit depth.
    // Max size roughly: 2^27 * 2^27 * 3 bytes (worst case planar high bit depth) is too big.
    // Fuzzing logic: use small dimensions for wrapping to avoid OOM.
    unsigned int wrap_w = (d_w > 256) ? 256 : d_w;
    unsigned int wrap_h = (d_h > 256) ? 256 : d_h;
    
    // Calculate approximate size for I44416 (largest common case)
    // Y: wrap_w * wrap_h * 2
    // U: wrap_w * wrap_h * 2
    // V: wrap_w * wrap_h * 2
    // Stride alignment might add padding.
    size_t estimated_size = (size_t)wrap_w * wrap_h * 2 * 3 + 4096; 
    unsigned char *img_data = (unsigned char *)malloc(estimated_size);
    
    if (img_data) {
        // Initialize data to avoid uninitialized memory usage
        memset(img_data, 0, estimated_size);
        
        // Wrap with NULL img (heap alloc for struct)
        img_ptr = aom_img_wrap(NULL, fmt, wrap_w, wrap_h, stride_align, img_data);
        if (img_ptr) {
            aom_img_flip(img_ptr);
            aom_img_set_rect(img_ptr, 0, 0, wrap_w, wrap_h, border);
            aom_img_remove_metadata(img_ptr);
            aom_img_free(img_ptr);
            img_ptr = NULL;
        }

        // Wrap with pre-allocated struct
        memset(&img, 0, sizeof(img));
        img_ptr = aom_img_wrap(&img, fmt, wrap_w, wrap_h, stride_align, img_data);
        if (img_ptr) {
            aom_img_flip(img_ptr);
            aom_img_set_rect(img_ptr, 0, 0, wrap_w, wrap_h, border);
            aom_img_remove_metadata(img_ptr);
            aom_img_free(img_ptr);
            img_ptr = NULL;
        }
        
        free(img_data);
    }

    // Test edge cases with invalid parameters (should return NULL or handle gracefully)
    // Very large dimensions (should be capped or fail)
    img_ptr = aom_img_alloc_with_border(NULL, fmt, 0x08000000, 0x08000000, 65536, 65536, 65536);
    if (img_ptr) {
        aom_img_free(img_ptr);
    }

    // Invalid format
    img_ptr = aom_img_alloc_with_border(NULL, (aom_img_fmt_t)999, 32, 32, 32, 16, 4);
    if (img_ptr) {
        aom_img_free(img_ptr);
    }

    return 0;
}