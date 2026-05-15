// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
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

static aom_image_t *create_dummy_image(void) {
    static aom_image_t img = {0};
    
    // Initialize with some default values
    img.fmt = AOM_IMG_FMT_I420;
    img.cp = AOM_CICP_CP_BT_709;
    img.tc = AOM_CICP_TC_BT_709;
    img.mc = AOM_CICP_MC_BT_709;
    img.monochrome = 0;
    img.csp = AOM_CSP_VERTICAL;
    img.range = AOM_CR_STUDIO_RANGE;
    img.w = 64;
    img.h = 64;
    img.bit_depth = 8;
    img.d_w = 64;
    img.d_h = 64;
    img.r_w = 64;
    img.r_h = 64;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    img.bps = 12;
    img.temporal_id = 0;
    img.spatial_id = 0;
    
    // Allocate dummy plane data
    for (int i = 0; i < 3; i++) {
        img.stride[i] = 64;
        img.planes[i] = malloc(64 * 64);
        if (img.planes[i]) {
            memset(img.planes[i], 0, 64 * 64);
        }
    }
    
    img.sz = 64 * 64 * 3;
    img.img_data_owner = 1;
    img.self_allocd = 1;
    
    return &img;
}

static void cleanup_dummy_image(aom_image_t *img) {
    if (!img) return;
    
    for (int i = 0; i < 3; i++) {
        if (img->planes[i]) {
            free(img->planes[i]);
            img->planes[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy image
    aom_image_t *img = create_dummy_image();
    if (!img) return 0;
    
    // Test aom_img_num_metadata with NULL
    size_t num1 = aom_img_num_metadata(NULL);
    
    // Test aom_img_num_metadata with empty image
    size_t num2 = aom_img_num_metadata(img);
    
    // Test aom_img_metadata_free with NULL
    aom_img_metadata_free(NULL);
    
    // Test aom_img_remove_metadata with NULL
    aom_img_remove_metadata(NULL);
    
    // Test aom_img_remove_metadata with empty image
    aom_img_remove_metadata(img);
    
    // Test aom_img_get_metadata with NULL image
    const aom_metadata_t *meta1 = aom_img_get_metadata(NULL, 0);
    
    // Test aom_img_get_metadata with out of bounds index
    const aom_metadata_t *meta2 = aom_img_get_metadata(img, 100);
    
    // Try to add metadata if we have enough data
    if (Size >= 5) {
        uint32_t type = *(const uint32_t *)Data;
        size_t sz = Data[4] % 256;  // Limit size to prevent excessive allocation
        if (sz > Size - 5) sz = Size - 5;
        
        const uint8_t *data = Data + 5;
        uint8_t flag_byte = Data[4] % 19;  // There are 19 flag values
        
        // Test aom_img_add_metadata with various flags
        int result = aom_img_add_metadata(img, type, data, sz, 
                                         (aom_metadata_insert_flags_t)flag_byte);
        
        // Test aom_img_num_metadata after adding
        size_t num3 = aom_img_num_metadata(img);
        
        // Test aom_img_get_metadata if we have metadata
        if (num3 > 0) {
            size_t index = Data[0] % num3;
            const aom_metadata_t *meta3 = aom_img_get_metadata(img, index);
            
            // Test aom_img_metadata_alloc
            aom_metadata_t *allocated_meta = aom_img_metadata_alloc(
                type, data, sz, (aom_metadata_insert_flags_t)flag_byte);
            
            if (allocated_meta) {
                // Test aom_img_metadata_free with allocated metadata
                aom_img_metadata_free(allocated_meta);
            }
        }
        
        // Test aom_img_remove_metadata after adding
        aom_img_remove_metadata(img);
        
        // Test aom_img_add_metadata with NULL data
        int result2 = aom_img_add_metadata(img, type, NULL, sz, 
                                          AOM_MIF_ANY_FRAME);
        
        // Test aom_img_add_metadata with zero size
        int result3 = aom_img_add_metadata(img, type, data, 0, 
                                          AOM_MIF_ANY_FRAME);
        
        // Test aom_img_add_metadata with NULL image
        int result4 = aom_img_add_metadata(NULL, type, data, sz, 
                                          AOM_MIF_ANY_FRAME);
    }
    
    // Test aom_img_metadata_alloc with various inputs
    if (Size >= 2) {
        uint32_t type = *(const uint32_t *)Data;
        size_t sz = Data[1] % 256;
        if (sz > Size - 2) sz = Size - 2;
        const uint8_t *data = Size >= 2 + sz ? Data + 2 : NULL;
        uint8_t flag_byte = Data[1] % 19;
        
        // Test with valid data
        aom_metadata_t *meta4 = aom_img_metadata_alloc(
            type, data, sz, (aom_metadata_insert_flags_t)flag_byte);
        
        if (meta4) {
            aom_img_metadata_free(meta4);
        }
        
        // Test with NULL data
        aom_metadata_t *meta5 = aom_img_metadata_alloc(
            type, NULL, sz, (aom_metadata_insert_flags_t)flag_byte);
        
        if (meta5) {
            aom_img_metadata_free(meta5);
        }
        
        // Test with zero size
        aom_metadata_t *meta6 = aom_img_metadata_alloc(
            type, data, 0, (aom_metadata_insert_flags_t)flag_byte);
        
        if (meta6) {
            aom_img_metadata_free(meta6);
        }
    }
    
    // Clean up
    aom_img_remove_metadata(img);
    cleanup_dummy_image(img);
    
    return 0;
}