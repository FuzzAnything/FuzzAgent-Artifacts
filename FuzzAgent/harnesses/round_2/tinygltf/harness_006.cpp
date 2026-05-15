#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>
#include <fuzzer/FuzzedDataProvider.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

namespace harness_006 {
// Harness 006: Targeted stb_image loading fuzzer for tinygltf
// Targets: stbi_load, stbi_load_16, stbi_loadf, stbi_info, stbi_is_16_bit through LoadImageData
// Goal: Exercise stb_image.h with valid image formats to achieve 0%->20%+ coverage
// Creates minimal GLTF 2.0 JSON with embedded base64-encoded images in multiple formats


// Include FuzzedDataProvider for structured input splitting

// Define implementations for stb_image and tinygltf


// Helper function to create minimal valid image data with proper signatures
std::vector<uint8_t> create_image_with_signature(FuzzedDataProvider& fdp, uint8_t format_type, bool is_16bit, bool is_hdr) {
    std::vector<uint8_t> image_data;
    
    // Determine base size based on format
    size_t base_size = fdp.ConsumeIntegralInRange<size_t>(32, 4096);
    
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
        // case 5: Unknown/corrupted format (keep as-is)
    }
    
    return image_data;
}

// Base64 encoding function (simplified for fuzzing)
std::string base64_encode_simple(const std::vector<uint8_t>& data) {
    const char base64_chars[] = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    
    std::string encoded;
    size_t in_len = data.size();
    const uint8_t* bytes_to_encode = data.data();
    
    int i = 0;
    int j = 0;
    uint8_t char_array_3[3];
    uint8_t char_array_4[4];
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for (i = 0; i < 4; i++) {
                encoded += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; j < i + 1; j++) {
            encoded += base64_chars[char_array_4[j]];
        }
        
        while (i++ < 3) {
            encoded += '=';
        }
    }
    
    return encoded;
}

// Create minimal GLTF 2.0 JSON with embedded base64 image
std::string create_gltf_json_with_image(const std::string& mime_type, const std::string& base64_data, 
                                        int width, int height, bool use_buffer_view) {
    std::string gltf_json;
    
    if (use_buffer_view) {
        // Use bufferView reference (for binary glTF)
        gltf_json = R"({
    "asset": {"version": "2.0"},
    "images": [{"bufferView": 0, "mimeType": ")" + mime_type + R"("}],
    "bufferViews": [{"buffer": 0, "byteOffset": 0, "byteLength": )" + std::to_string(base64_data.size() * 3 / 4) + R"(}],
    "buffers": [{"byteLength": )" + std::to_string(base64_data.size() * 3 / 4) + R"(}]
})";
    } else {
        // Use data URI with base64
        std::string data_uri = "data:" + mime_type + ";base64," + base64_data;
        gltf_json = R"({
    "asset": {"version": "2.0"},
    "images": [{"uri": ")" + data_uri + R"("}],
    "textures": [{"source": 0}],
    "materials": [{"pbrMetallicRoughness": {"baseColorTexture": {"index": 0}}}],
    "meshes": [{"primitives": [{"attributes": {"POSITION": 0}, "indices": 0}]}],
    "accessors": [{"bufferView": 0, "componentType": 5126, "count": 3, "type": "VEC3"}],
    "bufferViews": [{"buffer": 0, "byteOffset": 0, "byteLength": 36}],
    "buffers": [{"uri": "data:application/octet-stream;base64,AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=", "byteLength": 36}]
})";
    }
    
    return gltf_json;
}

