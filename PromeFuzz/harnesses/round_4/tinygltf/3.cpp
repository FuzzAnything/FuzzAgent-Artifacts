// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
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
#include <vector>
#include <fstream>

#include "tiny_gltf.h"

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
    
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        if (err) *err = "Failed to open file";
        return false;
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    out->resize(size);
    if (!file.read(reinterpret_cast<char*>(out->data()), size)) {
        if (err) *err = "Failed to read file";
        return false;
    }
    
    return true;
}

static bool dummyWriteWholeFile(std::string* err, const std::string& filepath,
                                const std::vector<unsigned char>& contents,
                                void* user_data) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "Failed to open file for writing";
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    return file.good();
}

static bool dummyGetFileSize(size_t* filesize_out, std::string* err,
                             const std::string& filepath, void* user_data) {
    if (filesize_out == nullptr) return false;
    
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        if (err) *err = "Failed to open file";
        return false;
    }
    
    *filesize_out = file.tellg();
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to determine which function to test
    uint8_t function_selector = Data[0];
    const uint8_t* remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    // Write data to dummy file for file-based operations
    writeDummyFile(remaining_data, remaining_size);
    
    switch (function_selector % 6) {
        case 0: {  // Test LoadASCIIFromFile
            tinygltf::TinyGLTF loader;
            tinygltf::Model model;
            std::string err;
            std::string warn;
            
            // Set up filesystem callbacks
            tinygltf::FsCallbacks callbacks;
            callbacks.FileExists = dummyFileExists;
            callbacks.ExpandFilePath = dummyExpandFilePath;
            callbacks.ReadWholeFile = dummyReadWholeFile;
            callbacks.WriteWholeFile = dummyWriteWholeFile;
            callbacks.GetFileSizeInBytes = dummyGetFileSize;
            callbacks.user_data = nullptr;
            
            loader.SetFsCallbacks(callbacks, &err);
            
            // Set random external file size limit
            if (remaining_size > 0) {
                size_t max_size = static_cast<size_t>(remaining_data[0]) * 1024;
                loader.SetMaxExternalFileSize(max_size);
            }
            
            // Try to load the file
            loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
            break;
        }
        
        case 1: {  // Test SetMaxExternalFileSize
            tinygltf::TinyGLTF loader;
            
            // Use remaining data to determine file size limit
            size_t max_bytes = 0;
            if (remaining_size >= sizeof(size_t)) {
                memcpy(&max_bytes, remaining_data, sizeof(size_t));
            } else if (remaining_size > 0) {
                max_bytes = static_cast<size_t>(remaining_data[0]) * 1024;
            }
            
            loader.SetMaxExternalFileSize(max_bytes);
            break;
        }
        
        case 2: {  // Test DecodeDataURI
            std::vector<unsigned char> out;
            std::string mime_type;
            
            // Create a data URI from the input
            std::string data_uri = "data:application/octet-stream;base64,";
            if (remaining_size > 0) {
                // Simple base64-like encoding for testing
                data_uri.append(reinterpret_cast<const char*>(remaining_data), 
                               std::min(remaining_size, static_cast<size_t>(100)));
            }
            
            // Try different combinations of parameters
            bool checkSize = (remaining_size % 2 == 0);
            size_t reqBytes = 0;
            if (remaining_size > 1) {
                reqBytes = static_cast<size_t>(remaining_data[1]);
            }
            
            tinygltf::DecodeDataURI(&out, mime_type, data_uri, reqBytes, checkSize);
            break;
        }
        
        case 3: {  // Test GetFileSizeInBytes
            size_t filesize = 0;
            std::string err;
            
            // Try with dummy file
            tinygltf::GetFileSizeInBytes(&filesize, &err, "./dummy_file", nullptr);
            
            // Try with random file path
            if (remaining_size > 0) {
                std::string random_path(reinterpret_cast<const char*>(remaining_data),
                                       std::min(remaining_size, static_cast<size_t>(50)));
                tinygltf::GetFileSizeInBytes(&filesize, &err, random_path, nullptr);
            }
            break;
        }
        
        case 4: {  // Test SetFsCallbacks
            tinygltf::TinyGLTF loader;
            std::string err;
            
            // Create callbacks
            tinygltf::FsCallbacks callbacks;
            callbacks.FileExists = dummyFileExists;
            callbacks.ExpandFilePath = dummyExpandFilePath;
            callbacks.ReadWholeFile = dummyReadWholeFile;
            callbacks.WriteWholeFile = dummyWriteWholeFile;
            callbacks.GetFileSizeInBytes = dummyGetFileSize;
            callbacks.user_data = nullptr;
            
            // Test setting callbacks
            loader.SetFsCallbacks(callbacks, &err);
            
            // Test with null callbacks (should fail)
            if (remaining_size % 3 == 0) {
                tinygltf::FsCallbacks null_callbacks = {};
                loader.SetFsCallbacks(null_callbacks, &err);
            }
            break;
        }
        
        case 5: {  // Test ReadWholeFile
            std::vector<unsigned char> out;
            std::string err;
            
            // Try to read dummy file
            tinygltf::ReadWholeFile(&out, &err, "./dummy_file", nullptr);
            
            // Try with random file path
            if (remaining_size > 0) {
                std::string random_path(reinterpret_cast<const char*>(remaining_data),
                                       std::min(remaining_size, static_cast<size_t>(50)));
                tinygltf::ReadWholeFile(&out, &err, random_path, nullptr);
            }
            break;
        }
    }
    
    return 0;
}