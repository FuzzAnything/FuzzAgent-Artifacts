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

// Helper to safely convert fuzz input to a valid JSON string
static std::string SanitizeJsonInput(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return "{}";
    }
    std::string raw(reinterpret_cast<const char*>(Data), Size);
    
    bool has_non_ws = false;
    for (size_t i = 0; i < raw.size(); ++i) {
        if (!std::isspace(static_cast<unsigned char>(raw[i]))) {
            has_non_ws = true;
            break;
        }
    }
    if (!has_non_ws) return "{}";
    return raw;
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    try {
        std::string json_str = SanitizeJsonInput(Data, Size);
        
        tinygltf::Model model;
        tinygltf::TinyGLTF ctx;
        std::string err;
        std::string warn;
        std::string base_dir = "./dummy_file";

        // Fuzz the main entry point which triggers all the parsing logic
        // We pass the raw data as a JSON string.
        // The function signature requires 6 arguments: model, err, warn, str, length, base_dir, [check_sections]
        ctx.LoadASCIIFromString(&model, &err, &warn, json_str.c_str(), static_cast<unsigned int>(json_str.size()), base_dir);
        
    } catch (const std::exception &) {
        return 0;
    } catch (...) {
        return 0;
    }

    return 0;
}