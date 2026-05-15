#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_023 {
/*
 * Harness for testing PNG transformation module (pngrtran.c) identified as
 * the most critical coverage gap with 35.42% line coverage and 49+ undiscovered
 * branches. Specifically targets transformation APIs that are completely or 
 * partially uncovered in existing harnesses.
 * 
 * Target APIs from PNG transformation module:
 * - png_set_user_transform_info: User-defined transformation setup
 * - png_set_tRNS_to_alpha: Convert transparency to alpha channel
 * - png_set_palette_to_rgb: Expand paletted images to RGB
 * - png_set_sRGB_gAMA_and_cHRM: Set sRGB color space with gamma and chromaticity
 * - png_build_grayscale_palette: Build grayscale palette for transformation
 * 
 * Additional transformation APIs for comprehensive coverage:
 * - png_set_expand: General expansion transformation
 * - png_set_expand_16: Expand to 16-bit channels
 * - png_set_gray_to_rgb: Convert grayscale to RGB
 * - png_set_rgb_to_gray: Convert RGB to grayscale
 * - png_set_bgr: BGR to RGB mapping
 * 
 * Required helper functions for complete transformation lifecycle:
 * - png_create_read_struct, png_create_info_struct
 * - png_set_IHDR for image setup
 * - png_set_read_user_transform_fn for user transform callbacks
 * - png_read_info, png_read_image, png_read_end
 * - png_destroy_read_struct
 * 
 * This harness provides semantic diversity from existing harnesses:
 * - harness_008.cpp: Basic transformations (packing, shift, swap, bgr, etc.)
 * - harness_013.cpp: Color transformation operations (rgb_to_gray, gamma, background)
 * - This harness: Comprehensive PNG transformation module targeting pngrtran.c
 *   with user transforms, palette expansion, alpha conversion, and sRGB setup
 * 
 * The PNG transformation module (pngrtran.c) has significant coverage gaps:
 * - 35.42% line coverage indicates large portions of transformation logic untested
 * - 49+ undiscovered branches in transformation decision paths
 * - User transform functionality completely untested in existing harnesses
 * - Palette and alpha transformation paths largely unexplored
 */


// ==================== MEMORY BUFFER FOR READING ====================

struct mem_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Callback functions for libpng error handling
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom read function for reading from memory buffer
static void png_read_from_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    mem_buffer* buffer = (mem_buffer*)io_ptr;
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

