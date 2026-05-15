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

namespace harness_027 {

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

// Custom read function for memory buffers
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
    // Need minimum data for APNG frame reading operations
    // We need enough data for:
    // - PNG signature (8 bytes if we skip)
    // - Basic PNG header setup
    // - APNG animation control parameters
    // - Frame control parameters
    if (size < 256) {
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
    
    // Step 3: Set up custom read functions - TARGET HELPER: png_set_read_fn
    png_memory_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    png_set_read_fn(png_ptr, &buffer, pngtest_read_data);
    
    // Set error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Skip PNG signature (first 8 bytes) if we have enough data
    if (size >= 8) {
        png_set_sig_bytes(png_ptr, 8);
        buffer.offset = 8;
    }
    
    // Step 4: Read PNG header - TARGET HELPER: png_read_info
    // This will attempt to read PNG info from the data
    // It may fail if data isn't valid PNG, but that's OK for fuzzing
    png_read_info(png_ptr, info_ptr);
    
#ifdef PNG_APNG_SUPPORTED
    // Step 5: Set up APNG animation control - TARGET HELPER: png_set_acTL
    // Consume APNG parameters from fuzzer input
    png_uint_32 num_frames = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_uint_32 num_plays = fdp.ConsumeIntegralInRange<png_uint_32>(0, 100);
    
    png_set_acTL(png_ptr, info_ptr, num_frames, num_plays);
    
    // Step 6: Set up APNG frame control - TARGET HELPER: png_set_next_frame_fcTL
    // Consume frame control parameters from fuzzer input
    png_uint_32 frame_width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 frame_height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 x_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, 1000);
    png_uint_32 y_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, 1000);
    png_uint_16 delay_num = fdp.ConsumeIntegralInRange<png_uint_16>(1, 1000);
    png_uint_16 delay_den = fdp.ConsumeIntegralInRange<png_uint_16>(1, 1000);
    png_byte dispose_op = fdp.ConsumeIntegral<uint8_t>() % 3; // PNG_DISPOSE_OP values: 0-2
    png_byte blend_op = fdp.ConsumeIntegral<uint8_t>() % 2;   // PNG_BLEND_OP values: 0-1
    
    png_set_next_frame_fcTL(png_ptr, info_ptr,
                           frame_width, frame_height,
                           x_offset, y_offset,
                           delay_num, delay_den,
                           dispose_op, blend_op);
    
    // Step 7: PRIMARY TARGET 1: png_read_frame_head (34 undiscovered branches)
    // This function reads the frame header for APNG frames
    // Note: This requires proper APNG setup and will likely fail without valid PNG data,
    // but we call it to exercise the API paths
    // The function checks for PNG_HAVE_acTL internally, which should be set by png_set_acTL
    png_read_frame_head(png_ptr, info_ptr);
    
    // Step 8: PRIMARY TARGET 2: png_get_next_frame_fcTL (22 undiscovered branches)
    // Retrieve frame control data that was set earlier
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
    
    // Also test individual frame property getters for additional coverage
    png_uint_32 next_frame_width = png_get_next_frame_width(png_ptr, info_ptr);
    png_uint_32 next_frame_height = png_get_next_frame_height(png_ptr, info_ptr);
    png_uint_32 next_frame_x_offset = png_get_next_frame_x_offset(png_ptr, info_ptr);
    png_uint_32 next_frame_y_offset = png_get_next_frame_y_offset(png_ptr, info_ptr);
    png_uint_16 next_frame_delay_num = png_get_next_frame_delay_num(png_ptr, info_ptr);
    png_uint_16 next_frame_delay_den = png_get_next_frame_delay_den(png_ptr, info_ptr);
    png_byte next_frame_dispose_op = png_get_next_frame_dispose_op(png_ptr, info_ptr);
    png_byte next_frame_blend_op = png_get_next_frame_blend_op(png_ptr, info_ptr);
    
    (void)next_frame_width;
    (void)next_frame_height;
    (void)next_frame_x_offset;
    (void)next_frame_y_offset;
    (void)next_frame_delay_num;
    (void)next_frame_delay_den;
    (void)next_frame_dispose_op;
    (void)next_frame_blend_op;
    
    // Test other APNG-related getters
    png_uint_32 num_frames_result = png_get_num_frames(png_ptr, info_ptr);
    png_uint_32 num_plays_result = png_get_num_plays(png_ptr, info_ptr);
    (void)num_frames_result;
    (void)num_plays_result;
    
    // Test first frame hidden flag
    png_byte is_hidden = fdp.ConsumeBool() ? 1 : 0;
    png_set_first_frame_is_hidden(png_ptr, info_ptr, is_hidden);
    png_byte first_frame_hidden = png_get_first_frame_is_hidden(png_ptr, info_ptr);
    (void)first_frame_hidden;
    
#endif // PNG_APNG_SUPPORTED
    
    // Step 9: Clean up - TARGET HELPER: png_destroy_read_struct
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_027
