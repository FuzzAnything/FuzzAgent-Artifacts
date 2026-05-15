// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
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

static void testValueFunctions(const uint8_t* data, size_t size) {
    // Create a Value object using the double constructor
    double value = 0.0;
    if (size >= sizeof(double)) {
        // Use first 8 bytes as double value if available
        memcpy(&value, data, sizeof(double));
    } else if (size > 0) {
        // Use first byte as integer value for double conversion
        value = static_cast<double>(data[0]);
    }
    
    tinygltf::Value val(value);
    
    // Test IsArray() function
    bool isArray = val.IsArray();
    (void)isArray; // Prevent unused variable warning
    
    // Test IsString() function
    bool isString = val.IsString();
    (void)isString;
    
    // Test IsObject() function
    bool isObject = val.IsObject();
    (void)isObject;
    
    // Test Has() function - only valid for object type
    // Create a key from the input data
    std::string key;
    if (size > sizeof(double)) {
        size_t key_len = std::min(size - sizeof(double), static_cast<size_t>(100));
        key = std::string(reinterpret_cast<const char*>(data + sizeof(double)), key_len);
    } else if (size > 0) {
        key = std::string(reinterpret_cast<const char*>(data), size);
    } else {
        key = "default_key";
    }
    
    bool hasKey = val.Has(key);
    (void)hasKey;
    
    // Test Get() function overloads
    // For array_value_ type
    try {
        auto array_val = val.Get<tinygltf::Value::Array>();
        (void)array_val;
    } catch (...) {
        // Ignore exceptions from Get() when type doesn't match
    }
    
    // For object_value_ type
    try {
        auto object_val = val.Get<tinygltf::Value::Object>();
        (void)object_val;
    } catch (...) {
        // Ignore exceptions from Get() when type doesn't match
    }
    
    // Test additional Get() overloads for other types
    try {
        auto bool_val = val.Get<bool>();
        (void)bool_val;
    } catch (...) {}
    
    try {
        auto double_val = val.Get<double>();
        (void)double_val;
    } catch (...) {}
    
    try {
        auto int_val = val.Get<int>();
        (void)int_val;
    } catch (...) {}
    
    try {
        auto string_val = val.Get<std::string>();
        (void)string_val;
    } catch (...) {}
    
    try {
        auto binary_val = val.Get<std::vector<unsigned char>>();
        (void)binary_val;
    } catch (...) {}
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Test with the provided data
    testValueFunctions(Data, Size);
    
    // Return 0 indicates successful execution
    return 0;
}