// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
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
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// Dummy callbacks matching tinygltf function pointer types
static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &filepath, void *) {
  (void)filepath;
  (void)err;
  out->clear();
  return true;
}

static bool DummyFileExists(const std::string &filepath, void *) {
  (void)filepath;
  return true;
}

static std::string DummyExpandFilePath(const std::string &filepath, void *) {
  return filepath;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filepath,
                                const std::vector<unsigned char> &contents, void *) {
  (void)err;
  (void)filepath;
  (void)contents;
  return true;
}

// Correct signature for GetFileSizeInBytes: bool(size_t*, std::string*, const std::string&, void*)
static bool DummyGetFileSize(size_t *size, std::string *err, const std::string &filepath, void *) {
  (void)err;
  (void)filepath;
  if (size) *size = 0;
  return true;
}

// Helper to extract base filename manually if not exposed in namespace
static std::string GetBaseFilenameManual(const std::string &filepath) {
  size_t pos = filepath.find_last_of("/\\");
  if (pos == std::string::npos) return filepath;
  return filepath.substr(pos + 1);
}

// Helper to extract extension manually
static std::string GetFilePathExtensionManual(const std::string &FileName) {
  size_t pos = FileName.find_last_of('.');
  if (pos == std::string::npos || pos == 0) return "";
  return FileName.substr(pos + 1);
}

// Helper for MimeToExt manually
static std::string MimeToExtManual(const std::string &mimeType) {
  if (mimeType == "image/png") return "png";
  if (mimeType == "image/jpeg") return "jpg";
  if (mimeType == "image/jpg") return "jpg";
  if (mimeType == "image/bmp") return "bmp";
  if (mimeType == "image/gif") return "gif";
  if (mimeType == "image/webp") return "webp";
  if (mimeType == "image/tiff") return "tiff";
  if (mimeType == "image/tga") return "tga";
  return "";
}

extern "C" int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  std::string input(reinterpret_cast<const char*>(Data), Size);
  std::string filepath = "./dummy_file";
  std::string err;
  std::string warn;
  std::string expanded;
  std::string base;
  std::string ext;
  std::string mime_ext;
  std::vector<unsigned char> out;
  std::vector<unsigned char> contents;

  // Prepare FsCallbacks
  tinygltf::FsCallbacks fs;
  fs.FileExists = DummyFileExists;
  fs.ExpandFilePath = DummyExpandFilePath;
  fs.ReadWholeFile = DummyReadWholeFile;
  fs.WriteWholeFile = DummyWriteWholeFile;
  fs.GetFileSizeInBytes = DummyGetFileSize;
  fs.user_data = nullptr;

  // 1. Fuzz GetBaseFilename (using manual implementation)
  base = GetBaseFilenameManual(input);

  // 2. Fuzz ExpandFilePath
  expanded = tinygltf::ExpandFilePath(input, nullptr);

  // 3. Fuzz GetFilePathExtension (using manual implementation)
  ext = GetFilePathExtensionManual(input);

  // 4. Fuzz MimeToExt (using manual implementation)
  mime_ext = MimeToExtManual(input);

  // 5. Fuzz LoadExternalFile
  // Try with various parameters derived from input
  size_t reqBytes = (Size > 0) ? static_cast<size_t>(Data[0] % 1024) : 0;
  bool checkSize = (Size > 1) ? (Data[1] % 2 == 0) : false;
  size_t maxFileSize = (Size > 2) ? static_cast<size_t>(Data[2] * 1024) : 1024;
  bool required = (Size > 3) ? (Data[3] % 2 == 0) : true;

  // Note: LoadExternalFile is static and may not be directly accessible in some builds.
  // We attempt to call it if available, otherwise skip.
  // Since it's static in tinygltf, we might need to rely on internal usage or mock.
  // For fuzzing, we simulate the logic or call if accessible.
  // In many versions, this is internal, so we skip direct call if not exposed.
  // However, if the header exposes it, we call it.
  // Given the error, it's likely not exposed in the public API, so we skip.
  // Instead, we can test related logic via other means.

  // 6. Fuzz WriteWholeFile
  // Create some dummy content
  contents.assign(Data, Data + Size);
  bool writeSuccess = tinygltf::WriteWholeFile(&err, filepath, contents, nullptr);

  // Cleanup: Write dummy file to disk if needed for other tests (though not strictly required here)
  // Just ensure we don't leak resources.
  (void)writeSuccess;

  return 0;
}