// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static aom_image_t *create_test_image(const uint8_t *data, size_t size) {
    if (size < 4) return NULL;
    
    // Extract parameters from fuzzer data
    unsigned int width = (data[0] << 8) | data[1];
    unsigned int height = (data[2] << 8) | data[3];
    
    // Limit dimensions to reasonable values
    if (width == 0) width = 1;
    if (height == 0) height = 1;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Choose a format based on fuzzer input
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (size > 4) {
        switch (data[4] % 8) {
            case 0: fmt = AOM_IMG_FMT_I420; break;
            case 1: fmt = AOM_IMG_FMT_YV12; break;
            case 2: fmt = AOM_IMG_FMT_AOMI420; break;
            case 3: fmt = AOM_IMG_FMT_AOMYV12; break;
            case 4: fmt = AOM_IMG_FMT_I422; break;
            case 5: fmt = AOM_IMG_FMT_I444; break;
            case 6: fmt = AOM_IMG_FMT_NV12; break;
            case 7: fmt = AOM_IMG_FMT_I42016; break;
        }
    }
    
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, 16);
    if (!img) return NULL;
    
    // Set some reasonable defaults
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_VERTICAL;
    img->monochrome = 0;
    
    return img;
}

static void test_metadata_functions(aom_image_t *img, const uint8_t *data, size_t size) {
    if (!img || size < 4) return;
    
    size_t offset = 0;
    
    // Test aom_img_metadata_alloc with various inputs
    while (offset + 8 < size) {
        uint32_t type = (data[offset] << 24) | (data[offset + 1] << 16) | 
                       (data[offset + 2] << 8) | data[offset + 3];
        uint16_t sz = (data[offset + 4] << 8) | data[offset + 5];
        uint8_t flag_byte = data[offset + 6];
        
        // Limit metadata size to prevent excessive allocations
        if (sz > 1024) sz = 1024;
        
        // Ensure we have enough data for the payload
        size_t payload_offset = offset + 7;
        if (payload_offset + sz > size) {
            sz = size - payload_offset;
        }
        
        // Map flag_byte to valid insert flags
        aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
        switch (flag_byte % 6) {
            case 0: insert_flag = AOM_MIF_NON_KEY_FRAME; break;
            case 1: insert_flag = AOM_MIF_KEY_FRAME; break;
            case 2: insert_flag = AOM_MIF_ANY_FRAME; break;
            case 3: insert_flag = AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC; break;
            case 4: insert_flag = AOM_MIF_KEY_FRAME_LAYER_SPECIFIC; break;
            case 5: insert_flag = AOM_MIF_ANY_FRAME_LAYER_SPECIFIC; break;
        }
        
        // Test metadata allocation
        aom_metadata_t *metadata = aom_img_metadata_alloc(
            type, 
            payload_offset < size ? &data[payload_offset] : NULL,
            sz,
            insert_flag
        );
        
        if (metadata) {
            // Free the allocated metadata
            aom_img_metadata_free(metadata);
        }
        
        // Test adding metadata to image
        int add_result = aom_img_add_metadata(
            img,
            type,
            payload_offset < size ? &data[payload_offset] : NULL,
            sz,
            insert_flag
        );
        
        // Move offset for next iteration
        offset = payload_offset + sz;
        if (offset >= size) break;
        
        // Limit number of metadata operations
        if (aom_img_num_metadata(img) > 10) {
            break;
        }
    }
    
    // Test aom_img_num_metadata
    size_t num_metadata = aom_img_num_metadata(img);
    
    // Test aom_img_get_metadata with valid and invalid indices
    for (size_t i = 0; i < num_metadata + 2; i++) {
        const aom_metadata_t *md = aom_img_get_metadata(img, i);
        // Just call the function, don't need to use the result
        (void)md;
    }
    
    // Test aom_img_remove_metadata
    aom_img_remove_metadata(img);
    
    // Verify removal worked
    size_t after_removal = aom_img_num_metadata(img);
    (void)after_removal;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Test with NULL image pointer
    aom_img_get_metadata(NULL, 0);
    aom_img_num_metadata(NULL);
    aom_img_remove_metadata(NULL);
    
    // Test aom_img_metadata_free with NULL
    aom_img_metadata_free(NULL);
    
    // Test aom_img_metadata_alloc with invalid inputs
    aom_metadata_t *invalid_metadata = aom_img_metadata_alloc(0, NULL, 0, AOM_MIF_ANY_FRAME);
    if (invalid_metadata) {
        aom_img_metadata_free(invalid_metadata);
    }
    
    // Test aom_img_add_metadata with NULL image
    if (size > 4) {
        aom_img_add_metadata(NULL, 0, data, size > 1024 ? 1024 : size, AOM_MIF_ANY_FRAME);
    }
    
    // Create a test image
    aom_image_t *img = create_test_image(data, size);
    if (!img) return 0;
    
    // Test metadata functions with the created image
    test_metadata_functions(img, data, size);
    
    // Clean up
    aom_img_free(img);
    
    return 0;
}