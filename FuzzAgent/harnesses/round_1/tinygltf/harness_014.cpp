#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include "stb_image.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <unistd.h>

namespace harness_014 {
/*
 * Fuzzing harness for stb_image.h comprehensive image loading APIs (File-based focus)
 * 
 * This harness targets uncovered file-based image loading APIs in stb_image.h with
 * 66+ undiscovered branches across 10+ APIs. Focuses on complete image loading lifecycle
 * including file-based variants that are 0% covered.
 * 
 * Targets file-based APIs (currently uncovered):
 * 1. stbi_load_from_file - Load 8-bit image from FILE*
 * 2. stbi_load_from_file_16 - Load 16-bit image from FILE*  
 * 3. stbi_loadf_from_file - Load float (HDR) image from FILE*
 * 4. stbi_info_from_file - Get image info from FILE*
 * 5. stbi_is_16_bit_from_file - Check 16-bit from FILE*
 * 
 * Also tests memory-based APIs for comprehensive coverage:
 * 6. stbi_load - Load 8-bit image from filename
 * 7. stbi_load_16 - Load 16-bit image from filename
 * 8. stbi_loadf - Load float (HDR) image from filename
 * 9. stbi_info - Get image info from filename
 * 10. stbi_is_16_bit - Check 16-bit from filename
 * 11. stbi_is_hdr - Check HDR format
 * 
 * Testing strategy:
 * 1. Create temporary files with fuzzed image data
 * 2. Test full image loading lifecycle: Setup → Inspection → Loading → Cleanup
 * 3. Test multiple image formats (PNG, JPEG, BMP, TGA, HDR, GIF) via fuzzed data
 * 4. Use FILE* APIs for direct file handle testing
 * 5. Test different desired channels (0=preserve, 1-4=force)
 * 6. Proper resource cleanup with stbi_image_free
 * 
 * Semantically different from harness_013 (memory-based focus) by targeting
 * file-based APIs and complete loading lifecycle.
 */

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful image testing with temporary files
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Strategy selection: 0=file-based APIs, 1=filename APIs, 2=mixed
    uint8_t strategy = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume desired channels (0 = preserve, 1-4 = force specific channels)
    int desired_channels = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Consume image format hint for more targeted testing
    uint8_t format_hint = fdp.ConsumeIntegral<uint8_t>() % 6; // PNG, JPEG, BMP, TGA, HDR, GIF
    
    // Consume remaining bytes as image data
    std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (image_data.empty()) return 0;
    
