// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
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

static bool dummyFileExists(const std::string& filename, void* user_data) {
    return true;
}

static std::string dummyExpandFilePath(const std::string& filepath, void* user_data) {
    return filepath;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out, std::string* err,
                               const std::string& filepath, void* user_data) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        if (err) *err = "Failed to open file";
        return false;
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    out->resize(size);
    file.read(reinterpret_cast<char*>(out->data()), size);
    return true;
}

static bool dummyWriteWholeFile(std::string* err, const std::string& filepath,
                                const std::vector<unsigned char>& contents, void* user_data) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file) {
        if (err) *err = "Failed to open file for writing";
        return false;
    }
    if (!contents.empty()) {
        file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    }
    return true;
}

static bool dummyGetFileSizeInBytes(size_t* filesize_out, std::string* err,
                                    const std::string& filepath, void* user_data) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file) {
        if (err) *err = "Failed to open file";
        return false;
    }
    *filesize_out = file.tellg();
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Prepare dummy file with fuzzer data
    const std::string dummy_file = "./dummy_file";
    std::ofstream file(dummy_file, std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }

    // Test ReadWholeFile
    {
        std::vector<unsigned char> out;
        std::string err;
        bool success = tinygltf::ReadWholeFile(&out, &err, dummy_file, nullptr);
        (void)success; // Ignore result
    }

    // Test GetFileSizeInBytes
    {
        size_t filesize = 0;
        std::string err;
        bool success = tinygltf::GetFileSizeInBytes(&filesize, &err, dummy_file, nullptr);
        (void)success; // Ignore result
    }

    // Test WriteWholeFile with various inputs
    {
        std::string err;
        // Write empty vector
        std::vector<unsigned char> empty_contents;
        bool success1 = tinygltf::WriteWholeFile(&err, dummy_file, empty_contents, nullptr);
        (void)success1;

        // Write non-empty vector using fuzzer data
        std::vector<unsigned char> contents(Data, Data + Size);
        if (!contents.empty()) {
            bool success2 = tinygltf::WriteWholeFile(&err, dummy_file, contents, nullptr);
            (void)success2;
        }
    }

    // Test TinyGLTF member functions
    {
        tinygltf::TinyGLTF loader;

        // Test GetMaxExternalFileSize
        size_t max_size = loader.GetMaxExternalFileSize();
        (void)max_size;

        // Test SetMaxExternalFileSize with fuzzer-derived size
        loader.SetMaxExternalFileSize(Size);

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
        (void)success;

        // Test with null callbacks (should fail)
        tinygltf::FsCallbacks null_callbacks = {};
        bool should_fail = loader.SetFsCallbacks(null_callbacks, &err);
        (void)should_fail;
    }

    // Clean up dummy file
    std::remove(dummy_file.c_str());

    return 0;
}