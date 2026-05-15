#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "stb_image.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_003 {
/*
 * Fuzzing harness for tinygltf library - targeting STB image file-based loading APIs
 * This harness tests file-based STB image functions to increase coverage of stb_image.h
 * which currently has only 0.45% region coverage and 0.10% branch coverage.
 * 
 * This harness complements harness_002 (memory-based) by focusing on file I/O operations.
 * 
 * Targets:
 * - stbi_load (file-based 8-bit loading)
 * - stbi_load_16 (file-based 16-bit loading)
 * - stbi_loadf (file-based float/HDR loading)
 * - stbi_load_from_file (FILE* based 8-bit loading)
 * - stbi_load_from_file_16 (FILE* based 16-bit loading)
 * - stbi_loadf_from_file (FILE* based float/HDR loading)
 * - stbi_info (file info query)
 * - stbi_info_from_file (FILE* info query)
 * - stbi_is_hdr (HDR detection)
 * - stbi_is_hdr_from_file (FILE* HDR detection)
 * - stbi_is_16_bit (16-bit detection)
 * - stbi_is_16_bit_from_file (FILE* 16-bit detection)
 * - stbi_image_free
 * - stbi_failure_reason
 * 
 * Strategy: Creates temporary files from fuzzer input and tests file-based APIs.
 * Uses FuzzedDataProvider for structured input splitting.
 */


// Include STB image implementation directly
#define STB_IMAGE_IMPLEMENTATION


// Helper function to create a temporary file with unique name
static std::string create_temp_filename() {
    char template_name[] = "/tmp/stb_fuzz_XXXXXX";
    int fd = mkstemp(template_name);
    if (fd != -1) {
        close(fd);
        return std::string(template_name);
    }
    return "";
}

