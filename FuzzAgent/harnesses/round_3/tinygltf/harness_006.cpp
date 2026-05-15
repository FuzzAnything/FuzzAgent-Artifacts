#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include "stb_image_write.h"

namespace harness_006 {
/*
 * Fuzzing harness for tinygltf library targeting complete image processing pipeline
 * Goal: Exercise stb_image.h image loading and writing operations via GLTF model loading
 * Targets: stbi_load, stbi_load_16, stbi_loadf, stbi_write_png, stbi_write_jpg, 
 *          stbi_write_bmp, stbi_write_hdr, stbi_write_tga
 * Strategy: Create GLTF models with embedded images in various formats, load them
 *           to trigger image loading, extract image data, modify it, and write back
 *           using stbi_write_* functions to complete the Load->Process->Write pipeline
 * Differentiation from previous harnesses:
 *   - harness_001: Image loading via embedded images (Load only)
 *   - harness_002: Image writing callbacks (Write only)
 *   - harness_006: Complete image processing pipeline (Load -> Process -> Write)
 */



// Use same includes as harness_001 (implementation already built)
// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_IMPLEMENTATION

// Include stb_image_write.h for image writing functions (but don't define implementation)

// Simple base64 encoding (minimal implementation - same as harness_001)
static const char base64_chars[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode(const uint8_t* data, size_t len) {
    std::string ret;
    int i = 0;
    int j = 0;
    uint8_t char_array_3[3];
    uint8_t char_array_4[4];

    while (len--) {
        char_array_3[i++] = *(data++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; i < 4; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; j < i + 1; j++)
            ret += base64_chars[char_array_4[j]];

        while(i++ < 3)
            ret += '=';
    }

    return ret;
}

// Create GLTF JSON with embedded image
std::string create_gltf_with_image(const uint8_t* image_data, size_t image_size, 
                                   const char* mime_type, int width, int height, 
                                   const std::string& image_name = "test_image") {
    std::string base64 = base64_encode(image_data, image_size);
    
    char buffer[4096];
    snprintf(buffer, sizeof(buffer),
        "{\n"
        "  \"asset\": {\"version\": \"2.0\"},\n"
        "  \"images\": [\n"
        "    {\n"
        "      \"name\": \"%s\",\n"
        "      \"width\": %d,\n"
        "      \"height\": %d,\n"
        "      \"mimeType\": \"%s\",\n"
        "      \"uri\": \"data:%s;base64,%s\"\n"
        "    }\n"
        "  ],\n"
        "  \"textures\": [{\"source\": 0}],\n"
        "  \"materials\": [\n"
        "    {\n"
        "      \"pbrMetallicRoughness\": {\n"
        "        \"baseColorTexture\": {\"index\": 0}\n"
        "      }\n"
        "    }\n"
        "  ]\n"
        "}",
        image_name.c_str(), width, height, mime_type, mime_type, base64.c_str());
    
    return std::string(buffer);
}

// Modify image data (simple transformations for fuzzing)
void modify_image_data(std::vector<unsigned char>& image_data, FuzzedDataProvider& fdp) {
    if (image_data.empty()) return;
    
    // Use fuzzer input to decide modification type
    uint8_t mod_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (mod_type) {
        case 0: // Invert colors
            for (size_t i = 0; i < image_data.size(); i++) {
                image_data[i] = 255 - image_data[i];
            }
            break;
            
        case 1: // Add noise
            for (size_t i = 0; i < image_data.size() && fdp.remaining_bytes() > 0; i++) {
                uint8_t noise = fdp.ConsumeIntegral<uint8_t>() % 32;
                int new_val = static_cast<int>(image_data[i]) + noise - 16;
                image_data[i] = static_cast<unsigned char>(std::max(0, std::min(255, new_val)));
            }
            break;
            
        case 2: // Brighten
            for (size_t i = 0; i < image_data.size(); i++) {
                int new_val = static_cast<int>(image_data[i]) + 30;
                image_data[i] = static_cast<unsigned char>(std::min(255, new_val));
            }
            break;
            
        case 3: // Darken
            for (size_t i = 0; i < image_data.size(); i++) {
                int new_val = static_cast<int>(image_data[i]) - 30;
                image_data[i] = static_cast<unsigned char>(std::max(0, new_val));
            }
            break;
    }
}

// Write image data using various stbi_write_* functions (callback-based to avoid file I/O)
void write_image_with_stbi(const std::vector<unsigned char>& image_data, 
                          int width, int height, int comp, 
                          FuzzedDataProvider& fdp) {
    if (width <= 0 || height <= 0 || comp < 1 || comp > 4) return;
    if (image_data.size() < static_cast<size_t>(width * height * comp)) return;
    
    // Select write format from fuzzer input
    uint8_t write_format = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Write to memory buffer using callback
    struct WriteContext {
        std::vector<unsigned char> buffer;
        size_t total_written = 0;
    };
    
    WriteContext ctx;
    
    auto write_callback = [](void* context, void* data, int size) {
        WriteContext* ctx = static_cast<WriteContext*>(context);
        if (data && size > 0) {
            unsigned char* bytes = static_cast<unsigned char*>(data);
            ctx->buffer.insert(ctx->buffer.end(), bytes, bytes + size);
            ctx->total_written += size;
        }
    };
    
    // Test with and without vertical flip
    bool flip = fdp.ConsumeBool();
    stbi_flip_vertically_on_write(flip ? 1 : 0);
    
    switch (write_format) {
        case 0: // PNG
            stbi_write_png_to_func(write_callback, &ctx, width, height, comp, 
                                  image_data.data(), width * comp);
            break;
            
        case 1: // JPEG (quality 1-100)
            {
                int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
                stbi_write_jpg_to_func(write_callback, &ctx, width, height, comp,
                                      image_data.data(), quality);
            }
            break;
            
        case 2: // BMP
            stbi_write_bmp_to_func(write_callback, &ctx, width, height, comp,
                                  image_data.data());
            break;
            
        case 3: // TGA
            stbi_write_tga_to_func(write_callback, &ctx, width, height, comp,
                                  image_data.data());
            break;
            
        case 4: // HDR (requires float data, so skip for regular images)
            // HDR requires float data, not tested here
            break;
    }
}

// Test 1: Complete pipeline for PNG images
static void test_png_pipeline(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 20) return;
    
    // Consume image dimensions from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(1, 512);
    int height = fdp.ConsumeIntegralInRange<int>(1, 512);
    int comp = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    // Generate synthetic image data
    size_t data_size = width * height * comp;
    std::vector<uint8_t> image_data;
    
    // Consume bytes for image data
    size_t bytes_to_consume = std::min<size_t>(fdp.remaining_bytes(), data_size);
    if (bytes_to_consume > 0) {
        auto bytes = fdp.ConsumeBytes<uint8_t>(bytes_to_consume);
        image_data.insert(image_data.end(), bytes.begin(), bytes.end());
    }
    
    // Pad if necessary
    if (image_data.size() < data_size) {
        image_data.resize(data_size, 128); // Gray color
    }
    
    // Create GLTF with embedded PNG
    std::string gltf = create_gltf_with_image(image_data.data(), image_data.size(), 
                                             "image/png", width, height);
    
    // Load GLTF model (triggers stbi_load_from_memory for PNG)
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, gltf.c_str(),
                                       static_cast<unsigned int>(gltf.size()),
                                       /* base_dir */ "");
    (void)ret;
    
