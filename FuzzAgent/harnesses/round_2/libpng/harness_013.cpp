#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace harness_013 {

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
    // Need sufficient data for chromaticity testing
    // Required: image parameters + multiple chromaticity values (white point, red, green, blue)
    // Each chromaticity value needs at least 8 bytes (double) or 4 bytes (fixed point)
    // We need values for both xy and XYZ color spaces, plus get functions
    if (size < 500) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(8, 16);
    int color_type = PNG_COLOR_TYPE_RGB; // Use RGB for chromaticity testing
    int interlace_type = fdp.ConsumeBool() ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
    int compression_type = PNG_COMPRESSION_TYPE_BASE;
    int filter_type = PNG_FILTER_TYPE_BASE;
    
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
    // TARGET APIs: Chromaticity (cHRM) Metadata Handling
    // Targeting ~110+ undiscovered branch complexity across 8 related APIs
    ////////////////////////////////////////////////////////////////////////////
    
#ifdef PNG_cHRM_SUPPORTED
    // Generate random but valid chromaticity values
    // Chromaticity coordinates should be in range [0.0, 1.0] for valid color space
    // White point (x, y), Red (x, y), Green (x, y), Blue (x, y)
    
    // Consume floating point chromaticity values for xy color space
    double white_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double white_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double red_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double red_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double green_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double green_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double blue_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double blue_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 1: png_set_cHRM (0% covered, 14 branches)
    // Set chromaticity using floating-point values (xy color space)
    ////////////////////////////////////////////////////////////////////////////
    png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 2: png_get_cHRM (5 hits but 19 undiscovered branches)
    // Get chromaticity values to verify they were stored correctly
    ////////////////////////////////////////////////////////////////////////////
    double check_white_x, check_white_y, check_red_x, check_red_y;
    double check_green_x, check_green_y, check_blue_x, check_blue_y;
    png_uint_32 get_result = png_get_cHRM(png_ptr, info_ptr,
                                          &check_white_x, &check_white_y,
                                          &check_red_x, &check_red_y,
                                          &check_green_x, &check_green_y,
                                          &check_blue_x, &check_blue_y);
    // Use the result to avoid unused variable warning
    (void)get_result;
    
    // Consume fixed-point chromaticity values
    png_fixed_point int_white_x = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_white_y = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_red_x = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_red_y = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_green_x = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_green_y = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_blue_x = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_blue_y = fdp.ConsumeIntegral<png_fixed_point>();
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 3: png_set_cHRM_fixed (partially covered, 4 branches)
    // Set chromaticity using fixed-point values (xy color space)
    ////////////////////////////////////////////////////////////////////////////
    png_set_cHRM_fixed(png_ptr, info_ptr,
                       int_white_x, int_white_y,
                       int_red_x, int_red_y,
                       int_green_x, int_green_y,
                       int_blue_x, int_blue_y);
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 4: png_get_cHRM_fixed (5 hits but 19 undiscovered branches)
    // Get chromaticity fixed-point values
    ////////////////////////////////////////////////////////////////////////////
    png_fixed_point check_int_white_x, check_int_white_y, check_int_red_x, check_int_red_y;
    png_fixed_point check_int_green_x, check_int_green_y, check_int_blue_x, check_int_blue_y;
    png_uint_32 get_fixed_result = png_get_cHRM_fixed(png_ptr, info_ptr,
                                                      &check_int_white_x, &check_int_white_y,
                                                      &check_int_red_x, &check_int_red_y,
                                                      &check_int_green_x, &check_int_green_y,
                                                      &check_int_blue_x, &check_int_blue_y);
    (void)get_fixed_result;
    
    // Consume XYZ chromaticity values (floating point)
    double red_X = fdp.ConsumeFloatingPointInRange<double>(0.0, 2.0);
    double red_Y = fdp.ConsumeFloatingPointInRange<double>(0.0, 2.0);
    double red_Z = fdp.ConsumeFloatingPointInRange<double>(0.0, 2.0);
    double green_X = fdp.ConsumeFloatingPointInRange<double>(0.0, 2.0);
    double green_Y = fdp.ConsumeFloatingPointInRange<double>(0.0, 2.0);
    double green_Z = fdp.ConsumeFloatingPointInRange<double>(0.0, 2.0);
    double blue_X = fdp.ConsumeFloatingPointInRange<double>(0.0, 2.0);
    double blue_Y = fdp.ConsumeFloatingPointInRange<double>(0.0, 2.0);
    double blue_Z = fdp.ConsumeFloatingPointInRange<double>(0.0, 2.0);
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 5: png_set_cHRM_XYZ (5 hits but 10 undiscovered branches)
    // Set chromaticity in XYZ color space using floating-point
    ////////////////////////////////////////////////////////////////////////////
    png_set_cHRM_XYZ(png_ptr, info_ptr,
                     red_X, red_Y, red_Z,
                     green_X, green_Y, green_Z,
                     blue_X, blue_Y, blue_Z);
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 6: png_get_cHRM_XYZ (5 hits but 23 undiscovered branches)
    // Get chromaticity XYZ values
    ////////////////////////////////////////////////////////////////////////////
    double check_red_X, check_red_Y, check_red_Z;
    double check_green_X, check_green_Y, check_green_Z;
    double check_blue_X, check_blue_Y, check_blue_Z;
    png_uint_32 get_xyz_result = png_get_cHRM_XYZ(png_ptr, info_ptr,
                                                  &check_red_X, &check_red_Y, &check_red_Z,
                                                  &check_green_X, &check_green_Y, &check_green_Z,
                                                  &check_blue_X, &check_blue_Y, &check_blue_Z);
    (void)get_xyz_result;
    
    // Consume fixed-point XYZ chromaticity values
    png_fixed_point int_red_X = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_red_Y = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_red_Z = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_green_X = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_green_Y = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_green_Z = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_blue_X = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_blue_Y = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_blue_Z = fdp.ConsumeIntegral<png_fixed_point>();
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 7: png_set_cHRM_XYZ_fixed (1 hit but 10 undiscovered branches)
    // Set chromaticity in XYZ color space using fixed-point
    ////////////////////////////////////////////////////////////////////////////
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
                           int_red_X, int_red_Y, int_red_Z,
                           int_green_X, int_green_Y, int_green_Z,
                           int_blue_X, int_blue_Y, int_blue_Z);
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 8: png_get_cHRM_XYZ_fixed (0% covered, 26 branches)
    // Get chromaticity XYZ fixed-point values
    ////////////////////////////////////////////////////////////////////////////
    png_fixed_point check_int_red_X, check_int_red_Y, check_int_red_Z;
    png_fixed_point check_int_green_X, check_int_green_Y, check_int_green_Z;
    png_fixed_point check_int_blue_X, check_int_blue_Y, check_int_blue_Z;
    png_uint_32 get_xyz_fixed_result = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                                              &check_int_red_X, &check_int_red_Y, &check_int_red_Z,
                                                              &check_int_green_X, &check_int_green_Y, &check_int_green_Z,
                                                              &check_int_blue_X, &check_int_blue_Y, &check_int_blue_Z);
    (void)get_xyz_fixed_result;
    
#endif // PNG_cHRM_SUPPORTED
    
    // Write PNG info to validate chromaticity settings
    png_write_info(png_ptr, info_ptr);
    
    // Generate simple image data (single row for testing)
    // Calculate correct row buffer size based on bit depth and color type
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    std::vector<png_byte> row(rowbytes);
    for (size_t i = 0; i < row.size(); i++) {
        row[i] = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // Write image data
    for (png_uint_32 y = 0; y < height; y++) {
        png_write_row(png_ptr, row.data());
    }
    
    // Finalize PNG
    png_write_end(png_ptr, info_ptr);
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_013
