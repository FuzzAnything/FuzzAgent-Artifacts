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

// The functions GetFilePathExtension, GetBaseDir, GetBaseFilename, and JoinPath
// are often defined as static inside the tinygltf namespace in the header file.
// However, if they are not visible in the current translation unit (e.g., due to
// conditional compilation or specific header versions), we can implement them locally
// based on their documented behavior to ensure the fuzzer compiles and runs.
// Alternatively, if the header exposes them differently, we would use them.
// Given the compilation error, we assume they are not accessible as public static members
// in this specific build configuration, so we provide local implementations that mimic
// the expected behavior for fuzzing purposes.

static std::string LocalGetFilePathExtension(const std::string &FileName) {
  size_t pos = FileName.rfind('.');
  if (pos == std::string::npos || pos == 0) return "";
  return FileName.substr(pos + 1);
}

static std::string LocalGetBaseDir(const std::string &filepath) {
  size_t pos = filepath.find_last_of("/\\");
  if (pos == std::string::npos) return "";
  return filepath.substr(0, pos + 1);
}

static std::string LocalGetBaseFilename(const std::string &filepath) {
  size_t pos = filepath.find_last_of("/\\");
  if (pos == std::string::npos) return filepath;
  return filepath.substr(pos + 1);
}

static std::string LocalJoinPath(const std::string &path0, const std::string &path1) {
  if (path0.empty()) return path1;
  if (path1.empty()) return path0;
  
  char lastChar = path0.back();
  if (lastChar == '/' || lastChar == '\\') {
    return path0 + path1;
  }
  return path0 + "/" + path1;
}

static void FuzzExpandFilePath(const uint8_t *Data, size_t Size) {
  if (Size == 0) return;
  std::string filepath(reinterpret_cast<const char*>(Data), Size);
  std::string expanded = tinygltf::ExpandFilePath(filepath, nullptr);
  (void)expanded;
}

static void FuzzGetFilePathExtension(const uint8_t *Data, size_t Size) {
  if (Size == 0) return;
  std::string filename(reinterpret_cast<const char*>(Data), Size);
  std::string ext = LocalGetFilePathExtension(filename);
  (void)ext;
}

static void FuzzGetBaseDir(const uint8_t *Data, size_t Size) {
  if (Size == 0) return;
  std::string filepath(reinterpret_cast<const char*>(Data), Size);
  std::string baseDir = LocalGetBaseDir(filepath);
  (void)baseDir;
}

static void FuzzGetBaseFilename(const uint8_t *Data, size_t Size) {
  if (Size == 0) return;
  std::string filepath(reinterpret_cast<const char*>(Data), Size);
  std::string baseFilename = LocalGetBaseFilename(filepath);
  (void)baseFilename;
}

static void FuzzJoinPath(const uint8_t *Data, size_t Size) {
  if (Size == 0) return;
  size_t split = Size / 2;
  std::string path0(reinterpret_cast<const char*>(Data), split);
  std::string path1(reinterpret_cast<const char*>(Data + split), Size - split);
  std::string joined = LocalJoinPath(path0, path1);
  (void)joined;
}

static void FuzzReadWholeFile(const uint8_t *Data, size_t Size) {
  (void)Data;
  (void)Size;
  std::string filepath("./dummy_file");
  std::vector<unsigned char> out;
  std::string err;
  tinygltf::ReadWholeFile(&out, &err, filepath, nullptr);
  (void)out;
  (void)err;
}

extern "C" int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  FuzzExpandFilePath(Data, Size);
  FuzzGetFilePathExtension(Data, Size);
  FuzzGetBaseDir(Data, Size);
  FuzzGetBaseFilename(Data, Size);
  FuzzJoinPath(Data, Size);
  FuzzReadWholeFile(Data, Size);

  return 0;
}