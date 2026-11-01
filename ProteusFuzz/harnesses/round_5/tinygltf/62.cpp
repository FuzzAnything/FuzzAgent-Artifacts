// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.h:1478:8 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.h:1478:8 in tiny_gltf.h
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
#include "tiny_gltf.h"

// Static helper functions for URI callbacks to match expected signatures
static bool DummyURIEncode(const std::string &uri, const std::string &base_dir, std::string *err, void *user_data) {
  (void)base_dir;
  (void)err;
  (void)user_data;
  return true;
}

static bool DummyURIDecode(const std::string &uri, std::string *err, void *user_data) {
  (void)err;
  (void)user_data;
  return true;
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  tinygltf::TinyGLTF parser;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Create a dummy file for file-based loading tests
  const std::string filename = "./dummy_file";
  {
    std::ofstream ofs(filename, std::ios::binary);
    if (ofs.is_open()) {
      ofs.write(reinterpret_cast<const char*>(Data), Size);
      ofs.close();
    }
  }

  // Test 1: LoadASCIIFromFile
  parser.LoadASCIIFromFile(&model, &err, &warn, filename,
                           tinygltf::REQUIRE_VERSION);
  parser.LoadASCIIFromFile(&model, &err, &warn, filename,
                           tinygltf::REQUIRE_VERSION);

  // Test 2: LoadBinaryFromFile
  parser.LoadBinaryFromFile(&model, &err, &warn, filename,
                            tinygltf::REQUIRE_VERSION);
  parser.LoadBinaryFromFile(&model, &err, &warn, filename,
                            tinygltf::REQUIRE_VERSION);

  // Test 3: LoadASCIIFromString (Overload 1: length specified)
  parser.LoadASCIIFromString(&model, &err, &warn,
                             reinterpret_cast<const char*>(Data),
                             Size,
                             "./",
                             tinygltf::REQUIRE_VERSION);

  // Test 4: LoadASCIIFromString (Overload 2: length calculated via strlen)
  // We must ensure the string is null-terminated before calling strlen.
  // Since the input data may contain null bytes or not be null-terminated,
  // we create a temporary std::string to safely handle the length calculation.
  std::string str_data(reinterpret_cast<const char*>(Data), Size);
  parser.LoadASCIIFromString(&model, &err, &warn, str_data.c_str(),
                             str_data.length(),
                             "./",
                             tinygltf::REQUIRE_VERSION);

  // Test 5: SetURICallbacks
  // Configure with valid callbacks
  tinygltf::URICallbacks callbacks;
  callbacks.encode = DummyURIEncode;
  callbacks.decode = DummyURIDecode;
  callbacks.user_data = nullptr;
  parser.SetURICallbacks(callbacks, &err);

  // Configure with invalid callbacks (null functions) to test error handling
  tinygltf::URICallbacks invalid_callbacks;
  invalid_callbacks.encode = nullptr;
  invalid_callbacks.decode = nullptr;
  invalid_callbacks.user_data = nullptr;
  parser.SetURICallbacks(invalid_callbacks, &err);

  // Test 6: RemoveImageLoader
  parser.RemoveImageLoader();
  parser.RemoveImageLoader();

  // Cleanup: Remove the dummy file
  std::remove(filename.c_str());

  return 0;
}