// User transformation callback for testing png_set_user_transform_info
static void user_transform_callback(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    (void)png_ptr;
    (void)row_info;
    (void)data;
    
    // Simple transformation: do nothing in this test harness
    // In real usage, this would apply custom pixel transformations
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful PNG transformation operations
    // We need: transformation parameters, image setup, and some PNG data
    if (size < 300) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Prepare memory buffer for custom read function
    struct mem_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    
    // ==================== CONSUME TRANSFORMATION PARAMETERS ====================
    
    // Consume transformation configuration mask
    uint32_t transform_mask = fdp.ConsumeIntegral<uint32_t>();
    
    // Consume image parameters for setup
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint8_t bit_depth = fdp.ConsumeIntegral<uint8_t>() % 5; // 1, 2, 4, 8, 16
    if (bit_depth == 0) bit_depth = 1;
    else if (bit_depth == 1) bit_depth = 2;
    else if (bit_depth == 2) bit_depth = 4;
    else if (bit_depth == 3) bit_depth = 8;
    else bit_depth = 16;
    
    uint8_t color_type_choice = fdp.ConsumeIntegral<uint8_t>() % 6;
    png_byte color_type;
    switch (color_type_choice) {
        case 0: color_type = PNG_COLOR_TYPE_GRAY; break;
        case 1: color_type = PNG_COLOR_TYPE_GRAY_ALPHA; break;
        case 2: color_type = PNG_COLOR_TYPE_PALETTE; break;
        case 3: color_type = PNG_COLOR_TYPE_RGB; break;
        case 4: color_type = PNG_COLOR_TYPE_RGB_ALPHA; break;
        default: color_type = PNG_COLOR_TYPE_GRAY; break;
    }
    
    // Consume user transform parameters
    int user_transform_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int user_transform_channels = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    // Consume sRGB intent
    int srgb_intent = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // ==================== CREATE READ STRUCTURES ====================
    
    // Create read structure
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        png_error_callback,
        png_warning_callback);
    
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set up error handling via setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Setup custom read function
    png_set_read_fn(png_ptr, &buffer, png_read_from_memory);
    
    // ==================== APPLY TRANSFORMATIONS BASED ON CONFIGURATION ====================
    
    // Set up a dummy image header for transformation testing
    // Note: We're setting up transformations before reading actual PNG data
    // This tests transformation setup APIs
    
    // 1. TEST USER TRANSFORM FUNCTIONS (if enabled)
    if (transform_mask & 0x01) {
        // Set up user transform callback
        png_set_read_user_transform_fn(png_ptr, user_transform_callback);
        
        // Set user transform information
        void* user_data = (void*)(intptr_t)fdp.ConsumeIntegral<uint32_t>();
        png_set_user_transform_info(png_ptr, user_data, 
                                   user_transform_depth, user_transform_channels);
    }
    
    // 2. TEST PALETTE AND ALPHA TRANSFORMATIONS
    if (transform_mask & 0x02) {
        // Test palette to RGB transformation
        png_set_palette_to_rgb(png_ptr);
        
        // Test tRNS to alpha transformation
        png_set_tRNS_to_alpha(png_ptr);
        
        // Test general expansion
        png_set_expand(png_ptr);
        
        // Test 16-bit expansion if bit depth allows
        if (bit_depth == 8 && (transform_mask & 0x10)) {
            png_set_expand_16(png_ptr);
        }
    }
    
    // 3. TEST COLOR SPACE TRANSFORMATIONS
    if (transform_mask & 0x04) {
        // Test sRGB color space setup
        png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);
        
        // Test grayscale to RGB conversion
        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            png_set_gray_to_rgb(png_ptr);
        }
        
        // Test RGB to grayscale conversion
        if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
            double weights[2];
            weights[0] = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
            weights[1] = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
            png_set_rgb_to_gray(png_ptr, PNG_ERROR_ACTION_NONE, weights[0], weights[1]);
        }
        
        // Test BGR mapping for RGB images
        if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
            png_set_bgr(png_ptr);
        }
    }
    
    // 4. TEST GRAYSCALE PALETTE BUILDING
    if (transform_mask & 0x08) {
        // Build grayscale palette for testing
        // This API is not used internally but available for applications
        if (bit_depth <= 8) {
            int palette_size = 1 << bit_depth;
            if (palette_size <= 256) {
                png_color palette[256];
                png_build_grayscale_palette(bit_depth, palette);
                
                // Note: png_set_PLTE would be needed to actually use this palette
                // but we're testing the palette building function itself
            }
        }
    }
    
    // 5. TEST ADDITIONAL TRANSFORMATIONS
    if (transform_mask & 0x10) {
        // Test byte swapping for 16-bit images
        if (bit_depth == 16) {
            png_set_swap(png_ptr);
        }
        
        // Test packing for low bit depths
        if (bit_depth < 8) {
            png_set_packing(png_ptr);
        }
        
        // Test shift transformation
        if ((transform_mask & 0x20) && bit_depth <= 8) {
            png_color_8 true_bits;
            true_bits.red = fdp.ConsumeIntegralInRange<uint8_t>(1, bit_depth);
            true_bits.green = fdp.ConsumeIntegralInRange<uint8_t>(1, bit_depth);
            true_bits.blue = fdp.ConsumeIntegralInRange<uint8_t>(1, bit_depth);
            true_bits.gray = fdp.ConsumeIntegralInRange<uint8_t>(1, bit_depth);
            true_bits.alpha = fdp.ConsumeIntegralInRange<uint8_t>(1, bit_depth);
            png_set_shift(png_ptr, &true_bits);
        }
    }
    
    // ==================== ATTEMPT TO READ PNG DATA ====================
    
    // Try to read PNG info with transformations applied
    // This will test if transformations are properly set up
    try {
        png_read_info(png_ptr, info_ptr);
        
        // If we successfully read info, update transformations
        png_read_update_info(png_ptr, info_ptr);
        
        // Allocate row buffers based on transformed row bytes
        png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        if (row_bytes > 0 && row_bytes < 1024 * 1024) { // Reasonable limit
            png_bytep row = (png_bytep)png_malloc(png_ptr, row_bytes);
            if (row != NULL) {
                // Read a few rows to test transformations
                for (int i = 0; i < std::min(5, (int)height); i++) {
                    if (buffer.offset + row_bytes <= buffer.size) {
                        png_read_row(png_ptr, row, NULL);
                    }
                }
                png_free(png_ptr, row);
            }
        }
        
        // Skip to end of image data
        png_read_end(png_ptr, info_ptr);
        
    } catch (...) {
        // Some transformations may cause errors with invalid PNG data
        // This is expected and part of testing error paths
    }
    
    // ==================== CLEANUP ====================
    
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}

} // namespace harness_023
