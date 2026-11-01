// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static int read_uint32(const uint8_t **data, size_t *size) {
    if (*size < 4) return 0;
    // Ensure we don't read beyond the buffer
    uint32_t val = (uint32_t)(*data)[0] | ((uint32_t)(*data)[1] << 8) |
                   ((uint32_t)(*data)[2] << 16) | ((uint32_t)(*data)[3] << 24);
    *data += 4;
    *size -= 4;
    return val;
}

static int read_int32(const uint8_t **data, size_t *size) {
    if (*size < 4) return 0;
    int32_t val = (int32_t)(*data)[0] | ((int32_t)(*data)[1] << 8) |
                  ((int32_t)(*data)[2] << 16) | ((int32_t)(*data)[3] << 24);
    *data += 4;
    *size -= 4;
    return val;
}

static int read_uint8(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    uint8_t val = (*data)[0];
    *data += 1;
    *size -= 1;
    return val;
}

int LLVMFuzzerTestOneInput_262(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // Read parameters for image creation
    uint32_t w = read_uint32(&ptr, &remaining);
    uint32_t h = read_uint32(&ptr, &remaining);
    uint32_t fmt_val = read_uint32(&ptr, &remaining);
    uint32_t bit_depth = read_uint32(&ptr, &remaining);

    // Sanity checks to prevent crashes during allocation
    if (w == 0 || h == 0 || w > 8192 || h > 8192) return 0;
    if (bit_depth == 0 || bit_depth > 16) bit_depth = 8;

    aom_img_fmt_t fmt = (aom_img_fmt_t)fmt_val;
    
    // aom_img_alloc signature: aom_image_t *aom_img_alloc(aom_image_t *img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align);
    // We pass NULL for img to allocate a new one.
    aom_image_t *img = aom_img_alloc(NULL, fmt, w, h, 16);
    if (img == NULL) return 0;

    // Fuzz aom_img_set_rect
    if (remaining >= 20) {
        uint32_t x = read_uint32(&ptr, &remaining);
        uint32_t y = read_uint32(&ptr, &remaining);
        uint32_t rect_w = read_uint32(&ptr, &remaining);
        uint32_t rect_h = read_uint32(&ptr, &remaining);
        uint32_t border = read_uint32(&ptr, &remaining);
        
        // Attempt to set rectangle with potentially invalid coordinates
        aom_img_set_rect(img, x, y, rect_w, rect_h, border);
    }

    // Fuzz aom_img_plane_width and aom_img_plane_height
    // We try planes 0, 1, 2 and some negative/overflow values
    int planes_to_test[] = {0, 1, 2, -1, 3, 100, INT_MAX, INT_MIN};
    for (int i = 0; i < 8; i++) {
        int plane = planes_to_test[i];
        int w_ret = aom_img_plane_width(img, plane);
        (void)w_ret; // Suppress unused variable warning
        int h_ret = aom_img_plane_height(img, plane);
        (void)h_ret; // Suppress unused variable warning
    }

    // Fuzz aom_img_remove_metadata
    // Calling it on a clean image is safe and tests the NULL check.
    aom_img_remove_metadata(img);

    // Note: aom_codec_control_typechecked_AOME_USE_REFERENCE cannot be tested
    // as per the prompt's description of missing implementation/signature.

    // Cleanup
    aom_img_free(img);

    return 0;
}