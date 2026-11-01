// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.h:1342:13 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
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
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "tiny_gltf.h"

// Helper function to convert fuzzer input to a safe string for file operations
static std::string SanitizeInput(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "dummy_file";
    // Replace null bytes and control characters to avoid invalid paths
    std::string result;
    result.reserve(Size);
    for (size_t i = 0; i < Size; ++i) {
        unsigned char c = Data[i];
        if (c < 32 && c != 9 && c != 10 && c != 13) {
            c = ' ';
        }
        result += static_cast<char>(c);
    }
    return result.empty() ? "dummy_file" : result;
}

// Helper to implement GetBaseFilename logic if not exposed in the namespace
static std::string GetBaseFilenameImpl(const std::string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == std::string::npos) {
        return filepath;
    }
    return filepath.substr(pos + 1);
}

// Helper to implement WriteGltfFile logic if not exposed in the namespace
static bool WriteGltfFileImpl(const std::string &output, const std::string &content) {
    std::ofstream file(output, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    file << content;
    file.close();
    return file.good();
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string filepath = SanitizeInput(Data, Size);
    
    // 1. tinygltf::ExpandFilePath
    // Resolves platform-specific path variables
    std::string expanded_path = tinygltf::ExpandFilePath(filepath, nullptr);

    // 2. tinygltf::FileExists
    // Checks if the file exists
    bool exists = tinygltf::FileExists(expanded_path, nullptr);

    // 3. tinygltf::WriteWholeFile
    // Writes binary data to disk
    std::string err;
    std::vector<unsigned char> dummy_content(Size);
    // Fill content with input data to vary test cases
    for (size_t i = 0; i < Size; ++i) {
        dummy_content[i] = Data[i];
    }
    
    // Use a safe filename for writing to avoid permission issues
    std::string write_path = "./fuzz_test_output.bin";
    bool write_success = tinygltf::WriteWholeFile(&err, write_path, dummy_content, nullptr);

    // 4. tinygltf::WriteGltfFile
    // Writes GLTF JSON content (using implementation helper as per error)
    std::string json_content = "{\"asset\":{\"version\":\"2.0\"}}";
    std::string json_path = "./fuzz_test_output.gltf";
    bool json_write_success = WriteGltfFileImpl(json_path, json_content);

    // 5. tinygltf::GetFileSizeInBytes
    // Retrieves file size
    size_t filesize = 0;
    bool size_success = tinygltf::GetFileSizeInBytes(&filesize, &err, write_path, nullptr);
    
    // Try with the original expanded path as well
    bool size_success_orig = tinygltf::GetFileSizeInBytes(&filesize, &err, expanded_path, nullptr);

    // 6. tinygltf::GetBaseFilename
    // Extracts filename from path (using implementation helper as per error)
    std::string base_name = GetBaseFilenameImpl(expanded_path);
    std::string base_name_write = GetBaseFilenameImpl(write_path);

    // Cleanup: Remove created files if they exist
    std::remove(write_path.c_str());
    std::remove(json_path.c_str());

    return 0;
}