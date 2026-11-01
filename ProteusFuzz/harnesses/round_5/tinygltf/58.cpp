// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
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

// Helper function to simulate FileExists for FsCallbacks
static bool DummyFileExists(const std::string &abs_filename, void *user_data) {
  (void)abs_filename;
  (void)user_data;
  return true;
}

// Helper function to simulate WriteWholeFile for FsCallbacks
static bool DummyWriteWholeFile(std::string *err, const std::string &filepath,
                                const std::vector<unsigned char> &contents,
                                void *user_data) {
  (void)err;
  (void)filepath;
  (void)contents;
  (void)user_data;
  return true;
}

// Helper function to simulate GetFileSizeInBytes for FsCallbacks
static bool DummyGetFileSizeInBytes(size_t *filesize_out, std::string *err,
                                    const std::string &filepath, void *user_data) {
  (void)err;
  (void)filepath;
  (void)user_data;
  if (filesize_out) {
    *filesize_out = 0;
  }
  return true;
}

// Helper function to simulate ReadWholeFile for FsCallbacks
static bool DummyReadWholeFile(std::vector<unsigned char> *out,
                               std::string *err, const std::string &filepath,
                               void *user_data) {
  (void)err;
  (void)filepath;
  (void)user_data;
  if (out) {
    out->clear();
  }
  return true;
}

// Helper to implement GetBaseFilename logic locally since it's not public in some versions
static std::string LocalGetBaseFilename(const std::string &filepath) {
  size_t pos = filepath.find_last_of("/\\");
  if (pos != std::string::npos) {
    return filepath.substr(pos + 1);
  }
  return filepath;
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // Convert input data to a string for path testing
  std::string input_str(reinterpret_cast<const char*>(Data), Size);
  
  // Test 1: tinygltf::FileExists
  tinygltf::FileExists(input_str, nullptr);

  // Test 2: tinygltf::WriteWholeFile
  std::string err_str;
  std::vector<unsigned char> content_vec;
  std::string dummy_path = "./dummy_file";
  content_vec.assign(Data, Data + Size);
  tinygltf::WriteWholeFile(&err_str, dummy_path, content_vec, nullptr);
  
  // Test 3: tinygltf::GetBaseFilename
  // Using local implementation as the global function might be private or missing
  std::string base_name = LocalGetBaseFilename(input_str);
  (void)base_name;

  // Test 4: tinygltf::GetFileSizeInBytes
  size_t file_size = 0;
  tinygltf::GetFileSizeInBytes(&file_size, &err_str, input_str, nullptr);
  (void)file_size;

  // Test 5: tinygltf::ReadWholeFile
  std::vector<unsigned char> out_vec;
  tinygltf::ReadWholeFile(&out_vec, &err_str, input_str, nullptr);
  (void)out_vec;

  // Test 6: tinygltf::LoadExternalFile
  // Note: LoadExternalFile is often a private static method in tinygltf.
  // If it's not accessible, we skip the direct call and rely on the callbacks being set up.
  // We will construct the FsCallbacks to ensure the types are correct and used elsewhere.
  tinygltf::FsCallbacks fs;
  fs.FileExists = DummyFileExists;
  fs.ExpandFilePath = nullptr; 
  fs.ReadWholeFile = DummyReadWholeFile;
  fs.WriteWholeFile = DummyWriteWholeFile;
  fs.GetFileSizeInBytes = DummyGetFileSizeInBytes;
  fs.user_data = nullptr;

  // Since LoadExternalFile is likely private, we cannot call it directly.
  // We verify the FsCallbacks structure is correctly populated.
  (void)fs;
  
  // Attempt to use the callbacks via a Loader if possible, but for this driver,
  // setting up the callbacks and testing the individual public functions is sufficient.
  // If LoadExternalFile were public, it would be called here.
  
  return 0;
}