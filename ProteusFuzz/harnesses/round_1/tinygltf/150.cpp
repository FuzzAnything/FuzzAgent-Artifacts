// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
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
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

static void WriteDummyFile(const std::string& filename, const uint8_t* data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to test file-related functions
    const std::string dummy_filename = "./dummy_file";
    WriteDummyFile(dummy_filename, Data, Size);

    tinygltf::TinyGLTF ctx;
    std::string err;
    std::vector<unsigned char> out;
    size_t filesize_out = 0;

    // Test SetMaxExternalFileSize and GetMaxExternalFileSize
    // Use data from input to determine max file size, ensuring it's not zero to avoid trivial cases
    size_t max_bytes = 0;
    if (Size >= sizeof(size_t)) {
        max_bytes = *reinterpret_cast<const size_t*>(Data);
    } else {
        max_bytes = Size;
    }
    // Ensure a reasonable range to avoid immediate failure or overflow, but still fuzz
    if (max_bytes == 0) max_bytes = 1;
    
    ctx.SetMaxExternalFileSize(max_bytes);
    size_t current_limit = ctx.GetMaxExternalFileSize();
    (void)current_limit; // Suppress unused variable warning

    // Test FileExists
    bool exists = tinygltf::FileExists(dummy_filename, nullptr);
    (void)exists;

    // Test ReadWholeFile
    bool read_success = tinygltf::ReadWholeFile(&out, &err, dummy_filename, nullptr);
    (void)read_success;
    out.clear(); // Cleanup

    // Test GetFileSizeInBytes (Overload 1: with void* userdata)
    bool size_success1 = tinygltf::GetFileSizeInBytes(&filesize_out, &err, dummy_filename, nullptr);
    (void)size_success1;

    // Test GetFileSizeInBytes (Overload 2: explicit void*)
    // Note: The signatures provided are identical in C++ for the user, 
    // but we call it again to ensure coverage of the specific overload if distinct in implementation.
    bool size_success2 = tinygltf::GetFileSizeInBytes(&filesize_out, &err, dummy_filename, (void*)nullptr);
    (void)size_success2;

    // Test with a non-existent file to exercise error paths
    const std::string non_existent = "./non_existent_file_xyz";
    bool exists_non = tinygltf::FileExists(non_existent, nullptr);
    (void)exists_non;

    bool read_fail = tinygltf::ReadWholeFile(&out, &err, non_existent, nullptr);
    (void)read_fail;

    bool size_fail1 = tinygltf::GetFileSizeInBytes(&filesize_out, &err, non_existent, nullptr);
    (void)size_fail1;

    bool size_fail2 = tinygltf::GetFileSizeInBytes(&filesize_out, &err, non_existent, (void*)nullptr);
    (void)size_fail2;

    return 0;
}