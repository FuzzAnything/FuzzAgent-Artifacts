// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.h:1478:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
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
#include <cstddef>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>

extern "C" int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Fuzz SetMaxExternalFileSize
  // Use a portion of the input to determine the max size limit
  size_t max_bytes = 0;
  if (Size >= sizeof(size_t)) {
    std::memcpy(&max_bytes, Data, sizeof(size_t));
    // Cap to a reasonable limit to avoid allocation failures in the fuzzer itself
    if (max_bytes > 1024 * 1024 * 1024) {
      max_bytes = 1024 * 1024 * 1024; 
    }
  }
  ctx.SetMaxExternalFileSize(max_bytes);

  // Fuzz SetImagesAsIs
  // Use the next byte to determine the boolean flag
  bool images_as_is = (Size > sizeof(size_t) && (Data[sizeof(size_t)] % 2 == 0));
  ctx.SetImagesAsIs(images_as_is);

  // Fuzz RemoveImageLoader
  // Always call to ensure state reset is exercised
  ctx.RemoveImageLoader();

  // Prepare file content for file-based loaders
  // We write the input data to a dummy file to test file loading functions
  std::string dummy_file = "./dummy_file";
  {
    std::ofstream ofs(dummy_file, std::ios::binary);
    if (ofs.is_open()) {
      ofs.write(reinterpret_cast<const char*>(Data), Size);
      ofs.close();
    }
  }

  // Fuzz LoadASCIIFromFile
  // Attempt to load the dummy file as ASCII glTF
  // We reset the model to ensure clean state
  model = tinygltf::Model();
  ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);

  // Fuzz LoadBinaryFromFile
  // Attempt to load the dummy file as Binary glTF
  model = tinygltf::Model();
  ctx.LoadBinaryFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);

  // Fuzz LoadASCIIFromString
  // Load from the input data directly as a string
  // Handle potential null bytes in the middle of the string by using the raw data
  // We pass the whole buffer, but ensure we don't read past Size
  model = tinygltf::Model();
  std::string base_dir = "";
  // Use a subset of data for base_dir if available to vary state
  if (Size > 20) {
    base_dir = std::string(reinterpret_cast<const char*>(Data + 10), 10);
  }
  ctx.LoadASCIIFromString(&model, &err, &warn, 
                          reinterpret_cast<const char*>(Data), 
                          Size, 
                          base_dir, 
                          tinygltf::REQUIRE_VERSION);

  // Fuzz LoadASCIIFromString with REQUIRE_NODES to test different validation states
  model = tinygltf::Model();
  ctx.LoadASCIIFromString(&model, &err, &warn, 
                          reinterpret_cast<const char*>(Data), 
                          Size, 
                          base_dir, 
                          tinygltf::REQUIRE_NODES);

  // Fuzz LoadASCIIFromString with REQUIRE_VERSION (default) again to ensure coverage
  model = tinygltf::Model();
  ctx.LoadASCIIFromString(&model, &err, &warn, 
                          reinterpret_cast<const char*>(Data), 
                          Size, 
                          base_dir, 
                          tinygltf::REQUIRE_VERSION);

  return 0;
}