#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include "stb_image_write.h"
#include "stb_image.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_009 {
/*
 * Fuzzing harness for stb_image_write module - targeting image writing operations with only 19.16% line coverage
 * This harness specifically targets the low-coverage stb_image_write APIs through direct function calls.
 * 
 * Target APIs:
 * - stbi_write_png (Target: 4 undiscovered branches, 0% covered)
 * - stbi_write_bmp (Target: 2 undiscovered branches, 0% covered)
 * - stbi_write_tga (Target: 2 undiscovered branches, 0% covered)
 * - stbi_write_hdr (Target: 2 undiscovered branches, 0% covered) 
 * - stbi_write_jpg (Target: 2 undiscovered branches, 0% covered)
 * 
 * Required Helpers:
 * - stbi_load_from_memory (Init: Load test image data for LDR formats)
 * - stbi_loadf_from_memory (Init: Load test image data for HDR format)
 * - stbi_image_free (Cleanup: Free loaded image memory)
 * 
 * Invocation sequence:
 * 1. Load test image data using stbi_load_from_memory or stbi_loadf_from_memory for HDR
 * 2. Call stbi_write_png to write PNG format
 * 3. Call stbi_write_bmp to write BMP format
 * 4. Call stbi_write_tga to write TGA format
 * 5. Call stbi_write_hdr to write HDR format (requires HDR image data)
 * 6. Call stbi_write_jpg to write JPEG format
 * 7. Clean up with stbi_image_free
 * 
 * Implementation uses FuzzedDataProvider for structured input splitting to generate various
 * image parameters and test different image writing scenarios.
 */


// Include stb_image_write implementation
#define STB_IMAGE_WRITE_IMPLEMENTATION

// Include stb_image for loading functions
#define STB_IMAGE_IMPLEMENTATION


// Memory writer callback for testing stbi_write_*_to_func functions
struct MemoryWriterContext {
    std::vector<unsigned char> buffer;
};

void MemoryWriterCallback(void* context, void* data, int size) {
    MemoryWriterContext* ctx = static_cast<MemoryWriterContext*>(context);
    unsigned char* byte_data = static_cast<unsigned char*>(data);
    ctx->buffer.insert(ctx->buffer.end(), byte_data, byte_data + size);
}

// Generate synthetic image data based on fuzzer input
// Returns image data and updates width/height if limited to prevent buffer overflow
std::vector<unsigned char> GenerateImageData(int& width, int& height, int components, FuzzedDataProvider& fdp) {
    std::vector<unsigned char> image_data;
    size_t pixel_count = width * height * components;
    
    if (pixel_count == 0) {
        return image_data;
    }
    
    // Limit size to prevent excessive memory usage
    size_t max_pixels = 64 * 64 * 4; // 64x64 RGBA max
    if (pixel_count > max_pixels) {
        // Recalculate dimensions to match limited data size
        // Use 64x64 as base, maintaining component count
        width = 64;
        height = 64;
        pixel_count = width * height * components; // This should be at most max_pixels
    }
    
    image_data = fdp.ConsumeBytes<unsigned char>(pixel_count);
    
    // If not enough data, fill with pattern
    if (image_data.size() < pixel_count) {
        size_t remaining = pixel_count - image_data.size();
        std::vector<unsigned char> pattern = fdp.ConsumeBytes<unsigned char>(std::min(remaining, (size_t)256));
        while (image_data.size() < pixel_count) {
            for (unsigned char val : pattern) {
                if (image_data.size() >= pixel_count) break;
                image_data.push_back(val);
            }
        }
    }
    
    return image_data;
}

