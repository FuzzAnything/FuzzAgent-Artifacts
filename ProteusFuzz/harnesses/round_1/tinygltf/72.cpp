// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::detail::FindMember at tiny_gltf.h:3592:6 in tiny_gltf.h
// tinygltf::detail::FindMember at tiny_gltf.h:3592:6 in tiny_gltf.h
// tinygltf::detail::JsonAddMember at tiny_gltf.h:6983:6 in tiny_gltf.h
// tinygltf::detail::JsonAddMember at tiny_gltf.h:6983:6 in tiny_gltf.h
// tinygltf::ParseIntegerProperty at tiny_gltf.h:3821:13 in tiny_gltf.h
// tinygltf::ParseIntegerProperty at tiny_gltf.h:3821:13 in tiny_gltf.h
// tinygltf::ParseIntegerProperty at tiny_gltf.h:3821:13 in tiny_gltf.h
// tinygltf::ParseIntegerProperty at tiny_gltf.h:3821:13 in tiny_gltf.h
// tinygltf::ParseIntegerProperty at tiny_gltf.h:3821:13 in tiny_gltf.h
// tinygltf::detail::IsEmpty at tiny_gltf.h:3614:6 in tiny_gltf.h
// tinygltf::detail::IsEmpty at tiny_gltf.h:3614:6 in tiny_gltf.h
// tinygltf::detail::IsEmpty at tiny_gltf.h:3614:6 in tiny_gltf.h
// tinygltf::detail::IsEmpty at tiny_gltf.h:3614:6 in tiny_gltf.h
// tinygltf::detail::IsEmpty at tiny_gltf.h:3614:6 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::detail::IsObject at tiny_gltf.h:3544:6 in tiny_gltf.h
// tinygltf::detail::IsArray at tiny_gltf.h:3520:6 in tiny_gltf.h
// tinygltf::detail::IsArray at tiny_gltf.h:3520:6 in tiny_gltf.h
// tinygltf::detail::IsArray at tiny_gltf.h:3520:6 in tiny_gltf.h
// tinygltf::detail::IsArray at tiny_gltf.h:3520:6 in tiny_gltf.h
// tinygltf::detail::IsArray at tiny_gltf.h:3520:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>

// Define implementation before including to expose detail namespace
#define TINYGLTF_IMPLEMENTATION

#include "tiny_gltf.h"

