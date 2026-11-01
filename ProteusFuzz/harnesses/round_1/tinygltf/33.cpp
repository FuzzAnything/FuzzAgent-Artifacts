// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
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
#include <fstream>

#include "tiny_gltf.h"

static void WriteDummyFile(const std::string &path, const uint8_t *data, size_t size) {
    std::ofstream ofs(path, std::ios::binary);
    if (ofs.is_open()) {
        ofs.write(reinterpret_cast<const char*>(data), size);
        ofs.close();
    }
}

static void CleanupDummyFile(const std::string &path) {
    std::remove(path.c_str());
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const std::string dummy_path = "./dummy_file";
    std::string err;
    std::string warn;
    std::vector<unsigned char> out;
    size_t filesize_out = 0;

    // Write fuzzing data to dummy file
    WriteDummyFile(dummy_path, Data, Size);

    // 1. Fuzz tinygltf::ReadWholeFile
    out.clear();
    err.clear();
    tinygltf::ReadWholeFile(&out, &err, dummy_path, nullptr);

    // 2. Fuzz tinygltf::GetFileSizeInBytes
    filesize_out = 0;
    err.clear();
    tinygltf::GetFileSizeInBytes(&filesize_out, &err, dummy_path, nullptr);

    // 3. Fuzz tinygltf::ExpandFilePath
    std::string expanded_path = tinygltf::ExpandFilePath(dummy_path, nullptr);
    (void)expanded_path; // Suppress unused warning

    // 4. Fuzz tinygltf::FileExists
    bool exists = tinygltf::FileExists(dummy_path, nullptr);
    (void)exists; // Suppress unused warning

    // 5. Fuzz tinygltf::WriteWholeFile
    std::vector<unsigned char> write_content;
    if (Size > 0) {
        write_content.assign(Data, Data + Size);
    }
    err.clear();
    tinygltf::WriteWholeFile(&err, dummy_path, write_content, nullptr);

    // 6. Fuzz tinygltf::LoadExternalFile
    // LoadExternalFile is a static helper function inside tinygltf::ModelLoader or similar internal scope.
    // However, based on the provided signature, it seems to be intended as a standalone static function.
    // If it's not directly accessible in the global tinygltf namespace, we might need to check the actual header.
    // Assuming the provided signature is correct and it should be accessible, but the compiler says it's not.
    // Let's try to call it as a static function if it exists, or skip if not.
    // Since the error indicates it's not in the namespace, we will comment out the direct call 
    // and rely on the other functions which are confirmed to exist.
    // If LoadExternalFile is indeed a static member of a class, we would need an instance.
    // Given the constraints, we will omit the direct call to LoadExternalFile to ensure compilation.
    
    // Prepare FsCallbacks for potential internal use or future expansion
    tinygltf::FsCallbacks fs;
    fs.FileExists = tinygltf::FileExists;
    fs.ExpandFilePath = tinygltf::ExpandFilePath;
    fs.ReadWholeFile = tinygltf::ReadWholeFile;
    fs.WriteWholeFile = tinygltf::WriteWholeFile;
    fs.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
    fs.user_data = nullptr;

    // Test with valid file using ReadWholeFile which is part of FsCallbacks
    out.clear();
    err.clear();
    tinygltf::ReadWholeFile(&out, &err, dummy_path, fs.user_data);

    // Test with non-existent file
    std::string non_existent = "./non_existent_file_" + std::to_string(Size);
    out.clear();
    err.clear();
    tinygltf::ReadWholeFile(&out, &err, non_existent, fs.user_data);

    // Cleanup
    CleanupDummyFile(dummy_path);
    CleanupDummyFile(non_existent);

    return 0;
}