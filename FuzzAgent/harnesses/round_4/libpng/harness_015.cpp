#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_015 {

// Error handling callbacks for libpng
static void pngtest_error(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

static void pngtest_warning(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

// Progressive read callbacks - more comprehensive than harness_003
static void progressive_info_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
    // Called when info is available - try to get some info
    if (png_ptr && info_ptr) {
        // Try to get image dimensions
        png_get_image_width(png_ptr, info_ptr);
        png_get_image_height(png_ptr, info_ptr);
        png_get_color_type(png_ptr, info_ptr);
        png_get_bit_depth(png_ptr, info_ptr);
    }
}

static void progressive_row_callback(png_structp png_ptr, png_bytep new_row,
                                     png_uint_32 row_num, int pass) {
    (void)png_ptr;
    (void)new_row;
    (void)row_num;
    (void)pass;
    // Called for each row as it's decoded
    
    // If we have a valid new_row and png_ptr, test png_progressive_combine_row
    if (png_ptr && new_row) {
        // Create a mock old row for testing combine functionality
        png_byte old_row[4096] = {0}; // Buffer for testing
        
        // Call png_progressive_combine_row to test interlaced image handling
        // Note: This requires PNG_READ_INTERLACING_SUPPORTED
        #ifdef PNG_READ_INTERLACING_SUPPORTED
        png_progressive_combine_row(png_ptr, old_row, new_row);
        #endif
    }
}

static void progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
    // Called when decoding is complete
}

// Custom read function for memory buffer
struct png_memory_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void pngtest_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_memory_buffer* buffer = (png_memory_buffer*)png_get_io_ptr(png_ptr);
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for progressive reading operations
    // - PNG signature: 8 bytes
    // - Basic PNG header chunk: ~25 bytes  
    // - At least 2 chunks for progressive processing: ~100 bytes
    // Total minimum: ~150 bytes
    if (size < 150) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a memory buffer for PNG operations
    png_memory_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    
    // ============================================
    // PART 1: PROGRESSIVE READING SETUP
    // ============================================
    
    // Create read structure for progressive reading
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // ============================================
    // PART 2: SETUP PROGRESSIVE READ FUNCTIONS
    // ============================================
    
    // Target: png_set_progressive_read_fn (5 undiscovered branches)
    png_set_progressive_read_fn(png_ptr, 
                               nullptr,  // progressive_ptr
                               progressive_info_callback,
                               progressive_row_callback,
                               progressive_end_callback);
    
    // Set up custom read functions
    png_set_read_fn(png_ptr, &buffer, pngtest_read_data);
    
    // ============================================
    // PART 3: PROGRESSIVE DATA PROCESSING
    // ============================================
    
    // Consume parameters for progressive processing
    uint8_t processing_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    size_t max_chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    bool use_pause = fdp.ConsumeBool();
    bool use_skip = fdp.ConsumeBool();
    
    // Process data in chunks to simulate progressive reading
    size_t total_processed = 0;
    
    while (buffer.offset < size && total_processed < size) {
        // Determine chunk size for this iteration
        size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, max_chunk_size);
        if (chunk_size > size - buffer.offset) {
            chunk_size = size - buffer.offset;
        }
        
        if (chunk_size == 0) {
            break;
        }
        
        // Target: png_process_data (60 undiscovered branches)
        png_process_data(png_ptr, info_ptr, 
                        const_cast<png_byte*>(data + buffer.offset), chunk_size);
        
        total_processed += chunk_size;
        buffer.offset += chunk_size;
        
        // Occasionally test pause functionality
        if (use_pause && (total_processed % 128 == 0)) {
            // Target: png_process_data_pause (19 undiscovered branches)
            int save_state = fdp.ConsumeBool() ? 1 : 0;
            size_t remaining = png_process_data_pause(png_ptr, save_state);
            (void)remaining; // Use result to avoid unused warning
            
            // Simulate resuming after pause
            if (save_state) {
                // Process a small amount more after pause
                size_t resume_chunk = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                if (resume_chunk > size - buffer.offset) {
                    resume_chunk = size - buffer.offset;
                }
                if (resume_chunk > 0) {
                    png_process_data(png_ptr, info_ptr, 
                                    const_cast<png_byte*>(data + buffer.offset), resume_chunk);
                    buffer.offset += resume_chunk;
                    total_processed += resume_chunk;
                }
            }
        }
        
        // Test skip functionality (though deprecated)
        if (use_skip && (total_processed % 256 == 0)) {
            // Target: png_process_data_skip (7 undiscovered branches)
            #ifndef PNG_DEPRECATED
            png_uint_32 skip_result = png_process_data_skip(png_ptr);
            (void)skip_result;
            #endif
        }
        
        // Break early if we've processed enough
        if (total_processed >= 1024 * 1024) { // Cap at 1MB processed
            break;
        }
    }
    
    // ============================================
    // PART 4: TEST INTERLACED IMAGE SCENARIOS
    // ============================================
    
    // Create another PNG structure for testing interlaced scenarios
    buffer.offset = 0; // Reset for new test
    
    png_structp png_ptr2 = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (png_ptr2) {
        png_infop info_ptr2 = png_create_info_struct(png_ptr2);
        
        if (info_ptr2) {
            if (!setjmp(png_jmpbuf(png_ptr2))) {
                // Set progressive functions
                png_set_progressive_read_fn(png_ptr2, 
                                           nullptr,
                                           progressive_info_callback,
                                           progressive_row_callback,
                                           progressive_end_callback);
                
                png_set_read_fn(png_ptr2, &buffer, pngtest_read_data);
                
                // Enable interlacing support if available
                #ifdef PNG_READ_INTERLACING_SUPPORTED
                png_set_interlace_handling(png_ptr2);
                #endif
                
                // Process a small amount of data to trigger callbacks
                size_t test_chunk = fdp.ConsumeIntegralInRange<size_t>(8, 256);
                if (test_chunk > size) test_chunk = size;
                
                if (test_chunk > 0) {
                    png_process_data(png_ptr2, info_ptr2, 
                                    const_cast<png_byte*>(data), test_chunk);
                }
                
                png_destroy_read_struct(&png_ptr2, &info_ptr2, nullptr);
            } else {
                png_destroy_read_struct(&png_ptr2, &info_ptr2, nullptr);
            }
        } else {
            png_destroy_read_struct(&png_ptr2, nullptr, nullptr);
        }
    }
    
    // ============================================
    // PART 5: CLEANUP
    // ============================================
    
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_015
