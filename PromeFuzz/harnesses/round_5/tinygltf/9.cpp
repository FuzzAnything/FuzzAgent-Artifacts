// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <map>
#include <array>

static void fuzzValueFunctions(const uint8_t* data, size_t size) {
    // Create a Value object with different types to test the functions
    // We'll create multiple Value objects with different internal types
    
    // 1. Create an object type Value
    tinygltf::Value::Object obj;
    
    // Use fuzzer data to create some key-value pairs if we have enough data
    if (size > 0) {
        // Create a simple key from the first byte
        std::string key1 = "key" + std::to_string(data[0] % 10);
        
        // Create different types of values to put in the object
        tinygltf::Value intVal(static_cast<int>(data[0]));
        tinygltf::Value boolVal((data[0] & 1) == 1);
        tinygltf::Value doubleVal(static_cast<double>(data[0]));
        tinygltf::Value stringVal("test" + std::to_string(data[0]));
        
        obj[key1] = intVal;
        
        // Add more keys if we have more data
        if (size > 1) {
            std::string key2 = "key" + std::to_string(data[1] % 10);
            obj[key2] = boolVal;
        }
    }
    
    tinygltf::Value objectValue(obj);
    
    // 2. Create an array type Value
    tinygltf::Value::Array arr;
    if (size > 0) {
        for (size_t i = 0; i < std::min(size, static_cast<size_t>(5)); i++) {
            arr.push_back(tinygltf::Value(static_cast<int>(data[i])));
        }
    }
    tinygltf::Value arrayValue(arr);
    
    // 3. Create a string type Value
    std::string strData;
    if (size > 0) {
        strData = std::string(reinterpret_cast<const char*>(data), 
                             std::min(size, static_cast<size_t>(100)));
    }
    tinygltf::Value stringValue(strData);
    
    // Test IsArray() function
    bool isObjectArray = objectValue.IsArray();
    bool isArrayArray = arrayValue.IsArray();
    bool isStringArray = stringValue.IsArray();
    
    // Test IsString() function
    bool isObjectString = objectValue.IsString();
    bool isArrayString = arrayValue.IsString();
    bool isStringString = stringValue.IsString();
    
    // Test IsObject() function
    bool isObjectObject = objectValue.IsObject();
    bool isArrayObject = arrayValue.IsObject();
    bool isStringObject = stringValue.IsObject();
    
    // Test Has() function - only valid for objects
    if (objectValue.IsObject()) {
        if (size > 0) {
            std::string testKey = "key" + std::to_string(data[0] % 10);
            bool hasKey = objectValue.Has(testKey);
            
            // Test with a key that likely doesn't exist
            bool hasNonExistentKey = objectValue.Has("nonexistent_key_12345");
        }
    }
    
    // Test Get() function overloads
    
    // Get by key - only valid for objects
    if (objectValue.IsObject() && size > 0) {
        std::string testKey = "key" + std::to_string(data[0] % 10);
        const tinygltf::Value& retrievedValue = objectValue.Get(testKey);
        
        // Try to get a non-existent key
        const tinygltf::Value& nonExistentValue = objectValue.Get("nonexistent_key_98765");
    }
    
    // Test templated Get() functions on different value types
    
    // On object value
    if (objectValue.IsObject()) {
        try {
            auto objMap = objectValue.Get<tinygltf::Value::Object>();
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // On array value
    if (arrayValue.IsArray()) {
        try {
            auto arrVec = arrayValue.Get<tinygltf::Value::Array>();
        } catch (...) {
            // Ignore exceptions
        }
        
        // Also test getting individual elements if array is not empty
        if (!arr.empty()) {
            try {
                int intElem = arrayValue.Get(0).Get<int>();
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // On string value
    if (stringValue.IsString()) {
        try {
            std::string str = stringValue.Get<std::string>();
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test Get() with different types on the same value
    if (size > 0) {
        // Create a simple int value
        tinygltf::Value intTestVal(static_cast<int>(data[0]));
        
        try {
            int intVal = intTestVal.Get<int>();
        } catch (...) {
            // Ignore exceptions
        }
        
        try {
            bool boolVal = intTestVal.Get<bool>();
        } catch (...) {
            // Ignore exceptions
        }
        
        try {
            double doubleVal = intTestVal.Get<double>();
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Create a binary value if we have data
    if (size > 0) {
        // Use the constructor that takes a pointer and size
        tinygltf::Value binaryValue(data, std::min(size, static_cast<size_t>(50)));
        
        try {
            auto bin = binaryValue.Get<std::vector<unsigned char>>();
        } catch (...) {
            // Ignore exceptions
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    try {
        fuzzValueFunctions(Data, Size);
    } catch (...) {
        // Catch all exceptions to prevent crashes from stopping fuzzing
    }
    
    return 0;
}