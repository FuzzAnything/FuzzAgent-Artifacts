// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static void init_image(aom_image_t *img, aom_img_fmt_t fmt, unsigned int w, unsigned int h) {
    memset(img, 0, sizeof(aom_image_t));
    img->fmt = fmt;
    img->w = w;
    img->h = h;
    img->d_w = w;
    img->d_h = h;
    img->r_w = w;
    img->r_h = h;
    img->bit_depth = 8;
    img->cp = AOM_CICP_CP_UNSPECIFIED;
    img->tc = AOM_CICP_TC_UNSPECIFIED;
    img->mc = AOM_CICP_MC_UNSPECIFIED;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_COLOCATED;
    img->monochrome = 0;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->metadata = NULL;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from fuzz input
    uint32_t type = 0;
    uint32_t insert_flag_val = 0;
    uint32_t fmt_val = 0;
    uint32_t width = 16;
    uint32_t height = 16;
    uint32_t data_offset = 0;
    uint32_t data_size = 0;

    if (Size >= 4) {
        type = Data[0];
        insert_flag_val = Data[1];
        fmt_val = Data[2] & 0x0F; // Limit format to lower bits
        width = (Data[3] % 100) + 1;
    }
    if (Size >= 5) {
        height = (Data[4] % 100) + 1;
    }
    if (Size >= 8) {
        data_offset = (Data[5] * 256 + Data[6]) % (Size - 7);
        data_size = (Data[7] % (Size - data_offset));
    }

    // Ensure valid ranges for safety
    if (width == 0 || height == 0) { width = 16; height = 16; }
    if (data_size > Size) data_size = Size;
    if (data_offset + data_size > Size) data_size = Size - data_offset;

    const uint8_t *metadata_data = (data_size > 0) ? &Data[data_offset] : NULL;

    // Initialize image
    aom_image_t img;
    aom_img_fmt_t fmt = (aom_img_fmt_t)fmt_val;
    // Ensure format is valid for allocation logic if needed, though we mostly test metadata
    if (fmt == AOM_IMG_FMT_NONE) fmt = AOM_IMG_FMT_I420;
    
    init_image(&img, fmt, width, height);

    // Allocate planes to avoid null pointer issues in some internal checks if any
    // We don't strictly need valid plane data for metadata functions, but good practice
    size_t plane_size = width * height;
    img.planes[0] = malloc(plane_size);
    img.stride[0] = width;
    img.planes[1] = malloc(plane_size / 4);
    img.stride[1] = width / 2;
    img.planes[2] = malloc(plane_size / 4);
    img.stride[2] = width / 2;
    img.sz = plane_size * 1.5;

    if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
        // Cleanup on allocation failure
        free(img.planes[0]); free(img.planes[1]); free(img.planes[2]);
        return 0;
    }

    // 1. Test aom_img_metadata_alloc
    aom_metadata_insert_flags_t insert_flag = (aom_metadata_insert_flags_t)insert_flag_val;
    aom_metadata_t *meta_alloc = aom_img_metadata_alloc(type, metadata_data, data_size, insert_flag);

    if (meta_alloc) {
        // Test aom_img_metadata_free immediately on the allocated one
        aom_img_metadata_free(meta_alloc);
        meta_alloc = NULL;
    }

    // 2. Test aom_img_add_metadata
    // Add multiple metadata entries to test array handling
    int add_count = 0;
    
    // Determine safe iteration limit based on available data size
    // We need Data[8 + i] and Data[9 + i] to be valid
    size_t max_iter = 0;
    if (Size > 8) {
        max_iter = Size - 8;
    }
    // Cap iterations to prevent excessive work
    if (max_iter > 5) max_iter = 5;

    for (size_t i = 0; i < max_iter && add_count < 5; i++) {
        // Safe access: 8 + i is guaranteed < Size because i < Size - 8
        uint32_t t = Data[8 + i] % 100;
        
        uint32_t sz = 0;
        // Check if 9 + i is within bounds
        if (9 + i < Size) {
            sz = (Data[9 + i] % 50) + 1;
        } else {
            sz = 1; // Fallback if we hit the edge
        }

        // Ensure sz does not exceed the total Size to avoid reading past buffer
        if (sz > Size) sz = Size;
        
        // Ensure we don't pass a NULL pointer if sz > 0
        const uint8_t *safe_data = (sz > 0) ? Data : NULL;
        
        int ret = aom_img_add_metadata(&img, t, safe_data, sz, insert_flag);
        if (ret == 0) add_count++;
    }

    // 3. Test aom_img_num_metadata
    size_t num_meta = aom_img_num_metadata(&img);
    (void)num_meta; // Use variable to avoid unused warning

    // 4. Test aom_img_get_metadata
    // Iterate through valid indices and try out-of-bounds
    for (size_t i = 0; i <= num_meta + 1; i++) {
        const aom_metadata_t *meta = aom_img_get_metadata(&img, i);
        // No action needed, just ensuring it doesn't crash
        (void)meta;
    }

    // 5. Test aom_img_remove_metadata
    aom_img_remove_metadata(&img);

    // Cleanup image planes
    free(img.planes[0]);
    free(img.planes[1]);
    free(img.planes[2]);

    return 0;
}