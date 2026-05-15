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

namespace harness_009 {

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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for PNG metadata operations
    // We need enough data for:
    // - Basic image parameters (width, height, bit_depth, color_type, etc.): ~20 bytes
    // - mDCV parameters: 10 png_fixed_point values + 2 png_uint_32 values = ~48 bytes  
    // - cHRM parameters: 8 png_fixed_point values = ~32 bytes
    // - pCAL parameters: purpose string, X0, X1, type, nparams, units, params
    if (size < 200) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create PNG write structure - TARGET HELPER: png_create_write_struct
    // We use write structure since we're setting metadata
    png_structp png_ptr = png_create_write_struct(
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
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_info_struct(png_ptr, &info_ptr);
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
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
    
    // Step 4: Set color metadata using various setter functions
    
    // Determine which metadata operations to perform based on fuzzer input
    uint8_t operation_mask = fdp.ConsumeIntegral<uint8_t>();
    
    // mDCV metadata (Mastering Display Color Volume)
    if (operation_mask & 0x01) {
        // Use fixed-point version: png_set_mDCV_fixed
        // Consume 10 png_fixed_point values for chromaticities
        png_fixed_point white_x = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point white_y = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point red_x = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point red_y = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point green_x = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point green_y = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point blue_x = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point blue_y = fdp.ConsumeIntegral<png_fixed_point>();
        png_uint_32 maxDL = fdp.ConsumeIntegral<png_uint_32>();
        png_uint_32 minDL = fdp.ConsumeIntegral<png_uint_32>();
        
        // PRIMARY TARGET: png_set_mDCV_fixed (called by png_set_mDCV)
        // Note: png_set_mDCV is the floating-point version that calls png_set_mDCV_fixed
        // We'll test both paths
        if (operation_mask & 0x02) {
            // Use floating-point version
            double white_x_fp = fdp.ConsumeFloatingPoint<double>();
            double white_y_fp = fdp.ConsumeFloatingPoint<double>();
            double red_x_fp = fdp.ConsumeFloatingPoint<double>();
            double red_y_fp = fdp.ConsumeFloatingPoint<double>();
            double green_x_fp = fdp.ConsumeFloatingPoint<double>();
            double green_y_fp = fdp.ConsumeFloatingPoint<double>();
            double blue_x_fp = fdp.ConsumeFloatingPoint<double>();
            double blue_y_fp = fdp.ConsumeFloatingPoint<double>();
            double maxDL_fp = fdp.ConsumeFloatingPoint<double>();
            double minDL_fp = fdp.ConsumeFloatingPoint<double>();
            
            // PRIMARY TARGET: png_set_mDCV (20 branches)
            png_set_mDCV(png_ptr, info_ptr, white_x_fp, white_y_fp, red_x_fp, red_y_fp,
                        green_x_fp, green_y_fp, blue_x_fp, blue_y_fp,
                        maxDL_fp, minDL_fp);
        } else {
            // Use fixed-point version directly
            #ifdef PNG_FIXED_POINT_SUPPORTED
            png_set_mDCV_fixed(png_ptr, info_ptr, white_x, white_y, red_x, red_y,
                              green_x, green_y, blue_x, blue_y, maxDL, minDL);
            #endif
        }
    }
    
    // cHRM metadata (Chromaticity and White Point)
    if (operation_mask & 0x04) {
        // Use cHRM XYZ fixed-point version
        png_fixed_point int_red_X = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point int_red_Y = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point int_red_Z = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point int_green_X = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point int_green_Y = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point int_green_Z = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point int_blue_X = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point int_blue_Y = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point int_blue_Z = fdp.ConsumeIntegral<png_fixed_point>();
        
        // PRIMARY TARGET: png_set_cHRM_XYZ_fixed (20 branches)
        png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, int_red_X, int_red_Y, int_red_Z,
                              int_green_X, int_green_Y, int_green_Z,
                              int_blue_X, int_blue_Y, int_blue_Z);
    }
    
    if (operation_mask & 0x08) {
        // Use regular cHRM fixed-point version
        png_fixed_point white_x = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point white_y = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point red_x = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point red_y = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point green_x = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point green_y = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point blue_x = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point blue_y = fdp.ConsumeIntegral<png_fixed_point>();
        
        png_set_cHRM_fixed(png_ptr, info_ptr, white_x, white_y, red_x, red_y,
                          green_x, green_y, blue_x, blue_y);
    }
    
