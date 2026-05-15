// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_cleanup(void) {
    // Nothing specific needed for this fuzzer
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Initialize a dummy image structure
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Set some basic values to make the image descriptor valid
    img.fmt = AOM_IMG_FMT_I420;
    img.w = 64;
    img.h = 64;
    img.d_w = 64;
    img.d_h = 64;
    img.r_w = 64;
    img.r_h = 64;
    img.bit_depth = 8;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    img.metadata = NULL;
    
    // Initialize other required fields
    img.cp = AOM_CICP_CP_BT_709;
    img.tc = AOM_CICP_TC_BT_709;
    img.mc = AOM_CICP_MC_BT_709;
    img.monochrome = 0;
    img.csp = AOM_CSP_VERTICAL;
    img.range = AOM_CR_STUDIO_RANGE;
    
    // Allocate dummy plane data
    size_t y_size = img.w * img.h;
    size_t uv_size = (img.w >> img.x_chroma_shift) * (img.h >> img.y_chroma_shift);
    
    img.planes[0] = malloc(y_size);
    img.planes[1] = malloc(uv_size);
    img.planes[2] = malloc(uv_size);
    
    if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
        free(img.planes[0]);
        free(img.planes[1]);
        free(img.planes[2]);
        return 0;
    }
    
    img.stride[0] = img.w;
    img.stride[1] = img.w >> img.x_chroma_shift;
    img.stride[2] = img.w >> img.x_chroma_shift;
    img.sz = y_size + 2 * uv_size;
    img.bps = 8;
    img.temporal_id = 0;
    img.spatial_id = 0;
    img.user_priv = NULL;
    img.img_data = NULL;
    img.img_data_owner = 0;
    img.self_allocd = 0;
    img.fb_priv = NULL;
    
    // Use fuzzer data to generate parameters
    if (Size < 1) {
        // Cleanup and return if no data
        free(img.planes[0]);
        free(img.planes[1]);
        free(img.planes[2]);
        return 0;
    }
    
    // Extract parameters from fuzzer input
    uint32_t type = 0;
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
    const uint8_t *metadata_data = NULL;
    size_t metadata_sz = 0;
    
    // Use first byte for type (if available)
    if (Size >= 1) {
        type = Data[0];
    }
    
    // Use second byte for insert flag (if available)
    if (Size >= 2) {
        insert_flag = (aom_metadata_insert_flags_t)(Data[1] % 19); // 19 possible values in enum
    }
    
    // Use remaining bytes for metadata payload
    if (Size > 2) {
        metadata_data = Data + 2;
        metadata_sz = Size - 2;
    } else {
        metadata_data = Data;
        metadata_sz = Size;
    }
    
    // Test aom_img_add_metadata
    int add_result = aom_img_add_metadata(&img, type, metadata_data, metadata_sz, insert_flag);
    
    // Test aom_img_num_metadata
    size_t num_metadata = aom_img_num_metadata(&img);
    
    // Test aom_img_get_metadata for valid indices
    for (size_t i = 0; i < num_metadata && i < 10; i++) {
        const aom_metadata_t *metadata = aom_img_get_metadata(&img, i);
        if (metadata) {
            // Access metadata fields (read-only)
            uint32_t retrieved_type = metadata->type;
            size_t retrieved_sz = metadata->sz;
            aom_metadata_insert_flags_t retrieved_flag = metadata->insert_flag;
            
            // Do something with the values to avoid dead code elimination
            (void)retrieved_type;
            (void)retrieved_sz;
            (void)retrieved_flag;
        }
    }
    
    // Test aom_img_metadata_alloc
    aom_metadata_t *allocated_metadata = aom_img_metadata_alloc(type, metadata_data, metadata_sz, insert_flag);
    
    if (allocated_metadata) {
        // Test aom_img_metadata_free
        aom_img_metadata_free(allocated_metadata);
    }
    
    // Test aom_img_remove_metadata
    aom_img_remove_metadata(&img);
    
    // Verify metadata is removed
    size_t num_after_remove = aom_img_num_metadata(&img);
    
    // Clean up image planes
    free(img.planes[0]);
    free(img.planes[1]);
    free(img.planes[2]);
    
    // Call dummy cleanup
    dummy_cleanup();
    
    // Use the results to avoid dead code elimination
    (void)add_result;
    (void)num_metadata;
    (void)num_after_remove;
    
    return 0;
}