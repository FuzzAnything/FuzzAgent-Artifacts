// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
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
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <array>
#include <limits>

static void FuzzValueFromData(const uint8_t *Data, size_t Size, tinygltf::Value &val) {
    if (Size == 0) return;

    uint8_t typeSelector = Data[0] % 7; // 0: Null, 1: Bool, 2: Int, 3: Real, 4: String, 5: Array, 6: Object

    switch (typeSelector) {
        case 0: // Null
            val = tinygltf::Value();
            break;
        case 1: // Bool
            val = tinygltf::Value(Size > 1 ? (Data[1] % 2 != 0) : true);
            break;
        case 2: // Int
            if (Size >= 5) {
                int32_t intVal = 0;
                std::memcpy(&intVal, Data + 1, sizeof(int32_t));
                val = tinygltf::Value(intVal);
            } else {
                val = tinygltf::Value(0);
            }
            break;
        case 3: // Real
            if (Size >= 9) {
                double realVal = 0.0;
                std::memcpy(&realVal, Data + 1, sizeof(double));
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
                val = tinygltf::Value("");
            }
            break;
        case 5: // Array
            {
                tinygltf::Value::Array arr;
                if (Size > 1) {
                    size_t count = Data[1] % 10; // Limit array size for safety
                    for (size_t i = 0; i < count && (1 + 1 + i * 2) < Size; ++i) {
                        uint8_t subType = Data[2 + i * 2] % 2;
                        if (subType == 0) {
                            arr.push_back(tinygltf::Value(0));
                        } else {
                            arr.push_back(tinygltf::Value(0.0));
                        }
                    }
                }
                val = tinygltf::Value(std::move(arr));
            }
            break;
        case 6: // Object
            {
                tinygltf::Value::Object obj;
                if (Size > 1) {
                    size_t count = Data[1] % 5;
                    for (size_t i = 0; i < count && (1 + 1 + i * 2) < Size; ++i) {
                        std::string key = "key_" + std::to_string(i);
                        uint8_t valType = Data[2 + i * 2] % 2;
                        if (valType == 0) {
                            obj[key] = tinygltf::Value(0);
                        } else {
                            obj[key] = tinygltf::Value(0.0);
                        }
                    }
                }
                val = tinygltf::Value(std::move(obj));
            }
            break;
    }
}

extern "C" int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::Value val;
    FuzzValueFromData(Data, Size, val);

    // Test IsReal
    bool isReal = val.IsReal();
    (void)isReal;

    // Test IsInt
    bool isInt = val.IsInt();
    (void)isInt;

    // Test GetNumberAsDouble (Only if numeric to avoid UB, though fuzzer might hit UB if logic is flawed, 
    // but the prompt says "ensure type is numeric first". We will check types before calling to be safe 
    // and demonstrate correct usage, but also try to call it blindly if the fuzzer wants to test robustness?
    // The prompt says: "Non-number types yield undefined behavior; ensure type is numeric first."
    // To be a good security test, we should follow the rule, but we can also test the state where we check.
    // Let's strictly follow the "ensure type" rule for safety in the driver, but vary the checks.
    if (val.IsReal() || val.IsInt()) {
        double num = val.GetNumberAsDouble();
        (void)num;
    }

    // Test IsObject
    bool isObj = val.IsObject();
    (void)isObj;

    // Test IsArray
    bool isArray = val.IsArray();
    (void)isArray;

    // Test Get(size_t)
    // "Before invoking Get(index), explicitly check if the Value is an array"
    if (isArray) {
        // Use data to determine index
        size_t index = 0;
        if (Size > 2) {
            index = (Data[1] + Data[2]) % 100;
        }
        const tinygltf::Value &elem = val.Get(index);
        (void)elem;
        
        // Also test out of bounds if array is small
        if (Size > 3) {
            size_t largeIndex = (Data[3] * 1000) + 1000;
            const tinygltf::Value &elem2 = val.Get(largeIndex);
            (void)elem2;
        }
    }

    return 0;
}