#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "stb_image.h"

namespace harness_011 {
/*
 * Fuzzing harness for tinygltf library targeting comprehensive stb_image loading APIs
 * Goal: Exercise comprehensive image loading functionality with callback-based and memory-based APIs
 * Targets: 
 *   - stbi_load (file-based 8-bit loading - 12 branches, 9 undiscovered)
 *   - stbi_loadf (HDR image loading - 10 branches, 7 undiscovered)
 *   - stbi_load_16 (16-bit image loading - 12 branches, 9 undiscovered)
 *   - stbi_load_from_callbacks (callback-based loading - 6 branches, 3 undiscovered)
 * Required Helper: stbi_image_free (cleanup)
 * Strategy: Create synthetic/corrupted image data, implement custom I/O callbacks, test all loading APIs
 *           with both valid and invalid image data to exercise error handling paths
 * Differentiation from existing harnesses (harness_000 to harness_010):
 *   - harness_001: Image loading via GLTF embedded images (indirect API usage)
 *   - harness_007: Direct STB loading but no callback-based APIs
 *   - harness_008: 16-bit APIs only, limited callback testing
 *   - harness_009: GIF/zlib specific APIs
 *   - harness_011: COMPREHENSIVE stb_image loading APIs with custom callbacks and all formats
 */



// Include STB headers (implementation already built via tinygltf)
// #define STB_IMAGE_IMPLEMENTATION

// Simple callback context for simulated I/O operations
struct CallbackContext {
    FuzzedDataProvider* fdp;
    const uint8_t* data_ptr;
    size_t data_size;
    size_t position;
    bool is_valid;  // Simulate valid/invalid I/O operations
};

// Read callback for stbi_io_callbacks
static int custom_read(void* user, char* data, int size) {
    CallbackContext* ctx = static_cast<CallbackContext*>(user);
    
    if (!ctx || !ctx->is_valid || size <= 0) {
        return 0;
    }
    
    // Calculate how many bytes we can read
    size_t remaining = ctx->data_size - ctx->position;
    size_t to_read = std::min<size_t>(remaining, size);
    
    if (to_read == 0) {
        return 0;  // EOF
    }
    
    // Copy data from our buffer
    if (ctx->data_ptr) {
        memcpy(data, ctx->data_ptr + ctx->position, to_read);
        ctx->position += to_read;
    } else if (ctx->fdp) {
        // Consume from fuzzer data if no static buffer
        auto bytes = ctx->fdp->ConsumeBytes<uint8_t>(to_read);
        memcpy(data, bytes.data(), bytes.size());
        to_read = bytes.size();
    }
    
    return static_cast<int>(to_read);
}

// Skip callback for stbi_io_callbacks
static void custom_skip(void* user, int n) {
    CallbackContext* ctx = static_cast<CallbackContext*>(user);
    
    if (!ctx || !ctx->is_valid) {
        return;
    }
    
    if (n > 0) {
        // Skip forward
        size_t remaining = ctx->data_size - ctx->position;
        size_t to_skip = std::min<size_t>(remaining, n);
        ctx->position += to_skip;
        
        // If using fuzzer data, consume bytes
        if (ctx->fdp && to_skip > 0) {
            ctx->fdp->ConsumeBytes<uint8_t>(to_skip);
        }
    } else if (n < 0) {
        // Skip backward (unget) - limited to what we've already read
        size_t to_back = std::min<size_t>(ctx->position, -n);
        ctx->position -= to_back;
    }
}

// EOF callback for stbi_io_callbacks
static int custom_eof(void* user) {
    CallbackContext* ctx = static_cast<CallbackContext*>(user);
    
    if (!ctx || !ctx->is_valid) {
        return 1;  // EOF on invalid context
    }
    
    bool eof = false;
    if (ctx->data_ptr) {
        eof = (ctx->position >= ctx->data_size);
    } else if (ctx->fdp) {
        eof = (ctx->fdp->remaining_bytes() == 0);
    }
    
    return eof ? 1 : 0;
}

// Generate synthetic image header for various formats
static std::vector<uint8_t> generate_image_header(FuzzedDataProvider& fdp, int format_type) {
    std::vector<uint8_t> header;
    
    switch (format_type % 5) {
        case 0: { // PNG-like header
            header = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}; // PNG signature
            break;
        }
        case 1: { // JPEG-like header
            header = {0xFF, 0xD8, 0xFF}; // JPEG SOI marker
            break;
        }
        case 2: { // BMP-like header
            header = {'B', 'M'}; // BMP signature
            break;
        }
        case 3: { // HDR-like header
            header = {'#', '?', 'R', 'A', 'D', 'I', 'A', 'N', 'C', 'E'};
            break;
        }
        case 4: { // GIF-like header
            header = {'G', 'I', 'F', '8', '9', 'a'};
            break;
        }
    }
    
