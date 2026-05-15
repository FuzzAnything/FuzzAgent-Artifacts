// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include "code/png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - prevent longjmp
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Do nothing
}

static png_structp create_png_struct(bool for_write) {
    png_structp png_ptr;
    
    if (for_write) {
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                          dummy_error_fn, dummy_warning_fn);
    } else {
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                         dummy_error_fn, dummy_warning_fn);
    }
    
    if (png_ptr) {
        // Disable error handling that triggers longjmp
        png_set_error_fn(png_ptr, nullptr, dummy_error_fn, dummy_warning_fn);
    }
    
    return png_ptr;
}

static png_infop create_info_struct(png_structp png_ptr) {
    return png_create_info_struct(png_ptr);
}

static void extract_doubles(const uint8_t* data, size_t size, size_t offset, 
                           double* values, size_t count) {
    if (offset + count * sizeof(double) > size) {
        for (size_t i = 0; i < count; i++) {
            // Use safe default values that won't trigger validation errors
            values[i] = 0.5;
        }
        return;
    }
    
    for (size_t i = 0; i < count; i++) {
        double val;
        memcpy(&val, data + offset + i * sizeof(double), sizeof(double));
        
        // Handle special floating-point values
        if (std::isnan(val) || std::isinf(val) || std::fpclassify(val) == FP_SUBNORMAL) {
            val = 0.5;
        }
        
        // Clamp to reasonable range for color coordinates
        // cHRM XYZ values must be positive and typically within 0-1 range
        if (val < 0.0) val = 0.0;
        if (val > 2.0) val = 2.0;  // Allow slightly above 1.0
        
        values[i] = val;
    }
}

static void extract_fixed_points(const uint8_t* data, size_t size, size_t offset,
                                png_fixed_point* values, size_t count) {
    // Ensure we don't write beyond the allocated array
    if (offset > size) {
        for (size_t i = 0; i < count; i++) {
            values[i] = 50000;  // Safe default
        }
        return;
    }
    
    // Calculate how many values we can safely extract
    size_t max_values = (size - offset) / sizeof(png_fixed_point);
    size_t values_to_extract = (max_values < count) ? max_values : count;
    
    // Extract available values
    for (size_t i = 0; i < values_to_extract; i++) {
        png_fixed_point val;
        memcpy(&val, data + offset + i * sizeof(png_fixed_point), sizeof(png_fixed_point));
        
        // Clamp to reasonable range to avoid validation errors
        if (val < 0) val = 0;
        if (val > 100000) val = 100000;
        
        values[i] = val;
    }
    
    // Fill remaining values with safe defaults
    for (size_t i = values_to_extract; i < count; i++) {
        values[i] = 50000;
    }
}

static void extract_bytes(const uint8_t* data, size_t size, size_t offset,
                         png_byte* values, size_t count) {
    if (offset > size) {
        for (size_t i = 0; i < count; i++) {
            values[i] = 1;  // Safe default
        }
        return;
    }
    
    // Calculate how many bytes we can safely extract
    size_t max_bytes = size - offset;
    size_t bytes_to_extract = (max_bytes < count) ? max_bytes : count;
    
    // Extract available bytes
    for (size_t i = 0; i < bytes_to_extract; i++) {
        values[i] = data[offset + i];
    }
    
    // Fill remaining bytes with safe defaults
    for (size_t i = bytes_to_extract; i < count; i++) {
        values[i] = 1;
    }
}

