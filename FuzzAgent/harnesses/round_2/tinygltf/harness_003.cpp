#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>
#include <functional>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_003 {
// Harness 003: Image loading and decoding operations fuzzer for tinygltf
// Targets: LoadImageData, WriteImageData, SetImageLoader, SetImageWriter, and stb_* functions
// Specifically targets stb_image.h module which has 0% coverage (4606 lines)
// Uses FuzzedDataProvider for structured input splitting


// Include FuzzedDataProvider for structured input splitting

// Define implementations for stb_image and tinygltf
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// Custom image loader callback for SetImageLoader
bool custom_image_loader(tinygltf::Image* image, const int image_idx, 
                         std::string* err, std::string* warn, 
                         int req_width, int req_height,
                         const unsigned char* bytes, int size, void* user_data) {
    (void)user_data; // Unused parameter
    
    // Log that custom loader was called (for coverage)
    if (warn) {
        *warn += "Custom image loader called for image[" + std::to_string(image_idx) + "]\n";
    }
    
    // For fuzzing, we can either pass through to default loader or simulate different behaviors
    // Based on input, we can choose to succeed or fail
    uint32_t* behavior = reinterpret_cast<uint32_t*>(user_data);
    if (behavior && (*behavior & 0x1)) {
        // Simulate failure
        if (err) {
            *err += "Custom loader failed for image[" + std::to_string(image_idx) + "]\n";
        }
        return false;
    }
    
    // Otherwise use default LoadImageData
    return tinygltf::LoadImageData(image, image_idx, err, warn, 
                                   req_width, req_height, bytes, size, nullptr);
}

