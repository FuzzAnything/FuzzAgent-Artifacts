// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
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

static bool dummyImageLoader(tinygltf::Image* image, const int image_idx, std::string* err,
                             std::string* warn, int req_width, int req_height,
                             const unsigned char* bytes, int size, void* user_data) {
    // Simple dummy loader that just copies data if size is positive
    if (size > 0) {
        image->image.resize(size);
        std::memcpy(image->image.data(), bytes, size);
        image->width = req_width > 0 ? req_width : 1;
        return true;
    }
    if (err) *err = "dummy loader error";
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Initialize TinyGLTF object
    tinygltf::TinyGLTF loader;
    
    // Use first byte to determine boolean values for various settings
    bool bool_val = false;
    if (Size > 0) {
        bool_val = (Data[0] & 0x01) != 0;
    }
    
    // Test SetImagesAsIs and GetImagesAsIs
    loader.SetImagesAsIs(bool_val);
    bool get_as_is = loader.GetImagesAsIs();
    (void)get_as_is; // Use result to avoid unused variable warning
    
    // Test SetPreserveImageChannels and GetPreserveImageChannels
    bool preserve_channels = false;
    if (Size > 1) {
        preserve_channels = (Data[1] & 0x01) != 0;
    }
    loader.SetPreserveImageChannels(preserve_channels);
    bool get_preserve = loader.GetPreserveImageChannels();
    (void)get_preserve;
    
    // Test SetImageLoader with both nullptr and custom loader
    static uint8_t user_data = 42;
    if (Size > 2) {
        if ((Data[2] & 0x01) != 0) {
            // Set custom loader
            loader.SetImageLoader(dummyImageLoader, &user_data);
        } else {
            // Set nullptr loader
            loader.SetImageLoader(nullptr, nullptr);
        }
    } else {
        // Default: set custom loader
        loader.SetImageLoader(dummyImageLoader, &user_data);
    }
    
    // Test LoadImageData function with various inputs
    if (Size > 3) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        
        // Use different combinations of parameters based on input data
        int req_width = -1;
        int req_height = -1;
        bool as_is_setting = false;
        
        if (Size > 7) {
            // Use bytes 3-6 for width/height (interpret as 16-bit values)
            req_width = (Data[3] << 8) | Data[4];
            req_height = (Data[5] << 8) | Data[6];
            as_is_setting = (Data[7] & 0x01) != 0;
            
            // Set image.as_is flag based on input
            image.as_is = as_is_setting;
            
            // Call LoadImageData with remaining data as image bytes
            const unsigned char* image_data = Data + 8;
            size_t image_size = Size - 8;
            
            if (image_size > 0) {
                bool load_result = tinygltf::LoadImageData(&image, 0, &err, &warn,
                                                          req_width, req_height,
                                                          image_data, static_cast<int>(image_size),
                                                          nullptr);
                (void)load_result;
            }
        } else {
            // Minimal test with default parameters
            const unsigned char* image_data = Data + 3;
            size_t image_size = Size - 3;
            
            if (image_size > 0) {
                bool load_result = tinygltf::LoadImageData(&image, 0, &err, &warn,
                                                          req_width, req_height,
                                                          image_data, static_cast<int>(image_size),
                                                          nullptr);
                (void)load_result;
            }
        }
    }
    
    // Test with file-based approach using dummy file
    if (Size > 0) {
        FILE* dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file) {
            fwrite(Data, 1, Size, dummy_file);
            fclose(dummy_file);
            
            // Reinitialize loader with different settings
            tinygltf::TinyGLTF loader2;
            loader2.SetImagesAsIs((Size & 0x01) != 0);
            loader2.SetPreserveImageChannels((Size & 0x02) != 0);
            
            // Remove the dummy file
            remove("./dummy_file");
        }
    }
    
    return 0;
}