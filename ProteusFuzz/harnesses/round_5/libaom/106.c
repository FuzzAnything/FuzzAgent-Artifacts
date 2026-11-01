// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static uint32_t read_u32(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 > size) {
        return 0;
    }
    uint32_t val = 0;
    memcpy(&val, data + *offset, 4);
    *offset += 4;
    return val;
}

static int read_int(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 > size) {
        return 0;
    }
    int val = 0;
    memcpy(&val, data + *offset, 4);
    *offset += 4;
    return val;
}

static uint8_t read_u8(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 1 > size) {
        return 0;
    }
    uint8_t val = data[*offset];
    *offset += 1;
    return val;
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    size_t offset = 0;
    aom_image_t img = {0};
    aom_image_t *img_ptr = &img;
    aom_image_t *heap_img = NULL;

    // Read parameters from fuzz input
    uint32_t fmt_val = read_u32(Data, &offset, Size);
    uint32_t d_w = read_u32(Data, &offset, Size);
    uint32_t d_h = read_u32(Data, &offset, Size);
    uint32_t align = read_u32(Data, &offset, Size);
    uint32_t size_align = read_u32(Data, &offset, Size);
    uint32_t border = read_u32(Data, &offset, Size);
    uint32_t x = read_u32(Data, &offset, Size);
    uint32_t y = read_u32(Data, &offset, Size);
    uint32_t w = read_u32(Data, &offset, Size);
    uint32_t h = read_u32(Data, &offset, Size);
    int plane = read_int(Data, &offset, Size);

    // Sanitize inputs to prevent immediate crashes or undefined behavior
    // Limit dimensions to reasonable values to avoid OOM or allocation failures
    if (d_w > 0x08000000) d_w = 0x08000000;
    if (d_h > 0x08000000) d_h = 0x08000000;
    if (align > 65536) align = 65536;
    if (size_align > 65536) size_align = 65536;
    if (border > 65536) border = 65536;
    if (fmt_val > AOM_IMG_FMT_I44416) fmt_val = AOM_IMG_FMT_I420; // Default to safe format

    aom_img_fmt_t fmt = (aom_img_fmt_t)fmt_val;

    // 1. Test aom_img_alloc_with_border
    // Try allocating with a pre-allocated struct first
    heap_img = aom_img_alloc_with_border(&img, fmt, d_w, d_h, align, size_align, border);
    if (heap_img != NULL) {
        img_ptr = heap_img;
    } else {
        // Fallback to NULL if pre-alloc failed, though usually it returns the passed pointer
        // If it returns NULL here, it might be an allocation failure, which is fine for fuzzing
        // We proceed with the struct we have if it was partially initialized, but safer to skip if NULL
        if (img_ptr == NULL) {
            // Try allocating on heap directly
            heap_img = aom_img_alloc_with_border(NULL, fmt, d_w, d_h, align, size_align, border);
            if (heap_img != NULL) {
                img_ptr = heap_img;
            } else {
                return 0; // Allocation failed, nothing to fuzz
            }
        }
    }

    // Ensure we have a valid image pointer before proceeding
    if (img_ptr == NULL) {
        return 0;
    }

    // 2. Test aom_img_set_rect
    // This function modifies the image descriptor
    int ret = aom_img_set_rect(img_ptr, x, y, w, h, border);
    (void)ret; // Ignore return for fuzzing, just trigger the logic

    // 3. Test aom_img_plane_width
    // Ensure plane index is within bounds [0, 2]
    int safe_plane = plane;
    if (safe_plane < 0) safe_plane = 0;
    if (safe_plane > 2) safe_plane = 2;

    int width = aom_img_plane_width(img_ptr, safe_plane);
    (void)width;

    // 4. Test aom_img_plane_height
    int height = aom_img_plane_height(img_ptr, safe_plane);
    (void)height;

    // 5. Test aom_img_flip
    // Flip once
    aom_img_flip(img_ptr);
    // Flip again to restore (optional, but explores state)
    aom_img_flip(img_ptr);

    // 6. Test aom_img_remove_metadata
    // This cleans up metadata if any exists
    aom_img_remove_metadata(img_ptr);

    // Cleanup
    // If we allocated on the heap (img_ptr != &img), we need to free it.
    // aom_img_free is the standard way to free an image allocated by aom_img_alloc_with_border
    if (img_ptr != &img) {
        aom_img_free(img_ptr);
    }

    return 0;
}