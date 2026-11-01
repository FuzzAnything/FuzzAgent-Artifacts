// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
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

// Helper functions to mimic the internal static functions of tinygltf
// since they are not exposed in the public namespace in some versions.
static std::string FuzzGetBaseFilename(const std::string &filepath) {
  std::string path = filepath;
  size_t pos = path.find_last_of("/\\");
  if (pos != std::string::npos) {
    return path.substr(pos + 1);
  }
  return path;
}

static std::string FuzzJoinPath(const std::string &path0, const std::string &path1) {
  if (path0.empty()) return path1;
  std::string result = path0;
  if (result.back() != '/' && result.back() != '\\') {
    result += '/';
  }
  return result + path1;
}

static std::string FuzzGetFilePathExtension(const std::string &FileName) {
  size_t pos = FileName.find_last_of('.');
  if (pos == std::string::npos || pos == 0) {
    return "";
  }
  return FileName.substr(pos + 1);
}

static std::string FuzzGetBaseDir(const std::string &filepath) {
  size_t pos = filepath.find_last_of("/\\");
  if (pos == std::string::npos) {
    return "";
  }
  return filepath.substr(0, pos + 1);
}

extern "C" int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Create a dummy file to test file-based operations
  const char* dummy_file = "./dummy_file";
  std::ofstream ofs(dummy_file, std::ios::binary);
  if (ofs.is_open()) {
    ofs.write(reinterpret_cast<const char*>(Data), Size);
    ofs.close();
  }

  std::string input(reinterpret_cast<const char*>(Data), Size);
  std::string filepath = input;
  std::string path0 = input;
  std::string path1 = input;
  std::string err;
  std::vector<unsigned char> out;
  std::string expanded_path;
  std::string base_filename;
  std::string base_dir;
  std::string extension;
  std::string joined_path;

  // 1. Test GetBaseFilename (using local implementation)
  try {
    base_filename = FuzzGetBaseFilename(filepath);
  } catch (...) {
    // Ignore exceptions
  }

  // 2. Test ExpandFilePath (public API)
  try {
    expanded_path = tinygltf::ExpandFilePath(filepath, nullptr);
  } catch (...) {
    // Ignore exceptions
  }

  // 3. Test ReadWholeFile (public API)
  try {
    out.clear();
    err.clear();
    tinygltf::ReadWholeFile(&out, &err, dummy_file, nullptr);
  } catch (...) {
    // Ignore exceptions
  }

  // 4. Test JoinPath (using local implementation)
  try {
    joined_path = FuzzJoinPath(path0, path1);
  } catch (...) {
    // Ignore exceptions
  }

  // 5. Test GetFilePathExtension (using local implementation)
  try {
    extension = FuzzGetFilePathExtension(filepath);
  } catch (...) {
    // Ignore exceptions
  }

  // 6. Test GetBaseDir (using local implementation)
  try {
    base_dir = FuzzGetBaseDir(filepath);
  } catch (...) {
    // Ignore exceptions
  }

  // Cleanup: remove dummy file
  std::remove(dummy_file);

  return 0;
}