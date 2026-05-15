// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.cc:944:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
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
#include "tiny_gltf.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

static std::string dummy_file_path = "./dummy_file";

static void WriteDummyFile(const uint8_t *data, size_t size) {
    std::ofstream file(dummy_file_path, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void CleanupDummyFile() {
    std::remove(dummy_file_path.c_str());
}

static tinygltf::URICallbacks CreateURICallbacks() {
    tinygltf::URICallbacks callbacks;
    callbacks.encode = nullptr;
    callbacks.decode = nullptr;
    callbacks.user_data = nullptr;
    return callbacks;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    // Initialize objects
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Set parse strictness using first byte
    if (Size > 0) {
        uint8_t strictness_byte = Data[0] % 2;
        tinygltf::ParseStrictness strictness = (strictness_byte == 0) ? 
            tinygltf::ParseStrictness::Permissive : 
            tinygltf::ParseStrictness::Strict;
        loader.SetParseStrictness(strictness);
    }
    
    // Set URI callbacks
    tinygltf::URICallbacks uri_callbacks = CreateURICallbacks();
    loader.SetURICallbacks(uri_callbacks, &err);
    
    // Test LoadBinaryFromMemory (requires at least 20 bytes)
    if (Size >= 20) {
        const unsigned char* binary_data = reinterpret_cast<const unsigned char*>(Data);
        unsigned int data_size = static_cast<unsigned int>(Size);
        loader.LoadBinaryFromMemory(&model, &err, &warn, binary_data, data_size, ".");
    }
    
    // Reset model for next test
    model = tinygltf::Model();
    
    // Test LoadASCIIFromString
    if (Size > 0) {
        const char* ascii_data = reinterpret_cast<const char*>(Data);
        unsigned int data_size = static_cast<unsigned int>(Size);
        loader.LoadASCIIFromString(&model, &err, &warn, ascii_data, data_size, ".");
    }
    
    // Reset model for file tests
    model = tinygltf::Model();
    
    // Write data to dummy file for file-based tests
    WriteDummyFile(Data, Size);
    
    // Test LoadBinaryFromFile
    loader.LoadBinaryFromFile(&model, &err, &warn, dummy_file_path);
    
    // Reset model for ASCII file test
    model = tinygltf::Model();
    
    // Test LoadASCIIFromFile
    loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file_path);
    
    // Clean up dummy file
    CleanupDummyFile();
    
    return 0;
}