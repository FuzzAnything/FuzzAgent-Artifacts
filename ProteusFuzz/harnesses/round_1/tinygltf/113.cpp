// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ParseIntegerProperty at tiny_gltf.h:3821:13 in tiny_gltf.h
// tinygltf::ParsePositionalEmitter at tiny_gltf.h:5875:13 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::ParseExtensionsProperty at tiny_gltf.h:4220:13 in tiny_gltf.h
// tinygltf::ParseAsset at tiny_gltf.h:4280:13 in tiny_gltf.h
// tinygltf::ParseLight at tiny_gltf.h:5831:13 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tiny_gltf.h"

// Helper to create a valid tinygltf::detail::json object from raw input.
static tinygltf::detail::json CreateJsonFromInput(const uint8_t *Data, size_t Size) {
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    #ifdef TINYGLTF_USE_NLOHMANN_JSON
    try {
        nlohmann::json j = nlohmann::json::parse(input_str);
        // Cast to tinygltf::detail::json which is an alias for nlohmann::json
        return static_cast<tinygltf::detail::json>(j);
    } catch (...) {
        // Return an empty JSON object on parse failure
        return tinygltf::detail::json(nlohmann::json());
    }
    #else
    // For non-nlohmann builds (e.g., RapidJSON), we cannot easily construct
    // a valid json object from raw bytes without the specific internal types.
    // We return a default constructed json object. The parser functions
    // should handle invalid/empty inputs gracefully.
    return tinygltf::detail::json();
    #endif
}

extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare the JSON object from input
    tinygltf::detail::json json_obj = CreateJsonFromInput(Data, Size);
    
    std::string err;
    bool store_original_json = true; 

    // 1. Fuzz tinygltf::ParseIntegerProperty
    int int_ret = 0;
    std::vector<std::string> prop_names = {"count", "id", "index", "size", "invalid"};
    for (const auto& prop : prop_names) {
        bool required = (Size % 2 == 0);
        err.clear();
        tinygltf::ParseIntegerProperty(&int_ret, &err, json_obj, prop, required);
    }

    // 2. Fuzz tinygltf::ParsePositionalEmitter
    tinygltf::PositionalEmitter emitter;
    err.clear();
    tinygltf::ParsePositionalEmitter(&emitter, &err, json_obj, store_original_json);

    // 3. Fuzz tinygltf::detail::IsObject (Corrected namespace)
    bool is_obj = tinygltf::detail::IsObject(json_obj);
    (void)is_obj; 

    // 4. Fuzz tinygltf::ParseExtensionsProperty
    tinygltf::ExtensionMap extensions;
    err.clear();
    tinygltf::ParseExtensionsProperty(&extensions, &err, json_obj);

    // 5. Fuzz tinygltf::ParseAsset
    tinygltf::Asset asset;
    err.clear();
    tinygltf::ParseAsset(&asset, &err, json_obj, store_original_json);

    // 6. Fuzz tinygltf::ParseLight
    tinygltf::Light light;
    err.clear();
    tinygltf::ParseLight(&light, &err, json_obj, store_original_json);

    return 0;
}