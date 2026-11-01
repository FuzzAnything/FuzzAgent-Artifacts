// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
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
#include <functional>
#include <fstream>
#include "tiny_gltf.h"

// Forward declarations for callback functions matching tinygltf types
static bool FileExists(const std::string &filename, void *user_data) {
  return false;
}

static std::string ExpandFilePath(const std::string &filepath, void *user_data) {
  return filepath;
}

static bool ReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                          const std::string &filename, void *user_data) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    if (err) *err = "File not found";
    return false;
  }
  out->assign((std::istreambuf_iterator<char>(file)),
              std::istreambuf_iterator<char>());
  return true;
}

static bool WriteWholeFile(std::string *err, const std::string &filename,
                           const std::vector<unsigned char> &contents,
                           void *user_data) {
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    if (err) *err = "Failed to open file for writing";
    return false;
  }
  file.write(reinterpret_cast<const char *>(contents.data()), contents.size());
  return true;
}

// Correct signature for GetFileSizeInBytes based on tinygltf.h
// It returns bool and fills size via pointer
static bool GetFileSizeInBytes(size_t *size, std::string *err,
                               const std::string &filename, void *user_data) {
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  if (!file) {
    if (err) *err = "File not found";
    return false;
  }
  *size = static_cast<size_t>(file.tellg());
  return true;
}

// Correct signature for URICallbacks
// encode: (const std::string&, const std::string&, std::string*, void*) -> bool
static bool URICallbacksEncode(const std::string &in, const std::string &base,
                               std::string *out, void *user_data) {
  if (out) *out = in;
  return true;
}

// decode: (const std::string&, std::string*, void*) -> bool
static bool URICallbacksDecode(const std::string &in, std::string *out,
                               void *user_data) {
  if (out) *out = in;
  return true;
}

static bool DummyWriteImageData(const std::string *basepath,
                                const std::string *filename,
                                const tinygltf::Image *image, bool embedImages,
                                const tinygltf::FsCallbacks *fs_cb,
                                const tinygltf::URICallbacks *uri_cb,
                                std::string *out_uri, void *) {
  if (embedImages && out_uri) {
    out_uri->clear();
    return true;
  }
  return true;
}

static bool DummyLoadImageData(tinygltf::Image *image, const int image_idx,
                               std::string *err, std::string *warn,
                               int req_width, int req_height,
                               const unsigned char *bytes, int size,
                               void *user_data) {
  if (!bytes || size <= 0) {
    if (err) *err = "Invalid image data";
    return false;
  }
  image->width = 1;
  image->height = 1;
  image->image.assign(bytes, bytes + size);
  return true;
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // Initialize callbacks with correct types
  tinygltf::FsCallbacks fs_cb;
  fs_cb.FileExists = FileExists;
  fs_cb.ExpandFilePath = ExpandFilePath;
  fs_cb.ReadWholeFile = ReadWholeFile;
  fs_cb.WriteWholeFile = WriteWholeFile;
  fs_cb.GetFileSizeInBytes = GetFileSizeInBytes;
  fs_cb.user_data = nullptr;

  tinygltf::URICallbacks uri_cb;
  uri_cb.encode = URICallbacksEncode;
  uri_cb.decode = URICallbacksDecode;
  uri_cb.user_data = nullptr;

  std::string input(reinterpret_cast<const char *>(Data), Size);

  // Fuzz IsDataURI
  tinygltf::IsDataURI(input);

  // Fuzz DecodeDataURI
  std::vector<unsigned char> decoded_out;
  std::string mime_type;
  tinygltf::DecodeDataURI(&decoded_out, mime_type, input, 0, false);

  // Fuzz LoadImageData
  tinygltf::Image img;
  std::string err, warn;
  int req_width = 0;
  int req_height = 0;
  int size_int = static_cast<int>(Size);
  if (size_int > 0) {
    tinygltf::LoadImageData(&img, 0, &err, &warn, req_width, req_height, Data,
                            size_int, nullptr);
  }

  // Fuzz WriteImageData
  std::string basepath = "./dummy_file";
  std::string filename = "test.png";
  std::string out_uri;
  bool embedImages = (Size > 100);
  tinygltf::WriteImageData(&basepath, &filename, &img, embedImages, &fs_cb,
                           &uri_cb, &out_uri, nullptr);

  // Note: UpdateImageObject is static inside the library and not exposed in the public namespace
  // in the standard tinygltf.h header for direct external calling in many versions.
  // It is typically used internally. We skip direct call to avoid compilation errors
  // unless the specific version exposes it. Based on error, it's not available.

  // Note: LoadExternalFile is also static and internal in many versions.
  // We skip direct call to avoid compilation errors.

  return 0;
}