// Get MIME type string for format
std::string get_mime_type(uint8_t format_type) {
    switch (format_type) {
        case 0: return "image/png";
        case 1: return "image/jpeg";
        case 2: return "image/bmp";
        case 3: return "image/gif";
        case 4: return "image/vnd.radiance"; // HDR
        default: return "image/png";
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for meaningful testing
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test configuration
    uint8_t format_type = fdp.ConsumeIntegral<uint8_t>() % 6; // 0-5 formats (5 = unknown)
    bool test_16bit = fdp.ConsumeBool();
    bool test_hdr = fdp.ConsumeBool();
    bool use_buffer_view = fdp.ConsumeBool();
    uint8_t load_option_flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Create image data with proper signature
    std::vector<uint8_t> image_data = create_image_with_signature(fdp, format_type, test_16bit, test_hdr);
    
    if (image_data.empty() || image_data.size() < 16) {
        return 0; // Not enough data for meaningful test
    }
    
    // Encode image data as base64
    std::string base64_encoded = base64_encode_simple(image_data);
    if (base64_encoded.empty()) {
        return 0;
    }
    
    // Get MIME type for the format
    std::string mime_type = get_mime_type(format_type);
    
    // Create GLTF JSON
    std::string gltf_json = create_gltf_json_with_image(mime_type, base64_encoded, 0, 0, use_buffer_view);
    
    // Initialize TinyGLTF context
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    bool load_result = false;
    
    if (use_buffer_view) {
        // For bufferView reference, we need to load as binary glTF
        // Create binary glTF with image data in buffer
        std::vector<uint8_t> binary_data = image_data;
        
        // We'll simulate binary glTF loading by calling LoadImageData directly
        // since creating a full binary glTF is complex
        tinygltf::Image test_image;
        test_image.name = "test_image_format_" + std::to_string(format_type);
        
        // Configure LoadImageData options
        tinygltf::LoadImageDataOption option;
        option.as_is = (load_option_flags & 0x1) != 0;
        option.preserve_channels = (load_option_flags & 0x2) != 0;
        
        // Direct LoadImageData call to exercise stb_image functions
        bool image_result = tinygltf::LoadImageData(&test_image, 0, &err, &warn,
                                                   0, 0, image_data.data(),
                                                   static_cast<int>(image_data.size()),
                                                   &option);
        (void)image_result;
        
        // Also test without options
        if (fdp.ConsumeBool()) {
            tinygltf::Image test_image2;
            test_image2.name = "test_image_no_options";
            bool image_result2 = tinygltf::LoadImageData(&test_image2, 0, &err, &warn,
                                                       0, 0, image_data.data(),
                                                       static_cast<int>(image_data.size()),
                                                       nullptr);
            (void)image_result2;
        }
        
        // Test with width/height requirements
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 8) {
            tinygltf::Image test_image3;
            test_image3.name = "test_image_with_size";
            int req_width = fdp.ConsumeIntegralInRange<int>(1, 2048);
            int req_height = fdp.ConsumeIntegralInRange<int>(1, 2048);
            
            bool image_result3 = tinygltf::LoadImageData(&test_image3, 0, &err, &warn,
                                                       req_width, req_height, image_data.data(),
                                                       static_cast<int>(image_data.size()),
                                                       nullptr);
            (void)image_result3;
        }
    } else {
        // Load as ASCII glTF with data URI
        load_result = ctx.LoadASCIIFromString(&model, &err, &warn,
                                             gltf_json.c_str(),
                                             static_cast<unsigned int>(gltf_json.size()),
                                             "");
        (void)load_result;
    }
    
    // Additional direct stb_image function testing
    // Test various stb_image functions directly if we have enough data
    if (fdp.remaining_bytes() >= 32) {
        std::vector<uint8_t> test_img = fdp.ConsumeBytes<uint8_t>(
            fdp.ConsumeIntegralInRange<size_t>(32, fdp.remaining_bytes())
        );
        
        if (test_img.size() >= 32) {
            // Test stbi_info_from_memory
            int w = 0, h = 0, comp = 0;
            bool info_result = stbi_info_from_memory(test_img.data(), 
                                                     static_cast<int>(test_img.size()), 
                                                     &w, &h, &comp);
            (void)info_result;
            
            // Test stbi_is_16_bit_from_memory
            bool is_16bit = stbi_is_16_bit_from_memory(test_img.data(),
                                                      static_cast<int>(test_img.size()));
            (void)is_16bit;
            
            // Try to load as 8-bit
            unsigned char* loaded_8bit = stbi_load_from_memory(test_img.data(),
                                                              static_cast<int>(test_img.size()),
                                                              &w, &h, &comp, 0);
            if (loaded_8bit) {
                stbi_image_free(loaded_8bit);
            }
            // Try to load as 16-bit if image might be 16-bit
            if (is_16bit || test_16bit) {
                unsigned short* loaded_16bit = stbi_load_16_from_memory(test_img.data(),
                                                                       static_cast<int>(test_img.size()),
                                                                       &w, &h, &comp, 0);
                if (loaded_16bit) {
                    stbi_image_free(loaded_16bit);
                }
            }
            
            // Try to load as float for HDR
            if (test_hdr || format_type == 4) {
                float* loaded_float = stbi_loadf_from_memory(test_img.data(),
                                                            static_cast<int>(test_img.size()),
                                                            &w, &h, &comp, 0);
                if (loaded_float) {
                    stbi_image_free(loaded_float);
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_006
