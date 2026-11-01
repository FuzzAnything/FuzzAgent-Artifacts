// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
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

// Helper to manually implement base64 check if not exposed in namespace
static bool is_base64_char(unsigned char c) {
  return (c >= 'A' && c <= 'Z') ||
         (c >= 'a' && c <= 'z') ||
         (c >= '0' && c <= '9') ||
         (c == '+') || (c == '/');
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Scenario 1: Test DecodeDataURI
  {
    std::vector<unsigned char> out;
    std::string mime_type;
    
    // Construct a potential Data URI from input
    std::string data_uri = "data:image/png;base64,";
    
    // Filter input to only valid base64 chars to create a plausible URI for fuzzing
    for (size_t i = 0; i < Size; ++i) {
      if (is_base64_char(Data[i])) {
        data_uri += static_cast<char>(Data[i]);
      }
    }
    
    // Try to decode with various parameters
    tinygltf::DecodeDataURI(&out, mime_type, data_uri, 0, false);
    tinygltf::DecodeDataURI(&out, mime_type, data_uri, Size, true);
    tinygltf::DecodeDataURI(&out, mime_type, data_uri, Size, false);
    
    // Test with empty or malformed URI
    tinygltf::DecodeDataURI(&out, mime_type, "", 0, false);
    tinygltf::DecodeDataURI(&out, mime_type, "not_a_uri", 0, false);
  }

  // Scenario 2: Test LoadImageData
  {
    tinygltf::Image image;
    std::string err;
    std::string warn;
    
    // Try loading with different parameters
    tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0, Data, static_cast<int>(Size), nullptr);
    tinygltf::LoadImageData(&image, 1, &err, &warn, 100, 100, Data, static_cast<int>(Size), nullptr);
    tinygltf::LoadImageData(&image, -1, &err, &warn, -1, -1, Data, static_cast<int>(Size), nullptr);
    
    // Test with empty data
    tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0, nullptr, 0, nullptr);
    tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0, Data, 0, nullptr);
  }

  // Scenario 3: Test WriteImageData
  {
    tinygltf::Image image;
    image.width = static_cast<int>(Size % 1000);
    image.height = static_cast<int>(Size % 1000);
    image.image.assign(Data, Data + Size);
    image.name = "test_image";
    
    // Ensure non-null strings to prevent dereferencing null pointers inside tinygltf
    std::string basepath = "./dummy_file";
    std::string filename = "test.png";
    std::string out_uri;
    
    // Initialize callback structures with null pointers (will likely fail but test code paths)
    tinygltf::FsCallbacks fs_cb = {};
    tinygltf::URICallbacks uri_cb = {};
    
    // Test with embedImages true and false
    tinygltf::WriteImageData(&basepath, &filename, &image, true, &fs_cb, &uri_cb, &out_uri, nullptr);
    tinygltf::WriteImageData(&basepath, &filename, &image, false, &fs_cb, &uri_cb, &out_uri, nullptr);
    
    // Removed: Passing nullptr for image causes SEGV in tinygltf::WriteImageData
    // as it dereferences image->image.empty() without a null check.
  }

  return 0;
}