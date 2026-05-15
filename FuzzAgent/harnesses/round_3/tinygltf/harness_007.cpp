#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

namespace harness_007 {
/*
 * Fuzzing harness for tinygltf library targeting direct STB image loading and writing operations
 * Goal: Exercise stb_image.h and stb_image_write.h APIs directly without GLTF integration
 * Targets: stbi_load (12 branches, 9 undiscovered), stbi_load_16 (12 branches, 9 undiscovered),
 *          stbi_loadf (10 branches, 7 undiscovered), stbi_write_png (4 branches, 4 undiscovered),
 *          stbi_write_hdr (2 branches, 2 undiscovered), stbi_write_jpg (2 branches, 2 undiscovered),
 *          stbi_write_tga (2 branches, 2 undiscovered), stbi_write_bmp (2 branches, 2 undiscovered)
 * Strategy: Direct API calls to image loading/writing functions with fuzzer-provided input
 *           Complete image processing pipeline: Validate → Load → Write → Cleanup
 * Differentiation from previous harnesses:
 *   - harness_006: Image pipeline via GLTF model integration
 *   - harness_007: Direct STB API calls without GLTF abstraction
 */



// Include STB headers (implementation already built via tinygltf)
// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION

// Temporary file handling utilities

// Create a temporary file with random name
class TempFile {
public:
    TempFile(const std::string& prefix = "fuzz_img_") {
        char template_name[] = "/tmp/fuzz_img_XXXXXX";
        fd = mkstemp(template_name);
        if (fd != -1) {
            filename = template_name;
        }
    }
    
    ~TempFile() {
        if (fd != -1) {
            close(fd);
            unlink(filename.c_str());
        }
    }
    
    const std::string& get_filename() const { return filename; }
    bool is_valid() const { return fd != -1; }
    
private:
    int fd;
    std::string filename;
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input: part for image data, part for parameters
    // Reserve some bytes for image data (actual image bytes)
    size_t image_data_size = fdp.ConsumeIntegralInRange<size_t>(50, std::min(size - 50, size_t(4096)));
    std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_data_size);
    
    if (image_data.empty()) {
        return 0;
    }
    
    // Consume parameters for API calls
    int desired_channels = fdp.ConsumeIntegralInRange<int>(1, 4);
    int write_quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    int operation = fdp.ConsumeIntegralInRange<int>(0, 7); // Which API to test
    
    // Step 1: Validate input using stbi_info_from_memory
    int width = 0, height = 0, comp = 0;
    int info_result = stbi_info_from_memory(image_data.data(), image_data.size(), 
                                           &width, &height, &comp);
    
    // info_result is 0 if image data is invalid/unsupported format
    // We'll proceed with loading attempts even if info fails, as stbi_load
    // functions handle invalid data internally
    
    // Step 2: Load image data using different loaders based on operation
    void* loaded_data = nullptr;
    int actual_width = 0, actual_height = 0, actual_channels = 0;
    
    switch (operation % 3) {
        case 0: {
            // Test stbi_load_from_memory (8-bit unsigned char)
            unsigned char* uc_data = stbi_load_from_memory(
                image_data.data(), image_data.size(),
                &actual_width, &actual_height, &actual_channels, desired_channels);
            loaded_data = uc_data;
            break;
        }
        case 1: {
            // Test stbi_load_16_from_memory (16-bit unsigned short)
            unsigned short* us_data = stbi_load_16_from_memory(
                image_data.data(), image_data.size(),
                &actual_width, &actual_height, &actual_channels, desired_channels);
            loaded_data = us_data;
            break;
        }
        case 2: {
            // Test stbi_loadf_from_memory (32-bit float)
            float* float_data = stbi_loadf_from_memory(
                image_data.data(), image_data.size(),
                &actual_width, &actual_height, &actual_channels, desired_channels);
            loaded_data = float_data;
            break;
        }
    }
    
    // If loading failed, we can't proceed to writing
    if (!loaded_data || actual_width <= 0 || actual_height <= 0 || actual_channels <= 0) {
        if (loaded_data) {
            stbi_image_free(loaded_data);
        }
        return 0;
    }
    
