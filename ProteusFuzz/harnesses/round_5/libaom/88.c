// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
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

static uint32_t read_u32(const uint8_t *data, size_t *idx, size_t size) {
    if (*idx + 4 > size) return 0;
    uint32_t val = ((uint32_t)data[*idx] << 24) |
                   ((uint32_t)data[*idx + 1] << 16) |
                   ((uint32_t)data[*idx + 2] << 8) |
                   ((uint32_t)data[*idx + 3]);
    *idx += 4;
    return val;
}

static uint32_t read_u32_clamped(const uint8_t *data, size_t *idx, size_t size, uint32_t max_val) {
    uint32_t val = read_u32(data, idx, size);
    if (val > max_val) val = max_val;
    return val;
}

static aom_img_fmt_t read_fmt(const uint8_t *data, size_t *idx, size_t size) {
    if (*idx + 4 > size) return AOM_IMG_FMT_NONE;
    uint32_t val = read_u32(data, idx, size);
    // Map to known formats or clamp to safe range
    if (val == 0) return AOM_IMG_FMT_NONE;
    if (val == 1) return AOM_IMG_FMT_YV12;
    if (val == 2) return AOM_IMG_FMT_I420;
    if (val == 3) return AOM_IMG_FMT_AOMYV12;
    if (val == 4) return AOM_IMG_FMT_AOMI420;
    if (val == 5) return AOM_IMG_FMT_I422;
    if (val == 6) return AOM_IMG_FMT_I444;
    if (val == 7) return AOM_IMG_FMT_NV12;
    if (val == 8) return AOM_IMG_FMT_I42016;
    if (val == 9) return AOM_IMG_FMT_YV1216;
    if (val == 10) return AOM_IMG_FMT_I42216;
    if (val == 11) return AOM_IMG_FMT_I44416;
    // Fallback to a safe planar format if out of range but not zero
    return AOM_IMG_FMT_I420;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    size_t idx = 0;
    uint32_t d_w = read_u32_clamped(Data, &idx, Size, 0x08000000);
    uint32_t d_h = read_u32_clamped(Data, &idx, Size, 0x08000000);
    uint32_t align = read_u32_clamped(Data, &idx, Size, 65536);
    if (align == 0) align = 1; // Avoid division by zero or invalid alignment

    aom_img_fmt_t fmt = read_fmt(Data, &idx, Size);
    if (fmt == AOM_IMG_FMT_NONE) return 0;

    // Test 1: aom_img_alloc
    aom_image_t *img_alloc = aom_img_alloc(NULL, fmt, d_w, d_h, align);
    if (img_alloc) {
        // Test 2: aom_img_set_rect
        uint32_t x = read_u32_clamped(Data, &idx, Size, d_w);
        uint32_t y = read_u32_clamped(Data, &idx, Size, d_h);
        uint32_t w = read_u32_clamped(Data, &idx, Size, d_w - x);
        uint32_t h = read_u32_clamped(Data, &idx, Size, d_h - y);
        uint32_t border = read_u32_clamped(Data, &idx, Size, 65536);

        aom_img_set_rect(img_alloc, x, y, w, h, border);

        // Test 3: aom_img_flip
        aom_img_flip(img_alloc);
        aom_img_flip(img_alloc); // Flip back

        // Test 4: aom_img_remove_metadata
        aom_img_remove_metadata(img_alloc);

        aom_img_free(img_alloc);
    }

    // Test 5: aom_img_alloc_with_border
    uint32_t size_align = read_u32_clamped(Data, &idx, Size, 65536);
    if (size_align == 0) size_align = 1;
    uint32_t border_val = read_u32_clamped(Data, &idx, Size, 65536);

    aom_image_t *img_border = aom_img_alloc_with_border(NULL, fmt, d_w, d_h, align, size_align, border_val);
    if (img_border) {
        aom_img_set_rect(img_border, 0, 0, d_w, d_h, border_val);
        aom_img_flip(img_border);
        aom_img_remove_metadata(img_border);
        aom_img_free(img_border);
    }

    // Test 6: aom_img_wrap
    // Allocate a buffer large enough for the image
    size_t buffer_size = (size_t)d_w * (size_t)d_h * 3; // Rough estimate for planar
    if (buffer_size == 0) buffer_size = 1;
    unsigned char *img_data = (unsigned char *)malloc(buffer_size);
    if (!img_data) return 0;

    // Initialize buffer to avoid uninitialized memory issues
    memset(img_data, 0, buffer_size);

    aom_image_t img_wrap;
    aom_image_t *img_wrapped = aom_img_wrap(&img_wrap, fmt, d_w, d_h, align, img_data);
    if (img_wrapped) {
        aom_img_set_rect(img_wrapped, 0, 0, d_w, d_h, 0);
        aom_img_flip(img_wrapped);
        aom_img_remove_metadata(img_wrapped);
        // Note: Do not call aom_img_free on wrapped image if we allocated the buffer manually
        // The buffer ownership remains with us.
    }

    free(img_data);

    // Test 7: Reuse descriptor with aom_img_alloc
    aom_image_t img_reuse;
    aom_image_t *img_reused = aom_img_alloc(&img_reuse, fmt, d_w, d_h, align);
    if (img_reused) {
        aom_img_set_rect(img_reused, 0, 0, d_w, d_h, 0);
        aom_img_flip(img_reused);
        aom_img_remove_metadata(img_reused);
        aom_img_free(img_reused);
    }

    return 0;
}