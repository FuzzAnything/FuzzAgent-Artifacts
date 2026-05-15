// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
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
#include <array>

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Use a small subset of the input to control behavior
    if (Size < 1) return 0;
    
    // Create array for Value constructor
    tinygltf::Value::Array array_data;
    
    // Create some sample values to populate the array
    // Use input data to create varied content
    for (size_t i = 0; i < std::min(static_cast<size_t>(5), Size); ++i) {
        // Create different types of values based on input
        if ((Data[i] % 5) == 0) {
            array_data.push_back(tinygltf::Value(static_cast<int>(Data[i])));
        } else if ((Data[i] % 5) == 1) {
            array_data.push_back(tinygltf::Value(static_cast<double>(Data[i])));
        } else if ((Data[i] % 5) == 2) {
            array_data.push_back(tinygltf::Value(std::string("test") + std::to_string(Data[i])));
        } else if ((Data[i] % 5) == 3) {
            // Use the correct constructor for binary data
            std::vector<unsigned char> binary_data = {static_cast<unsigned char>(Data[i])};
            array_data.push_back(tinygltf::Value(binary_data.data(), binary_data.size()));
        } else {
            // Create nested array
            tinygltf::Value::Array nested_array;
            array_data.push_back(tinygltf::Value(nested_array));
        }
    }
    
    // Create Value object using array constructor
    tinygltf::Value value(array_data);
    
    // Test IsArray()
    bool is_array = value.IsArray();
    
    // Test Get() overloads
    // Only call Get() if the type matches to avoid undefined behavior
    if (value.IsArray()) {
        try {
            const tinygltf::Value::Array& arr = value.Get<tinygltf::Value::Array>();
            (void)arr; // Avoid unused variable warning
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Try other Get() types - they may fail but we want to test them
    try {
        const int& int_val = value.Get<int>();
        (void)int_val;
    } catch (...) {}
    
    try {
        const double& double_val = value.Get<double>();
        (void)double_val;
    } catch (...) {}
    
    try {
        const std::string& string_val = value.Get<std::string>();
        (void)string_val;
    } catch (...) {}
    
    try {
        const std::vector<unsigned char>& binary_val = value.Get<std::vector<unsigned char>>();
        (void)binary_val;
    } catch (...) {}
    
    try {
        const bool& bool_val = value.Get<bool>();
        (void)bool_val;
    } catch (...) {}
    
    // Create an object value for testing Has() and Keys()
    tinygltf::Value::Object object_data;
    
    // Add some key-value pairs using input data
    for (size_t i = 0; i < std::min(static_cast<size_t>(3), Size); ++i) {
        std::string key = "key" + std::to_string(i);
        object_data[key] = tinygltf::Value(static_cast<int>(Data[i % Size]));
    }
    
    tinygltf::Value object_value(object_data);
    
    // Test IsObject()
    bool is_object = object_value.IsObject();
    
    // Test Has() with various keys
    if (object_value.IsObject()) {
        // Test with existing keys
        for (size_t i = 0; i < std::min(static_cast<size_t>(3), Size); ++i) {
            std::string key = "key" + std::to_string(i);
            bool has_key = object_value.Has(key);
            (void)has_key;
        }
        
        // Test with non-existent key
        bool has_nonexistent = object_value.Has("nonexistent_key");
        (void)has_nonexistent;
        
        // Test with key generated from input data
        if (Size > 0) {
            std::string input_key(reinterpret_cast<const char*>(Data), std::min(static_cast<size_t>(10), Size));
            bool has_input_key = object_value.Has(input_key);
            (void)has_input_key;
        }
    }
    
    // Test Keys()
    if (object_value.IsObject()) {
        std::vector<std::string> keys = object_value.Keys();
        (void)keys;
    }
    
    // Also test Keys() on non-object (should return empty vector)
    if (value.IsArray()) {
        std::vector<std::string> array_keys = value.Keys();
        (void)array_keys;
    }
    
    // Test Get<Value::Object>() on object value
    if (object_value.IsObject()) {
        try {
            const tinygltf::Value::Object& obj = object_value.Get<tinygltf::Value::Object>();
            (void)obj;
        } catch (...) {}
    }
    
    return 0;
}