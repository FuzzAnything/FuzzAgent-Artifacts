// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
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

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Helper lambda to safely read data with bounds checking
  auto safe_read = [&](size_t index, uint8_t default_val) -> uint8_t {
    if (index < Size) {
      return Data[index];
    }
    return default_val;
  };

  // Step 1: SetImagesAsIs
  bool flag1 = (safe_read(0, 0) % 2) == 0;
  ctx.SetImagesAsIs(flag1);

  // Write input data to dummy file for ASCII load
  {
    std::ofstream out("./dummy_file", std::ios::binary);
    if (!out) return 0;
    out.write(reinterpret_cast<const char*>(Data), Size);
    out.close();
  }

  // Step 2: LoadASCIIFromFile
  ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");

  // Step 3: WriteGltfSceneToFile (first time)
  bool embedImages1 = (safe_read(0, 0) % 3) == 0;
  bool embedBuffers1 = (safe_read(0, 0) % 3) == 1;
  bool prettyPrint1 = (safe_read(0, 0) % 3) == 2;
  bool writeBinary1 = (safe_read(1, 0) % 2) == 0;
  ctx.WriteGltfSceneToFile(&model, "./output1.gltf", embedImages1, embedBuffers1, prettyPrint1, writeBinary1);

  // Step 4: WriteGltfSceneToFile (second time)
  bool embedImages2 = (safe_read(1, 0) % 3) == 0;
  bool embedBuffers2 = (safe_read(1, 0) % 3) == 1;
  bool prettyPrint2 = (safe_read(1, 0) % 3) == 2;
  bool writeBinary2 = (safe_read(2, 0) % 2) == 0;
  ctx.WriteGltfSceneToFile(&model, "./output2.gltf", embedImages2, embedBuffers2, prettyPrint2, writeBinary2);

  // Step 5: SetImagesAsIs (second time)
  bool flag2 = (safe_read(2, 0) % 2) == 0;
  ctx.SetImagesAsIs(flag2);

  // Step 6: LoadASCIIFromFile (second time)
  ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");

  // Step 7: WriteGltfSceneToFile (third time)
  bool embedImages3 = (safe_read(3, 0) % 3) == 0;
  bool embedBuffers3 = (safe_read(3, 0) % 3) == 1;
  bool prettyPrint3 = (safe_read(3, 0) % 3) == 2;
  bool writeBinary3 = (safe_read(4, 0) % 2) == 0;
  ctx.WriteGltfSceneToFile(&model, "./output3.gltf", embedImages3, embedBuffers3, prettyPrint3, writeBinary3);

  // Step 8: SetImagesAsIs (third time)
  bool flag3 = (safe_read(4, 0) % 2) == 0;
  ctx.SetImagesAsIs(flag3);

  // Step 9: LoadBinaryFromFile
  // We need a binary file. Let's write the input data to a .glb file.
  {
    std::ofstream out("./dummy_file.glb", std::ios::binary);
    if (!out) return 0;
    out.write(reinterpret_cast<const char*>(Data), Size);
    out.close();
  }
  
  ctx.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file.glb");

  return 0;
}