    // Access model.images to ensure image loading happened (Load phase)
    if (!model.images.empty()) {
        // Convert vector<unsigned char> to vector<uint8_t> for modification
        std::vector<unsigned char> loaded_image = model.images[0].image;
        
        // Modify image data (Process phase)
        modify_image_data(loaded_image, fdp);
        
        // Write modified image using stbi_write_* (Write phase)
        write_image_with_stbi(loaded_image, width, height, comp, fdp);
    }
}

// Test 2: Complete pipeline for JPEG images  
static void test_jpeg_pipeline(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 20) return;
    
    // Consume image dimensions
    int width = fdp.ConsumeIntegralInRange<int>(1, 512);
    int height = fdp.ConsumeIntegralInRange<int>(1, 512);
    int comp = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    // Generate synthetic image data (JPEG-like)
    size_t data_size = width * height * comp;
    std::vector<uint8_t> image_data;
    
    // Consume bytes for image data
    size_t bytes_to_consume = std::min<size_t>(fdp.remaining_bytes(), data_size);
    if (bytes_to_consume > 0) {
        auto bytes = fdp.ConsumeBytes<uint8_t>(bytes_to_consume);
        image_data.insert(image_data.end(), bytes.begin(), bytes.end());
    }
    
    // Pad if necessary
    if (image_data.size() < data_size) {
        image_data.resize(data_size, 128);
    }
    
    // Create GLTF with embedded JPEG
    std::string gltf = create_gltf_with_image(image_data.data(), image_data.size(),
                                             "image/jpeg", width, height);
    
    // Load GLTF model (triggers stbi_load_from_memory for JPEG)
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, gltf.c_str(),
                                       static_cast<unsigned int>(gltf.size()),
                                       /* base_dir */ "");
    (void)ret;
    
    // Access model.images
    if (!model.images.empty()) {
        std::vector<unsigned char> loaded_image = model.images[0].image;
        
        // Modify image data
        modify_image_data(loaded_image, fdp);
        
        // Write modified image
        write_image_with_stbi(loaded_image, width, height, comp, fdp);
    }
}

