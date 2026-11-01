// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_image(aom_image_t *img) {
    if (img) {
        aom_img_remove_metadata(img);
        aom_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    // Need at least 12 uint32_t values (48 bytes) to safely access all parameters
    if (Size < 48) return 0;

    const uint32_t *params = (const uint32_t *)Data;
    
    // Limit dimensions to 2^27 as per API requirements
    uint32_t width = params[0] & 0x07FFFFFF; 
    uint32_t height = params[1] & 0x07FFFFFF;
    
    // Limit alignment to 65536 (0xFFFF)
    uint32_t align = params[2] & 0xFFFF; 
    uint32_t size_align = params[3] & 0xFFFF;
    uint32_t border = params[4] & 0xFFFF;
    uint32_t stride_align = params[5] & 0xFFFF;
    
    // Use modulo to select a valid format index
    uint32_t fmt_idx = params[6] % 10;
    
    // Rectangle parameters
    uint32_t x = params[7] & 0x07FFFFFF;
    uint32_t y = params[8] & 0x07FFFFFF;
    uint32_t w = params[9] & 0x07FFFFFF;
    uint32_t h = params[10] & 0x07FFFFFF;

    // Ensure minimum dimensions to avoid invalid allocations
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (align == 0) align = 1;
    if (stride_align == 0) stride_align = 1;

    aom_img_fmt_t fmt = (aom_img_fmt_t)fmt_idx;
    // Ensure we have a valid format
    if (fmt == AOM_IMG_FMT_NONE) fmt = AOM_IMG_FMT_I420;

    aom_image_t *img = NULL;
    unsigned char *img_data = NULL;
    int ret = 0;

    // Test aom_img_alloc
    img = aom_img_alloc(NULL, fmt, width, height, align);
    if (img) {
        aom_img_remove_metadata(img);
        aom_img_free(img);
        img = NULL;
    }

    // Test aom_img_alloc_with_border
    img = aom_img_alloc_with_border(NULL, fmt, width, height, align, size_align, border);
    if (img) {
        aom_img_remove_metadata(img);
        aom_img_free(img);
        img = NULL;
    }

    // Test aom_img_wrap
    // Calculate required buffer size roughly (simplified for fuzzing)
    // Using a safe overestimate to prevent under-allocation
    size_t plane_size = (size_t)(width + align) * (height + align) * 4; 
    img_data = (unsigned char *)malloc(plane_size);
    if (img_data) {
        img = aom_img_wrap(NULL, fmt, width, height, stride_align, img_data);
        if (img) {
            aom_img_remove_metadata(img);
            aom_img_free(img);
            img = NULL;
        }
        free(img_data);
        img_data = NULL;
    }

    // Test aom_img_set_rect and aom_img_flip
    img = aom_img_alloc(NULL, fmt, width, height, align);
    if (img) {
        // Try to set a rectangle
        ret = aom_img_set_rect(img, x, y, w, h, border);
        (void)ret;

        // Try to flip the image
        aom_img_flip(img);
        aom_img_flip(img); // Flip back

        // Try to remove metadata (should be safe even if NULL)
        aom_img_remove_metadata(img);

        aom_img_free(img);
        img = NULL;
    }

    return 0;
}