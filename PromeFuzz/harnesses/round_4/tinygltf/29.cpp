// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.cc:958:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.cc:958:16 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "tiny_gltf.h"

// Custom image loader function for testing
static bool custom_image_loader(tinygltf::Image* image, int image_idx, std::string* err,
                               std::string* warn, int req_width, int req_height,
                               const unsigned char* bytes, int size, void* user_data) {
    // Simple loader that just copies data if size > 0
    if (size > 0) {
        image->image.resize(size);
        memcpy(image->image.data(), bytes, size);
        image->width = req_width > 0 ? req_width : 100; // Default width
        return true;
    }
    if (err) *err = "No image data";
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Initialize objects
    tinygltf::TinyGLTF loader;
    tinygltf::Image image;
    std::string err;
    std::string warn;
    
    // Use first byte to determine test path
    uint8_t selector = 0;
    if (Size > 0) {
        selector = Data[0];
        Data++;
        Size--;
    }
    
    // Test RemoveImageLoader
    loader.RemoveImageLoader();
    
    // Test SetPreserveImageChannels and GetPreserveImageChannels
    bool preserve_channels = (selector & 0x01) != 0;
    loader.SetPreserveImageChannels(preserve_channels);
    bool current_preserve = loader.GetPreserveImageChannels();
    (void)current_preserve; // Use variable to avoid unused warning
    
    // Test SetImagesAsIs
    bool images_as_is = (selector & 0x02) != 0;
    loader.SetImagesAsIs(images_as_is);
    
    // Test SetImageLoader with different configurations
    if ((selector & 0x04) != 0) {
        // Set custom image loader
        loader.SetImageLoader(custom_image_loader, nullptr);
    } else if ((selector & 0x08) != 0) {
        // Set to nullptr (should remove loader)
        loader.SetImageLoader(nullptr, nullptr);
    } else {
        // Set default loader
        loader.SetImageLoader(tinygltf::LoadImageData, nullptr);
    }
    
    // Test LoadImageData with the provided data
    if (Size > 0) {
        // Set image.as_is flag based on selector
        image.as_is = (selector & 0x10) != 0;
        
        // Try loading image data with various parameters
        int req_width = (selector & 0x20) ? 256 : 0;
        int req_height = (selector & 0x40) ? 256 : 0;
        
        // Call LoadImageData with the fuzzer data
        bool load_result = tinygltf::LoadImageData(&image, 0, &err, &warn,
                                                   req_width, req_height,
                                                   Data, Size, nullptr);
        (void)load_result; // Use variable to avoid unused warning
    }
    
    // Clean up by removing image loader
    loader.RemoveImageLoader();
    
    return 0;
}