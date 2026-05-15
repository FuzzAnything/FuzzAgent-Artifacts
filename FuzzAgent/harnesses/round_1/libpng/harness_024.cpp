#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_024 {
/*
 * Harness for testing 8 completely uncovered PNG getter functions with 76+ undiscovered
 * branches in pngget.c (30.69% branch coverage). Targets PNG chunk data extraction
 * functions that are critical for accessing PNG metadata but remain largely untested.
 * 
 * Target APIs - 8 completely uncovered PNG getter functions:
 * 1. png_get_palette_max: Get maximum palette index (14+ undiscovered branches)
 * 2. png_get_sPLT: Get suggested palette chunk (12+ undiscovered branches)
 * 3. png_get_io_state: Get current I/O state (10+ undiscovered branches)
 * 4. png_get_io_chunk_type: Get current chunk type (10+ undiscovered branches)
 * 5. png_get_compression_buffer_size: Get compression buffer size (10+ undiscovered branches)
 * 6. png_get_user_chunk_ptr: Get user chunk pointer (10+ undiscovered branches)
 * 7. png_get_user_width_max: Get maximum user width (10+ undiscovered branches)
 * 8. png_get_user_height_max: Get maximum user height (10+ undiscovered branches)
 * 
 * Total: 76+ undiscovered branches across these 8 getter functions
 * 
 * These functions are critical for:
 * - Palette management and validation
 * - I/O state tracking during PNG processing
 * - Buffer management for compression
 * - User-defined chunk handling
 * - Dimension validation for custom PNG extensions
 * 
 * Required helper functions for complete lifecycle:
 * - png_create_read_struct, png_create_info_struct
 * - png_set_IHDR for basic image setup
 * - png_set_PLTE for palette setup
 * - png_set_sPLT for suggested palette
 * - png_set_read_user_chunk_fn for user chunk handling
 * - png_set_user_limits for user dimension limits
 * - png_read_info, png_read_image, png_read_end
 * - png_destroy_read_struct
 * 
 * This harness provides semantic diversity from existing harnesses:
 * - harness_005.cpp: APNG-related getters (num_frames, num_plays, etc.)
 * - harness_010.cpp: Basic chunk getters (bKGD, hIST, etc.)
 * - harness_015.cpp: Physical dimension getters (oFFs, pixels_per_meter, etc.)
 * - This harness: Palette, I/O state, compression buffer, and user limit getters
 *   targeting completely uncovered functions in pngget.c with 76+ undiscovered branches
 * 
 * The pngget.c module has only 30.69% branch coverage, indicating large portions
 * of getter function logic remain untested. These 8 functions represent significant
 * coverage gaps in PNG metadata extraction and state management.
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

// User chunk callback for testing png_get_user_chunk_ptr
static int user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    (void)png_ptr;
    (void)chunk;
    // Return 1 to keep chunk, 0 to discard
    return 1;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful PNG operations with palette and chunk data
    // We need: image setup, palette data, sPLT data, and PNG content
    if (size < 500) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Prepare memory buffer for custom read function
    struct mem_buffer buffer;
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
    
    // Consume configuration parameters from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Set up basic image parameters for PNG processing
    png_set_IHDR(png_ptr, info_ptr,
        fdp.ConsumeIntegralInRange<png_uint_32>(1, 4096),  // width
        fdp.ConsumeIntegralInRange<png_uint_32>(1, 4096),  // height
        fdp.ConsumeIntegralInRange<int>(1, 16),           // bit_depth
        fdp.ConsumeIntegral<uint8_t>() % 7,               // color_type
        PNG_INTERLACE_NONE,                               // interlace_type
        PNG_COMPRESSION_TYPE_DEFAULT,                     // compression_type
        PNG_FILTER_TYPE_DEFAULT);                         // filter_type
    
    // Set up palette for testing palette-related getters
    if (operation == 0 || operation == 1) {
        // Create palette data
        size_t palette_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::vector<png_color> palette(palette_size);
        for (size_t i = 0; i < palette_size && fdp.remaining_bytes() >= 3; i++) {
            palette[i].red = fdp.ConsumeIntegral<uint8_t>();
            palette[i].green = fdp.ConsumeIntegral<uint8_t>();
            palette[i].blue = fdp.ConsumeIntegral<uint8_t>();
        }
        
        if (palette_size > 0) {
            png_set_PLTE(png_ptr, info_ptr, palette.data(), palette_size);
        }
    }
    
    // Set up suggested palette (sPLT) for testing png_get_sPLT
    if (operation == 0 || operation == 2) {
        // Create sPLT entry data
        size_t splt_entries = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        if (splt_entries > 0 && fdp.remaining_bytes() >= splt_entries * 6) {
            png_sPLT_t splt;
            memset(&splt, 0, sizeof(splt));
            
            // Set basic sPLT properties
            splt.name = (png_charp)"test_splt";
            splt.depth = 8;
            splt.nentries = splt_entries;
            
            // Create entries array
            std::vector<png_sPLT_entry> entries(splt_entries);
            for (size_t i = 0; i < splt_entries; i++) {
                entries[i].red = fdp.ConsumeIntegral<uint16_t>();
                entries[i].green = fdp.ConsumeIntegral<uint16_t>();
                entries[i].blue = fdp.ConsumeIntegral<uint16_t>();
                entries[i].alpha = fdp.ConsumeIntegral<uint16_t>();
                entries[i].frequency = fdp.ConsumeIntegral<uint16_t>();
            }
            splt.entries = entries.data();
            
            png_set_sPLT(png_ptr, info_ptr, &splt, 1);
        }
    }
    
    // Set user chunk handler for testing png_get_user_chunk_ptr
    if (operation == 0 || operation == 3) {
        png_set_read_user_chunk_fn(png_ptr, (png_voidp)&fdp, user_chunk_callback);
    }
    
    // Set user limits for testing png_get_user_width_max/height_max
    png_set_user_limits(png_ptr,
        fdp.ConsumeIntegralInRange<png_uint_32>(100, 10000),  // user_width_max
        fdp.ConsumeIntegralInRange<png_uint_32>(100, 10000)); // user_height_max
    
    // Try to read PNG info to populate internal structures
    // This will fail for non-PNG data, but that's OK for getter testing
    png_read_info(png_ptr, info_ptr);
    
    // ==================== TARGET GETTER FUNCTIONS ====================
    
    // 1. png_get_palette_max - Get maximum palette index (14+ undiscovered branches)
    int palette_max = png_get_palette_max(png_ptr, info_ptr);
    (void)palette_max; // Use result to avoid unused variable warning
    
    // 2. png_get_sPLT - Get suggested palette chunk (12+ undiscovered branches)
    png_sPLT_tp splt_ptr;
    int splt_count = png_get_sPLT(png_ptr, info_ptr, &splt_ptr);
    (void)splt_count;
    (void)splt_ptr;
    
    // 3. png_get_io_state - Get current I/O state (10+ undiscovered branches)
    png_uint_32 io_state = png_get_io_state(png_ptr);
    (void)io_state;
    
    // 4. png_get_io_chunk_type - Get current chunk type (10+ undiscovered branches)
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr);
    (void)chunk_type;
    
    // 5. png_get_compression_buffer_size - Get compression buffer size (10+ undiscovered branches)
    png_uint_32 comp_buf_size = png_get_compression_buffer_size(png_ptr);
    (void)comp_buf_size;
    
    // 6. png_get_user_chunk_ptr - Get user chunk pointer (10+ undiscovered branches)
    png_voidp user_chunk_ptr = png_get_user_chunk_ptr(png_ptr);
    (void)user_chunk_ptr;
    
    // 7. png_get_user_width_max - Get maximum user width (10+ undiscovered branches)
    png_uint_32 user_width_max = png_get_user_width_max(png_ptr);
    (void)user_width_max;
    
    // 8. png_get_user_height_max - Get maximum user height (10+ undiscovered branches)
    png_uint_32 user_height_max = png_get_user_height_max(png_ptr);
    (void)user_height_max;
    
    // ==================== ADDITIONAL GETTERS FOR COMPREHENSIVE COVERAGE ====================
    
    // Test chunk cache and malloc limits getters
    png_uint_32 chunk_cache_max = png_get_chunk_cache_max(png_ptr);
    (void)chunk_cache_max;
    
    png_uint_32 chunk_malloc_max = png_get_chunk_malloc_max(png_ptr);
    (void)chunk_malloc_max;
    
    // Test rgb_to_gray status getter
    png_byte rgb_to_gray_status = png_get_rgb_to_gray_status(png_ptr);
    (void)rgb_to_gray_status;
    
    // Try to read image data if PNG structure is valid
    // This exercises I/O state transitions
    if (operation == 0) {
        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        
        if (width > 0 && height > 0 && width < 1000 && height < 1000) {
            png_byte color_type = png_get_color_type(png_ptr, info_ptr);
            png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
            
            // Calculate row bytes
            png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
            
            if (row_bytes > 0 && row_bytes < 10000) {
                // Allocate row pointers
                png_bytep row_pointers[height];
                for (png_uint_32 y = 0; y < height; y++) {
                    row_pointers[y] = (png_bytep)png_malloc(png_ptr, row_bytes);
                    if (row_pointers[y] == NULL) {
                        // Clean up allocated rows
                        for (png_uint_32 i = 0; i < y; i++) {
                            png_free(png_ptr, row_pointers[i]);
                        }
                        break;
                    }
                }
                
                // Read image data
                png_read_image(png_ptr, row_pointers);
                
                // Free row pointers
                for (png_uint_32 y = 0; y < height; y++) {
                    if (row_pointers[y] != NULL) {
                        png_free(png_ptr, row_pointers[y]);
                    }
                }
                
                // Read end of PNG
                png_read_end(png_ptr, info_ptr);
            }
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}

} // namespace harness_024
