#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>

namespace harness_009 {

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

// Custom read function for libpng that reads from memory buffer
void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) return;
    
    FuzzedDataProvider* fdp_ptr = static_cast<FuzzedDataProvider*>(io_ptr);
    
    if (length > 0 && fdp_ptr->remaining_bytes() >= length) {
        std::vector<uint8_t> bytes = fdp_ptr->ConsumeBytes<uint8_t>(length);
        memcpy(data, bytes.data(), length);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for APNG animation testing
    // Required: image parameters, animation control, frame control parameters
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume basic image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int color_type = fdp.ConsumeIntegralInRange<int>(0, 6);
    int interlace_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int compression_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int filter_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Consume APNG animation parameters
    png_uint_32 num_frames = fdp.ConsumeIntegralInRange<png_uint_32>(1, 10);
    png_uint_32 num_plays = fdp.ConsumeIntegralInRange<png_uint_32>(0, 100);
    
    // Consume frame control parameters
    png_uint_32 frame_width = fdp.ConsumeIntegralInRange<png_uint_32>(1, width);
    png_uint_32 frame_height = fdp.ConsumeIntegralInRange<png_uint_32>(1, height);
    png_uint_32 x_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, width - frame_width);
    png_uint_32 y_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, height - frame_height);
    png_uint_16 delay_num = fdp.ConsumeIntegralInRange<png_uint_16>(1, 1000);
    png_uint_16 delay_den = fdp.ConsumeIntegralInRange<png_uint_16>(1, 1000);
    png_byte dispose_op = fdp.ConsumeIntegralInRange<png_byte>(0, 3);
    png_byte blend_op = fdp.ConsumeIntegralInRange<png_byte>(0, 1);
    
    // Check if APNG is supported at compile time
#ifdef PNG_APNG_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // PART 1: Create PNG write structures for APNG writing
    ////////////////////////////////////////////////////////////////////////////
    
    // Create PNG write structure
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                   NULL, 
                                                   user_error_fn, 
                                                   user_warning_fn);
    if (write_ptr == NULL) {
        return 0;
    }
    
    // Create PNG info structure for writing
    png_infop write_info_ptr = png_create_info_struct(write_ptr);
    if (write_info_ptr == NULL) {
        png_destroy_write_struct(&write_ptr, NULL);
        return 0;
    }
    
    // Set up error handling for write structure
    if (setjmp(png_jmpbuf(write_ptr))) {
        png_destroy_write_struct(&write_ptr, &write_info_ptr);
        return 0;
    }
    
    // Set up output buffer for writing
    std::vector<uint8_t> output_buffer;
    png_set_write_fn(write_ptr, &output_buffer, user_write_data, user_flush_data);
    
    // Set image header information (Required Setup API)
    png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 1: png_set_acTL (0% coverage, reached but 0 hits)
    ////////////////////////////////////////////////////////////////////////////
    
#ifdef PNG_WRITE_APNG_SUPPORTED
    png_set_acTL(write_ptr, write_info_ptr, num_frames, num_plays);
#endif
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 2: png_set_next_frame_fcTL (0% coverage, reached but 0 hits)
    ////////////////////////////////////////////////////////////////////////////
    
#ifdef PNG_WRITE_APNG_SUPPORTED
    png_set_next_frame_fcTL(write_ptr, write_info_ptr, 
                           frame_width, frame_height, 
                           x_offset, y_offset,
                           delay_num, delay_den,
                           dispose_op, blend_op);
#endif
    
    // Write PNG info (required before writing frames)
    png_write_info(write_ptr, write_info_ptr);
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 3: png_write_frame_head (40 branches, 18 undiscovered)
    ////////////////////////////////////////////////////////////////////////////
    
#ifdef PNG_WRITE_APNG_SUPPORTED
    // Create dummy row pointers for frame writing
    png_bytepp row_pointers = NULL;
    png_uint_32 rows = frame_height;
    
    // Allocate row pointers (simplified for fuzzing)
    if (rows > 0 && frame_width > 0) {
        size_t row_bytes = png_get_rowbytes(write_ptr, write_info_ptr);
        if (row_bytes > 0 && row_bytes < 10000) { // Reasonable limit
            row_pointers = (png_bytepp)png_malloc(write_ptr, rows * sizeof(png_bytep));
            for (png_uint_32 i = 0; i < rows; i++) {
                row_pointers[i] = (png_bytep)png_malloc(write_ptr, row_bytes);
                // Fill with some data (could be from fuzzer input)
                memset(row_pointers[i], 0, row_bytes);
            }
            
            png_write_frame_head(write_ptr, write_info_ptr, row_pointers,
                                frame_width, frame_height,
                                x_offset, y_offset,
                                delay_num, delay_den,
                                dispose_op, blend_op);
            
            // Free allocated row pointers
            for (png_uint_32 i = 0; i < rows; i++) {
                png_free(write_ptr, row_pointers[i]);
            }
            png_free(write_ptr, row_pointers);
        }
    }
