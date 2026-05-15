#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include "tiny_gltf.h"

namespace harness_001 {
/*
 * Fuzzing harness for tinygltf library targeting stb_image APIs
 * Goal: Exercise image/texture loading functionality via embedded image data
 * Targets: stbi_load, stbi_load_16, stbi_loadf, stbi_info, stbi_image_free
 * Strategy: Create GLTF files with embedded image data to trigger stb_image
 * Differentiation from harness_000: Focuses on image loading paths
 */


// Use same includes as harness_000
// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_IMPLEMENTATION

// Simple base64 encoding (minimal implementation)
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
                                   const char* mime_type, int width, int height) {
    std::string base64 = base64_encode(image_data, image_size);
    
    char buffer[4096];
    snprintf(buffer, sizeof(buffer),
        "{\n"
        "  \"asset\": {\"version\": \"2.0\"},\n"
        "  \"images\": [\n"
        "    {\n"
        "      \"name\": \"test\",\n"
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
        width, height, mime_type, mime_type, base64.c_str());
    
    return std::string(buffer);
}

// Test 1: ASCII GLTF with PNG image data
static void test_ascii_png(const uint8_t* data, size_t size) {
    if (size < 16) return;
    
    // Use first bytes for dimensions
    int width = (data[0] * 256 + data[1]) % 1024 + 1;
    int height = (data[2] * 256 + data[3]) % 1024 + 1;
    
    // Rest as image data
    const uint8_t* image_data = data + 4;
    size_t image_size = size - 4;
    if (image_size == 0) return;
    
    std::string gltf = create_gltf_with_image(image_data, image_size, "image/png", width, height);
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, gltf.c_str(),
                                       static_cast<unsigned int>(gltf.size()),
                                       /* base_dir */ "");
    (void)ret;
}

// Test 2: ASCII GLTF with JPEG image data
static void test_ascii_jpeg(const uint8_t* data, size_t size) {
    if (size < 16) return;
    
    // Use first bytes for dimensions
    int width = (data[0] * 256 + data[1]) % 1024 + 1;
    int height = (data[2] * 256 + data[3]) % 1024 + 1;
    
    // Rest as image data
    const uint8_t* image_data = data + 4;
    size_t image_size = size - 4;
    if (image_size == 0) return;
    
    std::string gltf = create_gltf_with_image(image_data, image_size, "image/jpeg", width, height);
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, gltf.c_str(),
                                       static_cast<unsigned int>(gltf.size()),
                                       /* base_dir */ "");
    (void)ret;
}

// Test 3: Binary GLTF loading (may contain images)
static void test_binary(const uint8_t* data, size_t size) {
    if (size < 16) return;
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    bool ret = ctx.LoadBinaryFromMemory(&model, &err, &warn, data,
                                       static_cast<unsigned int>(size),
                                       /* base_dir */ "");
    (void)ret;
}

// Test 4: Test with various image formats
static void test_various_formats(const uint8_t* data, size_t size) {
    if (size < 20) return;
    
    // Format selector
    int format = data[0] % 4;
    const char* mime_type;
    switch (format) {
        case 0: mime_type = "image/png"; break;
        case 1: mime_type = "image/jpeg"; break;
        case 2: mime_type = "image/bmp"; break;
        case 3: mime_type = "image/gif"; break;
        default: mime_type = "image/png"; break;
    }
    
    // Dimensions
    int width = (data[1] * 256 + data[2]) % 512 + 1;
    int height = (data[3] * 256 + data[4]) % 512 + 1;
    
    // Image data
    const uint8_t* image_data = data + 5;
    size_t image_size = size - 5;
    if (image_size == 0) return;
    
    std::string gltf = create_gltf_with_image(image_data, image_size, mime_type, width, height);
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, gltf.c_str(),
                                       static_cast<unsigned int>(gltf.size()),
                                       /* base_dir */ "");
    (void)ret;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size == 0) return 0;
    
    // Use first byte as selector (similar to harness_000)
    uint8_t selector = data[0] % 4;
    const uint8_t* payload = data + 1;
    size_t payload_size = size - 1;
    
    if (payload_size == 0) return 0;
    
    switch (selector) {
        case 0:
            test_ascii_png(payload, payload_size);
            break;
        case 1:
            test_ascii_jpeg(payload, payload_size);
            break;
        case 2:
            test_binary(payload, payload_size);
            break;
        case 3:
            test_various_formats(payload, payload_size);
            break;
    }
    
    return 0;
}

} // namespace harness_001
