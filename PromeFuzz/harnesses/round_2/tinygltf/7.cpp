// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
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
#include <cstddef>
#include <string>
#include <vector>

static bool custom_image_loader(tinygltf::Image *image, const int image_idx, std::string *err,
                               std::string *warn, int req_width, int req_height,
                               const unsigned char *bytes, int size, void *user_data) {
    // Simple custom loader that just copies data if size > 0
    if (size > 0 && bytes != nullptr) {
        image->image.assign(bytes, bytes + size);
        image->width = req_width > 0 ? req_width : 1;
        return true;
    }
    if (err) *err = "Custom loader: No data";
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Initialize TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Test GetPreserveImageChannels and SetPreserveImageChannels
    bool initial_preserve = loader.GetPreserveImageChannels();
    
    // Use fuzzer data to determine boolean values
    bool new_preserve = (Size > 0) ? (Data[0] % 2 == 0) : false;
    loader.SetPreserveImageChannels(new_preserve);
    bool after_preserve = loader.GetPreserveImageChannels();
    
    // Test GetImagesAsIs and SetImagesAsIs
    bool initial_as_is = loader.GetImagesAsIs();
    
    bool new_as_is = (Size > 1) ? (Data[1] % 2 == 0) : false;
    loader.SetImagesAsIs(new_as_is);
    bool after_as_is = loader.GetImagesAsIs();
    
    // Test SetImageLoader with different configurations
    if (Size > 2) {
        uint8_t loader_choice = Data[2] % 3;
        
        switch (loader_choice) {
            case 0:
                // Set to nullptr (remove loader)
                loader.SetImageLoader(nullptr, nullptr);
                break;
            case 1:
                // Set to default loader (nullptr with specific meaning in tinygltf)
                loader.SetImageLoader(nullptr, nullptr);
                break;
            case 2:
                // Set to custom loader
                loader.SetImageLoader(custom_image_loader, nullptr);
                break;
        }
    }
    
    // Test LoadImageData function directly
    if (Size > 10) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        
        // Use fuzzer data for image parameters
        int req_width = (Size > 3) ? static_cast<int>(Data[3]) : 0;
        int req_height = (Size > 4) ? static_cast<int>(Data[4]) : 0;
        
        // Ensure non-negative dimensions
        if (req_width < 0) req_width = 0;
        if (req_height < 0) req_height = 0;
        
        // Use remaining data as image bytes
        const unsigned char* image_data = (Size > 5) ? Data + 5 : nullptr;
        int image_size = (Size > 5) ? static_cast<int>(Size - 5) : 0;
        
        // Call LoadImageData with different configurations
        bool load_success = tinygltf::LoadImageData(&image, 0, &err, &warn,
                                                   req_width, req_height,
                                                   image_data, image_size, nullptr);
        
        // Try with as_is flag set
        if (Size > 20) {
            tinygltf::Image image2;
            image2.as_is = (Data[5] % 2 == 0);
            
            bool load_success2 = tinygltf::LoadImageData(&image2, 1, nullptr, nullptr,
                                                       0, 0, image_data, image_size, nullptr);
        }
    }
    
    // Test with dummy file if needed
    if (Size > 0) {
        FILE* dummy = fopen("./dummy_file", "wb");
        if (dummy) {
            fwrite(Data, 1, Size, dummy);
            fclose(dummy);
        }
    }
    
    return 0;
}