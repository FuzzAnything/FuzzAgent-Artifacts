// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
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
#include <fstream>
#include <vector>
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static bool dummyFileExists(const std::string& filename, void* user_data) {
    return true;
}

static std::string dummyExpandFilePath(const std::string& filepath, void* user_data) {
    return filepath;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out, std::string* err,
                               const std::string& filepath, void* user_data) {
    if (out == nullptr) return false;
    out->push_back('d');
    out->push_back('u');
    out->push_back('m');
    out->push_back('m');
    out->push_back('y');
    return true;
}

static bool dummyWriteWholeFile(std::string* err, const std::string& filename,
                                const std::vector<unsigned char>& contents,
                                void* user_data) {
    return true;
}

static bool dummyGetFileSizeInBytes(size_t* filesize_out, std::string* err,
                                    const std::string& filepath, void* user_data) {
    if (filesize_out == nullptr) return false;
    *filesize_out = 42;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* Data, size_t Size) {
    // Write fuzzer data to dummy file for file-based operations
    writeDummyFile(Data, Size);
    
    // Test GetFileSizeInBytes
    {
        size_t file_size = 0;
        std::string err;
        bool success = tinygltf::GetFileSizeInBytes(&file_size, &err, "./dummy_file", nullptr);
        (void)success; // Use result to avoid unused variable warning
    }
    
    // Test SetMaxExternalFileSize
    {
        tinygltf::TinyGLTF loader;
        if (Size >= sizeof(size_t)) {
            size_t max_size = 0;
            memcpy(&max_size, Data, sizeof(size_t));
            loader.SetMaxExternalFileSize(max_size);
        } else {
            loader.SetMaxExternalFileSize(Size);
        }
    }
    
    // Test DecodeDataURI
    {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string in_str(reinterpret_cast<const char*>(Data), Size);
        
        // Test with checkSize = false
        bool success1 = tinygltf::DecodeDataURI(&out, mime_type, in_str, 0, false);
        (void)success1;
        
        // Test with checkSize = true and random required bytes
        size_t req_bytes = 0;
        if (Size >= sizeof(size_t)) {
            memcpy(&req_bytes, Data, sizeof(size_t));
        }
        bool success2 = tinygltf::DecodeDataURI(&out, mime_type, in_str, req_bytes, true);
        (void)success2;
    }
    
    // Test LoadASCIIFromFile
    {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;
        std::string err;
        std::string warn;
        
        // Try loading with default check_sections
        bool success1 = loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
        (void)success1;
        
        // Try loading with different check_sections values
        unsigned int check_sections = 0;
        if (Size >= sizeof(unsigned int)) {
            memcpy(&check_sections, Data, sizeof(unsigned int));
        }
        bool success2 = loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", check_sections);
        (void)success2;
    }
    
    // Test ReadWholeFile
    {
        std::vector<unsigned char> out;
        std::string err;
        bool success = tinygltf::ReadWholeFile(&out, &err, "./dummy_file", nullptr);
        (void)success;
    }
    
    // Test SetFsCallbacks
    {
        tinygltf::TinyGLTF loader;
        tinygltf::FsCallbacks callbacks;
        std::string err;
        
        // Set valid callbacks
        callbacks.FileExists = dummyFileExists;
        callbacks.ExpandFilePath = dummyExpandFilePath;
        callbacks.ReadWholeFile = dummyReadWholeFile;
        callbacks.WriteWholeFile = dummyWriteWholeFile;
        callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
        callbacks.user_data = nullptr;
        
        bool success1 = loader.SetFsCallbacks(callbacks, &err);
        (void)success1;
        
        // Test with null callbacks (should fail)
        tinygltf::FsCallbacks null_callbacks = {};
        bool success2 = loader.SetFsCallbacks(null_callbacks, &err);
        (void)success2;
    }
    
    return 0;
}