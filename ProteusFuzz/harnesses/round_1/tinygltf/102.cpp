// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include "tiny_gltf.h"

// Helper implementations for functions that might be static or not exposed in the namespace
// based on the logic described in the prompt.

static std::string FuzzGetBaseDir(const std::string &filepath) {
  size_t pos = filepath.find_last_of("/\\");
  if (pos == std::string::npos) {
    return "";
  }
  return filepath.substr(0, pos + 1);
}

static std::string FuzzGetBaseFilename(const std::string &filepath) {
  size_t pos = filepath.find_last_of("/\\");
  if (pos == std::string::npos) {
    return filepath;
  }
  return filepath.substr(pos + 1);
}

static std::string FuzzGetFilePathExtension(const std::string &FileName) {
  size_t pos = FileName.find_last_of('.');
  if (pos == std::string::npos) {
    return "";
  }
  // Check if it's a hidden file (starts with .) and has no other dots
  if (pos == 0) {
    return "";
  }
  return FileName.substr(pos + 1);
}

static std::string FuzzJoinPath(const std::string &path0, const std::string &path1) {
  if (path0.empty()) {
    return path1;
  }
  if (path1.empty()) {
    return path0;
  }
  if (path0.back() == '/' || path0.back() == '\\') {
    return path0 + path1;
  }
  return path0 + "/" + path1;
}

// Wrapper for ExpandFilePath
static std::string FuzzExpandFilePath(const std::string &filepath, void *userdata) {
  return tinygltf::ExpandFilePath(filepath, userdata);
}

// Wrapper for WriteGltfFile - implementing logic if not available in namespace
// The error indicates tinygltf::WriteGltfFile is not found. 
// We will implement the logic based on the description: saves GLTF JSON content to a file.
static bool FuzzWriteGltfFile(const std::string &output, const std::string &content) {
#ifdef TINYGLTF_NO_FS
  return false;
#endif
  FILE *fp = fopen(output.c_str(), "wb");
  if (!fp) {
    return false;
  }
  size_t written = fwrite(content.data(), 1, content.size(), fp);
  fclose(fp);
  return (written == content.size());
}

static std::string SanitizeInput(const uint8_t *Data, size_t Size) {
  if (Size == 0) return "";
  std::string result(reinterpret_cast<const char*>(Data), Size);
  // Remove null bytes to prevent premature truncation in string operations
  result.erase(std::remove(result.begin(), result.end(), '\0'), result.end());
  return result;
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  std::string input = SanitizeInput(Data, Size);

  // Test GetBaseDir (using local implementation)
  std::string baseDir = FuzzGetBaseDir(input);

  // Test GetBaseFilename (using local implementation)
  std::string baseFilename = FuzzGetBaseFilename(input);

  // Test GetFilePathExtension (using local implementation)
  std::string extension = FuzzGetFilePathExtension(input);

  // Test ExpandFilePath
  std::string expandedPath = FuzzExpandFilePath(input, nullptr);

  // Test JoinPath (using local implementation)
  std::string path1 = FuzzJoinPath(input, input);
  std::string path2 = FuzzJoinPath("", input);
  std::string path3 = FuzzJoinPath(input, "");
  std::string path4 = FuzzJoinPath(baseDir, baseFilename);

  // Test WriteGltfFile
  std::string dummyFile = "./dummy_file";
  std::string content = input;
  (void)FuzzWriteGltfFile(dummyFile, content);

  // Additional state exploration: try with modified inputs
  std::string pathWithSlash = input + "/subdir";
  std::string baseDir2 = FuzzGetBaseDir(pathWithSlash);
  std::string baseFilename2 = FuzzGetBaseFilename(pathWithSlash);
  std::string extension2 = FuzzGetFilePathExtension(pathWithSlash);
  std::string expandedPath2 = FuzzExpandFilePath(pathWithSlash, nullptr);
  std::string path5 = FuzzJoinPath(baseDir2, baseFilename2);

  // Try writing with the modified content
  (void)FuzzWriteGltfFile(dummyFile, path5);

  return 0;
}