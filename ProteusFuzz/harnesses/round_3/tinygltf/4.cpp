// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.h:1483:8 in tiny_gltf.h
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
#include <stdint.h>
#include <stddef.h>
#include <fstream>
#include <functional>
#include <tiny_gltf.h>

// Static helper to avoid name conflicts
// The signatures must match the typedefs in tiny_gltf.h exactly.
// Based on the error message, the expected signature for LoadImageDataFunction is:
// bool (tinygltf::Image *, const int, std::string *, std::string *, int, int, const unsigned char *, int, void *)
// Note: width and height are passed by value, not by pointer.
static bool DummyImageLoader(tinygltf::Image *image,
                             const int index,
                             std::string *ext,
                             std::string *mime_type,
                             int width,
                             int height,
                             const unsigned char *data,
                             int size,
                             void *user_data) {
  (void)image;
  (void)index;
  (void)ext;
  (void)mime_type;
  (void)width;
  (void)height;
  (void)data;
  (void)size;
  (void)user_data;
  return false;
}

// Based on the error message, the expected signature for WriteImageDataFunction is:
// bool (const std::string *, const std::string *, const tinygltf::Image *, bool, const tinygltf::FsCallbacks *, const tinygltf::URICallbacks *, std::string *, void *)
static bool DummyImageWriter(const std::string *filename,
                             const std::string *base_path,
                             const tinygltf::Image *image,
                             bool embed,
                             const tinygltf::FsCallbacks *fs,
                             const tinygltf::URICallbacks *uri_callbacks,
                             std::string *out_uri,
                             void *user_data) {
  (void)filename;
  (void)base_path;
  (void)image;
  (void)embed;
  (void)fs;
  (void)uri_callbacks;
  (void)out_uri;
  (void)user_data;
  return false;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // 1. Prepare the environment
  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Write input data to a dummy file to simulate a glTF file
  const char *dummy_filename = "./dummy_file.gltf";
  {
    std::ofstream ofs(dummy_filename, std::ios::binary);
    if (!ofs) return 0;
    ofs.write(reinterpret_cast<const char*>(Data), Size);
    ofs.close();
  }

  // 2. Invoke target functions in the specified order

  // Step 1: SetImageLoader
  ctx.SetImageLoader(DummyImageLoader, nullptr);

  // Step 2: LoadASCIIFromFile
  ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_filename, tinygltf::REQUIRE_VERSION);

  // Step 3: SetImageWriter
  ctx.SetImageWriter(DummyImageWriter, nullptr);

  // Step 4: WriteGltfSceneToFile
  const char *output_filename = "./dummy_output.gltf";
  
  // Try ASCII mode
  ctx.WriteGltfSceneToFile(
      &model,
      output_filename,
      true,   // embedImages
      true,   // embedBuffers
      true,   // prettyPrint
      false   // writeBinary
  );
  
  // Try Binary mode to explore more states
  ctx.WriteGltfSceneToFile(
      &model,
      output_filename,
      false,
      false,
      false,
      true  // writeBinary
  );

  // 3. Cleanup
  std::remove(dummy_filename);
  std::remove(output_filename);

  return 0;
}