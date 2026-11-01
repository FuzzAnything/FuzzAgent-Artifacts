// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>

extern "C" int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  tinygltf::TinyGLTF ctx;
  tinygltf::Model model;
  std::string err;
  std::string warn;

  // Safely access input data with bounds checking
  bool preserve_channels = (Size > 0) ? ((Data[0] % 2) != 0) : false;
  bool images_as_is = (Size > 1) ? ((Data[1] % 2) != 0) : false;

  ctx.SetPreserveImageChannels(preserve_channels);
  ctx.SetImagesAsIs(images_as_is);

  // Verify GetImagesAsIs returns the set value
  bool current_images_as_is = ctx.GetImagesAsIs();
  (void)current_images_as_is; // Use to avoid unused variable warning

  // Write input data to a dummy file for file-based loading tests
  const char *dummy_file = "./dummy_file";
  std::ofstream ofs(dummy_file, std::ios::binary);
  if (ofs.is_open()) {
    ofs.write(reinterpret_cast<const char*>(Data), Size);
    ofs.close();
  }

  // Fuzz LoadBinaryFromFile
  // Try loading as binary glTF from the dummy file
  ctx.LoadBinaryFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);
  model = tinygltf::Model(); // Reset model
  err.clear();
  warn.clear();

  // Fuzz LoadASCIIFromFile
  // Try loading as ASCII glTF from the dummy file
  ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);
  model = tinygltf::Model(); // Reset model
  err.clear();
  warn.clear();

  // Fuzz LoadBinaryFromMemory
  // Try loading from memory buffer
  ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, static_cast<unsigned int>(Size), "", tinygltf::REQUIRE_VERSION);
  model = tinygltf::Model(); // Reset model
  err.clear();
  warn.clear();

  // Try with different check_sections flags
  // REQUIRE_VERSION is default, try 0 (no check) if Size is large enough to avoid trivial rejection
  if (Size > 10) {
    ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, static_cast<unsigned int>(Size), "", 0);
  }

  // Toggle settings and try again to explore different states
  ctx.SetImagesAsIs(!images_as_is);
  ctx.SetPreserveImageChannels(!preserve_channels);

  if (Size > 10) {
    ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, static_cast<unsigned int>(Size), "", tinygltf::REQUIRE_VERSION);
  }

  // Cleanup: Model destructor handles internal resources automatically
  return 0;
}