    // Add some random data to make it look more like an image
    size_t extra_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
    if (extra_size > 0 && fdp.remaining_bytes() >= extra_size) {
        auto extra_data = fdp.ConsumeBytes<uint8_t>(extra_size);
        header.insert(header.end(), extra_data.begin(), extra_data.end());
    }
    
    return header;
}

// Test stbi_load (file-based 8-bit loading)
static void test_stbi_load(FuzzedDataProvider& fdp, const std::vector<uint8_t>& image_data) {
    if (image_data.empty()) return;
    
    // Create a temporary file (in memory simulation)
    FILE* tmpf = tmpfile();
    if (!tmpf) return;
    
    fwrite(image_data.data(), 1, image_data.size(), tmpf);
    rewind(tmpf);
    
    int width = 0, height = 0, channels = 0;
    int desired_channels = fdp.ConsumeIntegralInRange<int>(0, 4); // 0 = original channels
    
    // Try to load the image
    unsigned char* loaded_data = stbi_load_from_file(tmpf, &width, &height, &channels, desired_channels);
    
    if (loaded_data) {
        stbi_image_free(loaded_data);
    }
    
    fclose(tmpf);
}

// Test stbi_loadf (HDR image loading)
static void test_stbi_loadf(FuzzedDataProvider& fdp, const std::vector<uint8_t>& image_data) {
    if (image_data.empty()) return;
    
    // Create a temporary file for HDR loading
    FILE* tmpf = tmpfile();
    if (!tmpf) return;
    
    fwrite(image_data.data(), 1, image_data.size(), tmpf);
    rewind(tmpf);
    
    int width = 0, height = 0, channels = 0;
    int desired_channels = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Try to load as HDR
    float* loaded_data = stbi_loadf_from_file(tmpf, &width, &height, &channels, desired_channels);
    
    if (loaded_data) {
        stbi_image_free(loaded_data);
    }
    
    fclose(tmpf);
}

// Test stbi_load_16 (16-bit image loading)
static void test_stbi_load_16(FuzzedDataProvider& fdp, const std::vector<uint8_t>& image_data) {
    if (image_data.empty()) return;
    
    // Create a temporary file for 16-bit loading
    FILE* tmpf = tmpfile();
    if (!tmpf) return;
    
    fwrite(image_data.data(), 1, image_data.size(), tmpf);
    rewind(tmpf);
    
    int width = 0, height = 0, channels = 0;
    int desired_channels = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Try to load as 16-bit
    unsigned short* loaded_data = stbi_load_from_file_16(tmpf, &width, &height, &channels, desired_channels);
    
    if (loaded_data) {
        stbi_image_free(loaded_data);
    }
    
    fclose(tmpf);
}

