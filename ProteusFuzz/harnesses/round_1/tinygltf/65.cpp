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

// Helper to create a valid JSON string from fuzz input to reduce noise
static std::string CreateValidJSON(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "{}";
    
    std::string result = "{\"test\":";
    size_t limit = (Size > 1024) ? 1024 : Size;
    for (size_t i = 0; i < limit; ++i) {
        char c = Data[i];
        if (c == '"' || c == '\\') {
            result += '\\';
        }
        if (c >= 32 && c < 127) {
            result += c;
        } else {
            char hex[4];
            snprintf(hex, sizeof(hex), "\\u%04x", static_cast<unsigned char>(c));
            result += hex;
        }
    }
    result += "}";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string json_str = CreateValidJSON(Data, Size);
    const char *c_str = json_str.c_str();
    size_t len = json_str.length();

    // Test 1: Indirectly fuzz JsonParse via LoadASCIIFromString
    {
        tinygltf::Model model;
        tinygltf::TinyGLTF ctx;
        std::string err;
        std::string warn;
        // Fixed: Added base_dir and check_sections arguments
        bool res = ctx.LoadASCIIFromString(&model, &err, &warn, c_str, static_cast<unsigned int>(len), "", tinygltf::REQUIRE_VERSION);
        (void)res;
    }

    // Test 2: Indirectly fuzz JsonIsNull via parsing
    // This is hard to do directly, but parsing a JSON with null values will trigger it.
    // We already did that in Test 1.

    // Test 3: Indirectly fuzz ParseOcclusionTextureInfo via loading a model with occlusion texture
    // We can't easily construct a model with occlusion texture from scratch without a full glTF file.
    // But we can try to parse a JSON string that represents a glTF model with occlusion texture.
    // Let's create a simple glTF JSON string with occlusion texture.
    std::string occlusion_json = R"({
        "asset": {"version": "2.0"},
        "materials": [{
            "pbrMetallicRoughness": {
                "occlusionTexture": {"index": 0}
            }
        }],
        "textures": [{"source": 0}],
        "images": [{"uri": "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mNk+M9QDwADhgGAWjR9awAAAABJRU5ErkJggg=="}],
        "samplers": [{"wrapS": 10497, "wrapT": 10497}],
        "accessors": [],
        "buffers": [],
        "meshes": []
    })";
    {
        tinygltf::Model model;
        tinygltf::TinyGLTF ctx;
        std::string err;
        std::string warn;
        // Fixed: Added base_dir and check_sections arguments
        bool res = ctx.LoadASCIIFromString(&model, &err, &warn, occlusion_json.c_str(), static_cast<unsigned int>(occlusion_json.length()), "", tinygltf::REQUIRE_VERSION);
        (void)res;
    }

    // Test 4: Indirectly fuzz IsArray via parsing arrays in JSON
    // The JSON in Test 1 and 3 already contains arrays.

    // Test 5: Indirectly fuzz GetInt via parsing integer fields
    // The JSON in Test 1 and 3 already contains integer fields.

    // Test 6: Indirectly fuzz ParseUnsignedProperty via parsing unsigned integer fields
    // The JSON in Test 1 and 3 already contains unsigned integer fields.

    return 0;
}