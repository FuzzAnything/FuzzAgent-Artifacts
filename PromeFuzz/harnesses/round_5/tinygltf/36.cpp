// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
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
#include <cstring>
#include <string>
#include <fstream>
#include <vector>

#include "tiny_gltf.h"

static bool dummy_file_exists(const std::string& filename, void* user_data) {
    (void)user_data;
    return filename == "./dummy_file";
}

static std::string dummy_expand_path(const std::string& path, void* user_data) {
    (void)user_data;
    return path;
}

static bool dummy_read_file(std::vector<unsigned char>* out, std::string* err,
                           const std::string& filename, void* user_data) {
    (void)user_data;
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        if (err) *err = "Cannot open file";
        return false;
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    out->resize(size);
    file.read(reinterpret_cast<char*>(out->data()), size);
    return true;
}

static bool dummy_write_file(std::string* err, const std::string& filename,
                            const std::vector<unsigned char>& data,
                            void* user_data) {
    (void)user_data;
    (void)filename;
    (void)data;
    if (err) *err = "Write not implemented";
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file
    std::ofstream out_file("./dummy_file", std::ios::binary);
    if (out_file.is_open()) {
        out_file.write(reinterpret_cast<const char*>(Data), Size);
        out_file.close();
    }

    // Test GetFileSizeInBytes
    {
        size_t file_size = 0;
        std::string err_msg;
        bool success = tinygltf::GetFileSizeInBytes(&file_size, &err_msg, "./dummy_file", nullptr);
        (void)success;
        (void)file_size;
    }

    // Test TinyGLTF member functions
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Test SetMaxExternalFileSize and GetMaxExternalFileSize
    if (Size > 0) {
        size_t max_size = static_cast<size_t>(Data[0]) * 1024;
        loader.SetMaxExternalFileSize(max_size);
        size_t current_max = loader.GetMaxExternalFileSize();
        (void)current_max;
    }

    // Test SetFsCallbacks
    {
        tinygltf::FsCallbacks callbacks;
        callbacks.FileExists = dummy_file_exists;
        callbacks.ExpandFilePath = dummy_expand_path;
        callbacks.ReadWholeFile = dummy_read_file;
        callbacks.WriteWholeFile = dummy_write_file;
        callbacks.GetFileSizeInBytes = nullptr;
        callbacks.user_data = nullptr;
        
        bool set = loader.SetFsCallbacks(callbacks, &err);
        (void)set;
    }

    // Test LoadASCIIFromFile
    {
        bool load_success = loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
        (void)load_success;
    }

    // Test LoadBinaryFromFile
    {
        bool load_success = loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file");
        (void)load_success;
    }

    // Clean up dummy file
    std::remove("./dummy_file");

    return 0;
}