// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
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
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>

static std::string dummy_file_path = "./dummy_file";

static void write_dummy_file(const uint8_t *data, size_t size) {
    std::ofstream file(dummy_file_path, std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static bool dummy_decode(const std::string &uri, std::string *out, void *user_data) {
    if (out) {
        *out = uri;
    }
    return true;
}

static tinygltf::URICallbacks create_uri_callbacks() {
    tinygltf::URICallbacks callbacks;
    callbacks.encode = nullptr;
    callbacks.decode = dummy_decode;
    callbacks.user_data = nullptr;
    return callbacks;
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Initialize objects
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Set parse strictness based on input
    if (Size > 0) {
        loader.SetParseStrictness(Data[0] % 2 ? tinygltf::Permissive : tinygltf::Strict);
    }
    
    // Set URI callbacks
    tinygltf::URICallbacks uri_callbacks = create_uri_callbacks();
    loader.SetURICallbacks(uri_callbacks, &err);
    
    // Test LoadBinaryFromMemory
    if (Size > 20) {
        loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", tinygltf::REQUIRE_VERSION);
    }
    
    // Test LoadASCIIFromString
    if (Size > 0) {
        const char* str = reinterpret_cast<const char*>(Data);
        unsigned int length = Size;
        if (Size > 1024) {
            length = 1024;
        }
        loader.LoadASCIIFromString(&model, &err, &warn, str, length, "");
    }
    
    // Test file-based loading functions
    write_dummy_file(Data, Size);
    
    // Test LoadASCIIFromFile
    loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file_path, tinygltf::REQUIRE_VERSION);
    
    // Test LoadBinaryFromFile
    loader.LoadBinaryFromFile(&model, &err, &warn, dummy_file_path, tinygltf::REQUIRE_VERSION);
    
    // Clean up dummy file
    std::remove(dummy_file_path.c_str());
    
    return 0;
}