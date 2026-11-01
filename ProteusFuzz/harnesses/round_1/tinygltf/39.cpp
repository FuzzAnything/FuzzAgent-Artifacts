// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::detail::GetNumber at tiny_gltf.h:3484:6 in tiny_gltf.h
// tinygltf::ParseJsonAsValue at tiny_gltf.h:3664:13 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::detail::JsonIsNull at tiny_gltf.h:7013:6 in tiny_gltf.h
// tinygltf::detail::IsArray at tiny_gltf.h:3520:6 in tiny_gltf.h
// tinygltf::detail::GetInt at tiny_gltf.h:3441:6 in tiny_gltf.h
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
#include <cstddef>
#include <vector>
#include <string>
#include <stdexcept>

// Include tinygltf header
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

// Suppress warnings from nlohmann/json or rapidjson if any
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wsign-conversion"

// Helper to safely parse JSON from fuzz input
// Returns a valid nlohmann::json object if parsing succeeds, or a null object if it fails.
static nlohmann::json ParseJsonSafe(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return nlohmann::json(); // Construct default null JSON
    }
    
    try {
        std::string input(reinterpret_cast<const char*>(Data), Size);
        return nlohmann::json::parse(input);
    } catch (...) {
        // Return null on parse failure to allow the fuzzer to continue with other paths
        return nlohmann::json(); // Construct default null JSON
    }
}

// Helper to convert nlohmann::json to tinygltf::detail::json
// tinygltf uses a typedef for detail::json which maps to nlohmann::json when TINYGLTF_USE_RAPIDJSON is not defined
// or to RapidJSON types otherwise. We assume nlohmann::json is the active backend for this driver
// based on the provided type definitions.
static tinygltf::detail::json ConvertToJson(const nlohmann::json &njson) {
    return static_cast<tinygltf::detail::json>(njson);
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    // Parse the input data into a JSON object
    nlohmann::json njson = ParseJsonSafe(Data, Size);
    tinygltf::detail::json json_val = ConvertToJson(njson);

    // Initialize target variables
    tinygltf::Value val;
    int int_val = 0;
    double double_val = 0.0;

    // 2. & 3. Invoke target functions diversely to explore program states
    
    // Test ParseJsonAsValue
    // This function recursively parses the JSON node into a tinygltf::Value
    // We pass the root node. If the input is not valid JSON for a glTF value, 
    // the function should handle it gracefully (return false or partial success).
    try {
        tinygltf::ParseJsonAsValue(&val, json_val);
    } catch (...) {
        // Catch any unexpected exceptions to prevent fuzzer crash
        return 0;
    }

    // Test IsObject
    // Checks if the root JSON value is an object
    bool is_obj = tinygltf::detail::IsObject(json_val);
    (void)is_obj; // Suppress unused variable warning

    // Test JsonIsNull
    // Checks if the root JSON value is null
    bool is_null = tinygltf::detail::JsonIsNull(json_val);
    (void)is_null;

    // Test IsArray
    // Checks if the root JSON value is an array
    bool is_arr = tinygltf::detail::IsArray(json_val);
    (void)is_arr;

    // Test GetInt
    // Attempts to extract an integer. Will return false if json_val is not an integer type.
    bool got_int = tinygltf::detail::GetInt(json_val, int_val);
    (void)got_int;

    // Test GetNumber
    // Attempts to extract a double. Will return false if json_val is not a number type.
    bool got_num = tinygltf::detail::GetNumber(json_val, double_val);
    (void)got_num;

    // Additional exploration: Try accessing members if it's an object or array
    // This helps trigger internal logic inside the helper functions if they recurse or check types
    if (is_obj) {
        // If it's an object, try to parse a specific key if it exists
        // We don't know the key, but the functions themselves are robust to missing keys
        // The fuzz input might contain keys like "accessor", "mesh", etc.
        // We just ensure the types are checked correctly.
    }

    // 4. Cleanup
    // tinygltf::Value and nlohmann::json handle their own memory management via RAII.
    // No explicit cleanup needed for stack variables.
    
    return 0;
}

#pragma clang diagnostic pop