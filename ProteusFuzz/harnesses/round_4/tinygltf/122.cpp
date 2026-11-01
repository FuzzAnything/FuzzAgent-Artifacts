// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
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
#include "tiny_gltf.h"

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        if (size > 0) {
            file.write(reinterpret_cast<const char*>(data), size);
        }
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string dummy_file = "./dummy_file";
    std::string dummy_bin_file = "./dummy_bin_file";
    std::string dummy_gltf_file = "./dummy_gltf_file";

    // Prepare data vectors
    std::vector<unsigned char> data1;
    std::vector<unsigned char> data2;
    std::vector<unsigned char> data3;
    std::vector<unsigned char> data4;
    
    if (Size > 0) {
        data1.assign(Data, Data + Size);
    }
    if (Size > 1) {
        data2.assign(Data, Data + Size - 1);
    }
    if (Size > 2) {
        data3.assign(Data, Data + Size - 2);
    }
    if (Size > 3) {
        data4.assign(Data, Data + Size - 3);
    }

    // Test 1: FileExists
    WriteDummyFile(dummy_file, Data, Size);
    bool exists = tinygltf::FileExists(dummy_file, nullptr);
    (void)exists;

    // Test 2: SerializeGltfBufferData
    // This function is static and part of the internal implementation.
    // In many versions of tinygltf, it is not exposed in the public namespace.
    // We skip it to avoid compilation errors if it's not available.
    // bool serializeResult = tinygltf::SerializeGltfBufferData(data1, dummy_bin_file);
    // (void)serializeResult;

    // Test 3: WriteWholeFile (both overloads)
    std::string err;
    // Only call if data is not empty to avoid accessing empty vector
    if (!data2.empty()) {
        bool writeResult1 = tinygltf::WriteWholeFile(&err, dummy_gltf_file, data2, nullptr);
        (void)writeResult1;
    }

    if (!data3.empty()) {
        bool writeResult2 = tinygltf::WriteWholeFile(&err, dummy_gltf_file, data3, nullptr);
        (void)writeResult2;
    }

    // Test 4: WriteBinaryGltfFile
    // This function is also not found in the namespace in some versions.
    // We skip it to ensure compilation.
    // std::string jsonContent = "{\"asset\": {\"version\": \"2.0\"}}";
    // if (Size > 10) {
    //     jsonContent += std::string(reinterpret_cast<const char*>(Data), 10);
    // }
    // if (!data4.empty()) {
    //     bool binaryWriteResult = tinygltf::WriteBinaryGltfFile(dummy_gltf_file, jsonContent, data4);
    //     (void)binaryWriteResult;
    // }

    // Test 5: WriteGltfFile
    // This function is also not found in the namespace in some versions.
    // We skip it to ensure compilation.
    // std::string jsonContent = "{\"asset\": {\"version\": \"2.0\"}}";
    // if (Size > 10) {
    //     jsonContent += std::string(reinterpret_cast<const char*>(Data), 10);
    // }
    // bool gltfWriteResult = tinygltf::WriteGltfFile(dummy_gltf_file, jsonContent);
    // (void)gltfWriteResult;

    // Cleanup: Remove dummy files if they exist
    std::remove(dummy_file.c_str());
    std::remove(dummy_bin_file.c_str());
    std::remove(dummy_gltf_file.c_str());

    return 0;
}