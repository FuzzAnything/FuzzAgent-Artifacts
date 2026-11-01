// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.h:1483:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.h:1483:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
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

// Correct signature for WriteImageDataFunction based on tinygltf.h
static bool DummyImageWriter(const std::string *filename, const std::string *base_path,
                             const tinygltf::Image *image, bool embed_images,
                             const tinygltf::FsCallbacks *fs, const tinygltf::URICallbacks *uri,
                             std::string *out_uri, void *user_data) {
  return true;
}

// Correct signature for LoadImageDataFunction based on the compiler error message:
// function<bool (Image *, const int, basic_string<char> *, basic_string<char> *, int, int, const unsigned char *, int, void *)>
// Note: The second argument is 'const int', not 'int *'.
static bool DummyImageLoader(tinygltf::Image *image, const int width, std::string *height_str,
                             std::string *mime_type, int component, int bits_per_channel,
                             const unsigned char *bytes, int size, void *user_data) {
  return false;
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;

  // Seed random behavior from input data to diversify states
  // Ensure bounds checking before accessing Data array
  bool flag1 = (Size > 0) ? (Data[0] % 2) == 0 : false;
  bool flag2 = (Size > 1) ? (Data[1] % 2) == 0 : false;
  bool flag3 = (Size > 2) ? (Data[2] % 2) == 0 : false;
  bool flag4 = (Size > 3) ? (Data[3] % 2) == 0 : false;

  // 1. Test SetImagesAsIs
  ctx.SetImagesAsIs(flag1);
  ctx.SetImagesAsIs(!flag1); // Toggle again

  // 2. Test SetImageWriter
  void *dummy_user_data = const_cast<void*>(reinterpret_cast<const void*>(&flag2));
  ctx.SetImageWriter(DummyImageWriter, dummy_user_data);
  ctx.SetImageWriter(nullptr, nullptr); // Remove writer

  // 3. Test SetImageLoader (both overloads conceptually covered by calling twice)
  ctx.SetImageLoader(DummyImageLoader, dummy_user_data);
  ctx.SetImageLoader(nullptr, nullptr); // Remove loader

  // 4. Test WriteGltfSceneToStream
  {
    std::ostringstream stream;
    bool success = ctx.WriteGltfSceneToStream(&model, stream, flag3, flag4);
    (void)success; // Suppress unused warning
  }

  {
    std::ostringstream stream;
    bool success = ctx.WriteGltfSceneToStream(&model, stream, !flag3, !flag4);
    (void)success;
  }

  // 5. Test WriteGltfSceneToFile
  // Write to a dummy file as per instructions
  const char *dummy_file = "./dummy_file";
  bool success = ctx.WriteGltfSceneToFile(&model, dummy_file, flag1, flag2, flag3, flag4);
  (void)success;

  success = ctx.WriteGltfSceneToFile(&model, dummy_file, !flag1, !flag2, !flag3, !flag4);
  (void)success;

  // Cleanup: remove dummy file if it exists
  std::remove(dummy_file);

  return 0;
}