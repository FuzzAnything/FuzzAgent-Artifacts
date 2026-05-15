// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
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

static void testValueFunctions(const uint8_t *Data, size_t Size) {
    // Create a Value object using the binary constructor
    tinygltf::Value binaryValue(Data, Size);
    
    // Test IsArray on binary value (should be false)
    (void)binaryValue.IsArray();
    
    // Test IsBool on binary value (should be false)
    (void)binaryValue.IsBool();
    
    // Test IsString on binary value (should be false)
    (void)binaryValue.IsString();
    
    // Test IsObject on binary value (should be false)
    (void)binaryValue.IsObject();
    
    // Create other types of Value objects to test more thoroughly
    // Boolean value
    tinygltf::Value boolValue(true);
    (void)boolValue.IsBool();
    (void)boolValue.IsArray();
    (void)boolValue.IsString();
    (void)boolValue.IsObject();
    
    // String value
    std::string strData;
    if (Size > 0) {
        size_t strLen = Size % 100;
        if (strLen > 0) {
            strData = std::string(reinterpret_cast<const char*>(Data), strLen);
        }
    }
    tinygltf::Value stringValue(strData);
    (void)stringValue.IsString();
    (void)stringValue.IsBool();
    (void)stringValue.IsArray();
    (void)stringValue.IsObject();
    
    // Array value
    tinygltf::Value::Array array;
    if (Size > 0) {
        // Add some elements to the array
        array.push_back(tinygltf::Value(true));
        array.push_back(tinygltf::Value(static_cast<int>(Data[0])));
        if (Size > 1) {
            std::string elemStr;
            size_t elemLen = Size % 50;
            if (elemLen > 0) {
                elemStr = std::string(reinterpret_cast<const char*>(Data), elemLen);
            }
            array.push_back(tinygltf::Value(elemStr));
        }
    }
    tinygltf::Value arrayValue(array);
    
    (void)arrayValue.IsArray();
    (void)arrayValue.IsBool();
    (void)arrayValue.IsString();
    (void)arrayValue.IsObject();
    
    // Test Get on array - only if it's actually an array
    if (arrayValue.IsArray() && !array.empty()) {
        (void)arrayValue.Get(0);  // Valid index
        // Test out of bounds only if we have an array
        if (array.size() < 100) {
            (void)arrayValue.Get(100); // Out of bounds
        }
    }
    
    // Object value
    tinygltf::Value::Object obj;
    if (Size > 0) {
        // Create some keys from the fuzz data
        std::string key1 = "key1";
        std::string key2 = "key2";
        std::string key3;
        size_t key3Len = Size % 20;
        if (key3Len > 0) {
            key3 = std::string(reinterpret_cast<const char*>(Data), key3Len);
        } else {
            key3 = "key3";
        }
        
        obj[key1] = tinygltf::Value(true);
        obj[key2] = tinygltf::Value(static_cast<int>(Data[0] % 256));
        obj[key3] = tinygltf::Value(strData);
    }
    tinygltf::Value objectValue(obj);
    
    (void)objectValue.IsObject();
    (void)objectValue.IsArray();
    (void)objectValue.IsBool();
    (void)objectValue.IsString();
    
    // Test Has on object
    if (objectValue.IsObject()) {
        (void)objectValue.Has("key1");
        (void)objectValue.Has("key2");
        (void)objectValue.Has("nonexistent");
        
        // Test Get on object - only if it's actually an object
        if (obj.find("key1") != obj.end()) {
            (void)objectValue.Get("key1");
        }
        (void)objectValue.Get("nonexistent_key");
    }
    
    // Do NOT call Get on non-array/non-object types to avoid asserts
    // The library uses asserts to check types before accessing
    // We should only call Get when we know the type is correct
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    try {
        testValueFunctions(Data, Size);
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}