// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

// Helper to safely extract integers from fuzz input
static int get_int_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        *offset = 0;
        return 0;
    }
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

static unsigned int get_uint_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(unsigned int) > size) {
        *offset = 0;
        return 0;
    }
    unsigned int val;
    memcpy(&val, data + *offset, sizeof(unsigned int));
    *offset += sizeof(unsigned int);
    return val;
}

static aom_image_t* create_test_image(const uint8_t *data, size_t size, size_t *offset) {
    // Default allocation
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 1, 1, 1);
    if (!img) return NULL;

    // Check if we have enough data to read width and height
    if (*offset + 2 * sizeof(unsigned int) <= size) {
        unsigned int w, h;
        memcpy(&w, data + *offset, sizeof(unsigned int));
        *offset += sizeof(unsigned int);
        memcpy(&h, data + *offset, sizeof(unsigned int));
        *offset += sizeof(unsigned int);

        // Clamp to reasonable limits to avoid OOM or overflow
        if (w > 4096) w = 4096;
        if (h > 4096) h = 4096;
        if (w == 0) w = 1;
        if (h == 0) h = 1;

        // Reallocate with new dimensions
        aom_img_free(img);
        img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, w, h, 1);
        if (!img) return NULL;
    } else {
        // Not enough data for dimensions, reset offset to 0 for subsequent reads
        *offset = 0;
    }

    // Fill planes with fuzz data if possible
    // Calculate the actual allocated size for each plane to avoid overflow
    for (int i = 0; i < 3; i++) {
        if (img->planes[i]) {
            size_t plane_height = img->h;
            size_t plane_stride = (size_t)img->stride[i];
            
            // Check for potential overflow in multiplication
            if (plane_stride > 0 && plane_height > SIZE_MAX / plane_stride) {
                continue;
            }
            
            size_t plane_size = plane_stride * plane_height;
            
            if (*offset + plane_size <= size) {
                memcpy(img->planes[i], data + *offset, plane_size);
                *offset += plane_size;
            } else {
                // Fill with pattern if not enough data
                size_t available = size - *offset;
                if (available > 0) {
                    memset(img->planes[i], 0xAA, available);
                    *offset += available;
                }
            }
        }
    }

    return img;
}

int LLVMFuzzerTestOneInput_256(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    size_t offset = 0;
    aom_image_t *img = NULL;

    // 1. Create an image
    img = create_test_image(Data, Size, &offset);
    if (!img) return 0;

    // 2. Test aom_img_plane_width
    int plane_idx = get_int_from_data(Data, Size, &offset);
    // Clamp plane index to valid range [0, 2] to avoid out-of-bounds access in library
    if (plane_idx < 0) plane_idx = 0;
    if (plane_idx > 2) plane_idx = 2;
    
    int width = aom_img_plane_width(img, plane_idx);
    (void)width; // Suppress unused variable warning

    // 3. Test aom_img_plane_height
    plane_idx = get_int_from_data(Data, Size, &offset);
    if (plane_idx < 0) plane_idx = 0;
    if (plane_idx > 2) plane_idx = 2;
    
    int height = aom_img_plane_height(img, plane_idx);
    (void)height;

    // 4. Test aom_img_set_rect
    unsigned int x = get_uint_from_data(Data, Size, &offset);
    unsigned int y = get_uint_from_data(Data, Size, &offset);
    unsigned int w = get_uint_from_data(Data, Size, &offset);
    unsigned int h = get_uint_from_data(Data, Size, &offset);
    unsigned int border = get_uint_from_data(Data, Size, &offset);

    // Clamp values to prevent overflow or invalid states
    if (x > img->w) x = img->w;
    if (y > img->h) y = img->h;
    if (w > img->w - x) w = img->w - x;
    if (h > img->h - y) h = img->h - y;
    if (border > 10) border = 10;

    int ret_rect = aom_img_set_rect(img, x, y, w, h, border);
    (void)ret_rect;

    // 5. Test aom_img_flip
    aom_img_flip(img);
    // Flip again to restore (optional, but good for state exploration)
    aom_img_flip(img);

    // 6. Test aom_img_free
    aom_img_free(img);
    img = NULL;

    return 0;
}