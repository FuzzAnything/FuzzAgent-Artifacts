// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
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

static tinygltf::Value CreateValueFromFuzzData(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return tinygltf::Value(0);
    }

    // Use the first byte to determine the type of Value to create
    uint8_t typeSelector = Data[0] % 8;

    switch (typeSelector) {
        case 0: // Null
            return tinygltf::Value();
        case 1: // Bool
            return tinygltf::Value(Size % 2 != 0);
        case 2: // Int
            if (Size >= 1 + sizeof(int)) {
                int val = 0;
                std::memcpy(&val, Data + 1, sizeof(int));
                return tinygltf::Value(val);
            }
            return tinygltf::Value(0);
        case 3: // Real (Double)
            if (Size >= 1 + sizeof(double)) {
                double val = 0.0;
                std::memcpy(&val, Data + 1, sizeof(double));
                return tinygltf::Value(val);
            }
            return tinygltf::Value(0.0);
        case 4: // String
            if (Size > 1) {
                std::string str(reinterpret_cast<const char*>(Data + 1), Size - 1);
                return tinygltf::Value(str);
            }
            return tinygltf::Value(std::string());
        case 5: // Array
            {
                tinygltf::Value::Array arr;
                // Create a small array based on remaining data
                if (Size > 1) {
                    int count = (Size - 1) % 5 + 1;
                    for (int i = 0; i < count && (size_t)(i + 1) < Size; ++i) {
                        arr.push_back(tinygltf::Value(Data[1 + i]));
                    }
                }
                return tinygltf::Value(arr);
            }
        case 6: // Object
            {
                tinygltf::Value::Object obj;
                if (Size > 1) {
                    int count = (Size - 1) % 3 + 1;
                    for (int i = 0; i < count && (size_t)(i + 1) < Size; ++i) {
                        std::string key = "key_" + std::to_string(i);
                        obj[key] = tinygltf::Value(Data[1 + i]);
                    }
                }
                return tinygltf::Value(obj);
            }
        case 7: // Binary
            {
                if (Size > 1) {
                    // Use the constructor that takes pointer and size
                    return tinygltf::Value(Data + 1, Size - 1);
                }
                // Empty binary: construct using pointer and size 0
                return tinygltf::Value(nullptr, 0);
            }
        default:
            return tinygltf::Value(0);
    }
}

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a Value based on fuzz data
    tinygltf::Value val = CreateValueFromFuzzData(Data, Size);

    // Test IsString
    bool isString = val.IsString();
    (void)isString; // Suppress unused warning

    // Test IsNumber
    bool isNumber = val.IsNumber();
    (void)isNumber;

    // Test IsInt
    bool isInt = val.IsInt();
    (void)isInt;

    // Test IsBool
    bool isBool = val.IsBool();
    (void)isBool;

    // Test Has (only valid for objects, but safe to call on others)
    std::string testKey = "test_key";
    bool hasKey = val.Has(testKey);
    (void)hasKey;

    // Test Get (only valid for arrays, but safe to call on others due to internal checks)
    // We try to access index 0 if the value is an array
    if (val.IsArray()) {
        const tinygltf::Value &elem = val.Get(0);
        (void)elem;
        
        // Try accessing an out of bounds index if array is small
        const tinygltf::Value &outOfBounds = val.Get(1000);
        (void)outOfBounds;
    }

    // Additional diverse invocations to explore states
    // Create a second value to test interactions or repeated calls
    tinygltf::Value val2 = CreateValueFromFuzzData(Data, Size);
    
    // Chain checks
    if (val2.IsNumber()) {
        if (val2.IsInt()) {
            // Logic path for integer
        } else {
            // Logic path for real
        }
    } else if (val2.IsString()) {
        // Logic path for string
    } else if (val2.IsBool()) {
        // Logic path for bool
    }

    return 0;
}