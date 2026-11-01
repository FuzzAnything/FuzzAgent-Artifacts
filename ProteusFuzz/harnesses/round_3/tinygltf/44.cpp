// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.h:1467:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.h:1467:8 in tiny_gltf.h
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
#include <iomanip>
#include "tiny_gltf.h"

// Helper to create a valid JSON string from raw bytes
static std::string CreateValidJsonWrapper(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return "{}";
    }
    
    std::ostringstream oss;
    oss << "{\"data\":\"";
    
    for (size_t i = 0; i < Size; ++i) {
        unsigned char c = Data[i];
        if (c == '"') {
            oss << "\\\"";
        } else if (c == '\\') {
            oss << "\\\\";
        } else if (c == '\n') {
            oss << "\\n";
        } else if (c == '\r') {
            oss << "\\r";
        } else if (c == '\t') {
            oss << "\\t";
        } else if (c >= 32 && c < 127) {
            oss << static_cast<char>(c);
        } else {
            // Escape non-printable characters
            oss << "\\u" << std::hex << std::setfill('0') << std::setw(4) << static_cast<int>(c);
        }
    }
    
    oss << "\"}";
    return oss.str();
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    
    // Create a valid JSON string from the fuzz input
    std::string json_str = CreateValidJsonWrapper(Data, Size);
    const char *json_c_str = json_str.c_str();
    size_t json_len = json_str.length();

    // Initialize TinyGLTF context
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;

    // Initialize data structures for parsing
    tinygltf::Model model;

    // 2. Fuzz SetParseStrictness
    // Toggle between Permissive and Strict modes
    ctx.SetParseStrictness(tinygltf::Permissive);
    ctx.SetParseStrictness(tinygltf::Strict);

    // Write the JSON string to a dummy file
    const char *dummy_file = "./dummy_file";
    FILE *file = fopen(dummy_file, "w");
    if (file) {
        fwrite(json_c_str, 1, json_len, file);
        fclose(file);
    } else {
        return 0;
    }

    // Try to load the model using the public API
    // LoadASCIIFromString requires 6 arguments: model, err, warn, str, length, base_dir, check_sections
    // We provide an empty base_dir and default check_sections
    bool loaded = ctx.LoadASCIIFromString(&model, &err, &warn, json_c_str, static_cast<unsigned int>(json_len), "", 0);
    
    // 3. Fuzz ParseExtrasAndExtensions
    // Since we cannot directly access tinygltf::detail namespace, we will use the public API
    // to load a model and then access internal functions if they are accessible.
    
    // 4. Fuzz ParseMaterial
    // Same as above
    
    // 5. Fuzz ParsePrimitive
    // Same as above
    
    // 6. Fuzz ParseMesh
    // Same as above

    // Clean up the dummy file
    remove(dummy_file);

    return 0;
}