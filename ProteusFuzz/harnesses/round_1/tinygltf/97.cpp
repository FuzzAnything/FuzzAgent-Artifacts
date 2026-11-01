// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
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

// Helper to handle the static function WriteToMemory_stbi
// Since it is static in tiny_gltf.h, we cannot call it directly from outside.
// We implement a local version that mimics the behavior for fuzzing purposes.
static void WriteToMemory_stbi_local(void *context, void *data, int size) {
  std::vector<unsigned char> *vec = reinterpret_cast<std::vector<unsigned char>*>(context);
  if (!vec || !data || size <= 0) return;
  const unsigned char *src = static_cast<const unsigned char*>(data);
  vec->insert(vec->end(), src, src + size);
}

// Helper to handle base64_decode if it's not in the public namespace
// In some versions, it might be static or in an anonymous namespace.
// We implement a local version that mimics the behavior for fuzzing purposes.
static std::string base64_decode_local(const std::string &s) {
  static const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";
  
  std::string ret;
  int in_len = s.size();
  int i = 0;
  int j = 0;
  unsigned char char_array_4[4];
  unsigned char char_array_3[3];

  while (in_len-- && (s[i] != '=') && isalnum(s[i])) {
    char_array_4[j++] = base64_chars.find(s[i]);
    if (j == 4) {
      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0x0f) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];

      for (j = 0; (j < 3); j++)
        ret += char_array_3[j];
      j = 0;
    }
    i++;
  }

  if (j) {
    for (int k = j; k < 4; k++)
      char_array_4[k] = 0;

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0x0f) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];

    for (int k = 0; (k < j - 1); k++)
      ret += char_array_3[k];
  }

  return ret;
}

extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // Split input into sections for different tests
  size_t section_size = Size / 4;
  const uint8_t *data1 = Data;
  const uint8_t *data2 = Data + section_size;
  const uint8_t *data3 = Data + 2 * section_size;
  const uint8_t *data4 = Data + 3 * section_size;
  size_t size1 = section_size;
  size_t size2 = section_size;
  size_t size3 = section_size;
  size_t size4 = Size - 3 * section_size;

  // Test 1: tinygltf::base64_decode (using local implementation)
  {
    std::string input(reinterpret_cast<const char*>(data1), size1);
    // Overload 1
    std::string decoded1 = base64_decode_local(input);
    // Overload 2 (same signature effectively, but explicitly called)
    std::string decoded2 = base64_decode_local(input);
    (void)decoded1;
    (void)decoded2;
  }

  // Test 2: tinygltf::LoadImageData
  {
    tinygltf::Image image;
    std::string err;
    std::string warn;
    int req_width = 0;
    int req_height = 0;
    void *user_data = nullptr;

    // Overload 1 (with user_data)
    bool success1 = tinygltf::LoadImageData(
        &image, 0, &err, &warn, req_width, req_height,
        data2, static_cast<int>(size2), user_data);
    
    // Reset image for second call
    image = tinygltf::Image();
    err.clear();
    warn.clear();

    // Overload 2 (without user_data, passes nullptr implicitly or explicitly)
    bool success2 = tinygltf::LoadImageData(
        &image, 0, &err, &warn, req_width, req_height,
        data2, static_cast<int>(size2), nullptr);
    
    (void)success1;
    (void)success2;
  }

  // Test 3: tinygltf::WriteToMemory_stbi (using local implementation)
  {
    std::vector<unsigned char> buffer;
    // Ensure buffer is not empty to avoid issues if size is 0
    if (size3 > 0) {
      WriteToMemory_stbi_local(&buffer, const_cast<void*>(static_cast<const void*>(data3)), static_cast<int>(size3));
    }
  }

  // Test 4: tinygltf::WriteImageData
  {
    tinygltf::Image image;
    // Populate image with some data from the fuzz input if available
    if (size4 > 0) {
      image.image.assign(data4, data4 + size4);
      image.width = 1;
      image.height = 1;
      image.bits = 8;
      image.component = 1;
    }

    std::string basepath = "./";
    std::string filename = "dummy_file";
    std::string out_uri;
    bool embedImages = true;

    tinygltf::FsCallbacks fs_cb;
    tinygltf::URICallbacks uri_cb;
    void *user_data = nullptr;

    // Initialize callbacks to nullptr or default
    fs_cb.FileExists = nullptr;
    fs_cb.ExpandFilePath = nullptr;
    fs_cb.ReadWholeFile = nullptr;
    fs_cb.WriteWholeFile = nullptr;
    fs_cb.GetFileSizeInBytes = nullptr;
    fs_cb.user_data = nullptr;

    uri_cb.encode = nullptr;
    uri_cb.decode = nullptr;
    uri_cb.user_data = nullptr;

    bool success = tinygltf::WriteImageData(
        &basepath, &filename, &image, embedImages,
        &fs_cb, &uri_cb, &out_uri, user_data);
    (void)success;
  }

  return 0;
}