// Custom image writer callback for SetImageWriter
bool custom_image_writer(const std::string* basepath, const std::string* filename,
                         const tinygltf::Image* image, bool embedImages,
                         const tinygltf::FsCallbacks* fs_cb, 
                         const tinygltf::URICallbacks* uri_cb,
                         std::string* out_uri, void* user_data) {
    (void)user_data; // Unused parameter
    
    // Log that custom writer was called
    // For fuzzing, we can either pass through to default writer or simulate behaviors
    // Based on input, we can choose different formats or simulate failures
    
    uint32_t* behavior = reinterpret_cast<uint32_t*>(user_data);
    if (behavior && (*behavior & 0x2)) {
        // Simulate failure for certain inputs
        return false;
    }
    
    // Use default WriteImageData
    return tinygltf::WriteImageData(basepath, filename, image, embedImages, 
                                   fs_cb, uri_cb, out_uri, nullptr);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for image operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create TinyGLTF context and model objects
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Consume operation selector (0-4 for different test paths)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Behavior flags for custom callbacks
    uint32_t behavior_flags = fdp.ConsumeIntegral<uint32_t>();
    
    // Path 0: Test LoadImageData directly with various image formats
    if (operation == 0) {
        // Consume image data from remaining input
        size_t image_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
        std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size);
        
        if (image_data.empty()) return 0;
        
        // Create an Image object
        tinygltf::Image image;
        image.name = "fuzzed_image";
        image.mimeType = "image/png"; // Default, but LoadImageData detects format
        
        // Call LoadImageData directly with different parameters
        int req_width = fdp.ConsumeBool() ? fdp.ConsumeIntegralInRange<int>(1, 4096) : 0;
        int req_height = fdp.ConsumeBool() ? fdp.ConsumeIntegralInRange<int>(1, 4096) : 0;
        
        bool result = tinygltf::LoadImageData(&image, 0, &err, &warn, 
                                             req_width, req_height,
                                             image_data.data(), 
                                             static_cast<int>(image_data.size()),
                                             nullptr);
        (void)result; // Ignore result for fuzzing
        
        // Also test with as_is option through LoadImageDataOption
        if (fdp.ConsumeBool() && image_data.size() > 16) {
            //tinygltf::LoadImageDataOption option;
            //option.as_is = fdp.ConsumeBool();
            //option.preserve_channels = fdp.ConsumeBool();
            
            tinygltf::Image image2;
            image2.name = "fuzzed_image_as_is";
            
            // Would need to pass option as user_data, but LoadImageData expects LoadImageDataOption*
            // We'll rely on the default LoadImageData to test both paths
        }
    }
    
    // Path 1: Set custom image loader and load glTF with embedded images
    else if (operation == 1) {
        // Set custom image loader
        ctx.SetImageLoader(custom_image_loader, &behavior_flags);
        
        // Create simple glTF JSON with embedded image data URI
        std::string image_data_uri = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mNkYPhfDwAChwGA60e6kgAAAABJRU5ErkJggg=="; // 1x1 transparent PNG
        
        // Use remaining input as JSON
        std::string json_str = fdp.ConsumeRemainingBytesAsString();
        if (json_str.empty()) {
            // Fallback minimal glTF with image
            json_str = R"({
                "asset": {"version": "2.0"},
                "images": [{"uri": ")" + image_data_uri + R"("}],
                "textures": [{"source": 0}],
                "materials": [{"pbrMetallicRoughness": {"baseColorTexture": {"index": 0}}}],
                "meshes": [{"primitives": [{"attributes": {"POSITION": 0}, "indices": 0}]}],
                "accessors": [{"bufferView": 0, "componentType": 5126, "count": 3, "type": "VEC3"}],
                "bufferViews": [{"buffer": 0, "byteOffset": 0, "byteLength": 36}],
                "buffers": [{"uri": "data:application/octet-stream;base64,AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=", "byteLength": 36}]
            })";
        }
        
        bool load_result = ctx.LoadASCIIFromString(&model, &err, &warn,
                                                  json_str.c_str(),
                                                  static_cast<unsigned int>(json_str.size()),
                                                  "");
        (void)load_result;
        
        // Remove custom loader to restore default
        ctx.RemoveImageLoader();
    }
    
    // Path 2: Test SetImageWriter and WriteGltfSceneToStream with images
    else if (operation == 2) {
        // First create a model with an image
        tinygltf::Image img;
        img.name = "test_image";
        img.width = 2;
        img.height = 2;
        img.component = 4; // RGBA
        img.bits = 8;
        img.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
        img.image = {255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 255, 255};
        img.uri = "test.png";
        
        model.images.push_back(img);
        model.asset.version = "2.0";
        
        // Set custom image writer
        ctx.SetImageWriter(custom_image_writer, &behavior_flags);
        
        // Write to stream
        std::ostringstream stream;
        bool pretty_print = fdp.ConsumeBool();
        bool write_binary = fdp.ConsumeBool();
        
        bool write_result = ctx.WriteGltfSceneToStream(&model, stream,
                                                      pretty_print,
                                                      write_binary);
        (void)write_result;
        
        // Also test WriteGltfSceneToFile with temporary file
        if (fdp.ConsumeBool()) {
            std::string temp_file = "/tmp/tinygltf_fuzz_write_" + 
                                   std::to_string(fdp.ConsumeIntegral<uint32_t>()) + 
                                   (write_binary ? ".glb" : ".gltf");
            
            bool embed_images = fdp.ConsumeBool();
            bool embed_buffers = fdp.ConsumeBool();
            
            bool file_result = ctx.WriteGltfSceneToFile(&model, temp_file,
                                                       embed_images,
                                                       embed_buffers,
                                                       pretty_print,
                                                       write_binary);
            (void)file_result;
            
            // Clean up
            remove(temp_file.c_str());
        }
    
    // Path 3: Test both custom loader and writer together
    else if (operation == 3) {
        // Set both custom callbacks
        ctx.SetImageLoader(custom_image_loader, &behavior_flags);
        ctx.SetImageWriter(custom_image_writer, &behavior_flags);
        
        // Create minimal glTF with image reference
        std::string json_str = R"({
            "asset": {"version": "2.0"},
            "images": [{"uri": "test.png"}],
            "textures": [{"source": 0}]
        })";
        
        // Load
        bool load_result = ctx.LoadASCIIFromString(&model, &err, &warn,
                                                  json_str.c_str(),
                                                  static_cast<unsigned int>(json_str.size()),
                                                  "");
        (void)load_result;
        // Write if model was loaded successfully
        if (!model.images.empty()) {
            std::ostringstream stream;
            bool pretty_print = fdp.ConsumeBool();
            bool write_binary = fdp.ConsumeBool();
            bool write_result = ctx.WriteGltfSceneToStream(&model, stream,
                                                          pretty_print,
                                                          write_binary);
            (void)write_result;
        }
        
        // Restore defaults
        ctx.RemoveImageLoader();
        // Note: There's no RemoveImageWriter, but SetImageWriter with nullptr should work
        ctx.SetImageWriter(nullptr, nullptr);
    }
    
    // Path 4: Direct stb_image function testing with various image data
    else if (operation == 4) {
        // Test stb_image functions directly by creating different image data patterns
        
        // Consume image data
        size_t img_size = fdp.ConsumeIntegralInRange<size_t>(16, fdp.remaining_bytes());
        std::vector<uint8_t> img_data = fdp.ConsumeBytes<uint8_t>(img_size);
        
        if (img_data.size() < 16) return 0;
        
        // Simulate different image formats by setting up data patterns
        // PNG signature
        if (fdp.ConsumeBool() && img_data.size() >= 8) {
            img_data[0] = 0x89; img_data[1] = 0x50; img_data[2] = 0x4E; img_data[3] = 0x47;
            img_data[4] = 0x0D; img_data[5] = 0x0A; img_data[6] = 0x1A; img_data[7] = 0x0A;
        }
        // JPEG signature
        else if (fdp.ConsumeBool() && img_data.size() >= 2) {
            img_data[0] = 0xFF; img_data[1] = 0xD8;
        }
        // BMP signature
        else if (fdp.ConsumeBool() && img_data.size() >= 2) {
            img_data[0] = 'B'; img_data[1] = 'M';
        }
        
        // Create Image and test LoadImageData with various parameters
        tinygltf::Image test_image;
        test_image.name = "direct_stb_test";
        
        int req_width = 0, req_height = 0;
        if (fdp.ConsumeBool()) {
            req_width = fdp.ConsumeIntegralInRange<int>(1, 1024);
        }
        if (fdp.ConsumeBool()) {
            req_height = fdp.ConsumeIntegralInRange<int>(1, 1024);
        }
        
        bool load_result = tinygltf::LoadImageData(&test_image, 0, &err, &warn,
                                                  req_width, req_height,
                                                  img_data.data(), 
                                                  static_cast<int>(img_data.size()),
                                                  nullptr);
        (void)load_result;
        
        // Test writing if image was successfully loaded
        if (test_image.width > 0 && test_image.height > 0 && !test_image.image.empty()) {
            // Create a temporary model with the image
            tinygltf::Model temp_model;
            temp_model.images.push_back(test_image);
            temp_model.asset.version = "2.0";
            
            // Write using default writer
            std::ostringstream stream;
            bool pretty_print = fdp.ConsumeBool();
            bool write_binary = fdp.ConsumeBool();
            bool write_result = ctx.WriteGltfSceneToStream(&temp_model, stream,
                                                          pretty_print,
                                                          write_binary);
            (void)write_result;
        }
        }
    }
    
    return 0;
}

} // namespace harness_003
