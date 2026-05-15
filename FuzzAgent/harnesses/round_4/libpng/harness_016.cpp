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

namespace harness_016 {

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

// Progressive read callbacks with user data
struct ProgressiveUserData {
    int callback_count;
    bool use_combine_row;
    size_t max_rows_to_process;
};

static void progressive_info_callback(png_structp png_ptr, png_infop info_ptr) {
    ProgressiveUserData* user_data = (ProgressiveUserData*)png_get_progressive_ptr(png_ptr);
    if (user_data) {
        user_data->callback_count++;
    }
    
    (void)info_ptr;
    // Called when info is available
}

static void progressive_row_callback(png_structp png_ptr, png_bytep new_row,
                                     png_uint_32 row_num, int pass) {
    ProgressiveUserData* user_data = (ProgressiveUserData*)png_get_progressive_ptr(png_ptr);
    if (user_data) {
        user_data->callback_count++;
        
        // Test png_progressive_combine_row if enabled
        if (user_data->use_combine_row && new_row) {
            #ifdef PNG_READ_INTERLACING_SUPPORTED
            // Create a buffer for old row
            png_byte old_row[4096] = {0};
            png_progressive_combine_row(png_ptr, old_row, new_row);
            #endif
        }
    }
    
    (void)row_num;
    (void)pass;
    // Called for each row as it's decoded
}

static void progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    ProgressiveUserData* user_data = (ProgressiveUserData*)png_get_progressive_ptr(png_ptr);
    if (user_data) {
        user_data->callback_count++;
    }
    
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
    // Minimum size for progressive reading with partial signature
    // - PNG signature: 8 bytes
    // - At least some data for processing: 32 bytes
    if (size < 40) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    bool use_create_read_struct_2 = fdp.ConsumeBool();
    bool use_partial_signature = fdp.ConsumeBool();
    size_t partial_sig_bytes = fdp.ConsumeIntegralInRange<size_t>(0, 8);
    bool test_pause_resume = fdp.ConsumeBool();
    bool test_skip = fdp.ConsumeBool();
    bool test_combine_row = fdp.ConsumeBool();
    size_t max_chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 2048);
    size_t max_rows_to_process = fdp.ConsumeIntegralInRange<size_t>(0, 100);
    
    // Create user data for progressive callbacks
    ProgressiveUserData user_data = {
        .callback_count = 0,
        .use_combine_row = test_combine_row,
        .max_rows_to_process = max_rows_to_process
    };
    
    // Create a memory buffer for PNG operations
    png_memory_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    
    // ============================================
    // PART 1: INITIALIZATION WITH ALTERNATIVE API
    // ============================================
    
    png_structp png_ptr = nullptr;
    
    if (use_create_read_struct_2) {
        // Test png_create_read_struct_2 alternative
        png_ptr = png_create_read_struct_2(
            PNG_LIBPNG_VER_STRING,
            &user_data,  // Use user_data as error_ptr for testing
            pngtest_error,
            pngtest_warning,
            nullptr,     // mem_ptr (use default)
            nullptr,     // malloc_fn (use default)
            nullptr      // free_fn (use default)
        );
    } else {
        // Use standard png_create_read_struct
        png_ptr = png_create_read_struct(
            PNG_LIBPNG_VER_STRING,
            &user_data,  // Use user_data as error_ptr for testing
            pngtest_error,
            pngtest_warning
        );
    }
    
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
    // PART 2: SETUP PROGRESSIVE READING
    // ============================================
    
    // Set up custom read functions
    png_set_read_fn(png_ptr, &buffer, pngtest_read_data);
    
    // Test partial signature scenario if requested
    if (use_partial_signature && partial_sig_bytes > 0 && partial_sig_bytes <= 8) {
        // Consume signature bytes from input
        std::vector<uint8_t> sig_bytes = fdp.ConsumeBytes<uint8_t>(partial_sig_bytes);
        if (sig_bytes.size() == partial_sig_bytes) {
            png_set_sig_bytes(png_ptr, partial_sig_bytes);
        }
    }
    
    // Target: png_set_progressive_read_fn (5 undiscovered branches)
    png_set_progressive_read_fn(png_ptr,
                               &user_data,  // progressive_ptr (non-null for png_get_progressive_ptr)
                               progressive_info_callback,
                               progressive_row_callback,
                               progressive_end_callback);
    
    // ============================================
    // PART 3: PROGRESSIVE DATA PROCESSING
    // ============================================
    
    // Process data in variable-sized chunks
    size_t total_processed = 0;
    size_t pause_count = 0;
    size_t skip_count = 0;
    
    while (buffer.offset < size && total_processed < size) {
        // Determine chunk size for this iteration
        size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, max_chunk_size);
        if (chunk_size > size - buffer.offset) {
            chunk_size = size - buffer.offset;
        }
        
        if (chunk_size == 0) {
            break;
        }
        
        // Target: png_process_data (59 undiscovered branches)
        png_process_data(png_ptr, info_ptr,
                        const_cast<png_byte*>(data + buffer.offset), chunk_size);
        
        total_processed += chunk_size;
        buffer.offset += chunk_size;
        
        // Test pause functionality if enabled
        if (test_pause_resume && pause_count < 3 && (total_processed % 64 == 0)) {
            // Target: png_process_data_pause (19 undiscovered branches)
            int save_state = fdp.ConsumeBool() ? 1 : 0;
            size_t remaining = png_process_data_pause(png_ptr, save_state);
            (void)remaining;
            
            pause_count++;
            
            // If we saved state, process a bit more to test resume
            if (save_state && buffer.offset < size) {
                size_t resume_size = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (resume_size > size - buffer.offset) {
                    resume_size = size - buffer.offset;
                }
                
                if (resume_size > 0) {
                    png_process_data(png_ptr, info_ptr,
                                    const_cast<png_byte*>(data + buffer.offset), resume_size);
                    buffer.offset += resume_size;
                    total_processed += resume_size;
                }
            }
        }
        
        // Test skip functionality if enabled (deprecated but still present)
        if (test_skip && skip_count < 2 && (total_processed % 128 == 0)) {
            // Target: png_process_data_skip (7 undiscovered branches)
            #ifndef PNG_DEPRECATED
            png_uint_32 skip_result = png_process_data_skip(png_ptr);
            (void)skip_result;
            #endif
            skip_count++;
        }
        
        // Test png_get_progressive_ptr
        void* retrieved_ptr = png_get_progressive_ptr(png_ptr);
        if (retrieved_ptr != &user_data) {
            // This shouldn't happen if API works correctly
            // But we continue anyway
        }
        
        // Limit processing if we've done enough
        if (total_processed >= 2 * 1024 * 1024) { // Cap at 2MB processed
            break;
        }
        
        // Limit based on row callback count
        if (user_data.max_rows_to_process > 0 && 
            user_data.callback_count > user_data.max_rows_to_process * 3) {
            // Rough estimate: info + rows + end callbacks
            break;
        }
    }
    
    // ============================================
    // PART 4: TEST PROGRESSIVE POINTER RETRIEVAL
    // ============================================
    
    // Create another PNG structure specifically to test png_get_progressive_ptr
    // with different configurations
    {
        png_memory_buffer buffer2;
        buffer2.data = data;
        buffer2.size = size;
        buffer2.offset = 0;
        
        ProgressiveUserData user_data2 = {0, false, 0};
        
        png_structp png_ptr2 = png_create_read_struct(
            PNG_LIBPNG_VER_STRING,
            nullptr,
            pngtest_error,
            pngtest_warning
        );
        
        if (png_ptr2) {
            png_infop info_ptr2 = png_create_info_struct(png_ptr2);
            
            if (info_ptr2) {
                if (!setjmp(png_jmpbuf(png_ptr2))) {
                    // Set read function
                    png_set_read_fn(png_ptr2, &buffer2, pngtest_read_data);
                    
                    // Set progressive functions with different user data
                    png_set_progressive_read_fn(png_ptr2,
                                               &user_data2,
                                               progressive_info_callback,
                                               progressive_row_callback,
                                               progressive_end_callback);
                    
                    // Verify png_get_progressive_ptr returns correct pointer
                    void* ptr_check = png_get_progressive_ptr(png_ptr2);
                    if (ptr_check != &user_data2) {
                        // Mismatch, but continue
                    }
                    
                    // Process a small amount of data
                    size_t test_chunk = fdp.ConsumeIntegralInRange<size_t>(8, 128);
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
    }
    
    // ============================================
    // PART 5: CLEANUP
    // ============================================
    
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_016
