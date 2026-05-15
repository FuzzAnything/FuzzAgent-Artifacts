#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_018 {

// Custom write callback structure for png_set_write_fn
struct png_write_memory_data {
    uint8_t* buffer;
    size_t size;
    size_t offset;
    size_t capacity;
};

// Write callback function for memory-based output
static void png_write_memory_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_write_memory_data* write_data = (png_write_memory_data*)png_get_io_ptr(png_ptr);
    
    // Resize buffer if needed
    if (write_data->offset + length > write_data->capacity) {
        size_t new_capacity = write_data->capacity * 2;
        if (new_capacity < write_data->offset + length) {
            new_capacity = write_data->offset + length;
        }
        uint8_t* new_buffer = (uint8_t*)realloc(write_data->buffer, new_capacity);
        if (!new_buffer) {
            png_error(png_ptr, "Memory allocation failed in write callback");
            return;
        }
        write_data->buffer = new_buffer;
        write_data->capacity = new_capacity;
    }
    
    memcpy(write_data->buffer + write_data->offset, data, length);
    write_data->offset += length;
    write_data->size = write_data->offset;
}

// Flush callback (required but does nothing for memory writing)
static void png_write_memory_flush(png_structp png_ptr) {
    // Nothing to flush for memory buffer
    (void)png_ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for color management testing
    if (size < 200) {
        return 0;  // Minimum size for meaningful color parameter testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ====================================================
    // STEP 1: CREATE PNG WRITE STRUCTURE
    // ====================================================
    png_struct *write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                       nullptr, nullptr, nullptr);
    if (write_png_ptr == nullptr) {
        return 0;
    }
    
    // ====================================================
    // STEP 2: CREATE PNG INFO STRUCTURE
    // ====================================================
    png_info *write_info_ptr = png_create_info_struct(write_png_ptr);
    if (write_info_ptr == nullptr) {
        png_destroy_write_struct(&write_png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling for write
    if (setjmp(png_jmpbuf(write_png_ptr))) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    // ====================================================
    // STEP 3: SETUP MEMORY-BASED WRITING
    // ====================================================
    png_write_memory_data write_data;
    write_data.buffer = nullptr;
    write_data.size = 0;
    write_data.offset = 0;
    write_data.capacity = 8192;
    
    write_data.buffer = (uint8_t*)malloc(write_data.capacity);
    if (write_data.buffer == nullptr) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    png_set_write_fn(write_png_ptr, &write_data, png_write_memory_callback, png_write_memory_flush);
    
    // ====================================================
    // STEP 4: SET BASIC PNG HEADER FOR TESTING
    // ====================================================
    // Set basic PNG parameters so we can write color metadata
    png_set_IHDR(write_png_ptr, write_info_ptr,
                32, 32, // Width, height
                8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_BASE,
                PNG_FILTER_TYPE_BASE);
    
    // ====================================================
    // STEP 5: CONSUME FUZZER INPUT FOR COLOR MANAGEMENT PARAMETERS
    // ====================================================
    
    // Consume test scenario (which color management APIs to test)
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 3);
    bool use_fixed_point = fdp.ConsumeBool();
    
    // Consume chromaticity values (0.0 to 1.3107 valid range for PNG color values)
    double white_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double white_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double red_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double red_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double green_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double green_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double blue_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double blue_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    
    // Consume XYZ chromaticity values (for cHRM_XYZ)
    double red_X = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double red_Y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double red_Z = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double green_X = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double green_Y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double green_Z = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double blue_X = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double blue_Y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    double blue_Z = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.5);
    
    // Consume luminance values (for mDCV and cLLI)
    double max_luminance = fdp.ConsumeFloatingPointInRange<double>(0.0, 10000.0);
    double min_luminance = fdp.ConsumeFloatingPointInRange<double>(0.0, 10000.0);
    double content_light_level = fdp.ConsumeFloatingPointInRange<double>(0.0, 10000.0);
    double frame_avg_light_level = fdp.ConsumeFloatingPointInRange<double>(0.0, 10000.0);
    
    // For fixed-point versions, convert and constrain values
    const double MAX_FIXED_VALUE = 21474.83647;  // PNG_FP_MAX / 100000.0
    
    png_fixed_point int_white_x = (png_fixed_point)(white_x * 100000.0);
    png_fixed_point int_white_y = (png_fixed_point)(white_y * 100000.0);
    png_fixed_point int_red_x = (png_fixed_point)(red_x * 100000.0);
    png_fixed_point int_red_y = (png_fixed_point)(red_y * 100000.0);
    png_fixed_point int_green_x = (png_fixed_point)(green_x * 100000.0);
    png_fixed_point int_green_y = (png_fixed_point)(green_y * 100000.0);
    png_fixed_point int_blue_x = (png_fixed_point)(blue_x * 100000.0);
    png_fixed_point int_blue_y = (png_fixed_point)(blue_y * 100000.0);
    
    png_fixed_point int_red_X = (png_fixed_point)(red_X * 100000.0);
    png_fixed_point int_red_Y = (png_fixed_point)(red_Y * 100000.0);
    png_fixed_point int_red_Z = (png_fixed_point)(red_Z * 100000.0);
    png_fixed_point int_green_X = (png_fixed_point)(green_X * 100000.0);
    png_fixed_point int_green_Y = (png_fixed_point)(green_Y * 100000.0);
    png_fixed_point int_green_Z = (png_fixed_point)(green_Z * 100000.0);
    png_fixed_point int_blue_X = (png_fixed_point)(blue_X * 100000.0);
    png_fixed_point int_blue_Y = (png_fixed_point)(blue_Y * 100000.0);
    png_fixed_point int_blue_Z = (png_fixed_point)(blue_Z * 100000.0);
    
    // For mDCV_fixed: luminance values are scaled by 10,000, not 100,000
    png_uint_32 int_max_luminance = (png_uint_32)(max_luminance * 10000.0);
    png_uint_32 int_min_luminance = (png_uint_32)(min_luminance * 10000.0);
    
    // For cLLI_fixed: light levels are scaled by 10,000
    png_uint_32 int_content_light_level = (png_uint_32)(content_light_level * 10000.0);
    png_uint_32 int_frame_avg_light_level = (png_uint_32)(frame_avg_light_level * 10000.0);
    
    // ====================================================
    // STEP 6: TEST PNG COLOR MANAGEMENT SETTER APIS
    // ====================================================
    
    // Test different color management scenarios based on fuzzer input
    switch (test_scenario) {
        case 0:
            // Test mastering display color volume (mDCV) APIs
            if (use_fixed_point) {
                // Fixed-point version
                png_set_mDCV_fixed(write_png_ptr, write_info_ptr,
                                 int_white_x, int_white_y,
                                 int_red_x, int_red_y,
                                 int_green_x, int_green_y,
                                 int_blue_x, int_blue_y,
                                 int_max_luminance, int_min_luminance);
            } else {
                // Floating-point version
                png_set_mDCV(write_png_ptr, write_info_ptr,
                           white_x, white_y,
                           red_x, red_y,
                           green_x, green_y,
                           blue_x, blue_y,
                           max_luminance, min_luminance);
            }
            break;
            
        case 1:
            // Test content light level information (cLLI) APIs
            if (use_fixed_point) {
                // Fixed-point version (scaled by 10,000)
                png_set_cLLI_fixed(write_png_ptr, write_info_ptr,
                                 int_content_light_level, int_frame_avg_light_level);
            } else {
                // Floating-point version
                png_set_cLLI(write_png_ptr, write_info_ptr,
                           content_light_level, frame_avg_light_level);
            }
            break;
            
        case 2:
            // Test XYZ chromaticity (cHRM_XYZ) APIs
            if (use_fixed_point) {
                // Fixed-point version
                png_set_cHRM_XYZ_fixed(write_png_ptr, write_info_ptr,
                                     int_red_X, int_red_Y, int_red_Z,
                                     int_green_X, int_green_Y, int_green_Z,
                                     int_blue_X, int_blue_Y, int_blue_Z);
            } else {
                // Floating-point version
                png_set_cHRM_XYZ(write_png_ptr, write_info_ptr,
                               red_X, red_Y, red_Z,
                               green_X, green_Y, green_Z,
                               blue_X, blue_Y, blue_Z);
            }
            break;
            
        case 3:
            // Test combination of multiple color management APIs
            // First set standard cHRM (chromaticity)
            if (use_fixed_point) {
                png_set_cHRM_fixed(write_png_ptr, write_info_ptr,
                                 int_white_x, int_white_y,
                                 int_red_x, int_red_y,
                                 int_green_x, int_green_y,
                                 int_blue_x, int_blue_y);
                
                // Then set XYZ chromaticity
                png_set_cHRM_XYZ_fixed(write_png_ptr, write_info_ptr,
                                     int_red_X, int_red_Y, int_red_Z,
                                     int_green_X, int_green_Y, int_green_Z,
                                     int_blue_X, int_blue_Y, int_blue_Z);
            } else {
                png_set_cHRM(write_png_ptr, write_info_ptr,
                           white_x, white_y,
                           red_x, red_y,
                           green_x, green_y,
                           blue_x, blue_y);
                
                png_set_cHRM_XYZ(write_png_ptr, write_info_ptr,
                               red_X, red_Y, red_Z,
                               green_X, green_Y, green_Z,
                               blue_X, blue_Y, blue_Z);
            }
            
            // Also set gamma for complete color management
            double gamma = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
            if (use_fixed_point) {
                png_fixed_point int_gamma = (png_fixed_point)(gamma * 100000.0);
                png_set_gAMA_fixed(write_png_ptr, write_info_ptr, int_gamma);
            } else {
                png_set_gAMA(write_png_ptr, write_info_ptr, gamma);
            }
            break;
    }
    
    // ====================================================
    // STEP 7: WRITE PNG TO TEST COLOR METADATA PERSISTENCE
    // ====================================================
    
    // Write PNG info (this will include our color metadata)
    png_write_info(write_png_ptr, write_info_ptr);
    
    // Create dummy image data (32x32 RGB)
    std::vector<png_byte> row(32 * 3); // 32 pixels * 3 bytes per pixel (RGB)
    for (size_t i = 0; i < row.size(); i++) {
        row[i] = (png_byte)(i % 256);
    }
    
    // Write dummy rows
    for (int y = 0; y < 32; y++) {
        png_write_row(write_png_ptr, row.data());
    }
    
    // End write
    png_write_end(write_png_ptr, write_info_ptr);
    
    // ====================================================
    // STEP 8: CLEANUP
    // ====================================================
    
    // Free memory buffer
    free(write_data.buffer);
    
    // Destroy PNG structures
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    
    return 0;
}

} // namespace harness_018
