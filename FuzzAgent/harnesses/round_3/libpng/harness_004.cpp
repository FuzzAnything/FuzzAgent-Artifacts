#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_004 {

// Custom read function structure to work with libpng's C API
struct png_read_data {
    FuzzedDataProvider* fdp;
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void png_read_callback(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead) {
    png_read_data* read_data = (png_read_data*)png_get_io_ptr(png_ptr);
    
    if (read_data->offset + byteCountToRead > read_data->size) {
        png_error(png_ptr, "Insufficient data");
    }
    
    memcpy(outBytes, read_data->data + read_data->offset, byteCountToRead);
    read_data->offset += byteCountToRead;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        return 0;  // Minimum size for meaningful testing with transformations
    }

    FuzzedDataProvider fdp(data, size);

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Set up custom read data structure
    png_read_data read_data;
    read_data.data = data;
    read_data.size = size;
    read_data.offset = 0;
    
    // Set up the read function
    png_set_read_fn(png_ptr, &read_data, png_read_callback);

    // Set the number of bytes already read (0 since we start from beginning)
    png_set_sig_bytes(png_ptr, 0);

    // Read PNG info to get basic image information
    png_read_info(png_ptr, info_ptr);

    // Get image information using getter functions (target API: png_get_IHDR)
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0, interlace_type = 0, compression_type = 0, filter_type = 0;
    
    if (png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                     &interlace_type, &compression_type, &filter_type) == 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Get valid chunks using png_get_valid (target API)
    png_uint_32 valid = png_get_valid(png_ptr, info_ptr, 0xFFFFFFFF);
    // Consume fuzzer data to decide which transformations to apply
    int transform_flags = fdp.ConsumeIntegral<int>();
    bool apply_quantize = fdp.ConsumeBool();
    bool apply_rgb_to_gray = fdp.ConsumeBool();
    bool apply_gamma = fdp.ConsumeBool();
    bool apply_alpha_mode = fdp.ConsumeBool();
    bool apply_background = fdp.ConsumeBool();
    bool apply_palette_to_rgb = fdp.ConsumeBool();
    bool apply_tRNS_to_alpha = fdp.ConsumeBool();
    bool test_apng = fdp.ConsumeBool();

    // Apply various transformation APIs based on consumed flags

    // 1. png_set_quantize - highest priority with 128 undiscovered branches
    if (apply_quantize && fdp.remaining_bytes() >= 256 * 3 + sizeof(int) * 3 + 256 * 2) {
        // Create a palette with up to 256 colors
        int num_palette = fdp.ConsumeIntegralInRange<int>(1, 256);
        int maximum_colors = fdp.ConsumeIntegralInRange<int>(1, num_palette);
        int full_quantize = fdp.ConsumeBool() ? 1 : 0;
        
        png_color* palette = (png_color*)png_malloc(png_ptr, num_palette * sizeof(png_color));
        png_uint_16* histogram = (png_uint_16*)png_malloc(png_ptr, num_palette * sizeof(png_uint_16));
        
        if (palette && histogram) {
            for (int i = 0; i < num_palette; i++) {
                palette[i].red = fdp.ConsumeIntegral<uint8_t>();
                palette[i].green = fdp.ConsumeIntegral<uint8_t>();
                palette[i].blue = fdp.ConsumeIntegral<uint8_t>();
                histogram[i] = fdp.ConsumeIntegral<uint16_t>();
            }
            
            png_set_quantize(png_ptr, palette, num_palette, maximum_colors, histogram, full_quantize);
            
            png_free(png_ptr, palette);
            png_free(png_ptr, histogram);
        }
    }

    // 2. png_set_rgb_to_gray - RGB to gray conversion
    if (apply_rgb_to_gray && fdp.remaining_bytes() >= sizeof(double) * 2 + sizeof(int)) {
        int error_action = fdp.ConsumeIntegralInRange<int>(0, 2); // PNG_ERROR_ACTION values
        double red_weight = fdp.ConsumeFloatingPoint<double>();
        double green_weight = fdp.ConsumeFloatingPoint<double>();
        
        // Clamp weights to reasonable values
        if (red_weight < 0.0) red_weight = 0.0;
        if (red_weight > 1.0) red_weight = 1.0;
        if (green_weight < 0.0) green_weight = 0.0;
        if (green_weight > 1.0) green_weight = 1.0;
        
        png_set_rgb_to_gray(png_ptr, error_action, red_weight, green_weight);
    }

