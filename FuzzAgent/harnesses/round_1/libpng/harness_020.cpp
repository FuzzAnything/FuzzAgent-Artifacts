#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_020 {
/*
 * Harness for testing libpng compression parameter configuration APIs with 
 * comprehensive write/read lifecycle testing. Targets the pngwrite.c module 
 * compression customization functions that are completely uncovered according 
 * to coverage analysis guidance.
 * 
 * Targets 9 completely uncovered compression parameter APIs representing 34+ 
 * undiscovered branches:
 * 
 * IDAT compression setters (5 functions):
 * - png_set_compression_level (completely uncovered)
 * - png_set_compression_mem_level (completely uncovered)  
 * - png_set_compression_strategy (completely uncovered)
 * - png_set_compression_window_bits (completely uncovered)
 * - png_set_compression_method (completely uncovered)
 * 
 * Text compression setters (5 functions):
 * - png_set_text_compression_level (completely uncovered)
 * - png_set_text_compression_mem_level (completely uncovered)
 * - png_set_text_compression_strategy (completely uncovered)
 * - png_set_text_compression_window_bits (completely uncovered)
 * - png_set_text_compression_method (completely uncovered)
 *
 * Required helper functions for complete lifecycle:
 * - png_create_write_struct, png_create_info_struct
 * - png_set_IHDR for basic image setup
 * - png_set_text for text chunks to test text compression
 * - png_write_info, png_write_image, png_write_end
 * - png_destroy_write_struct
 *
 * Invocation sequence (critical for compression parameter lifecycle):
 * 1. Create write and info structs
 * 2. Set compression parameters BEFORE png_write_info (as per libpng docs)
 * 3. Set image header and text chunks
 * 4. Write PNG with custom compression settings
 * 5. Clean up resources properly
 *
 * This harness provides semantic diversity from existing harnesses:
 * - harness_009.cpp: Tests iCCP compression type only (fixed to 0)
 * - harness_018.cpp: Tests writing operations but not compression parameters
 * - This harness: Comprehensive compression parameter configuration with
 *   proper lifecycle sequencing before PNG writing operations
 *
 * The pngwrite.c compression customization functions have significant coverage
 * gaps with all 9 parameter setters completely uncovered. These functions
 * control zlib compression behavior for both IDAT chunks and text chunks,
 * representing 34+ points of undiscovered complexity in compression tuning.
 */


// ==================== MEMORY BUFFER FOR WRITING ====================

struct write_buffer {
    std::vector<uint8_t> data;
    size_t offset;
};

// Custom write function for writing to memory buffer
static void png_write_to_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    write_buffer* buffer = (write_buffer*)io_ptr;
    
    // Append data to buffer
    size_t new_size = buffer->data.size() + length;
    buffer->data.resize(new_size);
    memcpy(buffer->data.data() + buffer->offset, data, length);
    buffer->offset += length;
}

// Custom flush function (required but does nothing for memory buffer)
static void png_flush_memory(png_structp png_ptr) {
    (void)png_ptr;
}

