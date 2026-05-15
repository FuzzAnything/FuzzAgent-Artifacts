// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
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
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

static bool customURIDecode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri + "_custom";
        return true;
    }
    return false;
}

static bool customURIEncode(const std::string &in_uri, const std::string &, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri + "_encoded";
        return true;
    }
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // 1. Test URIDecode
    if (Size > 0) {
        std::string in_uri(reinterpret_cast<const char*>(Data), Size);
        std::string out_uri;
        bool decode_result = tinygltf::URIDecode(in_uri, &out_uri, nullptr);
        (void)decode_result;
    }

    // 2. Test SetURICallbacks
    {
        tinygltf::TinyGLTF loader;
        tinygltf::URICallbacks callbacks;
        callbacks.decode = customURIDecode;
        callbacks.encode = customURIEncode;
        callbacks.user_data = nullptr;
        std::string err;
        bool set_result = loader.SetURICallbacks(callbacks, &err);
        (void)set_result;
    }

    // 3. Test DecodeDataURI
    if (Size > 0) {
        std::vector<unsigned char> out_data;
        std::string mime_type;
        std::string in_string(reinterpret_cast<const char*>(Data), Size);
        bool decode_data_result = tinygltf::DecodeDataURI(&out_data, mime_type, in_string, 0, false);
        (void)decode_data_result;
    }

    // 4. Test LoadASCIIFromString
    if (Size > 0) {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;
        std::string err;
        std::string warn;
        const char* str = reinterpret_cast<const char*>(Data);
        unsigned int length = (Size < 0xFFFFFFFF) ? static_cast<unsigned int>(Size) : 0xFFFFFFFF;
        std::string base_dir = "./";
        bool load_result = loader.LoadASCIIFromString(&model, &err, &warn, str, length, base_dir);
        (void)load_result;
    }

    // 5. Test LoadASCIIFromFile
    {
        const std::string filename = "./dummy_file";
        std::ofstream out_file(filename, std::ios::binary);
        if (out_file.is_open()) {
            out_file.write(reinterpret_cast<const char*>(Data), Size);
            out_file.close();
            
            tinygltf::TinyGLTF loader;
            tinygltf::Model model;
            std::string err;
            std::string warn;
            bool load_result = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
            (void)load_result;
            
            std::remove(filename.c_str());
        }
    }

    // 6. Test LoadBinaryFromMemory
    if (Size > 0) {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;
        std::string err;
        std::string warn;
        const unsigned char* bytes = Data;
        unsigned int length = (Size < 0xFFFFFFFF) ? static_cast<unsigned int>(Size) : 0xFFFFFFFF;
        std::string base_dir = "./";
        bool load_result = loader.LoadBinaryFromMemory(&model, &err, &warn, bytes, length, base_dir);
        (void)load_result;
    }

    return 0;
}