#endif
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 4: png_write_frame_tail (write counterpart)
    ////////////////////////////////////////////////////////////////////////////
    
#ifdef PNG_WRITE_APNG_SUPPORTED
    png_write_frame_tail(write_ptr, write_info_ptr);
#endif
    
    // Finish writing
    png_write_end(write_ptr, write_info_ptr);
    
    // Clean up write structures
    png_destroy_write_struct(&write_ptr, &write_info_ptr);
    
    ////////////////////////////////////////////////////////////////////////////
    // PART 2: Create PNG read structures for APNG reading (if we have output)
    ////////////////////////////////////////////////////////////////////////////
    
    if (output_buffer.size() > 0) {
        // Create PNG read structure
        png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                                     NULL,
                                                     user_error_fn,
                                                     user_warning_fn);
        if (read_ptr == NULL) {
            return 0;
        }
        
        // Create PNG info structure for reading
        png_infop read_info_ptr = png_create_info_struct(read_ptr);
        if (read_info_ptr == NULL) {
            png_destroy_read_struct(&read_ptr, NULL, NULL);
            return 0;
        }
        
        // Set up error handling for read structure
        if (setjmp(png_jmpbuf(read_ptr))) {
            png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
            return 0;
        }
        
        // Set up read function to read from our output buffer
        // We'll use a simple approach: create a new FuzzedDataProvider from output
        std::vector<uint8_t> read_buffer = output_buffer;
        FuzzedDataProvider read_fdp(read_buffer.data(), read_buffer.size());
        png_set_read_fn(read_ptr, &read_fdp, user_read_data);
        
        // Set signature bytes (8 bytes for PNG signature)
        png_set_sig_bytes(read_ptr, 8);
        
        // Read PNG info
        png_read_info(read_ptr, read_info_ptr);
        
        ////////////////////////////////////////////////////////////////////////
        // TARGET API 5: png_get_acTL (10 branches, 10 undiscovered)
        ////////////////////////////////////////////////////////////////////////
        
#ifdef PNG_READ_APNG_SUPPORTED
        png_uint_32 read_num_frames = 0;
        png_uint_32 read_num_plays = 0;
        png_get_acTL(read_ptr, read_info_ptr, &read_num_frames, &read_num_plays);
#endif
        
        ////////////////////////////////////////////////////////////////////////
        // TARGET API 6: png_get_next_frame_fcTL (22 branches, 22 undiscovered)
        ////////////////////////////////////////////////////////////////////////
        
#ifdef PNG_READ_APNG_SUPPORTED
        png_uint_32 read_width = 0, read_height = 0;
        png_uint_32 read_x_offset = 0, read_y_offset = 0;
        png_uint_16 read_delay_num = 0, read_delay_den = 0;
        png_byte read_dispose_op = 0, read_blend_op = 0;
        
        png_get_next_frame_fcTL(read_ptr, read_info_ptr,
                               &read_width, &read_height,
                               &read_x_offset, &read_y_offset,
                               &read_delay_num, &read_delay_den,
                               &read_dispose_op, &read_blend_op);
#endif
        
        ////////////////////////////////////////////////////////////////////////
        // TARGET API 7: png_read_frame_head (58 branches, 42 undiscovered)
        ////////////////////////////////////////////////////////////////////////
        
#ifdef PNG_READ_APNG_SUPPORTED
        // Note: png_read_frame_head requires APNG-specific setup and valid state
        // We'll try to call it, but it may fail if the PNG isn't properly set up as APNG
        // This is okay for fuzzing - we want to explore error paths too
        png_read_frame_head(read_ptr, read_info_ptr);
#endif
        
        // Clean up read structures
        png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
    }
    
#else // PNG_APNG_SUPPORTED not defined
    // If APNG is not supported at compile time, just return
    (void)width; (void)height; (void)bit_depth; (void)color_type;
    (void)interlace_type; (void)compression_type; (void)filter_type;
    (void)num_frames; (void)num_plays;
    (void)frame_width; (void)frame_height; (void)x_offset; (void)y_offset;
    (void)delay_num; (void)delay_den; (void)dispose_op; (void)blend_op;
#endif // PNG_APNG_SUPPORTED
    
    return 0;
}

} // namespace harness_009
