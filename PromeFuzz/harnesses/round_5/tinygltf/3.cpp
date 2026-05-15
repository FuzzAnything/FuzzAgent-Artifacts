// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.cc:944:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.cc:944:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
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

static void write_data_to_file(const char* filename, const uint8_t* data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static bool dummy_decode(const std::string& uri, std::string* out, void* user_data) {
    if (out) {
        *out = uri;
    }
    return true;
}

static bool dummy_encode(const std::string& uri, const std::string& base, std::string* out, void* user_data) {
    if (out) {
        *out = uri;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Use first byte to determine which function to test
    uint8_t selector = Data[0];
    const uint8_t* remaining_data = Data + 1;
    size_t remaining_size = Size - 1;

    // Set up URI callbacks
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = dummy_encode;
    uri_callbacks.decode = dummy_decode;
    uri_callbacks.user_data = nullptr;

    // Set parse strictness randomly
    if (selector & 0x01) {
        loader.SetParseStrictness(tinygltf::ParseStrictness::Strict);
    } else {
        loader.SetParseStrictness(tinygltf::ParseStrictness::Permissive);
    }

    // Try to set URI callbacks
    loader.SetURICallbacks(uri_callbacks, &err);

    // Test different load functions based on selector
    switch (selector % 6) {
        case 0: {
            // LoadASCIIFromString
            std::string base_dir = ".";
            const char* str = reinterpret_cast<const char*>(remaining_data);
            unsigned int length = static_cast<unsigned int>(remaining_size);
            if (length > 0) {
                loader.LoadASCIIFromString(&model, &err, &warn, str, length, base_dir);
            }
            break;
        }
        case 1: {
            // LoadASCIIFromFile
            write_data_to_file("./dummy_file", remaining_data, remaining_size);
            loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
            std::remove("./dummy_file");
            break;
        }
        case 2: {
            // LoadBinaryFromMemory
            std::string base_dir = ".";
            unsigned int length = static_cast<unsigned int>(remaining_size);
            if (length > 0) {
                loader.LoadBinaryFromMemory(&model, &err, &warn, remaining_data, length, base_dir);
            }
            break;
        }
        case 3: {
            // LoadBinaryFromFile
            write_data_to_file("./dummy_file", remaining_data, remaining_size);
            loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file");
            std::remove("./dummy_file");
            break;
        }
        case 4: {
            // Test LoadASCIIFromString with empty base_dir
            const char* str = reinterpret_cast<const char*>(remaining_data);
            unsigned int length = static_cast<unsigned int>(remaining_size);
            if (length > 0) {
                loader.LoadASCIIFromString(&model, &err, &warn, str, length, "");
            }
            break;
        }
        case 5: {
            // Test LoadBinaryFromMemory with empty base_dir
            unsigned int length = static_cast<unsigned int>(remaining_size);
            if (length > 0) {
                loader.LoadBinaryFromMemory(&model, &err, &warn, remaining_data, length, "");
            }
            break;
        }
    }

    // Clean up any temporary files
    std::remove("./dummy_file");

    return 0;
}