// Test stbi_load_from_callbacks (callback-based loading)
static void test_stbi_load_from_callbacks(FuzzedDataProvider& fdp, const std::vector<uint8_t>& image_data) {
    if (image_data.empty()) return;
    
    // Setup callback context
    CallbackContext ctx;
    ctx.data_ptr = image_data.data();
    ctx.data_size = image_data.size();
    ctx.position = 0;
    ctx.fdp = nullptr;  // Using static data
    ctx.is_valid = true;
    
    // Setup callbacks
    stbi_io_callbacks callbacks;
    callbacks.read = custom_read;
    callbacks.skip = custom_skip;
    callbacks.eof = custom_eof;
    
    int width = 0, height = 0, channels = 0;
    int desired_channels = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Try different callback-based loading functions
    uint8_t api_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    void* loaded_data = nullptr;
    
    switch (api_choice) {
        case 0: // 8-bit loading
            loaded_data = stbi_load_from_callbacks(&callbacks, &ctx, &width, &height, &channels, desired_channels);
            break;
        case 1: // 16-bit loading
            loaded_data = stbi_load_16_from_callbacks(&callbacks, &ctx, &width, &height, &channels, desired_channels);
            break;
        case 2: // HDR loading
            loaded_data = stbi_loadf_from_callbacks(&callbacks, &ctx, &width, &height, &channels, desired_channels);
            break;
    }
    
    if (loaded_data) {
        stbi_image_free(loaded_data);
    }
}

// Test memory-based loading APIs
static void test_memory_based_loading(FuzzedDataProvider& fdp, const std::vector<uint8_t>& image_data) {
    if (image_data.empty()) return;
    
    int width = 0, height = 0, channels = 0;
    int desired_channels = fdp.ConsumeIntegralInRange<int>(0, 4);
    uint8_t api_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    void* loaded_data = nullptr;
    
    switch (api_choice) {
        case 0: // 8-bit from memory
            loaded_data = stbi_load_from_memory(image_data.data(), static_cast<int>(image_data.size()),
                                               &width, &height, &channels, desired_channels);
            break;
        case 1: // 16-bit from memory
            loaded_data = stbi_load_16_from_memory(image_data.data(), static_cast<int>(image_data.size()),
                                                  &width, &height, &channels, desired_channels);
            break;
        case 2: // HDR from memory
            loaded_data = stbi_loadf_from_memory(image_data.data(), static_cast<int>(image_data.size()),
                                                &width, &height, &channels, desired_channels);
            break;
    }
    
    if (loaded_data) {
        stbi_image_free(loaded_data);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input: part for image data generation, part for API parameters
    size_t image_data_size = fdp.ConsumeIntegralInRange<size_t>(50, std::min(size - 50, size_t(4096)));
    
    // Generate synthetic image data with proper header
    int format_type = fdp.ConsumeIntegralInRange<int>(0, 4);
    auto image_header = generate_image_header(fdp, format_type);
    
    // Get remaining bytes for image body
    size_t body_size = std::min(image_data_size - image_header.size(), fdp.remaining_bytes());
    auto image_body = fdp.ConsumeBytes<uint8_t>(body_size);
    
    // Combine header and body
    std::vector<uint8_t> image_data;
    image_data.reserve(image_header.size() + image_body.size());
    image_data.insert(image_data.end(), image_header.begin(), image_header.end());
    image_data.insert(image_data.end(), image_body.begin(), image_body.end());
    
    if (image_data.empty()) {
        return 0;
    }
    
    // Choose which API to test based on remaining input
    uint8_t test_selection = 0;
    if (fdp.remaining_bytes() > 0) {
        test_selection = fdp.ConsumeIntegral<uint8_t>() % 5;
    }
    
    switch (test_selection) {
        case 0:
            test_stbi_load(fdp, image_data);
            break;
        case 1:
            test_stbi_loadf(fdp, image_data);
            break;
        case 2:
            test_stbi_load_16(fdp, image_data);
            break;
        case 3:
            test_stbi_load_from_callbacks(fdp, image_data);
            break;
        case 4:
            test_memory_based_loading(fdp, image_data);
            break;
    }
    
    return 0;
}

} // namespace harness_011
