#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "png.h"

namespace harness_009 {


// Simple error handling for fuzzing
static void pngtest_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void pngtest_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom read function for reading from memory buffer
struct png_read_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void png_read_from_buffer(png_structp png_ptr, png_bytep out, png_size_t length) {
    png_read_data* rd = (png_read_data*)png_get_io_ptr(png_ptr);
    if (rd->offset + length > rd->size) {
        png_error(png_ptr, "Read beyond buffer");
    }
    memcpy(out, rd->data + rd->offset, length);
    rd->offset += length;
}

// Custom write function for writing to memory buffer
struct png_write_data {
    uint8_t* buffer;
    size_t size;
    size_t capacity;
    size_t offset;
};

static void png_write_to_buffer(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_write_data* wd = (png_write_data*)png_get_io_ptr(png_ptr);
    
    // Check if we need to expand buffer
    if (wd->offset + length > wd->capacity) {
        // For fuzzing, we don't actually need to store all data
        // Just simulate successful write
        wd->offset += length;
        return;
    }
    
    // Write data to buffer
    if (wd->buffer != NULL && wd->offset + length <= wd->capacity) {
        memcpy(wd->buffer + wd->offset, data, length);
    }
    wd->offset += length;
}

static void png_flush_buffer(png_structp png_ptr) {
    (void)png_ptr;
    // Nothing to do for memory buffer
}



//       png_get_valid, png_read_frame_head, png_read_frame_tail,
//       png_write_frame_head, png_write_frame_tail, png_write_frame_data
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        // Need enough data for APNG configuration and frame data
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume APNG configuration from fuzzer input
    uint8_t mode = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0: APNG read, 1: APNG write, 2: Mixed, 3: Progressive
    
    // Consume animation parameters
    png_uint_32 num_frames = fdp.ConsumeIntegralInRange<png_uint_32>(1, 10);  // Reasonable limit for fuzzing
    png_uint_32 num_plays = fdp.ConsumeIntegralInRange<png_uint_32>(0, 10);   // 0 = infinite
    