// Helper to generate a random property name
static std::string GeneratePropertyName(const uint8_t* data, size_t size) {
    if (size == 0) return "test_prop";
    std::string prop(reinterpret_cast<const char*>(data), size);
    // Ensure it's not empty
    if (prop.empty()) return "test_prop";
    return prop;
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    // Use the type alias from tinygltf::detail
    using JsonType = tinygltf::detail::json;

    JsonType json_obj;
    JsonType json_array;
    JsonType json_null;
    JsonType json_string;
    JsonType json_number;

    try {
        // Attempt to parse the input as a JSON object
        std::string input_str(reinterpret_cast<const char*>(Data), Size);
        // Try to parse. This works for nlohmann::json. 
        json_obj = JsonType::parse(input_str);
    } catch (...) {
        // If parsing fails, we still want to test with a valid empty/null JSON
        json_obj = JsonType();
    }

    // Create specific types for diverse testing
    try {
        json_array = JsonType::array();
        json_null = JsonType(nullptr);
        json_string = JsonType("fuzz_string");
        json_number = JsonType(42);
        
        // Add some data to array and object if possible
        if (!json_array.is_null()) {
            json_array.push_back(1);
            json_array.push_back("test");
        }
        
        if (json_obj.is_object()) {
            json_obj["key1"] = "value1";
            json_obj["int_val"] = 100;
        }
    } catch (...) {
        // Ignore construction errors
    }

    // 2. Invoke Target API Functions

    // Test 1: tinygltf::detail::IsEmpty
    {
        bool r1 = tinygltf::detail::IsEmpty(json_obj);
        bool r2 = tinygltf::detail::IsEmpty(json_array);
        bool r3 = tinygltf::detail::IsEmpty(json_null);
        bool r4 = tinygltf::detail::IsEmpty(json_string);
        bool r5 = tinygltf::detail::IsEmpty(json_number);
        (void)r1; (void)r2; (void)r3; (void)r4; (void)r5;
    }

    // Test 2: tinygltf::detail::IsObject
    {
        bool r1 = tinygltf::detail::IsObject(json_obj);
        bool r2 = tinygltf::detail::IsObject(json_array);
        bool r3 = tinygltf::detail::IsObject(json_null);
        bool r4 = tinygltf::detail::IsObject(json_string);
        bool r5 = tinygltf::detail::IsObject(json_number);
        (void)r1; (void)r2; (void)r3; (void)r4; (void)r5;
    }

    // Test 3: tinygltf::detail::IsArray
    {
        bool r1 = tinygltf::detail::IsArray(json_obj);
        bool r2 = tinygltf::detail::IsArray(json_array);
        bool r3 = tinygltf::detail::IsArray(json_null);
        bool r4 = tinygltf::detail::IsArray(json_string);
        bool r5 = tinygltf::detail::IsArray(json_number);
        (void)r1; (void)r2; (void)r3; (void)r4; (void)r5;
    }

    // Test 4: tinygltf::detail::FindMember
    {
        tinygltf::detail::json_iterator it;
        // Try to find a member in the parsed object
        if (json_obj.is_object()) {
            // Use a key derived from input or a fixed one
            const char* key = "key1";
            if (Size > 10) {
                // Try to use input data as key (careful with null bytes)
                // We'll just use a safe substring or fixed key for stability
                key = "fuzz_key";
            }
            bool found = tinygltf::detail::FindMember(json_obj, key, it);
            (void)found;
        }
        // Test on non-object
        bool found_array = tinygltf::detail::FindMember(json_array, "key", it);
        (void)found_array;
    }

    // Test 5: tinygltf::detail::JsonAddMember
    {
        JsonType target_obj = JsonType::object();
        JsonType value_to_add = JsonType("test_value");
        
        // Add to object
        if (target_obj.is_object()) {
            tinygltf::detail::JsonAddMember(target_obj, "new_key", std::move(value_to_add));
        }
        
        // Try to add to non-object (should handle gracefully or assert depending on impl)
        JsonType target_array = JsonType::array();
        JsonType value_to_add2 = JsonType(123);
        // This might trigger an assertion or do nothing depending on implementation
        // We wrap in try-catch just in case, though the API is void
        try {
            tinygltf::detail::JsonAddMember(target_array, "bad_key", std::move(value_to_add2));
        } catch (...) {
            // Ignore
        }
    }

    // Test 6: tinygltf::ParseIntegerProperty
    {
        int ret_val = 0;
        std::string err_str;
        
        // Test on valid object with integer
        if (json_obj.is_object()) {
            // Try to parse a known key
            bool success1 = tinygltf::ParseIntegerProperty(&ret_val, &err_str, json_obj, "int_val", true, "root");
            (void)success1;
            
            // Try to parse a missing required key
            bool success2 = tinygltf::ParseIntegerProperty(&ret_val, &err_str, json_obj, "missing_key", true, "root");
            (void)success2;
            
            // Try to parse a non-integer key as integer
            bool success3 = tinygltf::ParseIntegerProperty(&ret_val, &err_str, json_obj, "key1", false, "root");
            (void)success3;
        }
        
        // Test on non-object
        bool success4 = tinygltf::ParseIntegerProperty(&ret_val, &err_str, json_array, "any", true, "root");
        (void)success4;
        
        // Test with dynamic property name from input
        std::string prop_name = GeneratePropertyName(Data, Size);
        bool success5 = tinygltf::ParseIntegerProperty(&ret_val, &err_str, json_obj, prop_name, false, "root");
        (void)success5;
    }

    // 3. Cleanup
    // Since we use RAII (std::string, std::vector, nlohmann::json), no explicit cleanup is needed.
    // The function returns 0 to indicate success.

    return 0;
}