    // Create temporary file for file-based testing
    char temp_filename[] = "/tmp/stbi_fuzz_XXXXXX";
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd == -1) return 0;
    
    // Write image data to temporary file
    FILE* temp_file = fdopen(temp_fd, "wb");
    if (!temp_file) {
        close(temp_fd);
        return 0;
    }
    
    size_t written = fwrite(image_data.data(), 1, image_data.size(), temp_file);
    fclose(temp_file);
    
    if (written != image_data.size()) {
        unlink(temp_filename);
        return 0;
    }
    
    // Reopen file for reading
    FILE* image_file = fopen(temp_filename, "rb");
    if (!image_file) {
        unlink(temp_filename);
        return 0;
    }
    
    // Variables for image information
    int width = 0, height = 0, channels = 0;
    
    // Phase 1: INSPECTION PHASE - Get image info without loading
    switch (strategy) {
        case 0: { // File-based APIs using FILE*
            // Test stbi_info_from_file
            int info_result = stbi_info_from_file(image_file, &width, &height, &channels);
            (void)info_result;
            
            // Rewind file for next test
            fseek(image_file, 0, SEEK_SET);
            
            // Test stbi_is_16_bit_from_file
            bool is_16_bit = stbi_is_16_bit_from_file(image_file);
            (void)is_16_bit;
            
            // Rewind file for next test  
            fseek(image_file, 0, SEEK_SET);
            
            // Note: stbi_is_hdr doesn't have FILE* variant, test memory version
            bool is_hdr = stbi_is_hdr_from_memory(image_data.data(), static_cast<int>(image_data.size()));
            (void)is_hdr;
            break;
        }
        
        case 1: { // Filename-based APIs
            // Test stbi_info
            int info_result = stbi_info(temp_filename, &width, &height, &channels);
            (void)info_result;
            
            // Test stbi_is_16_bit
            bool is_16_bit = stbi_is_16_bit(temp_filename);
            (void)is_16_bit;
            
            // Test stbi_is_hdr
            bool is_hdr = stbi_is_hdr(temp_filename);
            (void)is_hdr;
            break;
        }
        
        case 2: { // Mixed approach - test both
            // Test FILE* variants
            int info_result_file = stbi_info_from_file(image_file, &width, &height, &channels);
            (void)info_result_file;
            
            fseek(image_file, 0, SEEK_SET);
            bool is_16_bit_file = stbi_is_16_bit_from_file(image_file);
            (void)is_16_bit_file;
            
            fseek(image_file, 0, SEEK_SET);
            
            // Test filename variants
            int info_result_name = stbi_info(temp_filename, &width, &height, &channels);
            (void)info_result_name;
            
            bool is_16_bit_name = stbi_is_16_bit(temp_filename);
            (void)is_16_bit_name;
            
            bool is_hdr_name = stbi_is_hdr(temp_filename);
            (void)is_hdr_name;
            break;
        }
    }
    
    // Phase 2: LOADING PHASE - Load image data
    unsigned char* loaded_data_8bit = nullptr;
    unsigned short* loaded_data_16bit = nullptr;
    float* loaded_data_float = nullptr;
    
    // Select loading method based on strategy and format hint
    uint8_t load_method = (strategy * 2 + format_hint) % 4;
    
    switch (load_method) {
        case 0: { // stbi_load_from_file (8-bit from FILE*)
            fseek(image_file, 0, SEEK_SET);
            loaded_data_8bit = stbi_load_from_file(
                image_file,
                &width, &height, &channels,
                desired_channels
            );
            break;
        }
        
        case 1: { // stbi_load_from_file_16 (16-bit from FILE*)
            fseek(image_file, 0, SEEK_SET);
            loaded_data_16bit = stbi_load_from_file_16(
                image_file,
                &width, &height, &channels,
                desired_channels
            );
            break;
        }
        
        case 2: { // stbi_loadf_from_file (float/HDR from FILE*)
            fseek(image_file, 0, SEEK_SET);
            loaded_data_float = stbi_loadf_from_file(
                image_file,
                &width, &height, &channels,
                desired_channels
            );
            break;
        }
        
        case 3: { // Filename-based loading (mix of all types)
            // Randomly select which filename-based function to test
            uint8_t filename_func = format_hint % 3;
            
            if (filename_func == 0) {
                loaded_data_8bit = stbi_load(
                    temp_filename,
                    &width, &height, &channels,
                    desired_channels
                );
            } else if (filename_func == 1) {
                loaded_data_16bit = stbi_load_16(
                    temp_filename,
                    &width, &height, &channels,
                    desired_channels
                );
            } else {
                loaded_data_float = stbi_loadf(
                    temp_filename,
                    &width, &height, &channels,
                    desired_channels
                );
            }
            break;
        }
    }
    
    // Phase 3: TEST ADDITIONAL VARIANTS if we have enough data
    if (image_data.size() > 100) {
        // Test memory-based variants for comparison
        int mem_width = 0, mem_height = 0, mem_channels = 0;
        
        switch (format_hint % 3) {
            case 0:
                loaded_data_8bit = stbi_load_from_memory(
                    image_data.data(),
                    static_cast<int>(image_data.size()),
                    &mem_width, &mem_height, &mem_channels,
                    desired_channels
                );
                break;
                
            case 1:
                loaded_data_16bit = stbi_load_16_from_memory(
                    image_data.data(),
                    static_cast<int>(image_data.size()),
                    &mem_width, &mem_height, &mem_channels,
                    desired_channels
                );
                break;
                
            case 2:
                loaded_data_float = stbi_loadf_from_memory(
                    image_data.data(),
                    static_cast<int>(image_data.size()),
                    &mem_width, &mem_height, &mem_channels,
                    desired_channels
                );
                break;
        }
    }
    
    // Phase 4: TEST IMAGE PROCESSING OPTIONS
    if (image_data.size() > 50) {
        // Test configuration functions
        int config_value = format_hint % 2;
        
        stbi_set_flip_vertically_on_load(config_value);
        stbi_set_unpremultiply_on_load(config_value);
        stbi_convert_iphone_png_to_rgb(config_value);
        
        // Reset to defaults
        stbi_set_flip_vertically_on_load(0);
        stbi_set_unpremultiply_on_load(0);
        stbi_convert_iphone_png_to_rgb(0);
    }
    
    // Phase 5: CLEANUP PHASE
    // Free loaded image data
    if (loaded_data_8bit) {
        stbi_image_free(loaded_data_8bit);
    }
    if (loaded_data_16bit) {
        stbi_image_free(loaded_data_16bit);
    }
    if (loaded_data_float) {
        stbi_image_free(loaded_data_float);
    }
    
    // Close file and remove temporary file
    fclose(image_file);
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_014
