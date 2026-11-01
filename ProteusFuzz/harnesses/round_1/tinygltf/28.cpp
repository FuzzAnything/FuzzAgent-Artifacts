// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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
#include <fstream>

// Dummy URI Encode Function matching tinygltf signature
static bool DummyURIEncode(const std::string &uri, const std::string &mime_type, std::string *out, void *user_data) {
  (void)user_data;
  (void)mime_type;
  if (out) {
    *out = uri;
  }
  return true;
}

// Dummy URI Decode Function matching tinygltf signature
static bool DummyURIDecode(const std::string &uri, std::string *out, void *user_data) {
  (void)user_data;
  if (out) {
    *out = uri;
  }
  return true;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Fuzz SetURICallbacks
  tinygltf::URICallbacks callbacks;
  // Randomly choose valid or invalid callbacks to test error handling
  if (Data[0] & 1) {
    callbacks.encode = DummyURIEncode;
  } else {
    callbacks.encode = nullptr;
  }
  if (Data[0] & 2) {
    callbacks.decode = DummyURIDecode;
  } else {
    callbacks.decode = nullptr;
  }
  callbacks.user_data = const_cast<void*>(static_cast<const void*>(Data));
  
  ctx.SetURICallbacks(callbacks, &err);

  // Fuzz SetPreserveImageChannels
  ctx.SetPreserveImageChannels((Data[0] & 4) != 0);

  // Fuzz LoadBinaryFromMemory
  // Use the fuzzing data as binary glTF content
  ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, static_cast<unsigned int>(Size), "./dummy_file", 0);

  // Fuzz LoadBinaryFromFile
  // Write fuzzing data to a dummy file
  {
    std::ofstream dummy_file("./dummy_file.glb", std::ios::binary);
    if (dummy_file.is_open()) {
      dummy_file.write(reinterpret_cast<const char*>(Data), Size);
      dummy_file.close();
      ctx.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file.glb", 0);
    }
  }

  // Fuzz GetImagesAsIs
  bool images_as_is = ctx.GetImagesAsIs();
  (void)images_as_is; // Suppress unused variable warning

  // Fuzz LoadASCIIFromFile
  // Write fuzzing data as ASCII (though it might be binary, testing robustness)
  {
    std::ofstream dummy_ascii("./dummy_file.gltf", std::ios::binary);
    if (dummy_ascii.is_open()) {
      dummy_ascii.write(reinterpret_cast<const char*>(Data), Size);
      dummy_ascii.close();
      ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file.gltf", 0);
    }
  }

  return 0;
}