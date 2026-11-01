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

// Helper to convert fuzzing input to a valid JSON string
static std::string SanitizeJSON(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "{}";
    std::string input(reinterpret_cast<const char*>(Data), Size);
    std::string result;
    bool in_string = false;
    bool escape = false;
    
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        
        if (escape) {
            result += c;
            escape = false;
            continue;
        }
        
        if (c == '\\' && in_string) {
            result += c;
            escape = true;
            continue;
        }
        
        if (c == '"') {
            in_string = !in_string;
            result += c;
            continue;
        }
        
        if (in_string) {
            if (c < 32) {
                result += "\\u0000";
            } else {
                result += c;
            }
            continue;
        }
        
        if (c >= 32 && c <= 126) {
            result += c;
        } else if (c == '\n' || c == '\r' || c == '\t') {
            result += c;
        } else {
            continue;
        }
    }
    
    if (result.empty() || result.find('{') == std::string::npos) {
        return "{}";
    }
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string json_str = SanitizeJSON(Data, Size);
    
    try {
        // We cannot directly construct tinygltf::detail::json without the backend header.
        // Instead, we use the Model loader to parse the JSON string.
        // The Model loader internally uses the JSON backend and populates the Model.
        // However, the target functions (ParseUnsignedProperty, etc.) operate on the raw JSON object.
        // To fuzz these specific functions, we need to access the internal JSON representation.
        // Since we can't include nlohmann/json.hpp, we rely on tinygltf's internal mechanisms.
        // If tinygltf is built with nlohmann, the internal type is nlohmann::json.
        // We can try to parse the JSON string using the Model loader and then access the internal JSON if possible.
        // But the Model loader doesn't expose the raw JSON object.
        
        // Alternative: Use the LoadASCIIFromString function to parse the JSON.
        // This function returns a Model, but we can't easily access the internal JSON.
        // However, we can try to create a dummy Model and then manually construct the JSON object
        // if we know the backend. Since we don't, we'll try to use the Model loader to trigger the parsing logic.
        
        // Since we can't directly access the internal JSON, we'll use the Model loader to parse the JSON string.
        // This will exercise the parsing logic, including the target functions, indirectly.
        tinygltf::Model model;
        std::string err;
        std::string warn;
        
        // Try to load the JSON string as a glTF model.
        // This will trigger the parsing logic, including the target functions.
        tinygltf::TinyGLTF ctx;
        ctx.LoadASCIIFromString(&model, &err, &warn, json_str.c_str(), json_str.size(), nullptr);
        
        // Even if loading fails, the parsing logic will have been exercised.
        // We can also try to parse the JSON string as a material directly.
        // But we need the internal JSON object for that.
        
        // Since we can't directly access the internal JSON, we'll rely on the Model loader to exercise the parsing logic.
        // This is the best we can do without including the backend header.
        
    } catch (...) {
        return 0;
    }

    return 0;
}