extern "C" int LLVMFuzzerTestOneInput_210(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine test case
    uint8_t test_case = Data[0] % 6;
    size_t data_offset = 1;
    
    // Create PNG structures
    png_structp png_ptr = create_png_struct(true); // Use write struct for setting
    if (!png_ptr) return 0;
    
    png_infop info_ptr = create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set up write functions
    png_set_write_fn(png_ptr, nullptr, dummy_write_fn, dummy_flush_fn);
    
    // Test different API functions based on test_case
    switch (test_case) {
        case 0: { // Test png_set_cHRM_XYZ and png_get_cHRM_XYZ
            double color_vals[9];
            extract_doubles(Data, Size, data_offset, color_vals, 9);
            
            // Ensure values are valid to avoid png_error
            // cHRM XYZ values must sum appropriately, but we'll just use safe values
            // Convert any negative values to positive
            for (int i = 0; i < 9; i++) {
                if (color_vals[i] < 0.0) color_vals[i] = 0.0;
            }
            
            // Set values
            png_set_cHRM_XYZ(png_ptr, info_ptr, 
                            color_vals[0], color_vals[1], color_vals[2],
                            color_vals[3], color_vals[4], color_vals[5],
                            color_vals[6], color_vals[7], color_vals[8]);
            
            // Try to get them back
            double out_vals[9];
            png_get_cHRM_XYZ(png_ptr, info_ptr,
                            &out_vals[0], &out_vals[1], &out_vals[2],
                            &out_vals[3], &out_vals[4], &out_vals[5],
                            &out_vals[6], &out_vals[7], &out_vals[8]);
            break;
        }
        
        case 1: { // Test png_get_cHRM (will return 0 since we haven't set chromaticity)
            double get_vals[8];
            png_uint_32 result = png_get_cHRM(png_ptr, info_ptr,
                                             &get_vals[0], &get_vals[1],
                                             &get_vals[2], &get_vals[3],
                                             &get_vals[4], &get_vals[5],
                                             &get_vals[6], &get_vals[7]);
            
            // Now set some values and try again
            double set_vals[8];
            extract_doubles(Data, Size, data_offset, set_vals, 8);
            
            // Ensure values are valid
            for (int i = 0; i < 8; i++) {
                if (set_vals[i] < 0.0) set_vals[i] = 0.0;
                if (set_vals[i] > 1.0) set_vals[i] = 1.0;
            }
            
            png_set_cHRM(png_ptr, info_ptr,
                        set_vals[0], set_vals[1],
                        set_vals[2], set_vals[3],
                        set_vals[4], set_vals[5],
                        set_vals[6], set_vals[7]);
            
            result = png_get_cHRM(png_ptr, info_ptr,
                                 &get_vals[0], &get_vals[1],
                                 &get_vals[2], &get_vals[3],
                                 &get_vals[4], &get_vals[5],
                                 &get_vals[6], &get_vals[7]);
            break;
        }
        
        case 2: { // Test png_get_cICP and png_set_cICP
            png_byte byte_vals[4];
            extract_bytes(Data, Size, data_offset, byte_vals, 4);
            
            // Set values first
            png_set_cICP(png_ptr, info_ptr,
                        byte_vals[0], byte_vals[1],
                        byte_vals[2], byte_vals[3]);
            
            // Try to get them back
            png_byte out_vals[4];
            png_uint_32 result = png_get_cICP(png_ptr, info_ptr,
                                             &out_vals[0], &out_vals[1], 
                                             &out_vals[2], &out_vals[3]);
            break;
        }
        
        case 3: { // Test png_set_cICP with different values
            png_byte byte_vals[4];
            extract_bytes(Data, Size, data_offset, byte_vals, 4);
            
            // Test with various combinations
            for (int i = 0; i < 3; i++) {
                png_byte test_vals[4];
                for (int j = 0; j < 4; j++) {
                    test_vals[j] = (byte_vals[j] + i) % 256;
                }
                
                png_set_cICP(png_ptr, info_ptr,
                            test_vals[0], 
                            test_vals[1],
                            test_vals[2],
                            test_vals[3]);
            }
            break;
        }
        
        case 4: { // Test png_set_cHRM with fixed point
            png_fixed_point fixed_vals[8];
            extract_fixed_points(Data, Size, data_offset, fixed_vals, 8);
            
            // Ensure values are within valid range
            for (int i = 0; i < 8; i++) {
                if (fixed_vals[i] < 0) fixed_vals[i] = 0;
                if (fixed_vals[i] > 100000) fixed_vals[i] = 100000;
            }
            
            png_set_cHRM_fixed(png_ptr, info_ptr,
                              fixed_vals[0], fixed_vals[1],
                              fixed_vals[2], fixed_vals[3],
                              fixed_vals[4], fixed_vals[5],
                              fixed_vals[6], fixed_vals[7]);
            
            // Try to get them back
            png_fixed_point out_vals[8];
            png_get_cHRM_fixed(png_ptr, info_ptr,
                              &out_vals[0], &out_vals[1],
                              &out_vals[2], &out_vals[3],
                              &out_vals[4], &out_vals[5],
                              &out_vals[6], &out_vals[7]);
            break;
        }
        
        case 5: { // Test png_set_cHRM_XYZ_fixed
            png_fixed_point fixed_vals[9];
            extract_fixed_points(Data, Size, data_offset, fixed_vals, 9);
            
            // Ensure values are within valid range
            for (int i = 0; i < 9; i++) {
                if (fixed_vals[i] < 0) fixed_vals[i] = 0;
                if (fixed_vals[i] > 100000) fixed_vals[i] = 100000;
            }
            
            png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                  fixed_vals[0], fixed_vals[1], fixed_vals[2],
                                  fixed_vals[3], fixed_vals[4], fixed_vals[5],
                                  fixed_vals[6], fixed_vals[7], fixed_vals[8]);
            
            // Try to get them back
            png_fixed_point out_vals[9];
            png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                  &out_vals[0], &out_vals[1], &out_vals[2],
                                  &out_vals[3], &out_vals[4], &out_vals[5],
                                  &out_vals[6], &out_vals[7], &out_vals[8]);
            break;
        }
    }
    
    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}