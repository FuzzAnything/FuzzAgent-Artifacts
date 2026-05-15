#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_017 {
/*
 * Harness for testing libpng progressive/interlaced PNG reading with chunked data feeding.
 * Targets high-complexity, currently uncovered APIs in pngpread.c:
 * - png_progressive_combine_row (8 undiscovered branches, 0% coverage)
 * - png_process_data_skip (5 undiscovered branches, 0% coverage)
 * - png_get_progressive_ptr (2 undiscovered branches, 0% coverage)
 * - png_process_data_pause (17 undiscovered branches, 0% coverage)
 * - png_process_data (56 undiscovered branches, 33.33% coverage)
 * - png_set_progressive_read_fn (required helper)
 * - png_set_progressive_frame_fn (optional for APNG support)
 *
 * Required helper functions:
 * - png_create_read_struct
 * - png_create_info_struct
 * - png_set_read_fn
 * - png_destroy_read_struct
 *
 * This harness implements progressive/interlaced PNG reading with chunked data feeding:
 * 1. Initialize PNG structures with progressive callbacks
 * 2. Set progressive read callbacks with user data pointer
 * 3. Optionally set APNG frame support for animated PNGs
 * 4. Process PNG data in chunks to simulate streaming/progressive download
 * 5. In row callback, call png_progressive_combine_row for interlaced rows
 * 6. Test png_process_data_pause and png_process_data_skip for data flow control
 * 7. Retrieve user pointer with png_get_progressive_ptr
 * 8. Clean up resources properly
 *
 * The pngpread.c module has only 15% coverage - the lowest of all modules.
 * These 6 target APIs represent 92+ undiscovered branches and are completely 
 * (or nearly) uncovered. This harness complements existing harness_002.cpp
 * by focusing on more advanced progressive features including interlacing,
 * data flow control, and APNG support.
 */


// ==================== PROGRESSIVE READING INFRASTRUCTURE ====================

// Structure to hold progressive reading context
struct progressive_context {
    uint8_t* user_data;
    size_t data_size;
    int row_count;
    int frame_count;
    bool has_apng;
};

// Buffer structure for memory I/O in progressive reading
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

// ==================== PROGRESSIVE CALLBACKS ====================

// Progressive info callback - called when PNG header info is available
static void progressive_info_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)info_ptr;
    
    // Retrieve user context to demonstrate png_get_progressive_ptr
    void* user_ptr = png_get_progressive_ptr(png_ptr);
    if (user_ptr != NULL) {
        progressive_context* ctx = (progressive_context*)user_ptr;
        ctx->has_apng = false;  // Will be set if APNG is detected
    }
}

// Progressive row callback - called for each row of image data
static void progressive_row_callback(png_structp png_ptr, png_bytep row, 
                                     png_uint_32 row_num, int pass) {
    (void)row_num;
    
    // Retrieve user context
    void* user_ptr = png_get_progressive_ptr(png_ptr);
    progressive_context* ctx = NULL;
    if (user_ptr != NULL) {
        ctx = (progressive_context*)user_ptr;
        ctx->row_count++;
    }
    
    // For interlaced images (pass > 0), we need to combine rows
    // Note: In real usage, we'd have a buffer of old rows to combine with
    if (pass > 0 && row != NULL) {
        // Create a dummy old row buffer for demonstration
        // In real progressive display, old_row would be the previous partially
        // decoded row for this scanline in the current pass
        png_byte old_row[4096] = {0};  // Dummy buffer
        
        // Call png_progressive_combine_row for interlaced rows
        // This combines new row data with previously received data
        png_progressive_combine_row(png_ptr, old_row, row);
    }
}

// Progressive end callback - called when PNG decoding is complete
static void progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
    
    // Retrieve user context
    void* user_ptr = png_get_progressive_ptr(png_ptr);
    if (user_ptr != NULL) {
        progressive_context* ctx = (progressive_context*)user_ptr;
        // Mark completion
    }
}

// APNG frame info callback (optional)
static void progressive_frame_info_callback(png_structp png_ptr, 
                                           png_uint_32 frame_number) {
    (void)png_ptr;
    
    // Retrieve user context
    void* user_ptr = png_get_progressive_ptr(png_ptr);
    if (user_ptr != NULL) {
        progressive_context* ctx = (progressive_context*)user_ptr;
        ctx->has_apng = true;
        ctx->frame_count++;
    }
}

// APNG frame end callback (optional)
static void progressive_frame_end_callback(png_structp png_ptr, 
                                          png_uint_32 frame_number) {
    (void)png_ptr;
    (void)frame_number;
    
    // Frame processing complete
}

