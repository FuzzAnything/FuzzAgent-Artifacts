// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
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
#include <cstring>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Use a small portion of data to determine test parameters
    if (Size < 1) return 0;
    
    // Create different types of Value objects to test the functions
    uint8_t test_type = Data[0] % 7;
    tinygltf::Value val;
    
    // Initialize Value based on test_type
    switch (test_type) {
        case 0:
            // Null value
            val = tinygltf::Value();
            break;
        case 1:
            // Boolean value
            val = tinygltf::Value(true);
            break;
        case 2:
            // Integer value
            val = tinygltf::Value(static_cast<int>(Size));
            break;
        case 3:
            // Real value
            val = tinygltf::Value(static_cast<double>(Size));
            break;
        case 4:
            // String value
            if (Size > 1) {
                std::string str(reinterpret_cast<const char*>(Data + 1), 
                               std::min(Size - 1, static_cast<size_t>(100)));
                val = tinygltf::Value(str);
            } else {
                val = tinygltf::Value("test");
            }
            break;
        case 5:
            // Array value
            {
                tinygltf::Value::Array arr;
                arr.push_back(tinygltf::Value(1));
                arr.push_back(tinygltf::Value(2));
                arr.push_back(tinygltf::Value(3));
                val = tinygltf::Value(arr);
            }
            break;
        case 6:
            // Object value
            {
                tinygltf::Value::Object obj;
                obj["key1"] = tinygltf::Value(1);
                obj["key2"] = tinygltf::Value("value");
                val = tinygltf::Value(obj);
            }
            break;
    }
    
    // Test IsObject function
    bool isObject = val.IsObject();
    
    // Test IsArray function  
    bool isArray = val.IsArray();
    
    // Test IsString function
    bool isString = val.IsString();
    
    // Test Get function for different types
    // For int type
    if (val.IsInt()) {
        int intVal = val.Get<int>();
    }
    
    // For string type
    if (val.IsString()) {
        std::string strVal = val.Get<std::string>();
    }
    
    // For array type
    if (val.IsArray()) {
        tinygltf::Value::Array arrVal = val.Get<tinygltf::Value::Array>();
    }
    
    // For object type
    if (val.IsObject()) {
        tinygltf::Value::Object objVal = val.Get<tinygltf::Value::Object>();
    }
    
    // For bool type
    if (val.IsBool()) {
        bool boolVal = val.Get<bool>();
    }
    
    // For real type
    if (val.IsReal()) {
        double realVal = val.Get<double>();
    }
    
    // Test Has function (only meaningful for objects)
    if (val.IsObject()) {
        bool hasKey = val.Has("key1");
        bool hasNonExistent = val.Has("nonexistent");
    }
    
    return 0;
}