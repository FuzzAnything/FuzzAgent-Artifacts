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

namespace harness_010 {

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
    // Need minimum data for APNG frame control operations
    // We need enough data for:
    // - PNG signature check: 8 bytes
    // - APNG setup: num_frames, num_plays: 8 bytes
    // - Frame control: width, height, x_offset, y_offset, delay_num, delay_den, dispose_op, blend_op: ~20 bytes
    // - Basic image parameters for IHDR: ~30 bytes
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create PNG read structure - TARGET HELPER: png_create_read_struct
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr) {
        return 0;
    }
    
    // Step 2: Create info structure - TARGET HELPER: png_create_info_struct
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
    
    // Set up custom read functions
    png_memory_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    png_set_read_fn(png_ptr, &buffer, pngtest_read_data);
    
    // Skip PNG signature (first 8 bytes) if we have enough data
    if (size >= 8) {
        png_set_sig_bytes(png_ptr, 8);
        buffer.offset = 8;
    }
    
    // Step 3: Set basic image parameters - TARGET HELPER: png_set_IHDR
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({PNG_COLOR_TYPE_GRAY, PNG_COLOR_TYPE_RGB, 
                                           PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_GRAY_ALPHA,
                                           PNG_COLOR_TYPE_RGB_ALPHA});
    int interlace_method = fdp.PickValueInArray({PNG_INTERLACE_NONE, PNG_INTERLACE_ADAM7});
    int compression_method = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_method = PNG_FILTER_TYPE_DEFAULT;
    
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
#ifdef PNG_APNG_SUPPORTED
    // Step 4: Mark PNG as animated - TARGET HELPER: png_set_acTL
    png_uint_32 num_frames = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_uint_32 num_plays = fdp.ConsumeIntegralInRange<png_uint_32>(0, 100);
    
    png_set_acTL(png_ptr, info_ptr, num_frames, num_plays);
    
    // Step 5: Set frame control data - TARGET HELPER: png_set_next_frame_fcTL
    png_uint_32 frame_width = fdp.ConsumeIntegralInRange<png_uint_32>(1, width);
    png_uint_32 frame_height = fdp.ConsumeIntegralInRange<png_uint_32>(1, height);
    png_uint_32 x_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, width - frame_width);
    png_uint_32 y_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, height - frame_height);
    png_uint_16 delay_num = fdp.ConsumeIntegral<png_uint_16>();
    png_uint_16 delay_den = fdp.ConsumeIntegralInRange<png_uint_16>(1, 1000);  // Avoid zero denominator
    png_byte dispose_op = fdp.PickValueInArray({PNG_fcTL_DISPOSE_OP_NONE, PNG_fcTL_DISPOSE_OP_BACKGROUND, 
                                                PNG_fcTL_DISPOSE_OP_PREVIOUS});
    png_byte blend_op = fdp.PickValueInArray({PNG_fcTL_BLEND_OP_SOURCE, PNG_fcTL_BLEND_OP_OVER});
    
    png_set_next_frame_fcTL(png_ptr, info_ptr, frame_width, frame_height,
                           x_offset, y_offset, delay_num, delay_den,
                           dispose_op, blend_op);
    
    // Step 6: Try to read PNG info - TARGET HELPER: png_read_info
    // This may fail since we don't have valid PNG data, but we call it anyway
    // The setjmp will catch any errors
    png_read_info(png_ptr, info_ptr);
    
    // Step 7: PRIMARY TARGET: png_get_next_frame_fcTL - comprehensive getter with 22 undiscovered branches
    png_uint_32 retrieved_width, retrieved_height, retrieved_x_offset, retrieved_y_offset;
    png_uint_16 retrieved_delay_num, retrieved_delay_den;
    png_byte retrieved_dispose_op, retrieved_blend_op;
    
    png_uint_32 fcTL_result = png_get_next_frame_fcTL(png_ptr, info_ptr,
                                                     &retrieved_width, &retrieved_height,
                                                     &retrieved_x_offset, &retrieved_y_offset,
                                                     &retrieved_delay_num, &retrieved_delay_den,
                                                     &retrieved_dispose_op, &retrieved_blend_op);
    
    (void)fcTL_result;
    (void)retrieved_width;
    (void)retrieved_height;
    (void)retrieved_x_offset;
    (void)retrieved_y_offset;
    (void)retrieved_delay_num;
    (void)retrieved_delay_den;
    (void)retrieved_dispose_op;
    (void)retrieved_blend_op;
    
    // Step 8: Call individual getter functions for additional coverage
    // These have 2 undiscovered branches each according to the guidance
    
    // TARGET: png_get_next_frame_blend_op (50% coverage, 2 undiscovered branches)
    png_byte blend_op_result = png_get_next_frame_blend_op(png_ptr, info_ptr);
    (void)blend_op_result;
    
    // TARGET: png_get_next_frame_width (50% coverage, 2 undiscovered branches)
    png_uint_32 width_result = png_get_next_frame_width(png_ptr, info_ptr);
    (void)width_result;
    
    // TARGET: png_get_next_frame_x_offset (50% coverage, 2 undiscovered branches)
    png_uint_32 x_offset_result = png_get_next_frame_x_offset(png_ptr, info_ptr);
    (void)x_offset_result;
    
    // TARGET: png_get_next_frame_y_offset (50% coverage, 2 undiscovered branches)
    png_uint_32 y_offset_result = png_get_next_frame_y_offset(png_ptr, info_ptr);
    (void)y_offset_result;
    
    // TARGET: png_get_next_frame_dispose_op (50% coverage, 2 undiscovered branches)
    png_byte dispose_op_result = png_get_next_frame_dispose_op(png_ptr, info_ptr);
    (void)dispose_op_result;
    
    // TARGET: png_get_next_frame_delay_den (50% coverage, 2 undiscovered branches)
    png_uint_16 delay_den_result = png_get_next_frame_delay_den(png_ptr, info_ptr);
    (void)delay_den_result;
    
    // TARGET: png_get_next_frame_delay_num (50% coverage, 2 undiscovered branches)
    png_uint_16 delay_num_result = png_get_next_frame_delay_num(png_ptr, info_ptr);
    (void)delay_num_result;
    
    // Also test png_get_next_frame_height which wasn't in guidance but is part of the family
    png_uint_32 height_result = png_get_next_frame_height(png_ptr, info_ptr);
    (void)height_result;
    
    // Test other APNG-related getters
    png_uint_32 num_frames_result = png_get_num_frames(png_ptr, info_ptr);
    png_uint_32 num_plays_result = png_get_num_plays(png_ptr, info_ptr);
    (void)num_frames_result;
    (void)num_plays_result;
    
    // Test first frame hidden flag
    png_byte first_frame_hidden = png_get_first_frame_is_hidden(png_ptr, info_ptr);
    (void)first_frame_hidden;
    
    // Step 9: Try to read image data - TARGET HELPER: png_read_image
    // This will likely fail but we include it for completeness
    // We need to allocate row pointers first
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    if (rowbytes > 0 && height > 0) {
        png_bytep row_pointers[height];
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, rowbytes);
            // Fill with some data from fuzzer input
            size_t bytes_to_fill = rowbytes < fdp.remaining_bytes() ? rowbytes : fdp.remaining_bytes();
            if (bytes_to_fill > 0) {
                auto bytes = fdp.ConsumeBytes<uint8_t>(bytes_to_fill);
                memcpy(row_pointers[y], bytes.data(), bytes_to_fill);
            }
        }
        
        png_read_image(png_ptr, row_pointers);
        
        // Clean up row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
    }
    
#endif // PNG_APNG_SUPPORTED
    
    // Step 10: Clean up - TARGET HELPER: png_destroy_read_struct
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_010
