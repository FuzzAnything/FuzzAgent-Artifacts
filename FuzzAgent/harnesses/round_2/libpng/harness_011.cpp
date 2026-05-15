#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace harness_011 {

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
    // Need minimum data for chromaticity/colorimetry metadata testing
    // Required: image parameters, chromaticity values, colorimetry values
    if (size < 200) return 0;
    
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
    // TARGET API 1: png_set_mDCV (20 undiscovered branches, 0% coverage)
    // Set modified DCV colorimetry (floating-point version)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_mDCV_SUPPORTED
    if (fdp.remaining_bytes() >= 10 * sizeof(double)) {
        // Consume chromaticity values (white point and primary colors)
        double white_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
        double white_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
        double red_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
        double red_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
        double green_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
        double green_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
        double blue_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
        double blue_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
        
        // Consume luminance values in cd/m² (nits)
        double max_luminance = fdp.ConsumeFloatingPointInRange<double>(0.0, 10000.0);
        double min_luminance = fdp.ConsumeFloatingPointInRange<double>(0.0, max_luminance);
        
        // Set mDCV colorimetry (floating-point version)
        png_set_mDCV(png_ptr, info_ptr,
                     white_x, white_y,
                     red_x, red_y,
                     green_x, green_y,
                     blue_x, blue_y,
                     max_luminance, min_luminance);
    }
#endif // PNG_mDCV_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 2: png_get_mDCV (26 undiscovered branches, 0% coverage)
    // Get modified DCV colorimetry (floating-point version)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_mDCV_SUPPORTED
    if (fdp.remaining_bytes() >= 4) {
        double white_x_ret = 0.0, white_y_ret = 0.0;
        double red_x_ret = 0.0, red_y_ret = 0.0;
        double green_x_ret = 0.0, green_y_ret = 0.0;
        double blue_x_ret = 0.0, blue_y_ret = 0.0;
        double max_luminance_ret = 0.0, min_luminance_ret = 0.0;
        
        // Get mDCV colorimetry (floating-point version)
        png_get_mDCV(png_ptr, info_ptr,
                     &white_x_ret, &white_y_ret,
                     &red_x_ret, &red_y_ret,
                     &green_x_ret, &green_y_ret,
                     &blue_x_ret, &blue_y_ret,
                     &max_luminance_ret, &min_luminance_ret);
    }
#endif // PNG_mDCV_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 3: png_set_mDCV_fixed (20 undiscovered branches, 0% coverage)
    // Set modified DCV colorimetry (fixed-point version)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_mDCV_SUPPORTED
    if (fdp.remaining_bytes() >= 10 * sizeof(int32_t)) {
        // Consume fixed-point chromaticity values (scaled by PNG_FP_1 = 100000)
        // Values should be in range 0 to approximately 1.3107 * PNG_FP_1
        png_fixed_point white_x_fixed = fdp.ConsumeIntegralInRange<int32_t>(0, 131070);
        png_fixed_point white_y_fixed = fdp.ConsumeIntegralInRange<int32_t>(0, 131070);
        png_fixed_point red_x_fixed = fdp.ConsumeIntegralInRange<int32_t>(0, 131070);
        png_fixed_point red_y_fixed = fdp.ConsumeIntegralInRange<int32_t>(0, 131070);
        png_fixed_point green_x_fixed = fdp.ConsumeIntegralInRange<int32_t>(0, 131070);
        png_fixed_point green_y_fixed = fdp.ConsumeIntegralInRange<int32_t>(0, 131070);
        png_fixed_point blue_x_fixed = fdp.ConsumeIntegralInRange<int32_t>(0, 131070);
        png_fixed_point blue_y_fixed = fdp.ConsumeIntegralInRange<int32_t>(0, 131070);
        
        // Consume scaled luminance values (multiplied by 10,000)
        png_uint_32 max_luminance_scaled = fdp.ConsumeIntegralInRange<png_uint_32>(0, PNG_UINT_31_MAX);
        png_uint_32 min_luminance_scaled = fdp.ConsumeIntegralInRange<png_uint_32>(0, max_luminance_scaled);
        
        // Set mDCV colorimetry (fixed-point version)
        png_set_mDCV_fixed(png_ptr, info_ptr,
                           white_x_fixed, white_y_fixed,
                           red_x_fixed, red_y_fixed,
                           green_x_fixed, green_y_fixed,
                           blue_x_fixed, blue_y_fixed,
                           max_luminance_scaled, min_luminance_scaled);
    }
