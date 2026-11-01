// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

/* Helper function to safely extract unsigned int from fuzz data */
static unsigned int get_uint(const uint8_t *data, size_t *offset, size_t size, unsigned int max_val) {
    if (*offset + 4 > size) return 0;
    unsigned int val = (unsigned int)data[*offset] | 
                       ((unsigned int)data[*offset + 1] << 8) |
                       ((unsigned int)data[*offset + 2] << 16) |
                       ((unsigned int)data[*offset + 3] << 24);
    *offset += 4;
    if (val > max_val) val = max_val;
    return val;
}

/* Helper function to get a valid image format */
static aom_img_fmt_t get_fmt(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset >= size) return AOM_IMG_FMT_I420;
    unsigned int val = data[*offset];
    (*offset)++;
    
    // Map arbitrary value to a known format to avoid invalid enums
    switch (val % 10) {
        case 0: return AOM_IMG_FMT_I420;
        case 1: return AOM_IMG_FMT_YV12;
        case 2: return AOM_IMG_FMT_AOMYV12;
        case 3: return AOM_IMG_FMT_AOMI420;
        case 4: return AOM_IMG_FMT_I422;
        case 5: return AOM_IMG_FMT_I444;
        case 6: return AOM_IMG_FMT_NV12;
        case 7: return AOM_IMG_FMT_I42016;
        case 8: return AOM_IMG_FMT_YV1216;
        case 9: return AOM_IMG_FMT_I42216;
        default: return AOM_IMG_FMT_I420;
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    size_t offset = 0;
    aom_image_t *img = NULL;
    aom_image_t local_img;
    int use_heap_img = (Data[offset++] % 2 == 0);
    
    aom_img_fmt_t fmt = get_fmt(Data, &offset, Size);
    
    // Dimensions: max 2^27 as per spec
    unsigned int d_w = get_uint(Data, &offset, Size, 0x08000000);
    unsigned int d_h = get_uint(Data, &offset, Size, 0x08000000);
    
    // Alignment: max 65536
    unsigned int align = get_uint(Data, &offset, Size, 65536);
    if (align == 0) align = 1;
    
    unsigned int size_align = get_uint(Data, &offset, Size, 65536);
    if (size_align == 0) size_align = 1;
    
    unsigned int border = get_uint(Data, &offset, Size, 65536);
    
    unsigned int x = get_uint(Data, &offset, Size, d_w);
    unsigned int y = get_uint(Data, &offset, Size, d_h);
    unsigned int rect_w = get_uint(Data, &offset, Size, d_w - x);
    unsigned int rect_h = get_uint(Data, &offset, Size, d_h - y);

    // Strategy 1: aom_img_alloc
    if (use_heap_img) {
        img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
    } else {
        img = aom_img_alloc(&local_img, fmt, d_w, d_h, align);
    }

    if (img) {
        // Test aom_img_set_rect
        // Ensure rect_w and rect_h don't cause overflow or negative logic
        if (x + rect_w <= d_w && y + rect_h <= d_h) {
            aom_img_set_rect(img, x, y, rect_w, rect_h, border);
        }
        
        // Test aom_img_remove_metadata (safe to call even if no metadata)
        aom_img_remove_metadata(img);
        
        // Test aom_img_alloc_with_border (re-allocating or creating new)
        // We need to free the current one first if we want to test the alloc function cleanly
        // But to test state diversity, let's try to wrap or alloc_with_border on a fresh struct
        aom_img_free(img);
        img = NULL;
        
        // Strategy 2: aom_img_alloc_with_border
        if (use_heap_img) {
            img = aom_img_alloc_with_border(NULL, fmt, d_w, d_h, align, size_align, border);
        } else {
            img = aom_img_alloc_with_border(&local_img, fmt, d_w, d_h, align, size_align, border);
        }

        if (img) {
            // Test aom_img_set_rect again on bordered image
            if (x + rect_w <= img->w && y + rect_h <= img->h) {
                aom_img_set_rect(img, x, y, rect_w, rect_h, border);
            }
            aom_img_remove_metadata(img);
            aom_img_free(img);
            img = NULL;
        }

        // Strategy 3: aom_img_wrap
        // We need to allocate a buffer manually to wrap
        // Calculate a safe buffer size. For fuzzing, we'll allocate a generous amount
        // or use the dimensions provided.
        // Note: aom_img_wrap requires specific alignment and size. 
        // To simplify fuzzing, we allocate a large buffer and let the API validate.
        // If the buffer is too small, aom_img_wrap might return NULL or behave safely.
        
        size_t alloc_size = (size_t)d_w * (size_t)d_h * 3 * 2; // Rough estimate for highbitdepth planar
        if (alloc_size > 10 * 1024 * 1024) alloc_size = 10 * 1024 * 1024; // Cap at 10MB
        if (alloc_size == 0) alloc_size = 1024;

        unsigned char *user_buffer = malloc(alloc_size);
        if (user_buffer) {
            // Try wrapping with NULL img (heap alloc for struct)
            aom_image_t *wrapped_img = aom_img_wrap(NULL, fmt, d_w, d_h, align, user_buffer);
            if (wrapped_img) {
                if (x + rect_w <= wrapped_img->w && y + rect_h <= wrapped_img->h) {
                    aom_img_set_rect(wrapped_img, x, y, rect_w, rect_h, border);
                }
                aom_img_remove_metadata(wrapped_img);
                aom_img_free(wrapped_img);
            }

            // Try wrapping with pre-allocated struct
            memset(&local_img, 0, sizeof(local_img));
            wrapped_img = aom_img_wrap(&local_img, fmt, d_w, d_h, align, user_buffer);
            if (wrapped_img) {
                if (x + rect_w <= wrapped_img->w && y + rect_h <= wrapped_img->h) {
                    aom_img_set_rect(wrapped_img, x, y, rect_w, rect_h, border);
                }
                aom_img_remove_metadata(wrapped_img);
                aom_img_free(wrapped_img);
            }
            
            free(user_buffer);
        }
    }

    return 0;
}