#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "aom/aom_image.h"
#include "aom/aom_codec.h"

namespace harness_002 {
/*
 * Fuzzing harness for libaom image metadata management API cluster
 * Targets comprehensive metadata lifecycle: allocation, addition, retrieval,
 * counting, removal, and cleanup of image metadata.
 * Focuses on aom_image.c metadata APIs which currently have 50+ undiscovered branches.
 */




int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for basic image and metadata configuration
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume image configuration parameters from fuzzer input
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 1024);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 1024);
    
    // Consume image format - select from valid formats using modulo
    const aom_img_fmt_t formats[] = {
        AOM_IMG_FMT_YV12,
        AOM_IMG_FMT_I420,
        AOM_IMG_FMT_I422,
        AOM_IMG_FMT_I444,
        AOM_IMG_FMT_NV12,
        AOM_IMG_FMT_I42016,
        AOM_IMG_FMT_YV1216,
        AOM_IMG_FMT_I42216,
        AOM_IMG_FMT_I44416
    };
    const size_t num_formats = sizeof(formats) / sizeof(formats[0]);
    size_t format_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_formats - 1);
    aom_img_fmt_t format = formats[format_idx];
    
    // Consume alignment parameter
    unsigned int align = fdp.ConsumeIntegralInRange<unsigned int>(1, 32);
    
    // Step 1: Initialize image using aom_img_alloc (dependency already covered)
    aom_image_t img;
    if (!aom_img_alloc(&img, format, width, height, align)) {
        return 0;
    }
    
    // Consume metadata operation parameters
    unsigned int num_metadata_entries = fdp.ConsumeIntegralInRange<unsigned int>(0, 16);
    
    // Create vector to store allocated metadata for later cleanup
    std::vector<aom_metadata_t*> allocated_metadata;
    
    // Step 2: Add metadata entries to the image
    for (unsigned int i = 0; i < num_metadata_entries && fdp.remaining_bytes() > 16; i++) {
        // Consume metadata type - use both defined types and arbitrary values
        uint32_t metadata_type;
        if (fdp.ConsumeBool()) {
            // Use defined OBU_METADATA_TYPE values
            const uint32_t defined_types[] = {
                0,  // OBU_METADATA_TYPE_AOM_RESERVED_0
                1,  // OBU_METADATA_TYPE_HDR_CLL
                2,  // OBU_METADATA_TYPE_HDR_MDCV
                3,  // OBU_METADATA_TYPE_SCALABILITY
                4,  // OBU_METADATA_TYPE_ITUT_T35
                5   // OBU_METADATA_TYPE_TIMECODE
            };
            const size_t num_types = sizeof(defined_types) / sizeof(defined_types[0]);
            size_t type_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_types - 1);
            metadata_type = defined_types[type_idx];
        } else {
            // Use arbitrary uint32_t value
            metadata_type = fdp.ConsumeIntegral<uint32_t>();
        }
        
        // Consume metadata insert flags using modulo
        const aom_metadata_insert_flags_t insert_flags[] = {
            AOM_MIF_NON_KEY_FRAME,
            AOM_MIF_KEY_FRAME,
            AOM_MIF_ANY_FRAME,
            static_cast<aom_metadata_insert_flags_t>(AOM_MIF_NON_KEY_FRAME | (1 << 2)), // LAYER_SPECIFIC variants
            static_cast<aom_metadata_insert_flags_t>(AOM_MIF_KEY_FRAME | (1 << 2)),
            static_cast<aom_metadata_insert_flags_t>(AOM_MIF_ANY_FRAME | (1 << 2))
        };
        const size_t num_insert_flags = sizeof(insert_flags) / sizeof(insert_flags[0]);
        size_t flag_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_insert_flags - 1);
        aom_metadata_insert_flags_t insert_flag = insert_flags[flag_idx];
        
        // Consume metadata payload size and data - allow zero size to test edge case
        size_t payload_size = fdp.ConsumeIntegralInRange<size_t>(0, 4096);
        if (payload_size > 0 && fdp.remaining_bytes() < payload_size) {
            break; // Not enough data for payload
        }
        
        std::vector<uint8_t> payload;
        if (payload_size > 0) {
            payload = fdp.ConsumeBytes<uint8_t>(payload_size);
        }
        
        // Step 2a: Allocate metadata structure using aom_img_metadata_alloc
        aom_metadata_t *metadata = aom_img_metadata_alloc(
            metadata_type, 
            payload_size > 0 ? payload.data() : NULL, 
            payload_size, 
            insert_flag
        );
        
        if (metadata == NULL) {
            // Allocation failed, skip this entry
            continue;
        }
        
        // Store for potential later use
        allocated_metadata.push_back(metadata);
        
        // Step 2b: Add metadata to image using aom_img_add_metadata
        int add_result = aom_img_add_metadata(
            &img,
            metadata_type,
            payload_size > 0 ? payload.data() : NULL,
            payload_size,
            insert_flag
        );
        
        // We don't check add_result as it may fail due to various reasons
        // (invalid parameters, memory allocation failure, etc.) which is what fuzzing tests
    }
    // Step 3: Get number of metadata entries using aom_img_num_metadata
    size_t actual_num_metadata = aom_img_num_metadata(&img);
    
    // Step 4: Retrieve and inspect metadata entries if any exist
    if (actual_num_metadata > 0 && fdp.remaining_bytes() > 0) {
        // Consume random indices to test retrieval
        unsigned int num_retrievals = fdp.ConsumeIntegralInRange<unsigned int>(0, actual_num_metadata);
        
        for (unsigned int i = 0; i < num_retrievals && fdp.remaining_bytes() > 0; i++) {
            size_t index = fdp.ConsumeIntegralInRange<size_t>(0, actual_num_metadata - 1);
            
            // Step 4a: Get metadata using aom_img_get_metadata
            const aom_metadata_t *retrieved_metadata = aom_img_get_metadata(&img, index);
            
            if (retrieved_metadata != NULL) {
                // Metadata retrieved successfully, we can examine its fields
                // but we don't need to do anything with them for fuzzing
                (void)retrieved_metadata->type;
                (void)retrieved_metadata->sz;
                (void)retrieved_metadata->insert_flag;
            }
        }
        
        // Test removal of some metadata if we have multiple entries
        if (actual_num_metadata > 1 && fdp.ConsumeBool()) {
            // Step 5: Remove all metadata using aom_img_remove_metadata
            aom_img_remove_metadata(&img);
            
            // Verify removal by checking count
            size_t after_removal_count = aom_img_num_metadata(&img);
            // after_removal_count should be 0, but we don't assert in fuzzer
        }
    }
    
    // Step 6: Free any standalone metadata we allocated but didn't add
    for (aom_metadata_t *metadata : allocated_metadata) {
        // Step 6a: Free metadata using aom_img_metadata_free
        aom_img_metadata_free(metadata);
    }
    
    // Step 7: Clean up image using aom_img_free
    aom_img_free(&img);
    
    return 0;
}

} // namespace harness_002
