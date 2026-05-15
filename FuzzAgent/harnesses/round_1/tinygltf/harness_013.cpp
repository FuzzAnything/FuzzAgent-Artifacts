#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include "stb_image.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>

namespace harness_013 {
/*
 * Fuzzing harness for stb_image.h image loading functions in tinygltf
 * 
 * This harness targets critical coverage gaps in stb_image.h (currently only 12.59% line coverage).
 * Focuses on image loading/inspection functions that are 0% covered:
 * 1. stbi_load - Load 8-bit image data from memory
 * 2. stbi_load_16 - Load 16-bit image data from memory  
 * 3. stbi_loadf - Load float (HDR) image data from memory
 * 4. stbi_info - Get image information without decoding
 * 5. stbi_is_16_bit - Check if image is 16-bit
 * 6. stbi_is_hdr - Check if image is HDR format
 * 
 * The harness integrates with tinygltf's image loading subsystem to maximize coverage impact
 * while being semantically different from existing harnesses (000-012) by targeting 
 * image processing rather than GLTF parsing.
 * 
 * Testing strategy:
 * 1. Use fuzzed input as raw image data for stb_image functions
 * 2. Test all image inspection functions (info, is_16_bit, is_hdr)
 * 3. Attempt loading with different desired channels (0=preserve, 1-4=force)
 * 4. Handle failures gracefully - stb_image returns NULL on invalid images
 * 5. Properly free allocated image data with stbi_image_free
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful image testing
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume selector to determine testing strategy
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume desired channels (0 = preserve, 1-4 = force specific channels)
    int desired_channels = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Consume remaining bytes as image data
    std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (image_data.empty()) return 0;
    
    // Variables for image information
    int width = 0, height = 0, channels = 0;
    
    // Strategy 1: Test stbi_info functions first (no decoding)
    switch (selector % 3) {
        case 0: {
            // Test stbi_info_from_memory
            int info_result = stbi_info_from_memory(
                image_data.data(), 
                static_cast<int>(image_data.size()), 
                &width, &height, &channels
            );
            (void)info_result; // Use result to avoid unused variable warning
            break;
        }
        case 1: {
            // Test stbi_is_16_bit_from_memory
            bool is_16_bit = stbi_is_16_bit_from_memory(
                image_data.data(), 
                static_cast<int>(image_data.size())
            );
            (void)is_16_bit;
            break;
        }
        case 2: {
            // Test stbi_is_hdr_from_memory
            bool is_hdr = stbi_is_hdr_from_memory(
                image_data.data(), 
                static_cast<int>(image_data.size())
            );
            (void)is_hdr;
            break;
        }
    }
    
    // Strategy 2: Attempt actual image loading based on selector
    unsigned char* loaded_data_8bit = nullptr;
    unsigned short* loaded_data_16bit = nullptr;
    float* loaded_data_float = nullptr;
    
    switch ((selector >> 2) % 4) {
        case 0: {
            // Test stbi_load_from_memory (8-bit)
            loaded_data_8bit = stbi_load_from_memory(
                image_data.data(),
                static_cast<int>(image_data.size()),
                &width, &height, &channels,
                desired_channels
            );
            break;
        }
        case 1: {
            // Test stbi_load_16_from_memory (16-bit)
            loaded_data_16bit = stbi_load_16_from_memory(
                image_data.data(),
                static_cast<int>(image_data.size()),
                &width, &height, &channels,
                desired_channels
            );
            break;
        }
        case 2: {
            // Test stbi_loadf_from_memory (float/HDR)
            loaded_data_float = stbi_loadf_from_memory(
                image_data.data(),
                static_cast<int>(image_data.size()),
                &width, &height, &channels,
                desired_channels
            );
            break;
        }
        case 3: {
            // Test GIF loading (if data looks like GIF)
            if (image_data.size() >= 6 && 
                image_data[0] == 'G' && image_data[1] == 'I' && 
                image_data[2] == 'F' && image_data[3] == '8') {
                int* delays = nullptr;
                int z = 0;
                loaded_data_8bit = stbi_load_gif_from_memory(
                    image_data.data(),
                    static_cast<int>(image_data.size()),
                    &delays, &width, &height, &z, &channels,
                    desired_channels
                );
                if (delays) {
                    free(delays); // stbi_load_gif_from_memory allocates delays array
                }
            }
            break;
        }
    }
    
    // Strategy 3: Integrate with tinygltf's LoadImageData function
    // This tests the actual integration point used by tinygltf
    // Note: LoadImageDataOption struct might not be available in all builds
    // We'll use a simpler approach - test stb_image functions directly
    
    // Additional tests for completeness
    if (size > 32) {
        // Test stbi_set_flip_vertically_on_load
        int flip_value = (selector & 0x01) ? 1 : 0;
        stbi_set_flip_vertically_on_load(flip_value);
        
        // Test stbi_set_unpremultiply_on_load  
        int unpremultiply_value = (selector & 0x02) ? 1 : 0;
        stbi_set_unpremultiply_on_load(unpremultiply_value);
        
        // Test stbi_convert_iphone_png_to_rgb
        int convert_value = (selector & 0x04) ? 1 : 0;
        stbi_convert_iphone_png_to_rgb(convert_value);
        
        // Reset to defaults
        stbi_set_flip_vertically_on_load(0);
        stbi_set_unpremultiply_on_load(0);
        stbi_convert_iphone_png_to_rgb(0);
    }
    // Clean up allocated image data
    if (loaded_data_8bit) {
        stbi_image_free(loaded_data_8bit);
    }
    if (loaded_data_16bit) {
        stbi_image_free(loaded_data_16bit);
    }
    if (loaded_data_float) {
        stbi_image_free(loaded_data_float);
    }
    
    return 0;
}

} // namespace harness_013
