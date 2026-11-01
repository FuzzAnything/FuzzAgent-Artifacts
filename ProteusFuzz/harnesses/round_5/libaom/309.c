// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
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

static aom_image_t *create_test_image(const uint8_t *data, size_t size) {
    if (size < 2) return NULL;

    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 1);
    if (!img) return NULL;

    // Use data to determine dimensions if possible, otherwise use defaults
    unsigned int w = 64;
    unsigned int h = 64;
    
    if (size >= 4) {
        w = (unsigned int)(data[0] & 0x3F) + 1; // 1-64
        h = (unsigned int)(data[1] & 0x3F) + 1; // 1-64
        // Ensure minimum size for allocation safety
        if (w < 2) w = 2;
        if (h < 2) h = 2;
    }

    // Re-allocate if dimensions changed significantly to avoid reallocation overhead in loop
    if (w != 64 || h != 64) {
        aom_img_free(img);
        img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, w, h, 1);
        if (!img) return NULL;
    }

    // Fill image data with fuzz input if available
    if (img->planes[0] && size > 0) {
        size_t bytes_needed = (size_t)w * h;
        if (bytes_needed > size) bytes_needed = size;
        memcpy(img->planes[0], data, bytes_needed);
    }

    return img;
}

int LLVMFuzzerTestOneInput_309(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_image_t *img = create_test_image(Data, Size);
    if (!img) return 0;

    // Extract parameters from fuzz data
    unsigned int x, y, w, h, border;
    int plane_idx;

    if (Size >= 5) {
        x = (unsigned int)Data[0];
        y = (unsigned int)Data[1];
        w = (unsigned int)Data[2];
        h = (unsigned int)Data[3];
        border = (unsigned int)Data[4];
    } else {
        x = 0; y = 0; w = 0; h = 0; border = 0;
    }

    if (Size >= 6) {
        plane_idx = (int)(Data[5] & 0x03); // 0, 1, 2
    } else {
        plane_idx = 0;
    }

    // Fuzz aom_img_set_rect
    // Try valid and invalid coordinates to trigger error paths
    int ret_rect = aom_img_set_rect(img, x, y, w, h, border);
    (void)ret_rect; // Suppress unused warning, check logic internally

    // Fuzz aom_img_plane_width
    int width = aom_img_plane_width(img, plane_idx);
    (void)width;

    // Fuzz aom_img_plane_height
    int height = aom_img_plane_height(img, plane_idx);
    (void)height;

    // Fuzz aom_img_remove_metadata
    // Create a dummy metadata array to test removal logic if needed, 
    // but aom_img_remove_metadata handles NULL gracefully.
    aom_img_remove_metadata(img);

    // Fuzz aom_img_free
    aom_img_free(img);

    return 0;
}