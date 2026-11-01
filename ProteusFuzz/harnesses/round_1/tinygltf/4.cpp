// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
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
#include "tiny_gltf.h"

// Custom image loader callback matching the expected signature
// Signature: bool (tinygltf::Image *, const int, std::string *, std::string *, int, int, const unsigned char *, int, void *)
static bool CustomImageLoader(
    tinygltf::Image *image,
    const int index,
    std::string *ext,
    std::string *mime_type,
    int width,
    int height,
    const unsigned char *bytes,
    int size,
    void *user_data) {
  // Fallback to default behavior or return failure to trigger error paths
  // We can't easily populate the image struct without knowing the format,
  // so we return false to simulate a loading failure or unsupported format.
  return false;
}

// Custom image writer callback matching the expected signature
// Signature: bool (const std::string *, const std::string *, const tinygltf::Image *, bool, const tinygltf::FsCallbacks *, const tinygltf::URICallbacks *, std::string *, void *)
static bool CustomImageWriter(
    const std::string *image_filename,
    const std::string *base_dir,
    const tinygltf::Image *image,
    bool embed,
    const tinygltf::FsCallbacks *fs_callbacks,
    const tinygltf::URICallbacks *uri_callbacks,
    std::string *out_uri,
    void *user_data) {
  // Attempt to write to a dummy file to test I/O logic
  if (image_filename && !image_filename->empty()) {
    FILE *fp = fopen(image_filename->c_str(), "wb");
    if (fp) {
      fwrite(image->image.data(), 1, image->image.size(), fp);
      fclose(fp);
      return true;
    }
  }
  return false;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // 1. SetImageLoader
  // Register the custom loader.
  ctx.SetImageLoader(CustomImageLoader, const_cast<void*>(reinterpret_cast<const void*>(Data)));

  // 2. LoadASCIIFromFile
  // Write the fuzzed input to a temporary file to act as the glTF source.
  const char *dummy_filename = "./dummy_file";
  FILE *fp = fopen(dummy_filename, "wb");
  if (fp) {
    fwrite(Data, 1, Size, fp);
    fclose(fp);
  } else {
    return 0; // Cannot create test file
  }

  // Attempt to load the file. This may fail, which is expected for fuzzing.
  bool load_success = ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_filename);

  // 3. SetImageWriter
  // Register the custom writer for the export phase.
  ctx.SetImageWriter(CustomImageWriter, const_cast<void*>(reinterpret_cast<const void*>(Data + (Size > 1 ? 1 : 0))));

  // 4. WriteGltfSceneToFile
  // Attempt to write the model (which might be empty or partially loaded) back to a file.
  // We vary the flags based on the first few bytes of input to explore different code paths.
  bool embed_images = (Size > 0 && Data[0] % 2 == 0);
  bool embed_buffers = (Size > 1 && Data[1] % 2 == 0);
  bool pretty_print = (Size > 2 && Data[2] % 2 == 0);
  bool write_binary = (Size > 3 && Data[3] % 2 == 0);

  const char *output_filename = "./dummy_output_file";
  bool write_success = ctx.WriteGltfSceneToFile(
      &model, output_filename, embed_images, embed_buffers, pretty_print, write_binary);

  // Cleanup: Remove dummy files if they exist
  std::remove(dummy_filename);
  std::remove(output_filename);

  // Return 0 to indicate successful execution of the fuzzer harness
  return 0;
}