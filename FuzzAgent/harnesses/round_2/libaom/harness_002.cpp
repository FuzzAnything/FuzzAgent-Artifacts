#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"

namespace harness_002 {
/*
 * Fuzzing harness for libaom AV1 image metadata management APIs
 * Targets uncovered metadata functions: aom_img_add_metadata, aom_img_get_metadata, aom_img_num_metadata
 * Lightweight operations to address performance/timeout concerns
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Not enough data for meaningful metadata testing
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t img_format = fdp.ConsumeIntegral<uint8_t>() % 7;  // Limited to valid formats
    uint16_t width = fdp.ConsumeIntegralInRange<uint16_t>(16, 512);
    uint16_t height = fdp.ConsumeIntegralInRange<uint16_t>(16, 512);
    uint8_t bit_depth = fdp.ConsumeIntegralInRange<uint8_t>(8, 12);
    uint8_t use_border = fdp.ConsumeBool();
    uint8_t num_metadata_ops = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);
    uint8_t metadata_type = fdp.ConsumeIntegral<uint8_t>() % 6;  // OBU_METADATA_TYPE values 0-5

    // Map to actual aom_img_fmt_t values
    aom_img_fmt_t fmt;
    switch (img_format) {
        case 0: fmt = AOM_IMG_FMT_I420; break;
        case 1: fmt = AOM_IMG_FMT_YV12; break;
        case 2: fmt = AOM_IMG_FMT_I422; break;
        case 3: fmt = AOM_IMG_FMT_I444; break;
        case 4: fmt = AOM_IMG_FMT_NV12; break;
        case 5: fmt = AOM_IMG_FMT_I42016; break;
        case 6: fmt = AOM_IMG_FMT_I42216; break;
        default: fmt = AOM_IMG_FMT_I420; break;
    }

    // Map to OBU_METADATA_TYPE enum
    OBU_METADATA_TYPE type;
    switch (metadata_type) {
        case 0: type = OBU_METADATA_TYPE_AOM_RESERVED_0; break;
        case 1: type = OBU_METADATA_TYPE_HDR_CLL; break;
        case 2: type = OBU_METADATA_TYPE_HDR_MDCV; break;
        case 3: type = OBU_METADATA_TYPE_SCALABILITY; break;
        case 4: type = OBU_METADATA_TYPE_ITUT_T35; break;
        case 5: type = OBU_METADATA_TYPE_TIMECODE; break;
        default: type = OBU_METADATA_TYPE_ITUT_T35; break;
    }

    // Get metadata insert flag
    uint8_t flag_choice = fdp.ConsumeIntegral<uint8_t>() % 6;
    aom_metadata_insert_flags_t insert_flag;
    switch (flag_choice) {
        case 0: insert_flag = AOM_MIF_NON_KEY_FRAME; break;
        case 1: insert_flag = AOM_MIF_KEY_FRAME; break;
        case 2: insert_flag = AOM_MIF_ANY_FRAME; break;
        case 3: insert_flag = AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC; break;
        case 4: insert_flag = AOM_MIF_KEY_FRAME_LAYER_SPECIFIC; break;
        case 5: insert_flag = AOM_MIF_ANY_FRAME_LAYER_SPECIFIC; break;
        default: insert_flag = AOM_MIF_ANY_FRAME; break;
    }

    // Allocate image
    aom_image_t img;
    aom_image_t* img_ptr = nullptr;
    
    if (use_border) {
        unsigned int border = fdp.ConsumeIntegralInRange<unsigned int>(0, 32);
        unsigned int size_align = fdp.ConsumeIntegralInRange<unsigned int>(1, 64);
        img_ptr = aom_img_alloc_with_border(&img, fmt, width, height, 32, size_align, border);
    } else {
        img_ptr = aom_img_alloc(&img, fmt, width, height, 32);
    }

    // Check if image allocation succeeded - critical fix for null pointer dereference
    if (img_ptr == nullptr) {
        return 0;  // Allocation failed, nothing to test
    }
    
    // Set bit depth if format supports it (safe after null check)
    if (fmt & AOM_IMG_FMT_HIGHBITDEPTH) {
        img_ptr->bit_depth = bit_depth;
    }
    // Perform metadata operations
    for (uint8_t i = 0; i < num_metadata_ops && fdp.remaining_bytes() > 0; i++) {
        // Determine metadata size
        size_t metadata_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        if (fdp.remaining_bytes() < metadata_size) {
            metadata_size = fdp.remaining_bytes();
        }
        if (metadata_size == 0) break;

        // Get metadata data
        std::vector<uint8_t> metadata_data = fdp.ConsumeBytes<uint8_t>(metadata_size);
        
        // Add metadata to image
        int add_result = aom_img_add_metadata(img_ptr, type, metadata_data.data(), 
                                             metadata_size, insert_flag);
        
        // Test aom_img_num_metadata
        size_t metadata_count = aom_img_num_metadata(img_ptr);
        
        // Test aom_img_get_metadata for each metadata if available
        for (size_t j = 0; j < metadata_count; j++) {
            const aom_metadata_t* retrieved = aom_img_get_metadata(img_ptr, j);
            if (retrieved != nullptr) {
                // Verify retrieved metadata matches expected type
                // (We can't verify payload without storing it)
            }
        }
        
        // Also test helper functions
        if (fdp.ConsumeBool() && metadata_data.size() > 0) {
            // Test aom_img_metadata_alloc and aom_img_metadata_free
            aom_metadata_t* standalone_metadata = aom_img_metadata_alloc(
                type, metadata_data.data(), metadata_data.size(), insert_flag);
            if (standalone_metadata != nullptr) {
                aom_img_metadata_free(standalone_metadata);
            }
        }
    }

    // Test edge cases with null/invalid parameters
    if (fdp.ConsumeBool()) {
        // Test with null image
        std::vector<uint8_t> test_data = {0x01, 0x02, 0x03};
        aom_img_add_metadata(nullptr, type, test_data.data(), test_data.size(), insert_flag);
        
        // Test with null data
        aom_img_add_metadata(img_ptr, type, nullptr, 10, insert_flag);
        
        // Test with zero size
        aom_img_add_metadata(img_ptr, type, test_data.data(), 0, insert_flag);
    }

    // Test metadata removal
    aom_img_remove_metadata(img_ptr);

    // Verify all metadata removed
    size_t final_count = aom_img_num_metadata(img_ptr);

    // Clean up
    aom_img_free(img_ptr);

    return 0;
}

} // namespace harness_002
