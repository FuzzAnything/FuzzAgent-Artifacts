// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
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
#include "tiny_gltf.h"

// Helper functions for FsCallbacks
static bool DummyFileExists(const std::string &f, void *) { return false; }

static std::string DummyExpandFilePath(const std::string &f, void *) { return f; }

static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &filename, void *) {
  (void)filename;
  out->clear();
  if (err) *err = "";
  return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents, void *) {
  (void)filename;
  (void)contents;
  if (err) *err = "";
  return true;
}

static bool DummyGetFileSizeInBytes(unsigned long *size, std::string *err,
                                    const std::string &filename, void *) {
  (void)filename;
  if (size) *size = 0;
  if (err) *err = "";
  return false;
}

// Helper functions for URICallbacks
// Based on the error message, the expected signature for encode is:
// bool(const std::string &, const std::string &, std::string *, void *)
// And for decode:
// bool(const std::string &, std::string *, void *)
// This is different from the standard tinygltf.h in many versions, so we adapt.
static bool DummyURIEncode(const std::string &uri, const std::string &basepath, std::string *out, void *) {
  if (out) *out = uri;
  return true;
}

static bool DummyURIDecode(const std::string &uri, std::string *out, void *) {
  if (out) *out = uri;
  return true;
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // 1. is_base64
  // Since it's static inline and not found in namespace, we implement a local version
  // or skip it if we can't access it. Given the error, we skip direct access.
  // We can implement a local check if needed, but the task is to fuzz the library.
  // If the library doesn't expose it, we can't fuzz it directly.
  // However, base64_decode and base64_encode are also not found.
  // This suggests the version of tinygltf.h might be different or these are private.
  // Let's try to use the public API that might use these internally.
  // But the task specifically asks to fuzz these functions.
  // If they are not in the namespace, we cannot call them.
  // We will skip these specific calls and focus on the others.

  // 2. DecodeDataURI
  std::vector<unsigned char> out;
  std::string mime_type;
  std::string input_str(reinterpret_cast<const char *>(Data), Size);
  
  // Try with different reqBytes and checkSize
  size_t max_req = (Size > 100) ? 100 : Size;
  for (size_t req = 0; req < max_req; ++req) {
    tinygltf::DecodeDataURI(&out, mime_type, input_str, req, true);
    tinygltf::DecodeDataURI(&out, mime_type, input_str, req, false);
  }

  // 3. LoadImageData
  tinygltf::Image image;
  std::string err, warn;
  
  // Prepare image bytes
  const unsigned char *img_bytes = Data;
  int img_size = static_cast<int>(Size);
  
  // Try various req_width/height
  for (int w = -1; w <= 2; ++w) {
    for (int h = -1; h <= 2; ++h) {
      tinygltf::LoadImageData(&image, 0, &err, &warn, w, h, img_bytes, img_size, nullptr);
    }
  }

  // 4. WriteImageData
  std::string basepath = "./";
  std::string filename = "dummy_file";
  std::string out_uri;
  
  tinygltf::FsCallbacks fs_cb;
  fs_cb.FileExists = DummyFileExists;
  fs_cb.ExpandFilePath = DummyExpandFilePath;
  fs_cb.ReadWholeFile = DummyReadWholeFile;
  fs_cb.WriteWholeFile = DummyWriteWholeFile;
  fs_cb.GetFileSizeInBytes = DummyGetFileSizeInBytes;
  fs_cb.user_data = nullptr;

  tinygltf::URICallbacks uri_cb;
  // Assigning function pointers with the correct signatures based on error messages
  uri_cb.encode = DummyURIEncode;
  uri_cb.decode = DummyURIDecode;
  uri_cb.user_data = nullptr;

  // Try with embedImages true and false
  tinygltf::WriteImageData(&basepath, &filename, &image, true, &fs_cb, &uri_cb, &out_uri, nullptr);
  tinygltf::WriteImageData(&basepath, &filename, &image, false, &fs_cb, &uri_cb, &out_uri, nullptr);

  // Test with empty image
  tinygltf::Image empty_image;
  tinygltf::WriteImageData(&basepath, &filename, &empty_image, true, &fs_cb, &uri_cb, &out_uri, nullptr);

  return 0;
}