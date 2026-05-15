// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.cc:958:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.cc:958:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
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

// Custom image loader function for testing
static bool CustomImageLoader(tinygltf::Image* image, int image_idx, std::string* err,
                             std::string* warn, int req_width, int req_height,
                             const unsigned char* bytes, int size, void* user_data) {
    // Simple implementation that just copies data
    if (size > 0) {
        image->image.resize(size);
        std::memcpy(image->image.data(), bytes, size);
        image->width = req_width > 0 ? req_width : 1;
        return true;
    }
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* Data, size_t Size) {
    // Initialize objects
    tinygltf::TinyGLTF loader;
    tinygltf::Image image;
    std::string err;
    std::string warn;
    
    // Use fuzz data to determine various parameters
    bool bool_param = Size > 0 ? (Data[0] % 2 == 0) : false;
    int int_param = Size > 1 ? static_cast<int>(Data[1]) : 0;
    
    // Test SetPreserveImageChannels and GetPreserveImageChannels
    loader.SetPreserveImageChannels(bool_param);
    bool preserve_channels = loader.GetPreserveImageChannels();
    (void)preserve_channels; // Use result to avoid unused variable warning
    
    // Test SetImagesAsIs
    loader.SetImagesAsIs(bool_param);
    
    // Test SetImageLoader with custom loader
    loader.SetImageLoader(CustomImageLoader, nullptr);
    
    // Test RemoveImageLoader
    loader.RemoveImageLoader();
    
    // Test SetImageLoader with nullptr (should be equivalent to RemoveImageLoader)
    loader.SetImageLoader(nullptr, nullptr);
    
    // Test LoadImageData function with fuzz data
    if (Size > 0) {
        // Create parameters from fuzz data
        int req_width = Size > 2 ? static_cast<int>(Data[2]) : 0;
        int req_height = Size > 3 ? static_cast<int>(Data[3]) : 0;
        
        // Use remaining data as image bytes
        const unsigned char* image_bytes = Size > 4 ? Data + 4 : Data;
        int image_size = Size > 4 ? static_cast<int>(Size - 4) : static_cast<int>(Size);
        
        // Call LoadImageData
        bool result = tinygltf::LoadImageData(&image, int_param, &err, &warn,
                                             req_width, req_height,
                                             image_bytes, image_size, nullptr);
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Test with different combinations
    if (Size > 1) {
        // Test with different preserve channels setting
        loader.SetPreserveImageChannels(!bool_param);
        
        // Test with different as-is setting
        loader.SetImagesAsIs(!bool_param);
        
        // Test SetImageLoader again with different user data
        int user_data_dummy = 42;
        loader.SetImageLoader(CustomImageLoader, &user_data_dummy);
        
        // Test LoadImageData with different parameters
        if (Size > 8) {
            tinygltf::Image image2;
            int req_width2 = static_cast<int>(Data[4]);
            int req_height2 = static_cast<int>(Data[5]);
            const unsigned char* image_bytes2 = Data + 8;
            int image_size2 = static_cast<int>(Size - 8);
            
            bool result2 = tinygltf::LoadImageData(&image2, int_param + 1, &err, &warn,
                                                  req_width2, req_height2,
                                                  image_bytes2, image_size2, nullptr);
            (void)result2;
        }
    }
    
    // Clean up by removing image loader
    loader.RemoveImageLoader();
    
    return 0;
}