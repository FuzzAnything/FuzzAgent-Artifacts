// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
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
#include <vector>
#include <string>
#include <fstream>

static bool dummyFileExists(const std::string&, void*) { return false; }
static std::string dummyExpandFilePath(const std::string&, void*) { return ""; }
static bool dummyReadWholeFile(std::vector<unsigned char>*, std::string*, const std::string&, void*) { return false; }
static bool dummyWriteWholeFile(std::string*, const std::string&, const std::vector<unsigned char>&, void*) { return false; }
static bool dummyGetFileSizeInBytes(size_t*, std::string*, const std::string&, void*) { return false; }

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file for file-based functions
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }

    // Test ReadWholeFile
    {
        std::vector<unsigned char> out;
        std::string err;
        bool success = tinygltf::ReadWholeFile(&out, &err, "./dummy_file", nullptr);
        (void)success; // Ignore result
    }

    // Test GetFileSizeInBytes
    {
        size_t filesize;
        std::string err;
        bool success = tinygltf::GetFileSizeInBytes(&filesize, &err, "./dummy_file", nullptr);
        (void)success; // Ignore result
    }

    // Test DecodeDataURI
    {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string input(reinterpret_cast<const char*>(Data), Size);
        
        // Test with different parameters
        bool success1 = tinygltf::DecodeDataURI(&out, mime_type, input, 0, false);
        bool success2 = tinygltf::DecodeDataURI(&out, mime_type, input, Size, true);
        bool success3 = tinygltf::DecodeDataURI(&out, mime_type, input, Size/2, true);
        (void)success1; (void)success2; (void)success3; // Ignore results
    }

    // Test TinyGLTF member functions
    {
        tinygltf::TinyGLTF loader;
        
        // Test GetMaxExternalFileSize
        size_t max_size = loader.GetMaxExternalFileSize();
        (void)max_size;
        
        // Test SetMaxExternalFileSize with various values
        loader.SetMaxExternalFileSize(0);
        loader.SetMaxExternalFileSize(Size);
        loader.SetMaxExternalFileSize(1024*1024);
        loader.SetMaxExternalFileSize(2ULL*1024*1024*1024);
        
        // Test SetFsCallbacks
        tinygltf::FsCallbacks callbacks;
        callbacks.FileExists = dummyFileExists;
        callbacks.ExpandFilePath = dummyExpandFilePath;
        callbacks.ReadWholeFile = dummyReadWholeFile;
        callbacks.WriteWholeFile = dummyWriteWholeFile;
        callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
        callbacks.user_data = nullptr;
        
        std::string err;
        bool success = loader.SetFsCallbacks(callbacks, &err);
        (void)success; // Ignore result
        
        // Test with null callbacks (should fail)
        tinygltf::FsCallbacks null_callbacks = {};
        bool should_fail = loader.SetFsCallbacks(null_callbacks, &err);
        (void)should_fail; // Ignore result
    }

    return 0;
}