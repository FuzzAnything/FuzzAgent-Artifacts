// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.cc:1333:13 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.cc:1333:13 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
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
#include <vector>
#include <string>
#include <fstream>

static void writeDummyFile(const std::string& path, const uint8_t* data, size_t size) {
    std::ofstream file(path, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* Data, size_t Size) {
    // Write fuzzer data to dummy file for file-based operations
    const std::string dummy_path = "./dummy_file";
    writeDummyFile(dummy_path, Data, Size);
    
    // Test GetFileSizeInBytes
    {
        size_t file_size = 0;
        std::string err;
        bool success = tinygltf::GetFileSizeInBytes(&file_size, &err, dummy_path, nullptr);
        (void)success; // Use result to avoid unused variable warning
    }
    
    // Test WriteWholeFile
    {
        std::string err;
        std::vector<unsigned char> contents;
        if (Size > 0) {
            contents.assign(Data, Data + Size);
        }
        // Test both empty and non-empty cases
        bool success = tinygltf::WriteWholeFile(&err, dummy_path + "_out", contents, nullptr);
        (void)success;
    }
    
    // Test FileExists
    {
        bool exists = tinygltf::FileExists(dummy_path, nullptr);
        (void)exists;
        // Also test non-existent file
        bool not_exists = tinygltf::FileExists("./non_existent_file_12345", nullptr);
        (void)not_exists;
    }
    
    // Test ExpandFilePath
    {
        std::string expanded = tinygltf::ExpandFilePath(dummy_path, nullptr);
        (void)expanded;
        // Test with potential expansion patterns (even though disabled)
        std::string with_tilde = tinygltf::ExpandFilePath("~/test", nullptr);
        (void)with_tilde;
    }
    
    // Test ReadWholeFile
    {
        std::vector<unsigned char> out;
        std::string err;
        bool success = tinygltf::ReadWholeFile(&out, &err, dummy_path, nullptr);
        (void)success;
    }
    
    // Test SetFsCallbacks
    {
        tinygltf::TinyGLTF loader;
        std::string err;
        
        // Test with null callbacks (should fail)
        tinygltf::FsCallbacks null_callbacks = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        bool fail_result = loader.SetFsCallbacks(null_callbacks, &err);
        (void)fail_result;
        
        // Test with valid callbacks
        tinygltf::FsCallbacks valid_callbacks = {
            tinygltf::FileExists,
            tinygltf::ExpandFilePath,
            tinygltf::ReadWholeFile,
            tinygltf::WriteWholeFile,
            tinygltf::GetFileSizeInBytes,
            nullptr
        };
        bool success_result = loader.SetFsCallbacks(valid_callbacks, &err);
        (void)success_result;
    }
    
    // Clean up created files
    std::remove(dummy_path.c_str());
    std::remove((dummy_path + "_out").c_str());
    
    return 0;
}