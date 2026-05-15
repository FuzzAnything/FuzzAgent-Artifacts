// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
// tinygltf::ReadWholeFile at tiny_gltf.cc:1492:6 in tiny_gltf.h
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
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

// Dummy filesystem callbacks for fuzzing
static bool dummy_FileExists(const std::string& filename, void* user_data) {
    return true;
}

static std::string dummy_ExpandFilePath(const std::string& filename, void* user_data) {
    return filename;
}

static bool dummy_ReadWholeFile(std::vector<unsigned char>* out, std::string* err,
                                const std::string& filepath, void* user_data) {
    std::ifstream file(filepath, std::ios::binary);
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

static bool dummy_WriteWholeFile(std::string* err, const std::string& filepath,
                                 const std::vector<unsigned char>& contents,
                                 void* user_data) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file) {
        if (err) *err = "Cannot open file for writing";
        return false;
    }
    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    return true;
}

static bool dummy_GetFileSizeInBytes(size_t* filesize_out, std::string* err,
                                     const std::string& filepath, void* user_data) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file) {
        if (err) *err = "Cannot open file";
        return false;
    }
    *filesize_out = file.tellg();
    return true;
}

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static void remove_dummy_file() {
    std::remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Write fuzzer data to dummy file for file-based operations
    write_dummy_file(Data, Size);

    // Test 1: SetFsCallbacks
    {
        tinygltf::TinyGLTF loader;
        tinygltf::FsCallbacks callbacks;
        std::string err;
        
        // Test with valid callbacks
        callbacks.FileExists = dummy_FileExists;
        callbacks.ExpandFilePath = dummy_ExpandFilePath;
        callbacks.ReadWholeFile = dummy_ReadWholeFile;
        callbacks.WriteWholeFile = dummy_WriteWholeFile;
        callbacks.GetFileSizeInBytes = dummy_GetFileSizeInBytes;
        callbacks.user_data = nullptr;
        
        bool result = loader.SetFsCallbacks(callbacks, &err);
        (void)result; // Use result to avoid unused variable warning
        
        // Test with null callbacks (should fail)
        tinygltf::FsCallbacks null_callbacks = {};
        loader.SetFsCallbacks(null_callbacks, nullptr);
    }

    // Test 2: LoadASCIIFromFile (requires callbacks to be set)
    {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;
        std::string err, warn;
        
        // Set up callbacks first
        tinygltf::FsCallbacks callbacks;
        callbacks.FileExists = dummy_FileExists;
        callbacks.ExpandFilePath = dummy_ExpandFilePath;
        callbacks.ReadWholeFile = dummy_ReadWholeFile;
        callbacks.WriteWholeFile = dummy_WriteWholeFile;
        callbacks.GetFileSizeInBytes = dummy_GetFileSizeInBytes;
        callbacks.user_data = nullptr;
        
        loader.SetFsCallbacks(callbacks);
        
        // Try to load from dummy file
        bool result = loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
        (void)result;
        
        // Test with different check_sections values
        if (Size > 0) {
            unsigned int check_sections = Data[0] % 4; // Random value 0-3
            loader.LoadASCIIFromFile(&model, nullptr, nullptr, "./dummy_file", check_sections);
        }
    }

    // Test 3: GetFileSizeInBytes
    {
        size_t filesize = 0;
        std::string err;
        
        // Test with dummy file
        bool result = tinygltf::GetFileSizeInBytes(&filesize, &err, "./dummy_file", nullptr);
        (void)result;
        
        // Test with null error pointer
        tinygltf::GetFileSizeInBytes(&filesize, nullptr, "./dummy_file", nullptr);
    }

    // Test 4: DecodeDataURI
    {
        std::vector<unsigned char> out;
        std::string mime_type;
        
        // Create a data URI from fuzzer input
        std::string data_uri = "data:application/octet-stream;base64,";
        // Simple base64 encoding of first part of input (max 100 bytes to avoid huge strings)
        size_t encode_size = Size > 100 ? 100 : Size;
        std::string base64_data;
        for (size_t i = 0; i < encode_size; i++) {
            char c = Data[i];
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
                (c >= '0' && c <= '9') || c == '+' || c == '/' || c == '=') {
                base64_data.push_back(c);
            } else {
                base64_data.push_back('A'); // Replace invalid chars
            }
        }
        data_uri += base64_data;
        
        // Test with various reqBytes and checkSize values
        bool result = tinygltf::DecodeDataURI(&out, mime_type, data_uri, 0, false);
        (void)result;
        
        if (Size > 1) {
            size_t reqBytes = Data[0] * 256 + Data[1];
            bool checkSize = (Data[0] % 2) == 0;
            tinygltf::DecodeDataURI(&out, mime_type, data_uri, reqBytes, checkSize);
        }
        
        // Test with empty string
        std::string empty;
        tinygltf::DecodeDataURI(&out, mime_type, empty, 0, false);
        
        // Test with invalid data URI
        tinygltf::DecodeDataURI(&out, mime_type, "invalid_uri", 0, false);
    }

    // Test 5: SetMaxExternalFileSize
    {
        tinygltf::TinyGLTF loader;
        
        // Set various file size limits based on fuzzer input
        if (Size > 0) {
            size_t max_size = 0;
            for (size_t i = 0; i < (Size > 8 ? 8 : Size); i++) {
                max_size = (max_size << 8) | Data[i];
            }
            loader.SetMaxExternalFileSize(max_size);
        }
        
        // Test with zero size
        loader.SetMaxExternalFileSize(0);
        
        // Test with large size
        loader.SetMaxExternalFileSize(1024 * 1024 * 1024); // 1GB
    }

    // Test 6: ReadWholeFile
    {
        std::vector<unsigned char> out;
        std::string err;
        
        // Test with dummy file
        bool result = tinygltf::ReadWholeFile(&out, &err, "./dummy_file", nullptr);
        (void)result;
        
        // Test with null error pointer
        tinygltf::ReadWholeFile(&out, nullptr, "./dummy_file", nullptr);
        
        // Test with non-existent file
        tinygltf::ReadWholeFile(&out, &err, "./non_existent_file.bin", nullptr);
    }

    // Clean up dummy file
    remove_dummy_file();
    
    return 0;
}