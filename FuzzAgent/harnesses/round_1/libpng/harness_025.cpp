#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_025 {
/*
 * Harness for testing png_read_png as the most critical uncovered API with 
 * 209 undiscovered branches identified by coverage analysis. Focus on comprehensive 
 * PNG reading operations using png_read_png API with proper initialization, 
 * error handling, and cleanup sequences.
 * 
 * Target API: png_read_png - High-level PNG reading function with 209 undiscovered branches
 * 
 * This harness provides semantic diversity from existing harnesses:
 * - harness_004.cpp: Basic png_read_png with transform flags and row reading
 * - harness_010.cpp: png_read_png with various PNG_SET_* transformations
 * - This harness: Comprehensive png_read_png testing with multiple parameter combinations,
 *   error scenarios, and PNG configurations to target the 209 undiscovered branches
 * 
 * The png_read_png function in pngread.c has significant complexity:
 * - 16 different PNG_TRANSFORM flags combinations
 * - Multiple supported/unsupported feature paths
 * - Complex error handling for different PNG configurations
 * - Memory allocation and row pointer management
 * - Image dimension validation and processing
 * 
 * This harness specifically targets:
 * 1. All 16 PNG_TRANSFORM flag combinations and their interactions
 * 2. Edge cases in image dimension validation (height overflow check)
 * 3. Parameter validation (NULL pointer checks)
 * 4. Different PNG color types and bit depths
 * 5. Various filter methods and compression types
 * 6. Error paths in transformation application
 * 7. Memory management during row pointer allocation
 */


// Buffer structure for memory I/O
struct mem_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Error callback
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

// Warning callback
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

