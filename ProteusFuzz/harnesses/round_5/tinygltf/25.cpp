// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
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
#include <fstream>
#include <functional>
#include "tiny_gltf.h"

static void WriteDummyFile(const std::string &path, const uint8_t *data, size_t size) {
    std::ofstream file(path, std::ios::binary);
    if (file.is_open()) {
        if (size > 0) {
            file.write(reinterpret_cast<const char*>(data), size);
        }
        file.close();
    }
}

// Stub for ExpandFilePath to satisfy FsCallbacks requirements
// Signature: std::string ExpandFilePathFunction(const std::string &filepath, void *);
static std::string DummyExpandFilePath(const std::string &filepath, void *) {
    return filepath;
}

// Helper to implement GetBaseFilename logic
static std::string GetBaseFilenameLocal(const std::string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return filepath;
    }
    return filepath.substr(pos + 1);
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *dummy_path = "./dummy_file";

    // Write fuzzing data to dummy file for file-based operations
    WriteDummyFile(dummy_path, Data, Size);

    std::string err;
    std::string warn;
    std::vector<unsigned char> out;
    size_t filesize = 0;

    // 1. Test FileExists
    bool exists = tinygltf::FileExists(dummy_path, nullptr);
    (void)exists;

    // Test FileExists with a non-existent file
    bool not_exists = tinygltf::FileExists("./non_existent_file_xyz", nullptr);
    (void)not_exists;

    // 2. Test ReadWholeFile
    out.clear();
    err.clear();
    bool read_success = tinygltf::ReadWholeFile(&out, &err, dummy_path, nullptr);
    (void)read_success;

    // Test ReadWholeFile with non-existent file
    out.clear();
    err.clear();
    bool read_fail = tinygltf::ReadWholeFile(&out, &err, "./non_existent_file_xyz", nullptr);
    (void)read_fail;

    // 3. Test GetBaseFilename (using local implementation)
    std::string base = GetBaseFilenameLocal(dummy_path);
    (void)base;
    std::string base_nested = GetBaseFilenameLocal("/path/to/./dummy_file");
    (void)base_nested;
    std::string base_no_sep = GetBaseFilenameLocal("just_filename");
    (void)base_no_sep;

    // 4. Test GetFileSizeInBytes
    filesize = 0;
    err.clear();
    bool size_success = tinygltf::GetFileSizeInBytes(&filesize, &err, dummy_path, nullptr);
    (void)size_success;

    // Test GetFileSizeInBytes with non-existent file
    filesize = 0;
    err.clear();
    bool size_fail = tinygltf::GetFileSizeInBytes(&filesize, &err, "./non_existent_file_xyz", nullptr);
    (void)size_fail;

    // 5. Test WriteWholeFile
    std::vector<unsigned char> write_data(Size);
    std::memcpy(write_data.data(), Data, Size);
    std::string write_path = "./fuzz_output_file";
    err.clear();
    bool write_success = tinygltf::WriteWholeFile(&err, write_path, write_data, nullptr);
    (void)write_success;

    // 6. Test LoadExternalFile
    // Note: LoadExternalFile is often a static method inside the Model class in some versions,
    // but the compiler error indicates it's not available as tinygltf::Model::LoadExternalFile.
    // In some versions, it might be a free function or inside a different class.
    // Since we cannot determine the exact location from the error, we will skip calling it directly
    // to avoid compilation errors, but we can still test the FsCallbacks setup.
    
    // Setup FsCallbacks with default implementations
    tinygltf::FsCallbacks fs;
    fs.FileExists = tinygltf::FileExists;
    fs.ReadWholeFile = tinygltf::ReadWholeFile;
    fs.WriteWholeFile = tinygltf::WriteWholeFile;
    fs.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
    fs.ExpandFilePath = DummyExpandFilePath; // Fixed signature
    fs.user_data = nullptr;

    // Since LoadExternalFile is not accessible as a static method of Model, we skip it.
    // However, we can still test the FsCallbacks setup and other functions.

    return 0;
}