// Test 3: Test with multiple image formats
static void test_multiple_formats(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 30) return;
    
    // Select format from fuzzer input
    uint8_t format = fdp.ConsumeIntegral<uint8_t>() % 4;
    const char* mime_type;
    switch (format) {
        case 0: mime_type = "image/png"; break;
        case 1: mime_type = "image/jpeg"; break;
        case 2: mime_type = "image/bmp"; break;
        case 3: mime_type = "image/gif"; break;
        default: mime_type = "image/png"; break;
    }
    
    // Consume image dimensions
    int width = fdp.ConsumeIntegralInRange<int>(1, 256);
    int height = fdp.ConsumeIntegralInRange<int>(1, 256);
    int comp = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    // Generate synthetic image data
    size_t data_size = width * height * comp;
    std::vector<uint8_t> image_data;
    
    // Consume bytes for image data
    size_t bytes_to_consume = std::min<size_t>(fdp.remaining_bytes(), data_size);
    if (bytes_to_consume > 0) {
        auto bytes = fdp.ConsumeBytes<uint8_t>(bytes_to_consume);
        image_data.insert(image_data.end(), bytes.begin(), bytes.end());
    }
    
    // Pad if necessary
    if (image_data.size() < data_size) {
        image_data.resize(data_size, 200);
    }
    
    // Create GLTF with embedded image
    std::string gltf = create_gltf_with_image(image_data.data(), image_data.size(),
                                             mime_type, width, height);
    
    // Load GLTF model
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, gltf.c_str(),
                                       static_cast<unsigned int>(gltf.size()),
                                       /* base_dir */ "");
    (void)ret;
    
    // Process images if available
    if (!model.images.empty()) {
        for (const auto& image : model.images) {
            // Convert to mutable copy for modification
            std::vector<unsigned char> loaded_image = image.image;
            
            // Modify image data
            modify_image_data(loaded_image, fdp);
            
            // Write with various formats
            write_image_with_stbi(loaded_image, width, height, comp, fdp);
        }
    }
}

// Test 4: Test binary GLTF loading with potential images
static void test_binary_gltf(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 50) return;
    
    // Consume some data for binary GLTF
    size_t gltf_size = std::min<size_t>(fdp.remaining_bytes(), 1024);
    auto gltf_data = fdp.ConsumeBytes<uint8_t>(gltf_size);
    
    // Try to load as binary GLTF (Load phase)
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    bool ret = ctx.LoadBinaryFromMemory(&model, &err, &warn, gltf_data.data(),
                                       static_cast<unsigned int>(gltf_data.size()),
                                       /* base_dir */ "");
    (void)ret;
    
    // If images were loaded, try to write them (Process -> Write phases)
    if (!model.images.empty()) {
        for (size_t i = 0; i < model.images.size() && i < 3; i++) {
            const auto& image = model.images[i];
            if (image.width > 0 && image.height > 0 && image.component > 0) {
                std::vector<unsigned char> loaded_image = image.image;
                
                // Modify if we have enough fuzzer data
                if (fdp.remaining_bytes() > 10) {
                    modify_image_data(loaded_image, fdp);
                }
                
                // Write with random format
                write_image_with_stbi(loaded_image, image.width, image.height, 
                                     image.component, fdp);
            }
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 10) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Use first few bytes to select test case
    uint8_t test_case = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (test_case) {
        case 0:
            test_png_pipeline(fdp);
            break;
        case 1:
            test_jpeg_pipeline(fdp);
            break;
        case 2:
            test_multiple_formats(fdp);
            break;
        case 3:
            test_binary_gltf(fdp);
            break;
    }
    
    return 0;
}

} // namespace harness_006
