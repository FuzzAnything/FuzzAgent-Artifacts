// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
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

// Helper to convert fuzzer input to a valid JSON string
static std::string SanitizeJsonInput(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return "{}";
    }
    
    std::string result;
    result.reserve(Size + 10);
    
    // Start with an object
    result += "{";
    
    // Add a key
    result += "\"key\":";
    
    // Try to interpret data as a string value
    result += "\"";
    
    for (size_t i = 0; i < Size; ++i) {
        unsigned char c = Data[i];
        if (c == '"' || c == '\\') {
            result += '\\';
        }
        if (c >= 32 && c < 127) {
            result += c;
        } else {
            // Escape non-printable characters
            result += "\\u";
            char hex[5];
            snprintf(hex, sizeof(hex), "%04x", c);
            result += hex;
        }
    }
    
    result += "\"";
    result += "}";
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string json_str = SanitizeJsonInput(Data, Size);
    
    try {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        // Set flag to exercise extras/extensions code paths
        loader.SetStoreOriginalJSONForExtrasAndExtensions(true);
        
        // LoadASCIIFromString requires 6 arguments: model, err, warn, str, length, base_dir
        // We pass an empty string for base_dir
        bool success = loader.LoadASCIIFromString(&model, &err, &warn, json_str.c_str(), json_str.length(), "");
        
        // The internal parsing functions (ParsePositionalEmitter, ParseUnsignedProperty, etc.)
        // are invoked during the loading process if the JSON structure triggers them.
        // Even if the JSON is invalid or doesn't contain specific glTF fields,
        // the parser will attempt to read fields, exercising the logic.
        
    } catch (const std::exception &) {
        return 0;
    }

    return 0;
}