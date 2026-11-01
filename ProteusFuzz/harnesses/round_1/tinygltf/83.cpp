// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
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
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

// Suppress unused variable warnings for fuzzing
#define FUZZ_UNUSED(x) (void)(x)

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Write input to a dummy file for parsing
  const char *filename = "./dummy_file";
  std::ofstream ofs(filename, std::ios::binary);
  if (!ofs) {
    return 0;
  }
  ofs.write(reinterpret_cast<const char *>(Data), Size);
  ofs.close();

  // Initialize loader
  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err;
  std::string warn;

  // Attempt to load the model from the file
  bool success = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
  
  // If loading failed, try binary mode as well to explore different code paths
  if (!success) {
    success = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
  }

  // Additional fuzzing: Try loading as string directly using the public API
  // LoadFromString is private, so we use LoadASCIIFromFile with a string stream approach or just rely on file loading.
  // However, we can try to parse the string as JSON if it's valid by writing it to a temp file again with a different name
  // or just rely on the file loading which exercises the internal JSON parsing logic.
  
  // To further exercise the internal JSON parsing functions (ObjectBegin, ArrayBegin, etc.),
  // we can try to load the same data as a binary glTF file if it's not ASCII.
  
  // Cleanup: Remove the dummy file
  std::remove(filename);

  return 0;
}