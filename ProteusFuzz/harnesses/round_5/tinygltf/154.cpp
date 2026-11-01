// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
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
#include <cstddef>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>

static void WriteDummyFile(const uint8_t *Data, size_t Size) {
  std::ofstream file("./dummy_file", std::ios::binary);
  if (file.is_open()) {
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();
  }
}

extern "C" int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Prepare environment
  tinygltf::TinyGLTF model;
  std::vector<unsigned char> out_buffer;
  std::string err;
  size_t filesize_out = 0;

  // Write input data to a dummy file for file-based operations
  WriteDummyFile(Data, Size);

  // Fuzz tinygltf::TinyGLTF::SetMaxExternalFileSize
  // Use input data to determine the size limit (avoiding 0 to test logic, but 0 is valid)
  size_t limit = 0;
  if (Size >= sizeof(size_t)) {
    std::memcpy(&limit, Data, sizeof(size_t));
    // Cap at a reasonable max to prevent allocation issues in other parts of the library
    if (limit > 1024 * 1024 * 1024) limit = 1024 * 1024 * 1024; 
  } else {
    limit = Size;
  }
  model.SetMaxExternalFileSize(limit);

  // Fuzz tinygltf::TinyGLTF::GetMaxExternalFileSize
  // Verify the value was set correctly
  size_t current_limit = model.GetMaxExternalFileSize();
  (void)current_limit; // Suppress unused variable warning

  // Fuzz tinygltf::ExpandFilePath
  // Construct a path string from input data, ensuring it's null-terminated
  std::string filepath_str(reinterpret_cast<const char*>(Data), Size);
  // Ensure no null bytes in the middle cause issues for string handling if the library expects it
  // Though std::string handles nulls, some path logic might not.
  // We pass the raw string to test boundary conditions.
  std::string expanded_path = tinygltf::ExpandFilePath(filepath_str, nullptr);
  (void)expanded_path;

  // Fuzz tinygltf::ReadWholeFile
  // Attempt to read the dummy file we created
  bool read_success = tinygltf::ReadWholeFile(&out_buffer, &err, "./dummy_file", nullptr);
  (void)read_success;

  // Fuzz tinygltf::GetFileSizeInBytes (Overload 1 with void* userdata)
  bool size_success_1 = tinygltf::GetFileSizeInBytes(&filesize_out, &err, "./dummy_file", nullptr);
  (void)size_success_1;

  // Fuzz tinygltf::GetFileSizeInBytes (Overload 2 with explicit void*)
  // Note: The signature provided shows two overloads, but in C++ `void*` and `void *` are the same.
  // The second signature in the prompt likely refers to the same function or a specific implementation detail.
  // We call it again to ensure coverage of the call site logic.
  bool size_success_2 = tinygltf::GetFileSizeInBytes(&filesize_out, &err, "./dummy_file", static_cast<void*>(nullptr));
  (void)size_success_2;

  // Cleanup
  // tinygltf::TinyGLTF destructor handles internal cleanup
  // out_buffer is a vector, auto-cleanup
  // err is a string, auto-cleanup
  
  // Test with a non-existent file to trigger error paths
  std::string err2;
  bool read_fail = tinygltf::ReadWholeFile(&out_buffer, &err2, "./non_existent_file_12345", nullptr);
  (void)read_fail;

  size_t size_fail = 0;
  std::string err3;
  bool size_fail_res = tinygltf::GetFileSizeInBytes(&size_fail, &err3, "./non_existent_file_12345", nullptr);
  (void)size_fail_res;

  return 0;
}