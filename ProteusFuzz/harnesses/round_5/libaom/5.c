// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

static int create_dummy_image(aom_image_t *img, const uint8_t *data, size_t size) {
    if (!img || !data || size == 0) return -1;

    // Initialize image with minimal valid data
    img->fmt = AOM_IMG_FMT_I420;
    img->w = 16;
    img->h = 16;
    img->bit_depth = 8;
    img->d_w = 16;
    img->d_h = 16;
    img->r_w = 16;
    img->r_h = 16;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_COLOCATED;
    img->monochrome = 0;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->fb_priv = NULL;
    img->metadata = NULL;

    // Allocate planes
    size_t plane_size = img->w * img->h;
    img->planes[0] = malloc(plane_size);
    img->planes[1] = malloc(plane_size / 4);
    img->planes[2] = malloc(plane_size / 4);
    img->stride[0] = img->w;
    img->stride[1] = img->w / 2;
    img->stride[2] = img->w / 2;

    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        if (img->planes[0]) free(img->planes[0]);
        if (img->planes[1]) free(img->planes[1]);
        if (img->planes[2]) free(img->planes[2]);
        return -1;
    }

    // Copy input data to Y plane (safe copy)
    size_t copy_size = (size < plane_size) ? size : plane_size;
    memcpy(img->planes[0], data, copy_size);
    memset(img->planes[1], 128, plane_size / 4);
    memset(img->planes[2], 128, plane_size / 4);

    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->sz = plane_size + (plane_size / 4) * 2;
    img->bps = 0;

    return 0;
}

static void cleanup_image(aom_image_t *img) {
    if (!img) return;
    if (img->planes[0]) free(img->planes[0]);
    if (img->planes[1]) free(img->planes[1]);
    if (img->planes[2]) free(img->planes[2]);
    img->planes[0] = NULL;
    img->planes[1] = NULL;
    img->planes[2] = NULL;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use first byte to determine test path
    uint8_t mode = Data[0] % 5;

    // Test 0: aom_img_metadata_alloc and aom_img_metadata_free
    if (mode == 0) {
        uint32_t type = (Size > 1) ? Data[1] : 0;
        aom_metadata_insert_flags_t flag = (Size > 2) ? (Data[2] % 19) : 0;
        size_t sz = (Size > 3) ? Data[3] : 0;
        
        // Ensure we don't pass a size larger than available data
        const uint8_t *meta_data = NULL;
        if (Size > 4) {
            meta_data = &Data[4];
            // Cap sz to available data size to prevent buffer overflow
            size_t available = Size - 4;
            if (sz > available) sz = available;
        }

        aom_metadata_t *meta = aom_img_metadata_alloc(type, meta_data, sz, flag);
        if (meta) {
            // Just verify allocation succeeded
            aom_img_metadata_free(meta);
        }
        return 0;
    }

    // Test 1: aom_img_add_metadata and aom_img_get_metadata
    if (mode == 1) {
        aom_image_t img;
        memset(&img, 0, sizeof(img));

        if (create_dummy_image(&img, Data, Size) != 0) return 0;

        uint32_t type = (Size > 1) ? Data[1] : 0;
        aom_metadata_insert_flags_t flag = (Size > 2) ? (Data[2] % 19) : 0;
        size_t sz = (Size > 3) ? Data[3] : 0;
        
        const uint8_t *meta_data = NULL;
        if (Size > 4) {
            meta_data = &Data[4];
            // Cap sz to available data size to prevent buffer overflow
            size_t available = Size - 4;
            if (sz > available) sz = available;
        }

        int ret = aom_img_add_metadata(&img, type, meta_data, sz, flag);
        (void)ret; // Suppress unused warning

        // Try to retrieve metadata
        size_t num_meta = aom_img_num_metadata(&img);
        for (size_t i = 0; i < num_meta; i++) {
            const aom_metadata_t *m = aom_img_get_metadata(&img, i);
            if (m) {
                // Safe to access
            }
        }

        // Ensure metadata and image are freed to prevent leaks
        aom_img_free(&img);
        return 0;
    }

    // Test 2: aom_img_num_metadata on empty image
    if (mode == 2) {
        aom_image_t img;
        memset(&img, 0, sizeof(img));
        img.metadata = NULL;
        size_t count = aom_img_num_metadata(&img);
        (void)count;
        return 0;
    }

    // Test 3: aom_img_free on manually allocated image
    if (mode == 3) {
        aom_image_t img;
        memset(&img, 0, sizeof(img));

        if (create_dummy_image(&img, Data, Size) != 0) return 0;

        // Add some metadata to ensure cleanup handles it
        uint32_t type = (Size > 1) ? Data[1] : 0;
        aom_metadata_insert_flags_t flag = (Size > 2) ? (Data[2] % 19) : 0;
        size_t sz = (Size > 3) ? Data[3] : 0;
        
        const uint8_t *meta_data = NULL;
        if (Size > 4) {
            meta_data = &Data[4];
            // Cap sz to available data size to prevent buffer overflow
            size_t available = Size - 4;
            if (sz > available) sz = available;
        }
        aom_img_add_metadata(&img, type, meta_data, sz, flag);

        aom_img_free(&img);
        return 0;
    }

    // Test 4: Edge case - NULL pointers and zero sizes
    if (mode == 4) {
        aom_metadata_t *meta = aom_img_metadata_alloc(0, NULL, 0, 0);
        if (meta) aom_img_metadata_free(meta);

        aom_image_t img;
        memset(&img, 0, sizeof(img));
        aom_img_add_metadata(&img, 0, NULL, 0, 0);
        aom_img_add_metadata(NULL, 0, Data, Size, 0);
        aom_img_get_metadata(NULL, 0);
        aom_img_num_metadata(NULL);
        aom_img_metadata_free(NULL);
        aom_img_free(NULL);

        return 0;
    }

    return 0;
}