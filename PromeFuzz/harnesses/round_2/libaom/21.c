// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_image_init(aom_image_t *img) {
    memset(img, 0, sizeof(*img));
    img->fmt = AOM_IMG_FMT_I420;
    img->w = 64;
    img->h = 64;
    img->d_w = 64;
    img->d_h = 64;
    img->r_w = 64;
    img->r_h = 64;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_VERTICAL;
    img->monochrome = 0;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->metadata = NULL;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_image_t img;
    dummy_image_init(&img);

    // Use first byte to determine number of metadata operations
    size_t num_ops = (Data[0] % 16) + 1;
    size_t data_offset = 1;

    // Add metadata to image
    for (size_t i = 0; i < num_ops && data_offset + 8 <= Size; i++) {
        uint32_t type = 0;
        size_t sz = 0;
        aom_metadata_insert_flags_t flag = AOM_MIF_ANY_FRAME;

        // Extract type (4 bytes if available)
        if (data_offset + 4 <= Size) {
            memcpy(&type, Data + data_offset, 4);
            data_offset += 4;
        }

        // Extract size (1-4 bytes depending on remaining data)
        if (data_offset < Size) {
            sz = Data[data_offset] % 256;
            data_offset++;
        }

        // Extract flag (1 byte if available)
        if (data_offset < Size) {
            uint8_t flag_val = Data[data_offset] % 19;
            flag = (aom_metadata_insert_flags_t)flag_val;
            data_offset++;
        }

        // Prepare metadata data
        const uint8_t *metadata_data = NULL;
        size_t actual_sz = 0;
        if (sz > 0 && data_offset < Size) {
            actual_sz = (Size - data_offset < sz) ? (Size - data_offset) : sz;
            metadata_data = Data + data_offset;
            data_offset += actual_sz;
        } else if (sz > 0) {
            // No data available, use dummy
            actual_sz = 1;
            metadata_data = Data;
        }

        if (actual_sz > 0 && metadata_data != NULL) {
            aom_img_add_metadata(&img, type, metadata_data, actual_sz, flag);
        }
    }

    // Test aom_img_num_metadata
    size_t num_metadata = aom_img_num_metadata(&img);

    // Test aom_img_get_metadata for valid indices
    for (size_t i = 0; i < num_metadata; i++) {
        const aom_metadata_t *meta = aom_img_get_metadata(&img, i);
        if (meta) {
            // Access fields (read-only)
            uint32_t type = meta->type;
            size_t sz = meta->sz;
            aom_metadata_insert_flags_t flag = meta->insert_flag;
            (void)type;
            (void)sz;
            (void)flag;
        }
    }

    // Test aom_img_get_metadata with out-of-bounds index
    if (num_metadata > 0) {
        aom_img_get_metadata(&img, num_metadata); // Should return NULL
        aom_img_get_metadata(&img, num_metadata + 100); // Should return NULL
    }

    // Test aom_img_metadata_alloc with remaining data
    if (data_offset + 8 <= Size) {
        uint32_t type = 0;
        size_t sz = 0;
        aom_metadata_insert_flags_t flag = AOM_MIF_ANY_FRAME;

        memcpy(&type, Data + data_offset, 4);
        data_offset += 4;

        if (data_offset < Size) {
            sz = Data[data_offset] % 256;
            data_offset++;
        }

        if (data_offset < Size) {
            uint8_t flag_val = Data[data_offset] % 19;
            flag = (aom_metadata_insert_flags_t)flag_val;
            data_offset++;
        }

        const uint8_t *alloc_data = NULL;
        size_t alloc_sz = 0;
        if (sz > 0 && data_offset < Size) {
            alloc_sz = (Size - data_offset < sz) ? (Size - data_offset) : sz;
            alloc_data = Data + data_offset;
        }

        if (alloc_sz > 0 && alloc_data != NULL) {
            aom_metadata_t *meta = aom_img_metadata_alloc(type, alloc_data, alloc_sz, flag);
            if (meta) {
                aom_img_metadata_free(meta);
            }
        }
    }

    // Test aom_img_metadata_alloc with NULL data
    aom_img_metadata_alloc(0, NULL, 0, AOM_MIF_ANY_FRAME);

    // Test aom_img_metadata_free with NULL
    aom_img_metadata_free(NULL);

    // Test aom_img_remove_metadata
    aom_img_remove_metadata(&img);

    // Test aom_img_num_metadata after removal
    num_metadata = aom_img_num_metadata(&img);

    // Test aom_img_remove_metadata again (should be safe)
    aom_img_remove_metadata(&img);

    // Test with NULL image pointer
    aom_img_num_metadata(NULL);
    aom_img_remove_metadata(NULL);
    aom_img_get_metadata(NULL, 0);

    // Test aom_img_add_metadata with NULL image
    if (Size > 0) {
        aom_img_add_metadata(NULL, 0, Data, Size > 100 ? 100 : Size, AOM_MIF_ANY_FRAME);
    }

    return 0;
}