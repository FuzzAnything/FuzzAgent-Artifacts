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

namespace harness_018 {
// Error callback function for libpng
static void pngtest_error(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

// Warning callback function for libpng
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

// User transform callback for testing png_set_user_transform_info
static void user_transform_callback(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    (void)png_ptr;
    (void)row_info;
    (void)data;
    // Simple transform: do nothing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for basic PNG operations and transformation parameters
    // We need:
    // - PNG signature check: 8 bytes
    // - Transformation parameters: ~50 bytes
    // - Some PNG data for reading: ~100 bytes
    if (size < 160) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Check PNG signature
    png_byte png_sig[8];
    auto sig_bytes = fdp.ConsumeBytes<uint8_t>(8);
    memcpy(png_sig, sig_bytes.data(), 8);
    int is_png = png_sig_cmp(png_sig, 0, 8);
    (void)is_png; // Result can be ignored for fuzzing
    
    // Step 2: Create PNG read structure - TARGET HELPER: png_create_read_struct
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr) {
        return 0;
    }
    
    // Step 3: Create info structure - TARGET HELPER: png_create_info_struct
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
    
    // Step 4: Set up custom read functions - TARGET HELPER: png_set_read_fn
    // Consume remaining bytes for PNG data after parameter consumption
    std::vector<uint8_t> png_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    png_memory_buffer buffer;
    buffer.data = png_data.data();
    buffer.size = png_data.size();
    buffer.offset = 0;
    png_set_read_fn(png_ptr, &buffer, pngtest_read_data);
    
    // Step 5: Set transformation functions - PRIMARY TARGETS
    
    // PRIMARY TARGET: png_set_shift - requires png_color_8 structure
    png_color_8 true_bits;
    true_bits.red = fdp.ConsumeIntegralInRange<png_byte>(1, 16);   // Must be 1-16 for valid shift
    true_bits.green = fdp.ConsumeIntegralInRange<png_byte>(1, 16);
    true_bits.blue = fdp.ConsumeIntegralInRange<png_byte>(1, 16);
    true_bits.gray = fdp.ConsumeIntegralInRange<png_byte>(1, 16);
    true_bits.alpha = fdp.ConsumeIntegralInRange<png_byte>(1, 16);
    png_set_shift(png_ptr, &true_bits);
    
    // PRIMARY TARGET: png_set_gamma - screen gamma and file gamma
    // Use fixed-point values for gamma (PNG_FP_1 = 1.0)
    double screen_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
    double file_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
    png_set_gamma(png_ptr, screen_gamma, file_gamma);
    
    // PRIMARY TARGET: png_set_alpha_mode - mode and output gamma
    int alpha_mode = fdp.PickValueInArray({
        PNG_ALPHA_PNG,           // 0
        PNG_ALPHA_STANDARD,      // 1  
        PNG_ALPHA_OPTIMIZED,     // 2
        PNG_ALPHA_BROKEN         // 3
    });
    
    double output_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
    png_set_alpha_mode(png_ptr, alpha_mode, output_gamma);
    
    // PRIMARY TARGET: png_set_user_transform_info - user data, depth, and channels
    int user_transform_data = fdp.ConsumeIntegral<int>();
    int user_transform_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int user_transform_channels = fdp.ConsumeIntegralInRange<int>(1, 4);
    png_set_user_transform_info(png_ptr, (void*)&user_transform_data, user_transform_depth, user_transform_channels);
    png_set_read_user_transform_fn(png_ptr, user_transform_callback);
    
    // Step 6: Read PNG header - TARGET HELPER: png_read_info
    // This will apply the transformations we just set
    png_read_info(png_ptr, info_ptr);
    
    // Step 7: Prepare for reading - TARGET HELPER: png_start_read_image
    png_start_read_image(png_ptr);
    
    // Step 8: Read some image data - TARGET HELPER: png_read_row
    // Get image dimensions to allocate row buffer
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    
    // Calculate row bytes
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    
    if (row_bytes > 0 && row_bytes < 10000) { // Reasonable limit
        // Allocate row buffer
        std::vector<png_byte> row_buffer(row_bytes, 0);
        
        // Try to read at least one row if we have data
        if (buffer.offset < buffer.size && height > 0) {
            // Fill buffer with some data from fuzzer if available
            size_t bytes_to_read = row_bytes;
            if (buffer.offset + bytes_to_read > buffer.size) {
                bytes_to_read = buffer.size - buffer.offset;
            }
            
            if (bytes_to_read > 0) {
                memcpy(row_buffer.data(), buffer.data + buffer.offset, bytes_to_read);
                buffer.offset += bytes_to_read;
            }
            
            // Read one row with transformations applied
            png_read_row(png_ptr, row_buffer.data(), nullptr);
        }
    }
    
    // Step 9: Call helper functions - TARGET HELPERS
    // Helper: png_get_current_pass_number
    int current_pass = png_get_current_pass_number(png_ptr);
    (void)current_pass;
    
    // Helper: png_get_current_row_number  
    png_uint_32 current_row = png_get_current_row_number(png_ptr);
    (void)current_row;
    
    // Step 10: Cleanup - TARGET HELPER: png_destroy_read_struct
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_018
