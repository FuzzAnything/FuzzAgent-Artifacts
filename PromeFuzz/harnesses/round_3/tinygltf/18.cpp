// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

static bool customImageLoader(tinygltf::Image* image, const int image_idx,
                              std::string* err, std::string* warn,
                              int req_width, int req_height,
                              const unsigned char* bytes, int size, void* user_data) {
    // Simple custom loader that just copies data if size matches expectations
    if (size > 0 && bytes != nullptr) {
        image->image.resize(size);
        std::memcpy(image->image.data(), bytes, size);
        image->width = req_width > 0 ? req_width : 128;
        return true;
    }
    if (err) *err = "Custom loader: invalid input";
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Initialize TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Use first byte to determine which functions to call
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // SetImagesAsIs with random boolean
    loader.SetImagesAsIs((selector & 0x01) != 0);
    
    // SetImageLoader with either custom loader or nullptr
    if ((selector & 0x02) != 0) {
        loader.SetImageLoader(customImageLoader, nullptr);
    } else {
        loader.SetImageLoader(nullptr, nullptr);
    }
    
    // SetPreserveImageChannels with random boolean
    loader.SetPreserveImageChannels((selector & 0x04) != 0);
    
    // Call getters to verify state
    bool imagesAsIs = loader.GetImagesAsIs();
    bool preserveChannels = loader.GetPreserveImageChannels();
    
    // Test LoadImageData function if we have enough data
    if (Size > data_offset + 4) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        
        // Extract small integers from data for width/height
        int req_width = 0;
        int req_height = 0;
        if (Size >= data_offset + 8) {
            // Use bytes 1-4 for width, 5-8 for height (mod 1024 to keep reasonable)
            req_width = static_cast<int>(Data[data_offset] | (Data[data_offset+1] << 8)) % 1024;
            req_height = static_cast<int>(Data[data_offset+4] | (Data[data_offset+5] << 8)) % 1024;
            data_offset += 8;
        }
        
        // Calculate remaining data size for image bytes
        size_t remaining_size = Size - data_offset;
        if (remaining_size > 0) {
            const unsigned char* image_data = Data + data_offset;
            int image_size = static_cast<int>(remaining_size);
            
            // Test with different combinations of error/warn pointers
            if ((selector & 0x08) != 0) {
                // Call with both error and warning strings
                tinygltf::LoadImageData(&image, 0, &err, &warn, req_width, req_height,
                                       image_data, image_size, nullptr);
            } else if ((selector & 0x10) != 0) {
                // Call with only error string
                tinygltf::LoadImageData(&image, 0, &err, nullptr, req_width, req_height,
                                       image_data, image_size, nullptr);
            } else {
                // Call with no error/warning strings
                tinygltf::LoadImageData(&image, 0, nullptr, nullptr, req_width, req_height,
                                       image_data, image_size, nullptr);
            }
            
            // Test with as_is flag set on image
            if ((selector & 0x20) != 0 && remaining_size > 10) {
                tinygltf::Image image2;
                image2.as_is = true;
                tinygltf::LoadImageData(&image2, 1, nullptr, nullptr, 0, 0,
                                       image_data, image_size, nullptr);
            }
        }
    }
    
    // Additional test: create dummy file and test loader with it
    if (Size > 10) {
        FILE* dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file) {
            // Write first 100 bytes or all available data
            size_t write_size = Size > 100 ? 100 : Size;
            fwrite(Data, 1, write_size, dummy_file);
            fclose(dummy_file);
            
            // Try to load as glTF (will likely fail, but exercises code paths)
            tinygltf::Model model;
            std::string err;
            std::string warn;
            loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
            
            // Clean up
            remove("./dummy_file");
        }
    }
    
    return 0;
}