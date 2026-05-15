#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <memory>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom_image.h"

namespace harness_009 {
/*
 * Fuzzing harness for libaom AV1 comprehensive image buffer testing
 * Targets: aom_img_alloc_with_border and aom_img_wrap (32 branches each, 13 undiscovered)
 * Also covers: aom_img_plane_height, aom_img_plane_width (2 branches each, 2 undiscovered), aom_img_flip (0 branches)
 * Focus: Simplified, focused testing of image buffer lifecycle with diverse parameter combinations
 * Differentiation from harness_008: Simpler approach, different testing patterns, isolation testing
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for basic image testing
    // Need: format, width, height, alignments, border
    if (size < 24) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume core image parameters with reasonable limits
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(1, 512);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(1, 512);
    
    // Generate alignments (must be power of 2)
    unsigned int align_pow = fdp.ConsumeIntegralInRange<unsigned int>(0, 6); // 0-6 gives 1-64
    unsigned int align = 1 << align_pow;
    if (align == 0) align = 1;
    
    unsigned int size_align_pow = fdp.ConsumeIntegralInRange<unsigned int>(0, 6);
    unsigned int size_align = 1 << size_align_pow;
    if (size_align == 0) size_align = 1;
    
    unsigned int stride_align_pow = fdp.ConsumeIntegralInRange<unsigned int>(0, 6);
    unsigned int stride_align = 1 << stride_align_pow;
    if (stride_align == 0) stride_align = 1;
    
    // Border size - test various scenarios
    unsigned int border = fdp.ConsumeIntegralInRange<unsigned int>(0, 64);
    
    // Image format - pick from valid formats using bitmask approach
    uint32_t format_choice = fdp.ConsumeIntegralInRange<uint32_t>(0, 7);
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420; // Default
    
    switch (format_choice) {
        case 0: fmt = AOM_IMG_FMT_I420; break;
        case 1: fmt = AOM_IMG_FMT_YV12; break;
        case 2: fmt = AOM_IMG_FMT_I422; break;
        case 3: fmt = AOM_IMG_FMT_I444; break;
        case 4: fmt = AOM_IMG_FMT_NV12; break;
        case 5: fmt = AOM_IMG_FMT_I42016; break;
        case 6: fmt = AOM_IMG_FMT_I42216; break;
        case 7: fmt = AOM_IMG_FMT_I44416; break;
    }
    
    // Test Strategy 1: Standalone aom_img_alloc_with_border testing
    // Focus on different border scenarios and alignment combinations
    aom_image_t* img_border = aom_img_alloc_with_border(nullptr, fmt, width, height, 
                                                         align, size_align, border);
    if (!img_border) {
        // Try with simpler parameters
        border = border / 2;
        img_border = aom_img_alloc_with_border(nullptr, fmt, width, height, 
                                               1, 1, border);
        if (!img_border) {
            return 0; // Could not allocate any border image
        }
    }
    
    // Test Strategy 2: Standalone aom_img_wrap testing
    // Calculate required buffer size based on format
    size_t buffer_size = 0;
    
    // Basic buffer size calculation
    switch (fmt & ~AOM_IMG_FMT_HIGHBITDEPTH) {
        case AOM_IMG_FMT_I420:
        case AOM_IMG_FMT_YV12:
        case AOM_IMG_FMT_NV12:
            buffer_size = width * height * 3 / 2;
            break;
        case AOM_IMG_FMT_I422:
            buffer_size = width * height * 2;
            break;
        case AOM_IMG_FMT_I444:
            buffer_size = width * height * 3;
            break;
        default:
            buffer_size = width * height * 4; // Conservative
            break;
    }
    
    // Adjust for high bitdepth
    if (fmt & AOM_IMG_FMT_HIGHBITDEPTH) {
        buffer_size *= 2;
    }
    
    // Add extra for safety
    buffer_size += 256;
    
    // Check if we have enough data
    if (fdp.remaining_bytes() < buffer_size) {
        aom_img_free(img_border);
        return 0;
    }
    
    // Consume buffer from fuzzer input
    std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
    
    // Test aom_img_wrap
    aom_image_t* img_wrapped = aom_img_wrap(nullptr, fmt, width, height, 
                                           stride_align, buffer.data());
    if (!img_wrapped) {
        // Try with default stride alignment
        img_wrapped = aom_img_wrap(nullptr, fmt, width, height, 1, buffer.data());
        if (!img_wrapped) {
            aom_img_free(img_border);
            return 0;
        }
    }
    
    // Test Strategy 3: Comprehensive plane dimension queries
    // Test aom_img_plane_width and aom_img_plane_height for all planes
    
    // For border-allocated image
    int plane_y_width_border = aom_img_plane_width(img_border, AOM_PLANE_Y);
    int plane_y_height_border = aom_img_plane_height(img_border, AOM_PLANE_Y);
    
    int plane_u_width_border = aom_img_plane_width(img_border, AOM_PLANE_U);
    int plane_u_height_border = aom_img_plane_height(img_border, AOM_PLANE_U);
    
    int plane_v_width_border = aom_img_plane_width(img_border, AOM_PLANE_V);
    int plane_v_height_border = aom_img_plane_height(img_border, AOM_PLANE_V);
    
    // For wrapped image
    int plane_y_width_wrapped = aom_img_plane_width(img_wrapped, AOM_PLANE_Y);
    int plane_y_height_wrapped = aom_img_plane_height(img_wrapped, AOM_PLANE_Y);
    
    int plane_u_width_wrapped = aom_img_plane_width(img_wrapped, AOM_PLANE_U);
    int plane_u_height_wrapped = aom_img_plane_height(img_wrapped, AOM_PLANE_U);
    
    int plane_v_width_wrapped = aom_img_plane_width(img_wrapped, AOM_PLANE_V);
    int plane_v_height_wrapped = aom_img_plane_height(img_wrapped, AOM_PLANE_V);
    
    // Test Strategy 4: Image flipping operations
    // Test aom_img_flip multiple times with different sequences
    
    // Flip border-allocated image
    aom_img_flip(img_border);
    
    // Flip wrapped image
    aom_img_flip(img_wrapped);
    
    // Flip back to original (double flip returns to original)
    aom_img_flip(img_border);
    aom_img_flip(img_wrapped);
    
    // Test Strategy 5: Edge case testing with extreme parameters
    // Use remaining fuzzer data for additional parameter combinations
    if (fdp.remaining_bytes() >= 4) {
        unsigned int test_case = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
        
        switch (test_case) {
            case 0: {
                // Test with minimal dimensions
                if (fdp.remaining_bytes() >= 8) {
                    unsigned int min_width = 1;
                    unsigned int min_height = 1;
                    unsigned int min_border = fdp.ConsumeIntegralInRange<unsigned int>(0, 4);
                    
                    aom_image_t* img_min = aom_img_alloc_with_border(nullptr, fmt, 
                                                                     min_width, min_height,
                                                                     1, 1, min_border);
                    if (img_min) {
                        // Test plane queries
                        int pw = aom_img_plane_width(img_min, AOM_PLANE_Y);
                        int ph = aom_img_plane_height(img_min, AOM_PLANE_Y);
                        // Test flip
                        aom_img_flip(img_min);
                        aom_img_flip(img_min); // Flip back
                        aom_img_free(img_min);
                    }
                }
                break;
            }
            case 1: {
                // Test with large border relative to image size
                if (width > 4 && height > 4 && border < width/2 && border < height/2) {
                    unsigned int large_border = std::min(width/2, height/2);
                    aom_image_t* img_large_border = aom_img_alloc_with_border(nullptr, fmt,
                                                                              width, height,
                                                                              1, 1, large_border);
                    if (img_large_border) {
                        // Test plane queries with large border
                        int pw = aom_img_plane_width(img_large_border, AOM_PLANE_Y);
                        int ph = aom_img_plane_height(img_large_border, AOM_PLANE_Y);
                        aom_img_free(img_large_border);
                    }
                }
                break;
            }
            case 2: {
                // Test different alignment combinations
                if (fdp.remaining_bytes() >= 4) {
                    unsigned int align_combo = fdp.ConsumeIntegralInRange<unsigned int>(1, 16);
                    aom_image_t* img_align = aom_img_alloc_with_border(nullptr, fmt,
                                                                       width, height,
                                                                       align_combo, align_combo,
                                                                       border);
                    if (img_align) {
                        aom_img_free(img_align);
                    }
                }
                break;
            }
            case 3: {
                // Test wrapping with different buffer alignments
                if (fdp.remaining_bytes() >= buffer_size) {
                    std::vector<uint8_t> buffer2 = fdp.ConsumeBytes<uint8_t>(buffer_size);
                    unsigned int custom_stride = fdp.ConsumeIntegralInRange<unsigned int>(1, 32);
                    aom_image_t* img_wrap2 = aom_img_wrap(nullptr, fmt, width, height,
                                                         custom_stride, buffer2.data());
                    if (img_wrap2) {
                        aom_img_free(img_wrap2);
                    }
                }
                break;
            }
        }
    }
    
    // Cleanup
    aom_img_free(img_border);
    aom_img_free(img_wrapped);
    
    return 0;
}

} // namespace harness_009
