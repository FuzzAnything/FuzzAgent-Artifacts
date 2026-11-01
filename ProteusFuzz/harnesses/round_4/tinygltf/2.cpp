// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.h:1483:8 in tiny_gltf.h
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

// Correct signature for LoadImageDataFunction based on tinygltf.h
// The 5th and 6th arguments are 'int' (pass by value), not 'int*'
static bool FuzzImageLoader(tinygltf::Image *image,
                            int image_index,
                            std::string *ext,
                            std::string *mime_type,
                            int width,
                            int height,
                            const unsigned char *bytes,
                            int size,
                            void *user_data) {
  // Dummy implementation: always return false to simulate failure or no-op
  // This ensures the fuzzer explores error paths in the loader logic
  return false;
}

// Correct signature for WriteImageDataFunction based on tinygltf.h
static bool FuzzImageWriter(const std::string *image_uri,
                            const std::string *mime_type,
                            const tinygltf::Image *image,
                            bool embed,
                            const tinygltf::FsCallbacks *fs_callbacks,
                            const tinygltf::URICallbacks *uri_callbacks,
                            std::string *out_uri,
                            void *user_data) {
  // Dummy implementation: always return false to simulate failure
  return false;
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Create a dummy file with the fuzzed data
  const char *dummy_file = "./dummy_file";
  FILE *fp = fopen(dummy_file, "wb");
  if (!fp) {
    return 0;
  }
  fwrite(Data, 1, Size, fp);
  fclose(fp);

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // 1. SetImageLoader
  ctx.SetImageLoader(FuzzImageLoader, nullptr);

  // 2. LoadASCIIFromFile
  // We ignore the return value to allow the fuzzer to explore both success and failure paths
  // The function populates 'err' and 'warn' which is useful for state exploration
  ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file);

  // 3. SetImageWriter
  ctx.SetImageWriter(FuzzImageWriter, nullptr);

  // 4. WriteGltfSceneToFile
  // We try to write the model back to a file.
  // We use a fixed output filename to avoid issues with fuzzing filenames.
  // We pass false for embedImages/embedBuffers to test the path where external files are expected.
  // We pass true for prettyPrint and writeBinary to explore different serialization paths.
  // Note: WriteGltfSceneToFile might fail if the model is invalid or if the image writer fails,
  // which is exactly what we want to test.
  ctx.WriteGltfSceneToFile(&model, "./output.gltf", false, false, true, false);
  ctx.WriteGltfSceneToFile(&model, "./output.glb", false, false, false, true);

  // Cleanup
  remove(dummy_file);
  remove("./output.gltf");
  remove("./output.glb");

  return 0;
}