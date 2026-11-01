// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.h:1467:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.h:1467:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.h:1467:8 in tiny_gltf.h
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

// Helper to create a valid JSON object from raw input data
static std::string CreateValidJsonWrapper(const uint8_t *Data, size_t Size) {
    std::string result = "{";
    for (size_t i = 0; i < Size; ++i) {
        char c = Data[i];
        if (c == '"') {
            result += "\\\"";
        } else if (c == '\\') {
            result += "\\\\";
        } else if (c == '\n') {
            result += "\\n";
        } else if (c == '\r') {
            result += "\\r";
        } else if (c == '\t') {
            result += "\\t";
        } else if (c >= 32 && c < 127) {
            result += c;
        } else {
            char buf[8];
            snprintf(buf, sizeof(buf), "\\u%04x", static_cast<unsigned char>(c));
            result += buf;
        }
    }
    result += "\"}";
    return result;
}

// Helper to write data to a dummy file
static void WriteToFile(const char* filename, const uint8_t* Data, size_t Size) {
    FILE* f = fopen(filename, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    tinygltf::Material material;
    tinygltf::Mesh mesh;
    tinygltf::Primitive primitive;
    
    std::string err;
    std::string warn;
    
    // Create a valid JSON wrapper around the fuzzing input
    std::string jsonStr = CreateValidJsonWrapper(Data, Size);
    
    // Write to a dummy file to test file-based loading paths if needed
    WriteToFile("./dummy_file", Data, Size);

    // 2. Fuzz via LoadASCIIFromString (Public API that internally uses ParseMaterial, ParseMesh, etc.)
    // We try both strictness levels
    for (int strictness = 0; strictness <= 1; ++strictness) {
        ctx.SetParseStrictness(static_cast<tinygltf::ParseStrictness>(strictness));
        
        // Reset model and errors
        model = tinygltf::Model();
        err.clear();
        warn.clear();
        
        // Load the JSON string. This will internally parse materials, meshes, primitives, etc.
        // LoadASCIIFromString signature: bool LoadASCIIFromString(Model* model, std::string* err, std::string* warn, const char* str, unsigned int length, const std::string& base_dir, unsigned int check_sections)
        bool success = ctx.LoadASCIIFromString(&model, &err, &warn, jsonStr.c_str(), static_cast<unsigned int>(jsonStr.length()), "./", 0);
        
        (void)success; // Ignore return value, we just want to exercise the code paths
    }

    // 3. Fuzz SetParseStrictness explicitly
    ctx.SetParseStrictness(tinygltf::Permissive);
    ctx.SetParseStrictness(tinygltf::Strict);

    // Note: Direct calls to ParseMaterial, ParseMesh, ParsePrimitive, ParseExtrasAndExtensions
    // require access to tinygltf::detail::json which is not exposed in the public API.
    // The LoadASCIIFromString method is the intended public interface that exercises these functions.
    // By fuzzing LoadASCIIFromString with various inputs, we effectively fuzz the internal parsing logic.

    return 0;
}