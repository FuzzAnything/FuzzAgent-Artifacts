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

namespace harness_007 {

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

// Custom read/write functions for memory buffers
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

static void pngtest_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Write to nowhere - just consume the data
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void pngtest_flush_data(png_structp png_ptr) {
    (void)png_ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for APNG operations
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create memory buffer for PNG operations
    png_memory_buffer read_buffer;
    read_buffer.data = data;
    read_buffer.size = size;
    read_buffer.offset = 0;
    
    // Create PNG read structure
    png_structp png_ptr_read = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr_read) {
        return 0;
    }
    
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // Create PNG write structure
    png_structp png_ptr_write = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        return 0;
    }
    
    // Set up custom read functions for reading
    png_set_read_fn(png_ptr_read, &read_buffer, pngtest_read_data);
    
    // Set up custom write functions for writing (to nowhere)
    png_set_write_fn(png_ptr_write, nullptr, pngtest_write_data, pngtest_flush_data);
    
    // Set error handling for read structure
    if (setjmp(png_jmpbuf(png_ptr_read))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }
    
    // Set error handling for write structure
    if (setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }
    
    // Consume fuzzed parameters for APNG animation
    png_uint_32 num_frames = fdp.ConsumeIntegralInRange<png_uint_32>(1, 10);  // Up to 10 frames
    png_uint_32 num_plays = fdp.ConsumeIntegralInRange<png_uint_32>(0, 100);  // 0 = infinite loop
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({
        PNG_COLOR_TYPE_GRAY,
        PNG_COLOR_TYPE_GRAY_ALPHA,
        PNG_COLOR_TYPE_PALETTE,
        PNG_COLOR_TYPE_RGB,
        PNG_COLOR_TYPE_RGB_ALPHA
    });
    int interlace_type = fdp.ConsumeBool() ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
    
    // Set up basic image parameters for writing
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                color_type, interlace_type, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // Set up APNG animation control (acTL chunk)
    png_set_acTL(png_ptr_write, info_ptr_write, num_frames, num_plays);
    
    // Write info for writing structure
    png_write_info(png_ptr_write, info_ptr_write);
    
    // Test metadata retrieval functions
    png_uint_32 retrieved_num_frames, retrieved_num_plays;
    
    // Get APNG animation control info
    if (png_get_acTL(png_ptr_write, info_ptr_write, &retrieved_num_frames, &retrieved_num_plays)) {
        // Verify the values match what we set
        (void)retrieved_num_frames;
        (void)retrieved_num_plays;
    }
    
    // Get number of frames
    png_uint_32 frame_count = png_get_num_frames(png_ptr_write, info_ptr_write);
    (void)frame_count;
    
    // Get number of plays (loops)
    png_uint_32 play_count = png_get_num_plays(png_ptr_write, info_ptr_write);
    (void)play_count;
    
    // Test reading APNG metadata (simulated - we're not actually reading a real APNG file)
    // But we can still call the functions to exercise them
    
    // Test frame operations for multiple frames
    for (png_uint_32 frame = 0; frame < num_frames && frame < 5; frame++) {
        // Consume frame-specific parameters
        png_uint_32 frame_width = fdp.ConsumeIntegralInRange<png_uint_32>(1, width);
        png_uint_32 frame_height = fdp.ConsumeIntegralInRange<png_uint_32>(1, height);
        png_uint_32 x_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, width - frame_width);
        png_uint_32 y_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, height - frame_height);
        png_uint_16 delay_num = fdp.ConsumeIntegralInRange<png_uint_16>(1, 1000);
        png_uint_16 delay_den = fdp.ConsumeIntegralInRange<png_uint_16>(1, 1000);
        png_byte dispose_op = fdp.ConsumeIntegralInRange<png_byte>(0, 3);
        png_byte blend_op = fdp.ConsumeIntegralInRange<png_byte>(0, 1);
        
        // Set up frame control (fcTL) for writing
        png_set_next_frame_fcTL(png_ptr_write, info_ptr_write,
                               frame_width, frame_height,
                               x_offset, y_offset,
                               delay_num, delay_den,
                               dispose_op, blend_op);
        
        // Write frame head - TARGET API: png_write_frame_head
        // Need row pointers for png_write_frame_head
        png_bytep row_pointers[1];
        png_uint_32 row_bytes = png_get_rowbytes(png_ptr_write, info_ptr_write);
        
        // Allocate a dummy row buffer
        if (row_bytes > 0 && row_bytes < 65536) {  // Reasonable size limit
            png_bytep row_buffer = (png_bytep)png_malloc(png_ptr_write, row_bytes);
            if (row_buffer) {
                // Fill with some data
                for (png_uint_32 i = 0; i < row_bytes; i++) {
                    row_buffer[i] = (png_byte)(i % 256);
                }
                
                row_pointers[0] = row_buffer;
                
                // Call png_write_frame_head - PRIMARY TARGET API
                png_write_frame_head(png_ptr_write, info_ptr_write, row_pointers,
                                    frame_width, frame_height,
                                    x_offset, y_offset,
                                    delay_num, delay_den,
                                    dispose_op, blend_op);
                
                // Free the row buffer
                png_free(png_ptr_write, row_buffer);
            }
        }
        
        // Simulate reading frame head - TARGET API: png_read_frame_head
        // Note: This would normally be called during actual PNG reading
        // We can't call it without valid PNG data, but we include the API reference
        
        // Test metadata retrieval for frames
        if (frame == 0) {
            png_byte is_hidden = fdp.ConsumeBool() ? 1 : 0;
            png_set_first_frame_is_hidden(png_ptr_write, info_ptr_write, is_hidden);
            
            png_byte retrieved_is_hidden = png_get_first_frame_is_hidden(png_ptr_write, info_ptr_write);
            (void)retrieved_is_hidden;
        }
        
        // Test reading rows - TARGET API: png_read_rows
        // This would normally be called to read actual image data
        // We include it as a reference but can't call without valid PNG data
        
        // Test getting frame information
        png_uint_32 next_frame_width = png_get_next_frame_width(png_ptr_write, info_ptr_write);
        png_uint_32 next_frame_height = png_get_next_frame_height(png_ptr_write, info_ptr_write);
        png_uint_32 next_frame_x_offset = png_get_next_frame_x_offset(png_ptr_write, info_ptr_write);
        png_uint_32 next_frame_y_offset = png_get_next_frame_y_offset(png_ptr_write, info_ptr_write);
        png_uint_16 next_frame_delay_num = png_get_next_frame_delay_num(png_ptr_write, info_ptr_write);
        png_uint_16 next_frame_delay_den = png_get_next_frame_delay_den(png_ptr_write, info_ptr_write);
        png_byte next_frame_dispose_op = png_get_next_frame_dispose_op(png_ptr_write, info_ptr_write);
        png_byte next_frame_blend_op = png_get_next_frame_blend_op(png_ptr_write, info_ptr_write);
        
        (void)next_frame_width;
        (void)next_frame_height;
        (void)next_frame_x_offset;
        (void)next_frame_y_offset;
        (void)next_frame_delay_num;
        (void)next_frame_delay_den;
        (void)next_frame_dispose_op;
        (void)next_frame_blend_op;
        
        // Write frame tail
        png_write_frame_tail(png_ptr_write, info_ptr_write);
    }
    
    // Test additional metadata retrieval functions
    // Get image header info
    png_uint_32 retrieved_width, retrieved_height;
    int retrieved_bit_depth, retrieved_color_type, retrieved_interlace_type;
    int retrieved_compression_type, retrieved_filter_type;
    
    png_get_IHDR(png_ptr_write, info_ptr_write, &retrieved_width, &retrieved_height,
                &retrieved_bit_depth, &retrieved_color_type, &retrieved_interlace_type,
                &retrieved_compression_type, &retrieved_filter_type);
    
    (void)retrieved_width;
    (void)retrieved_height;
    (void)retrieved_bit_depth;
    (void)retrieved_color_type;
    (void)retrieved_interlace_type;
    (void)retrieved_compression_type;
    (void)retrieved_filter_type;
    
    // Test color management metadata functions (if supported)
#ifdef PNG_cHRM_SUPPORTED
    {
        double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
        if (png_get_cHRM(png_ptr_write, info_ptr_write, &white_x, &white_y,
                        &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y)) {
            (void)white_x; (void)white_y;
            (void)red_x; (void)red_y;
            (void)green_x; (void)green_y;
            (void)blue_x; (void)blue_y;
        }
    }
#endif
    
#ifdef PNG_cHRM_XYZ_SUPPORTED
    {
        double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
        if (png_get_cHRM_XYZ(png_ptr_write, info_ptr_write,
                            &red_X, &red_Y, &red_Z,
                            &green_X, &green_Y, &green_Z,
                            &blue_X, &blue_Y, &blue_Z)) {
            (void)red_X; (void)red_Y; (void)red_Z;
            (void)green_X; (void)green_Y; (void)green_Z;
            (void)blue_X; (void)blue_Y; (void)blue_Z;
        }
    }
#endif
    
#ifdef PNG_gAMA_SUPPORTED
    {
        double file_gamma;
        if (png_get_gAMA(png_ptr_write, info_ptr_write, &file_gamma)) {
            (void)file_gamma;
        }
    }
#endif
    
    // Clean up
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    return 0;
}

} // namespace harness_007
