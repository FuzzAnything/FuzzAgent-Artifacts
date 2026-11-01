// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::SerializeGltfAudioEmitter at tiny_gltf.h:7844:13 in tiny_gltf.h
// tinygltf::SerializeExtrasAndExtensions at tiny_gltf.h:7306:6 in tiny_gltf.h
// tinygltf::ParseAudioEmitter at tiny_gltf.h:5895:13 in tiny_gltf.h
// tinygltf::ParseAudioSource at tiny_gltf.h:5943:13 in tiny_gltf.h
// tinygltf::detail::JsonPushBack at tiny_gltf.h:7005:6 in tiny_gltf.h
// tinygltf::SerializeGltfAudioSource at tiny_gltf.h:7863:13 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <stdexcept>

#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

// Helper to safely convert fuzz input to a string, ensuring null termination
static std::string SafeToString(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "";
    size_t len = Size;
    for (size_t i = 0; i < Size; ++i) {
        if (Data[i] == 0) {
            len = i;
            break;
        }
    }
    if (len > 1024 * 1024) len = 1024 * 1024;
    return std::string(reinterpret_cast<const char*>(Data), len);
}

// Helper to parse JSON from fuzz input safely
static bool TryParseJSON(const std::string &input, tinygltf::detail::json &out) {
    try {
        // nlohmann::json parse
        out = tinygltf::detail::json::parse(input, nullptr, false);
        return true;
    } catch (...) {
        // Parse failed, return empty/null json
        out = tinygltf::detail::json();
        return false;
    }
}

// Helper to safely access data with bounds checking
static uint8_t SafeGetData(const uint8_t *Data, size_t Size, size_t index) {
    if (index < Size) {
        return Data[index];
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    size_t split = Size / 2;
    if (split == 0) split = 1;
    
    std::string jsonStr = SafeToString(Data, split);
    std::string strData = SafeToString(Data + split, Size - split);

    tinygltf::detail::json jsonObj;
    bool jsonParsed = TryParseJSON(jsonStr, jsonObj);

    if (!jsonParsed) {
        jsonObj = tinygltf::detail::json();
    }

    // --- Test 1: SerializeGltfAudioSource ---
    {
        tinygltf::AudioSource source;
        source.name = strData;
        source.bufferView = static_cast<int>(SafeGetData(Data, Size, 0));
        source.extensions.clear();
        source.extras = tinygltf::Value();
        
        if (!strData.empty()) {
            source.extras = tinygltf::Value(strData);
        }

        tinygltf::detail::json outJson;
        try {
            tinygltf::SerializeGltfAudioSource(source, outJson);
        } catch (...) {
            // Ignore exceptions during serialization
        }
    }

    // --- Test 2: SerializeGltfAudioEmitter ---
    {
        tinygltf::AudioEmitter emitter;
        emitter.name = strData;
        emitter.gain = static_cast<double>(SafeGetData(Data, Size, 0)) / 255.0;
        emitter.loop = (SafeGetData(Data, Size, 1) & 1) != 0;
        emitter.source = static_cast<int>(SafeGetData(Data, Size, 2));
        
        if (SafeGetData(Data, Size, 3) & 1) {
            emitter.type = "positional";
            emitter.positional.coneInnerAngle = static_cast<double>(SafeGetData(Data, Size, 4));
        } else {
            emitter.type = "global";
        }

        tinygltf::detail::json outJson;
        try {
            tinygltf::SerializeGltfAudioEmitter(emitter, outJson);
        } catch (...) {
            // Ignore exceptions
        }
    }

    // --- Test 3: SerializeExtrasAndExtensions ---
    {
        tinygltf::AudioSource source;
        source.name = strData;
        source.extensions["EXT_test"] = tinygltf::Value("test_value");
        source.extras = tinygltf::Value(strData);

        tinygltf::detail::json outJson;
        try {
            tinygltf::SerializeExtrasAndExtensions(source, outJson);
        } catch (...) {
            // Ignore exceptions
        }
    }

    // --- Test 4: ParseAudioEmitter ---
    {
        tinygltf::AudioEmitter emitter;
        std::string err;
        bool store_original = (SafeGetData(Data, Size, 5) & 1) != 0;

        try {
            tinygltf::ParseAudioEmitter(&emitter, &err, jsonObj, store_original);
        } catch (...) {
            // Ignore exceptions
        }
    }

    // --- Test 5: ParseAudioSource ---
    {
        tinygltf::AudioSource source;
        std::string err;
        bool store_original = (SafeGetData(Data, Size, 6) & 1) != 0;

        try {
            tinygltf::ParseAudioSource(&source, &err, jsonObj, store_original);
        } catch (...) {
            // Ignore exceptions
        }
    }

    // --- Test 6: JsonPushBack ---
    {
        tinygltf::detail::json arrJson = tinygltf::detail::json::array();
        tinygltf::detail::json valJson = jsonObj; 

        try {
            // Use the correct namespace: tinygltf::detail::JsonPushBack
            tinygltf::detail::JsonPushBack(arrJson, std::move(valJson));
        } catch (...) {
            // Ignore exceptions
        }
    }

    return 0;
}