// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
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
#include <cstdio>
#include <string>
#include <vector>

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
  FILE *f = fopen(filename.c_str(), "wb");
  if (f) {
    fwrite(data, 1, size, f);
    fclose(f);
  }
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF loader;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Step 1: LoadBinaryFromFile
  WriteDummyFile("./dummy_file.glb", Data, Size);
  loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file.glb", 0);

  // Step 2: LoadASCIIFromFile
  WriteDummyFile("./dummy_file.gltf", Data, Size);
  loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file.gltf", 0);

  // Step 3 & 4: Invoke Accessor::ByteStride diversely
  tinygltf::Accessor accessor;
  tinygltf::BufferView bufferView;

  // Test with default constructed objects
  accessor.ByteStride(bufferView);

  // Test with modified bufferView properties
  bufferView.byteOffset = 0;
  accessor.ByteStride(bufferView);

  bufferView.byteOffset = 1;
  accessor.ByteStride(bufferView);

  bufferView.buffer = 0;
  accessor.ByteStride(bufferView);

  bufferView.buffer = -1;
  accessor.ByteStride(bufferView);

  // Test with accessor modifications (though ByteStride primarily uses bufferView)
  accessor.byteOffset = 0;
  accessor.ByteStride(bufferView);

  accessor.byteOffset = 100;
  accessor.ByteStride(bufferView);

  return 0;
}