// Generate synthetic HDR image data (float)
// Returns image data and updates width/height if limited to prevent buffer overflow
std::vector<float> GenerateHDRImageData(int& width, int& height, int components, FuzzedDataProvider& fdp) {
    std::vector<float> image_data;
    size_t pixel_count = width * height * components;
    
    if (pixel_count == 0) {
        return image_data;
    }
    
    // Limit size to prevent excessive memory usage
    size_t max_pixels = 32 * 32 * 3; // 32x32 RGB max for HDR
    if (pixel_count > max_pixels) {
        // Recalculate dimensions to match limited data size
        // Use 32x32 as base, maintaining component count
        width = 32;
        height = 32;
        // Ensure components is 3 for HDR (typical for HDR images)
        if (components != 3) {
            components = 3;
        }
        pixel_count = width * height * components; // This should be exactly max_pixels (3072)
    }
    
    // Consume floats from fuzzer input
    for (size_t i = 0; i < pixel_count; i++) {
        if (fdp.remaining_bytes() >= sizeof(float)) {
            float val = fdp.ConsumeFloatingPoint<float>();
            // Clamp to reasonable HDR range
            val = std::min(std::max(val, 0.0f), 1000.0f);
            image_data.push_back(val);
        } else {
            // Fill with pattern if insufficient data
            image_data.push_back(static_cast<float>(i % 256) / 255.0f);
        }
    }
    
    return image_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for basic parameters
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume image parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(1, 128);
    int height = fdp.ConsumeIntegralInRange<int>(1, 128);
    int components = fdp.ConsumeIntegralInRange<int>(1, 4);
    int test_type = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Generate image data based on test type
    std::vector<unsigned char> ldr_image_data;
    std::vector<float> hdr_image_data;
    
    if (test_type == 0) {
        // Test LDR image formats (PNG, BMP, TGA, JPG)
        int ldr_width = width;
        int ldr_height = height;
        ldr_image_data = GenerateImageData(ldr_width, ldr_height, components, fdp);
        
        if (ldr_image_data.empty()) {
            return 0;
        }
        
        // Test stbi_load_from_memory
        int x, y, comp;
        unsigned char* loaded_data = stbi_load_from_memory(
            ldr_image_data.data(), ldr_image_data.size(), &x, &y, &comp, components);
        
        if (loaded_data != nullptr) {
            // Test file-based writing functions with temporary files
            const char* temp_prefix = "/tmp/stbi_test_";
            
            // Write PNG
            {
                std::string filename = std::string(temp_prefix) + "test.png";
                int stride = ldr_width * components;
                stbi_write_png(filename.c_str(), ldr_width, ldr_height, components, 
                              ldr_image_data.data(), stride);
            }
            
            // Write BMP
            {
                std::string filename = std::string(temp_prefix) + "test.bmp";
                stbi_write_bmp(filename.c_str(), ldr_width, ldr_height, components, 
                              ldr_image_data.data());
            }
            
            // Write TGA
            {
                std::string filename = std::string(temp_prefix) + "test.tga";
                stbi_write_tga(filename.c_str(), ldr_width, ldr_height, components, 
                              ldr_image_data.data());
            }
            
            // Write JPG with varying quality
            {
                std::string filename = std::string(temp_prefix) + "test.jpg";
                int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
                stbi_write_jpg(filename.c_str(), ldr_width, ldr_height, components, 
                              ldr_image_data.data(), quality);
            
            }
            
            // Test callback-based writing functions
            MemoryWriterContext png_ctx, bmp_ctx, tga_ctx, jpg_ctx;
            
            int stride = ldr_width * components;
            int callback_quality = fdp.ConsumeIntegralInRange<int>(1, 100);
            
            stbi_write_png_to_func(MemoryWriterCallback, &png_ctx, 
                                  ldr_width, ldr_height, components, ldr_image_data.data(), stride);
            stbi_write_bmp_to_func(MemoryWriterCallback, &bmp_ctx, 
                                  ldr_width, ldr_height, components, ldr_image_data.data());
            stbi_write_tga_to_func(MemoryWriterCallback, &tga_ctx, 
                                  ldr_width, ldr_height, components, ldr_image_data.data());
            stbi_write_jpg_to_func(MemoryWriterCallback, &jpg_ctx, 
                                  ldr_width, ldr_height, components, ldr_image_data.data(), callback_quality);
            // Free loaded data
            stbi_image_free(loaded_data);
        }
        
    } else if (test_type == 1) {
        // Test HDR image format specifically
        int hdr_width = width;
        int hdr_height = height;
        hdr_image_data = GenerateHDRImageData(hdr_width, hdr_height, components, fdp);
        if (hdr_image_data.empty()) {
            return 0;
        }
        
        // Test stbi_loadf_from_memory for HDR
        int x, y, comp;
        float* loaded_hdr_data = stbi_loadf_from_memory(
            reinterpret_cast<const unsigned char*>(hdr_image_data.data()),
            hdr_image_data.size() * sizeof(float), &x, &y, &comp, components);
        
        if (loaded_hdr_data != nullptr) {
            // Write HDR file - use the dimensions that match the actual data size
            std::string filename = "/tmp/stbi_test_hdr.hdr";
            stbi_write_hdr(filename.c_str(), hdr_width, hdr_height, components, hdr_image_data.data());
            
            // Test HDR callback-based writing
            MemoryWriterContext hdr_ctx;
            stbi_write_hdr_to_func(MemoryWriterCallback, &hdr_ctx, 
                                  hdr_width, hdr_height, components, hdr_image_data.data());
            // Free loaded HDR data
            stbi_image_free(loaded_hdr_data);
        }
        
    } else if (test_type == 2) {
        // Test mixed LDR and HDR formats
        int mixed_ldr_width = width;
        int mixed_ldr_height = height;
        ldr_image_data = GenerateImageData(mixed_ldr_width, mixed_ldr_height, components, fdp);
        
        // For HDR, use separate dimensions that may be adjusted
        int hdr_mixed_width = width;
        int hdr_mixed_height = height;
        hdr_image_data = GenerateHDRImageData(hdr_mixed_width, hdr_mixed_height, 3, fdp); // HDR typically RGB
        
        if (!ldr_image_data.empty()) {
            // Write LDR formats using dimensions that match the actual data size
            std::string png_filename = "/tmp/stbi_test_mixed.png";
            std::string bmp_filename = "/tmp/stbi_test_mixed.bmp";
            std::string tga_filename = "/tmp/stbi_test_mixed.tga";
            
            stbi_write_png(png_filename.c_str(), mixed_ldr_width, mixed_ldr_height, components, 
                          ldr_image_data.data(), mixed_ldr_width * components);
            stbi_write_bmp(bmp_filename.c_str(), mixed_ldr_width, mixed_ldr_height, components, 
                          ldr_image_data.data());
            stbi_write_tga(tga_filename.c_str(), mixed_ldr_width, mixed_ldr_height, components, 
                          ldr_image_data.data());
        }
        
        if (!hdr_image_data.empty()) {
            // Write HDR format - use dimensions that match the actual data size
            std::string hdr_filename = "/tmp/stbi_test_mixed.hdr";
            stbi_write_hdr(hdr_filename.c_str(), hdr_mixed_width, hdr_mixed_height, 3, hdr_image_data.data());
        }
    } else {
        // Test with configuration flags
        int flip_flag = fdp.ConsumeBool();
        stbi_flip_vertically_on_write(flip_flag);
        
        // Test with different compression levels for PNG
        if (fdp.ConsumeBool()) {
            int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
            stbi_write_png_compression_level = compression_level;
        }
        
        // Test with RLE for TGA
        if (fdp.ConsumeBool()) {
            int rle_flag = fdp.ConsumeBool();
            stbi_write_tga_with_rle = rle_flag;
        }
        
        // Generate and write test image
        int config_width = width;
        int config_height = height;
        ldr_image_data = GenerateImageData(config_width, config_height, components, fdp);
        
        if (!ldr_image_data.empty()) {
            std::string filename = "/tmp/stbi_test_config.png";
            stbi_write_png(filename.c_str(), config_width, config_height, components, 
                          ldr_image_data.data(), config_width * components);
        }
        
        // Reset flags
        stbi_flip_vertically_on_write(0);
        stbi_write_png_compression_level = 8;
        stbi_write_tga_with_rle = 1;
    }
    
    return 0;
}

} // namespace harness_009
