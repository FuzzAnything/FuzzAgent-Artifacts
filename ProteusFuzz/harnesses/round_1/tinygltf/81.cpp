// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
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
#include <functional>
#include "tiny_gltf.h"

// Dummy callbacks for FsCallbacks
static bool DummyFileExists(const std::string &filename, void *user_data) {
  return false;
}

static std::string DummyExpandFilePath(const std::string &filepath_in, void *user_data) {
  return filepath_in;
}

static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filename, void *user_data) {
  out->clear();
  err->clear();
  return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filename, const std::vector<unsigned char> &contents, void *user_data) {
  err->clear();
  return false;
}

static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *user_data) {
  if (size) *size = 0;
  if (err) err->clear();
  return false;
}

// Dummy callbacks for URICallbacks
static bool DummyURIEncode(const std::string &uri, const std::string &unused, std::string *out, void *user_data) {
  if (out) *out = uri;
  return true;
}

static bool DummyURIDecode(const std::string &uri, std::string *out, void *user_data) {
  if (out) *out = uri;
  return true;
}

// Dummy callback for ImageLoader
// Signature must match exactly: bool(Image*, const int, std::string*, std::string*, int, int, const unsigned char*, int, void*)
// Note the 5th and 6th arguments are int, not int*
static bool DummyImageLoader(tinygltf::Image *image, const int requested_components, std::string *err, std::string *warn, int width, int height, const unsigned char *bytes, int size, void *user_data) {
  if (image) {
    image->width = width;
    image->height = height;
    image->image.clear();
  }
  if (err) err->clear();
  if (warn) warn->clear();
  return false;
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Write input data to a dummy file for file-based loaders
  const std::string filename = "./dummy_file";
  {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) return 0;
    ofs.write(reinterpret_cast<const char*>(Data), Size);
    ofs.close();
  }

  // 1. Fuzz LoadASCIIFromFile
  ctx.LoadASCIIFromFile(&model, &err, &warn, filename, 0);
  ctx.LoadASCIIFromFile(&model, &err, &warn, filename, 1);
  ctx.LoadASCIIFromFile(&model, &err, &warn, filename, 2);
  model = tinygltf::Model();

  // 2. Fuzz LoadBinaryFromFile
  ctx.LoadBinaryFromFile(&model, &err, &warn, filename, 0);
  ctx.LoadBinaryFromFile(&model, &err, &warn, filename, 1);
  ctx.LoadBinaryFromFile(&model, &err, &warn, filename, 2);
  model = tinygltf::Model();

  // 3. Fuzz LoadBinaryFromMemory
  ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 0);
  ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 1);
  ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 2);
  model = tinygltf::Model();

  // 4. Fuzz SetURICallbacks
  tinygltf::URICallbacks uri_callbacks;
  uri_callbacks.encode = DummyURIEncode;
  uri_callbacks.decode = DummyURIDecode;
  uri_callbacks.user_data = nullptr;
  ctx.SetURICallbacks(uri_callbacks, &err);

  // Test with null callbacks to trigger error handling
  tinygltf::URICallbacks uri_callbacks_null;
  uri_callbacks_null.encode = nullptr;
  uri_callbacks_null.decode = nullptr;
  uri_callbacks_null.user_data = nullptr;
  ctx.SetURICallbacks(uri_callbacks_null, &err);

  // 5. Fuzz SetImageLoader
  ctx.SetImageLoader(DummyImageLoader, nullptr);
  ctx.SetImageLoader(nullptr, nullptr);

  // 6. Fuzz SetFsCallbacks
  tinygltf::FsCallbacks fs_callbacks;
  fs_callbacks.FileExists = DummyFileExists;
  fs_callbacks.ExpandFilePath = DummyExpandFilePath;
  fs_callbacks.ReadWholeFile = DummyReadWholeFile;
  fs_callbacks.WriteWholeFile = DummyWriteWholeFile;
  fs_callbacks.GetFileSizeInBytes = DummyGetFileSizeInBytes;
  fs_callbacks.user_data = nullptr;
  ctx.SetFsCallbacks(fs_callbacks, &err);

  // Test with null callbacks
  tinygltf::FsCallbacks fs_callbacks_null;
  fs_callbacks_null.FileExists = nullptr;
  fs_callbacks_null.ExpandFilePath = nullptr;
  fs_callbacks_null.ReadWholeFile = nullptr;
  fs_callbacks_null.WriteWholeFile = nullptr;
  fs_callbacks_null.GetFileSizeInBytes = nullptr;
  fs_callbacks_null.user_data = nullptr;
  ctx.SetFsCallbacks(fs_callbacks_null, &err);

  return 0;
}