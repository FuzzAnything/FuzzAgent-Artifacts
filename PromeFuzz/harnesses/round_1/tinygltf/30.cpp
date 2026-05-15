// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.cc:944:16 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>

#include "tiny_gltf.h"

static bool dummy_uri_decode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

static bool dummy_uri_encode(const std::string &in_uri, const std::string &base_dir, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Use first byte to select which function to test
    uint8_t selector = Data[0] % 6;
    const uint8_t* fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    switch (selector) {
        case 0: { // LoadASCIIFromFile
            std::ofstream outfile("./dummy_file", std::ios::binary);
            if (outfile.is_open()) {
                outfile.write(reinterpret_cast<const char*>(fuzz_data), fuzz_size);
                outfile.close();
                loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
            }
            break;
        }
        case 1: { // LoadBinaryFromMemory
            if (fuzz_size > 20) { // Minimum required by the function
                loader.LoadBinaryFromMemory(&model, &err, &warn, fuzz_data, fuzz_size, "");
            }
            break;
        }
        case 2: { // LoadBinaryFromFile
            std::ofstream outfile("./dummy_file", std::ios::binary);
            if (outfile.is_open()) {
                outfile.write(reinterpret_cast<const char*>(fuzz_data), fuzz_size);
                outfile.close();
                loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file");
            }
            break;
        }
        case 3: { // SetURICallbacks
            tinygltf::URICallbacks callbacks;
            callbacks.encode = dummy_uri_encode;
            callbacks.decode = dummy_uri_decode;
            callbacks.user_data = nullptr;
            loader.SetURICallbacks(callbacks, &err);
            break;
        }
        case 4: { // LoadASCIIFromString
            if (fuzz_size > 0) {
                const char* str_data = reinterpret_cast<const char*>(fuzz_data);
                loader.LoadASCIIFromString(&model, &err, &warn, str_data, fuzz_size, "");
            }
            break;
        }
        case 5: { // SetParseStrictness
            tinygltf::ParseStrictness strictness = (fuzz_size % 2 == 0) ? 
                tinygltf::ParseStrictness::Permissive : 
                tinygltf::ParseStrictness::Strict;
            loader.SetParseStrictness(strictness);
            break;
        }
    }

    return 0;
}