// Callback functions for libpng error handling
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful compression parameter testing
    // We need enough data for: compression parameters, image setup, text data
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Prepare write buffer for custom write function
    write_buffer buffer;
    buffer.offset = 0;
    
    // Create write structure
    png_structp png_ptr = png_create_write_struct(
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
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up error handling via setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set custom write function to memory buffer
    png_set_write_fn(png_ptr, &buffer, png_write_to_memory, png_flush_memory);
    
    // ==================== COMPRESSION PARAMETER CONFIGURATION ====================
    // Set compression parameters BEFORE png_write_info (critical lifecycle sequence)
    
    // 1. IDAT compression parameters (for image data)
    {
        // png_set_compression_level: 0-9 (0=no compression, 9=max compression)
        int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
        png_set_compression_level(png_ptr, compression_level);
        
        // png_set_compression_mem_level: 1-9 (1=min memory, 9=max memory/compression)
        int mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
        png_set_compression_mem_level(png_ptr, mem_level);
        
        // png_set_compression_strategy: 0-4 (zlib strategies)
        // 0=default, 1=filtered, 2=huffman, 3=rle, 4=fixed
        int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
        png_set_compression_strategy(png_ptr, strategy);
        
        // png_set_compression_window_bits: 8-15 (256-32768 byte window)
        int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
        png_set_compression_window_bits(png_ptr, window_bits);
        
        // png_set_compression_method: should be 8 for deflate (PNG standard)
        // Testing edge cases: method != 8 should trigger warning
        int method = fdp.ConsumeIntegralInRange<int>(1, 15);
        png_set_compression_method(png_ptr, method);
    }
    
    // 2. Text compression parameters (for tEXt, zTXt, iTXt chunks)
    {
        // png_set_text_compression_level: 0-9
        int text_compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
        png_set_text_compression_level(png_ptr, text_compression_level);
        
        // png_set_text_compression_mem_level: 1-9
        int text_mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
        png_set_text_compression_mem_level(png_ptr, text_mem_level);
        
        // png_set_text_compression_strategy: 0-4
        int text_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
        png_set_text_compression_strategy(png_ptr, text_strategy);
        
        // png_set_text_compression_window_bits: 8-15
        int text_window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
        png_set_text_compression_window_bits(png_ptr, text_window_bits);
        
        // png_set_text_compression_method: should be 8 for deflate
        int text_method = fdp.ConsumeIntegralInRange<int>(1, 15);
        png_set_text_compression_method(png_ptr, text_method);
    }
    
    // 3. Compression buffer size (from pngset.c)
    {
        // png_set_compression_buffer_size: valid sizes 6-PNG_UINT_31_MAX
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(6, 4096);
        png_set_compression_buffer_size(png_ptr, buffer_size);
    }
    
    // ==================== IMAGE SETUP ====================
    // Set image parameters after compression configuration
    
    // Basic image parameters
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({PNG_COLOR_TYPE_GRAY, 
                                           PNG_COLOR_TYPE_GRAY_ALPHA,
                                           PNG_COLOR_TYPE_PALETTE,
                                           PNG_COLOR_TYPE_RGB,
                                           PNG_COLOR_TYPE_RGB_ALPHA});
    int interlace_type = fdp.PickValueInArray({PNG_INTERLACE_NONE, PNG_INTERLACE_ADAM7});
    int compression_type = PNG_COMPRESSION_TYPE_BASE; // Must be 0 for PNG
    int filter_type = fdp.PickValueInArray({PNG_FILTER_TYPE_BASE, PNG_ALL_FILTERS});
    
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // ==================== TEXT CHUNKS FOR TEXT COMPRESSION TESTING ====================
    // Add text chunks to exercise text compression parameters
    
    {
        // Determine number of text entries (1-3 for reasonable testing)
        int num_text_entries = fdp.ConsumeIntegralInRange<int>(1, 3);
        std::vector<png_text> text_entries(num_text_entries);
        std::vector<std::string> persistent_strings;
        persistent_strings.reserve(num_text_entries * 4);
        
        for (int i = 0; i < num_text_entries; i++) {
            // Consume text compression type
            // -1=PNG_TEXT_COMPRESSION_NONE, 0=PNG_TEXT_COMPRESSION_zTXt, 
            // 1=PNG_ITXT_COMPRESSION_NONE, 2=PNG_ITXT_COMPRESSION_zTXt
            int compression = fdp.PickValueInArray({-1, 0, 1, 2});
            
            // Consume and store keyword (max 79 chars as per PNG spec)
            persistent_strings.push_back(fdp.ConsumeRandomLengthString(79));
            if (persistent_strings.back().empty()) {
                persistent_strings.back() = "Keyword" + std::to_string(i);
            }
            
            // Consume and store text content
            persistent_strings.push_back(fdp.ConsumeRandomLengthString(256));
            if (persistent_strings.back().empty()) {
                persistent_strings.back() = "Text content " + std::to_string(i);
            }
            
            // Set up png_text structure
            text_entries[i].compression = compression;
            text_entries[i].key = const_cast<char*>(persistent_strings[persistent_strings.size() - 2].c_str());
            text_entries[i].text = const_cast<char*>(persistent_strings.back().c_str());
            text_entries[i].text_length = persistent_strings.back().length();
            
            // For iTXt support (compression 1 or 2)
            if (compression == 1 || compression == 2) {
                // Consume and store language for iTXt
                persistent_strings.push_back(fdp.ConsumeRandomLengthString(79));
                
                // Consume and store language key for iTXt
                persistent_strings.push_back(fdp.ConsumeRandomLengthString(256));
                
                text_entries[i].lang = const_cast<char*>(persistent_strings[persistent_strings.size() - 2].c_str());
                text_entries[i].lang_key = const_cast<char*>(persistent_strings.back().c_str());
                text_entries[i].itxt_length = persistent_strings.back().length();
            } else {
                text_entries[i].lang = NULL;
                text_entries[i].lang_key = NULL;
                text_entries[i].itxt_length = 0;
            }
        }
        
        // Set text chunks to exercise text compression
        png_set_text(png_ptr, info_ptr, text_entries.data(), num_text_entries);
    }
    
    // ==================== PNG WRITING WITH CUSTOM COMPRESSION ====================
    // Write the PNG with custom compression settings
    
    png_write_info(png_ptr, info_ptr);
    
    // Create simple image data to compress
    png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    if (row_bytes == 0) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Allocate row pointers
    std::vector<png_bytep> row_pointers(height);
    std::vector<std::vector<png_byte>> row_data(height, std::vector<png_byte>(row_bytes));
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = row_data[y].data();
        // Fill with pattern based on fuzzer input
        for (png_size_t x = 0; x < row_bytes; x++) {
            if (fdp.remaining_bytes() > 0) {
                row_data[y][x] = fdp.ConsumeIntegral<uint8_t>();
            } else {
                row_data[y][x] = static_cast<png_byte>((x + y) % 256);
            }
        }
    }
    
    // Write image data with custom compression
    png_write_image(png_ptr, row_pointers.data());
    
    // Finish writing
    png_write_end(png_ptr, NULL);
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_020
