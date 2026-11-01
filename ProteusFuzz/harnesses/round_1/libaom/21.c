// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

/* Helper to extract unsigned integers from fuzz data safely */
static unsigned int get_uint(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(unsigned int) > size) {
        *offset = 0;
        return 0;
    }
    unsigned int val = 0;
    memcpy(&val, data + *offset, sizeof(unsigned int));
    *offset += sizeof(unsigned int);
    return val;
}

/* Helper to get a small integer for enums/flags */
static unsigned int get_small_uint(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(unsigned int) > size) {
        *offset = 0;
        return 0;
    }
    unsigned int val = 0;
    memcpy(&val, data + *offset, sizeof(unsigned int));
    *offset += sizeof(unsigned int);
    return val;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    size_t offset = 0;
    
    /* Extract parameters from fuzz input */
    unsigned int fmt_raw = get_small_uint(Data, Size, &offset);
    unsigned int w = get_uint(Data, Size, &offset);
    unsigned int h = get_uint(Data, Size, &offset);
    unsigned int align = get_uint(Data, Size, &offset);
    unsigned int size_align = get_uint(Data, Size, &offset);
    unsigned int border = get_uint(Data, Size, &offset);

    /* Constrain parameters to valid ranges as per API requirements */
    /* Max dimension 2^27 */
    if (w > 0x08000000) w = 0x08000000;
    if (h > 0x08000000) h = 0x08000000;
    /* Max alignment 65536 */
    if (align > 65536) align = 65536;
    if (size_align > 65536) size_align = 65536;
    if (border > 65536) border = 65536;
    if (align == 0) align = 1;
    if (size_align == 0) size_align = 1;

    /* Map format to valid enum values */
    aom_img_fmt_t fmt = AOM_IMG_FMT_NONE;
    switch (fmt_raw % 12) {
        case 0: fmt = AOM_IMG_FMT_YV12; break;
        case 1: fmt = AOM_IMG_FMT_I420; break;
        case 2: fmt = AOM_IMG_FMT_AOMYV12; break;
        case 3: fmt = AOM_IMG_FMT_AOMI420; break;
        case 4: fmt = AOM_IMG_FMT_I422; break;
        case 5: fmt = AOM_IMG_FMT_I444; break;
        case 6: fmt = AOM_IMG_FMT_NV12; break;
        case 7: fmt = AOM_IMG_FMT_I42016; break;
        case 8: fmt = AOM_IMG_FMT_YV1216; break;
        case 9: fmt = AOM_IMG_FMT_I42216; break;
        case 10: fmt = AOM_IMG_FMT_I44416; break;
        case 11: fmt = AOM_IMG_FMT_NONE; break;
    }

    /* Test Case 1: aom_img_alloc */
    aom_image_t *img_alloc = NULL;
    aom_image_t local_img;
    
    /* Try allocating with NULL img (heap allocation) */
    img_alloc = aom_img_alloc(NULL, fmt, w, h, align);
    if (img_alloc) {
        /* Test aom_img_flip */
        aom_img_flip(img_alloc);
        /* Flip again to restore */
        aom_img_flip(img_alloc);
        
        /* Test aom_img_remove_metadata */
        aom_img_remove_metadata(img_alloc);
        
        /* Free allocated image */
        aom_img_free(img_alloc);
        img_alloc = NULL;
    }

    /* Try allocating with pre-allocated struct */
    memset(&local_img, 0, sizeof(local_img));
    aom_image_t *img_local = aom_img_alloc(&local_img, fmt, w, h, align);
    if (img_local) {
        aom_img_flip(img_local);
        aom_img_flip(img_local);
        aom_img_remove_metadata(img_local);
        /* Do not free here as it might be self-allocated or not depending on implementation logic, 
           but aom_img_free handles self-allocated check. */
        aom_img_free(img_local);
    }

    /* Test Case 2: aom_img_alloc_with_border */
    aom_image_t *img_border = NULL;
    img_border = aom_img_alloc_with_border(NULL, fmt, w, h, align, size_align, border);
    if (img_border) {
        aom_img_flip(img_border);
        aom_img_flip(img_border);
        aom_img_remove_metadata(img_border);
        aom_img_free(img_border);
    }

    /* Test Case 3: aom_img_wrap */
    /* To wrap, we need to allocate a buffer that fits the image requirements.
       We first try to get the required size by wrapping a dummy pointer or calculating.
       Since calculating exact size is complex without internal helpers, we allocate a large buffer.
       Note: The API documentation suggests calling wrap with (unsigned char *)1 first to get info,
       but we will just allocate a sufficiently large buffer based on max dimensions for safety in fuzzing.
    */
    if (Size > 40) {
        offset = 0;
        unsigned int stride_align = get_uint(Data, Size, &offset);
        if (stride_align > 65536) stride_align = 65536;
        if (stride_align == 0) stride_align = 1;

        /* Estimate max buffer size needed: (w + align) * (h + align) * 4 (worst case 16bit planar) */
        /* Use a safe upper bound to avoid overflow in calculation if w/h are maxed */
        size_t max_w = (w > 1024) ? 1024 : w;
        size_t max_h = (h > 1024) ? 1024 : h;
        size_t buffer_size = (max_w + 64) * (max_h + 64) * 4;
        
        if (buffer_size > 0 && buffer_size < (size_t)(1024 * 1024 * 10)) { // Limit allocation size
            unsigned char *img_data = (unsigned char *)malloc(buffer_size);
            if (img_data) {
                aom_image_t img_wrap_local;
                memset(&img_wrap_local, 0, sizeof(img_wrap_local));
                
                aom_image_t *img_wrap = aom_img_wrap(&img_wrap_local, fmt, max_w, max_h, stride_align, img_data);
                if (img_wrap) {
                    aom_img_flip(img_wrap);
                    aom_img_flip(img_wrap);
                    aom_img_remove_metadata(img_wrap);
                    /* aom_img_free does not free img_data if it was passed in, only the struct if self-allocated */
                    aom_img_free(img_wrap);
                }
                free(img_data);
            }
        }
    }

    /* Test Case 4: Edge cases with NULL pointers */
    /* Note: aom_img_flip does not handle NULL safely in some versions, causing SEGV.
       We skip calling aom_img_flip(NULL) to avoid the crash reported by ASan.
       aom_img_free and aom_img_remove_metadata are documented to handle NULL safely. */
    // aom_img_flip(NULL); // Removed to prevent SEGV
    aom_img_free(NULL);
    aom_img_remove_metadata(NULL);
    
    /* Test Case 5: Reuse of freed memory (sanity check, though logic above handles it) */
    aom_image_t *img_test = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 16, 16, 1);
    if (img_test) {
        aom_img_free(img_test);
        /* Ensure no double free or use after free in our logic */
        img_test = NULL;
    }

    return 0;
}