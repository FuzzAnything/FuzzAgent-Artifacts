// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
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
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Fuzz LoadBinaryFromMemory
  ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, static_cast<unsigned int>(Size), "", 0);
  model = tinygltf::Model(); // Reset model

  // Fuzz LoadASCIIFromString
  // Ensure null termination for string interpretation if needed, though function takes length
  ctx.LoadASCIIFromString(&model, &err, &warn, reinterpret_cast<const char*>(Data), static_cast<unsigned int>(Size), "", 0);
  model = tinygltf::Model();

  // Fuzz SetFsCallbacks
  tinygltf::FsCallbacks fs_callbacks;
  // Initialize with nulls to test error handling or valid minimal callbacks
  fs_callbacks.FileExists = nullptr;
  fs_callbacks.ExpandFilePath = nullptr;
  fs_callbacks.ReadWholeFile = nullptr;
  fs_callbacks.WriteWholeFile = nullptr;
  fs_callbacks.GetFileSizeInBytes = nullptr;
  fs_callbacks.user_data = nullptr;
  ctx.SetFsCallbacks(fs_callbacks, &err);

  // Fuzz SetURICallbacks
  tinygltf::URICallbacks uri_callbacks;
  uri_callbacks.encode = nullptr;
  uri_callbacks.decode = nullptr;
  uri_callbacks.user_data = nullptr;
  ctx.SetURICallbacks(uri_callbacks, &err);

  // Fuzz LoadBinaryFromFile and LoadASCIIFromFile
  // Write input data to a dummy file to test file-based loading
  const std::string filename = "./dummy_file";
  std::ofstream ofs(filename, std::ios::binary);
  if (ofs.is_open()) {
    ofs.write(reinterpret_cast<const char*>(Data), Size);
    ofs.close();

    // Test LoadBinaryFromFile
    ctx.LoadBinaryFromFile(&model, &err, &warn, filename, 0);
    model = tinygltf::Model();

    // Test LoadASCIIFromFile
    ctx.LoadASCIIFromFile(&model, &err, &warn, filename, 0);
    model = tinygltf::Model();
  }

  return 0;
}