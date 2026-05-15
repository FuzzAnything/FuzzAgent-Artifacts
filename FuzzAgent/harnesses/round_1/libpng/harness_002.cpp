#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_002 {

// Buffer structure for memory I/O in progressive reading
struct prog_buffer {
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
    
    prog_buffer* buffer = (prog_buffer*)io_ptr;
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

// Progressive read callbacks
static void progressive_info_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
    // Called when PNG header info is available
}

static void progressive_row_callback(png_structp png_ptr, png_bytep row, 
                                     png_uint_32 row_num, int pass) {
    (void)png_ptr;
    (void)row;
    (void)row_num;
    (void)pass;
    // Called for each row of image data
    // For interlaced images, png_progressive_combine_row may be needed
}

static void progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
    // Called when PNG decoding is complete
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful progressive PNG operations
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Prepare memory buffer for progressive reading
    struct prog_buffer buffer;
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
    
    // Set up progressive read callbacks
    png_set_progressive_read_fn(png_ptr, 
                               NULL,  // progressive_ptr (user data)
                               progressive_info_callback,
                               progressive_row_callback,
                               progressive_end_callback);
    
    // Setup custom read function for streaming
    png_set_read_fn(png_ptr, &buffer, png_read_from_memory);
    
    // Determine how to process the data based on fuzzer input
    uint8_t processing_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    size_t total_processed = 0;
    
    // Process data in chunks to simulate streaming
    while (total_processed < size && buffer.offset < size) {
        // Determine chunk size (1-4096 bytes)
        size_t max_chunk = size - buffer.offset;
        size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, max_chunk > 4096 ? 4096 : max_chunk);
        
        if (chunk_size == 0 || buffer.offset + chunk_size > size) {
            break;
        }
        
        // Process this chunk of data
        png_process_data(png_ptr, info_ptr, 
                         (png_bytep)(buffer.data + buffer.offset), 
                         chunk_size);
        
        buffer.offset += chunk_size;
        total_processed += chunk_size;
        
        // Optionally pause/resume based on processing mode
        if (processing_mode == 1 && total_processed > size / 2) {
            // Test png_process_data_pause
            int save_data = fdp.ConsumeBool() ? 1 : 0;
            size_t remaining = png_process_data_pause(png_ptr, save_data);
            
            if (save_data && remaining == 0) {
                // If we saved data and got 0 remaining, we need to supply same data again
                // For simplicity in fuzzing, we'll just continue
                buffer.offset -= chunk_size;
                total_processed -= chunk_size;
            }
            
            // Note: png_process_data_skip is not implemented in current libpng
            // So we won't call it even though it's in the API
        }
        
        // Small chance to break early
        if (processing_mode == 2 && fdp.ConsumeBool()) {
            break;
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}

} // namespace harness_002
