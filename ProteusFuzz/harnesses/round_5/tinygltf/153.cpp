// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
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
#include <fstream>

static std::string g_dummy_file_path = "./dummy_file";

extern "C" int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Write input data to a dummy file for file-based API testing
  std::ofstream ofs(g_dummy_file_path, std::ios::binary);
  if (!ofs) {
    return 0;
  }
  ofs.write(reinterpret_cast<const char*>(Data), Size);
  ofs.close();

  // Test tinygltf::FileExists
  bool file_exists_result = tinygltf::FileExists(g_dummy_file_path, nullptr);
  (void)file_exists_result;

  // Test tinygltf::ReadWholeFile
  std::vector<unsigned char> file_content;
  std::string err_msg;
  bool read_result = tinygltf::ReadWholeFile(&file_content, &err_msg, g_dummy_file_path, nullptr);
  (void)read_result;
  (void)err_msg;

  // Initialize TinyGLTF instance
  tinygltf::TinyGLTF ctx;
  std::string err;
  std::string warn;

  // Test LoadASCIIFromFile (treating input as potential ASCII glTF)
  {
    tinygltf::Model model;
    ctx.LoadASCIIFromFile(&model, &err, &warn, g_dummy_file_path, tinygltf::REQUIRE_VERSION);
  }

  // Test LoadBinaryFromFile (treating input as potential binary glTF)
  {
    tinygltf::Model model;
    ctx.LoadBinaryFromFile(&model, &err, &warn, g_dummy_file_path, tinygltf::REQUIRE_VERSION);
  }

  // Test LoadBinaryFromMemory using the raw input data
  {
    tinygltf::Model model;
    ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", tinygltf::REQUIRE_VERSION);
  }

  // Test LoadBinaryFromMemory with different check_sections flag (REQUIRE_VERSION is default, try 0)
  {
    tinygltf::Model model;
    ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 0);
  }

  // Test LoadBinaryFromMemory with empty base_dir (already tested above, but explicit for clarity)
  {
    tinygltf::Model model;
    ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", tinygltf::REQUIRE_VERSION);
  }

  // Test SetFsCallbacks with default callbacks
  tinygltf::FsCallbacks callbacks;
  callbacks.FileExists = tinygltf::FileExists;
  callbacks.ReadWholeFile = tinygltf::ReadWholeFile;
  callbacks.WriteWholeFile = tinygltf::WriteWholeFile;
  callbacks.ExpandFilePath = tinygltf::ExpandFilePath;
  callbacks.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
  callbacks.user_data = nullptr;

  std::string fs_err;
  bool fs_result = ctx.SetFsCallbacks(callbacks, &fs_err);
  (void)fs_result;
  (void)fs_err;

  // Attempt to load again after setting callbacks to ensure state change is tested
  if (Size > 0) {
    tinygltf::Model model;
    ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", tinygltf::REQUIRE_VERSION);
  }

  return 0;
}