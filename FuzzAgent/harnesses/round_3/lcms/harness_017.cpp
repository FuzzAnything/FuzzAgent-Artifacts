#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_017 {
// Fuzzing harness for LittleCMS (lcms) CGATS/IT8 file handling module (cmscgats.c)
// This harness specifically targets cmsCreateDeviceLinkFromCubeFileTHR with 99 undiscovered branches
// and supporting IT8 APIs as identified in coverage guidance report


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing: some cube file data + parameters
    if (size < 128) {
        return 0;  // Need sufficient data for cube file content
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input: first part for cube file data, rest for fuzzing parameters
    // Reserve at least 64 bytes for parameters (context, transform settings, etc.)
    size_t min_param_size = 64;
    if (size < min_param_size) return 0;
    
    size_t max_cube_size = size - min_param_size;
    size_t cube_size = fdp.ConsumeIntegralInRange<size_t>(64, max_cube_size);
    std::vector<uint8_t> cube_data = fdp.ConsumeBytes<uint8_t>(cube_size);
    
    // Create a temporary file with cube data for cmsIT8LoadFromFile
    // We'll use a temporary file in memory using fmemopen if available, or write to temp file
    // For fuzzing, we'll create a temporary file path and write cube data
    const char* temp_filename = "/tmp/fuzz_cube.cube";
    FILE* temp_file = fopen(temp_filename, "wb");
    if (!temp_file) {
        return 0;  // Cannot create temp file
    }
    
    fwrite(cube_data.data(), 1, cube_data.size(), temp_file);
    fclose(temp_file);
    
    // Step 1: Create context for THR functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        // Clean up temp file
        remove(temp_filename);
        return 0;
    }
    
    // Step 2: Load cube file using IT8 APIs (two approaches)
    cmsHANDLE it8_handle = NULL;
    int load_method = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    if (load_method == 0) {
        // Method 1: Load from file
        it8_handle = cmsIT8LoadFromFile(context, temp_filename);
    } else {
        // Method 2: Allocate and populate with data
        it8_handle = cmsIT8Alloc(context);
        if (it8_handle) {
            // For cube files, we need to set cube-specific properties
            // Generate cube file content from fuzzer input
            std::string cube_content;
            
            // Consume cube parameters from fuzzer input
            std::string title = fdp.ConsumeRandomLengthString(100);
            int lut_size = fdp.ConsumeIntegralInRange<int>(1, 10);
            
            // Generate domain min/max from fuzzer input
            double domain_min[3], domain_max[3];
            for (int i = 0; i < 3; i++) {
                domain_min[i] = fdp.ConsumeFloatingPoint<double>();
                domain_max[i] = fdp.ConsumeFloatingPointInRange<double>(domain_min[i] + 0.1, domain_min[i] + 10.0);
            }
            
            // Build cube content from fuzzed data
            cube_content = "TITLE \"" + title + "\"\n";
            cube_content += "LUT_3D_SIZE " + std::to_string(lut_size) + "\n";
            cube_content += "DOMAIN_MIN " + std::to_string(domain_min[0]) + " " + 
                                           std::to_string(domain_min[1]) + " " + 
                                           std::to_string(domain_min[2]) + "\n";
            cube_content += "DOMAIN_MAX " + std::to_string(domain_max[0]) + " " + 
                                           std::to_string(domain_max[1]) + " " + 
                                           std::to_string(domain_max[2]) + "\n";
            
            // Generate LUT data points from remaining fuzzer input
            int num_points = lut_size * lut_size * lut_size;
            for (int i = 0; i < num_points && fdp.remaining_bytes() >= 12; i++) {
                double r = fdp.ConsumeFloatingPoint<double>();
                double g = fdp.ConsumeFloatingPoint<double>();
                double b = fdp.ConsumeFloatingPoint<double>();
                cube_content += std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + "\n";
            }
            
            // Write this content to temp file for loading
            temp_file = fopen(temp_filename, "wb");
            if (temp_file) {
                fwrite(cube_content.c_str(), 1, cube_content.size(), temp_file);
                fclose(temp_file);
                cmsIT8Free(it8_handle);
                it8_handle = cmsIT8LoadFromFile(context, temp_filename);
            }
        }
    }
    // Step 3: Create device link from cube file
    cmsHPROFILE device_link = NULL;
    if (it8_handle) {
        // For cmsCreateDeviceLinkFromCubeFileTHR, we need a valid cube file
        // Reuse the temp file with proper cube format
        device_link = cmsCreateDeviceLinkFromCubeFileTHR(context, temp_filename);
        // The function may return NULL for invalid cube files - this is expected
    }
    
    // If device link creation failed, try with a simple built-in profile as fallback
    // Only use fallback if we have a valid context
    if (!device_link && context) {
        // Create a simple sRGB profile as fallback for transform testing
        device_link = cmsCreate_sRGBProfileTHR(context);
    }
    // Step 4: Create transform from device link
    cmsHTRANSFORM transform = NULL;
    cmsUInt32Number input_format = TYPE_RGB_8;  // Default format
    cmsUInt32Number output_format = TYPE_RGB_8; // Default format
    
    if (device_link) {
        // Consume parameters for transform creation
        input_format = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
        output_format = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
        cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
        cmsUInt32Number flags = fdp.ConsumeIntegral<cmsUInt32Number>();
        
        // Common format types for RGB transforms
        cmsUInt32Number format_types[] = {
            TYPE_RGB_8, TYPE_RGB_16, TYPE_RGB_DBL, TYPE_RGB_FLT
        };
        
        input_format = format_types[input_format % 4];
        output_format = format_types[output_format % 4];
        
        // Use sRGB as input profile for the transform
        cmsHPROFILE srgb_profile = cmsCreate_sRGBProfileTHR(context);
        if (srgb_profile) {
            transform = cmsCreateTransformTHR(context,
                                             srgb_profile,    // Input profile
                                             input_format,    // Input format
                                             device_link,     // Output profile (device link)
                                             output_format,   // Output format  
                                             intent,          // Rendering intent
                                             flags);          // Flags
            
            cmsCloseProfile(srgb_profile);
        }
    }
    
    // Step 5: Execute transform if created
    if (transform) {
        // Consume parameters for transform execution
        size_t num_pixels = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        
        // Calculate buffer size based on input and output formats
        // Helper macros similar to lcms2.h
        auto T_CHANNELS = [](cmsUInt32Number c) -> cmsUInt32Number {
            return ((c >> 3) & 15);
        };
        auto T_BYTES = [](cmsUInt32Number b) -> cmsUInt32Number {
            return (b & 7);
        };
        auto T_FLOAT = [](cmsUInt32Number a) -> cmsUInt32Number {
            return ((a >> 22) & 1);
        };
        
        // Calculate bytes per pixel for a format
        auto get_bytes_per_pixel = [&](cmsUInt32Number format) -> size_t {
            cmsUInt32Number channels = T_CHANNELS(format);
            cmsUInt32Number bytes_per_channel = T_BYTES(format);
            
            // Special case: double precision floating point (BYTES_SH(0) with FLOAT_SH(1))
            if (T_FLOAT(format) && bytes_per_channel == 0) {
                bytes_per_channel = sizeof(cmsFloat64Number); // 8 bytes for double
            } else if (bytes_per_channel == 0) {
                // Should not happen for valid formats, but fallback
                bytes_per_channel = 1;
            }
            
            return channels * bytes_per_channel;
        };
        
        size_t input_buffer_size = num_pixels * get_bytes_per_pixel(input_format);
        size_t output_buffer_size = num_pixels * get_bytes_per_pixel(output_format);
        // Allocate input and output buffers
        std::vector<uint8_t> input_buffer(input_buffer_size);
        std::vector<uint8_t> output_buffer(output_buffer_size);
        
        // Fill input buffer with fuzzed data
        if (fdp.remaining_bytes() >= input_buffer_size) {
            std::vector<uint8_t> pixel_data = fdp.ConsumeBytes<uint8_t>(input_buffer_size);
            memcpy(input_buffer.data(), pixel_data.data(), input_buffer_size);
        } else {
            // Fill with pattern if not enough data
            for (size_t i = 0; i < input_buffer_size; i++) {
                input_buffer[i] = (i % 256);
            }
        }
        
        // Execute transform
        cmsDoTransform(transform, input_buffer.data(), output_buffer.data(), num_pixels);
    }
    
    // Step 6: Optional save IT8 file
    if (it8_handle && fdp.ConsumeBool()) {
        const char* save_filename = "/tmp/fuzz_save.cube";
        cmsIT8SaveToFile(it8_handle, save_filename);
        remove(save_filename);
    }
    
    // Step 7: Cleanup
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (device_link) {
        cmsCloseProfile(device_link);
    }
    
    if (it8_handle) {
        cmsIT8Free(it8_handle);
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    // Clean up temp file
    remove(temp_filename);
    
    return 0;
}

} // namespace harness_017
