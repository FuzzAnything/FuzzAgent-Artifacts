// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
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
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <map>

static void testValueFunctions(const tinygltf::Value& value) {
    // Test ArrayLen
    size_t array_len = value.ArrayLen();
    
    // Test IsArray
    bool is_array = value.IsArray();
    
    // Test Size
    size_t size = value.Size();
    
    // Test IsObject
    bool is_object = value.IsObject();
    
    // Test Has for object type
    if (is_object) {
        bool has_key = value.Has("test_key");
    }
    
    // Test Get for different types if applicable
    if (is_array && array_len > 0) {
        // Try to get array element at index 0
        tinygltf::Value array_elem = value.Get(0);
    }
    
    if (is_object) {
        // Try to get object element by key
        tinygltf::Value obj_elem = value.Get("test_key");
    }
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Check if we have at least some data to work with
    if (Size == 0) {
        return 0;
    }
    
    // Create a Value object with array type
    std::vector<tinygltf::Value> array_data;
    
    // Add some dummy values to the array, safely accessing Data
    int int_val = 0;
    double double_val = 0.0;
    
    if (Size >= 1) {
        int_val = static_cast<int>(Data[0] % 100);
    }
    array_data.push_back(tinygltf::Value(int_val));
    
    if (Size >= 2) {
        double_val = static_cast<double>(Data[1] % 100) / 100.0;
    }
    array_data.push_back(tinygltf::Value(double_val));
    
    tinygltf::Value array_value(array_data);
    testValueFunctions(array_value);
    
    // Create a Value object with object type
    std::map<std::string, tinygltf::Value> object_data;
    
    // Add some key-value pairs, safely accessing Data
    int int_key_val = 0;
    double double_key_val = 0.0;
    
    if (Size >= 3) {
        int_key_val = static_cast<int>(Data[2] % 100);
    }
    object_data["int_key"] = tinygltf::Value(int_key_val);
    
    if (Size >= 4) {
        double_key_val = static_cast<double>(Data[3] % 100) / 100.0;
    }
    object_data["double_key"] = tinygltf::Value(double_key_val);
    
    // Create string from fuzzer data
    if (Size > 4) {
        size_t str_len = std::min<size_t>(Size - 4, 100);
        std::string str_data(reinterpret_cast<const char*>(Data + 4), str_len);
        object_data["string_key"] = tinygltf::Value(str_data);
    }
    
    tinygltf::Value object_value(object_data);
    testValueFunctions(object_value);
    
    // Test with empty array
    std::vector<tinygltf::Value> empty_array;
    tinygltf::Value empty_array_value(empty_array);
    testValueFunctions(empty_array_value);
    
    // Test with empty object
    std::map<std::string, tinygltf::Value> empty_object;
    tinygltf::Value empty_object_value(empty_object);
    testValueFunctions(empty_object_value);
    
    return 0;
}