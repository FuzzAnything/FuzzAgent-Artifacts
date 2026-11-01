// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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

static aom_image_t *create_test_image(const uint8_t *data, size_t size) {
    if (size < 16) {
        return NULL;
    }

    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 1);
    if (img == NULL) {
        return NULL;
    }

    // Initialize image data with fuzzed input if available
    if (img->planes[0] != NULL) {
        size_t y_size = (size_t)img->stride[0] * img->h;
        if (size > y_size) size = y_size;
        memcpy(img->planes[0], data, size);
    }
    
    // Initialize U and V planes with pattern data
    if (img->planes[1] != NULL) {
        size_t uv_size = (size_t)img->stride[1] * (img->h / 2);
        memset(img->planes[1], 0x55, uv_size);
    }
    if (img->planes[2] != NULL) {
        size_t uv_size = (size_t)img->stride[2] * (img->h / 2);
        memset(img->planes[2], 0xAA, uv_size);
    }

    // Do not manually allocate metadata as the struct is opaque/incomplete in the header.
    // aom_img_remove_metadata handles NULL gracefully.

    return img;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_image_t *img = create_test_image(Data, Size);
    if (img == NULL) {
        return 0;
    }

    // Extract parameters from fuzz data for diversification
    unsigned int x = 0, y = 0, w = 0, h = 0, border = 0;
    int plane = 0;

    if (Size >= 6) {
        x = (Data[0] % 32);
        y = (Data[1] % 32);
        w = (Data[2] % 32) + 1;
        h = (Data[3] % 32) + 1;
        border = (Data[4] % 8);
        plane = (Data[5] % 3);
    }

    // Test aom_img_plane_width
    int width = aom_img_plane_width(img, plane);
    (void)width; // Use variable to avoid unused warning

    // Test aom_img_plane_height
    int height = aom_img_plane_height(img, plane);
    (void)height; // Use variable to avoid unused warning

    // Test aom_img_set_rect
    int ret = aom_img_set_rect(img, x, y, w, h, border);
    (void)ret; // Use variable to avoid unused warning

    // Test aom_img_flip
    aom_img_flip(img);
    
    // Flip again to restore
    aom_img_flip(img);

    // Test aom_img_remove_metadata (safe to call even if metadata is NULL)
    aom_img_remove_metadata(img);

    // Test aom_img_free
    aom_img_free(img);

    return 0;
}