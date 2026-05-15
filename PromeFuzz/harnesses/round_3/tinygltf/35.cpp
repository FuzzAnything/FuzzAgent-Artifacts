// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.cc:944:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.cc:944:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
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

#include "tiny_gltf.h"

static std::string dummy_file = "./dummy_file";

static void write_dummy_file(const uint8_t *data, size_t size) {
    std::ofstream file(dummy_file, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static tinygltf::URICallbacks create_uri_callbacks() {
    tinygltf::URICallbacks callbacks;
    callbacks.encode = nullptr;
    callbacks.decode = [](const std::string &uri, std::string *out, void *user_data) -> bool {
        if (out) {
            *out = uri;
        }
        return true;
    };
    callbacks.user_data = nullptr;
    return callbacks;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Initialize required objects
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Try different parse strictness modes
    if (Size > 0) {
        if (Data[0] % 2 == 0) {
            loader.SetParseStrictness(tinygltf::ParseStrictness::Permissive);
        } else {
            loader.SetParseStrictness(tinygltf::ParseStrictness::Strict);
        }
    }
    
    // Try setting URI callbacks
    auto uri_callbacks = create_uri_callbacks();
    loader.SetURICallbacks(uri_callbacks, &err);
    
    // Test LoadBinaryFromMemory (requires at least 20 bytes)
    if (Size >= 20) {
        loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "");
    }
    
    // Test LoadASCIIFromString
    if (Size > 0) {
        const char* str_data = reinterpret_cast<const char*>(Data);
        loader.LoadASCIIFromString(&model, &err, &warn, str_data, Size, "");
    }
    
    // Test file-based loading functions
    write_dummy_file(Data, Size);
    
    // Test LoadASCIIFromFile
    loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file);
    
    // Test LoadBinaryFromFile
    loader.LoadBinaryFromFile(&model, &err, &warn, dummy_file);
    
    // Clean up dummy file
    std::remove(dummy_file.c_str());
    
    return 0;
}