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

namespace harness_004 {
/*
 * Fuzzing harness for libaom image manipulation APIs
 * Targets comprehensive image lifecycle: creation via wrap or alloc_with_border,
 * dimension querying with plane_width and plane_height, manipulation with flip,
 * and cleanup with aom_img_free.
 * 
 * Target API sequence:
 * 1. aom_img_wrap - Create image from existing buffer (UNCOVERED - 11 undiscovered branches)
 * 2. aom_img_alloc_with_border - Allocate image with border (UNCOVERED - 11 undiscovered branches)
 * 3. aom_img_plane_width - Query plane width (UNCOVERED - 2 undiscovered branches)
 * 4. aom_img_plane_height - Query plane height (UNCOVERED - 2 undiscovered branches)
 * 5. aom_img_flip - Flip image vertically (UNCOVERED - 0% coverage)
 * 6. aom_img_free - Cleanup (already covered, required for lifecycle)
 */




int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for basic image configuration
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume image configuration parameters from fuzzer input
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(1, 4096);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(1, 4096);
    
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
    
    // Consume alignment parameters
    unsigned int stride_align = fdp.ConsumeIntegralInRange<unsigned int>(1, 64);
    unsigned int size_align = fdp.ConsumeIntegralInRange<unsigned int>(1, 64);
    
    // Consume border size for alloc_with_border
    unsigned int border = fdp.ConsumeIntegralInRange<unsigned int>(0, 256);
    
    // Determine which image creation method to use
    bool use_wrap = fdp.ConsumeBool();
    
    // Step 1: Image creation - choose between wrap and alloc_with_border
    aom_image_t img;
    aom_image_t* img_ptr = nullptr;
    
    if (use_wrap) {
        // Use aom_img_wrap - create image from existing buffer
        
        // First call to get buffer size requirements
        img_ptr = aom_img_wrap(&img, format, width, height, stride_align, (unsigned char *)1);
        if (img_ptr == nullptr) {
            // Failed to get buffer requirements
            return 0;
        }
        
        // Calculate approximate buffer size needed
        // Simplified calculation based on image properties
        size_t approx_buffer_size = 0;
        if (format & AOM_IMG_FMT_PLANAR) {
            // Planar format: Y + U + V planes
            int y_plane_size = width * height;
            int uv_plane_size = 0;
            
            if (format == AOM_IMG_FMT_I420 || format == AOM_IMG_FMT_YV12 || 
                format == AOM_IMG_FMT_I42016 || format == AOM_IMG_FMT_YV1216) {
                uv_plane_size = (width / 2) * (height / 2);
            } else if (format == AOM_IMG_FMT_I422 || format == AOM_IMG_FMT_I42216) {
                uv_plane_size = (width / 2) * height;
            } else if (format == AOM_IMG_FMT_I444 || format == AOM_IMG_FMT_I44416) {
                uv_plane_size = width * height;
            }
            
            // Account for high bit depth
            if (format & AOM_IMG_FMT_HIGHBITDEPTH) {
                y_plane_size *= 2;
                uv_plane_size *= 2;
            }
            
            approx_buffer_size = y_plane_size + 2 * uv_plane_size;
        } else {
            // Packed format: single buffer
            approx_buffer_size = width * height * 3; // Conservative estimate
        }
        
        // Add some padding for alignment
        approx_buffer_size += stride_align * 4;
        
        // Check if we have enough fuzzer data for the buffer
        if (fdp.remaining_bytes() < approx_buffer_size) {
            // Not enough data, try with smaller buffer
            approx_buffer_size = std::min(approx_buffer_size, fdp.remaining_bytes());
        }
        
        if (approx_buffer_size == 0) {
            // No data for buffer
            return 0;
        }
        
        // Consume buffer data from fuzzer input
        std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(approx_buffer_size);
        
        // Now call aom_img_wrap with actual buffer
        img_ptr = aom_img_wrap(&img, format, width, height, stride_align, buffer_data.data());
        if (img_ptr == nullptr) {
            // Wrap failed
            return 0;
        }
    } else {
        // Use aom_img_alloc_with_border - allocate image with border
        img_ptr = aom_img_alloc_with_border(&img, format, width, height, 
                                           stride_align, size_align, border);
        if (img_ptr == nullptr) {
            // Allocation failed
            return 0;
        }
    }
    
    // At this point, img_ptr points to a valid image (either wrapped or allocated)
    
    // Step 2: Query image dimensions using plane_width and plane_height
    // Test all possible plane indices
    for (int plane = 0; plane < 3; plane++) {
        int plane_w = aom_img_plane_width(img_ptr, plane);
        int plane_h = aom_img_plane_height(img_ptr, plane);
        
        // The results may be negative or zero for invalid planes or formats
        // We just call the APIs to exercise them
        (void)plane_w;
        (void)plane_h;
    }
    
    // Step 3: Perform image manipulation with aom_img_flip
    // Only flip if we have a valid image
    if (img_ptr != nullptr) {
        aom_img_flip(img_ptr);
        
        // Query dimensions again after flip to see if they changed
        for (int plane = 0; plane < 3; plane++) {
            int plane_w_after = aom_img_plane_width(img_ptr, plane);
            int plane_h_after = aom_img_plane_height(img_ptr, plane);
            
            (void)plane_w_after;
            (void)plane_h_after;
        }
        
        // Flip back to original orientation (optional, tests flip again)
        if (fdp.ConsumeBool()) {
            aom_img_flip(img_ptr);
        }
    }
    
    // Step 4: Cleanup with aom_img_free
    // Note: For wrapped images, the buffer was provided by us and will be
    // automatically cleaned up when the vector goes out of scope.
    // For allocated images, aom_img_free will free the internally allocated buffer.
    aom_img_free(img_ptr);
    
    return 0;
}

} // namespace harness_004
