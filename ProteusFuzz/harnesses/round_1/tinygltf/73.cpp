// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::detail::JsonPushBack at tiny_gltf.h:7005:6 in tiny_gltf.h
// tinygltf::SerializeGltfAnimation at tiny_gltf.h:7438:13 in tiny_gltf.h
// tinygltf::SerializeExtensionMap at tiny_gltf.h:7274:13 in tiny_gltf.h
// tinygltf::detail::JsonAssign at tiny_gltf.h:6975:6 in tiny_gltf.h
// tinygltf::SerializeGltfAsset at tiny_gltf.h:7471:13 in tiny_gltf.h
// tinygltf::detail::JsonReserveArray at tiny_gltf.h:7029:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>

// Define TINYGLTF_IMPLEMENTATION before including the header to expose internal details
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

// Helper to safely parse input data into a string
static std::string safe_string_from_input(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "";
    size_t len = Size;
    for (size_t i = 0; i < Size; ++i) {
        if (Data[i] == 0) {
            len = i;
            break;
        }
    }
    return std::string(reinterpret_cast<const char*>(Data), len);
}

// Helper to generate a size based on input
static size_t get_size_from_input(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    if (Size >= 4) {
        uint32_t val = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        return val % 10000; 
    }
    return Data[0] % 100;
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    try {
        // Use the correct type alias for json in tinygltf
        // With TINYGLTF_IMPLEMENTATION defined, tinygltf::detail should be visible.
        using JsonType = tinygltf::detail::json;
        
        // Initialize Asset
        tinygltf::Asset asset;
        std::string version_str = safe_string_from_input(Data, Size);
        if (!version_str.empty()) asset.version = version_str;
        
        // Initialize Animation
        tinygltf::Animation animation;
        std::string name_str = safe_string_from_input(Data + 1, Size > 1 ? Size - 1 : 0);
        if (!name_str.empty()) animation.name = name_str;
        
        // Initialize ExtensionMap
        tinygltf::ExtensionMap extensions;
        extensions["EXT_test"] = tinygltf::Value("test_value");
        extensions["EXT_null"] = tinygltf::Value(); // Empty value
        
        // Populate Animation Channels and Samplers based on input size
        size_t num_channels = get_size_from_input(Data, Size);
        for (size_t i = 0; i < num_channels; ++i) {
            tinygltf::AnimationChannel channel;
            channel.sampler = static_cast<int>(i);
            channel.target_path = "translation"; 
            animation.channels.push_back(channel);
        }
        
        size_t num_samplers = get_size_from_input(Data + 1, Size > 1 ? Size - 1 : 0);
        for (size_t i = 0; i < num_samplers; ++i) {
            tinygltf::AnimationSampler sampler;
            sampler.input = static_cast<int>(i);
            sampler.interpolation = "LINEAR";
            animation.samplers.push_back(sampler);
        }

        // Target 1: SerializeGltfAsset (Corrected namespace)
        {
            JsonType asset_json;
            tinygltf::SerializeGltfAsset(asset, asset_json);
            (void)asset_json.dump(); 
        }

        // Target 2: JsonReserveArray
        {
            JsonType array_json;
            array_json = JsonType::array();
            size_t reserve_size = get_size_from_input(Data, Size);
            tinygltf::detail::JsonReserveArray(array_json, reserve_size);
        }

        // Target 3: JsonPushBack
        {
            JsonType push_array;
            push_array = JsonType::array();
            
            JsonType value_to_push;
            value_to_push = "fuzz_string";
            
            for (size_t i = 0; i < 5; ++i) {
                tinygltf::detail::JsonPushBack(push_array, std::move(value_to_push));
                value_to_push = i; 
            }
        }

        // Target 4: SerializeGltfAnimation (Corrected namespace)
        {
            JsonType anim_json;
            tinygltf::SerializeGltfAnimation(animation, anim_json);
            (void)anim_json.dump();
        }

        // Target 5: SerializeExtensionMap (Corrected namespace)
        {
            JsonType ext_json;
            tinygltf::SerializeExtensionMap(extensions, ext_json);
            (void)ext_json.dump();
        }

        // Target 6: JsonAssign
        {
            JsonType src_json;
            src_json = "source_data";
            JsonType dest_json;
            tinygltf::detail::JsonAssign(dest_json, src_json);
            
            if (dest_json.is_string() && dest_json.get<std::string>() == "source_data") {
                // Success
            }
        }

    } catch (...) {
        return 0;
    }

    return 0;
}