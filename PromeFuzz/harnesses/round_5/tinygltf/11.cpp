// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
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

static void testValueFunctions(const tinygltf::Value& value, const std::string& key, int index) {
    // Test IsBinary
    bool isBinary = value.IsBinary();
    
    // Test Has - safe to call on any type
    bool hasKey = value.Has(key);
    
    // Test IsBool
    bool isBool = value.IsBool();
    
    // Test Get overloads - only call when appropriate
    // Get by key - only for objects
    if (value.IsObject()) {
        const tinygltf::Value& valByKey = value.Get(key);
    }
    
    // Get by index - only for arrays
    if (value.IsArray()) {
        const tinygltf::Value& valByIndex = value.Get(index);
    }
    
    // Test IsString
    bool isString = value.IsString();
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Use first byte to determine Value type
    uint8_t typeSelector = Data[0] % 8;
    
    // Use remaining data for content
    const uint8_t* contentData = Data + 1;
    size_t contentSize = Size - 1;
    
    tinygltf::Value value;
    
    switch (typeSelector) {
        case 0: // Null type (default)
            // value already null
            break;
            
        case 1: // Binary type
            if (contentSize > 0) {
                std::vector<unsigned char> binaryData(contentData, contentData + contentSize);
                value = tinygltf::Value(std::move(binaryData));
            }
            break;
            
        case 2: // Bool type
            value = tinygltf::Value(contentSize % 2 == 0);
            break;
            
        case 3: // Int type
            value = tinygltf::Value(static_cast<int>(contentSize));
            break;
            
        case 4: // Real type (double)
            value = tinygltf::Value(static_cast<double>(contentSize));
            break;
            
        case 5: // String type
            if (contentSize > 0) {
                std::string str(reinterpret_cast<const char*>(contentData), 
                               std::min(contentSize, static_cast<size_t>(100)));
                value = tinygltf::Value(str);
            } else {
                value = tinygltf::Value("");
            }
            break;
            
        case 6: // Array type
            {
                tinygltf::Value::Array array;
                // Add some dummy values to the array
                array.push_back(tinygltf::Value(1));
                array.push_back(tinygltf::Value(true));
                array.push_back(tinygltf::Value("test"));
                value = tinygltf::Value(std::move(array));
            }
            break;
            
        case 7: // Object type
            {
                tinygltf::Value::Object obj;
                // Add some key-value pairs
                obj["key1"] = tinygltf::Value(42);
                obj["key2"] = tinygltf::Value("value");
                obj["key3"] = tinygltf::Value(true);
                value = tinygltf::Value(std::move(obj));
            }
            break;
    }
    
    // Generate test key from data
    std::string testKey;
    if (contentSize > 0) {
        size_t keyLength = std::min(contentSize, static_cast<size_t>(10));
        testKey = std::string(reinterpret_cast<const char*>(contentData), keyLength);
    } else {
        testKey = "testKey";
    }
    
    // Generate test index from data
    int testIndex = static_cast<int>(contentSize % 100);
    
    // Test the value with our functions
    testValueFunctions(value, testKey, testIndex);
    
    return 0;
}