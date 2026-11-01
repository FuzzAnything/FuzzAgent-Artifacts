// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
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
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <cstdlib>

static void FuzzValueFromData(const uint8_t *Data, size_t Size, tinygltf::Value &val) {
    if (Size == 0) return;

    uint8_t type = Data[0] % 7; // 0: Null, 1: Bool, 2: Int, 3: Real, 4: String, 5: Array, 6: Object

    switch (type) {
        case 0: // Null
            // Default constructor creates null
            break;
        case 1: // Bool
            val = tinygltf::Value((Size > 1) ? (Data[1] % 2 != 0) : true);
            break;
        case 2: // Int
            if (Size >= 5) {
                int32_t intVal = *reinterpret_cast<const int32_t*>(Data + 1);
                val = tinygltf::Value(intVal);
            } else {
                val = tinygltf::Value(0);
            }
            break;
        case 3: // Real
            if (Size >= 9) {
                double realVal = *reinterpret_cast<const double*>(Data + 1);
                val = tinygltf::Value(realVal);
            } else {
                val = tinygltf::Value(0.0);
            }
            break;
        case 4: // String
            if (Size > 1) {
                std::string str(reinterpret_cast<const char*>(Data + 1), Size - 1);
                val = tinygltf::Value(str);
            } else {
                val = tinygltf::Value(std::string("default"));
            }
            break;
        case 5: // Array
            {
                tinygltf::Value::Array arr;
                // Limit recursion depth and size to prevent OOM
                if (Size > 100) {
                    size_t count = (Size - 1) / 10;
                    if (count > 100) count = 100;
                    for (size_t i = 0; i < count; ++i) {
                        tinygltf::Value subVal;
                        // Recursively create sub-values with limited data
                        size_t subSize = (Size - 1) / count;
                        FuzzValueFromData(Data + 1 + (i * subSize), subSize, subVal);
                        arr.push_back(subVal);
                    }
                }
                val = tinygltf::Value(arr);
            }
            break;
        case 6: // Object
            {
                tinygltf::Value::Object obj;
                if (Size > 10) {
                    size_t count = (Size - 1) / 20;
                    if (count > 50) count = 50;
                    for (size_t i = 0; i < count; ++i) {
                        std::string key = "key_" + std::to_string(i);
                        tinygltf::Value subVal;
                        size_t subSize = (Size - 1) / count;
                        FuzzValueFromData(Data + 1 + (i * subSize), subSize, subVal);
                        obj[key] = subVal;
                    }
                }
                val = tinygltf::Value(obj);
            }
            break;
    }
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::Value val;
    FuzzValueFromData(Data, Size, val);

    // Test IsString
    bool isString = val.IsString();
    (void)isString;

    // Test ArrayLen
    size_t len = val.ArrayLen();
    (void)len;

    // Test IsNumber
    bool isNumber = val.IsNumber();
    (void)isNumber;

    // Test Size
    size_t size = val.Size();
    (void)size;

    // Test IsInt
    bool isInt = val.IsInt();
    (void)isInt;

    // Test Get with various types and indices
    // Only call Get if IsArray() is true to follow safety guidelines
    if (val.IsArray()) {
        size_t arrLen = val.ArrayLen();
        if (arrLen > 0) {
            // Access valid index
            tinygltf::Value v1 = val.Get(0);
            (void)v1;

            // Access out of bounds (should return null safely)
            tinygltf::Value v2 = val.Get(arrLen + 100);
            (void)v2;

            // Access with fuzzed index
            if (Size > 0) {
                size_t idx = Data[0] % (arrLen + 5);
                tinygltf::Value v3 = val.Get(idx);
                (void)v3;
            }
        }
    }

    return 0;
}