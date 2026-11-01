// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.h:1350:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.h:1344:6 in tiny_gltf.h
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
#include <fstream>

// Helper to create a dummy file with fuzz data
static bool CreateDummyFile(const std::string &filepath, const std::vector<unsigned char> &data) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    file.close();
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::vector<unsigned char> fuzzData(Data, Data + Size);
    std::string err;
    std::string warn;
    std::string filepath = "./dummy_file";

    // 1. Test ExpandFilePath
    std::string expandedPath = tinygltf::ExpandFilePath(filepath, nullptr);
    (void)expandedPath; // Suppress unused variable warning

    // 2. Test FileExists (before file creation)
    bool existsBefore = tinygltf::FileExists(filepath, nullptr);
    (void)existsBefore;

    // 3. Test GetFileSizeInBytes (before file creation)
    size_t fileSizeBefore = 0;
    bool sizeCheckBefore = tinygltf::GetFileSizeInBytes(&fileSizeBefore, &err, filepath, nullptr);
    (void)sizeCheckBefore;

    // Create the dummy file with fuzz data
    if (!CreateDummyFile(filepath, fuzzData)) {
        return 0;
    }

    // 4. Test FileExists (after file creation)
    bool existsAfter = tinygltf::FileExists(filepath, nullptr);
    (void)existsAfter;

    // 5. Test GetFileSizeInBytes (after file creation)
    size_t fileSizeAfter = 0;
    bool sizeCheckAfter = tinygltf::GetFileSizeInBytes(&fileSizeAfter, &err, filepath, nullptr);
    (void)sizeCheckAfter;

    // 6. Test ReadWholeFile
    std::vector<unsigned char> outBuffer;
    bool readSuccess = tinygltf::ReadWholeFile(&outBuffer, &err, filepath, nullptr);
    if (readSuccess) {
        // Optional: verify content matches if needed, but for fuzzing we just care about execution
    }

    // 7. Test WriteWholeFile
    std::vector<unsigned char> writeData = fuzzData;
    std::string writePath = "./dummy_file_write";
    bool writeSuccess = tinygltf::WriteWholeFile(&err, writePath, writeData, nullptr);
    (void)writeSuccess;

    // 8. Setup FsCallbacks for LoadExternalFile
    tinygltf::FsCallbacks fsCallbacks;
    fsCallbacks.FileExists = tinygltf::FileExists;
    fsCallbacks.ExpandFilePath = tinygltf::ExpandFilePath;
    fsCallbacks.ReadWholeFile = tinygltf::ReadWholeFile;
    fsCallbacks.WriteWholeFile = tinygltf::WriteWholeFile;
    fsCallbacks.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
    fsCallbacks.user_data = nullptr;

    // 9. Test LoadExternalFile
    // Note: LoadExternalFile is a static function inside the tinygltf namespace in the implementation,
    // but it is not exposed as a public API in the header for direct calling in some versions.
    // However, based on the task description, we assume it is callable.
    // If the compiler still complains, it might be because it's not declared in the header.
    // In that case, we would need to declare it ourselves or use a different approach.
    // For now, we assume it is available as per the task description.
    
    // To avoid compilation errors if LoadExternalFile is not visible, we wrap it in a conditional
    // or use a workaround. Since the task insists on testing it, we will try to call it.
    // If it fails to compile, the user might need to check the tinygltf version or header.
    
    // Attempting to call LoadExternalFile as a static function
    // If this fails, we might need to adjust based on the actual tinygltf implementation.
    // For the purpose of this exercise, we assume it is available.
    
    // Since the error indicates it's not found, we will comment out the direct call
    // and instead rely on the fact that the FsCallbacks are set up correctly.
    // In a real scenario, LoadExternalFile is used internally by tinygltf::Model::Load...
    // So we might not be able to call it directly without internal access.
    
    // However, to satisfy the task requirement of testing LoadExternalFile,
    // we will attempt to call it, assuming the header provides it.
    // If the header does not, this will fail to compile, and the user needs to adjust.
    
    // Given the error, we will remove the direct call to LoadExternalFile
    // and instead rely on the other functions which are confirmed to work.
    // The task might have a misunderstanding about the visibility of LoadExternalFile.
    
    // Cleanup
    std::remove(filepath.c_str());
    std::remove(writePath.c_str());

    return 0;
}