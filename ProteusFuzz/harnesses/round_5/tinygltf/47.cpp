// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <map>
#include "tiny_gltf.h"

static void FuzzValueFromData(const uint8_t *Data, size_t Size, tinygltf::Value &val) {
    if (Size == 0) {
        return;
    }

    // Use the first byte to determine the type of Value to construct
    uint8_t type = Data[0] % 6; // 0: Null, 1: Bool, 2: Int, 3: Real, 4: String, 5: Array, 6: Object

    switch (type) {
        case 0: // Null
            // Default constructor creates null
            break;
        case 1: // Bool
            if (Size >= 2) {
                val = tinygltf::Value(Data[1] % 2 != 0);
            } else {
                val = tinygltf::Value(true);
            }
            break;
        case 2: // Int
            if (Size >= 5) {
                int32_t intVal = 0;
                std::memcpy(&intVal, &Data[1], 4);
                val = tinygltf::Value(intVal);
            } else {
                val = tinygltf::Value(0);
            }
            break;
        case 3: // Real
            if (Size >= 9) {
                double realVal = 0.0;
                std::memcpy(&realVal, &Data[1], 8);
                val = tinygltf::Value(realVal);
            } else {
                val = tinygltf::Value(0.0);
            }
            break;
        case 4: // String
            {
                size_t len = 0;
                if (Size > 1) {
                    len = Data[1] % (Size - 1);
                }
                if (len > 0 && Size > 1 + len) {
                    std::string s(reinterpret_cast<const char*>(Data + 2), len);
                    val = tinygltf::Value(s);
                } else {
                    val = tinygltf::Value(std::string(""));
                }
            }
            break;
        case 5: // Array
            {
                std::vector<tinygltf::Value> arr;
                size_t count = 0;
                if (Size > 1) {
                    count = Data[1] % 20;
                }
                size_t offset = 2;
                for (size_t i = 0; i < count; ++i) {
                    if (offset < Size) {
                        // Recursively create simple values for array elements
                        uint8_t subType = Data[offset] % 3;
                        if (subType == 0) arr.push_back(tinygltf::Value(0));
                        else if (subType == 1) arr.push_back(tinygltf::Value(0.0));
                        else arr.push_back(tinygltf::Value(std::string("elem")));
                        offset++;
                    }
                }
                val = tinygltf::Value(arr);
            }
            break;
        case 6: // Object
            {
                std::map<std::string, tinygltf::Value> obj;
                size_t count = 0;
                if (Size > 1) {
                    count = Data[1] % 10;
                }
                size_t offset = 2;
                for (size_t i = 0; i < count; ++i) {
                    if (offset < Size) {
                        size_t keyLen = Data[offset] % 10;
                        std::string key;
                        if (offset + 1 + keyLen <= Size) {
                            key = std::string(reinterpret_cast<const char*>(Data + offset + 1), keyLen);
                            offset += 1 + keyLen;
                        } else {
                            key = "key";
                            offset += 1;
                        }
                        // Simple value for object: explicitly cast to int to avoid ambiguity
                        obj[key] = tinygltf::Value(static_cast<int>(i));
                    }
                }
                val = tinygltf::Value(obj);
            }
            break;
    }
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::Value val;
    FuzzValueFromData(Data, Size, val);

    // Test 1: IsArray
    bool isArray = val.IsArray();
    (void)isArray;

    // Test 2: Get (only safe if IsArray is true)
    // The tinygltf implementation asserts if the value is not an array.
    // We must check IsArray() before calling Get() to avoid the assert failure.
    if (isArray && Size > 1) {
        size_t index = Data[1] % 100;
        // Even if it is an array, the index might be out of bounds.
        // The Get function handles out-of-bounds by returning a null value safely,
        // but we rely on the internal logic of tinygltf for that.
        tinygltf::Value retrieved = val.Get(index);
        (void)retrieved;
    }

    // Test 3: IsReal
    bool isReal = val.IsReal();
    (void)isReal;

    // Test 4: IsInt
    bool isInt = val.IsInt();
    (void)isInt;

    // Test 5: IsString
    bool isString = val.IsString();
    (void)isString;

    // Test 6: Keys
    std::vector<std::string> keys = val.Keys();
    (void)keys;

    return 0;
}