// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
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

// Helper to create a valid JSON string from fuzz input
static std::string CreateValidJson(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "{}";
    
    std::string result;
    result.reserve(Size * 2);
    result.push_back('{');
    
    bool first = true;
    for (size_t i = 0; i < Size; ++i) {
        unsigned char c = Data[i];
        if (c >= 32 && c < 127 && c != '"' && c != '\\' && c != ':' && c != ',' && c != '{' && c != '}') {
            if (!first) result.push_back(',');
            result.push_back('"');
            result.push_back(c);
            result.push_back('"');
            result.push_back(':');
            result.push_back('"');
            result.push_back(c);
            result.push_back('"');
            first = false;
        }
    }
    
    result.push_back('}');
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a valid JSON string
    std::string json_str = CreateValidJson(Data, Size);
    
    // Use tinygltf::Model and tinygltf::Loader to parse the file
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    // Try to load the model
    // This will trigger ParseScene, ParseCamera, etc. internally
    loader.LoadASCIIFromString(&model, &err, &warn, json_str.c_str(), json_str.size(), "./");

    return 0;
}