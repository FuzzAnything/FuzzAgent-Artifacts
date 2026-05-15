// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
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
#include <cstring>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // Create a Value object from integer constructor
    int int_val = 0;
    if (Size >= sizeof(int)) {
        memcpy(&int_val, Data, sizeof(int));
    }
    tinygltf::Value val(int_val);
    
    // Test IsObject
    bool isObject = val.IsObject();
    
    // Test IsArray
    bool isArray = val.IsArray();
    
    // Test IsString
    bool isString = val.IsString();
    
    // Test Get functions - invoke two overloads
    // Get<int> should work since we constructed with int
    int int_get = val.Get<int>();
    
    // Get<std::string> - may return default string if not string type
    std::string string_get = val.Get<std::string>();
    
    // Test Has function - need a key string
    // Create key from input data if available
    std::string key;
    if (Size > sizeof(int)) {
        size_t key_len = std::min(Size - sizeof(int), static_cast<size_t>(100));
        key.assign(reinterpret_cast<const char*>(Data + sizeof(int)), key_len);
    } else {
        key = "test_key";
    }
    
    bool hasKey = val.Has(key);
    
    // Try to create different Value types to explore more states
    // Create another Value with different data if we have enough input
    if (Size >= 2 * sizeof(int)) {
        int int_val2 = 0;
        memcpy(&int_val2, Data + sizeof(int), sizeof(int));
        tinygltf::Value val2(int_val2);
        
        // Test functions on second value
        bool isObject2 = val2.IsObject();
        bool isArray2 = val2.IsArray();
        bool isString2 = val2.IsString();
        
        // Test Get on second value
        int int_get2 = val2.Get<int>();
        std::string string_get2 = val2.Get<std::string>();
        
        // Test Has on second value
        std::string key2;
        if (Size > 2 * sizeof(int)) {
            size_t key_len2 = std::min(Size - 2 * sizeof(int), static_cast<size_t>(100));
            key2.assign(reinterpret_cast<const char*>(Data + 2 * sizeof(int)), key_len2);
        } else {
            key2 = "test_key2";
        }
        bool hasKey2 = val2.Has(key2);
    }
    
    return 0;
}