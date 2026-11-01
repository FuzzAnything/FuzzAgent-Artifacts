// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
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

// Fuzz driver for tinygltf parsing functions

static std::string CreateJSONString(const uint8_t *data, size_t size) {
    std::string json_str;
    json_str.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        unsigned char c = data[i];
        if (c >= 32 && c < 127) {
            json_str += static_cast<char>(c);
        } else {
            switch (c % 10) {
                case 0: json_str += '{'; break;
                case 1: json_str += '}'; break;
                case 2: json_str += '['; break;
                case 3: json_str += ']'; break;
                case 4: json_str += '"'; break;
                case 5: json_str += ':'; break;
                case 6: json_str += ','; break;
                case 7: json_str += 't'; break;
                case 8: json_str += 'f'; break;
                case 9: json_str += 'n'; break;
            }
        }
    }
    return json_str;
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string json_str = CreateJSONString(Data, Size);
    
    // Create a minimal glTF structure to ensure the parser runs
    std::string full_json = "{\"asset\":{\"version\":\"2.0\"}," + json_str + "}";
    
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Use LoadASCIIFromString which is the correct API for tinygltf
    tinygltf::TinyGLTF ctx;
    ctx.LoadASCIIFromString(&model, &err, &warn, full_json.c_str(), full_json.size(), "");
    
    // Additional attempt: Try to parse the raw input as a potential extension or extras field
    // by wrapping it in a valid glTF structure.
    std::string valid_gltf = "{\"asset\":{\"version\":\"2.0\"},\"extras\":" + json_str + "}";
    ctx.LoadASCIIFromString(&model, &err, &warn, valid_gltf.c_str(), valid_gltf.size(), "");

    return 0;
}