#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "stb_image.h"

namespace harness_009 {
/*
 * Fuzzing harness for tinygltf library targeting GIF loading and zlib decompression functions in stb_image.h
 * Goal: Exercise GIF image loading and zlib decompression paths which currently have 0% coverage
 * Targets: 
 *   - stbi_load_gif_from_memory (Target: 4 undiscovered branches)
 *   - stbi_zlib_decode_malloc_guesssize (Target: 6 undiscovered branches)
 *   - stbi_zlib_decode_malloc (Target: 6 undiscovered branches)
 *   - stbi_zlib_decode_noheader_malloc (Target: 6 undiscovered branches)
 *   - stbi_is_hdr (Target: 2 undiscovered branches)
 *   - stbi_zlib_decode_buffer (Target: 2 undiscovered branches)
 *   - stbi_zlib_decode_noheader_buffer (Target: 2 undiscovered branches)
 * Required Helpers: stbi_image_free (Cleanup)
 * Strategy: Test GIF loading from memory, various zlib decompression approaches, and HDR detection
 *           Use FuzzedDataProvider to split input for different API calls
 * Differentiation from previous harnesses (harness_000 to harness_008):
 *   - harness_001: Basic image loading via GLTF (PNG/JPG focus)
 *   - harness_002: Image writing callbacks
 *   - harness_004: File loading callbacks
 *   - harness_006: Complete image processing pipeline
 *   - harness_007: Direct STB image loading/writing (no GIF/zlib)
 *   - harness_008: 16-bit image loading APIs
 *   - harness_009: FOCUSED GIF loading and zlib decompression APIs (completely uncovered)
 */



// Include STB headers (implementation already built via tinygltf)
// #define STB_IMAGE_IMPLEMENTATION

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    // We need enough data for: GIF test + zlib test + HDR test + parameters
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input into parts for different tests
    // 1. GIF test data
    size_t gif_data_size = 0;
    if (fdp.remaining_bytes() >= 50) {
        gif_data_size = fdp.ConsumeIntegralInRange<size_t>(50, std::min(fdp.remaining_bytes(), size_t(2048)));
    }
    std::vector<uint8_t> gif_data = fdp.ConsumeBytes<uint8_t>(gif_data_size);
    
    // 2. zlib test data (compressed data for decompression)
    size_t zlib_data_size = 0;
    if (fdp.remaining_bytes() >= 50) {
        zlib_data_size = fdp.ConsumeIntegralInRange<size_t>(50, std::min(fdp.remaining_bytes(), size_t(2048)));
    }
    std::vector<uint8_t> zlib_data = fdp.ConsumeBytes<uint8_t>(zlib_data_size);
    
    // 3. HDR test data
    size_t hdr_data_size = 0;
    if (fdp.remaining_bytes() >= 50) {
        hdr_data_size = fdp.ConsumeIntegralInRange<size_t>(50, std::min(fdp.remaining_bytes(), size_t(1024)));
    }
    std::vector<uint8_t> hdr_data = fdp.ConsumeBytes<uint8_t>(hdr_data_size);
    // 4. Parameters for API calls
    int req_comp = 0, initial_size = 1024, buffer_size = 128; // Default values
    if (fdp.remaining_bytes() >= sizeof(int) * 3) {
        req_comp = fdp.ConsumeIntegralInRange<int>(0, 4); // 0 = original channels
        initial_size = fdp.ConsumeIntegralInRange<int>(1024, 65536);
        buffer_size = fdp.ConsumeIntegralInRange<int>(128, 8192);
    }
    // Track all allocated memory for cleanup
    std::vector<unsigned char*> image_pointers;
    std::vector<char*> zlib_pointers;
    // ==================== TEST 1: GIF LOADING ====================
    // Test stbi_load_gif_from_memory - expects GIF format data
    // Even with random bytes, we exercise error handling paths
    if (!gif_data.empty()) {
        int width = 0, height = 0, frames = 0, comp = 0;
        int* delays = nullptr;
        
        unsigned char* gif_result = stbi_load_gif_from_memory(
            gif_data.data(), static_cast<int>(gif_data.size()),
            &delays, &width, &height, &frames, &comp, req_comp);
        
        if (gif_result) {
            image_pointers.push_back(gif_result);
            
            // If delays were allocated, free them
            if (delays) {
                free(delays);
            }
        }
    }
    
    // ==================== TEST 2: ZLIB DECOMPRESSION ====================
    // Test various zlib decompression APIs
    if (!zlib_data.empty()) {
        int outlen = 0;
        
        // 2a: stbi_zlib_decode_malloc_guesssize
        char* zlib_result1 = stbi_zlib_decode_malloc_guesssize(
            reinterpret_cast<const char*>(zlib_data.data()), 
            static_cast<int>(zlib_data.size()),
            initial_size, &outlen);
        
        if (zlib_result1) {
            zlib_pointers.push_back(zlib_result1);
        }
        
        // 2b: stbi_zlib_decode_malloc (uses default 16384 initial size)
        char* zlib_result2 = stbi_zlib_decode_malloc(
            reinterpret_cast<const char*>(zlib_data.data()),
            static_cast<int>(zlib_data.size()), &outlen);
        
        if (zlib_result2) {
            zlib_pointers.push_back(zlib_result2);
        }
        
        // 2c: stbi_zlib_decode_noheader_malloc
        char* zlib_result3 = stbi_zlib_decode_noheader_malloc(
            reinterpret_cast<const char*>(zlib_data.data()),
            static_cast<int>(zlib_data.size()), &outlen);
        
        if (zlib_result3) {
            zlib_pointers.push_back(zlib_result3);
        }
        
        // 2d: stbi_zlib_decode_buffer (buffer-based decompression)
        std::vector<char> output_buffer(buffer_size);
        int buffer_result = stbi_zlib_decode_buffer(
            output_buffer.data(), buffer_size,
            reinterpret_cast<const char*>(zlib_data.data()),
            static_cast<int>(zlib_data.size()));
        
        // Return value is decompressed length or -1 on error
        // No allocation, so nothing to free
        
        // 2e: stbi_zlib_decode_noheader_buffer
        int noheader_result = stbi_zlib_decode_noheader_buffer(
            output_buffer.data(), buffer_size,
            reinterpret_cast<const char*>(zlib_data.data()),
            static_cast<int>(zlib_data.size()));
        
        // Return value is decompressed length or -1 on error
    }
    
    // ==================== TEST 3: HDR DETECTION ====================
    // Test stbi_is_hdr_from_memory (stbi_is_hdr is for files, use memory version)
    if (!hdr_data.empty()) {
        int is_hdr = stbi_is_hdr_from_memory(hdr_data.data(), static_cast<int>(hdr_data.size()));
        // Result is 1 if HDR, 0 if not HDR or error
        // No allocation needed
    }
    
    // ==================== CLEANUP ====================
    // Free all allocated image memory using stbi_image_free
    for (auto* img_ptr : image_pointers) {
        stbi_image_free(img_ptr);
    }
    
    // Free zlib allocated memory using free()
    for (auto* zlib_ptr : zlib_pointers) {
        free(zlib_ptr);
    }
    
    return 0;
}

} // namespace harness_009