// ==================== MAIN FUZZER FUNCTION ====================

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful progressive PNG operations
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create user context for progressive reading
    progressive_context user_ctx;
    user_ctx.user_data = NULL;
    user_ctx.data_size = 0;
    user_ctx.row_count = 0;
    user_ctx.frame_count = 0;
    user_ctx.has_apng = false;
    
    // Prepare memory buffer for progressive reading
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
    
    // Setup custom read function for streaming
    png_set_read_fn(png_ptr, &buffer, png_read_from_memory);
    
    // Set up progressive read callbacks with user context
    png_set_progressive_read_fn(png_ptr, 
                               &user_ctx,  // progressive_ptr (user data)
                               progressive_info_callback,
                               progressive_row_callback,
                               progressive_end_callback);
    
    // Optionally set APNG frame support based on fuzzer input
    bool enable_apng = fdp.ConsumeBool();
    if (enable_apng) {
#ifdef PNG_READ_APNG_SUPPORTED
        png_set_progressive_frame_fn(png_ptr,
                                     progressive_frame_info_callback,
                                     progressive_frame_end_callback);
#endif
    }
    
    // Determine processing mode from fuzzer input
    uint8_t processing_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    size_t total_processed = 0;
    bool use_pause = false;
    bool use_skip = false;
    
    // Consume configuration from fuzzer input
    if (size > 200) {
        use_pause = fdp.ConsumeBool();
        use_skip = fdp.ConsumeBool();
    }
    
    // Process data in chunks to simulate streaming/progressive download
    while (total_processed < size && buffer.offset < size) {
        // Determine chunk size (1-8192 bytes) based on fuzzer input
        size_t remaining_input = size - buffer.offset;
        size_t max_chunk = remaining_input > 8192 ? 8192 : remaining_input;
        
        // Use different chunk size strategies based on processing mode
        size_t chunk_size;
        switch (processing_mode) {
            case 0:  // Small chunks (1-256 bytes)
                if (max_chunk >= 1) {
                    size_t small_max = max_chunk > 256 ? 256 : max_chunk;
                    chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, small_max);
                } else {
                    chunk_size = max_chunk;  // Should be 0, will break later
                }
                break;
            case 1:  // Medium chunks (256-2048 bytes)
                if (max_chunk >= 256) {
                    size_t medium_max = max_chunk > 2048 ? 2048 : max_chunk;
                    chunk_size = fdp.ConsumeIntegralInRange<size_t>(256, medium_max);
                } else if (max_chunk >= 1) {
                    chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, max_chunk);
                } else {
                    chunk_size = max_chunk;  // Should be 0, will break later
                }
                break;
            case 2:  // Large chunks (2048-8192 bytes)
                if (max_chunk >= 2048) {
                    chunk_size = fdp.ConsumeIntegralInRange<size_t>(2048, max_chunk);
                } else if (max_chunk >= 1) {
                    chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, max_chunk);
                } else {
                    chunk_size = max_chunk;  // Should be 0, will break later
                }
                break;
            case 3:  // Random chunks
            default:
                if (max_chunk >= 1) {
                    chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, max_chunk);
                } else {
                    chunk_size = max_chunk;  // Should be 0, will break later
                }
                break;
        }
        
        if (chunk_size == 0 || buffer.offset + chunk_size > size) {
            break;
        }
        
        // Process this chunk of data with png_process_data
        png_process_data(png_ptr, info_ptr, 
                         (png_bytep)(buffer.data + buffer.offset), 
                         chunk_size);
        
        buffer.offset += chunk_size;
        total_processed += chunk_size;
        
        // Test png_process_data_pause if enabled
        if (use_pause && total_processed > size / 3) {
            // Determine whether to save buffer state
            int save_data = fdp.ConsumeBool() ? 1 : 0;
            
            // Call png_process_data_pause
            size_t remaining = png_process_data_pause(png_ptr, save_data);
            
            // If we saved data and got 0 remaining, we would need to supply same data again
            // For fuzzing simplicity, we just continue
            (void)remaining;  // Mark as used
            
            // Small chance to actually pause and exit
            if (fdp.ConsumeBool() && fdp.ConsumeBool()) {
                break;
            }
        }
        
        // Test png_process_data_skip if enabled (though deprecated)
        if (use_skip && total_processed > size / 2) {
            // Note: png_process_data_skip is deprecated and not fully implemented
            // but we call it to reach the warning/error paths
            png_uint_32 skipped = png_process_data_skip(png_ptr);
            (void)skipped;  // Mark as used
        }
        
        // Occasionally retrieve the progressive pointer to test that API
        if (fdp.ConsumeBool()) {
            void* retrieved_ptr = png_get_progressive_ptr(png_ptr);
            (void)retrieved_ptr;  // Mark as used
            
            // Verify it matches our user context
            if (retrieved_ptr != &user_ctx) {
                // This shouldn't happen, but handle gracefully
            }
        }
        
        // Small chance to break early based on fuzzer input
        if (fdp.ConsumeBool() && fdp.ConsumeProbability<double>() < 0.1) {
            break;
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}

} // namespace harness_017
