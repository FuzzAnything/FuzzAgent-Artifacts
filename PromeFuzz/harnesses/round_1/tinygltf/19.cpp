// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.cc:948:16 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
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

static bool dummy_image_loader(tinygltf::Image* image, const int image_idx, std::string* err,
                               std::string* warn, int req_width, int req_height,
                               const unsigned char* bytes, int size, void* user_data) {
  // Dummy implementation that returns true and does nothing
  return true;
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
  // Create TinyGLTF instance
  tinygltf::TinyGLTF loader;
  
  // Use fuzz data to determine boolean values for setters
  bool images_as_is = (Size > 0) ? (Data[0] & 0x01) : false;
  bool preserve_channels = (Size > 0) ? (Data[0] & 0x02) : false;
  bool use_custom_loader = (Size > 0) ? (Data[0] & 0x04) : false;
  
  // Test SetImagesAsIs and GetImagesAsIs
  loader.SetImagesAsIs(images_as_is);
  bool retrieved_as_is = loader.GetImagesAsIs();
  
  // Test SetPreserveImageChannels and GetPreserveImageChannels
  loader.SetPreserveImageChannels(preserve_channels);
  bool retrieved_preserve = loader.GetPreserveImageChannels();
  
  // Test SetImageLoader with custom loader or nullptr
  if (use_custom_loader) {
    loader.SetImageLoader(dummy_image_loader, nullptr);
  } else {
    loader.SetImageLoader(nullptr, nullptr);
  }
  
  // Test LoadImageData function with fuzz data
  if (Size > 0) {
    tinygltf::Image image;
    std::string err;
    std::string warn;
    
    // Use remaining data as image bytes
    const unsigned char* image_data = (Size > 1) ? Data + 1 : Data;
    size_t image_size = (Size > 1) ? Size - 1 : Size;
    
    // Try different combinations of parameters
    bool load_result = tinygltf::LoadImageData(&image, 0, &err, &warn, 
                                             0, 0, image_data, static_cast<int>(image_size), nullptr);
    
    // Test with as_is flag set
    image.as_is = images_as_is;
    
    // Test with different requested dimensions
    if (Size > 5) {
      int req_width = static_cast<int>(Data[1]) * 256 + Data[2];
      int req_height = static_cast<int>(Data[3]) * 256 + Data[4];
      load_result = tinygltf::LoadImageData(&image, 1, &err, &warn,
                                           req_width, req_height, image_data, 
                                           static_cast<int>(image_size), nullptr);
    }
  }
  
  // Test with different boolean values
  loader.SetImagesAsIs(!images_as_is);
  loader.SetPreserveImageChannels(!preserve_channels);
  
  // Retrieve again to ensure state changed
  bool final_as_is = loader.GetImagesAsIs();
  bool final_preserve = loader.GetPreserveImageChannels();
  
  return 0;
}