#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_005 {

// Buffer structure for memory I/O
struct mem_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Write buffer structure
struct write_buffer {
    std::vector<uint8_t> data;
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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful APNG operations
    if (size < 200) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Prepare memory buffer for custom read function
    struct mem_buffer read_buf;
    read_buf.data = data;
    read_buf.size = size;
    read_buf.offset = 0;
    
    // Prepare write buffer for custom write function
    struct write_buffer write_buf;
    write_buf.offset = 0;
    
    // Create read structure
    png_structp read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        png_error_callback,
        png_warning_callback);
    
    if (read_ptr == NULL) {
        return 0;
    }
    
    // Create write structure
    png_structp write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        png_error_callback,
        png_warning_callback);
    
    if (write_ptr == NULL) {
        png_destroy_read_struct(&read_ptr, NULL, NULL);
        return 0;
    }
    
    // Create info structure for reading
    png_infop read_info_ptr = png_create_info_struct(read_ptr);
    if (read_info_ptr == NULL) {
        png_destroy_read_struct(&read_ptr, NULL, NULL);
        png_destroy_write_struct(&write_ptr, NULL);
        return 0;
    }
    
    // Create info structure for writing
    png_infop write_info_ptr = png_create_info_struct(write_ptr);
    if (write_info_ptr == NULL) {
        png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
        png_destroy_write_struct(&write_ptr, NULL);
        return 0;
    }
    
    // Set up error handling via setjmp for reading
    if (setjmp(png_jmpbuf(read_ptr))) {
        // Error occurred during PNG processing
        png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
        png_destroy_write_struct(&write_ptr, &write_info_ptr);
        return 0;
    }
    
    // Set up error handling via setjmp for writing
    if (setjmp(png_jmpbuf(write_ptr))) {
        // Error occurred during PNG processing
        png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
        png_destroy_write_struct(&write_ptr, &write_info_ptr);
        return 0;
    }
    
    // Setup custom read function
    png_set_read_fn(read_ptr, &read_buf, png_read_from_memory);
    
    // Setup custom write function
    png_set_write_fn(write_ptr, &write_buf, png_write_to_memory, png_flush_memory);
    
    // Consume animation parameters from fuzzer input
    png_uint_32 num_frames = fdp.ConsumeIntegralInRange<png_uint_32>(1, 10);
    png_uint_32 num_plays = fdp.ConsumeIntegralInRange<png_uint_32>(0, 100);
    
    // Consume frame parameters
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1024);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1024);
    png_uint_32 x_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, width);
    png_uint_32 y_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, height);
    png_uint_16 delay_num = fdp.ConsumeIntegralInRange<png_uint_16>(1, 1000);
    png_uint_16 delay_den = fdp.ConsumeIntegralInRange<png_uint_16>(1, 1000);
    png_byte dispose_op = fdp.ConsumeIntegral<png_byte>() % 4; // 0-3 valid values
    png_byte blend_op = fdp.ConsumeIntegral<png_byte>() % 2; // 0-1 valid values
    
    // Target API 1: png_set_acTL - Set animation control
    png_set_acTL(read_ptr, read_info_ptr, num_frames, num_plays);
    png_set_acTL(write_ptr, write_info_ptr, num_frames, num_plays);
    
    // Target API 2: png_set_next_frame_fcTL - Set frame control
    png_set_next_frame_fcTL(read_ptr, read_info_ptr, 
                           width, height, x_offset, y_offset,
                           delay_num, delay_den, dispose_op, blend_op);
    png_set_next_frame_fcTL(write_ptr, write_info_ptr,
                           width, height, x_offset, y_offset,
                           delay_num, delay_den, dispose_op, blend_op);
    
    // Try to read PNG info (this might fail if input isn't valid PNG)
    png_read_info(read_ptr, read_info_ptr);
    
    // Setup write info based on read info
    png_uint_32 img_width = png_get_image_width(read_ptr, read_info_ptr);
    png_uint_32 img_height = png_get_image_height(read_ptr, read_info_ptr);
    int bit_depth = png_get_bit_depth(read_ptr, read_info_ptr);
    int color_type = png_get_color_type(read_ptr, read_info_ptr);
    int interlace_type = png_get_interlace_type(read_ptr, read_info_ptr);
    int compression_type = png_get_compression_type(read_ptr, read_info_ptr);
    int filter_type = png_get_filter_type(read_ptr, read_info_ptr);
    
    png_set_IHDR(write_ptr, write_info_ptr, img_width, img_height,
                 bit_depth, color_type, interlace_type,
                 compression_type, filter_type);
    
    // Write PNG info
    png_write_info(write_ptr, write_info_ptr);
    
    // Target API 3: Query animation information
    png_uint_32 queried_num_frames = 0;
    png_uint_32 queried_num_plays = 0;
    
    // png_get_acTL
    png_get_acTL(read_ptr, read_info_ptr, &queried_num_frames, &queried_num_plays);
    png_get_acTL(write_ptr, write_info_ptr, &queried_num_frames, &queried_num_plays);
    
    // png_get_num_frames
    png_uint_32 get_num_frames = png_get_num_frames(read_ptr, read_info_ptr);
    get_num_frames = png_get_num_frames(write_ptr, write_info_ptr);
    
    // png_get_num_plays
    png_uint_32 get_num_plays = png_get_num_plays(read_ptr, read_info_ptr);
    get_num_plays = png_get_num_plays(write_ptr, write_info_ptr);
    
    // png_get_next_frame_fcTL
    png_uint_32 frame_width = 0, frame_height = 0;
    png_uint_32 frame_x_offset = 0, frame_y_offset = 0;
    png_uint_16 frame_delay_num = 0, frame_delay_den = 0;
    png_byte frame_dispose_op = 0, frame_blend_op = 0;
    
    png_get_next_frame_fcTL(read_ptr, read_info_ptr,
                           &frame_width, &frame_height,
                           &frame_x_offset, &frame_y_offset,
                           &frame_delay_num, &frame_delay_den,
                           &frame_dispose_op, &frame_blend_op);
    
    png_get_next_frame_fcTL(write_ptr, write_info_ptr,
                           &frame_width, &frame_height,
                           &frame_x_offset, &frame_y_offset,
                           &frame_delay_num, &frame_delay_den,
                           &frame_dispose_op, &frame_blend_op);
    
    // Target API 4: png_read_frame_head
    // Note: This requires valid APNG data in the input
    // We'll attempt it but it might fail if input doesn't contain APNG chunks
    // Similar to pngtest.c, we check for acTL validity first
    if (png_get_valid(read_ptr, read_info_ptr, PNG_INFO_acTL)) {
        png_read_frame_head(read_ptr, read_info_ptr);
    }
    
    // Target API 5: png_write_frame_head
    // Prepare row pointers for writing (dummy data)
    png_bytep row_pointers[1] = {NULL};
    png_size_t row_bytes = png_get_rowbytes(read_ptr, read_info_ptr);
    if (row_bytes > 0 && row_bytes < 100000) {  // Reasonable size limit
        row_pointers[0] = (png_bytep)png_malloc(read_ptr, row_bytes);
        if (row_pointers[0]) {
            // Fill with some dummy data
            memset(row_pointers[0], 0, row_bytes);
            
            if (png_get_valid(write_ptr, write_info_ptr, PNG_INFO_acTL)) {
                png_write_frame_head(write_ptr, write_info_ptr, row_pointers,
                                    width, height, x_offset, y_offset,
                                    delay_num, delay_den, dispose_op, blend_op);
                
                // Target API 6: png_write_frame_tail
                png_write_frame_tail(write_ptr, write_info_ptr);
            }
            
            png_free(read_ptr, row_pointers[0]);
        }
    }
    
    // Cleanup
    png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
    png_destroy_write_struct(&write_ptr, &write_info_ptr);
    
    return 0;
}

} // namespace harness_005
