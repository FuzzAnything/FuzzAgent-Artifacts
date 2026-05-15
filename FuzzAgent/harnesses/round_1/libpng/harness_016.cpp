#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_016 {
/*
 * Harness for testing libpng advanced color calibration metadata operations.
 * Targets high-complexity, currently uncovered APIs in pngset.c and pngget.c:
 * - png_set_mDCV (completely uncovered, 13 undiscovered branches)
 * - png_set_cLLI (completely uncovered, 12 undiscovered branches)
 * - png_set_cHRM_XYZ (completely uncovered, 11 undiscovered branches)
 * - png_set_cHRM_XYZ_fixed (completely uncovered, 11 undiscovered branches)
 * - png_get_mDCV (reached but not executed, 26 undiscovered branches)
 * - png_get_cHRM_XYZ (reached but not executed, 26 undiscovered branches)
 *
 * This harness implements the complete lifecycle for advanced color calibration metadata:
 * 1. Mastering Display Color Volume (mDCV) - display chromaticities and luminance
 * 2. Content Light Level Information (cLLI) - maximum content/frame light levels
 * 3. Chromaticity XYZ (cHRM_XYZ) - display primaries in XYZ color space
 *
 * These functions represent 47+ points of completely undiscovered complexity plus
 * 52+ points of reached-but-not-executed complexity in getters. They form a logical
 * cluster for advanced color calibration metadata handling, semantically distinct
 * from existing harnesses.
 *
 * Semantically different from previous harnesses:
 * - harness_014.cpp: CICP colorimetric information and unknown chunk handling
 * - harness_015.cpp: Physical dimension metadata (pHYs and oFFs chunks)
 * - This harness: Advanced color calibration (mDCV, cLLI, cHRM_XYZ)
 */

// ==================== MEMORY BUFFER FOR WRITE/READ ====================

struct mem_buffer {
    uint8_t* data;
    size_t size;
    size_t capacity;
    size_t offset;  // For reading
};

// Custom write function for writing to memory buffer
static void png_write_to_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    mem_buffer* buffer = (mem_buffer*)io_ptr;
    
    // Resize buffer if needed
    if (buffer->offset + length > buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity < buffer->offset + length) {
            new_capacity = buffer->offset + length;
        }
        
        uint8_t* new_data = (uint8_t*)realloc(buffer->data, new_capacity);
        if (new_data == NULL) {
            png_error(png_ptr, "Memory allocation failed");
            return;
        }
        
        buffer->data = new_data;
        buffer->capacity = new_capacity;
    }
    
    // Copy data to buffer
    memcpy(buffer->data + buffer->offset, data, length);
    buffer->offset += length;
    buffer->size = buffer->offset;
}

// Custom flush function (does nothing for memory buffer)
static void png_flush_memory(png_structp png_ptr) {
    (void)png_ptr;
}

// Custom read function for reading from memory buffer
static void png_read_from_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    mem_buffer* buffer = (mem_buffer*)io_ptr;
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

