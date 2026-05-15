#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace harness_012 {

// Error handling functions
void user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing - just return
}

void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing - just return
}

// Custom write function for libpng that writes to memory buffer
void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Handle zero-length writes according to libpng's API contract
    if (length == 0) {
        return;  // Nothing to write
    }
    
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) return;
    
    // Cast to vector pointer and append data
    std::vector<uint8_t>* buffer = static_cast<std::vector<uint8_t>*>(io_ptr);
    buffer->insert(buffer->end(), data, data + length);
}

// Flush function (required but can be empty for memory buffers)
void user_flush_data(png_structp png_ptr) {
    (void)png_ptr;
    // Nothing to flush for memory buffer
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for compression configuration testing
    // Required: image parameters, compression configuration values
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int color_type = fdp.ConsumeIntegralInRange<int>(0, 6);
    int interlace_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int compression_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int filter_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Create PNG write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                  NULL, 
                                                  user_error_fn, 
                                                  user_warning_fn);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up error handling using setjmp/longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set up output buffer for writing
    std::vector<uint8_t> output_buffer;
    png_set_write_fn(png_ptr, &output_buffer, user_write_data, user_flush_data);
    
    // Set image header information (Required Setup API)
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 1: png_set_compression_buffer_size (29 undiscovered branches)
    // Set compression buffer size (must be between 6 and PNG_UINT_31_MAX)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_WRITE_SUPPORTED
    if (fdp.remaining_bytes() >= sizeof(size_t)) {
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(6, 10000); // Reasonable range
        png_set_compression_buffer_size(png_ptr, buffer_size);
    }
#endif // PNG_WRITE_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 2: png_set_compression_level (2 undiscovered branches)
    // Set compression level (0-9, where 0 = no compression, 9 = max)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_WRITE_CUSTOMIZE_COMPRESSION_SUPPORTED
    if (fdp.remaining_bytes() >= sizeof(int)) {
        int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
        png_set_compression_level(png_ptr, compression_level);
    }
#endif // PNG_WRITE_CUSTOMIZE_COMPRESSION_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 3: png_set_compression_strategy (2 undiscovered branches)
    // Set compression strategy (zlib strategies: 0=default, 1=filtered, 2=huffman, 3=rle, 4=fixed)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_WRITE_CUSTOMIZE_COMPRESSION_SUPPORTED
    if (fdp.remaining_bytes() >= sizeof(int)) {
        int compression_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
        png_set_compression_strategy(png_ptr, compression_strategy);
    }
#endif // PNG_WRITE_CUSTOMIZE_COMPRESSION_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 4: png_set_compression_window_bits (8 undiscovered branches)
    // Set compression window bits (8-15, where 15 = 32k window)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_WRITE_CUSTOMIZE_COMPRESSION_SUPPORTED
    if (fdp.remaining_bytes() >= sizeof(int)) {
        int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
        png_set_compression_window_bits(png_ptr, window_bits);
    }
#endif // PNG_WRITE_CUSTOMIZE_COMPRESSION_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 5: png_set_compression_mem_level (2 undiscovered branches)
    // Set compression memory level (1-9, where 1=minimum memory, 9=maximum)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_WRITE_CUSTOMIZE_COMPRESSION_SUPPORTED
    if (fdp.remaining_bytes() >= sizeof(int)) {
        int mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
        png_set_compression_mem_level(png_ptr, mem_level);
    }
#endif // PNG_WRITE_CUSTOMIZE_COMPRESSION_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 6: png_set_compression_method (6 undiscovered branches)
    // Set compression method (should be 8 for deflate, but test other values)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_WRITE_CUSTOMIZE_COMPRESSION_SUPPORTED
    if (fdp.remaining_bytes() >= sizeof(int)) {
        int method = fdp.ConsumeIntegralInRange<int>(0, 15);
        png_set_compression_method(png_ptr, method);
    }
#endif // PNG_WRITE_CUSTOMIZE_COMPRESSION_SUPPORTED
    
    // Optional extension: Text compression APIs
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 7: png_set_text_compression_window_bits (8 undiscovered branches)
    // Set text compression window bits (8-15)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED
    if (fdp.remaining_bytes() >= sizeof(int)) {
        int text_window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
        png_set_text_compression_window_bits(png_ptr, text_window_bits);
    }
#endif // PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 8: png_set_text_compression_method (6 undiscovered branches)
    // Set text compression method
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED
    if (fdp.remaining_bytes() >= sizeof(int)) {
        int text_method = fdp.ConsumeIntegralInRange<int>(0, 15);
        png_set_text_compression_method(png_ptr, text_method);
    }
#endif // PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 9: png_set_text_compression_mem_level (2 undiscovered branches)
    // Set text compression memory level (1-9)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED
    if (fdp.remaining_bytes() >= sizeof(int)) {
        int text_mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
        png_set_text_compression_mem_level(png_ptr, text_mem_level);
    }
#endif // PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 10: png_set_text_compression_strategy (2 undiscovered branches)
    // Set text compression strategy (0-4)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED
    if (fdp.remaining_bytes() >= sizeof(int)) {
        int text_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
        png_set_text_compression_strategy(png_ptr, text_strategy);
    }
#endif // PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED
    
    // Now actually write some PNG data to exercise the compression configuration
    // Write PNG info to start the image
    png_write_info(png_ptr, info_ptr);
    
    // Generate some simple image data based on consumed input
    // Use remaining bytes for image data
    if (fdp.remaining_bytes() > 0) {
        size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        if (row_bytes > 0 && height > 0) {
            // Limit to reasonable memory usage
            size_t max_rows = std::min(height, static_cast<png_uint_32>(10));
            size_t data_needed = row_bytes * max_rows;
            
            if (fdp.remaining_bytes() >= data_needed) {
                // Allocate rows
                png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr, max_rows * sizeof(png_bytep));
                
                for (size_t i = 0; i < max_rows; i++) {
                    row_pointers[i] = (png_byte*)png_malloc(png_ptr, row_bytes);
                    std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
                    memcpy(row_pointers[i], row_data.data(), row_bytes);
                }
                
                // Write the rows
                png_write_rows(png_ptr, row_pointers, max_rows);
                
                // Clean up
                for (size_t i = 0; i < max_rows; i++) {
                    png_free(png_ptr, row_pointers[i]);
                }
                png_free(png_ptr, row_pointers);
            }
        }
    }
    
    // Finish writing
    png_write_end(png_ptr, info_ptr);
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_012
