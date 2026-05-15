// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
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
#include <string>
#include <vector>

#include "tiny_gltf.h"

static void test_value_apis(const tinygltf::Value& value) {
    // 1. Test Keys() function
    std::vector<std::string> keys = value.Keys();
    
    // 2. Test Get() function for arrays
    if (value.IsArray() && value.ArrayLen() > 0) {
        const tinygltf::Value& elem = value.Get(0);
        (void)elem;
    }
    
    // 3. Test ArrayLen() function
    size_t array_len = value.ArrayLen();
    (void)array_len;
    
    // 4. Test Type() function
    char type = value.Type();
    (void)type;
    
    // 5. Test Has() function with various keys
    bool has_key = value.Has("");
    (void)has_key;
    has_key = value.Has("test");
    (void)has_key;
    has_key = value.Has("key");
    (void)has_key;
    
    // 6. Test Size() function
    size_t size = value.Size();
    (void)size;
    
    // Additional tests for Get() with object keys
    if (value.IsObject() && !keys.empty()) {
        for (const auto& key : keys) {
            const tinygltf::Value& obj_val = value.Get(key);
            (void)obj_val;
        }
    }
    
    // Test Get() with out-of-bounds indices
    if (value.IsArray()) {
        if (value.ArrayLen() > 0) {
            const tinygltf::Value& elem = value.Get(value.ArrayLen() - 1);
            (void)elem;
        }
        // This should return null reference
        const tinygltf::Value& out_of_bounds = value.Get(value.ArrayLen() + 1);
        (void)out_of_bounds;
    }
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Create a string from fuzzer input
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Test with string constructor
    tinygltf::Value string_value(input_str);
    test_value_apis(string_value);
    
    // Test with empty string
    tinygltf::Value empty_string_value("");
    test_value_apis(empty_string_value);
    
    // Test with various string contents
    if (Size > 0) {
        // Create a string with first byte as content
        std::string single_char(1, static_cast<char>(Data[0]));
        tinygltf::Value single_char_value(single_char);
        test_value_apis(single_char_value);
    }
    
    // Test nested values by creating a more complex scenario
    // We'll create a simple JSON-like structure manually
    // Note: This is a simplified approach since we don't have full constructors
    
    // Reuse the string value for additional testing
    // The Value class might have other constructors not shown here,
    // but we work with what's provided
    
    return 0;
}