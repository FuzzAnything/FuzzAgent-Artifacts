// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
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
#include <fstream>
#include "tiny_gltf.h"

// Dummy file callbacks to satisfy FsCallbacks requirements
static bool DummyFileExists(const std::string &filename, void *) {
  return false;
}

static std::string DummyExpandFilePath(const std::string &filepath, void *) {
  return filepath;
}

static bool DummyReadWholeFile(std::vector<unsigned char> *out,
                               std::string *err, const std::string &filename,
                               void *) {
  out->clear();
  if (err) *err = "";
  return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents,
                                void *) {
  if (err) *err = "";
  std::ofstream ofs(filename, std::ios::binary);
  if (!ofs) {
    if (err) *err = "Failed to open file";
    return false;
  }
  ofs.write(reinterpret_cast<const char*>(contents.data()), contents.size());
  return true;
}

// Correct signature for GetFileSizeInBytes: (size_t*, std::string*, const std::string&, void*)
static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename,
                                    void *) {
  if (size) *size = 0;
  if (err) *err = "";
  return false;
}

// Dummy URI callbacks
// Correct signature for URIEncodeFunction: (const std::string&, const std::string&, std::string*, void*)
static bool DummyURIEncode(const std::string &in_uri, const std::string &in_filename,
                           std::string *out, void *) {
  if (out) *out = in_uri;
  return true;
}

static bool DummyURIDecode(const std::string &in_uri, std::string *out,
                           void *) {
  if (out) *out = in_uri;
  return true;
}

// Dummy LoadImageDataFunction
static bool DummyLoadImageData(std::vector<unsigned char> *out, int *width,
                               int *height, int *component, int *pixel_type,
                               bool *sRGB, std::string *ext,
                               tinygltf::URICallbacks *uri_cb,
                               tinygltf::FsCallbacks *fs_cb,
                               const std::string &uri, const size_t index,
                               const tinygltf::Model *model, void *user_data) {
  return false;
}

// Dummy WriteImageDataFunction
static bool DummyWriteImageData(const std::string *basepath,
                                const std::string *filename,
                                const tinygltf::Image *image,
                                bool embedImages,
                                const tinygltf::FsCallbacks *fs_cb,
                                const tinygltf::URICallbacks *uri_cb,
                                std::string *out_uri, void *user_data) {
  return false;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // Initialize callbacks
  tinygltf::FsCallbacks fs_cb;
  fs_cb.FileExists = DummyFileExists;
  fs_cb.ExpandFilePath = DummyExpandFilePath;
  fs_cb.ReadWholeFile = DummyReadWholeFile;
  fs_cb.WriteWholeFile = DummyWriteWholeFile;
  fs_cb.GetFileSizeInBytes = DummyGetFileSizeInBytes;
  fs_cb.user_data = nullptr;

  tinygltf::URICallbacks uri_cb;
  uri_cb.encode = DummyURIEncode;
  uri_cb.decode = DummyURIDecode;
  uri_cb.user_data = nullptr;

  // 1. Test IsDataURI
  std::string input_str(reinterpret_cast<const char*>(Data), Size);
  tinygltf::IsDataURI(input_str);

  // 2. Test DecodeDataURI
  std::vector<unsigned char> decoded_out;
  std::string mime_type;
  tinygltf::DecodeDataURI(&decoded_out, mime_type, input_str, 0, false);
  tinygltf::DecodeDataURI(&decoded_out, mime_type, input_str, Size, true);

  // 3. Test URIDecode
  std::string decoded_uri;
  tinygltf::URIDecode(input_str, &decoded_uri, nullptr);

  // 4. Test WriteImageData
  tinygltf::Image image;
  image.width = static_cast<int>(Size % 1024);
  image.height = static_cast<int>(Size % 1024);
  image.image.assign(Data, Data + Size);
  image.name = "test_image";
  
  std::string basepath = "./dummy_file";
  std::string filename = "test.png";
  std::string out_uri;
  
  // Try with embedImages = false
  tinygltf::WriteImageData(&basepath, &filename, &image, false, &fs_cb, &uri_cb, &out_uri, nullptr);
  
  // Try with embedImages = true
  tinygltf::WriteImageData(&basepath, &filename, &image, true, &fs_cb, &uri_cb, &out_uri, nullptr);

  // 5. Test ParseImage
  try {
    #if defined(TINYGLTF_HAS_NLOHMANN_JSON)
    // Create a simple JSON object representing an image
    nlohmann::json o;
    o["uri"] = input_str; 
    o["mimeType"] = "image/png";
    
    std::string err;
    std::string warn;
    std::string basedir = "./dummy_file";
    size_t max_file_size = 1024 * 1024; 
    
    tinygltf::ParseImage(&image, 0, &err, &warn, o, false, basedir, max_file_size,
                         &fs_cb, &uri_cb, DummyLoadImageData, nullptr);

    // Try with bufferView instead of uri
    nlohmann::json o2;
    o2["bufferView"] = 0;
    tinygltf::ParseImage(&image, 0, &err, &warn, o2, false, basedir, max_file_size,
                         &fs_cb, &uri_cb, DummyLoadImageData, nullptr);
    
    // Try with both (should fail)
    nlohmann::json o3;
    o3["uri"] = input_str;
    o3["bufferView"] = 0;
    tinygltf::ParseImage(&image, 0, &err, &warn, o3, false, basedir, max_file_size,
                         &fs_cb, &uri_cb, DummyLoadImageData, nullptr);

    // Try with neither (should fail)
    nlohmann::json o4;
    tinygltf::ParseImage(&image, 0, &err, &warn, o4, false, basedir, max_file_size,
                         &fs_cb, &uri_cb, DummyLoadImageData, nullptr);
    #endif
  } catch (...) {
    // Ignore exceptions during JSON construction/parsing
  }

  // 6. Test UpdateImageObject
  // Note: UpdateImageObject is static in detail namespace or not exposed in public API in some versions.
  // If it's not accessible as tinygltf::UpdateImageObject, we skip it to avoid compilation errors.
  // Based on the error "no member named 'UpdateImageObject' in namespace 'tinygltf'", 
  // it seems it's not directly accessible or named differently in the public header.
  // We will skip this specific call to ensure compilation, as the task requires a compiling driver.
  // If the function exists in a specific version, it would need to be called via the correct namespace.
  // For now, we omit it to satisfy the compilation requirement.

  return 0;
}