#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <filesystem>
#include <fuzzer/FuzzedDataProvider.h>
#include "stb_image.h"
#include "stb_image_write.h"

namespace harness_009 {
// Harness 009: Targeted stb_image_write fuzzer for tinygltf
// Targets completely uncovered stb_image_write APIs: stbi_write_png, stbi_write_bmp, stbi_write_hdr, stbi_write_jpg
// Current coverage: stb_image_write module has only 17.30% line coverage
// Based on coverage guidance: stbi_write_png (0% coverage, 4 branches), stbi_write_bmp (0% coverage, 2 branches),
// stbi_write_hdr (0% coverage, 2 branches), stbi_write_jpg (0% coverage, 2 branches)
// Uses FuzzedDataProvider for structured input splitting to generate synthetic image data


// Include FuzzedDataProvider for structured input splitting

// Define implementations for stb_image and stb_image_write
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// Helper to create deterministic temporary filename
std::string create_temp_filename(uint32_t seed, const std::string& suffix) {
    uint32_t hash = seed * 0x5bd1e995;
    hash ^= hash >> 15;
    hash *= 0x5bd1e995;
    
    std::string filename = "/tmp/tinygltf_stb_fuzz_" + std::to_string(hash) + suffix;
    return filename;
}

// Helper to generate synthetic image data from fuzzer input
std::vector<unsigned char> generate_synthetic_image_data(FuzzedDataProvider& fdp, int width, int height, int components) {
    size_t pixel_count = width * height * components;
    
    // Limit memory usage for safety
    if (pixel_count > 1024 * 1024) { // 1MB limit
        pixel_count = 1024 * 1024;
    }
    
    std::vector<unsigned char> image_data;
    if (fdp.remaining_bytes() >= pixel_count) {
        image_data = fdp.ConsumeBytes<unsigned char>(pixel_count);
    } else {
        // Not enough data, use synthetic pattern
        image_data.resize(pixel_count);
        for (size_t i = 0; i < pixel_count; i++) {
            image_data[i] = static_cast<unsigned char>((i * 17) % 256);
        }
    }
    
    return image_data;
}

// Helper to generate synthetic HDR image data from fuzzer input
std::vector<float> generate_synthetic_hdr_data(FuzzedDataProvider& fdp, int width, int height, int components) {
    size_t pixel_count = width * height * components;
    
    // Limit memory usage for safety
    if (pixel_count > 512 * 512) { // More conservative limit for floats
        pixel_count = 512 * 512;
    }
    
    std::vector<float> hdr_data;
    hdr_data.resize(pixel_count);
    
    // Fill with random float values in reasonable HDR range
    for (size_t i = 0; i < pixel_count && fdp.remaining_bytes() > 0; i++) {
        // Consume float values from input, or use pattern
        if (fdp.remaining_bytes() >= sizeof(float)) {
            hdr_data[i] = fdp.ConsumeFloatingPoint<float>();
        } else {
            // Generate synthetic HDR values (0.0 to 100.0 typical for HDR)
            hdr_data[i] = static_cast<float>((i * 13) % 1000) / 10.0f;
        }
    }
    
    return hdr_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic image operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector (0-3 for different test paths)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume seeds for deterministic temporary filenames
    uint32_t temp_seed = fdp.ConsumeIntegral<uint32_t>();
    
    // Consume image parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(1, 256);
    int height = fdp.ConsumeIntegralInRange<int>(1, 256);
    int components = fdp.ConsumeIntegralInRange<int>(1, 4); // 1: grey, 2: grey+alpha, 3: RGB, 4: RGBA
    
    // Path 0: Direct stb_image_write API testing with synthetic image data
    if (operation == 0) {
        // Generate synthetic image data
        std::vector<unsigned char> image_data = generate_synthetic_image_data(fdp, width, height, components);
        
        if (image_data.empty()) return 0;
        
        // Test all four stb_image_write APIs
        std::vector<std::string> extensions = {".png", ".bmp", ".jpg", ".hdr"};
        
        for (size_t i = 0; i < extensions.size(); i++) {
            std::string filename = create_temp_filename(temp_seed + i, extensions[i]);
            
            int result = 0;
            switch (i) {
                case 0: // PNG
                    result = stbi_write_png(filename.c_str(), width, height, components,
                                           image_data.data(), width * components);
                    break;
                case 1: // BMP
                    result = stbi_write_bmp(filename.c_str(), width, height, components,
                                           image_data.data());
                    break;
                case 2: // JPG
                    result = stbi_write_jpg(filename.c_str(), width, height, components,
                                           image_data.data(), 85); // 85% quality
                    break;
                case 3: // HDR (requires float data)
                    // Generate HDR data for this test
                    std::vector<float> hdr_data = generate_synthetic_hdr_data(fdp, width, height, components);
                    result = stbi_write_hdr(filename.c_str(), width, height, components,
                                           hdr_data.data());
                    break;
            }
            
            (void)result; // Ignore result for fuzzing
            
            // Clean up temporary file if it was created
            std::remove(filename.c_str());
        }
    }
    
    // Path 1: stbi_load_from_memory -> stbi_write_* sequence
    else if (operation == 1) {
        // Use remaining input as image data to load
        size_t image_data_size = fdp.ConsumeIntegralInRange<size_t>(16, fdp.remaining_bytes());
        std::vector<unsigned char> raw_image_data = fdp.ConsumeBytes<unsigned char>(image_data_size);
        
        if (raw_image_data.empty()) return 0;
        
        // Try to load the image data using stbi_load_from_memory
        int loaded_width = 0, loaded_height = 0, loaded_components = 0;
        unsigned char* loaded_data = stbi_load_from_memory(
            raw_image_data.data(),
            static_cast<int>(raw_image_data.size()),
            &loaded_width, &loaded_height, &loaded_components,
            0); // desired_channels = 0 means load as-is
        
        if (loaded_data != nullptr && loaded_width > 0 && loaded_height > 0 && loaded_components > 0) {
            // Limit dimensions for safety
            if (loaded_width > 1024) loaded_width = 1024;
            if (loaded_height > 1024) loaded_height = 1024;
            
            // Test writing loaded image with different formats
            std::string png_filename = create_temp_filename(temp_seed, "_loaded.png");
            std::string bmp_filename = create_temp_filename(temp_seed, "_loaded.bmp");
            std::string jpg_filename = create_temp_filename(temp_seed, "_loaded.jpg");
            
            // Write PNG
            int png_result = stbi_write_png(png_filename.c_str(), loaded_width, loaded_height, loaded_components,
                                           loaded_data, loaded_width * loaded_components);
            
            // Write BMP
            int bmp_result = stbi_write_bmp(bmp_filename.c_str(), loaded_width, loaded_height, loaded_components,
                                           loaded_data);
            
            // Write JPG
            int jpg_result = stbi_write_jpg(jpg_filename.c_str(), loaded_width, loaded_height, loaded_components,
                                           loaded_data, 90);
            
            (void)png_result;
            (void)bmp_result;
            (void)jpg_result;
            
            // Clean up temporary files
            std::remove(png_filename.c_str());
            std::remove(bmp_filename.c_str());
            std::remove(jpg_filename.c_str());
            
            // Free loaded image data
            stbi_image_free(loaded_data);
        } else if (loaded_data != nullptr) {
            // Free data even if dimensions are invalid
            stbi_image_free(loaded_data);
        }
    }
    
    // Path 2: stbi_loadf_from_memory -> stbi_write_hdr sequence (HDR testing)
    else if (operation == 2) {
        // Use remaining input as image data to load as HDR
        size_t image_data_size = fdp.ConsumeIntegralInRange<size_t>(16, fdp.remaining_bytes());
        std::vector<unsigned char> raw_image_data = fdp.ConsumeBytes<unsigned char>(image_data_size);
        
        if (raw_image_data.empty()) return 0;
        
        // Try to load the image data as HDR using stbi_loadf_from_memory
        int loaded_width = 0, loaded_height = 0, loaded_components = 0;
        float* loaded_hdr_data = stbi_loadf_from_memory(
            raw_image_data.data(),
            static_cast<int>(raw_image_data.size()),
            &loaded_width, &loaded_height, &loaded_components,
            0); // desired_channels = 0 means load as-is
        
        if (loaded_hdr_data != nullptr && loaded_width > 0 && loaded_height > 0 && loaded_components > 0) {
            // Limit dimensions for safety
            if (loaded_width > 512) loaded_width = 512;
            if (loaded_height > 512) loaded_height = 512;
            
            // Write HDR file
            std::string hdr_filename = create_temp_filename(temp_seed, "_loaded.hdr");
            int hdr_result = stbi_write_hdr(hdr_filename.c_str(), loaded_width, loaded_height, loaded_components,
                                           loaded_hdr_data);
            
            (void)hdr_result;
            
            // Also test converting HDR to LDR and writing as PNG
            if (loaded_components >= 3) { // Need RGB for meaningful conversion
                // Allocate LDR buffer
                size_t ldr_size = loaded_width * loaded_height * loaded_components;
                std::vector<unsigned char> ldr_data(ldr_size);
                
                // Simple tone mapping: clamp to 0-255 range
                for (size_t i = 0; i < ldr_size; i++) {
                    float val = loaded_hdr_data[i];
                    // Basic tone mapping: clamp and scale
                    if (val < 0.0f) val = 0.0f;
                    if (val > 1.0f) val = 1.0f;
                    ldr_data[i] = static_cast<unsigned char>(val * 255.0f);
                }
                
                std::string png_filename = create_temp_filename(temp_seed, "_hdr_to_ldr.png");
                int png_result = stbi_write_png(png_filename.c_str(), loaded_width, loaded_height, loaded_components,
                                               ldr_data.data(), loaded_width * loaded_components);
                
                (void)png_result;
                std::remove(png_filename.c_str());
            }
            
            // Clean up temporary file
            std::remove(hdr_filename.c_str());
            
            // Free loaded HDR data
            stbi_image_free(loaded_hdr_data);
        } else if (loaded_hdr_data != nullptr) {
            // Free data even if dimensions are invalid
            stbi_image_free(loaded_hdr_data);
        }
    }
    
    // Path 3: Combined test - create, load, write sequence with various parameters
    else {
        // Generate synthetic image data
        std::vector<unsigned char> image_data = generate_synthetic_image_data(fdp, width, height, components);
        
        if (image_data.empty()) return 0;
        
        // Test with different quality settings for JPEG
        std::vector<int> jpg_qualities = {10, 50, 85, 100};
        for (int quality : jpg_qualities) {
            std::string jpg_filename = create_temp_filename(temp_seed + quality, "_quality.jpg");
            int jpg_result = stbi_write_jpg(jpg_filename.c_str(), width, height, components,
                                           image_data.data(), quality);
            (void)jpg_result;
            std::remove(jpg_filename.c_str());
        }
        
        // Test PNG with different stride values
        std::string png_filename = create_temp_filename(temp_seed, "_stride.png");
        int png_result = stbi_write_png(png_filename.c_str(), width, height, components,
                                       image_data.data(), width * components); // Normal stride
        (void)png_result;
        std::remove(png_filename.c_str());
        
        // Test with flipped vertically
        stbi_flip_vertically_on_write(1);
        std::string flipped_filename = create_temp_filename(temp_seed, "_flipped.bmp");
        int flipped_result = stbi_write_bmp(flipped_filename.c_str(), width, height, components,
                                           image_data.data());
        (void)flipped_result;
        std::remove(flipped_filename.c_str());
        stbi_flip_vertically_on_write(0); // Reset
        
        // Test PNG compression level variations if available
        #ifdef stbi_write_png_compression_level
        int original_level = stbi_write_png_compression_level;
        for (int level = 0; level <= 9; level += 3) {
            stbi_write_png_compression_level = level;
            std::string png_level_filename = create_temp_filename(temp_seed + level, "_level.png");
            int level_result = stbi_write_png(png_level_filename.c_str(), width, height, components,
                                             image_data.data(), width * components);
            (void)level_result;
            std::remove(png_level_filename.c_str());
        }
        stbi_write_png_compression_level = original_level;
        #endif
        
        // Generate and test HDR with synthetic float data
        std::vector<float> hdr_data = generate_synthetic_hdr_data(fdp, width, height, components);
        std::string hdr_filename = create_temp_filename(temp_seed, "_synthetic.hdr");
        int hdr_result = stbi_write_hdr(hdr_filename.c_str(), width, height, components,
                                       hdr_data.data());
        (void)hdr_result;
        std::remove(hdr_filename.c_str());
    }
    
    return 0;
}

} // namespace harness_009
