// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "aom/aom_image.h"

static void dummy_image_init(aom_image_t *img) {
    memset(img, 0, sizeof(*img));
    img->fmt = AOM_IMG_FMT_I420;
    img->w = 64;
    img->h = 64;
    img->d_w = 64;
    img->d_h = 64;
    img->r_w = 64;
    img->r_h = 64;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->bps = 8;
    img->bit_depth = 8;
    img->metadata = NULL;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_image_t img;
    dummy_image_init(&img);

    // Use first 4 bytes for metadata type
    uint32_t type = *(const uint32_t*)Data;
    Data += 4;
    Size -= 4;

    // Use next byte for insert flag (modulo number of flags)
    if (Size < 1) return 0;
    aom_metadata_insert_flags_t insert_flag = (aom_metadata_insert_flags_t)(Data[0] % 19);
    Data += 1;
    Size -= 1;

    // Test aom_img_metadata_alloc
    aom_metadata_t *metadata = NULL;
    if (Size > 0) {
        metadata = aom_img_metadata_alloc(type, Data, Size, insert_flag);
    }

    // Test aom_img_add_metadata
    if (Size > 0) {
        int add_result = aom_img_add_metadata(&img, type, Data, Size, insert_flag);
        (void)add_result; // Ignore result for fuzzing
    }

    // Test aom_img_num_metadata
    size_t num_meta = aom_img_num_metadata(&img);
    (void)num_meta;

    // Test aom_img_get_metadata for valid indices
    for (size_t i = 0; i < num_meta; i++) {
        const aom_metadata_t *retrieved = aom_img_get_metadata(&img, i);
        (void)retrieved;
    }

    // Test aom_img_get_metadata with out-of-bounds index
    const aom_metadata_t *out_of_bounds = aom_img_get_metadata(&img, num_meta + 100);
    (void)out_of_bounds;

    // Test aom_img_remove_metadata
    aom_img_remove_metadata(&img);

    // Test aom_img_metadata_free
    if (metadata) {
        aom_img_metadata_free(metadata);
    }

    // Test with NULL image pointer for safety
    aom_img_remove_metadata(NULL);
    size_t null_num = aom_img_num_metadata(NULL);
    (void)null_num;
    const aom_metadata_t *null_retrieved = aom_img_get_metadata(NULL, 0);
    (void)null_retrieved;

    // Test aom_img_metadata_alloc with invalid inputs
    aom_metadata_t *null_metadata = aom_img_metadata_alloc(0, NULL, 0, AOM_MIF_ANY_FRAME);
    (void)null_metadata;
    if (Size > 0) {
        aom_metadata_t *zero_size_metadata = aom_img_metadata_alloc(type, Data, 0, insert_flag);
        (void)zero_size_metadata;
    }

    return 0;
}