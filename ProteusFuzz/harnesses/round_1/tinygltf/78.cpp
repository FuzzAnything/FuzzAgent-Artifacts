// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::SerializeGltfCamera at tiny_gltf.h:8056:13 in tiny_gltf.h
// tinygltf::SerializeGltfOrthographicCamera at tiny_gltf.h:8031:13 in tiny_gltf.h
// tinygltf::SerializeExtrasAndExtensions at tiny_gltf.h:7306:6 in tiny_gltf.h
// tinygltf::ParseCamera at tiny_gltf.h:5755:13 in tiny_gltf.h
// tinygltf::ParseCamera at tiny_gltf.h:5755:13 in tiny_gltf.h
// tinygltf::ParseOrthographicCamera at tiny_gltf.h:5718:13 in tiny_gltf.h
// tinygltf::ParseOrthographicCamera at tiny_gltf.h:5718:13 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <cstring>

// Ensure tinygltf uses nlohmann/json if available or its internal implementation
// The provided signatures imply nlohmann::json usage via detail::json
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tiny_gltf.h"

// Helper to create a random nlohmann::json object from raw bytes
// This attempts to construct valid JSON structures to trigger parsing logic
static nlohmann::json CreateRandomJson(const uint8_t *Data, size_t Size) {
    nlohmann::json j;
    if (Size == 0) {
        return j;
    }

    // Use the data to seed a pseudo-random generator for structure creation
    unsigned int seed = 0;
    if (Size >= 4) {
        std::memcpy(&seed, Data, 4);
    } else {
        std::memcpy(&seed, Data, Size);
    }

    // Simple LCG to generate variety
    auto next_rand = [&]() -> int {
        seed = seed * 1103515245 + 12345;
        return (seed >> 16) & 0x7fff;
    };

    // Randomly choose type
    int type_choice = next_rand() % 5;
    
    switch (type_choice) {
        case 0: // Object
            j = nlohmann::json::object();
            for (int i = 0; i < 5 && i < Size; ++i) {
                std::string key = "key_" + std::to_string(i);
                int val = next_rand();
                j[key] = val;
            }
            break;
        case 1: // Array
            j = nlohmann::json::array();
            for (int i = 0; i < 5 && i < Size; ++i) {
                j.push_back(next_rand());
            }
            break;
        case 2: // String
            j = std::string(reinterpret_cast<const char*>(Data), std::min(Size, size_t(100)));
            break;
        case 3: // Number
            j = static_cast<int>(next_rand());
            break;
        case 4: // Boolean
            j = (next_rand() % 2) == 0;
            break;
    }
    
    return j;
}

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    tinygltf::Camera camera;
    tinygltf::OrthographicCamera orthoCamera;
    tinygltf::PerspectiveCamera perspCamera;
    
    // Initialize camera types based on input data to explore different branches
    int typeChoice = Data[0] % 3;
    if (typeChoice == 0) {
        camera.type = "perspective";
    } else if (typeChoice == 1) {
        camera.type = "orthographic";
    } else {
        camera.type = "unknown_type"; // Test unknown type handling
    }

    // Populate camera fields with fuzzed data
    if (Size > 1) camera.perspective.aspectRatio = static_cast<double>(Data[1]);
    if (Size > 2) camera.orthographic.xmag = static_cast<double>(Data[2]);
    if (Size > 3) camera.orthographic.ymag = static_cast<double>(Data[3]);
    if (Size > 4) camera.orthographic.znear = static_cast<double>(Data[4]);
    if (Size > 5) camera.orthographic.zfar = static_cast<double>(Data[5]);

    // Create JSON object for serialization tests
    nlohmann::json jsonOut;
    
    // 2. Execute Target Functions

    // Test 1: SerializeGltfCamera
    // This function writes to jsonOut. It handles "perspective", "orthographic", and unknown types.
    try {
        tinygltf::SerializeGltfCamera(camera, jsonOut);
    } catch (...) {
        // Ignore exceptions during serialization
    }

    // Test 2: SerializeGltfOrthographicCamera
    // Create a fresh JSON for this specific test
    nlohmann::json jsonOrtho;
    try {
        tinygltf::SerializeGltfOrthographicCamera(camera.orthographic, jsonOrtho);
    } catch (...) {
        // Ignore exceptions
    }

    // Test 3: SerializeExtrasAndExtensions
    // Test with Camera object
    nlohmann::json jsonExtras;
    try {
        tinygltf::SerializeExtrasAndExtensions<tinygltf::Camera>(camera, jsonExtras);
    } catch (...) {
        // Ignore exceptions
    }

    // Test 4: ParseCamera
    // Create a random JSON input to test parsing logic
    nlohmann::json jsonInput = CreateRandomJson(Data, Size);
    std::string err;
    tinygltf::Camera parsedCamera;
    
    // Ensure we try to parse as an object, as ParseCamera expects an object
    if (!jsonInput.is_object()) {
        jsonInput = nlohmann::json::object();
        // Inject some data from fuzz input into the object
        if (Size > 0) jsonInput["type"] = std::string(reinterpret_cast<const char*>(Data), std::min(Size, size_t(20)));
    }

    try {
        tinygltf::ParseCamera(&parsedCamera, &err, jsonInput, true);
        // Also test with store_original_json = false
        tinygltf::ParseCamera(&parsedCamera, &err, jsonInput, false);
    } catch (...) {
        // Ignore exceptions
    }

    // Test 5: ParseOrthographicCamera
    tinygltf::OrthographicCamera parsedOrtho;
    std::string err2;
    nlohmann::json jsonOrthoInput = CreateRandomJson(Data, Size);
    
    if (!jsonOrthoInput.is_object()) {
        jsonOrthoInput = nlohmann::json::object();
    }
    // Inject numeric fields to trigger parsing logic
    if (Size > 10) {
        jsonOrthoInput["xmag"] = static_cast<double>(Data[10]);
        jsonOrthoInput["ymag"] = static_cast<double>(Data[11]);
        jsonOrthoInput["zfar"] = static_cast<double>(Data[12]);
        jsonOrthoInput["znear"] = static_cast<double>(Data[13]);
    }

    try {
        tinygltf::ParseOrthographicCamera(&parsedOrtho, &err2, jsonOrthoInput, true);
        tinygltf::ParseOrthographicCamera(&parsedOrtho, &err2, jsonOrthoInput, false);
    } catch (...) {
        // Ignore exceptions
    }

    // Test 6: IsObject
    // Test with various JSON types generated
    bool isObj1 = tinygltf::detail::IsObject(jsonInput);
    bool isObj2 = tinygltf::detail::IsObject(jsonOut);
    bool isObj3 = tinygltf::detail::IsObject(nlohmann::json::array());
    bool isObj4 = tinygltf::detail::IsObject(nlohmann::json("string"));
    bool isObj5 = tinygltf::detail::IsObject(nlohmann::json(123));
    bool isObj6 = tinygltf::detail::IsObject(nlohmann::json(true));
    bool isObj7 = tinygltf::detail::IsObject(nlohmann::json(nullptr));

    // Use the boolean results to prevent unused variable warnings
    (void)isObj1;
    (void)isObj2;
    (void)isObj3;
    (void)isObj4;
    (void)isObj5;
    (void)isObj6;
    (void)isObj7;

    return 0;
}