// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
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

// Helper to safely convert fuzzer input to a valid JSON string
static std::string SanitizeJsonInput(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return "{}";
    }
    
    std::string result;
    result.reserve(Size);
    
    for (size_t i = 0; i < Size; ++i) {
        uint8_t c = Data[i];
        if (c == 0 || (c < 32 && c != 9 && c != 10 && c != 13)) {
            result += ' ';
        } else {
            result += static_cast<char>(c);
        }
    }
    
    if (result.empty()) return "{}";
    if (result[0] != '{' && result[0] != '[') {
        return "{" + result + "}";
    }
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    try {
        std::string json_str = SanitizeJsonInput(Data, Size);
        
        tinygltf::Model model;
        tinygltf::TinyGLTF ctx;
        std::string err;
        std::string warn;
        
        // Target 1: tinygltf::Parameter::TextureScale
        {
            tinygltf::Parameter param;
            param.json_double_value["scale"] = static_cast<double>(Data[0]);
            double scale = param.TextureScale();
            (void)scale;
        }

        // Target 3: tinygltf::Parameter::ColorFactor
        // Fixed: Ensure number_array has enough elements before calling ColorFactor()
        // ColorFactor() accesses indices 0, 1, 2, 3. We need at least 3 elements to avoid out-of-bounds.
        {
            tinygltf::Parameter param;
            param.number_array.clear();
            // Ensure we have at least 3 elements to satisfy the internal logic of ColorFactor
            // which accesses index 0, 1, 2. It handles index 3 safely if missing.
            size_t count = (Size >= 3) ? 3 : Size;
            for (size_t i = 0; i < count; ++i) {
                param.number_array.push_back(static_cast<double>(Data[i]));
            }
            // If the array is empty, ColorFactor might still access invalid memory depending on implementation.
            // The spec says it defaults alpha to 1.0 if fewer than 4 components.
            // However, accessing index 0, 1, 2 requires at least 3 elements.
            // If count < 3, we skip the call to prevent ASAN error.
            if (count >= 3) {
                tinygltf::ColorValue color = param.ColorFactor();
                (void)color;
            }
        }

        // Target 5: tinygltf::Parameter::TextureStrength
        {
            tinygltf::Parameter param;
            param.json_double_value["strength"] = static_cast<double>(Data[0]);
            double strength = param.TextureStrength();
            (void)strength;
        }

        // Target 6: tinygltf::Parameter::Factor
        {
            tinygltf::Parameter param;
            param.number_value = static_cast<double>(Data[0]);
            double factor = param.Factor();
            (void)factor;
        }

        // Additional test: Try to parse the JSON string using the loader to ensure it handles various inputs.
        // We write the JSON string to a dummy file and try to load it.
        const char* dummy_file = "./dummy_file";
        FILE* f = fopen(dummy_file, "w");
        if (f) {
            fwrite(json_str.c_str(), 1, json_str.size(), f);
            fclose(f);
            
            // LoadASCIIFromString requires 6 arguments: model, err, warn, str, length, base_dir, check_sections
            // We provide the required arguments, using empty base_dir and default check_sections
            ctx.LoadASCIIFromString(&model, &err, &warn, json_str.c_str(), json_str.size(), "", tinygltf::REQUIRE_VERSION);
            // If loading fails, it's okay; we just want to test the error handling.
        }

        // Clean up the dummy file
        std::remove(dummy_file);

    } catch (...) {
        return 0;
    }

    return 0;
}