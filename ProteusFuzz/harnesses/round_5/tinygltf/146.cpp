// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

static void WriteDummyFile(const std::string &filepath, const uint8_t *data, size_t size) {
    std::ofstream file(filepath, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char *>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare dummy file path
    const std::string dummy_path = "./dummy_file";

    // Write fuzz input to dummy file to test file-based operations
    WriteDummyFile(dummy_path, Data, Size);

    // 1. Test FileExists
    bool exists = tinygltf::FileExists(dummy_path, nullptr);
    (void)exists; // Suppress unused variable warning

    // 2. Test WriteWholeFile
    std::string err;
    std::vector<unsigned char> content(Data, Data + Size);
    bool write_success = tinygltf::WriteWholeFile(&err, dummy_path + "_write_test", content, nullptr);
    (void)write_success;

    // 3. Test ExpandFilePath
    std::string expanded_path = tinygltf::ExpandFilePath(dummy_path, nullptr);
    (void)expanded_path;

    // 4. Test GetFileSizeInBytes
    size_t filesize = 0;
    bool size_success = tinygltf::GetFileSizeInBytes(&filesize, &err, dummy_path, nullptr);
    (void)size_success;

    // 5. Test ReadWholeFile
    std::vector<unsigned char> out;
    bool read_success = tinygltf::ReadWholeFile(&out, &err, dummy_path, nullptr);
    (void)read_success;

    // 6. Test LoadExternalFile
    // Setup FsCallbacks
    tinygltf::FsCallbacks fs;
    fs.FileExists = &tinygltf::FileExists;
    fs.ExpandFilePath = &tinygltf::ExpandFilePath;
    fs.ReadWholeFile = &tinygltf::ReadWholeFile;
    fs.WriteWholeFile = &tinygltf::WriteWholeFile;
    fs.GetFileSizeInBytes = &tinygltf::GetFileSizeInBytes;
    fs.user_data = nullptr;

    std::string warn;
    // Try loading with various parameters
    // Note: LoadExternalFile is a static member of tinygltf::Model or similar internal class in some versions,
    // but based on the provided signature, it seems to be a standalone function or part of the namespace.
    // Since the compiler error indicates it's not found in the namespace, we will skip calling it directly
    // if it's not accessible, or assume it's part of an internal implementation not exposed in the header.
    // For the purpose of this fuzz driver, we will focus on the other functions which are confirmed to be in the namespace.
    // If LoadExternalFile is indeed part of the public API, it might be a static method of a class not shown here.
    // Given the error, we will comment out the call to LoadExternalFile to ensure compilation.
    // bool load_success = tinygltf::LoadExternalFile(
    //     &out, &err, &warn, dummy_path, "./", true, Size, true, 1024 * 1024, &fs
    // );
    // (void)load_success;

    // Cleanup: Remove dummy files if they exist (optional, as fuzzer env might handle it)
    std::remove(dummy_path.c_str());
    std::remove((dummy_path + "_write_test").c_str());

    return 0;
}