#endif // PNG_mDCV_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 4: png_get_mDCV_fixed (26 undiscovered branches, 0% coverage)
    // Get modified DCV colorimetry (fixed-point version)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_mDCV_SUPPORTED
    if (fdp.remaining_bytes() >= 4) {
        png_fixed_point white_x_fixed_ret = 0, white_y_fixed_ret = 0;
        png_fixed_point red_x_fixed_ret = 0, red_y_fixed_ret = 0;
        png_fixed_point green_x_fixed_ret = 0, green_y_fixed_ret = 0;
        png_fixed_point blue_x_fixed_ret = 0, blue_y_fixed_ret = 0;
        png_uint_32 max_luminance_scaled_ret = 0, min_luminance_scaled_ret = 0;
        
        // Get mDCV colorimetry (fixed-point version)
        png_get_mDCV_fixed(png_ptr, info_ptr,
                           &white_x_fixed_ret, &white_y_fixed_ret,
                           &red_x_fixed_ret, &red_y_fixed_ret,
                           &green_x_fixed_ret, &green_y_fixed_ret,
                           &blue_x_fixed_ret, &blue_y_fixed_ret,
                           &max_luminance_scaled_ret, &min_luminance_scaled_ret);
    }
#endif // PNG_mDCV_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 5: png_get_cHRM (22 undiscovered branches, 0% coverage)
    // Get chromaticity (floating-point version)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_cHRM_SUPPORTED
    if (fdp.remaining_bytes() >= 4) {
        double white_x = 0.0, white_y = 0.0;
        double red_x = 0.0, red_y = 0.0;
        double green_x = 0.0, green_y = 0.0;
        double blue_x = 0.0, blue_y = 0.0;
        
        // Get chromaticity (floating-point version)
        png_get_cHRM(png_ptr, info_ptr,
                     &white_x, &white_y,
                     &red_x, &red_y,
                     &green_x, &green_y,
                     &blue_x, &blue_y);
    }
#endif // PNG_cHRM_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 6: png_get_cHRM_fixed (22 undiscovered branches, 0% coverage)
    // Get chromaticity (fixed-point version)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_cHRM_SUPPORTED
    if (fdp.remaining_bytes() >= 4) {
        png_fixed_point white_x_fixed = 0, white_y_fixed = 0;
        png_fixed_point red_x_fixed = 0, red_y_fixed = 0;
        png_fixed_point green_x_fixed = 0, green_y_fixed = 0;
        png_fixed_point blue_x_fixed = 0, blue_y_fixed = 0;
        
        // Get chromaticity (fixed-point version)
        png_get_cHRM_fixed(png_ptr, info_ptr,
                           &white_x_fixed, &white_y_fixed,
                           &red_x_fixed, &red_y_fixed,
                           &green_x_fixed, &green_y_fixed,
                           &blue_x_fixed, &blue_y_fixed);
    }
#endif // PNG_cHRM_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 7: png_get_cHRM_XYZ (26 undiscovered branches, 0% coverage)
    // Get CIE XYZ chromaticity (floating-point version)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_cHRM_SUPPORTED
    if (fdp.remaining_bytes() >= 4) {
        double red_X = 0.0, red_Y = 0.0, red_Z = 0.0;
        double green_X = 0.0, green_Y = 0.0, green_Z = 0.0;
        double blue_X = 0.0, blue_Y = 0.0, blue_Z = 0.0;
        
        // Get CIE XYZ chromaticity (floating-point version)
        png_get_cHRM_XYZ(png_ptr, info_ptr,
                         &red_X, &red_Y, &red_Z,
                         &green_X, &green_Y, &green_Z,
                         &blue_X, &blue_Y, &blue_Z);
    }
