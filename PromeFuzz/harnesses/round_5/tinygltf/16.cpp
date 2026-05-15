// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.cc:958:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
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
#include "tiny_gltf.h"
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

static bool customImageLoader(tinygltf::Image* image, const int image_idx, std::string* err,
                             std::string* warn, int req_width, int req_height,
                             const unsigned char* bytes, int size, void* user_data) {
    // Simple custom loader that just copies data
    if (size > 0) {
        image->image.resize(size);
        memcpy(image->image.data(), bytes, size);
        image->width = req_width > 0 ? req_width : 1;
        return true;
    }
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Initialize TinyGLTF object
    tinygltf::TinyGLTF loader;
    
    // Use first byte to control boolean values
    if (Size > 0) {
        bool preserveChannels = (Data[0] & 0x01) != 0;
        bool imagesAsIs = (Data[0] & 0x02) != 0;
        bool useCustomLoader = (Data[0] & 0x04) != 0;
        
        // Test SetPreserveImageChannels and GetPreserveImageChannels
        loader.SetPreserveImageChannels(preserveChannels);
        bool retrievedPreserve = loader.GetPreserveImageChannels();
        (void)retrievedPreserve; // Avoid unused variable warning
        
        // Test SetImagesAsIs
        loader.SetImagesAsIs(imagesAsIs);
        
        // Test SetImageLoader and RemoveImageLoader
        if (useCustomLoader) {
            void* userData = nullptr;
            loader.SetImageLoader(customImageLoader, userData);
            
            // Test RemoveImageLoader
            loader.RemoveImageLoader();
        } else {
            // Set to nullptr to test removal
            loader.SetImageLoader(nullptr, nullptr);
        }
        
        // Test LoadImageData function with remaining data
        if (Size > 1) {
            tinygltf::Image image;
            std::string err;
            std::string warn;
            
            // Use some bytes for width/height parameters
            int req_width = 0;
            int req_height = 0;
            
            if (Size > 3) {
                // Use bytes 1-2 for width, 3-4 for height (if available)
                req_width = static_cast<int>(Data[1]);
                req_height = static_cast<int>(Data[2]);
            }
            
            // Use remaining bytes as image data
            const unsigned char* image_data = Data + (Size > 3 ? 3 : 1);
            int image_size = static_cast<int>(Size - (Size > 3 ? 3 : 1));
            
            if (image_size > 0) {
                // Call LoadImageData with the fuzzed data
                bool result = tinygltf::LoadImageData(&image, 0, &err, &warn,
                                                     req_width, req_height,
                                                     image_data, image_size, nullptr);
                (void)result; // Avoid unused variable warning
            }
        }
    } else {
        // Test with default values when no data
        loader.SetPreserveImageChannels(false);
        bool defaultPreserve = loader.GetPreserveImageChannels();
        (void)defaultPreserve;
        
        loader.SetImagesAsIs(false);
        loader.SetImageLoader(nullptr, nullptr);
        loader.RemoveImageLoader();
    }
    
    return 0;
}