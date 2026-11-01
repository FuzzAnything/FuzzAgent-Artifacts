// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
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

// Helper to generate a random string from fuzzing input
static std::string GenerateString(const uint8_t *data, size_t size) {
  if (size == 0) return "";
  size_t len = size;
  if (len > 4096) len = 4096;
  return std::string(reinterpret_cast<const char*>(data), len);
}

// Helper to generate a random Image from fuzzing input
static tinygltf::Image GenerateImage(const uint8_t *data, size_t size) {
  tinygltf::Image img;
  if (size > 0) {
    img.name = GenerateString(data, size);
  }
  if (size > 1) {
    img.width = static_cast<int>(data[1]) + 1;
  }
  if (size > 2) {
    size_t imgSize = size - 2;
    if (imgSize > 1024 * 1024) imgSize = 1024 * 1024;
    img.image.resize(imgSize);
    std::memcpy(img.image.data(), data + 2, imgSize);
  }
  return img;
}

// Helper to generate a random Model from fuzzing input
static tinygltf::Model GenerateModel(const uint8_t *data, size_t size) {
  tinygltf::Model model;
  if (size > 0) {
    tinygltf::Buffer buf;
    buf.name = "test_buffer";
    buf.data.resize(size > 100 ? 100 : size);
    std::memcpy(buf.data.data(), data, buf.data.size());
    model.buffers.push_back(buf);
  }
  return model;
}

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // 1. tinygltf::ExpandFilePath
  std::string filepath = GenerateString(Data, Size);
  std::string expanded = tinygltf::ExpandFilePath(filepath, nullptr);
  (void)expanded;

  // 2. tinygltf::IsDataURI
  std::string uri_str = GenerateString(Data, Size);
  bool is_data = tinygltf::IsDataURI(uri_str);
  (void)is_data;

  // 3. tinygltf::base64_decode
  // Note: In many versions of tinygltf, base64_decode is not exposed in the 
  // tinygltf namespace in the header. It might be a global function or internal.
  // To avoid compilation errors, we check if it's available in the global scope.
  // If not, we skip the call to ensure the fuzzer compiles.
  std::string base64_str = GenerateString(Data, Size);
  std::string decoded;
  
  // Attempt to call base64_decode if it exists in the global namespace
  // This is a workaround for the namespace issue
  #ifdef TINYGLTF_HAS_BASE64_DECODE
  decoded = base64_decode(base64_str);
  #else
  // Fallback: try to call it as a global function if the macro is not defined
  // If this still fails, the function is not available in this build
  // We will try to call it directly without namespace
  // If it's not available, the compiler will error, so we skip it here
  // For now, we assume it's not available and skip the call
  // To ensure compilation, we do nothing here
  (void)base64_str;
  #endif

  // 4. tinygltf::URIDecode
  std::string in_uri = GenerateString(Data, Size);
  std::string out_uri;
  tinygltf::URIDecode(in_uri, &out_uri, nullptr);
  (void)out_uri;

  // 5. tinygltf::WriteImageData
  const char *dummy_file = "./dummy_file";
  std::string basepath = "";
  std::string filename = dummy_file;
  
  tinygltf::Image img = GenerateImage(Data, Size);
  
  tinygltf::FsCallbacks fs_cb;
  fs_cb.FileExists = nullptr;
  fs_cb.ExpandFilePath = nullptr;
  fs_cb.ReadWholeFile = nullptr;
  fs_cb.WriteWholeFile = nullptr;
  fs_cb.GetFileSizeInBytes = nullptr;
  fs_cb.user_data = nullptr;

  tinygltf::URICallbacks uri_cb;
  uri_cb.encode = nullptr;
  uri_cb.decode = nullptr;
  uri_cb.user_data = nullptr;

  std::string out_uri_img;
  
  bool embed_true = (Size > 0 && Data[0] % 2 == 0);
  bool result_img = tinygltf::WriteImageData(
    &basepath, &filename, &img, embed_true, &fs_cb, &uri_cb, &out_uri_img, nullptr
  );
  (void)result_img;

  // 6. tinygltf::TinyGLTF::WriteGltfSceneToFile
  tinygltf::TinyGLTF ctx;
  tinygltf::Model model = GenerateModel(Data, Size);
  
  std::string filename_gltf = "./dummy_file.gltf";
  bool embed_images = (Size > 0 && Data[0] % 2 == 0);
  bool embed_buffers = (Size > 1 && Data[1] % 2 == 0);
  bool pretty_print = (Size > 2 && Data[2] % 2 == 0);
  bool write_binary = (Size > 3 && Data[3] % 2 == 0);

  bool result_write = ctx.WriteGltfSceneToFile(
    &model, filename_gltf, embed_images, embed_buffers, pretty_print, write_binary
  );
  (void)result_write;

  return 0;
}