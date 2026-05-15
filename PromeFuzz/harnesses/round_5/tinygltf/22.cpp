// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
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
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

static void WriteDummyFile(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static bool dummyFileExists(const std::string&, void*) {
    return true;
}

static std::string dummyExpandPath(const std::string& path, void*) {
    return path;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out, std::string* err,
                               const std::string& filepath, void*) {
    if (filepath == "./dummy_file") {
        std::ifstream file(filepath, std::ios::binary);
        if (file) {
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);
            out->resize(size);
            file.read(reinterpret_cast<char*>(out->data()), size);
            return true;
        }
    }
    if (err) *err = "File not found";
    return false;
}

static bool dummyWriteWholeFile(std::string*, const std::string&, const std::vector<unsigned char>&, void*) {
    return true;
}

static bool dummyGetFileSize(size_t* filesize, std::string* err,
                             const std::string& filepath, void*) {
    if (filepath == "./dummy_file") {
        std::ifstream file(filepath, std::ios::binary);
        if (file) {
            file.seekg(0, std::ios::end);
            *filesize = file.tellg();
            return true;
        }
    }
    if (err) *err = "File not found";
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Initialize required objects
    tinygltf::Model model;
    std::string err;
    std::string warn;
    std::string mime_type;
    size_t file_size = 0;
    std::vector<unsigned char> out_data;
    
    // Use first byte to choose different code paths
    uint8_t selector = 0;
    if (Size > 0) {
        selector = Data[0];
        Data++;
        Size--;
    }
    
    // Write dummy file for file-based operations
    WriteDummyFile(Data, Size);
    
    // Test GetFileSizeInBytes
    {
        size_t filesize_out = 0;
        std::string err_msg;
        tinygltf::GetFileSizeInBytes(&filesize_out, &err_msg, "./dummy_file", nullptr);
    }
    
    // Test TinyGLTF::SetMaxExternalFileSize
    {
        tinygltf::TinyGLTF loader;
        size_t max_size = Size;
        if (selector % 2 == 0) {
            max_size = 1024; // Small limit
        }
        loader.SetMaxExternalFileSize(max_size);
    }
    
    // Test DecodeDataURI
    {
        std::string data_uri;
        if (Size > 0) {
            // Create a simple data URI from input
            data_uri = "data:application/octet-stream;base64,";
            // Use a small subset to avoid excessive memory usage
            size_t use_size = Size > 100 ? 100 : Size;
            std::string base64_data(reinterpret_cast<const char*>(Data), use_size);
            data_uri += base64_data;
        } else {
            data_uri = "data:text/plain;base64,SGVsbG8gV29ybGQ="; // "Hello World"
        }
        
        bool checkSize = (selector % 3 == 0);
        size_t reqBytes = (selector % 4 == 0) ? 10 : 0;
        tinygltf::DecodeDataURI(&out_data, mime_type, data_uri, reqBytes, checkSize);
    }
    
    // Test LoadASCIIFromFile
    {
        tinygltf::TinyGLTF loader;
        // Try with different check_sections values
        unsigned int check_sections = tinygltf::REQUIRE_VERSION;
        if (selector % 5 == 0) {
            check_sections = tinygltf::NO_REQUIRE;
        } else if (selector % 5 == 1) {
            check_sections = tinygltf::REQUIRE_ALL;
        }
        
        loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", check_sections);
    }
    
    // Test ReadWholeFile
    {
        std::vector<unsigned char> file_data;
        std::string read_err;
        tinygltf::ReadWholeFile(&file_data, &read_err, "./dummy_file", nullptr);
    }
    
    // Test SetFsCallbacks
    {
        tinygltf::TinyGLTF loader;
        tinygltf::FsCallbacks callbacks;
        
        if (selector % 6 == 0) {
            // Test with null callbacks (should fail)
            callbacks.FileExists = nullptr;
            callbacks.ExpandFilePath = nullptr;
            callbacks.ReadWholeFile = nullptr;
            callbacks.WriteWholeFile = nullptr;
            callbacks.GetFileSizeInBytes = nullptr;
            callbacks.user_data = nullptr;
        } else {
            // Test with valid callbacks
            callbacks.FileExists = dummyFileExists;
            callbacks.ExpandFilePath = dummyExpandPath;
            callbacks.ReadWholeFile = dummyReadWholeFile;
            callbacks.WriteWholeFile = dummyWriteWholeFile;
            callbacks.GetFileSizeInBytes = dummyGetFileSize;
            callbacks.user_data = nullptr;
        }
        
        std::string set_err;
        loader.SetFsCallbacks(callbacks, &set_err);
    }
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}