    // 3. png_set_gamma - gamma correction
    if (apply_gamma && fdp.remaining_bytes() >= sizeof(double) * 2) {
        double screen_gamma = fdp.ConsumeFloatingPoint<double>();
        double file_gamma = fdp.ConsumeFloatingPoint<double>();
        
        // Clamp gamma values to reasonable ranges
        if (screen_gamma < 0.1) screen_gamma = 0.1;
        if (screen_gamma > 5.0) screen_gamma = 5.0;
        if (file_gamma < 0.1) file_gamma = 0.1;
        if (file_gamma > 5.0) file_gamma = 5.0;
        
        png_set_gamma(png_ptr, screen_gamma, file_gamma);
    }

    // 4. png_set_alpha_mode - alpha channel handling
    if (apply_alpha_mode && fdp.remaining_bytes() >= sizeof(int) + sizeof(double)) {
        int alpha_mode = fdp.ConsumeIntegralInRange<int>(0, 3); // PNG_ALPHA_PNG to PNG_ALPHA_BROKEN
        double output_gamma = fdp.ConsumeFloatingPoint<double>();
        
        if (output_gamma < 0.1) output_gamma = 0.1;
        if (output_gamma > 5.0) output_gamma = 5.0;
        
        png_set_alpha_mode(png_ptr, alpha_mode, output_gamma);
    }

    // 5. png_set_background - background color handling
    if (apply_background && fdp.remaining_bytes() >= sizeof(int) + sizeof(int) + sizeof(double) * 3) {
        int background_gamma_code = fdp.ConsumeIntegralInRange<int>(0, 3);
        int need_expand = fdp.ConsumeBool() ? 1 : 0;
        double background_gamma = fdp.ConsumeFloatingPoint<double>();
        
        // Create background color
        png_color_16 background_color;
        background_color.index = fdp.ConsumeIntegral<uint8_t>();
        background_color.red = fdp.ConsumeIntegral<uint16_t>();
        background_color.green = fdp.ConsumeIntegral<uint16_t>();
        background_color.blue = fdp.ConsumeIntegral<uint16_t>();
        background_color.gray = fdp.ConsumeIntegral<uint16_t>();
        
        png_set_background(png_ptr, &background_color, background_gamma_code, need_expand, background_gamma);
    }

    // 6. Other transformation APIs
    if (apply_palette_to_rgb) {
        png_set_palette_to_rgb(png_ptr);
    }
    
    if (apply_tRNS_to_alpha) {
        png_set_tRNS_to_alpha(png_ptr);
    }

    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);

    // Get updated image dimensions after transformations
    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    png_byte channels = png_get_channels(png_ptr, info_ptr);

    if (width == 0 || height == 0 || channels == 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Calculate row size
    size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
    if (row_size == 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Allocate row buffers for reading
    png_bytep row = (png_bytep)png_malloc(png_ptr, row_size);
    if (row == nullptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Read rows using png_read_rows (target API with 107 undiscovered branches)
    for (png_uint_32 y = 0; y < height; y++) {
        if (fdp.remaining_bytes() < 10) {
            // Not enough fuzzer data to continue
            break;
        }
        
        // Read single row
        png_read_row(png_ptr, row, nullptr);
        
        // Also test png_read_rows with batch reading
        if (y % 10 == 0 && y + 1 < height) {
            // Read multiple rows at once
            png_bytep rows_array[2];
            rows_array[0] = row;
            rows_array[1] = row; // For simplicity, using same buffer
            
            png_read_rows(png_ptr, rows_array, nullptr, 2);
            y++; // Skip next row since we read it already
        }
    }

    // Test APNG (Animated PNG) functionality if enabled and there's data
    if (test_apng && fdp.remaining_bytes() > 100) {
        // Try to read frame head (target API: png_read_frame_head)
        // Note: This only works if the PNG is actually an APNG
        png_read_frame_head(png_ptr, info_ptr);
    }

    // Clean up allocated memory
    png_free(png_ptr, row);
    
    // Final cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}

} // namespace harness_004
