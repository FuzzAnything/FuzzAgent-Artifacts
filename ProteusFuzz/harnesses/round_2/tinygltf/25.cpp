// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
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

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Fuzz SetStoreOriginalJSONForExtrasAndExtensions and GetStoreOriginalJSONForExtrasAndExtensions
  // Toggle state based on first byte to explore different code paths
  bool enable_original_json = (Data[0] & 1) != 0;
  ctx.SetStoreOriginalJSONForExtrasAndExtensions(enable_original_json);
  bool current_state = ctx.GetStoreOriginalJSONForExtrasAndExtensions();
  (void)current_state; // Suppress unused warning

  // Prepare data for LoadASCIIFromString
  std::string ascii_str(reinterpret_cast<const char*>(Data), Size);

  // Fuzz LoadASCIIFromString (overload 1: length specified)
  ctx.LoadASCIIFromString(&model, &err, &warn, ascii_str.c_str(), Size, "", tinygltf::REQUIRE_VERSION);

  // Fuzz LoadASCIIFromString (overload 2: length derived from string)
  // Re-initialize model and strings to test fresh state
  model = tinygltf::Model();
  err.clear();
  warn.clear();
  // The second overload requires all 6 arguments, the last one has a default but we must pass base_dir
  ctx.LoadASCIIFromString(&model, &err, &warn, ascii_str.c_str(), Size, "", tinygltf::REQUIRE_VERSION);

  // Prepare file for LoadASCIIFromFile and LoadBinaryFromFile
  const std::string filename = "./dummy_file";
  std::ofstream ofs(filename, std::ios::binary);
  if (ofs.is_open()) {
    ofs.write(reinterpret_cast<const char*>(Data), Size);
    ofs.close();

    // Fuzz LoadASCIIFromFile
    model = tinygltf::Model();
    err.clear();
    warn.clear();
    ctx.LoadASCIIFromFile(&model, &err, &warn, filename, tinygltf::REQUIRE_VERSION);

    // Fuzz LoadBinaryFromFile
    model = tinygltf::Model();
    err.clear();
    warn.clear();
    ctx.LoadBinaryFromFile(&model, &err, &warn, filename, tinygltf::REQUIRE_VERSION);
  }

  // Cleanup is handled by RAII (std::string, tinygltf::Model destructor)
  return 0;
}