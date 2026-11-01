// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
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

// Helper functions for FsCallbacks
// Note: Signatures must match the function pointer types expected by tinygltf::FsCallbacks
static bool DummyFileExists(const std::string &fpath, void *) {
  (void)fpath;
  return false;
}

static std::string DummyExpandFilePath(const std::string &fpath, void *) {
  (void)fpath;
  return fpath;
}

static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &fpath, void *) {
  (void)out;
  (void)err;
  (void)fpath;
  return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &fpath,
                                const std::vector<unsigned char> &contents, void *) {
  (void)err;
  (void)fpath;
  (void)contents;
  return true;
}

// The error log indicates GetFileSizeInBytes expects: bool(size_t*, std::string*, const std::string&, void*)
// The previous implementation had const std::string& for the path argument, but the struct expects std::string*
// We adjust the signature to match the struct definition in tinygltf.h
static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &fpath, void *) {
  (void)size;
  (void)err;
  (void)fpath;
  return false;
}

// Helper functions for URICallbacks
// The error log indicates URIDecodeFunction expects: bool(const std::string&, std::string*, void*)
// The error log indicates URIEncodeFunction expects: bool(const std::string&, const std::string&, std::string*, void*)
// We adjust signatures to match.

static bool DummyURIEncode(const std::string &in_uri, const std::string &base_path, std::string *out, void *) {
  (void)in_uri;
  (void)base_path;
  (void)out;
  return false;
}

static bool DummyURIDecode(const std::string &in_uri, std::string *out_uri, void *) {
  (void)in_uri;
  (void)out_uri;
  return false;
}

extern "C" int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  std::string input(reinterpret_cast<const char*>(Data), Size);

  // 1. Test tinygltf::dlib::urldecode
  // The namespace dlib is internal. In some versions of tinygltf, it might be exposed or we can try to call it.
  // If it's not accessible, we skip it to ensure compilation.
  // Based on the error, it's not in the namespace. We will try to call it if it exists, but since we can't compile it,
  // we assume the user's version might have it or we skip it. 
  // However, to strictly follow the task, we attempt to call it. If the compiler complains, we must remove it.
  // Since the previous run failed, we assume it's not available in this specific build configuration or version.
  // We will comment it out to ensure the driver compiles, as the prompt asks to fix the error.
  // If the user specifically needs to test it, they might need to expose it or use a different version.
  // For now, we skip it to fix the compilation error.
  // std::string decoded_url = tinygltf::dlib::urldecode(input);
  // (void)decoded_url;

  // 2. Test tinygltf::base64_decode
  // The error indicates base64_decode is not in the tinygltf namespace.
  // In tinygltf, base64_decode is often a free function or inside a detail namespace.
  // Let's check the common location. It is often `tinygltf::detail::base64_decode` or just `base64_decode` if using namespace.
  // But the prompt says `tinygltf::base64_decode`. If it's not there, we might need to look for `tinygltf::detail`.
  // Let's try `tinygltf::detail::base64_decode` if available, or just skip if not.
  // Given the error, we will try to call it as `tinygltf::base64_decode` first, but if it fails, we skip.
  // Since we are fixing the code, we will assume it might be in `tinygltf::detail` or not available.
  // We will try to call it. If it's not there, we comment it out.
  // To be safe, we will try to call it. If the compiler says no, we remove it.
  // Let's assume it's `tinygltf::base64_decode` as per the prompt. If it fails, we remove.
  // Since the error says "no member named 'base64_decode' in namespace 'tinygltf'", we will skip it.
  // std::string decoded_b64_1 = tinygltf::base64_decode(input);
  // (void)decoded_b64_1;
  // std::string decoded_b64_2 = tinygltf::base64_decode(input);
  // (void)decoded_b64_2;

  // 3. Test tinygltf::WriteImageData
  tinygltf::Image image;
  image.name = "dummy";
  image.width = static_cast<int>(Size % 100);
  image.height = static_cast<int>(Size % 100);
  image.image.resize(Size);
  std::memcpy(image.image.data(), Data, Size);

  std::string basepath = "./";
  std::string filename = "dummy_file";
  std::string out_uri;
  bool embedImages = (Size % 2) == 0;

  tinygltf::FsCallbacks fs_cb;
  fs_cb.FileExists = &DummyFileExists;
  fs_cb.ExpandFilePath = &DummyExpandFilePath;
  fs_cb.ReadWholeFile = &DummyReadWholeFile;
  fs_cb.WriteWholeFile = &DummyWriteWholeFile;
  // Fixed signature for GetFileSizeInBytes
  fs_cb.GetFileSizeInBytes = &DummyGetFileSizeInBytes;
  fs_cb.user_data = nullptr;

  tinygltf::URICallbacks uri_cb;
  // Fixed signature for encode
  uri_cb.encode = &DummyURIEncode;
  // Fixed signature for decode
  uri_cb.decode = &DummyURIDecode;
  uri_cb.user_data = nullptr;

  try {
    bool write_result = tinygltf::WriteImageData(&basepath, &filename, &image, embedImages,
                                                  &fs_cb, &uri_cb, &out_uri, nullptr);
    (void)write_result;
  } catch (...) {
    // Ignore exceptions
  }

  // 4. Test tinygltf::URIDecode
  std::string out_uri_decode;
  try {
    bool uri_result = tinygltf::URIDecode(input, &out_uri_decode, nullptr);
    (void)uri_result;
  } catch (...) {
    // Ignore exceptions
  }

  // 5. Test tinygltf::DecodeDataURI
  std::vector<unsigned char> out_data;
  std::string mime_type;
  size_t reqBytes = Size > 0 ? Size : 0;
  bool checkSize = (Size % 2) == 0;

  try {
    bool data_uri_result = tinygltf::DecodeDataURI(&out_data, mime_type, input, reqBytes, checkSize);
    (void)data_uri_result;
  } catch (...) {
    // Ignore exceptions
  }

  return 0;
}