// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::detail::JsonParse at tiny_gltf.h:1874:6 in tiny_gltf.h
// tinygltf::ParseStringIntegerProperty at tiny_gltf.h:4096:13 in tiny_gltf.h
// tinygltf::ParseSampler at tiny_gltf.h:5614:13 in tiny_gltf.h
// tinygltf::ParseAccessor at tiny_gltf.h:4770:13 in tiny_gltf.h
// tinygltf::ParseExtensionsProperty at tiny_gltf.h:4220:13 in tiny_gltf.h
// tinygltf::ParseMesh at tiny_gltf.h:5150:13 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <map>

// Define implementation to expose internal functions and types
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

// Helper to convert fuzzing input to a valid JSON string for testing
static std::string CreateJsonString(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return "{}";
    }

    std::string result(reinterpret_cast<const char*>(Data), Size);
    
    // Heuristic to ensure it looks somewhat like JSON
    if (Size > 0 && result[0] != '{' && result[0] != '[' && result[0] != '"' && result[0] != '-' && result[0] != 't' && result[0] != 'f' && result[0] != 'n') {
        std::string escaped;
        for (size_t i = 0; i < Size; ++i) {
            unsigned char c = Data[i];
            if (c == '"') escaped += "\\\"";
            else if (c == '\\') escaped += "\\\\";
            else if (c < 32) {
                char buf[8];
                snprintf(buf, sizeof(buf), "\\u%04x", c);
                escaped += buf;
            } else {
                escaped += c;
            }
        }
        return "{\"data\":\"" + escaped + "\"}";
    }
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    std::string json_str = CreateJsonString(Data, Size);
    std::string err;
    std::string warn;
    
    tinygltf::Model model;
    tinygltf::Mesh mesh;
    tinygltf::Sampler sampler;
    tinygltf::Accessor accessor;
    std::map<std::string, int> string_int_map;
    tinygltf::ExtensionMap ext_map;
    
    // Use tinygltf's internal JsonDocument type
    // This type is defined in tinygltf::detail namespace.
    tinygltf::detail::JsonDocument doc;
    
    // Parse the string into the document using tinygltf's JsonParse
    // This handles the backend abstraction (nlohmann vs custom)
    tinygltf::detail::JsonParse(doc, json_str.c_str(), json_str.length(), false);
    
    // Re-initialize error strings for each call
    err.clear();
    
    // Target 1: ParseStringIntegerProperty
    // Note: This function is static in tinygltf namespace.
    tinygltf::ParseStringIntegerProperty(&string_int_map, &err, doc, "test_property", false, "root");
    
    // Target 2: ParseSampler
    err.clear();
    tinygltf::ParseSampler(&sampler, &err, doc, false);
    
    // Target 3: ParseAccessor
    err.clear();
    tinygltf::ParseAccessor(&accessor, &err, doc, false);
    
    // Target 4: JsonParse is already called above
    
    // Target 5: ParseExtensionsProperty
    err.clear();
    tinygltf::ParseExtensionsProperty(&ext_map, &err, doc);
    
    // Target 6: ParseMesh
    err.clear();
    warn.clear();
    tinygltf::ParseMesh(&mesh, &model, &err, &warn, doc, false, tinygltf::Permissive);
    
    return 0;
}