    // Calculate total pixels for bounds checking
    size_t total_pixels = actual_width * actual_height;
    if (total_pixels == 0) {
        stbi_image_free(loaded_data);
        return 0;
    }
    
    // Step 3: Write processed image using different write functions
    // Create temporary file for writing output
    TempFile temp_file;
    if (!temp_file.is_valid()) {
        stbi_image_free(loaded_data);
        return 0;
    }
    
    int write_result = 0;
    const std::string& filename = temp_file.get_filename();
    
    switch (operation % 7) {
        case 0: {
            // Write PNG (supports stride parameter)
            int stride = actual_width * actual_channels * (operation % 3 == 2 ? sizeof(float) : 
                          (operation % 3 == 1 ? sizeof(unsigned short) : sizeof(unsigned char)));
            write_result = stbi_write_png(filename.c_str(), actual_width, actual_height, 
                                         actual_channels, loaded_data, stride);
            break;
        }
        case 1: {
            // Write JPEG (requires 8-bit data)
            if (operation % 3 == 0) { // Only for 8-bit data
                write_result = stbi_write_jpg(filename.c_str(), actual_width, actual_height,
                                             actual_channels, loaded_data, write_quality);
            } else {
                // Convert or skip for other data types
                write_result = 0;
            }
            break;
        }
        case 2: {
            // Write BMP (requires 8-bit data)
            if (operation % 3 == 0) { // Only for 8-bit data
                write_result = stbi_write_bmp(filename.c_str(), actual_width, actual_height,
                                             actual_channels, loaded_data);
            } else {
                write_result = 0;
            }
            break;
        }
        case 3: {
            // Write TGA (requires 8-bit data)
            if (operation % 3 == 0) { // Only for 8-bit data
                write_result = stbi_write_tga(filename.c_str(), actual_width, actual_height,
                                             actual_channels, loaded_data);
            } else {
                write_result = 0;
            }
            break;
        }
        case 4: {
            // Write HDR (requires float data)
            if (operation % 3 == 2) { // Only for float data
                write_result = stbi_write_hdr(filename.c_str(), actual_width, actual_height,
                                             actual_channels, (const float*)loaded_data);
            } else {
                write_result = 0;
            }
            break;
        }
        case 5: {
            // Test with flipped vertical orientation
            stbi_flip_vertically_on_write(1);
            if (operation % 3 == 0) {
                write_result = stbi_write_png(filename.c_str(), actual_width, actual_height,
                                             actual_channels, loaded_data, 0);
            }
            stbi_flip_vertically_on_write(0);
            break;
        }
        case 6: {
            // Test different PNG compression level
            int old_level = stbi_write_png_compression_level;
            stbi_write_png_compression_level = write_quality % 10; // 0-9
            if (operation % 3 == 0) {
                write_result = stbi_write_png(filename.c_str(), actual_width, actual_height,
                                             actual_channels, loaded_data, 0);
            }
            stbi_write_png_compression_level = old_level;
            break;
        }
    }
    
    // Step 4: Clean up
    stbi_image_free(loaded_data);
    
    // Optional: Test file-based loading APIs with remaining input
    if (fdp.remaining_bytes() > 100) {
        // Create another temp file with remaining data
        TempFile temp_file2;
        if (temp_file2.is_valid()) {
            FILE* f = fopen(temp_file2.get_filename().c_str(), "wb");
            if (f) {
                std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
                fwrite(remaining_data.data(), 1, remaining_data.size(), f);
                fclose(f);
                
                // Try file-based loading
                int file_width = 0, file_height = 0, file_channels = 0;
                if (operation % 2 == 0) {
                    unsigned char* file_data = stbi_load(temp_file2.get_filename().c_str(),
                                                         &file_width, &file_height, 
                                                         &file_channels, desired_channels);
                    if (file_data) {
                        stbi_image_free(file_data);
                    }
                } else {
                    float* file_data = stbi_loadf(temp_file2.get_filename().c_str(),
                                                 &file_width, &file_height,
                                                 &file_channels, desired_channels);
                    if (file_data) {
                        stbi_image_free(file_data);
                    }
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_007
