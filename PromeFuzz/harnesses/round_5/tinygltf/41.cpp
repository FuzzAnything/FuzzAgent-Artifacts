// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::GetNumComponentsInType at tiny_gltf.h:222:23 in tiny_gltf.h
// tinygltf::GetNumComponentsInType at tiny_gltf.h:222:23 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::GetComponentSizeInBytes at tiny_gltf.h:199:23 in tiny_gltf.h
// tinygltf::GetComponentSizeInBytes at tiny_gltf.h:199:23 in tiny_gltf.h
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

static void fuzz_GetNumComponentsInType(const uint8_t *data, size_t size) {
  if (size == 0) return;
  
  // Use first byte as type parameter
  uint32_t ty = static_cast<uint32_t>(data[0]);
  tinygltf::GetNumComponentsInType(ty);
  
  // Try with more values from the input
  for (size_t i = 1; i < std::min(size, static_cast<size_t>(4)); i++) {
    uint32_t ty2 = static_cast<uint32_t>(data[i]);
    tinygltf::GetNumComponentsInType(ty2);
  }
}

static void fuzz_DecodeDataURI(const uint8_t *data, size_t size) {
  if (size == 0) return;
  
  std::vector<unsigned char> out;
  std::string mime_type;
  
  // Create input string from fuzzer data
  std::string in_str(reinterpret_cast<const char*>(data), size);
  
  // Try different combinations of parameters
  bool checkSize = (size % 2 == 0);
  size_t reqBytes = size / 2;
  
  // First call
  tinygltf::DecodeDataURI(&out, mime_type, in_str, reqBytes, checkSize);
  
  // Try with different parameters
  tinygltf::DecodeDataURI(&out, mime_type, in_str, 0, false);
  tinygltf::DecodeDataURI(&out, mime_type, in_str, size, true);
  
  // Try with a substring
  if (size > 10) {
    std::string sub_str = in_str.substr(0, size / 2);
    tinygltf::DecodeDataURI(&out, mime_type, sub_str, reqBytes, !checkSize);
  }
}

static void fuzz_SetURICallbacks(const uint8_t *data, size_t size) {
  tinygltf::TinyGLTF loader;
  std::string err;
  
  // Create callbacks with different configurations
  tinygltf::URICallbacks callbacks1;
  callbacks1.decode = tinygltf::URIDecode;
  callbacks1.encode = nullptr;
  callbacks1.user_data = nullptr;
  
  loader.SetURICallbacks(callbacks1, &err);
  
  // Try with null decode function
  tinygltf::URICallbacks callbacks2;
  callbacks2.decode = nullptr;
  callbacks2.encode = nullptr;
  callbacks2.user_data = reinterpret_cast<void*>(const_cast<uint8_t*>(data));
  
  loader.SetURICallbacks(callbacks2, nullptr);
  
  // Try without error string
  loader.SetURICallbacks(callbacks1, nullptr);
}

static void fuzz_GetComponentSizeInBytes(const uint8_t *data, size_t size) {
  if (size == 0) return;
  
  // Use first byte as component type
  uint32_t componentType = static_cast<uint32_t>(data[0]);
  tinygltf::GetComponentSizeInBytes(componentType);
  
  // Try with more values from the input
  for (size_t i = 1; i < std::min(size, static_cast<size_t>(4)); i++) {
    uint32_t componentType2 = static_cast<uint32_t>(data[i]);
    tinygltf::GetComponentSizeInBytes(componentType2);
  }
}

static void fuzz_URIDecode(const uint8_t *data, size_t size) {
  if (size == 0) return;
  
  std::string out_uri;
  
  // Create input string from fuzzer data
  std::string in_uri(reinterpret_cast<const char*>(data), size);
  
  // Try with different user_data values
  tinygltf::URIDecode(in_uri, &out_uri, nullptr);
  
  // Try with user_data pointing to the input
  tinygltf::URIDecode(in_uri, &out_uri, reinterpret_cast<void*>(const_cast<uint8_t*>(data)));
  
  // Try with a substring
  if (size > 10) {
    std::string sub_uri = in_uri.substr(0, size / 2);
    tinygltf::URIDecode(sub_uri, &out_uri, nullptr);
  }
}

static void fuzz_LoadImageData(const uint8_t *data, size_t size) {
  if (size == 0) return;
  
  tinygltf::Image image;
  std::string err, warn;
  
  // Try different combinations of parameters
  int req_width = static_cast<int>(data[0]) * 10;
  int req_height = static_cast<int>(size > 1 ? data[1] : data[0]) * 10;
  
  // First call
  tinygltf::LoadImageData(&image, 0, &err, &warn, req_width, req_height, data, static_cast<int>(size), nullptr);
  
  // Try with different parameters
  tinygltf::LoadImageData(&image, 1, nullptr, nullptr, 0, 0, data, static_cast<int>(size), nullptr);
  
  // Try with as_is flag set
  image.as_is = true;
  tinygltf::LoadImageData(&image, 2, &err, &warn, -1, -1, data, static_cast<int>(size), nullptr);
  
  // Try with a subset of the data
  if (size > 100) {
    const unsigned char* subset = data + 50;
    int subset_size = static_cast<int>(size - 50);
    tinygltf::LoadImageData(&image, 3, &err, &warn, 100, 100, subset, subset_size, nullptr);
  }
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
  // Fuzz each API function with the input data
  fuzz_GetNumComponentsInType(Data, Size);
  fuzz_DecodeDataURI(Data, Size);
  fuzz_SetURICallbacks(Data, Size);
  fuzz_GetComponentSizeInBytes(Data, Size);
  fuzz_URIDecode(Data, Size);
  fuzz_LoadImageData(Data, Size);
  
  return 0;
}