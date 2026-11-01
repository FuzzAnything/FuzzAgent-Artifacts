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
#include <map>
#include <fstream>
#include <iomanip>

// Include tinygltf header
#include "tiny_gltf.h"

// Helper to generate a valid JSON string from fuzz input
static std::string GenerateJSON(const uint8_t *Data, size_t Size) {
    std::string json_str = "{";
    
    if (Size < 2) {
        return json_str + "}";
    }

    std::vector<std::string> keys = {
        "test_uint", "test_int", "test_double", 
        "test_int_array", "test_double_array", "test_map"
    };
    
    for (size_t i = 0; i < Size; ++i) {
        uint8_t b = Data[i];
        
        if (i > 0) json_str += ",";
        
        std::string key = keys[b % keys.size()];
        json_str += "\"" + key + "\":";
        
        switch (b % 6) {
            case 0: // Unsigned int
                json_str += std::to_string(static_cast<unsigned int>(b) + i);
                break;
            case 1: // Signed int
                json_str += std::to_string(static_cast<int>(b) - 128);
                break;
            case 2: // Double
                json_str += std::to_string(static_cast<double>(b) / 10.0);
                break;
            case 3: // Integer Array
                json_str += "[";
                for (int k = 0; k < (b % 5); ++k) {
                    if (k > 0) json_str += ",";
                    json_str += std::to_string(k);
                }
                json_str += "]";
                break;
            case 4: // Number Array
                json_str += "[";
                for (int k = 0; k < (b % 5); ++k) {
                    if (k > 0) json_str += ",";
                    json_str += std::to_string(k * 1.5);
                }
                json_str += "]";
                break;
            case 5: // Object (for map)
                json_str += "{\"key1\":1.1,\"key2\":2.2}";
                break;
        }
    }
    
    json_str += "}";
    return json_str;
}

// Helper to write data to a dummy file
static void WriteDummyFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

// Wrapper to safely parse JSON using tinygltf's loader
static void FuzzWithLoader(const std::string &json_str) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    // Load the JSON string. 
    // The function signature requires: str, length, base_dir, check_sections
    const char* str = json_str.c_str();
    unsigned int length = static_cast<unsigned int>(json_str.length());
    const std::string base_dir = "";
    unsigned int check_sections = tinygltf::REQUIRE_VERSION;

    bool ret = loader.LoadASCIIFromString(&model, &err, &warn, str, length, base_dir, check_sections);
    (void)ret; // We don't care if it succeeds, just that it runs
    (void)err;
    (void)warn;
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    WriteDummyFile(Data, Size);
    
    std::string json_str = GenerateJSON(Data, Size);
    
    // Strategy: Use the loader to exercise the parsing logic.
    // We construct a minimal valid glTF 2.0 JSON and inject fuzz data.
    
    std::string valid_gltf = "{";
    valid_gltf += "\"asset\":{\"version\":\"2.0\"},";
    valid_gltf += "\"buffers\":[],";
    valid_gltf += "\"bufferViews\":[],";
    valid_gltf += "\"accessors\":[],";
    valid_gltf += "\"meshes\":[],";
    valid_gltf += "\"nodes\":[],";
    valid_gltf += "\"scene\":0,";
    valid_gltf += "\"scenes\":[{}]";
    
    // Append the fuzz data as a string in "extras" to avoid syntax errors
    valid_gltf += ",\"extras\":{\"fuzz\":\"";
    for (size_t i = 0; i < Size; ++i) {
        unsigned char c = Data[i];
        if (c == '"' || c == '\\') valid_gltf += "\\";
        valid_gltf += static_cast<char>(c);
    }
    valid_gltf += "\"}";
    valid_gltf += "}";
    
    FuzzWithLoader(valid_gltf);
    
    // Also try to load the raw generated JSON (might be invalid, but tests error handling)
    FuzzWithLoader(json_str);
    
    // Try to construct a JSON with specific fields that trigger the parsers.
    // e.g. "componentType" for ParseUnsignedProperty
    std::string accessor_json = "{";
    accessor_json += "\"asset\":{\"version\":\"2.0\"},";
    accessor_json += "\"buffers\":[],";
    accessor_json += "\"bufferViews\":[],";
    accessor_json += "\"accessors\":[";
    // Inject fuzz data into accessor properties
    // componentType (unsigned), count (unsigned), type (string), byteOffset (unsigned), min/max (array)
    accessor_json += "{";
    accessor_json += "\"componentType\":";
    // Use first byte as componentType (might be invalid, tests error handling)
    accessor_json += std::to_string(Data[0]);
    accessor_json += ",\"count\":";
    accessor_json += std::to_string(Size);
    accessor_json += ",\"type\":\"SCALAR\"";
    accessor_json += "}]";
    accessor_json += ",\"meshes\":[],\"nodes\":[],\"scene\":0,\"scenes\":[{}]";
    accessor_json += "}";
    
    FuzzWithLoader(accessor_json);
    
    // Try to trigger ParseIntegerArrayProperty (e.g. in accessor min/max)
    // "min" expects an array, we provide a scalar to test error handling
    std::string min_json = "{";
    min_json += "\"asset\":{\"version\":\"2.0\"},";
    min_json += "\"buffers\":[],";
    min_json += "\"bufferViews\":[],";
    min_json += "\"accessors\":[{\"componentType\":5120,\"count\":1,\"type\":\"SCALAR\",\"min\":123}]"; 
    min_json += ",\"meshes\":[],\"nodes\":[],\"scene\":0,\"scenes\":[{}]";
    min_json += "}";
    FuzzWithLoader(min_json);
    
    // Try to trigger ParseNumberProperty with invalid types
    std::string num_json = "{";
    num_json += "\"asset\":{\"version\":\"2.0\"},";
    num_json += "\"buffers\":[],";
    num_json += "\"bufferViews\":[],";
    num_json += "\"accessors\":[{\"componentType\":5120,\"count\":1,\"type\":\"SCALAR\",\"min\":[1.0, 2.0, 3.0]}]"; 
    // Add a field that expects a number but gets a string
    num_json += ",\"meshes\":[{\"name\":\"test\"}],\"nodes\":[],\"scene\":0,\"scenes\":[{}]";
    num_json += "}";
    FuzzWithLoader(num_json);

    return 0;
}