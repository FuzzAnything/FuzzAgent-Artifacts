#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <memory>
#include <algorithm>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_image.h"

namespace harness_011 {
/*
 * Copyright (c) 2024 Fuzzing Harness Generator
 *
 * Targeted fuzzing harness for libaom image manipulation APIs.
 * Focuses on image wrapping and flipping operations identified as 0% coverage.
 * 
 * Primary Targets:
 * 1. aom_img_wrap - Image wrapping API with 32 branches (0% coverage)
 * 2. aom_img_flip - Image flipping API (0% coverage)
 * 
 * Helper APIs:
 * - aom_img_alloc / aom_img_alloc_with_border - Image creation
 * - aom_img_free - Resource cleanup
 * 
 * Semantic differentiation from previous 11 harnesses:
 * - harness_000: Broad decoder/encoder coverage
 * - harness_001: Basic encoder operations  
 * - harness_002: Common utilities and error handling
 * - harness_003: Advanced encoder configuration and buffer management
 * - harness_004: Decoder frame buffer management and metadata operations
 * - harness_005: Encoder metadata retrieval and informational API testing
 * - harness_006: Decoder tile operations, frame information, and decoder control APIs
 * - harness_007: SVC (Scalable Video Coding) multi-layer encoding
 * - harness_008: External rate control library integration with advanced error handling
 * - harness_009: Film grain controls, denoising, chroma subsampling, and advanced encoder feature combinations
 * - harness_010: Bitstream parsing utilities, memory management APIs, and intra prediction search functions
 * - harness_011: Image manipulation APIs - wrapping, flipping, allocation with borders (NEW)
 */



// Helper function to calculate minimum buffer size for a given image format and dimensions
// Based on the logic from aom_image.h documentation
static size_t calculate_min_image_buffer_size(aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, 
                                             unsigned int stride_align) {
    // First, we need to get the chroma shifts and bps from the format
    // We can do this by calling aom_img_wrap with a dummy pointer to get these values
    // But for simplicity in fuzzing, we'll use conservative estimates
    
    // For fuzzing, we'll allocate a conservative maximum size
    // Maximum bits per sample: 16 for high bit depth formats
    int bps = 48; // Conservative maximum (I44416: 48 bps)
    
    // Check if it's a planar format
    if (fmt & AOM_IMG_FMT_PLANAR) {
        // For planar formats, we need to account for all planes
        // Conservative calculation: assume worst-case dimensions and alignment
        unsigned int align = 31; // Maximum power of 2 alignment - 1
        unsigned int w = (d_w + align) & ~align;
        unsigned int h = (d_h + align) & ~align;
        
        // Apply stride alignment
        size_t s = w;
        if (stride_align > 1) {
            s = (s + stride_align - 1) & ~((size_t)stride_align - 1);
        }
        
        // For high bit depth, double the size
        if (fmt & AOM_IMG_FMT_HIGHBITDEPTH) {
            s *= 2;
        }
        
        // Calculate size based on format
        size_t alloc_size = 0;
        if (fmt == AOM_IMG_FMT_I420 || fmt == AOM_IMG_FMT_YV12 || fmt == AOM_IMG_FMT_NV12) {
            // 4:2:0 formats: Y plane + UV planes
            alloc_size = (size_t)h * s + 2 * ((size_t)(h/2) * (s/2));
        } else if (fmt == AOM_IMG_FMT_I422) {
            // 4:2:2 formats
            alloc_size = (size_t)h * s * 2;
        } else if (fmt == AOM_IMG_FMT_I444) {
            // 4:4:4 formats
            alloc_size = (size_t)h * s * 3;
        } else if (fmt == AOM_IMG_FMT_I42016 || fmt == AOM_IMG_FMT_YV1216) {
            // High bit depth 4:2:0
            alloc_size = (size_t)h * s * 3; // 2 bytes per sample
        } else if (fmt == AOM_IMG_FMT_I42216) {
            // High bit depth 4:2:2
            alloc_size = (size_t)h * s * 4;
        } else if (fmt == AOM_IMG_FMT_I44416) {
            // High bit depth 4:4:4
            alloc_size = (size_t)h * s * 6;
        } else {
            // Default conservative size
            alloc_size = (size_t)d_w * d_h * 6 * 2; // 6 bytes per pixel max * 2 for safety
        }
        
        return alloc_size;
    } else {
        // Packed format (none currently in libaom, but for completeness)
        return (size_t)d_w * d_h * bps / 8;
    }
}

// Array of valid image formats for testing
static const aom_img_fmt_t valid_formats[] = {
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

static const size_t num_valid_formats = sizeof(valid_formats) / sizeof(valid_formats[0]);

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum input size: we need enough for format selection, dimensions, alignment, etc.
    const size_t MIN_INPUT_SIZE = 64;
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Insufficient input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selection
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume image parameters
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(1, 1024);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(1, 1024);
    
    // Consume alignment parameters (must be power of 2 for valid cases)
    unsigned int align = 1 << (fdp.ConsumeIntegralInRange<unsigned int>(0, 6)); // 1, 2, 4, ..., 64
    unsigned int stride_align = 1 << (fdp.ConsumeIntegralInRange<unsigned int>(0, 6));
    unsigned int size_align = 1 << (fdp.ConsumeIntegralInRange<unsigned int>(0, 6));
    unsigned int border = fdp.ConsumeIntegralInRange<unsigned int>(0, 64);
    
    // Select image format
    aom_img_fmt_t format = valid_formats[fdp.ConsumeIntegralInRange<size_t>(0, num_valid_formats - 1)];
    
    // Get remaining bytes for image data
    std::vector<uint8_t> remaining_bytes = fdp.ConsumeRemainingBytes<uint8_t>();
    
    switch (test_scenario) {
        case 0: {
            // Test 1: Basic aom_img_wrap with allocated buffer
            aom_image_t img;
            
            // Calculate required buffer size
            size_t buffer_size = calculate_min_image_buffer_size(format, width, height, stride_align);
            
            if (buffer_size == 0 || buffer_size > 100 * 1024 * 1024) {
                // Skip excessively large allocations to avoid OOM
                return 0;
            }
            
            // Allocate buffer
            std::vector<unsigned char> buffer(buffer_size, 0);
            
            // Test aom_img_wrap with valid parameters
            aom_image_t* wrapped_img = aom_img_wrap(&img, format, width, height, stride_align, buffer.data());
            
            if (wrapped_img != nullptr) {
                // Successfully wrapped, verify some properties
                if (wrapped_img->fmt == format && 
                    wrapped_img->d_w == width && 
                    wrapped_img->d_h == height) {
                    // Valid wrapped image, try flipping it
                    aom_img_flip(wrapped_img);
                    
                    // Flip it back (flipping twice should restore original orientation)
                    aom_img_flip(wrapped_img);
                }
                
                // Cleanup - aom_img_wrap doesn't allocate, just wraps existing buffer
                // No need to free, but we should call aom_img_free if img was allocated on heap
                // In this case, img is stack-allocated, so we just let it go out of scope
            }
            
            // Also test with NULL img pointer (should allocate descriptor on heap)
            aom_image_t* heap_img = aom_img_wrap(nullptr, format, width, height, stride_align, buffer.data());
            if (heap_img != nullptr) {
                aom_img_free(heap_img);
            }
            break;
        }
        
        case 1: {
            // Test 2: aom_img_alloc with aom_img_flip
            aom_image_t img;
            
            // Test aom_img_alloc
            aom_image_t* allocated_img = aom_img_alloc(&img, format, width, height, align);
            
            if (allocated_img != nullptr) {
                // Successfully allocated, test flipping
                aom_img_flip(allocated_img);
                
                // Test flipping multiple times
                for (int i = 0; i < 3; i++) {
                    aom_img_flip(allocated_img);
                }
                
                // Cleanup
                aom_img_free(allocated_img);
            }
            
            // Also test with NULL img pointer
            aom_image_t* heap_allocated_img = aom_img_alloc(nullptr, format, width, height, align);
            if (heap_allocated_img != nullptr) {
                aom_img_flip(heap_allocated_img);
                aom_img_free(heap_allocated_img);
            }
            break;
        }
        
        case 2: {
            // Test 3: aom_img_alloc_with_border with flipping
            aom_image_t img;
            
            // Test aom_img_alloc_with_border
            aom_image_t* bordered_img = aom_img_alloc_with_border(&img, format, width, height, 
                                                                  align, size_align, border);
            
            if (bordered_img != nullptr) {
                // Successfully allocated with border, test flipping
                aom_img_flip(bordered_img);
                
                // Flip multiple times
                aom_img_flip(bordered_img);
                aom_img_flip(bordered_img);
                
                // Cleanup
                aom_img_free(bordered_img);
            }
            
            // Test with NULL img pointer
            aom_image_t* heap_bordered_img = aom_img_alloc_with_border(nullptr, format, width, height,
                                                                       align, size_align, border);
            if (heap_bordered_img != nullptr) {
                aom_img_flip(heap_bordered_img);
                aom_img_free(heap_bordered_img);
            }
            break;
        }
        
        case 3: {
            // Test 4: Error handling - test invalid parameters
            // Test aom_img_wrap with invalid alignment (not power of 2)
            aom_image_t img;
            
            // Invalid alignment (not power of 2)
            unsigned int invalid_align = 31;
            
            // Need some buffer for the test
            size_t buffer_size = calculate_min_image_buffer_size(format, width, height, 32);
            if (buffer_size == 0 || buffer_size > 10 * 1024 * 1024) {
                return 0;
            }
            
            std::vector<unsigned char> buffer(buffer_size, 0);
            
            // This should fail due to invalid alignment
            aom_image_t* invalid_wrapped = aom_img_wrap(&img, format, width, height, 
                                                        invalid_align, buffer.data());
            // Expecting nullptr for invalid alignment
            
            // Test with excessively large dimensions (should fail)
            unsigned int huge_width = 0x08000001; // Exceeds maximum of 0x08000000
            unsigned int huge_height = 0x08000001;
            
            if (buffer_size > 0) {
                aom_image_t* huge_wrapped = aom_img_wrap(&img, format, huge_width, huge_height,
                                                         align, buffer.data());
                // Should fail due to excessive dimensions
            }
            
            // Test invalid image format
            aom_img_fmt_t invalid_format = static_cast<aom_img_fmt_t>(999);
            aom_image_t* invalid_format_wrapped = aom_img_wrap(&img, invalid_format, width, height,
                                                               align, buffer.data());
            // Should fail due to invalid format
            
            // Test aom_img_alloc with invalid parameters
            aom_image_t* invalid_alloc = aom_img_alloc(&img, invalid_format, width, height, align);
            // Should fail
            
            // Test with alignment of 0 (should be treated as 1)
            aom_image_t* zero_align_img = aom_img_alloc(&img, format, width, height, 0);
            if (zero_align_img != nullptr) {
                aom_img_free(zero_align_img);
            }
            
            break;
        }
    }
    
    return 0;
}

} // namespace harness_011
