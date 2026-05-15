// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
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
#include <iostream>
#include <climits>

#include "tiny_gltf.h"

static bool dummy_file_exists(const std::string& filename, void* user_data) {
    return true;
}

static std::string dummy_expand_path(const std::string& path, void* user_data) {
    return path;
}

static bool dummy_read_file(std::vector<unsigned char>* out, std::string* err,
                           const std::string& filename, void* user_data) {
    std::ifstream file(filename, std::ios::binary);
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

static bool dummy_write_file(std::string* err, const std::string& filename,
                            const std::vector<unsigned char>& data,
                            void* user_data) {
    return false;
}

static bool dummy_get_file_size(unsigned long* size, std::string* err,
                               const std::string& filename, void* user_data) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        if (err) *err = "Failed to open file";
        return false;
    }
    *size = file.tellg();
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Test SetMaxExternalFileSize and GetMaxExternalFileSize
    size_t max_size = Size > 8 ? *reinterpret_cast<const size_t*>(Data) : 1024;
    loader.SetMaxExternalFileSize(max_size);
    size_t current_max = loader.GetMaxExternalFileSize();
    
    // Test SetFsCallbacks
    tinygltf::FsCallbacks callbacks;
    callbacks.FileExists = dummy_file_exists;
    callbacks.ExpandFilePath = dummy_expand_path;
    callbacks.ReadWholeFile = dummy_read_file;
    callbacks.WriteWholeFile = dummy_write_file;
    callbacks.GetFileSizeInBytes = dummy_get_file_size;
    callbacks.user_data = nullptr;
    
    bool set_cb_result = loader.SetFsCallbacks(callbacks, &err);
    
    // Write fuzzer data to dummy file for LoadASCIIFromFile
    const std::string dummy_filename = "./dummy_file";
    std::ofstream outfile(dummy_filename, std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(reinterpret_cast<const char*>(Data), Size);
        outfile.close();
    }
    
    // Test LoadASCIIFromFile with various check_sections values
    unsigned int check_sections = Size > 0 ? Data[0] % 4 : 0;
    bool load_result = loader.LoadASCIIFromFile(&model, &err, &warn,
                                                dummy_filename, check_sections);
    
    // Test DecodeDataURI
    std::vector<unsigned char> decoded_data;
    std::string mime_type;
    std::string data_uri_input;
    
    // Create a simple data URI from fuzzer input
    if (Size > 10) {
        data_uri_input = "data:application/octet-stream;base64,";
        // Simple base64-like encoding (not real base64, just for fuzzing)
        for (size_t i = 0; i < std::min(Size, (size_t)100); i++) {
            data_uri_input += static_cast<char>('A' + (Data[i] % 26));
        }
        
        bool decode_result = tinygltf::DecodeDataURI(&decoded_data, mime_type,
                                                    data_uri_input, 0, false);
        
        // Test with size checking
        if (Size > 20) {
            size_t req_bytes = Data[1] % 256;
            bool check_size = (Data[2] % 2) == 1;
            tinygltf::DecodeDataURI(&decoded_data, mime_type,
                                   data_uri_input, req_bytes, check_size);
        }
    }
    
    // Test LoadBinaryFromMemory
    if (Size > 20) {
        tinygltf::Model model2;
        std::string err2;
        std::string warn2;
        
        const unsigned char* binary_data = Data;
        unsigned int data_length = Size > UINT_MAX ? UINT_MAX : static_cast<unsigned int>(Size);
        
        // Test with different base_dir values
        std::string base_dir = Size > 30 ? "./test_dir" : "";
        unsigned int check_sections2 = Size > 1 ? Data[1] % 4 : 0;
        
        bool binary_result = loader.LoadBinaryFromMemory(&model2, &err2, &warn2,
                                                        binary_data, data_length,
                                                        base_dir, check_sections2);
    }
    
    // Clean up dummy file
    remove(dummy_filename.c_str());
    
    return 0;
}