// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::ExpandFilePath at tiny_gltf.cc:1333:13 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
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
#include <fstream>
#include <string>
#include <vector>

static void WriteDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static bool dummyFileExists(const std::string&, void*) {
    return true;
}

static std::string dummyExpandFilePath(const std::string& filepath, void*) {
    return filepath;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out,
                               std::string* err,
                               const std::string& filename,
                               void*) {
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

static bool dummyWriteWholeFile(std::string* err,
                                const std::string& filename,
                                const std::vector<unsigned char>& contents,
                                void*) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        if (err) *err = "Cannot open file for writing";
        return false;
    }
    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    return true;
}

static bool dummyGetFileSize(size_t* filesize, std::string* err,
                             const std::string& filename, void*) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        if (err) *err = "Cannot open file";
        return false;
    }
    *filesize = file.tellg();
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Write data to dummy file for file-based operations
    WriteDummyFile(Data, Size);
    
    // Create string from input data (truncate at null byte if present)
    const char* str_start = reinterpret_cast<const char*>(Data);
    size_t str_len = Size;
    const char* null_pos = static_cast<const char*>(memchr(Data, 0, Size));
    if (null_pos) {
        str_len = null_pos - str_start;
    }
    std::string input_str(str_start, str_len);
    
    // Test ExpandFilePath
    std::string expanded = tinygltf::ExpandFilePath(input_str, nullptr);
    
    // Test FileExists
    bool exists = tinygltf::FileExists("./dummy_file", nullptr);
    
    // Test GetFileSizeInBytes
    size_t file_size = 0;
    std::string err_msg;
    bool got_size = tinygltf::GetFileSizeInBytes(&file_size, &err_msg, "./dummy_file", nullptr);
    
    // Test TinyGLTF functions
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string warn;
    
    // First test with default callbacks
    bool ascii_loaded = loader.LoadASCIIFromFile(&model, &err_msg, &warn, "./dummy_file");
    bool binary_loaded = loader.LoadBinaryFromFile(&model, &err_msg, &warn, "./dummy_file");
    
    // Test SetFsCallbacks
    tinygltf::FsCallbacks callbacks;
    callbacks.FileExists = dummyFileExists;
    callbacks.ExpandFilePath = dummyExpandFilePath;
    callbacks.ReadWholeFile = dummyReadWholeFile;
    callbacks.WriteWholeFile = dummyWriteWholeFile;
    callbacks.GetFileSizeInBytes = dummyGetFileSize;
    callbacks.user_data = nullptr;
    
    bool set_callbacks = loader.SetFsCallbacks(callbacks, &err_msg);
    
    // Test loading with custom callbacks
    if (set_callbacks) {
        tinygltf::Model model2;
        bool ascii_loaded2 = loader.LoadASCIIFromFile(&model2, &err_msg, &warn, "./dummy_file");
        bool binary_loaded2 = loader.LoadBinaryFromFile(&model2, &err_msg, &warn, "./dummy_file");
    }
    
    // Test with different check_sections values
    tinygltf::Model model3;
    bool ascii_loaded3 = loader.LoadASCIIFromFile(&model3, &err_msg, &warn, "./dummy_file", 0);
    bool binary_loaded3 = loader.LoadBinaryFromFile(&model3, &err_msg, &warn, "./dummy_file", 0);
    
    // Test with null error/warning pointers
    tinygltf::Model model4;
    bool ascii_loaded4 = loader.LoadASCIIFromFile(&model4, nullptr, nullptr, "./dummy_file");
    bool binary_loaded4 = loader.LoadBinaryFromFile(&model4, nullptr, nullptr, "./dummy_file");
    
    // Clean up
    std::remove("./dummy_file");
    
    return 0;
}