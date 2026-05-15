// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.cc:958:16 in tiny_gltf.h
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

// Custom image loader callback for testing
static bool CustomImageLoader(tinygltf::Image* image, const int image_idx,
                             std::string* err, std::string* warn,
                             int req_width, int req_height,
                             const unsigned char* bytes, int size,
                             void* user_data) {
    // Simple loader that just copies data if size > 0
    if (size > 0 && bytes != nullptr) {
        image->image.resize(size);
        std::memcpy(image->image.data(), bytes, size);
        image->width = req_width > 0 ? req_width : 1;
        return true;
    }
    if (err) *err = "Custom loader: invalid input";
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* Data, size_t Size) {
    // Initialize TinyGLTF object
    tinygltf::TinyGLTF loader;
    
    // Use first byte to determine operation mode
    uint8_t mode = 0;
    if (Size > 0) {
        mode = Data[0];
        Data++;
        Size--;
    }
    
    // Test different combinations of settings
    bool preserve_channels = (mode & 0x01) != 0;
    bool images_as_is = (mode & 0x02) != 0;
    bool use_custom_loader = (mode & 0x04) != 0;
    bool remove_loader = (mode & 0x08) != 0;
    
    // Set preserve image channels
    loader.SetPreserveImageChannels(preserve_channels);
    
    // Verify getter returns what we set
    bool get_preserve = loader.GetPreserveImageChannels();
    if (get_preserve != preserve_channels) {
        // This shouldn't happen, but we check anyway
        volatile int dummy = 0;
        (void)dummy;
    }
    
    // Set images as-is
    loader.SetImagesAsIs(images_as_is);
    
    // Test custom image loader
    if (use_custom_loader) {
        // Use some user data (just a dummy integer)
        static int user_data = 42;
        loader.SetImageLoader(CustomImageLoader, &user_data);
    }
    
    // Test removing image loader
    if (remove_loader) {
        loader.RemoveImageLoader();
    }
    
    // Test LoadImageData function with remaining data
    if (Size > 0) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        
        // Use part of the data for image loading
        // Requested dimensions based on data
        int req_width = Size > 1 ? static_cast<int>(Data[0]) : 0;
        int req_height = Size > 2 ? static_cast<int>(Data[1]) : 0;
        
        const unsigned char* image_data = Data;
        size_t image_size = Size;
        
        // Adjust if we used bytes for dimensions
        if (Size > 2) {
            image_data += 2;
            image_size -= 2;
        }
        
        // Call LoadImageData with the data
        bool load_result = tinygltf::LoadImageData(&image, 0, &err, &warn,
                                                  req_width, req_height,
                                                  image_data,
                                                  static_cast<int>(image_size),
                                                  nullptr);
        
        // Use the result to avoid dead code elimination
        if (load_result) {
            volatile int dummy = image.width;
            (void)dummy;
        }
    }
    
    // Clean up - nothing specific needed for these functions
    
    return 0;
}