    // pCAL metadata (Pixel Calibration)
    if (operation_mask & 0x10 && fdp.remaining_bytes() > 100) {
        // Consume pCAL parameters
        std::string purpose = fdp.ConsumeRandomLengthString(50);
        png_int_32 X0 = fdp.ConsumeIntegral<png_int_32>();
        png_int_32 X1 = fdp.ConsumeIntegral<png_int_32>();
        int type = fdp.ConsumeIntegralInRange<int>(0, 3);
        int nparams = fdp.ConsumeIntegralInRange<int>(0, 100);  // Reasonable limit for fuzzing
        
        std::string units = fdp.ConsumeRandomLengthString(20);
        
        // Allocate params array if needed
        char** params = nullptr;
        
        if (nparams > 0) {
            // Allocate array for parameter strings
            params = (char**)png_malloc(png_ptr, nparams * sizeof(char*));
            if (!params) {
                // Memory allocation failed, skip pCAL
                nparams = 0;
            } else {
                // Initialize to nullptr
                for (int i = 0; i < nparams; i++) {
                    params[i] = nullptr;
                }
                
                // Allocate and populate individual parameter strings
                for (int i = 0; i < nparams && fdp.remaining_bytes() > 10; i++) {
                    // Consume string that should be a valid floating-point representation
                    std::string param = fdp.ConsumeRandomLengthString(50);
                    
                    // Simple validation: ensure string is not empty and looks like a number
                    // This helps png_check_fp_string() succeed more often
                    bool looks_like_number = false;
                    if (!param.empty()) {
                        // Check if first character could start a number: digit, '.', '+', or '-'
                        char first = param[0];
                        if (std::isdigit(first) || first == '.' || first == '+' || first == '-') {
                            looks_like_number = true;
                        }
                    }
                    
                    // If doesn't look like a number, use a simple default
                    if (!looks_like_number) {
                        // Use fuzzer input to generate a simple number instead of hardcoded value
                        int num = fdp.ConsumeIntegralInRange<int>(-1000, 1000);
                        param = std::to_string(num);
                    }
                    
                    params[i] = (char*)png_malloc(png_ptr, param.length() + 1);
                    if (!params[i]) {
                        // Allocation failed, free previous strings and abort
                        for (int j = 0; j < i; j++) {
                            if (params[j]) {
                                png_free(png_ptr, params[j]);
                            }
                        }
                        png_free(png_ptr, params);
                        params = nullptr;
                        nparams = 0;
                        break;
                    }
                    
                    // Copy string with null termination
                    strncpy(params[i], param.c_str(), param.length());
                    params[i][param.length()] = '\0';
                }
            }
        }
        
        // Call png_set_pCAL if we have valid parameters
        // Note: png_set_pCAL accepts params == nullptr when nparams == 0
        png_set_pCAL(png_ptr, info_ptr, purpose.c_str(), X0, X1, type, nparams,
                    units.c_str(), params);
        
        // Clean up our temporary allocations
        // png_set_pCAL makes its own copies, so we free our temporary memory
        if (params) {
            for (int i = 0; i < nparams; i++) {
                if (params[i]) {
                    png_free(png_ptr, params[i]);
                }
            }
            png_free(png_ptr, params);
        }
    }

    
    // Step 5: Retrieve and verify metadata using getter functions
    
    // Retrieve mDCV metadata (if set)
    if (operation_mask & 0x01) {
        png_fixed_point white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
        png_uint_32 maxDL, minDL;
        
        // PRIMARY TARGET: png_get_mDCV_fixed (26 branches)
        png_uint_32 result = png_get_mDCV_fixed(png_ptr, info_ptr, &white_x, &white_y,
                                               &red_x, &red_y, &green_x, &green_y,
                                               &blue_x, &blue_y, &maxDL, &minDL);
        (void)result; // Result indicates if metadata was present
        
        // Also test floating-point version
        double white_x_fp, white_y_fp, red_x_fp, red_y_fp, green_x_fp, green_y_fp, blue_x_fp, blue_y_fp;
        double maxDL_fp, minDL_fp;
        
        // PRIMARY TARGET: png_get_mDCV (26 branches)
        result = png_get_mDCV(png_ptr, info_ptr, &white_x_fp, &white_y_fp,
                             &red_x_fp, &red_y_fp, &green_x_fp, &green_y_fp,
                             &blue_x_fp, &blue_y_fp, &maxDL_fp, &minDL_fp);
        (void)result;
    }
    
    // Retrieve cHRM metadata (if set)
    if (operation_mask & 0x04) {
        png_fixed_point int_red_X, int_red_Y, int_red_Z;
        png_fixed_point int_green_X, int_green_Y, int_green_Z;
        png_fixed_point int_blue_X, int_blue_Y, int_blue_Z;
        
        // PRIMARY TARGET: png_get_cHRM_XYZ_fixed (26 branches)
        png_uint_32 result = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X, &int_red_Y, &int_red_Z,
                                                   &int_green_X, &int_green_Y, &int_green_Z,
                                                   &int_blue_X, &int_blue_Y, &int_blue_Z);
        (void)result;
    }
    
    if (operation_mask & 0x08) {
        png_fixed_point white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
        
        // PRIMARY TARGET: png_get_cHRM_fixed (22 branches)
        png_uint_32 result = png_get_cHRM_fixed(png_ptr, info_ptr, &white_x, &white_y,
                                               &red_x, &red_y, &green_x, &green_y,
                                               &blue_x, &blue_y);
        (void)result;
    }
    
    // Retrieve pCAL metadata (if set)
    if (operation_mask & 0x10) {
        char* purpose = nullptr;
        png_int_32 X0, X1;
        int type, nparams;
        char* units = nullptr;
        char** params = nullptr;
        
        // PRIMARY TARGET: png_get_pCAL (20 branches)
        png_uint_32 result = png_get_pCAL(png_ptr, info_ptr, &purpose, &X0, &X1,
                                         &type, &nparams, &units, &params);
        (void)result;
        
        // pCAL data should be freed by libpng, but we don't need to do anything here
    }
    
    // Step 6: Cleanup - TARGET HELPER: png_destroy_info_struct, png_destroy_write_struct
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_write_struct(&png_ptr, nullptr);
    
    return 0;
}

} // namespace harness_009
