// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
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
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

static void WriteDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static bool dummy_uri_encode(const std::string& in, const std::string& base, std::string* out, void* user_data) {
    *out = in;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Use first byte to decide which function to test
    uint8_t selector = Data[0] % 6;
    const uint8_t* remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    // Prepare base directory
    std::string base_dir = ".";
    
    switch (selector) {
        case 0: { // LoadASCIIFromString
            if (remaining_size == 0) break;
            std::string gltf_str(reinterpret_cast<const char*>(remaining_data), remaining_size);
            loader.LoadASCIIFromString(&model, &err, &warn, gltf_str.c_str(), 
                                      static_cast<unsigned int>(gltf_str.size()), 
                                      base_dir);
            break;
        }
        case 1: { // LoadASCIIFromFile
            WriteDummyFile(remaining_data, remaining_size);
            loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
            break;
        }
        case 2: { // DecodeDataURI
            if (remaining_size == 0) break;
            std::string data_uri(reinterpret_cast<const char*>(remaining_data), remaining_size);
            std::vector<unsigned char> out;
            std::string mime_type;
            // Use random values for reqBytes and checkSize from data
            size_t reqBytes = (remaining_size > 1) ? (remaining_data[0] % 1024) : 0;
            bool checkSize = (remaining_size > 2) ? (remaining_data[1] % 2 == 0) : false;
            tinygltf::DecodeDataURI(&out, mime_type, data_uri, reqBytes, checkSize);
            break;
        }
        case 3: { // SetURICallbacks
            tinygltf::URICallbacks callbacks;
            callbacks.encode = dummy_uri_encode;
            callbacks.decode = tinygltf::URIDecode;
            callbacks.user_data = nullptr;
            loader.SetURICallbacks(callbacks, &err);
            break;
        }
        case 4: { // URIDecode
            if (remaining_size == 0) break;
            std::string in_uri(reinterpret_cast<const char*>(remaining_data), remaining_size);
            std::string out_uri;
            tinygltf::URIDecode(in_uri, &out_uri, nullptr);
            break;
        }
        case 5: { // LoadBinaryFromMemory
            if (remaining_size == 0) break;
            loader.LoadBinaryFromMemory(&model, &err, &warn, remaining_data,
                                       static_cast<unsigned int>(remaining_size),
                                       base_dir);
            break;
        }
    }
    
    return 0;
}