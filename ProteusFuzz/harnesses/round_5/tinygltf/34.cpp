// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <functional>
#include "tiny_gltf.h"

// Helper to create a dummy model for testing
static tinygltf::Model CreateDummyModel() {
  tinygltf::Model model;
  model.asset.version = "2.0";
  return model;
}

// Helper to create a dummy image
static tinygltf::Image CreateDummyImage(const uint8_t *Data, size_t Size) {
  tinygltf::Image img;
  img.name = "dummy_image";
  img.width = 1;
  img.height = 1;
  if (Size > 0) {
    img.image.assign(Data, Data + Size);
  } else {
    img.image.push_back(0);
  }
  return img;
}

// Custom callback implementations for FsCallbacks
static bool FileExists(const std::string &abs_path, void *user_data) {
  (void)abs_path;
  (void)user_data;
  return false;
}

static std::string ExpandFilePath(const std::string &filepath, void *user_data) {
  (void)filepath;
  (void)user_data;
  return filepath;
}

static bool ReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                          const std::string &filepath, void *user_data) {
  (void)out;
  (void)err;
  (void)filepath;
  (void)user_data;
  return false;
}

static bool WriteWholeFile(std::string *err, const std::string &filepath,
                           const std::vector<unsigned char> &contents,
                           void *user_data) {
  (void)err;
  (void)filepath;
  (void)contents;
  (void)user_data;
  return false;
}

static bool GetFileSizeInBytes(size_t *size, std::string *err,
                               const std::string &filepath, void *user_data) {
  (void)size;
  (void)err;
  (void)filepath;
  (void)user_data;
  if (size) *size = 0;
  return false;
}

// Custom callback implementations for URICallbacks
static bool URICallbackEncode(const std::string &base, const std::string &uri, std::string *out, void *user_data) {
  (void)base;
  (void)user_data;
  if (out) *out = uri;
  return true;
}

static bool URICallbackDecode(const std::string &uri, std::string *out, void *user_data) {
  (void)user_data;
  if (out) *out = uri;
  return true;
}

// WriteImageData wrapper that matches the target signature
static bool WriteImageDataWrapper(const std::string *basepath,
                                  const std::string *filename,
                                  const tinygltf::Image *image,
                                  bool embedImages,
                                  const tinygltf::FsCallbacks *fs_cb,
                                  const tinygltf::URICallbacks *uri_cb,
                                  std::string *out_uri, void *user_data) {
  return tinygltf::WriteImageData(basepath, filename, image, embedImages,
                                  fs_cb, uri_cb, out_uri, user_data);
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  std::string err;
  std::string warn;

  // 1. Prepare environment for SetFsCallbacks and SetURICallbacks
  tinygltf::FsCallbacks fs_cb;
  fs_cb.FileExists = FileExists;
  fs_cb.ExpandFilePath = ExpandFilePath;
  fs_cb.ReadWholeFile = ReadWholeFile;
  fs_cb.WriteWholeFile = WriteWholeFile;
  fs_cb.GetFileSizeInBytes = GetFileSizeInBytes;
  fs_cb.user_data = nullptr;

  tinygltf::URICallbacks uri_cb;
  uri_cb.encode = URICallbackEncode;
  uri_cb.decode = URICallbackDecode;
  uri_cb.user_data = nullptr;

  // 2. Fuzz SetFsCallbacks
  ctx.SetFsCallbacks(fs_cb, &err);

  // 3. Fuzz SetURICallbacks
  ctx.SetURICallbacks(uri_cb, &err);

  // 4. Fuzz WriteGltfSceneToFile
  tinygltf::Model model = CreateDummyModel();
  std::string filename = "./dummy_file";
  ctx.WriteGltfSceneToFile(&model, filename, false, false, true, false);
  ctx.WriteGltfSceneToFile(&model, filename, true, true, false, true);

  // 5. Fuzz WriteGltfSceneToStream
  std::stringstream stream;
  ctx.WriteGltfSceneToStream(&model, stream, true, false);
  ctx.WriteGltfSceneToStream(&model, stream, false, true);

  // 6. Fuzz WriteImageData
  tinygltf::Image img = CreateDummyImage(Data, Size);
  std::string out_uri;
  std::string filename_str = "dummy.png";
  std::string basepath_str = "./";

  WriteImageDataWrapper(&basepath_str, &filename_str, &img, false, &fs_cb, &uri_cb, &out_uri, nullptr);
  WriteImageDataWrapper(&basepath_str, &filename_str, &img, true, &fs_cb, &uri_cb, &out_uri, nullptr);

  // 7. Additional fuzzing: Modify model components with input data to trigger different code paths
  tinygltf::Buffer buffer;
  buffer.name = "buffer";
  buffer.data.assign(Data, Data + Size);
  model.buffers.push_back(buffer);

  tinygltf::BufferView bufferView;
  bufferView.buffer = 0;
  model.bufferViews.push_back(bufferView);

  tinygltf::Accessor accessor;
  accessor.bufferView = 0;
  model.accessors.push_back(accessor);

  // Retry write operations with modified model
  ctx.WriteGltfSceneToFile(&model, filename, false, false, true, false);
  ctx.WriteGltfSceneToStream(&model, stream, true, false);

  return 0;
}