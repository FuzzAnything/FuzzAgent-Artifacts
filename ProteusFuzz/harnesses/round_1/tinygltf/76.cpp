// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <map>
#include <array>
#include "tiny_gltf.h"

static tinygltf::Value CreateValueFromFuzzData(const uint8_t *Data, size_t Size) {
    // Create a random object value to test Has, Keys, and Get
    tinygltf::Value::Object obj;
    
    // Parse fuzz data as key-value pairs to populate the object
    // We interpret chunks of data as keys and values to create diverse states
    size_t idx = 0;
    while (idx < Size) {
        // Generate a key string from data
        size_t key_len = (Size - idx > 10) ? (Data[idx] % 10) + 1 : (Size - idx);
        if (idx + key_len > Size) break;
        
        std::string key(reinterpret_cast<const char*>(&Data[idx]), key_len);
        idx += key_len;
        
        // Generate a value based on the next byte
        if (idx >= Size) break;
        uint8_t val_type = Data[idx];
        idx++;
        
        if (val_type == 0) {
            // Integer
            if (idx < Size) {
                obj[key] = tinygltf::Value(static_cast<int>(Data[idx]));
                idx++;
            }
        } else if (val_type == 1) {
            // Real (float)
            if (idx < Size) {
                obj[key] = tinygltf::Value(static_cast<double>(Data[idx]));
                idx++;
            }
        } else if (val_type == 2) {
            // String
            size_t str_len = (Size - idx > 20) ? (Data[idx] % 20) + 1 : (Size - idx);
            if (idx + str_len > Size) break;
            obj[key] = tinygltf::Value(std::string(reinterpret_cast<const char*>(&Data[idx]), str_len));
            idx += str_len;
        } else if (val_type == 3) {
            // Boolean
            obj[key] = tinygltf::Value(Data[idx] != 0);
            idx++;
        } else if (val_type == 4) {
            // Array
            size_t arr_len = (Size - idx > 10) ? (Data[idx] % 10) + 1 : (Size - idx);
            tinygltf::Value::Array arr;
            idx++;
            for (size_t i = 0; i < arr_len && idx < Size; ++i) {
                arr.push_back(tinygltf::Value(static_cast<int>(Data[idx])));
                idx++;
            }
            obj[key] = tinygltf::Value(arr);
        } else if (val_type == 5) {
            // Null
            obj[key] = tinygltf::Value();
        }
    }
    
    return tinygltf::Value(std::move(obj));
}

static tinygltf::Value CreateArrayValueFromFuzzData(const uint8_t *Data, size_t Size) {
    tinygltf::Value::Array arr;
    for (size_t i = 0; i < Size; ++i) {
        // Add various types to the array
        if (Data[i] % 3 == 0) {
            arr.push_back(tinygltf::Value(static_cast<int>(Data[i])));
        } else if (Data[i] % 3 == 1) {
            arr.push_back(tinygltf::Value(static_cast<double>(Data[i])));
        } else {
            arr.push_back(tinygltf::Value(std::string(1, static_cast<char>(Data[i]))));
        }
    }
    return tinygltf::Value(arr);
}

static tinygltf::Value CreatePrimitiveValueFromFuzzData(const uint8_t *Data, size_t Size) {
    if (Size == 0) return tinygltf::Value(); // Null
    
    uint8_t type = Data[0];
    if (type == 0) {
        return tinygltf::Value(static_cast<int>(Data[0]));
    } else if (type == 1) {
        return tinygltf::Value(static_cast<double>(Data[0]));
    } else if (type == 2) {
        return tinygltf::Value(std::string(reinterpret_cast<const char*>(Data), Size));
    } else if (type == 3) {
        return tinygltf::Value(Data[0] != 0);
    } else {
        return tinygltf::Value(); // Null
    }
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test 1: Create an Object Value and test Has, Keys, Type
    tinygltf::Value objVal = CreateValueFromFuzzData(Data, Size);
    
    // Test Type()
    char type = objVal.Type();
    (void)type; // Suppress unused variable warning
    
    // Test IsNumber(), IsInt() on object (should be false)
    bool isNum = objVal.IsNumber();
    bool isInt = objVal.IsInt();
    (void)isNum;
    (void)isInt;
    
    // Test Keys()
    std::vector<std::string> keys = objVal.Keys();
    
    // Test Has() with various keys derived from input
    for (size_t i = 0; i < Size && i < 100; ++i) {
        std::string testKey(reinterpret_cast<const char*>(&Data[i]), 1);
        bool hasKey = objVal.Has(testKey);
        (void)hasKey;
    }
    
    // Test 2: Create an Array Value and test Get, Type, IsNumber, IsInt
    tinygltf::Value arrVal = CreateArrayValueFromFuzzData(Data, Size);
    
    type = arrVal.Type();
    (void)type;
    
    isNum = arrVal.IsNumber();
    isInt = arrVal.IsInt();
    (void)isNum;
    (void)isInt;
    
    // Test Get() on array
    if (arrVal.IsArray()) {
        // Access array size directly or via GetArray if available, 
        // but tinygltf::Value doesn't expose GetArray() directly in all versions.
        // We rely on Get() for access and check IsArray() before.
        // To get size, we can try to access elements or use internal knowledge if needed.
        // However, for fuzzing, we just iterate based on input size or a safe limit.
        // Let's try to get elements up to a reasonable limit based on Size
        size_t max_index = (Size > 1000) ? 1000 : Size;
        for (size_t i = 0; i < max_index; ++i) {
            // Safe access via Get
            const tinygltf::Value &elem = arrVal.Get(i);
            (void)elem;
        }
        // Test out of bounds (should return null safely)
        const tinygltf::Value &outOfBounds = arrVal.Get(max_index + 100);
        (void)outOfBounds;
    }
    
    // Test 3: Create Primitive Values and test Type, IsNumber, IsInt
    tinygltf::Value primVal = CreatePrimitiveValueFromFuzzData(Data, Size);
    
    type = primVal.Type();
    (void)type;
    
    isNum = primVal.IsNumber();
    isInt = primVal.IsInt();
    (void)isNum;
    (void)isInt;
    
    // Test Has and Keys on non-object (should return false/empty)
    bool hasKeyOnPrim = primVal.Has("test");
    std::vector<std::string> keysOnPrim = primVal.Keys();
    (void)hasKeyOnPrim;
    (void)keysOnPrim;
    
    // Test Get on non-array (should be safe, but we check IsArray first as per instructions)
    if (primVal.IsArray()) {
        // This branch should rarely execute for primitives, but we handle it
        const tinygltf::Value &elem = primVal.Get(0);
        (void)elem;
    }
    
    return 0;
}