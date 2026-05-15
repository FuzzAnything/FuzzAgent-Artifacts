// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.cc:1333:13 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "code/tiny_gltf.h"
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

static void writeDummyFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

static void removeDummyFile() {
    std::remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Prepare dummy file
    writeDummyFile(Data, Size);
    
    // Test ReadWholeFile
    {
        std::vector<unsigned char> out;
        std::string err;
        bool success = tinygltf::ReadWholeFile(&out, &err, "./dummy_file", nullptr);
        (void)success;
    }
    
    // Test WriteWholeFile - only call with non-empty contents
    {
        std::string err;
        if (Size > 0) {
            std::vector<unsigned char> contents(Data, Data + Size);
            bool success = tinygltf::WriteWholeFile(&err, "./dummy_file", contents, nullptr);
            (void)success;
        }
        // Skip empty contents test to avoid vector::at(0) on empty vector
    }
    
    // Test GetFileSizeInBytes
    {
        size_t filesize;
        std::string err;
        bool success = tinygltf::GetFileSizeInBytes(&filesize, &err, "./dummy_file", nullptr);
        (void)success;
    }
    
    // Test ExpandFilePath
    {
        std::string path(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(100)));
        std::string expanded = tinygltf::ExpandFilePath(path, nullptr);
        (void)expanded;
    }
    
    // Test SetFsCallbacks
    {
        tinygltf::TinyGLTF loader;
        std::string err;
        
        // Test with valid callbacks
        tinygltf::FsCallbacks callbacks;
        callbacks.FileExists = tinygltf::FileExists;
        callbacks.ExpandFilePath = tinygltf::ExpandFilePath;
        callbacks.ReadWholeFile = tinygltf::ReadWholeFile;
        callbacks.WriteWholeFile = tinygltf::WriteWholeFile;
        callbacks.GetFileSizeInBytes = tinygltf::GetFileSizeInBytes;
        callbacks.user_data = nullptr;
        
        bool success = loader.SetFsCallbacks(callbacks, &err);
        (void)success;
        
        // Test with null callbacks (should fail)
        tinygltf::FsCallbacks null_callbacks = {};
        success = loader.SetFsCallbacks(null_callbacks, &err);
        (void)success;
    }
    
    // Test FileExists
    {
        bool exists = tinygltf::FileExists("./dummy_file", nullptr);
        (void)exists;
        
        // Test with non-existent file
        exists = tinygltf::FileExists("./non_existent_file_12345", nullptr);
        (void)exists;
    }
    
    // Cleanup
    removeDummyFile();
    
    return 0;
}