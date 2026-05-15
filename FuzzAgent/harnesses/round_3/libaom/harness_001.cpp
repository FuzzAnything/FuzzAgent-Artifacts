#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_image.h"

namespace harness_001 {
/*
 * Fuzzing harness for libaom image metadata operations
 * Targets metadata manipulation APIs with high undiscovered branch counts
 * Based on coverage guidance: aom_img_add_metadata (28), aom_img_get_metadata (6),
 * aom_img_num_metadata (4), aom_img_wrap (16), aom_img_metadata_alloc (8),
 * aom_img_metadata_free (4)
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for meaningful fuzzing
    // Minimum: image dimensions (2*4 bytes) + format (1) + stride_align (1) + 
    // metadata type (4) + metadata size (1) + insert_flag (1) = ~12 bytes
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume image parameters from fuzzer input
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(1, 512);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(1, 512);
    
    // Select image format from valid options
    aom_img_fmt_t format = static_cast<aom_img_fmt_t>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 8));
    
    // Ensure stride alignment is power of 2 (required by aom_img_wrap)
    unsigned int stride_align = 1;
    uint8_t align_power = fdp.ConsumeIntegralInRange<uint8_t>(0, 8);
    stride_align = 1 << align_power;
    if (stride_align == 0) stride_align = 1;

    // Calculate buffer size needed for the image
    // Simplified calculation based on format
    size_t buffer_size = 0;
    switch (format) {
        case AOM_IMG_FMT_I420:
        case AOM_IMG_FMT_YV12:
            buffer_size = width * height + 2 * ((width + 1) / 2) * ((height + 1) / 2);
            break;
        case AOM_IMG_FMT_I422:
            buffer_size = width * height + 2 * ((width + 1) / 2) * height;
            break;
        case AOM_IMG_FMT_I444:
            buffer_size = 3 * width * height;
            break;
        case AOM_IMG_FMT_NV12:
            buffer_size = width * height + ((width + 1) / 2) * ((height + 1) / 2) * 2;
            break;
        default:
            // Use I420 as default for unknown formats
            format = AOM_IMG_FMT_I420;
            buffer_size = width * height + 2 * ((width + 1) / 2) * ((height + 1) / 2);
            break;
    }

    // Check if we have enough data for the image buffer
    if (fdp.remaining_bytes() < buffer_size + 16) {
        return 0;
    }

    // Allocate image buffer
    std::vector<unsigned char> image_buffer = fdp.ConsumeBytes<unsigned char>(buffer_size);
    if (image_buffer.size() < buffer_size) {
        image_buffer.resize(buffer_size, 0);
    }

    // Step 1: Create image using aom_img_wrap
    aom_image_t img;
    aom_image_t* wrapped_img = aom_img_wrap(&img, format, width, height, stride_align, image_buffer.data());
    if (wrapped_img == nullptr) {
        // aom_img_wrap failed, likely due to invalid parameters
        return 0;
    }

    // Determine how many metadata entries to add (0-5)
    size_t num_metadata_entries = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    std::vector<aom_metadata_t*> metadata_ptrs;

    for (size_t i = 0; i < num_metadata_entries && fdp.remaining_bytes() > 10; i++) {
        // Step 2: Allocate metadata using aom_img_metadata_alloc
        uint32_t metadata_type = fdp.ConsumeIntegral<uint32_t>();
        
        // Metadata size (0-1024 bytes)
        size_t metadata_size = fdp.ConsumeIntegralInRange<size_t>(0, std::min<size_t>(1024, fdp.remaining_bytes()));
        std::vector<uint8_t> metadata_data = fdp.ConsumeBytes<uint8_t>(metadata_size);
        
        // Select insert flag
        aom_metadata_insert_flags_t insert_flag = static_cast<aom_metadata_insert_flags_t>(
            fdp.ConsumeIntegralInRange<uint8_t>(0, 5));
        
        // Normalize insert flag to valid values
        switch (insert_flag) {
            case 0: insert_flag = AOM_MIF_NON_KEY_FRAME; break;
            case 1: insert_flag = AOM_MIF_KEY_FRAME; break;
            case 2: insert_flag = AOM_MIF_ANY_FRAME; break;
            case 3: insert_flag = AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC; break;
            case 4: insert_flag = AOM_MIF_KEY_FRAME_LAYER_SPECIFIC; break;
            case 5: insert_flag = AOM_MIF_ANY_FRAME_LAYER_SPECIFIC; break;
            default: insert_flag = AOM_MIF_ANY_FRAME; break;
        }

        // Allocate metadata
        aom_metadata_t* metadata = aom_img_metadata_alloc(
            metadata_type, 
            metadata_data.empty() ? nullptr : metadata_data.data(), 
            metadata_data.size(), 
            insert_flag
        );
        
        if (metadata != nullptr) {
            metadata_ptrs.push_back(metadata);
            
            // Step 3: Add metadata to image using aom_img_add_metadata
            // Note: aom_img_add_metadata makes a copy, so we need to pass the data directly
            int add_result = aom_img_add_metadata(
                wrapped_img,
                metadata_type,
                metadata_data.empty() ? nullptr : metadata_data.data(),
                metadata_data.size(),
                insert_flag
            );
            
            // Result can be -1 (error) or 0 (success), both are interesting for fuzzing
            (void)add_result; // Ignore result for fuzzing purposes
        }
    }

    // Step 4: Get number of metadata entries using aom_img_num_metadata
    size_t actual_num_metadata = aom_img_num_metadata(wrapped_img);
    
    // Step 5: Retrieve and validate metadata using aom_img_get_metadata
    for (size_t i = 0; i < actual_num_metadata && i < 10; i++) {
        const aom_metadata_t* retrieved_metadata = aom_img_get_metadata(wrapped_img, i);
        if (retrieved_metadata != nullptr) {
            // Metadata retrieved successfully
            // Could validate type, size, etc., but just accessing is enough for coverage
            (void)retrieved_metadata->type;
            (void)retrieved_metadata->sz;
            (void)retrieved_metadata->insert_flag;
        }
    }

    // Step 6: Free allocated metadata structures
    for (aom_metadata_t* metadata : metadata_ptrs) {
        aom_img_metadata_free(metadata);
    }

    // Step 7: Free the image (aom_img_free doesn't free the buffer we allocated)
    // Note: aom_img_wrap doesn't allocate memory, so aom_img_free only cleans up
    // the descriptor, not the buffer
    aom_img_free(wrapped_img);

    return 0;
}

} // namespace harness_001
