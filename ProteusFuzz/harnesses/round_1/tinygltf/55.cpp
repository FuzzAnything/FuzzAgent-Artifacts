// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
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

// Dummy callback implementations for FsCallbacks
// Note: tinygltf uses std::function for callbacks. We must match the signatures exactly.
static bool DummyFileExists(const std::string &filename, void *) {
  return false;
}

static std::string DummyExpandFilePath(const std::string &filepath, void *) {
  return filepath;
}

static bool DummyReadWholeFile(std::vector<unsigned char> *out,
                               std::string *err, const std::string &filename,
                               void *) {
  if (err) *err = "DummyReadWholeFile: Not implemented";
  return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents,
                                void *) {
  // Actually write to disk to allow fuzzing file operations if needed
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    if (err) *err = "Failed to open file for writing";
    return false;
  }
  file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
  return true;
}

// Correct signature for GetFileSizeInBytes based on the error message:
// It expects: bool(size_t*, std::string*, const std::string&, void*)
// The function should write the size to the first pointer and return true on success.
static bool DummyGetFileSizeInBytes(size_t *size, std::string *err,
                                    const std::string &filename, void *) {
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  if (!file) {
    if (err) *err = "Failed to open file for size check";
    return false;
  }
  size_t s = static_cast<size_t>(file.tellg());
  if (size) *size = s;
  return true;
}

// Dummy callback implementations for URICallbacks
static bool DummyURIEncode(const std::string &uri, const std::string &base, std::string *out, void *) {
  if (out) *out = uri;
  return true;
}

static bool DummyURIDecode(const std::string &uri, std::string *out, void *) {
  if (out) *out = uri;
  return true;
}

// Helper to write data to a dummy file
static void WriteDummyFile(const uint8_t *data, size_t size) {
  std::ofstream file("./dummy_file", std::ios::binary);
  if (file.is_open()) {
    file.write(reinterpret_cast<const char *>(data), size);
  }
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // Initialize TinyGLTF instance
  tinygltf::TinyGLTF ctx;
  std::string err;

  // Prepare callbacks
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

  // 1. Test SetFsCallbacks
  ctx.SetFsCallbacks(fs_cb, &err);

  // 2. Test SetURICallbacks with valid and invalid callbacks
  ctx.SetURICallbacks(uri_cb, &err);

  // Test with null encode callback
  tinygltf::URICallbacks invalid_uri_cb = uri_cb;
  invalid_uri_cb.encode = nullptr;
  ctx.SetURICallbacks(invalid_uri_cb, &err);

  // Test with null decode callback
  invalid_uri_cb.encode = DummyURIEncode;
  invalid_uri_cb.decode = nullptr;
  ctx.SetURICallbacks(invalid_uri_cb, &err);

  // Prepare Model for writing
  tinygltf::Model model;
  model.asset.version = "2.0";

  // Create dummy image data from input
  tinygltf::Image image;
  image.name = "dummy_image";
  image.width = 1;
  image.height = 1;
  if (Size > 0) {
    image.image.assign(Data, Data + Size);
  } else {
    image.image.push_back(0);
  }
  image.bits = 8;
  model.images.push_back(image);

  // 3. Test WriteImageData
  std::string basepath = "./";
  std::string filename = "dummy_image.png";
  std::string out_uri;
  tinygltf::WriteImageData(&basepath, &filename, &image, false, &fs_cb,
                           &uri_cb, &out_uri, nullptr);

  // Test with embedImages = true
  tinygltf::WriteImageData(&basepath, &filename, &image, true, &fs_cb,
                           &uri_cb, &out_uri, nullptr);

  // 5. Test WriteGltfSceneToFile
  ctx.WriteGltfSceneToFile(&model, "./dummy_file.gltf", false, false, true, false);
  ctx.WriteGltfSceneToFile(&model, "./dummy_file.gltf", true, true, true, false);
  ctx.WriteGltfSceneToFile(&model, "./dummy_file.glb", false, false, true, true);

  // 6. Test WriteGltfSceneToStream
  std::ostringstream stream;
  ctx.WriteGltfSceneToStream(&model, stream, true, false);
  ctx.WriteGltfSceneToStream(&model, stream, false, true);

  // Write dummy file for potential file-based callbacks
  WriteDummyFile(Data, Size);

  return 0;
}