// User chunk callback for testing chunk handling
static int user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    (void)png_ptr;
    (void)chunk;
    // Return 1 to keep chunk, 0 to discard
    return 1;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful PNG reading operations
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Prepare memory buffer for custom read function
    mem_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    
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
    
    // Consume operation type from fuzzer input to determine testing strategy
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Configure PNG based on operation type
    switch (operation) {
        case 0: // Basic png_read_png with various transform flags
        {
            // Consume transform flags from fuzzer input
            int transforms = fdp.ConsumeIntegral<int>();
            
            // Consume number of signature bytes already read
            int sig_bytes = fdp.ConsumeIntegralInRange<int>(0, 8);
            png_set_sig_bytes(png_ptr, sig_bytes);
            
            // Test png_read_png with the specified transforms
            png_read_png(png_ptr, info_ptr, transforms, NULL);
            break;
        }
        
        case 1: // png_read_png with PNG configuration setup
        {
            // Consume transform flags
            int transforms = fdp.ConsumeIntegral<int>();
            
            // Setup various PNG configurations before reading
            // Set CRC action for error handling
            int crc_action = fdp.ConsumeIntegralInRange<int>(0, 2);
            png_set_crc_action(png_ptr, crc_action, crc_action);
            
            // Set filter method if supported
#ifdef PNG_READ_FILTER_SUPPORTED
            int filter_method = fdp.ConsumeIntegralInRange<int>(0, 5);
            png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, filter_method);
#endif
            
            // Set chunk handling
            int keep_unknown_chunks = fdp.ConsumeIntegralInRange<int>(0, 3);
            png_set_keep_unknown_chunks(png_ptr, keep_unknown_chunks, NULL, 0);
            
            // Set user chunk callback if supported
#ifdef PNG_READ_USER_CHUNKS_SUPPORTED
            if (fdp.ConsumeBool()) {
                png_set_read_user_chunk_fn(png_ptr, NULL, user_chunk_callback);
            }
#endif
            
            // Set user limits if supported
#ifdef PNG_SET_USER_LIMITS_SUPPORTED
            if (fdp.remaining_bytes() >= 8) {
                png_uint_32 max_width = fdp.ConsumeIntegral<uint32_t>();
                png_uint_32 max_height = fdp.ConsumeIntegral<uint32_t>();
                png_set_user_limits(png_ptr, max_width, max_height);
            }
#endif
            
            // Test png_read_png with configured settings
            png_read_png(png_ptr, info_ptr, transforms, NULL);
            break;
        }
        
        case 2: // png_read_png with color space and gamma setup
        {
            // Consume transform flags
            int transforms = fdp.ConsumeIntegral<int>();
            
            // Setup color space and gamma before reading
#ifdef PNG_READ_GAMMA_SUPPORTED
            if (fdp.remaining_bytes() >= 8) {
                double screen_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
                png_set_gamma(png_ptr, screen_gamma, -1.0);
            }
#endif
            
            // Setup RGB to gray conversion if supported
#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
            if (fdp.ConsumeBool()) {
                int rgb_to_gray_flags = fdp.ConsumeIntegralInRange<int>(0, 2);
                double red_coeff = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
                double green_coeff = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
                double blue_coeff = 1.0 - red_coeff - green_coeff;
                if (blue_coeff < 0.0) blue_coeff = 0.0;
                
                png_set_rgb_to_gray(png_ptr, rgb_to_gray_flags, red_coeff, green_coeff);
            }
#endif
            
            // Setup background if supported
#ifdef PNG_READ_BACKGROUND_SUPPORTED
            if (fdp.remaining_bytes() >= 8) {
                png_color_16 background;
                background.red = fdp.ConsumeIntegral<uint16_t>();
                background.green = fdp.ConsumeIntegral<uint16_t>();
                background.blue = fdp.ConsumeIntegral<uint16_t>();
                background.gray = fdp.ConsumeIntegral<uint16_t>();
                background.index = fdp.ConsumeIntegral<uint8_t>();
                
                png_set_background(png_ptr, &background, 
                    PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
            }
#endif
            
            // Test png_read_png with color space setup
            png_read_png(png_ptr, info_ptr, transforms, NULL);
            break;
        }
        
        case 3: // png_read_png with palette and transparency setup
        {
            // Consume transform flags
            int transforms = fdp.ConsumeIntegral<int>();
            
            // Setup palette if supported
#ifdef PNG_READ_QUANTIZE_SUPPORTED
            if (fdp.remaining_bytes() > 100) {
                // Create a simple palette for testing
                png_color palette[256];
                int num_palette = fdp.ConsumeIntegralInRange<int>(1, 256);
                int maximum_colors = fdp.ConsumeIntegralInRange<int>(1, 256);
                
                for (int i = 0; i < num_palette && i < 256; i++) {
                    palette[i].red = fdp.ConsumeIntegral<uint8_t>();
                    palette[i].green = fdp.ConsumeIntegral<uint8_t>();
                    palette[i].blue = fdp.ConsumeIntegral<uint8_t>();
                }
                
                png_set_quantize(png_ptr, palette, num_palette, maximum_colors, NULL, 0);
            }
#endif
            
            // Setup transparency if supported
#ifdef PNG_READ_EXPAND_SUPPORTED
            if (fdp.ConsumeBool()) {
                // Test transparency expansion
                png_set_expand(png_ptr);
                
                // Setup tRNS handling if there's enough data
                if (fdp.remaining_bytes() >= 256) {
                    png_color_16 trans_color;
                    trans_color.red = fdp.ConsumeIntegral<uint16_t>();
                    trans_color.green = fdp.ConsumeIntegral<uint16_t>();
                    trans_color.blue = fdp.ConsumeIntegral<uint16_t>();
                    trans_color.gray = fdp.ConsumeIntegral<uint16_t>();
                    trans_color.index = fdp.ConsumeIntegral<uint8_t>();
                    
                    png_set_tRNS_to_alpha(png_ptr);
                }
            }
#endif
            
            // Setup palette to RGB expansion if supported
#ifdef PNG_READ_EXPAND_SUPPORTED
            if (fdp.ConsumeBool()) {
                png_set_palette_to_rgb(png_ptr);
            }
#endif
            
            // Test png_read_png with palette setup
            png_read_png(png_ptr, info_ptr, transforms, NULL);
            break;
        }
        
        case 4: // png_read_png with error scenario testing
        {
            // Consume transform flags
            int transforms = fdp.ConsumeIntegral<int>();
            
            // Test various error scenarios
            
            // Test with excessive signature bytes
            int sig_bytes = fdp.ConsumeIntegralInRange<int>(0, 16);  // May be >8 for error
            png_set_sig_bytes(png_ptr, sig_bytes);
            
            // Test with invalid filter method
#ifdef PNG_READ_FILTER_SUPPORTED
            int filter_method = fdp.ConsumeIntegralInRange<int>(-10, 10);
            png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, filter_method);
#endif
            
            // Test with extreme chunk cache limits
#ifdef PNG_SET_USER_LIMITS_SUPPORTED
            if (fdp.ConsumeBool()) {
                png_uint_32 chunk_cache_max = fdp.ConsumeIntegral<uint32_t>();
                png_set_chunk_cache_max(png_ptr, chunk_cache_max);
            }
#endif
            
            // Test png_read_png with potential error conditions
            png_read_png(png_ptr, info_ptr, transforms, NULL);
            break;
        }
    }
    
    // After png_read_png, test retrieval of image information
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_IDAT)) {
        // Get basic image dimensions
        png_uint_32 width, height;
        int bit_depth, color_type, interlace_type, compression_type, filter_type;
        
        if (png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                        &interlace_type, &compression_type, &filter_type)) {
            // Successfully retrieved image dimensions
            (void)width; (void)height; (void)bit_depth; (void)color_type;
            (void)interlace_type; (void)compression_type; (void)filter_type;
            
            // Test rowbytes retrieval
            png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
            (void)rowbytes;
            
            // Test channels retrieval
            int channels = png_get_channels(png_ptr, info_ptr);
            (void)channels;
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}

} // namespace harness_025