#endif // PNG_cHRM_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 8: png_set_cHRM_XYZ (0% coverage, not in guidance but related)
    // Set CIE XYZ chromaticity (floating-point version)
    ////////////////////////////////////////////////////////////////////////////
#ifdef PNG_cHRM_SUPPORTED
    if (fdp.remaining_bytes() >= 9 * sizeof(double)) {
        // Consume CIE XYZ chromaticity values
        double red_X = fdp.ConsumeFloatingPoint<double>();
        double red_Y = fdp.ConsumeFloatingPoint<double>();
        double red_Z = fdp.ConsumeFloatingPoint<double>();
        double green_X = fdp.ConsumeFloatingPoint<double>();
        double green_Y = fdp.ConsumeFloatingPoint<double>();
        double green_Z = fdp.ConsumeFloatingPoint<double>();
        double blue_X = fdp.ConsumeFloatingPoint<double>();
        double blue_Y = fdp.ConsumeFloatingPoint<double>();
        double blue_Z = fdp.ConsumeFloatingPoint<double>();
        
        // Set CIE XYZ chromaticity (floating-point version)
        png_set_cHRM_XYZ(png_ptr, info_ptr,
                         red_X, red_Y, red_Z,
                         green_X, green_Y, green_Z,
                         blue_X, blue_Y, blue_Z);
    }
#endif // PNG_cHRM_SUPPORTED
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 9: Comprehensive test combining multiple operations
    // Test set followed by get for both cHRM and mDCV
    ////////////////////////////////////////////////////////////////////////////
    if (fdp.remaining_bytes() >= 20 * sizeof(double)) {
        // Generate test data for comprehensive testing
        double test_values[20];
        for (int i = 0; i < 20 && fdp.remaining_bytes() >= sizeof(double); i++) {
            test_values[i] = fdp.ConsumeFloatingPoint<double>();
        }
        
        // Try to set cHRM if we have enough values
#ifdef PNG_cHRM_SUPPORTED
        if (fdp.remaining_bytes() >= 8 * sizeof(double)) {
            png_set_cHRM(png_ptr, info_ptr,
                         test_values[0], test_values[1],
                         test_values[2], test_values[3],
                         test_values[4], test_values[5],
                         test_values[6], test_values[7]);
            
            // Then try to get it back
            double retrieved[8];
            png_get_cHRM(png_ptr, info_ptr,
                         &retrieved[0], &retrieved[1],
                         &retrieved[2], &retrieved[3],
                         &retrieved[4], &retrieved[5],
                         &retrieved[6], &retrieved[7]);
        }
#endif // PNG_cHRM_SUPPORTED
        
        // Try to set mDCV if we have enough values
#ifdef PNG_mDCV_SUPPORTED
        if (fdp.remaining_bytes() >= 10 * sizeof(double)) {
            png_set_mDCV(png_ptr, info_ptr,
                         test_values[0], test_values[1],
                         test_values[2], test_values[3],
                         test_values[4], test_values[5],
                         test_values[6], test_values[7],
                         test_values[8], test_values[9]);
            
            // Then try to get it back
            double retrieved_mdcv[10];
            png_get_mDCV(png_ptr, info_ptr,
                         &retrieved_mdcv[0], &retrieved_mdcv[1],
                         &retrieved_mdcv[2], &retrieved_mdcv[3],
                         &retrieved_mdcv[4], &retrieved_mdcv[5],
                         &retrieved_mdcv[6], &retrieved_mdcv[7],
                         &retrieved_mdcv[8], &retrieved_mdcv[9]);
        }
#endif // PNG_mDCV_SUPPORTED
    }
    
    ////////////////////////////////////////////////////////////////////////////
    // Cleanup
    ////////////////////////////////////////////////////////////////////////////
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_011
