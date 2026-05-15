// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <array>

static void testValueFunctions(const tinygltf::Value& value) {
    // Test IsBool
    (void)value.IsBool();
    
    // Test IsReal
    (void)value.IsReal();
    
    // Test IsString
    (void)value.IsString();
    
    // Test IsBinary
    (void)value.IsBinary();
    
    // Test Get overloads
    // First overload: Get(const std::string& key) - only call if IsObject()
    if (value.IsObject()) {
        (void)value.Get("test_key");
    }
    
    // Second overload: Get<T>() for various types - only call if type matches
    if (value.IsBool()) {
        (void)value.Get<bool>();
    }
    
    if (value.IsReal()) {
        (void)value.Get<double>();
    }
    
    // Use IsInt() to check for integer type instead of accessing type_ directly
    if (value.IsInt()) {
        (void)value.Get<int>();
    }
    
    if (value.IsString()) {
        (void)value.Get<std::string>();
    }
    
    if (value.IsBinary()) {
        (void)value.Get<std::vector<unsigned char>>();
    }
    
    if (value.IsArray()) {
        (void)value.Get<tinygltf::Value::Array>();
    }
    
    if (value.IsObject()) {
        (void)value.Get<tinygltf::Value::Object>();
    }
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Create a string from fuzzer input
    std::string input_string;
    if (Size > 0) {
        input_string = std::string(reinterpret_cast<const char*>(Data), Size);
    }
    
    // Create Value objects with different types to test various states
    // 1. String value
    tinygltf::Value string_value(std::move(input_string));
    testValueFunctions(string_value);
    
    // 2. Bool value
    tinygltf::Value bool_value(true);
    testValueFunctions(bool_value);
    
    // 3. Integer value
    tinygltf::Value int_value(42);
    testValueFunctions(int_value);
    
    // 4. Real value
    tinygltf::Value real_value(3.14159);
    testValueFunctions(real_value);
    
    // 5. Binary value
    std::vector<unsigned char> binary_data = {'b', 'i', 'n', 'a', 'r', 'y'};
    tinygltf::Value binary_value(std::move(binary_data));
    testValueFunctions(binary_value);
    
    // 6. Array value
    tinygltf::Value::Array array_data;
    array_data.push_back(tinygltf::Value(1));
    array_data.push_back(tinygltf::Value(2.0));
    array_data.push_back(tinygltf::Value("array_element"));
    tinygltf::Value array_value(std::move(array_data));
    testValueFunctions(array_value);
    
    // 7. Object value
    tinygltf::Value::Object object_data;
    object_data["key1"] = tinygltf::Value("value1");
    object_data["key2"] = tinygltf::Value(123);
    object_data["key3"] = tinygltf::Value(true);
    tinygltf::Value object_value(std::move(object_data));
    testValueFunctions(object_value);
    
    // 8. Color value (using the ColorValue type alias)
    std::array<double, 4> color_data = {1.0, 0.5, 0.0, 1.0};
    // Create an array value from color data
    tinygltf::Value::Array color_array;
    for (const auto& val : color_data) {
        color_array.push_back(tinygltf::Value(val));
    }
    tinygltf::Value color_value(std::move(color_array));
    testValueFunctions(color_value);
    
    // 9. Empty/default value
    tinygltf::Value empty_value;
    testValueFunctions(empty_value);
    
    return 0;
}