// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
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
    // No global cleanup needed for this fuzzer
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Initialize a dummy aom_image_t structure
    static aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Set some basic fields to make the image descriptor somewhat valid
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
    img.monochrome = 0;
    img.cp = AOM_CICP_CP_BT_709;
    img.tc = AOM_CICP_TC_BT_709;
    img.mc = AOM_CICP_MC_BT_709;
    img.csp = AOM_CSP_VERTICAL;
    img.range = AOM_CR_STUDIO_RANGE;
    
    // Initialize metadata array to NULL
    img.metadata = NULL;
    
    // Test aom_img_num_metadata with various states
    size_t num_meta = aom_img_num_metadata(&img);
    (void)num_meta; // Use result to avoid unused variable warning
    
    // Test with NULL image
    aom_img_num_metadata(NULL);
    
    // Test with NULL metadata (already set above)
    aom_image_t img_no_meta = img;
    img_no_meta.metadata = NULL;
    aom_img_num_metadata(&img_no_meta);
    
    // Test aom_img_metadata_free with various inputs
    if (Size > 0) {
        // Create a metadata struct using aom_img_metadata_alloc
        uint32_t type = 1;
        size_t data_size = Size > 100 ? 100 : Size;
        aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
        
        aom_metadata_t *metadata = aom_img_metadata_alloc(type, Data, data_size, insert_flag);
        
        if (metadata) {
            // Test aom_img_metadata_free
            aom_img_metadata_free(metadata);
            
            // Test with NULL
            aom_img_metadata_free(NULL);
            
            // Test adding metadata to image
            int add_result = aom_img_add_metadata(&img, type, Data, data_size, insert_flag);
            (void)add_result;
            
            // Test aom_img_get_metadata
            size_t current_meta_count = aom_img_num_metadata(&img);
            if (current_meta_count > 0) {
                const aom_metadata_t *retrieved = aom_img_get_metadata(&img, 0);
                (void)retrieved;
                
                // Test out of bounds access
                aom_img_get_metadata(&img, current_meta_count);
                aom_img_get_metadata(&img, (size_t)-1);
            }
            
            // Test with NULL image
            aom_img_get_metadata(NULL, 0);
            
            // Test aom_img_remove_metadata
            aom_img_remove_metadata(&img);
            
            // Test remove on already NULL metadata
            aom_img_remove_metadata(&img_no_meta);
            
            // Test remove on NULL image (should be safe)
            aom_img_remove_metadata(NULL);
        }
        
        // Test aom_img_metadata_alloc with edge cases
        // Test with NULL data
        aom_img_metadata_alloc(type, NULL, data_size, insert_flag);
        
        // Test with zero size
        aom_img_metadata_alloc(type, Data, 0, insert_flag);
        
        // Test aom_img_add_metadata with edge cases
        // Test with NULL image
        aom_img_add_metadata(NULL, type, Data, data_size, insert_flag);
        
        // Test with NULL data
        aom_img_add_metadata(&img, type, NULL, data_size, insert_flag);
        
        // Test with zero size
        aom_img_add_metadata(&img, type, Data, 0, insert_flag);
        
        // Test with different insert flags
        aom_metadata_insert_flags_t flags[] = {
            AOM_MIF_NON_KEY_FRAME,
            AOM_MIF_KEY_FRAME,
            AOM_MIF_ANY_FRAME,
            AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC,
            AOM_MIF_KEY_FRAME_LAYER_SPECIFIC,
            AOM_MIF_ANY_FRAME_LAYER_SPECIFIC
        };
        
        for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]) && i < 6; i++) {
            // Add some metadata with different flags
            if (Size > i + 10) {
                aom_img_add_metadata(&img, i, Data + i, Size - i, flags[i]);
            }
            
            // Test aom_img_metadata_alloc with different flags
            if (Size > i + 20) {
                aom_metadata_t *meta = aom_img_metadata_alloc(i, Data + i + 10, 
                                                             Size > i + 30 ? 20 : Size - i - 10, 
                                                             flags[i]);
                if (meta) {
                    aom_img_metadata_free(meta);
                }
            }
        }
        
        // Test aom_img_num_metadata after adding metadata
        size_t final_count = aom_img_num_metadata(&img);
        (void)final_count;
        
        // Test aom_img_get_metadata with various indices
        size_t meta_count = aom_img_num_metadata(&img);
        for (size_t i = 0; i < meta_count; i++) {
            const aom_metadata_t *meta = aom_img_get_metadata(&img, i);
            (void)meta;
        }
        
        // Clean up any remaining metadata
        aom_img_remove_metadata(&img);
    }
    
    dummy_cleanup();
    return 0;
}