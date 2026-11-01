// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_image.h"
#include "aom_decoder.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int read_uint32(const uint8_t **data, size_t *size) {
    if (*size < 4) return 0;
    uint32_t val = 0;
    memcpy(&val, *data, 4);
    *data += 4;
    *size -= 4;
    return val;
}

static int read_int32(const uint8_t **data, size_t *size) {
    if (*size < 4) return 0;
    int32_t val = 0;
    memcpy(&val, *data, 4);
    *data += 4;
    *size -= 4;
    return val;
}

static int read_uint8(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    uint8_t val = **data;
    (*data)++;
    (*size)--;
    return val;
}

static int read_int8(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    int8_t val = **(int8_t **)data;
    (*data)++;
    (*size)--;
    return val;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // Initialize basic fields to prevent undefined behavior
    aom_image_t img;
    memset(&img, 0, sizeof(aom_image_t));

    // Parse input to configure image
    int width = read_uint32(&ptr, &remaining);
    int height = read_uint32(&ptr, &remaining);
    int fmt_val = read_uint32(&ptr, &remaining);
    int bit_depth = read_uint8(&ptr, &remaining);

    // Sanity checks for dimensions
    if (width <= 0 || width > 16384) width = 64;
    if (height <= 0 || height > 16384) height = 64;
    if (bit_depth <= 0 || bit_depth > 16) bit_depth = 8;

    // Re-allocate with proper dimensions and format
    aom_img_fmt_t fmt = (aom_img_fmt_t)fmt_val;
    // Ensure format is valid, default to I420 if invalid
    if (fmt == AOM_IMG_FMT_NONE || fmt > AOM_IMG_FMT_I44416) {
        fmt = AOM_IMG_FMT_I420;
    }
    
    aom_image_t *img_ptr = aom_img_alloc(&img, fmt, width, height, 1);
    if (!img_ptr) return 0;

    // Set parsed values
    img_ptr->bit_depth = bit_depth;
    img_ptr->w = width;
    img_ptr->h = height;
    img_ptr->d_w = width;
    img_ptr->d_h = height;
    img_ptr->r_w = width;
    img_ptr->r_h = height;

    // Parse additional parameters for fuzzing
    int x = read_uint32(&ptr, &remaining);
    int y = read_uint32(&ptr, &remaining);
    int w = read_uint32(&ptr, &remaining);
    int h = read_uint32(&ptr, &remaining);
    int border = read_uint32(&ptr, &remaining);
    int plane_idx = read_int32(&ptr, &remaining);

    // Clamp values to reasonable ranges to avoid immediate crashes in logic
    if (x > width) x = 0;
    if (y > height) y = 0;
    if (w > width) w = width;
    if (h > height) h = height;
    if (border > 100) border = 0;
    if (plane_idx < 0) plane_idx = 0;
    if (plane_idx > 2) plane_idx = 2;

    // Fuzz aom_img_set_rect
    // This function modifies the image descriptor
    aom_img_set_rect(img_ptr, x, y, w, h, border);

    // Fuzz aom_img_plane_width
    int plane_w = aom_img_plane_width(img_ptr, plane_idx);
    (void)plane_w; // Use return value to suppress warnings

    // Fuzz aom_img_plane_height
    int plane_h = aom_img_plane_height(img_ptr, plane_idx);
    (void)plane_h; // Use return value to suppress warnings

    // Fuzz aom_img_flip
    // Flip once
    aom_img_flip(img_ptr);
    // Flip again to restore
    aom_img_flip(img_ptr);
    // Flip a third time to test state
    aom_img_flip(img_ptr);

    // Fuzz aom_img_remove_metadata
    // We don't set metadata here, but calling on NULL metadata is safe
    aom_img_remove_metadata(img_ptr);

    // Fuzz aom_img_free
    aom_img_free(img_ptr);
    img_ptr = NULL;

    // Test aom_img_free with NULL (should be safe)
    aom_img_free(NULL);

    return 0;
}