// Callback functions for libpng error handling
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// ==================== MAIN FUZZER FUNCTION ====================

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough data for all parameters
    // Fixed parameters: 8 chromaticity values (double) + 2 luminance values + 2 light levels
    // Plus image dimensions and pixel data
    const size_t MIN_SIZE = sizeof(double) * 10 + sizeof(uint32_t) * 2 + sizeof(uint8_t) * 100;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== CONSUME INPUT FOR ALL PARAMETERS ====================
    
    // Consume image dimensions (keep reasonable)
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 256);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 256);
    
    // Consume color calibration parameters with appropriate ranges
    // Chromaticity values typically range 0.0-1.0, but mDCV allows up to ~1.3107
    double white_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double white_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double red_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double red_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double green_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double green_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double blue_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double blue_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    
    // Mastering display luminance in cd/m² (nits) - reasonable range
    double maxDL = fdp.ConsumeFloatingPointInRange<double>(0.0, 200000.0);  // Up to 200k cd/m²
    double minDL = fdp.ConsumeFloatingPointInRange<double>(0.0, 200000.0);
    
    // Content light level information - also in cd/m²
    double maxCLL = fdp.ConsumeFloatingPointInRange<double>(0.0, 10000.0);
    double maxFALL = fdp.ConsumeFloatingPointInRange<double>(0.0, 10000.0);
    
    // Chromaticity XYZ values - XYZ coordinates for primaries
    // libpng requires strictly positive values (greater than 0), so use lower bound
    // Must be large enough that after conversion to fixed-point (multiply by 100000.0),
    // the value remains positive (>= 1 when cast to int32_t)
    const double MIN_XYZ_VALUE = 0.00001;  // 1e-5, ensures value * 100000.0 >= 1.0
    double red_X = fdp.ConsumeFloatingPointInRange<double>(MIN_XYZ_VALUE, 1.0);
    double red_Y = fdp.ConsumeFloatingPointInRange<double>(MIN_XYZ_VALUE, 1.0);
    double red_Z = fdp.ConsumeFloatingPointInRange<double>(MIN_XYZ_VALUE, 1.0);
    double green_X = fdp.ConsumeFloatingPointInRange<double>(MIN_XYZ_VALUE, 1.0);
    double green_Y = fdp.ConsumeFloatingPointInRange<double>(MIN_XYZ_VALUE, 1.0);
    double green_Z = fdp.ConsumeFloatingPointInRange<double>(MIN_XYZ_VALUE, 1.0);
    double blue_X = fdp.ConsumeFloatingPointInRange<double>(MIN_XYZ_VALUE, 1.0);
    double blue_Y = fdp.ConsumeFloatingPointInRange<double>(MIN_XYZ_VALUE, 1.0);
    double blue_Z = fdp.ConsumeFloatingPointInRange<double>(MIN_XYZ_VALUE, 1.0);
    // Consume pixel data for image
    size_t pixel_data_size = width * height * 4;  // RGBA
    if (fdp.remaining_bytes() < pixel_data_size) {
        return 0;  // Not enough data for image
    }
    std::vector<uint8_t> pixel_data = fdp.ConsumeBytes<uint8_t>(pixel_data_size);
    
    // ==================== SETUP PNG WRITE STRUCTURES ====================
    
    // Initialize memory buffer for PNG output
    mem_buffer buffer;
    buffer.data = (uint8_t*)malloc(4096);
    if (buffer.data == NULL) {
        return 0;
    }
    buffer.capacity = 4096;
    buffer.size = 0;
    buffer.offset = 0;
    
    // Create PNG write structure
    png_structp png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                        NULL, 
                                                        png_error_callback, 
                                                        png_warning_callback);
    if (png_write_ptr == NULL) {
        free(buffer.data);
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_write_ptr = png_create_info_struct(png_write_ptr);
    if (info_write_ptr == NULL) {
        png_destroy_write_struct(&png_write_ptr, NULL);
        free(buffer.data);
        return 0;
    }
    
    // Set up custom IO for memory buffer
    png_set_write_fn(png_write_ptr, &buffer, png_write_to_memory, png_flush_memory);
    
    // ==================== SET BASIC IMAGE PROPERTIES ====================
    
    // Set IHDR (basic image properties)
    png_set_IHDR(png_write_ptr, info_write_ptr, width, height,
                 8,  // bit depth
                 PNG_COLOR_TYPE_RGBA,  // color type
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    
    // ==================== SET ADVANCED COLOR CALIBRATION METADATA ====================
    
    // Set Mastering Display Color Volume (mDCV) - floating point version
    png_set_mDCV(png_write_ptr, info_write_ptr,
                 white_x, white_y,
                 red_x, red_y,
                 green_x, green_y,
                 blue_x, blue_y,
                 maxDL, minDL);
    
    // Set Content Light Level Information (cLLI) - floating point version
    png_set_cLLI(png_write_ptr, info_write_ptr, maxCLL, maxFALL);
    
    // Set chromaticity XYZ (cHRM_XYZ) - floating point version
    png_set_cHRM_XYZ(png_write_ptr, info_write_ptr,
                     red_X, red_Y, red_Z,
                     green_X, green_Y, green_Z,
                     blue_X, blue_Y, blue_Z);
    
    // Also test fixed-point versions with converted values
    // Convert doubles to fixed-point for testing both APIs
    png_fixed_point int_red_X = static_cast<png_fixed_point>(red_X * 100000.0);
    png_fixed_point int_red_Y = static_cast<png_fixed_point>(red_Y * 100000.0);
    png_fixed_point int_red_Z = static_cast<png_fixed_point>(red_Z * 100000.0);
    png_fixed_point int_green_X = static_cast<png_fixed_point>(green_X * 100000.0);
    png_fixed_point int_green_Y = static_cast<png_fixed_point>(green_Y * 100000.0);
    png_fixed_point int_green_Z = static_cast<png_fixed_point>(green_Z * 100000.0);
    png_fixed_point int_blue_X = static_cast<png_fixed_point>(blue_X * 100000.0);
    png_fixed_point int_blue_Y = static_cast<png_fixed_point>(blue_Y * 100000.0);
    png_fixed_point int_blue_Z = static_cast<png_fixed_point>(blue_Z * 100000.0);
    
    // Set cHRM_XYZ_fixed (fixed-point version)
    png_set_cHRM_XYZ_fixed(png_write_ptr, info_write_ptr,
                           int_red_X, int_red_Y, int_red_Z,
                           int_green_X, int_green_Y, int_green_Z,
                           int_blue_X, int_blue_Y, int_blue_Z);
    
    // ==================== WRITE PNG DATA ====================
    
    // Write PNG header with all metadata
    png_write_info(png_write_ptr, info_write_ptr);
    
    // Prepare row pointers for image data
    std::vector<png_bytep> row_pointers(height);
    for (uint32_t y = 0; y < height; y++) {
        row_pointers[y] = pixel_data.data() + (y * width * 4);
    }
    
    // Write image data
    png_write_image(png_write_ptr, row_pointers.data());
    
    // Finish writing
    png_write_end(png_write_ptr, NULL);
    
    // ==================== OPTIONAL: READ BACK AND VERIFY GETTERS ====================
    
    // Only try to read back if we have enough buffer data
    if (buffer.size > 0) {
        // Reset buffer offset for reading
        buffer.offset = 0;
        
        // Create PNG read structure
        png_structp png_read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                                          NULL,
                                                          png_error_callback,
                                                          png_warning_callback);
        if (png_read_ptr != NULL) {
            png_infop info_read_ptr = png_create_info_struct(png_read_ptr);
            if (info_read_ptr != NULL) {
                // Set up custom IO for reading from memory buffer
                png_set_read_fn(png_read_ptr, &buffer, png_read_from_memory);
                
                // Read PNG info
                png_read_info(png_read_ptr, info_read_ptr);
                
                // Test getter functions for the metadata we just wrote
                double out_white_x, out_white_y, out_red_x, out_red_y, out_green_x, out_green_y, out_blue_x, out_blue_y;
                double out_maxDL, out_minDL;
                
                // Get mDCV metadata
                png_get_mDCV(png_read_ptr, info_read_ptr,
                            &out_white_x, &out_white_y,
                            &out_red_x, &out_red_y,
                            &out_green_x, &out_green_y,
                            &out_blue_x, &out_blue_y,
                            &out_maxDL, &out_minDL);
                
                double out_red_X, out_red_Y, out_red_Z, out_green_X, out_green_Y, out_green_Z, out_blue_X, out_blue_Y, out_blue_Z;
                
                // Get cHRM_XYZ metadata
                png_get_cHRM_XYZ(png_read_ptr, info_read_ptr,
                                &out_red_X, &out_red_Y, &out_red_Z,
                                &out_green_X, &out_green_Y, &out_green_Z,
                                &out_blue_X, &out_blue_Y, &out_blue_Z);
                
                // Test fixed-point getters as well
                png_fixed_point int_out_red_X, int_out_red_Y, int_out_red_Z;
                png_fixed_point int_out_green_X, int_out_green_Y, int_out_green_Z;
                png_fixed_point int_out_blue_X, int_out_blue_Y, int_out_blue_Z;
                
                png_get_cHRM_XYZ_fixed(png_read_ptr, info_read_ptr,
                                      &int_out_red_X, &int_out_red_Y, &int_out_red_Z,
                                      &int_out_green_X, &int_out_green_Y, &int_out_green_Z,
                                      &int_out_blue_X, &int_out_blue_Y, &int_out_blue_Z);
                
                // Also test cLLI getters
                double out_maxCLL, out_maxFALL;
                png_get_cLLI(png_read_ptr, info_read_ptr, &out_maxCLL, &out_maxFALL);
                
                // Clean up read structures
                png_destroy_read_struct(&png_read_ptr, &info_read_ptr, NULL);
            } else {
                png_destroy_read_struct(&png_read_ptr, NULL, NULL);
            }
        }
    }
    
    // ==================== CLEANUP ====================
    
    // Clean up write structures
    png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
    
    // Free memory buffer
    free(buffer.data);
    
    return 0;
}

} // namespace harness_016
