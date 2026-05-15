// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
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
#include <vector>
#include <string>
#include <fstream>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static bool dummyFileExists(const std::string& filename, void* user_data) {
    return filename == "./dummy_file";
}

static std::string dummyExpandFilePath(const std::string& path, void* user_data) {
    return path;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out, std::string* err,
                               const std::string& filename, void* user_data) {
    if (filename == "./dummy_file") {
        std::ifstream file(filename, std::ios::binary);
        if (file) {
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);
            out->resize(size);
            file.read(reinterpret_cast<char*>(out->data()), size);
            return true;
        }
        if (err) *err = "Failed to open dummy file";
        return false;
    }
    if (err) *err = "File not found";
    return false;
}

static bool dummyWriteWholeFile(std::string* err, const std::string& filename,
                                const std::vector<unsigned char>& contents, void* user_data) {
    if (filename == "./dummy_file") {
        std::ofstream file(filename, std::ios::binary);
        if (file) {
            if (!contents.empty()) {
                file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
            }
            return true;
        }
        if (err) *err = "Failed to write dummy file";
        return false;
    }
    if (err) *err = "Invalid filename";
    return false;
}

static bool dummyGetFileSizeInBytes(size_t* filesize, std::string* err,
                                    const std::string& filename, void* user_data) {
    if (filename == "./dummy_file") {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (file) {
            *filesize = file.tellg();
            return true;
        }
        if (err) *err = "Failed to get file size";
        return false;
    }
    if (err) *err = "File not found";
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* Data, size_t Size) {
    // Write fuzzer data to dummy file
    writeDummyFile(Data, Size);
    
    std::string err;
    std::vector<unsigned char> out_vec;
    size_t file_size = 0;
    
    // Test ReadWholeFile
    tinygltf::ReadWholeFile(&out_vec, &err, "./dummy_file", nullptr);
    
    // Test GetFileSizeInBytes
    tinygltf::GetFileSizeInBytes(&file_size, &err, "./dummy_file", nullptr);
    
    // Test WriteWholeFile with various inputs
    std::vector<unsigned char> empty_vec;
    std::vector<unsigned char> non_empty_vec;
    if (Size > 0) {
        non_empty_vec.assign(Data, Data + Size);
    }
    
    // Test with empty contents
    tinygltf::WriteWholeFile(&err, "./dummy_file", empty_vec, nullptr);
    
    // Test with non-empty contents
    if (!non_empty_vec.empty()) {
        tinygltf::WriteWholeFile(&err, "./dummy_file", non_empty_vec, nullptr);
    }
    
    // Test TinyGLTF member functions
    tinygltf::TinyGLTF loader;
    
    // Test GetMaxExternalFileSize
    size_t max_size = loader.GetMaxExternalFileSize();
    
    // Test SetMaxExternalFileSize with various values
    if (Size >= sizeof(size_t)) {
        size_t new_size;
        memcpy(&new_size, Data, sizeof(size_t));
        loader.SetMaxExternalFileSize(new_size);
    } else {
        // Test with some default values
        loader.SetMaxExternalFileSize(0);  // Unlimited
        loader.SetMaxExternalFileSize(1024);  // 1KB limit
        loader.SetMaxExternalFileSize(1024 * 1024 * 1024);  // 1GB limit
    }
    
    // Test SetFsCallbacks with various callback combinations
    tinygltf::FsCallbacks callbacks;
    
    // Test 1: All callbacks set to dummy functions
    callbacks.FileExists = dummyFileExists;
    callbacks.ExpandFilePath = dummyExpandFilePath;
    callbacks.ReadWholeFile = dummyReadWholeFile;
    callbacks.WriteWholeFile = dummyWriteWholeFile;
    callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
    callbacks.user_data = nullptr;
    loader.SetFsCallbacks(callbacks, &err);
    
    // Test 2: Some null callbacks (should fail)
    tinygltf::FsCallbacks bad_callbacks;
    bad_callbacks.FileExists = dummyFileExists;
    bad_callbacks.ExpandFilePath = nullptr;  // Null callback
    bad_callbacks.ReadWholeFile = dummyReadWholeFile;
    bad_callbacks.WriteWholeFile = dummyWriteWholeFile;
    bad_callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
    bad_callbacks.user_data = nullptr;
    loader.SetFsCallbacks(bad_callbacks, &err);
    
    // Test 3: All null callbacks (should fail)
    tinygltf::FsCallbacks all_null_callbacks;
    all_null_callbacks.FileExists = nullptr;
    all_null_callbacks.ExpandFilePath = nullptr;
    all_null_callbacks.ReadWholeFile = nullptr;
    all_null_callbacks.WriteWholeFile = nullptr;
    all_null_callbacks.GetFileSizeInBytes = nullptr;
    all_null_callbacks.user_data = nullptr;
    loader.SetFsCallbacks(all_null_callbacks, &err);
    
    // Test with different user_data pointers
    int user_data = 42;
    callbacks.user_data = &user_data;
    loader.SetFsCallbacks(callbacks, &err);
    
    return 0;
}