// Helper function to write data to a file
static bool write_to_file(const std::string& filename, const uint8_t* data, size_t size) {
    FILE* f = fopen(filename.c_str(), "wb");
    if (!f) {
        return false;
    }
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return written == size;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Need minimum data for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration options from input
    uint8_t config_byte = fdp.ConsumeIntegral<uint8_t>();
    
    // Set STB configuration flags based on input
    stbi_set_flip_vertically_on_load((config_byte & 0x01) != 0);
    stbi_set_unpremultiply_on_load((config_byte & 0x02) != 0);
    stbi_convert_iphone_png_to_rgb((config_byte & 0x04) != 0);
    
    // Consume gamma/scale values for HDR/LDR conversion
    float gamma_val = fdp.ConsumeFloatingPointInRange<float>(0.1f, 10.0f);
    float scale_val = fdp.ConsumeFloatingPointInRange<float>(0.1f, 10.0f);
    stbi_hdr_to_ldr_gamma(gamma_val);
    stbi_hdr_to_ldr_scale(scale_val);
    stbi_ldr_to_hdr_gamma(gamma_val);
    stbi_ldr_to_hdr_scale(scale_val);
    
    // Get image data from fuzzer input
    std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (image_data.empty() || image_data.size() < 16) {
        return 0;  // Need some image data
    }
    
    // Create temporary file with image data
    std::string temp_file = create_temp_filename();
    if (temp_file.empty()) {
        return 0;  // Failed to create temp file
    }
    
    if (!write_to_file(temp_file, image_data.data(), image_data.size())) {
        unlink(temp_file.c_str());  // Clean up
        return 0;
    }
    
    // Consume operation selector - target different file-based APIs
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    int width = 0, height = 0, channels = 0;
    unsigned char* loaded_data = nullptr;
    unsigned short* loaded_data_16 = nullptr;
    float* loaded_data_float = nullptr;
    
    // Also test with FILE* APIs
    FILE* file_handle = fopen(temp_file.c_str(), "rb");
    if (!file_handle) {
        unlink(temp_file.c_str());
        return 0;
    }
    
    switch (operation) {
        case 0: {
            // Test stbi_info (file-based info query)
            bool info_success = stbi_info(temp_file.c_str(), &width, &height, &channels);
            (void)info_success;
            
            // Test stbi_is_hdr (file-based HDR detection)
            bool is_hdr = stbi_is_hdr(temp_file.c_str());
            (void)is_hdr;
            
            // Test stbi_is_16_bit (file-based 16-bit detection)
            bool is_16bit = stbi_is_16_bit(temp_file.c_str());
            (void)is_16bit;
            break;
        }
        
        case 1: {
            // Test stbi_load (file-based 8-bit loading)
            int desired_channels = 0;
            uint8_t channel_selector = (config_byte >> 3) & 0x03;
            switch (channel_selector) {
                case 0: desired_channels = 0; break;  // original channels
                case 1: desired_channels = 1; break;  // force greyscale
                case 2: desired_channels = 3; break;  // force RGB
                case 3: desired_channels = 4; break;  // force RGBA
            }
            
            loaded_data = stbi_load(temp_file.c_str(), &width, &height, &channels, desired_channels);
            
            if (loaded_data) {
                // Access some pixel data to exercise code paths
                if (width > 0 && height > 0 && channels > 0) {
                    int sample_count = std::min(10, width * height);
                    for (int i = 0; i < sample_count && i < width * height; i++) {
                        volatile uint8_t pixel_value = loaded_data[i * channels];
                        (void)pixel_value;
                    }
                }
                
                stbi_image_free(loaded_data);
            } else {
                // Test failure reason API
                const char* failure_reason = stbi_failure_reason();
                (void)failure_reason;
            }
            break;
        }
        
        case 2: {
            // Test stbi_load_16 (file-based 16-bit loading)
            int desired_channels = 0;
            uint8_t channel_selector = (config_byte >> 5) & 0x03;
            switch (channel_selector) {
                case 0: desired_channels = 0; break;  // original channels
                case 1: desired_channels = 1; break;  // force greyscale
                case 2: desired_channels = 3; break;  // force RGB
                case 3: desired_channels = 4; break;  // force RGBA
            }
            
            loaded_data_16 = stbi_load_16(temp_file.c_str(), &width, &height, &channels, desired_channels);
            
            if (loaded_data_16) {
                // Access some pixel data to exercise code paths
                if (width > 0 && height > 0 && channels > 0) {
                    int sample_count = std::min(10, width * height);
                    for (int i = 0; i < sample_count && i < width * height; i++) {
                        volatile uint16_t pixel_value = loaded_data_16[i * channels];
                        (void)pixel_value;
                    }
                }
                
                stbi_image_free(loaded_data_16);
            }
            break;
        }
        
        case 3: {
            // Test stbi_loadf (file-based float/HDR loading)
            int desired_channels = 0;
            uint8_t channel_selector = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (channel_selector) {
                case 0: desired_channels = 0; break;  // original channels
                case 1: desired_channels = 1; break;  // force greyscale
                case 2: desired_channels = 3; break;  // force RGB
                case 3: desired_channels = 4; break;  // force RGBA
            }
            
            loaded_data_float = stbi_loadf(temp_file.c_str(), &width, &height, &channels, desired_channels);
            
            if (loaded_data_float) {
                // Access some pixel data to exercise code paths
                if (width > 0 && height > 0 && channels > 0) {
                    int sample_count = std::min(10, width * height);
                    for (int i = 0; i < sample_count && i < width * height; i++) {
                        volatile float pixel_value = loaded_data_float[i * channels];
                        (void)pixel_value;
                    }
                }
                
                stbi_image_free(loaded_data_float);
            }
            break;
        }
        
        case 4: {
            // Test stbi_info_from_file (FILE* info query)
            bool info_success = stbi_info_from_file(file_handle, &width, &height, &channels);
            (void)info_success;
            
            // Reset file pointer for next operation
            fseek(file_handle, 0, SEEK_SET);
            break;
        }
        
        case 5: {
            // Test stbi_load_from_file (FILE* based 8-bit loading)
            int desired_channels = 0;
            uint8_t channel_selector = (config_byte >> 3) & 0x03;
            switch (channel_selector) {
                case 0: desired_channels = 0; break;  // original channels
                case 1: desired_channels = 1; break;  // force greyscale
                case 2: desired_channels = 3; break;  // force RGB
                case 3: desired_channels = 4; break;  // force RGBA
            }
            
            loaded_data = stbi_load_from_file(file_handle, &width, &height, &channels, desired_channels);
            
            if (loaded_data) {
                // Access some pixel data to exercise code paths
                if (width > 0 && height > 0 && channels > 0) {
                    int sample_count = std::min(10, width * height);
                    for (int i = 0; i < sample_count && i < width * height; i++) {
                        volatile uint8_t pixel_value = loaded_data[i * channels];
                        (void)pixel_value;
                    }
                }
                
                stbi_image_free(loaded_data);
            }
            break;
        }
        
        case 6: {
            // Test stbi_load_from_file_16 (FILE* based 16-bit loading)
            int desired_channels = 0;
            uint8_t channel_selector = (config_byte >> 5) & 0x03;
            switch (channel_selector) {
                case 0: desired_channels = 0; break;  // original channels
                case 1: desired_channels = 1; break;  // force greyscale
                case 2: desired_channels = 3; break;  // force RGB
                case 3: desired_channels = 4; break;  // force RGBA
            }
            
            loaded_data_16 = stbi_load_from_file_16(file_handle, &width, &height, &channels, desired_channels);
            
            if (loaded_data_16) {
                // Access some pixel data to exercise code paths
                if (width > 0 && height > 0 && channels > 0) {
                    int sample_count = std::min(10, width * height);
                    for (int i = 0; i < sample_count && i < width * height; i++) {
                        volatile uint16_t pixel_value = loaded_data_16[i * channels];
                        (void)pixel_value;
                    }
                }
                
                stbi_image_free(loaded_data_16);
            }
            break;
        }
        
        case 7: {
            // Test stbi_loadf_from_file (FILE* based float/HDR loading)
            int desired_channels = 0;
            uint8_t channel_selector = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (channel_selector) {
                case 0: desired_channels = 0; break;  // original channels
                case 1: desired_channels = 1; break;  // force greyscale
                case 2: desired_channels = 3; break;  // force RGB
                case 3: desired_channels = 4; break;  // force RGBA
            }
            
            loaded_data_float = stbi_loadf_from_file(file_handle, &width, &height, &channels, desired_channels);
            
            if (loaded_data_float) {
                // Access some pixel data to exercise code paths
                if (width > 0 && height > 0 && channels > 0) {
                    int sample_count = std::min(10, width * height);
                    for (int i = 0; i < sample_count && i < width * height; i++) {
                        volatile float pixel_value = loaded_data_float[i * channels];
                        (void)pixel_value;
                    }
                }
                
                stbi_image_free(loaded_data_float);
            }
            break;
        }
        
        case 8: {
            // Test stbi_is_hdr_from_file (FILE* HDR detection)
            bool is_hdr = stbi_is_hdr_from_file(file_handle);
            (void)is_hdr;
            
            // Reset file pointer
            fseek(file_handle, 0, SEEK_SET);
            
            // Test stbi_is_16_bit_from_file (FILE* 16-bit detection)
            bool is_16bit = stbi_is_16_bit_from_file(file_handle);
            (void)is_16bit;
            break;
        }
        
        case 9: {
            // Test combination of APIs
            // First get info
            bool info_success = stbi_info(temp_file.c_str(), &width, &height, &channels);
            (void)info_success;
            
            // Then try to load based on image type detection
            if (stbi_is_hdr(temp_file.c_str())) {
                loaded_data_float = stbi_loadf(temp_file.c_str(), &width, &height, &channels, 0);
                if (loaded_data_float) {
                    stbi_image_free(loaded_data_float);
                }
            } else if (stbi_is_16_bit(temp_file.c_str())) {
                loaded_data_16 = stbi_load_16(temp_file.c_str(), &width, &height, &channels, 0);
                if (loaded_data_16) {
                    stbi_image_free(loaded_data_16);
                }
            } else {
                loaded_data = stbi_load(temp_file.c_str(), &width, &height, &channels, 0);
                if (loaded_data) {
                    stbi_image_free(loaded_data);
                }
            }
            break;
        }
    }
    
    // Clean up
    fclose(file_handle);
    unlink(temp_file.c_str());
    
    return 0;
}

} // namespace harness_003
