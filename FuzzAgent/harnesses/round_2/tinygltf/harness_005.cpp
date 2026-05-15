#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>
#include <functional>
#include <fuzzer/FuzzedDataProvider.h>
#define TINYGLTF_IMPLEMENTATION
// Define implementations for stb_image and tinygltf
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

namespace harness_005 {
// Harness 005: Advanced image loading fuzzer for tinygltf targeting stb_image.h module
// Targets: LoadImageData with stb_image functions (stbi_load*, stbi_info*, stbi_is_16_bit*, etc.)
// Goal: Exercise stb_image.h which has critically low coverage (0.46% function coverage)
// Uses FuzzedDataProvider for structured input splitting with diverse image scenarios


// Include FuzzedDataProvider for structured input splitting



// Custom image loader callback with configurable behavior
bool custom_stb_image_loader(tinygltf::Image* image, const int image_idx,
                             std::string* err, std::string* warn,
                             int req_width, int req_height,
                             const unsigned char* bytes, int size, void* user_data) {
    // User data contains behavior flags
    uint32_t* flags = reinterpret_cast<uint32_t*>(user_data);
    uint32_t behavior = flags ? *flags : 0;
    
    // Log custom loader usage
    if (warn) {
        *warn += "Custom stb loader called for image[" + std::to_string(image_idx) + "], flags=" + std::to_string(behavior) + "\n";
    }
    
    // Force specific stb_image behaviors based on flags
    if (behavior & 0x1) {
        // Force as_is mode
        tinygltf::LoadImageDataOption option;
        option.as_is = true;
        option.preserve_channels = (behavior & 0x2) != 0;
        return tinygltf::LoadImageData(image, image_idx, err, warn,
                                      req_width, req_height, bytes, size, &option);
    }
    
    // Use default LoadImageData
    return tinygltf::LoadImageData(image, image_idx, err, warn,
                                  req_width, req_height, bytes, size, nullptr);
}

// Helper function to create glTF JSON with various image references
std::string create_gltf_with_image_reference(const std::string& image_uri, int image_width = 0, int image_height = 0) {
    std::string gltf_json;
    
    if (image_width > 0 && image_height > 0) {
        gltf_json = std::string(R"({
            "asset": {"version": "2.0"},
            "images": [{"uri": ")") + image_uri + R"(", "width": )" + std::to_string(image_width) + 
                     R"(, "height": )" + std::to_string(image_height) + R"(}])";
    } else {
        gltf_json = std::string(R"({
            "asset": {"version": "2.0"},
            "images": [{"uri": ")") + image_uri + R"("}])";
    }
    
    gltf_json += R"(
        "textures": [{"source": 0}],
        "materials": [{"pbrMetallicRoughness": {"baseColorTexture": {"index": 0}}}],
        "meshes": [{"primitives": [{"attributes": {"POSITION": 0}, "indices": 0}]}],
        "accessors": [{"bufferView": 0, "componentType": 5126, "count": 3, "type": "VEC3"}],
        "bufferViews": [{"buffer": 0, "byteOffset": 0, "byteLength": 36}],
        "buffers": [{"uri": "data:application/octet-stream;base64,AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=", "byteLength": 36}]
    })";
    
    return gltf_json;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration flags
    uint32_t config_flags = fdp.ConsumeIntegral<uint32_t>();
    
    // Consume operation selector (0-5 for different test paths)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Create TinyGLTF context and model objects
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    // Path 0: Direct LoadImageData testing with diverse image data
    if (operation == 0) {
        // Check if enough bytes remain before calling ConsumeIntegralInRange
        if (fdp.remaining_bytes() < 1) return 0;
        // Consume image data directly from fuzzer input
        size_t image_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
        std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size);
        
        if (image_data.empty()) return 0;
        // Create multiple Image objects with different parameters
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 16; i++) {
            tinygltf::Image image;
            image.name = "fuzzed_image_" + std::to_string(i);
            
            // Consume LoadImageData parameters
            int req_width = 0;
            int req_height = 0;
            if (fdp.ConsumeBool()) {
                req_width = fdp.ConsumeIntegralInRange<int>(1, 4096);
            }
            if (fdp.ConsumeBool()) {
                req_height = fdp.ConsumeIntegralInRange<int>(1, 4096);
            }
            
            // Test with different LoadImageDataOption configurations
            tinygltf::LoadImageDataOption option;
            option.as_is = fdp.ConsumeBool();
            option.preserve_channels = fdp.ConsumeBool();
            
            // Call LoadImageData with options
            bool result = tinygltf::LoadImageData(&image, i, &err, &warn,
                                                 req_width, req_height,
                                                 image_data.data(),
                                                 static_cast<int>(image_data.size()),
                                                 &option);
            (void)result;
            
            // Also test without options
            if (fdp.ConsumeBool()) {
                tinygltf::Image image2;
                image2.name = "fuzzed_image_no_options_" + std::to_string(i);
                bool result2 = tinygltf::LoadImageData(&image2, i, &err, &warn,
                                                      req_width, req_height,
                                                      image_data.data(),
                                                      static_cast<int>(image_data.size()),
                                                      nullptr);
                (void)result2;
            }
        }
    }
    
    // Path 1: Test with custom image loader focusing on stb_image behaviors
    else if (operation == 1) {
        // Set custom image loader with configurable behavior
        ctx.SetImageLoader(custom_stb_image_loader, &config_flags);
        
        // Check if enough bytes remain before calling ConsumeIntegralInRange
        if (fdp.remaining_bytes() < 16) return 0;
        // Create image data from fuzzer input
        size_t image_size = fdp.ConsumeIntegralInRange<size_t>(16, fdp.remaining_bytes());
        std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size);
        
        if (image_data.size() < 16) return 0;
        
        // Encode image data as base64 data URI
        std::string base64_uri = "data:image/png;base64,";
        // Note: In real harness we would base64 encode, but for fuzzing we'll simulate
        // For simplicity, we'll create a glTF with the raw data as a buffer reference
        
        // Create minimal glTF JSON with buffer reference to image data
        std::string json_str = R"({
            "asset": {"version": "2.0"},
            "images": [{"bufferView": 0, "mimeType": "image/png"}],
            "bufferViews": [{"buffer": 0, "byteOffset": 0, "byteLength": )" + std::to_string(image_data.size()) + R"(}],
            "buffers": [{"byteLength": )" + std::to_string(image_data.size()) + R"(}]
        })";
        
        // We need to load binary data into the model
        // For simplicity in this harness, we'll test LoadImageData directly
        // but in a full implementation we'd create proper binary glTF
        
        tinygltf::Image test_image;
        test_image.name = "custom_loader_test";
        
        // Test with the custom loader through direct call simulation
        bool result = custom_stb_image_loader(&test_image, 0, &err, &warn,
                                             0, 0, image_data.data(),
                                             static_cast<int>(image_data.size()),
                                             &config_flags);
        (void)result;
        
        // Remove custom loader
        ctx.RemoveImageLoader();
    }
    
    // Path 2: Test stb_image functions through embedded data URIs with various formats
    else if (operation == 2) {
        // Check if enough bytes remain before calling ConsumeIntegralInRange
        if (fdp.remaining_bytes() < 32) return 0;
        // Simulate different image formats by creating data patterns
        size_t img_size = fdp.ConsumeIntegralInRange<size_t>(32, fdp.remaining_bytes());
        std::vector<uint8_t> img_data = fdp.ConsumeBytes<uint8_t>(img_size);
        
        if (img_data.size() < 32) return 0;
        
        // Create different image format signatures based on fuzzer input
        uint8_t format_selector = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        // Set appropriate signatures (simulating different formats)
        switch (format_selector) {
            case 0: // PNG
                if (img_data.size() >= 8) {
                    img_data[0] = 0x89; img_data[1] = 0x50; img_data[2] = 0x4E; img_data[3] = 0x47;
                    img_data[4] = 0x0D; img_data[5] = 0x0A; img_data[6] = 0x1A; img_data[7] = 0x0A;
                }
                break;
            case 1: // JPEG
                if (img_data.size() >= 4) {
                    img_data[0] = 0xFF; img_data[1] = 0xD8; img_data[2] = 0xFF; img_data[3] = 0xE0;
                }
                break;
            case 2: // BMP
                if (img_data.size() >= 2) {
                    img_data[0] = 'B'; img_data[1] = 'M';
                }
                break;
            case 3: // GIF
                if (img_data.size() >= 6) {
                    img_data[0] = 'G'; img_data[1] = 'I'; img_data[2] = 'F'; 
                    img_data[3] = '8'; img_data[4] = '7'; img_data[5] = 'a';
                }
                break;
            case 4: // HDR (Radiance)
                if (img_data.size() >= 10) {
                    img_data[0] = '#';
                    img_data[1] = '?';
                    img_data[2] = 'R';
                    img_data[3] = 'A';
                    img_data[4] = 'D';
                    img_data[5] = 'I';
                    img_data[6] = 'A';
                    img_data[7] = 'N';
                    img_data[8] = 'C';
                    img_data[9] = 'E';
                }
                break;
            // case 5: Keep as corrupted/unknown format
        }
        
        // Test LoadImageData with various parameter combinations
        tinygltf::Image image;
        image.name = "format_test_" + std::to_string(format_selector);
        
        // Test different combinations of width/height requirements
        int test_cases = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < test_cases && fdp.remaining_bytes() > 8; i++) {
            tinygltf::Image test_image;
            test_image.name = image.name + "_case_" + std::to_string(i);
            
            int req_width = fdp.ConsumeBool() ? fdp.ConsumeIntegralInRange<int>(1, 2048) : 0;
            int req_height = fdp.ConsumeBool() ? fdp.ConsumeIntegralInRange<int>(1, 2048) : 0;
            
            bool result = tinygltf::LoadImageData(&test_image, i, &err, &warn,
                                                 req_width, req_height,
                                                 img_data.data(),
                                                 static_cast<int>(img_data.size()),
                                                 nullptr);
            (void)result;
        }
    }
    
    // Path 3: Test image loading with different URI schemes and references
    else if (operation == 3) {
        // Check if enough bytes remain before calling ConsumeIntegralInRange
        if (fdp.remaining_bytes() < 64) return 0;
        // Create image data
        size_t img_size = fdp.ConsumeIntegralInRange<size_t>(64, fdp.remaining_bytes());
        std::vector<uint8_t> img_data = fdp.ConsumeBytes<uint8_t>(img_size);
        
        if (img_data.size() < 64) return 0;
        
        // Test different URI schemes
        uint8_t uri_scheme = fdp.ConsumeIntegral<uint8_t>() % 4;
        std::string image_uri;
        
        switch (uri_scheme) {
            case 0:
                // Data URI with base64 (simulated)
                image_uri = "data:image/png;base64,AAAA"; // Minimal base64
                break;
            case 1:
                // External file reference
                image_uri = "textures/image_" + std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ".png";
                break;
            case 2:
                // Buffer view reference (for embedded binary)
                image_uri = ""; // Will use bufferView instead
                break;
            case 3:
                // Unusual URI scheme
                image_uri = "file:///tmp/image.bin";
                break;
        }
        
        // Create glTF JSON with the image reference
        std::string json_str = create_gltf_with_image_reference(image_uri);
        
        // For buffer view case, we need different JSON
        if (uri_scheme == 2) {
            json_str = R"({
                "asset": {"version": "2.0"},
                "images": [{"bufferView": 0, "mimeType": "image/png"}],
                "bufferViews": [{"buffer": 0, "byteOffset": 0, "byteLength": )" + std::to_string(img_data.size()) + R"(}],
                "buffers": [{"byteLength": )" + std::to_string(img_data.size()) + R"(}]
            })";
            
            // Load as binary glTF
            bool load_result = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                       img_data.data(),
                                                       static_cast<unsigned int>(img_data.size()),
                                                       "");
            (void)load_result;
        } else {
            // Load as ASCII glTF
            bool load_result = ctx.LoadASCIIFromString(&model, &err, &warn,
                                                      json_str.c_str(),
                                                      static_cast<unsigned int>(json_str.size()),
                                                      "");
            (void)load_result;
        }
        
        // If images were loaded, test WriteImageData
        if (!model.images.empty()) {
            tinygltf::FsCallbacks fs_cb = tinygltf::FsCallbacks();
            tinygltf::URICallbacks uri_cb = tinygltf::URICallbacks();
            std::string out_uri;
            std::string filename = "test_output.png";
            
            bool write_result = tinygltf::WriteImageData(nullptr, &filename,
                                                        &model.images[0], false,
                                                        &fs_cb, &uri_cb, &out_uri, nullptr);
            (void)write_result;
        }
    }
    
    // Path 4: Test 16-bit and HDR image loading paths
    else if (operation == 4) {
        // Check if enough bytes remain before calling ConsumeIntegralInRange
        if (fdp.remaining_bytes() < 128) return 0;
        // Create image data that might be interpreted as 16-bit or HDR
        size_t img_size = fdp.ConsumeIntegralInRange<size_t>(128, fdp.remaining_bytes());
        std::vector<uint8_t> img_data = fdp.ConsumeBytes<uint8_t>(img_size);
        
        if (img_data.size() < 128) return 0;
        
        // Simulate 16-bit PNG signature
        if (fdp.ConsumeBool() && img_data.size() >= 8) {
            // PNG signature
            img_data[0] = 0x89; img_data[1] = 0x50; img_data[2] = 0x4E; img_data[3] = 0x47;
            img_data[4] = 0x0D; img_data[5] = 0x0A; img_data[6] = 0x1A; img_data[7] = 0x0A;
            
            // Add IHDR chunk indicating 16-bit
            if (img_data.size() >= 40) {
                // IHDR chunk type at offset 12-15
                img_data[12] = 'I'; img_data[13] = 'H'; img_data[14] = 'D'; img_data[15] = 'R';
                // Bit depth at offset 24 (16 = 0x10)
                img_data[24] = 16;
            }
        }
        
        // Simulate HDR signature
        else if (fdp.ConsumeBool() && img_data.size() >= 10) {
            img_data[0] = '#'; img_data[1] = '?'; img_data[2] = 'R';
            img_data[3] = 'A'; img_data[4] = 'D'; img_data[5] = 'I';
            img_data[6] = 'A'; img_data[7] = 'N'; img_data[8] = 'C'; img_data[9] = 'E';
        }
        
        // Test LoadImageData with focus on 16-bit/HDR paths
        tinygltf::Image image;
        image.name = "bitdepth_test";
        
        // Test with and without LoadImageDataOption
        tinygltf::LoadImageDataOption option;
        option.as_is = fdp.ConsumeBool();
        option.preserve_channels = fdp.ConsumeBool();
        
        bool result = tinygltf::LoadImageData(&image, 0, &err, &warn,
                                             0, 0, img_data.data(),
                                             static_cast<int>(img_data.size()),
                                             &option);
        (void)result;
        
        // Also test without options
        if (fdp.ConsumeBool()) {
            tinygltf::Image image2;
            image2.name = "bitdepth_test_no_options";
            bool result2 = tinygltf::LoadImageData(&image2, 0, &err, &warn,
                                                  0, 0, img_data.data(),
                                                  static_cast<int>(img_data.size()),
                                                  nullptr);
            (void)result2;
        }
    }
    
    // Path 5: Corrupted image testing and error handling paths
    else if (operation == 5) {
        // Check if enough bytes remain before calling ConsumeIntegralInRange
        if (fdp.remaining_bytes() < 8) return 0;
        // Create intentionally corrupted image data
        size_t corrupt_size = fdp.ConsumeIntegralInRange<size_t>(8, fdp.remaining_bytes());
        std::vector<uint8_t> corrupt_data = fdp.ConsumeBytes<uint8_t>(corrupt_size);
        
        if (corrupt_data.empty()) return 0;
        
        // Corrupt the data in various ways
        uint8_t corruption_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        switch (corruption_type) {
            case 0:
                // Truncated data (simulate incomplete image)
                if (corrupt_data.size() > 10) {
                    corrupt_data.resize(corrupt_data.size() / 2);
                }
                break;
            case 1:
                // Invalid headers
                if (corrupt_data.size() >= 8) {
                    for (int i = 0; i < 8; i++) {
                        corrupt_data[i] = 0xFF;
                    }
                }
                break;
            case 2:
                // Zero-sized dimensions in header simulation
                // For PNG-like structure, set width/height to 0
                if (corrupt_data.size() >= 30) {
                    // Assuming IHDR chunk data starts at offset 16
                    // Width bytes at offset 16-19, height at 20-23
                    for (int i = 16; i < 24; i++) {
                        corrupt_data[i] = 0;
                    }
                }
                break;
            case 3:
                // Invalid pixel data
                if (corrupt_data.size() >= 50) {
                    // Add invalid pixel values
                    for (size_t i = 30; i < corrupt_data.size() && i < 100; i++) {
                        corrupt_data[i] = (corrupt_data[i] == 0) ? 255 : 0;
                    }
                }
                break;
            // case 4: Keep as random corruption
        }
        
        // Test error handling with corrupted data
        tinygltf::Image corrupt_image;
        corrupt_image.name = "corrupt_test_" + std::to_string(corruption_type);
        
        bool result = tinygltf::LoadImageData(&corrupt_image, 0, &err, &warn,
                                             0, 0, corrupt_data.data(),
                                             static_cast<int>(corrupt_data.size()),
                                             nullptr);
        (void)result;
        
        // Test with as_is option (should handle corruption differently)
        if (fdp.ConsumeBool()) {
            tinygltf::Image corrupt_image2;
            corrupt_image2.name = "corrupt_as_is_test";
            
            tinygltf::LoadImageDataOption option;
            option.as_is = true;
            option.preserve_channels = fdp.ConsumeBool();
            
            bool result2 = tinygltf::LoadImageData(&corrupt_image2, 0, &err, &warn,
                                                  0, 0, corrupt_data.data(),
                                                  static_cast<int>(corrupt_data.size()),
                                                  &option);
            (void)result2;
        }
    }
    
    return 0;
}

} // namespace harness_005
