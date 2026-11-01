// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
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
// The error message indicates the expected signature is:
// bool (tinygltf::Image *, const int, basic_string<char> *, basic_string<char> *, int, int, const unsigned char *, int, void *)
// However, looking at standard tinygltf implementations, the signature usually involves URICallbacks.
// Based on the error "no type named 'URICallback' ... did you mean 'URICallbacks'", we use URICallbacks.
// The error also shows the expected function signature for LoadImageDataFunction is quite specific.
// Let's define a static function matching the expected signature for SetImageLoader.
// Note: The exact signature in tiny_gltf.h for LoadImageDataFunction is typically:
// typedef bool (*LoadImageDataFunction)(tinygltf::Image* image, int index, const std::string& uri, const std::string& mime_type, tinygltf::URICallbacks* uri_cb, void* user_data);
// But the compiler error suggests a different signature is expected in this specific build or version.
// Let's try to match the signature hinted by the error message:
// bool (Image *, const int, basic_string<char> *, basic_string<char> *, int, int, const unsigned char *, int, void *)
// This looks like a custom or modified version. Let's assume the standard signature first but with URICallbacks.
// If the error persists, we might need to adapt to the specific signature shown in the error.
// Re-reading the error: "no viable conversion from 'bool (tinygltf::Image *, int, const std::string &, const std::string &, tinygltf::URICallbacks *, void *)' ... to 'LoadImageDataFunction' (aka 'function<bool (tinygltf::Image *, const int, basic_string<char> *, basic_string<char> *, int, int, const unsigned char *, int, void *)>')'"
// This implies the LoadImageDataFunction typedef in this specific tiny_gltf.h is:
// typedef std::function<bool(tinygltf::Image*, const int, std::string*, std::string*, int, int, const unsigned char*, int, void*)> LoadImageDataFunction;
// This is unusual. Let's try to define a function that matches this specific signature.

static bool CustomLoadImageData(tinygltf::Image *image, const int index,
                                std::string *uri, std::string *mime_type,
                                int width, int height, const unsigned char *data,
                                int data_length, void *user_data) {
  (void)index;
  (void)uri;
  (void)mime_type;
  (void)width;
  (void)height;
  (void)data;
  (void)data_length;
  (void)user_data;

  if (image) {
    // Simulate loading some dummy data
    image->width = 1;
    image->height = 1;
    image->image.resize(4);
    image->image[0] = 255;
    image->image[1] = 0;
    image->image[2] = 0;
    image->image[3] = 255;
    image->as_is = true;
  }
  return true;
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
  if (Size < 7) {
    return 0;
  }

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;

  // Use input data to drive state variations
  bool flag1 = (Data[0] % 2) != 0;
  bool flag2 = (Data[1] % 2) != 0;
  bool flag3 = (Data[2] % 2) != 0;
  bool flag4 = (Data[3] % 2) != 0;
  bool flag5 = (Data[4] % 2) != 0;
  bool flag6 = (Data[5] % 2) != 0;

  // 1. SetImagesAsIs
  ctx.SetImagesAsIs(flag1);

  // 2. GetPreserveImageChannels
  bool preserve_before = ctx.GetPreserveImageChannels();
  (void)preserve_before; // Suppress unused warning

  // 3. SetPreserveImageChannels
  ctx.SetPreserveImageChannels(flag2);

  // 4. GetImagesAsIs
  bool images_as_is = ctx.GetImagesAsIs();
  (void)images_as_is; // Suppress unused warning

  // 5. SetImageLoader
  if (flag3) {
    ctx.SetImageLoader(CustomLoadImageData, static_cast<void*>(const_cast<uint8_t*>(Data)));
  } else {
    ctx.SetImageLoader(nullptr, nullptr);
  }

  // 6. WriteGltfSceneToFile
  // Prepare a dummy file path
  const char *filename = "./dummy_file.gltf";
  
  // Write the model to file with various flags derived from input
  bool write_success = ctx.WriteGltfSceneToFile(
    &model,
    filename,
    flag4, // embedImages
    flag5, // embedBuffers
    flag6, // prettyPrint
    (Data[6] % 2) != 0 // writeBinary
  );
  
  (void)write_success; // Suppress unused warning

  return 0;
}