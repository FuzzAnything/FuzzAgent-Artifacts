// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <array>
#include "tiny_gltf.h"

// Helper function to generate a random Value based on input data
static tinygltf::Value CreateRandomValue(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return tinygltf::Value(); // Null value
    }

    uint8_t typeSelector = Data[0] % 5;
    size_t dataSize = (Size > 1) ? Data[1] % 100 : 0;
    
    // Ensure we don't access out of bounds for data content
    size_t contentStart = 2;
    size_t availableContent = (Size > 2) ? (Size - 2) : 0;
    size_t actualContent = (dataSize > availableContent) ? availableContent : dataSize;

    switch (typeSelector) {
        case 0: // Null
            return tinygltf::Value();
        case 1: // Number (Double)
            {
                // Construct a double from bytes safely
                double dVal = 0.0;
                if (actualContent >= 8) {
                    std::memcpy(&dVal, Data + contentStart, 8);
                } else if (actualContent > 0) {
                    // Fallback to simple integer interpretation if not enough bytes
                    dVal = static_cast<double>(Data[contentStart]);
                }
                return tinygltf::Value(dVal);
            }
        case 2: // String
            {
                std::string str;
                for (size_t i = 0; i < actualContent; ++i) {
                    // Avoid null terminators in middle to keep string valid
                    char c = static_cast<char>(Data[contentStart + i]);
                    if (c == '\0') c = ' ';
                    str += c;
                }
                return tinygltf::Value(str);
            }
        case 3: // Boolean
            {
                return tinygltf::Value((actualContent > 0) ? (Data[contentStart] % 2 == 0) : false);
            }
        case 4: // Array or Object (Randomly chosen)
            {
                if (Size > 3 && (Data[2] % 2 == 0)) {
                    // Create Array
                    tinygltf::Value::Array arr;
                    // Limit recursion depth/size to prevent stack overflow or memory issues
                    size_t count = (actualContent > 0) ? (actualContent % 50) : 0;
                    for (size_t i = 0; i < count; ++i) {
                        // Create simple scalar values for array elements to avoid deep recursion
                        arr.push_back(tinygltf::Value(static_cast<double>(i)));
                    }
                    return tinygltf::Value(arr);
                } else {
                    // Create Object
                    tinygltf::Value::Object obj;
                    size_t count = (actualContent > 0) ? (actualContent % 50) : 0;
                    for (size_t i = 0; i < count; ++i) {
                        std::string key = "key" + std::to_string(i);
                        obj[key] = tinygltf::Value(static_cast<double>(i));
                    }
                    return tinygltf::Value(obj);
                }
            }
    }
    return tinygltf::Value();
}

extern "C" int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    // Create a random Value instance to test against
    tinygltf::Value val = CreateRandomValue(Data, Size);

    // 2. & 3. Conduct tests and explore program states
    
    // Test IsObject
    bool isObj = val.IsObject();
    (void)isObj; // Suppress unused warning

    // Test IsArray
    bool isArray = val.IsArray();
    (void)isArray; // Suppress unused warning

    // Test ArrayLen
    size_t len = val.ArrayLen();
    (void)len; // Suppress unused warning

    // Test Size()
    // Note: Size() is undefined for scalars/nulls, but we call it to see behavior
    // In a real fuzzer, we might want to guard this, but the task says to explore states.
    // However, to avoid crashes on undefined behavior in release builds, 
    // we only call Size() if it's a container type, as per the description warning.
    if (val.IsArray() || val.IsObject()) {
        size_t sz = val.Size();
        (void)sz;
    }

    // Test Get(size_t) overload (Array access)
    if (val.IsArray()) {
        // Try accessing valid indices
        size_t arrLen = val.ArrayLen();
        for (size_t i = 0; i < arrLen; ++i) {
            const tinygltf::Value &elem = val.Get(i);
            // Just accessing it is enough for coverage
            (void)elem;
        }
        
        // Try accessing out of bounds (should return null safely)
        if (arrLen > 0) {
            const tinygltf::Value &outOfBounds = val.Get(arrLen);
            (void)outOfBounds;
        } else {
            const tinygltf::Value &outOfBounds = val.Get(0);
            (void)outOfBounds;
        }
    }

    // Test Get() for Object (Key access)
    // The prompt mentions "Invoke all 2 overloads". 
    // One is Get(size_t) for arrays. The other is Get(const std::string&) for objects.
    // The prompt snippet shows TINYGLTF_VALUE_GET(Value::Object, object_value_), 
    // which implies Get(const std::string&) exists.
    if (val.IsObject()) {
        // We need a key to test Get. Let's try a few common keys or derived from data.
        // Since we don't know the exact keys in the generated object, we try a few.
        // The generated object uses keys like "key0", "key1".
        for (size_t i = 0; i < 5; ++i) {
            std::string key = "key" + std::to_string(i);
            const tinygltf::Value &objVal = val.Get(key);
            (void)objVal;
        }
        
        // Try a non-existent key
        const tinygltf::Value &missing = val.Get("non_existent_key");
        (void)missing;
    }

    // 4. Cleanup is automatic via RAII for tinygltf::Value
    
    return 0;
}