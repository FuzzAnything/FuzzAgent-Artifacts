#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "stb_image.h"

namespace harness_007 {
// Harness 007: Direct stb_image.h file-based image loading fuzzer
// Targets: stbi_load, stbi_loadf, stbi_load_16, stbi_info, stbi_is_16_bit,
//          stbi_load_from_file, stbi_loadf_from_file, stbi_load_16_from_file,
//          stbi_info_from_file, stbi_is_16_bit_from_file, stbi_image_free
// Goal: Exercise file-based stb_image.h APIs that are currently 0% covered
// Creates temporary image files and tests all file-based loading paths


// Include FuzzedDataProvider for structured input splitting

// Define STB_IMAGE_IMPLEMENTATION for stb_image.h
#define STB_IMAGE_IMPLEMENTATION

// Helper function to create deterministic temporary filename
std::string create_temp_filename(uint32_t seed, const std::string& suffix) {
    uint32_t hash = seed * 0x5bd1e995;
    hash ^= hash >> 15;
    hash *= 0x5bd1e995;
    
    std::string filename = "/tmp/stbi_fuzz_" + std::to_string(hash) + suffix;
    return filename;
}

// Helper to write image data to temporary file
bool write_temp_file(const std::string& filename, const std::vector<uint8_t>& data) {
    FILE* f = fopen(filename.c_str(), "wb");
    if (!f) return false;
    
    size_t written = fwrite(data.data(), 1, data.size(), f);
    fclose(f);
    
    return written == data.size();
}

// Helper to create minimal valid image data with proper signatures
std::vector<uint8_t> create_image_data(FuzzedDataProvider& fdp, uint8_t format_type, bool is_16bit, bool is_hdr) {
    std::vector<uint8_t> image_data;
    
    // Determine base size based on format
    size_t base_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    
    if (fdp.remaining_bytes() < base_size) {
        return image_data; // Empty vector
    }
    
    image_data = fdp.ConsumeBytes<uint8_t>(base_size);
    
    // Add proper signature based on format type
    switch (format_type) {
        case 0: { // PNG
            if (image_data.size() >= 8) {
                // PNG signature: 89 50 4E 47 0D 0A 1A 0A
                image_data[0] = 0x89;
                image_data[1] = 0x50; // 'P'
                image_data[2] = 0x4E; // 'N'
                image_data[3] = 0x47; // 'G'
                image_data[4] = 0x0D;
                image_data[5] = 0x0A;
                image_data[6] = 0x1A;
                image_data[7] = 0x0A;
                
                // Add IHDR chunk for 16-bit PNG if requested
                if (is_16bit && image_data.size() >= 40) {
                    // IHDR chunk type at offset 12-15
                    image_data[12] = 'I';
                    image_data[13] = 'H';
                    image_data[14] = 'D';
                    image_data[15] = 'R';
                    // Bit depth at offset 24 (16 = 0x10)
                    image_data[24] = 16;
                }
            }
            break;
        }
        case 1: { // JPEG
            if (image_data.size() >= 4) {
                // JPEG signature: FF D8 FF
                image_data[0] = 0xFF;
                image_data[1] = 0xD8;
                image_data[2] = 0xFF;
                // Common JFIF header
                if (image_data.size() >= 20) {
                    image_data[3] = 0xE0; // APP0 marker
                }
            }
            break;
        }
        case 2: { // BMP
            if (image_data.size() >= 2) {
                // BMP signature: 'B' 'M'
                image_data[0] = 'B';
                image_data[1] = 'M';
            }
            break;
        }
        case 3: { // GIF
            if (image_data.size() >= 6) {
                // GIF signature: 'G' 'I' 'F' '8' '7' 'a' or '8' '9' 'a'
                image_data[0] = 'G';
                image_data[1] = 'I';
                image_data[2] = 'F';
                image_data[3] = '8';
                image_data[4] = fdp.ConsumeBool() ? '7' : '9';
                image_data[5] = 'a';
            }
            break;
        }
        case 4: { // HDR (Radiance RGBE)
            if (image_data.size() >= 10) {
                // HDR signature: '#?RADIANCE'
                image_data[0] = '#';
                image_data[1] = '?';
                image_data[2] = 'R';
                image_data[3] = 'A';
                image_data[4] = 'D';
                image_data[5] = 'I';
                image_data[6] = 'A';
                image_data[7] = 'N';
                image_data[8] = 'C';
                image_data[9] = 'E';
            }
            break;
        }
        default: { // Unknown/random format
            // No special signature - test with corrupt/malformed data
            break;
        }
    }
    
    return image_data;
}

// Test file-based APIs with given filename
void test_file_apis(const std::string& filename, uint8_t test_type, uint8_t desired_channels) {
    int width = 0, height = 0, comp = 0;
    
    // Test 1: stbi_info - get image info without loading
    int info_result = stbi_info(filename.c_str(), &width, &height, &comp);
    (void)info_result;
    
    // Test 2: stbi_is_16_bit - check if 16-bit image
    int is_16bit_result = stbi_is_16_bit(filename.c_str());
    (void)is_16bit_result;
    
    // Test 3: stbi_info_from_file - using FILE* API
    FILE* f = fopen(filename.c_str(), "rb");
    if (f) {
        int info_file_result = stbi_info_from_file(f, &width, &height, &comp);
        (void)info_file_result;
        
        // Rewind for next test
        fseek(f, 0, SEEK_SET);
        
        // Test 4: stbi_is_16_bit_from_file
        int is_16bit_file_result = stbi_is_16_bit_from_file(f);
        (void)is_16bit_file_result;
        
        fclose(f);
    }
    
    // Test loading based on test_type
    switch (test_type % 4) {
        case 0: { // stbi_load (8-bit)
            unsigned char* data = stbi_load(filename.c_str(), &width, &height, &comp, desired_channels);
            if (data) {
                stbi_image_free(data);
            }
            break;
        }
        case 1: { // stbi_load_16 (16-bit)
            stbi_us* data16 = stbi_load_16(filename.c_str(), &width, &height, &comp, desired_channels);
            if (data16) {
                stbi_image_free(data16);
            }
            break;
        }
        case 2: { // stbi_loadf (HDR/float)
            float* dataf = stbi_loadf(filename.c_str(), &width, &height, &comp, desired_channels);
            if (dataf) {
                stbi_image_free(dataf);
            }
            break;
        }
        case 3: { // FILE* based loading
            f = fopen(filename.c_str(), "rb");
            if (f) {
                // Try all FILE* variants
                unsigned char* data = stbi_load_from_file(f, &width, &height, &comp, desired_channels);
                if (data) {
                    stbi_image_free(data);
                } else {
                    // Rewind and try 16-bit
                    fseek(f, 0, SEEK_SET);
                    stbi_us* data16 = stbi_load_from_file_16(f, &width, &height, &comp, desired_channels);
                    if (data16) {
                        stbi_image_free(data16);
                    } else {
                        // Rewind and try float
                        fseek(f, 0, SEEK_SET);
                        float* dataf = stbi_loadf_from_file(f, &width, &height, &comp, desired_channels);
                        if (dataf) {
                            stbi_image_free(dataf);
                        }
                    }
                }
                fclose(f);
            }
            break;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for meaningful testing
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test configuration
    uint8_t format_type = fdp.ConsumeIntegral<uint8_t>() % 6; // 0-5 formats (5 = unknown)
    bool test_16bit = fdp.ConsumeBool();
    bool test_hdr = fdp.ConsumeBool();
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t desired_channels = fdp.ConsumeIntegral<uint8_t>() % 5; // 0-4 (0 = original)
    uint32_t filename_seed = fdp.ConsumeIntegral<uint32_t>();
    
    // Create image data with proper signature
    std::vector<uint8_t> image_data = create_image_data(fdp, format_type, test_16bit, test_hdr);
    
    if (image_data.empty() || image_data.size() < 16) {
        return 0; // Not enough data for meaningful test
    }
    
    // Create temporary filename
    std::string temp_file = create_temp_filename(filename_seed, ".img");
    
    // Write image data to temporary file
    if (!write_temp_file(temp_file, image_data)) {
        return 0; // Failed to create temp file
    }
    
    // Test all file-based APIs
    test_file_apis(temp_file, test_type, desired_channels);
    
    // Clean up temporary file
    remove(temp_file.c_str());
    
    return 0;
}

} // namespace harness_007
