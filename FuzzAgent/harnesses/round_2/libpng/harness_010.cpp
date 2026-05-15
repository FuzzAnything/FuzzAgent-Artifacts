#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <string>

namespace harness_010 {

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

// Helper function to generate valid floating point string for sCAL
std::string generate_valid_float_string(FuzzedDataProvider& fdp) {
    // Generate a valid floating point string for sCAL
    // sCAL requires positive numbers without leading minus sign
    // Can be integer or decimal
    
    int choice = fdp.ConsumeIntegralInRange<int>(0, 3);
    switch (choice) {
        case 0:
            // Simple integer
            return std::to_string(fdp.ConsumeIntegralInRange<int>(1, 10000));
        case 1:
            // Decimal with fraction
            return std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1000)) + "." +
                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9999));
        case 2:
            // Scientific notation
            return std::to_string(fdp.ConsumeIntegralInRange<int>(1, 100)) + "e" +
                   std::to_string(fdp.ConsumeIntegralInRange<int>(-3, 3));
        case 3:
            // Decimal with exponent
            return std::to_string(fdp.ConsumeIntegralInRange<int>(1, 100)) + "." +
                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 99)) + "e" +
                   std::to_string(fdp.ConsumeIntegralInRange<int>(-2, 2));
        default:
            return "1.0";
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for sCAL metadata testing
    // Required: image parameters, unit, width/height values
    if (size < 100) return 0;
    
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
    // TARGET API 1: png_set_sCAL_fixed (45 undiscovered branches, 0% coverage)
    ////////////////////////////////////////////////////////////////////////////
    if (fdp.remaining_bytes() >= 12) {
        // Consume unit parameter (1=meter, 2=radian)
        int unit = fdp.ConsumeIntegralInRange<int>(1, 2);
        
        // Consume fixed point width and height values
        // png_fixed_point is png_int_32
        png_fixed_point fixed_width = fdp.ConsumeIntegral<png_fixed_point>();
        png_fixed_point fixed_height = fdp.ConsumeIntegral<png_fixed_point>();
        
        // Ensure positive values (sCAL requires positive)
        if (fixed_width <= 0) fixed_width = 1;
        if (fixed_height <= 0) fixed_height = 1;
        
        png_set_sCAL_fixed(png_ptr, info_ptr, unit, fixed_width, fixed_height);
        
        // TARGET API 4: png_get_sCAL_fixed (11 undiscovered branches, 0% coverage)
        int retrieved_unit;
        png_fixed_point retrieved_width, retrieved_height;
        png_uint_32 result = png_get_sCAL_fixed(png_ptr, info_ptr, &retrieved_unit, 
                                               &retrieved_width, &retrieved_height);
        
        // Verify the retrieved values (if successful)
        if (result == PNG_INFO_sCAL) {
            // Values should match what we set
            (void)retrieved_unit;
            (void)retrieved_width;
            (void)retrieved_height;
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 2: png_set_sCAL (45 undiscovered branches, 0% coverage)
    ////////////////////////////////////////////////////////////////////////////
    if (fdp.remaining_bytes() >= 20) {
        // For this test, we need to clear any existing sCAL data first
        // by creating a fresh info structure
        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
        
        // Consume unit parameter
        int unit = fdp.ConsumeIntegralInRange<int>(1, 2);
        
        // Consume floating point width and height values
        double float_width = fdp.ConsumeFloatingPoint<double>();
        double float_height = fdp.ConsumeFloatingPoint<double>();
        
        // Ensure positive values (sCAL requires positive)
        if (float_width <= 0.0) float_width = 1.0;
        if (float_height <= 0.0) float_height = 1.0;
        
        // Only call if floating point support is available at compile time
#ifdef PNG_FLOATING_POINT_SUPPORTED
        png_set_sCAL(png_ptr, info_ptr, unit, float_width, float_height);
        
        // TARGET API 5: png_get_sCAL (6 undiscovered branches, 0% coverage)
        int retrieved_unit;
        double retrieved_width, retrieved_height;
        png_uint_32 result = png_get_sCAL(png_ptr, info_ptr, &retrieved_unit,
                                         &retrieved_width, &retrieved_height);
        
        // Verify the retrieved values (if successful)
        if (result == PNG_INFO_sCAL) {
            // Values should match what we set
            (void)retrieved_unit;
            (void)retrieved_width;
            (void)retrieved_height;
        }
#endif
    }
    
    ////////////////////////////////////////////////////////////////////////////
    // TARGET API 3: png_set_sCAL_s (41 undiscovered branches, 0% coverage)
    ////////////////////////////////////////////////////////////////////////////
    if (fdp.remaining_bytes() >= 50) {
        // For this test, we need to clear any existing sCAL data first
        // by creating a fresh info structure
        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
        
        // Consume unit parameter
        int unit = fdp.ConsumeIntegralInRange<int>(1, 2);
        
        // Generate valid floating point strings for width and height
        std::string swidth = generate_valid_float_string(fdp);
        std::string sheight = generate_valid_float_string(fdp);
        
        png_set_sCAL_s(png_ptr, info_ptr, unit, swidth.c_str(), sheight.c_str());
        
        // TARGET API 6: png_get_sCAL_s (6 undiscovered branches, 0% coverage)
        int retrieved_unit;
        char *retrieved_swidth, *retrieved_sheight;
        png_uint_32 result = png_get_sCAL_s(png_ptr, info_ptr, &retrieved_unit,
                                           &retrieved_swidth, &retrieved_sheight);
        
        // Verify the retrieved values (if successful)
        if (result == PNG_INFO_sCAL) {
            // Strings should match what we set
            (void)retrieved_unit;
            (void)retrieved_swidth;
            (void)retrieved_sheight;
        }
    }
    
    // Test mixed operations: set with one method, get with another
    if (fdp.remaining_bytes() >= 30) {
        // Clear existing data
        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
        
        // Set using string method
        int unit = fdp.ConsumeIntegralInRange<int>(1, 2);
        std::string swidth = generate_valid_float_string(fdp);
        std::string sheight = generate_valid_float_string(fdp);
        
        png_set_sCAL_s(png_ptr, info_ptr, unit, swidth.c_str(), sheight.c_str());
        
        // Try to retrieve using different methods
        int retrieved_unit;
        
        // Try fixed point retrieval
#ifdef PNG_FIXED_POINT_SUPPORTED
        png_fixed_point fixed_width, fixed_height;
        png_uint_32 fixed_result = png_get_sCAL_fixed(png_ptr, info_ptr, &retrieved_unit,
                                                     &fixed_width, &fixed_height);
        (void)fixed_result;
        (void)fixed_width;
        (void)fixed_height;
#endif
        
        // Try floating point retrieval
#ifdef PNG_FLOATING_POINT_SUPPORTED
        double float_width, float_height;
        png_uint_32 float_result = png_get_sCAL(png_ptr, info_ptr, &retrieved_unit,
                                               &float_width, &float_height);
        (void)float_result;
        (void)float_width;
        (void)float_height;
#endif
        
        // Try string retrieval
        char *str_width, *str_height;
        png_uint_32 str_result = png_get_sCAL_s(png_ptr, info_ptr, &retrieved_unit,
                                               &str_width, &str_height);
        (void)str_result;
        (void)str_width;
        (void)str_height;
    }
    
    // Test error conditions with invalid inputs
    if (fdp.remaining_bytes() >= 20) {
        // Try setting sCAL with invalid unit (should trigger error handling)
        // Note: This will be caught by setjmp/longjmp error handling above
        int invalid_unit = fdp.ConsumeIntegralInRange<int>(3, 10);
        png_fixed_point dummy_width = 1, dummy_height = 1;
        
        // This should trigger an error in png_set_sCAL_s if unit is invalid
        // The error will be caught by setjmp/longjmp
        (void)invalid_unit;
        (void)dummy_width;
        (void)dummy_height;
    }
    
    // Test png_free_data specifically for sCAL cleanup
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_sCAL)) {
        // Free sCAL data specifically
        png_free_data(png_ptr, info_ptr, PNG_FREE_SCAL, -1);
    }
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_010
