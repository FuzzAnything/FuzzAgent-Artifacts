#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "stb_image.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_002 {
/*
 * Fuzzing harness for tinygltf library - targeting STB image loading APIs
 * This harness directly tests STB image functions to increase coverage of stb_image.h
 * which currently has only 0.63% line coverage.
 * 
 * Targets:
 * - stbi_load_from_memory
 * - stbi_load_16_from_memory  
 * - stbi_loadf_from_memory (float/HDR loading)
 * - stbi_info_from_memory
 * - stbi_is_16_bit_from_memory
 * - stbi_is_hdr_from_memory
 * - stbi_image_free
 * - stbi_set_flip_vertically_on_load
 * - stbi_set_unpremultiply_on_load
 * - stbi_convert_iphone_png_to_rgb
 * - stbi_hdr_to_ldr_gamma/scale
 * - stbi_ldr_to_hdr_gamma/scale
 * - stbi_failure_reason
 * 
 * Uses FuzzedDataProvider for structured input splitting
 */


// Include STB image implementation directly
#define STB_IMAGE_IMPLEMENTATION


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
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
    
    // Consume operation selector - expanded to 8 operations
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Get remaining data as image payload
    std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (image_data.empty()) {
        return 0;
    }
    
    int width = 0, height = 0, channels = 0;
    unsigned char* loaded_data = nullptr;
    unsigned short* loaded_data_16 = nullptr;
    float* loaded_data_float = nullptr;
    
    switch (operation) {
        case 0: {
            // Test stbi_info_from_memory - just get image info
            bool info_success = stbi_info_from_memory(image_data.data(), 
                                                     static_cast<int>(image_data.size()),
                                                     &width, &height, &channels);
            (void)info_success; // Result doesn't matter for fuzzing
            
            // Also test stbi_is_16_bit_from_memory
            bool is_16bit = stbi_is_16_bit_from_memory(image_data.data(),
                                                      static_cast<int>(image_data.size()));
            (void)is_16bit;
            break;
        }
        
        case 1: {
            // Test stbi_load_from_memory - 8-bit loading
            // Consume desired channels from remaining bits in config_byte
            int desired_channels = 0;
            uint8_t channel_selector = (config_byte >> 3) & 0x03;
            switch (channel_selector) {
                case 0: desired_channels = 0; break;  // original channels
                case 1: desired_channels = 1; break;  // force greyscale
                case 2: desired_channels = 3; break;  // force RGB
                case 3: desired_channels = 4; break;  // force RGBA
            }
            
            loaded_data = stbi_load_from_memory(image_data.data(),
                                               static_cast<int>(image_data.size()),
                                               &width, &height, &channels,
                                               desired_channels);
            
            if (loaded_data) {
                // Test accessing some pixel data
                if (width > 0 && height > 0 && channels > 0) {
                    // Access a few pixels to exercise code paths
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
        
        case 2: {
            // Test stbi_load_16_from_memory - 16-bit loading
            // Consume desired channels from remaining bits in config_byte
            int desired_channels = 0;
            uint8_t channel_selector = (config_byte >> 5) & 0x03;
            switch (channel_selector) {
                case 0: desired_channels = 0; break;  // original channels
                case 1: desired_channels = 1; break;  // force greyscale
                case 2: desired_channels = 3; break;  // force RGB
                case 3: desired_channels = 4; break;  // force RGBA
            }
            
            loaded_data_16 = stbi_load_16_from_memory(image_data.data(),
                                                     static_cast<int>(image_data.size()),
                                                     &width, &height, &channels,
                                                     desired_channels);
            
            if (loaded_data_16) {
                // Test accessing some pixel data
                if (width > 0 && height > 0 && channels > 0) {
                    // Access a few pixels to exercise code paths
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
            // Test stbi_loadf_from_memory - float/HDR loading
            // Consume desired channels from remaining bits
            int desired_channels = 0;
            uint8_t channel_selector = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (channel_selector) {
                case 0: desired_channels = 0; break;  // original channels
                case 1: desired_channels = 1; break;  // force greyscale
                case 2: desired_channels = 3; break;  // force RGB
                case 3: desired_channels = 4; break;  // force RGBA
            }
            
            loaded_data_float = stbi_loadf_from_memory(image_data.data(),
                                                      static_cast<int>(image_data.size()),
                                                      &width, &height, &channels,
                                                      desired_channels);
            
            if (loaded_data_float) {
                // Test accessing some pixel data
                if (width > 0 && height > 0 && channels > 0) {
                    // Access a few pixels to exercise code paths
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
            // Test stbi_is_hdr_from_memory
            bool is_hdr = stbi_is_hdr_from_memory(image_data.data(),
                                                 static_cast<int>(image_data.size()));
            (void)is_hdr;
            
            // Also test failure reason API
            const char* failure_reason = stbi_failure_reason();
            (void)failure_reason;
            break;
        }
        
        case 5: {
            // Test combination of operations
            // First get info
            (void)stbi_info_from_memory(image_data.data(),
                                       static_cast<int>(image_data.size()),
                                       &width, &height, &channels);
            
            // Check if 16-bit
            bool is_16bit = stbi_is_16_bit_from_memory(image_data.data(),
                                                      static_cast<int>(image_data.size()));
            
            // Try loading based on bit depth
            if (is_16bit) {
                loaded_data_16 = stbi_load_16_from_memory(image_data.data(),
                                                         static_cast<int>(image_data.size()),
                                                         &width, &height, &channels, 0);
                if (loaded_data_16) {
                    stbi_image_free(loaded_data_16);
                }
            } else {
                loaded_data = stbi_load_from_memory(image_data.data(),
                                                   static_cast<int>(image_data.size()),
                                                   &width, &height, &channels, 0);
                if (loaded_data) {
                    stbi_image_free(loaded_data);
                }
            }
            break;
        }
        
        case 6: {
            // Test all info functions together
            (void)stbi_info_from_memory(image_data.data(),
                                       static_cast<int>(image_data.size()),
                                       &width, &height, &channels);
            (void)stbi_is_16_bit_from_memory(image_data.data(),
                                           static_cast<int>(image_data.size()));
            (void)stbi_is_hdr_from_memory(image_data.data(),
                                        static_cast<int>(image_data.size()));
            break;
        }
        
        case 7: {
            // Test multiple load attempts with different parameters
            // Try 8-bit load
            loaded_data = stbi_load_from_memory(image_data.data(),
                                               static_cast<int>(image_data.size()),
                                               &width, &height, &channels, 0);
            if (loaded_data) {
                stbi_image_free(loaded_data);
            }
            
            // Try 16-bit load
            loaded_data_16 = stbi_load_16_from_memory(image_data.data(),
                                                     static_cast<int>(image_data.size()),
                                                     &width, &height, &channels, 0);
            if (loaded_data_16) {
                stbi_image_free(loaded_data_16);
            }
            
            // Try float load
            loaded_data_float = stbi_loadf_from_memory(image_data.data(),
                                                      static_cast<int>(image_data.size()),
                                                      &width, &height, &channels, 0);
            if (loaded_data_float) {
                stbi_image_free(loaded_data_float);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_002