    // Consume frame parameters
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    png_uint_32 x_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, width-1);
    png_uint_32 y_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, height-1);
    png_uint_16 delay_num = fdp.ConsumeIntegralInRange<png_uint_16>(1, 100);
    png_uint_16 delay_den = fdp.ConsumeIntegralInRange<png_uint_16>(1, 100);
    png_byte dispose_op = fdp.ConsumeIntegralInRange<png_byte>(0, 3);  // PNG_DISPOSE_OP_*
    png_byte blend_op = fdp.ConsumeIntegralInRange<png_byte>(0, 1);    // PNG_BLEND_OP_*
    
    // Consume image format parameters
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({
        PNG_COLOR_TYPE_GRAY,
        PNG_COLOR_TYPE_GRAY_ALPHA,
        PNG_COLOR_TYPE_PALETTE,
        PNG_COLOR_TYPE_RGB,
        PNG_COLOR_TYPE_RGB_ALPHA
    });
    int interlace_type = fdp.PickValueInArray({
        PNG_INTERLACE_NONE,
        PNG_INTERLACE_ADAM7
    });
    
    // Consume remaining bytes for image data
    std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (image_data.size() < (width * height * 4)) {
        // Not enough data for even minimal image, but we'll try anyway
        // APNG APIs may handle this gracefully
    }
    
    // Test APNG reading path
    if (mode == 0 || mode == 2) {
        // First, create a simple PNG with APNG chunks to read
        png_structp write_ptr = NULL;
        png_infop write_info = NULL;
        png_write_data write_buffer = {NULL, 0, 65536, 0};
        
        write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, pngtest_error, pngtest_warning);
        if (write_ptr != NULL) {
            if (setjmp(png_jmpbuf(write_ptr))) {
                if (write_buffer.buffer != NULL) {
                    free(write_buffer.buffer);
                }
                png_destroy_write_struct(&write_ptr, &write_info);
                // Continue to reading test
            } else {
                write_info = png_create_info_struct(write_ptr);
                if (write_info != NULL) {
                    // Set up write buffer
                    write_buffer.buffer = (uint8_t*)malloc(65536);
                    if (write_buffer.buffer != NULL) {
                        png_set_write_fn(write_ptr, &write_buffer, png_write_to_buffer, png_flush_buffer);
                        
                        // Write basic PNG header
                        png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                                     interlace_type, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
                        
                        // Set APNG animation control
                        png_set_acTL(write_ptr, write_info, num_frames, num_plays);
                        
                        // Write IHDR
                        png_write_info(write_ptr, write_info);
                        
                        // Write some dummy image data
                        png_bytep row = (png_bytep)png_malloc(write_ptr, png_get_rowbytes(write_ptr, write_info));
                        if (row != NULL) {
                            memset(row, 0, png_get_rowbytes(write_ptr, write_info));
                            for (png_uint_32 y = 0; y < height; y++) {
                                png_write_row(write_ptr, row);
                            }
                            // Don't free row here - we'll use it for APNG frame data
                        }
                        
                        // Write fcTL for first frame
                        png_set_next_frame_fcTL(write_ptr, write_info, width, height, 
                                               x_offset, y_offset, delay_num, delay_den, 
                                               dispose_op, blend_op);
                        
                        png_write_frame_head(write_ptr, write_info, NULL, width, height,
                                            x_offset, y_offset, delay_num, delay_den,
                                            dispose_op, blend_op);
                        
                        // Write frame data using the same row buffer
                        if (row != NULL) {
                            for (png_uint_32 y = 0; y < height; y++) {
                                png_write_row(write_ptr, row);
                            }
                            // Now free the row buffer after all uses
                            png_free(write_ptr, row);
                        }
                        
                        png_write_frame_tail(write_ptr, write_info);
                        png_write_end(write_ptr, write_info);
                        
                        free(write_buffer.buffer);
                    }
                }
                png_destroy_write_struct(&write_ptr, &write_info);
            }
        }
    }
    
    // Test APNG reading of existing data (simulated)
    if (mode == 0 || mode == 2 || mode == 3) {
        png_structp read_ptr = NULL;
        png_infop read_info = NULL;
        png_infop end_info = NULL;
        
        read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngtest_error, pngtest_warning);
        if (read_ptr != NULL) {
            if (setjmp(png_jmpbuf(read_ptr))) {
                png_destroy_read_struct(&read_ptr, &read_info, &end_info);
                // Continue to next test
            } else {
                read_info = png_create_info_struct(read_ptr);
                if (read_info != NULL) {
                    end_info = png_create_info_struct(read_ptr);
                    if (end_info != NULL) {
                        // Set up read from our fuzzer data
                        png_read_data read_buffer = {data, size, 0};
                        png_set_read_fn(read_ptr, &read_buffer, png_read_from_buffer);
                        png_set_sig_bytes(read_ptr, 0);
                        
                        // Try to read PNG info
                        png_read_info(read_ptr, read_info);
                        
                        // Check if it's a valid APNG
                        png_uint_32 valid = png_get_valid(read_ptr, read_info, PNG_INFO_acTL);
                        if (valid & PNG_INFO_acTL) {
                            // It's an APNG - get animation info
                            png_uint_32 actual_frames, actual_plays;
                            png_get_acTL(read_ptr, read_info, &actual_frames, &actual_plays);
                            
                            png_uint_32 num_frames_read = png_get_num_frames(read_ptr, read_info);
                            png_uint_32 num_plays_read = png_get_num_plays(read_ptr, read_info);
                            
                            // Try to read frame control
                            png_uint_32 frame_width, frame_height, frame_x, frame_y;
                            png_uint_16 frame_delay_num, frame_delay_den;
                            png_byte frame_dispose, frame_blend;
                            
                            png_get_next_frame_fcTL(read_ptr, read_info, &frame_width, &frame_height,
                                                   &frame_x, &frame_y, &frame_delay_num, &frame_delay_den,
                                                   &frame_dispose, &frame_blend);
                            
                            // Check if first frame is hidden
                            png_byte is_hidden = png_get_first_frame_is_hidden(read_ptr, read_info);
                            
                            // Note: png_set_progressive_frame_fn requires callbacks with specific signatures
                            // Skipping progressive APNG reading for now to avoid compilation errors
                            
                            // Get actual image dimensions from PNG header
                            png_uint_32 actual_width, actual_height;
                            int actual_bit_depth, actual_color_type, actual_interlace_method;
                            int actual_compression_method, actual_filter_method;
                            
                            png_get_IHDR(read_ptr, read_info, &actual_width, &actual_height,
                                         &actual_bit_depth, &actual_color_type, &actual_interlace_method,
                                         &actual_compression_method, &actual_filter_method);
                            
                            // Get rowbytes before allocation using actual dimensions
                            png_size_t rowbytes = png_get_rowbytes(read_ptr, read_info);
                            png_bytep* row_pointers = (png_bytep*)png_malloc(read_ptr, actual_height * sizeof(png_bytep));
                            if (row_pointers != NULL) {
                                for (png_uint_32 y = 0; y < actual_height; y++) {
                                    row_pointers[y] = (png_bytep)png_malloc(read_ptr, rowbytes);
                                }
                                png_read_image(read_ptr, row_pointers);
                                
                                for (png_uint_32 y = 0; y < actual_height; y++) {
                                    png_free(read_ptr, row_pointers[y]);
                                }
                                png_free(read_ptr, row_pointers);
                            }
                            
                            // Note: There's no png_read_frame_tail API in libpng
                            // Only png_write_frame_tail exists for writing APNG
                        }
                    }
                }
            }
        }
    }

    // Test APNG writing path
    if (mode == 1 || mode == 2) {
        png_structp apng_write_ptr = NULL;
        png_infop apng_write_info = NULL;
        png_write_data apng_write_buffer = {NULL, 0, 65536, 0};

        apng_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, pngtest_error, pngtest_warning);
        if (apng_write_ptr != NULL) {
            if (setjmp(png_jmpbuf(apng_write_ptr))) {
                if (apng_write_buffer.buffer != NULL) {
                    free(apng_write_buffer.buffer);
                }
                png_destroy_write_struct(&apng_write_ptr, &apng_write_info);
                // Continue execution
            } else {
                apng_write_info = png_create_info_struct(apng_write_ptr);
                if (apng_write_info != NULL) {
                    // Set up write buffer
                    apng_write_buffer.buffer = (uint8_t*)malloc(65536);
                    if (apng_write_buffer.buffer != NULL) {
                        png_set_write_fn(apng_write_ptr, &apng_write_buffer, png_write_to_buffer, png_flush_buffer);
                        
                        // Write PNG with APNG extension
                        png_set_IHDR(apng_write_ptr, apng_write_info, width, height, bit_depth, color_type,
                                     interlace_type, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
                        
                        // Set APNG animation control
                        png_set_acTL(apng_write_ptr, apng_write_info, num_frames, num_plays);
                        
                        // Write IHDR
                        png_write_info(apng_write_ptr, apng_write_info);
                        
                        // Allocate row pointers for writing
                        png_size_t rowbytes = png_get_rowbytes(apng_write_ptr, apng_write_info);
                        png_bytep* row_pointers = (png_bytep*)png_malloc(apng_write_ptr, height * sizeof(png_bytep));
                        if (row_pointers != NULL && rowbytes > 0) {
                            // Create dummy image data from fuzzer input
                            size_t data_offset = 0;
                            for (png_uint_32 y = 0; y < height; y++) {
                                row_pointers[y] = (png_bytep)png_malloc(apng_write_ptr, rowbytes);
                                if (row_pointers[y] != NULL) {
                                    size_t bytes_to_copy = (data_offset + rowbytes <= image_data.size()) ? 
                                                          rowbytes : image_data.size() - data_offset;
                                    if (bytes_to_copy > 0) {
                                        memcpy(row_pointers[y], image_data.data() + data_offset, bytes_to_copy);
                                        data_offset += bytes_to_copy;
                                    } else {
                                        memset(row_pointers[y], 0, rowbytes);
                                    }
                                }
                            }
                            
                            // Write first frame
                            png_set_next_frame_fcTL(apng_write_ptr, apng_write_info, width, height,
                                                   x_offset, y_offset, delay_num, delay_den,
                                                   dispose_op, blend_op);
                            
                            png_write_frame_head(apng_write_ptr, apng_write_info, row_pointers,
                                                width, height, x_offset, y_offset,
                                                delay_num, delay_den, dispose_op, blend_op);
                            
                            // Write multiple frames based on num_frames with bounds checking
                            for (png_uint_32 frame = 0; frame < num_frames && frame < 5; frame++) {
                                // Write frame data
                                for (png_uint_32 y = 0; y < height; y++) {
                                    png_write_row(apng_write_ptr, row_pointers[y]);
                                }
                                
                                if (frame < num_frames - 1) {
                                    // Write frame tail and prepare for next frame
                                    png_write_frame_tail(apng_write_ptr, apng_write_info);
                                    
                                    // Calculate next frame dimensions with bounds checking
                                    png_uint_32 next_width = (frame * 10 < width) ? width - (frame * 10) : 1;
                                    png_uint_32 next_height = (frame * 10 < height) ? height - (frame * 10) : 1;
                                    png_uint_32 next_x_offset = x_offset + (frame * 5);
                                    png_uint_32 next_y_offset = y_offset + (frame * 5);
                                    
                                    // Ensure offsets don't exceed image bounds
                                    if (next_x_offset >= width) next_x_offset = width - 1;
                                    if (next_y_offset >= height) next_y_offset = height - 1;
                                    
                                    // Ensure frame fits within image
                                    if (next_x_offset + next_width > width) next_width = width - next_x_offset;
                                    if (next_y_offset + next_height > height) next_height = height - next_y_offset;
                                    
                                    // Ensure minimum dimensions
                                    if (next_width < 1) next_width = 1;
                                    if (next_height < 1) next_height = 1;
                                    
                                    png_set_next_frame_fcTL(apng_write_ptr, apng_write_info, 
                                                           next_width, next_height,
                                                           next_x_offset, next_y_offset,
                                                           delay_num + frame, delay_den,
                                                           (dispose_op + frame) % 4, (blend_op + frame) % 2);
                                    
                                    png_write_frame_head(apng_write_ptr, apng_write_info, row_pointers,
                                                        next_width, next_height,
                                                        next_x_offset, next_y_offset,
                                                        delay_num + frame, delay_den,
                                                        (dispose_op + frame) % 4, (blend_op + frame) % 2);
                                }
                            }
                            
                            png_write_frame_tail(apng_write_ptr, apng_write_info);
                            png_write_end(apng_write_ptr, apng_write_info);
                            
                            // Clean up row pointers
                            for (png_uint_32 y = 0; y < height; y++) {
                                if (row_pointers[y] != NULL) {
                                    png_free(apng_write_ptr, row_pointers[y]);
                                }
                            }
                            png_free(apng_write_ptr, row_pointers);
                        }
                        
                        free(apng_write_buffer.buffer);
                    }
                }
                png_destroy_write_struct(&apng_write_ptr, &apng_write_info);
            }
        }
    }
    
    return 0;
}

} // namespace harness_009
