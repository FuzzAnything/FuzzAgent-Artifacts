// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
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
#include <sstream>

static bool ParseFuzzDataToValue(const uint8_t *Data, size_t Size, tinygltf::Value &outVal) {
    if (Size == 0) {
        // Default to null value
        outVal = tinygltf::Value();
        return true;
    }

    // Interpret the first byte to decide the type
    uint8_t typeByte = Data[0];
    size_t offset = 1;
    size_t remaining = Size - 1;

    switch (typeByte) {
        case 0: // Null
            outVal = tinygltf::Value();
            return true;
        case 1: // Bool
            if (remaining > 0) {
                outVal = tinygltf::Value(Data[offset] != 0);
            } else {
                outVal = tinygltf::Value(false);
            }
            return true;
        case 2: // Int
            if (remaining >= sizeof(int)) {
                int intVal;
                std::memcpy(&intVal, &Data[offset], sizeof(int));
                outVal = tinygltf::Value(intVal);
            } else {
                outVal = tinygltf::Value(0);
            }
            return true;
        case 3: // Real (Double)
            if (remaining >= sizeof(double)) {
                double doubleVal;
                std::memcpy(&doubleVal, &Data[offset], sizeof(double));
                outVal = tinygltf::Value(doubleVal);
            } else {
                outVal = tinygltf::Value(0.0);
            }
            return true;
        case 4: // String
            if (remaining > 0) {
                // Use remaining bytes as string content (may contain nulls, but std::string handles it)
                std::string str(reinterpret_cast<const char*>(&Data[offset]), remaining);
                outVal = tinygltf::Value(str);
            } else {
                outVal = tinygltf::Value(std::string());
            }
            return true;
        case 5: // Array
            {
                tinygltf::Value::Array arr;
                if (remaining > 0) {
                    // Simple heuristic: treat remaining bytes as count of elements
                    // If count is too large, cap it to avoid OOM
                    size_t count = static_cast<size_t>(Data[offset]);
                    if (count > 100) count = 100;
                    
                    for (size_t i = 0; i < count; ++i) {
                        // Recursively create values or just add nulls for simplicity in fuzzing
                        // To keep it simple and robust, we add random types based on next byte if available
                        size_t idx = offset + 1 + i;
                        if (idx < Size) {
                            uint8_t subType = Data[idx];
                            tinygltf::Value subVal;
                            if (subType == 0) subVal = tinygltf::Value();
                            else if (subType == 1) subVal = tinygltf::Value(true);
                            else if (subType == 2) subVal = tinygltf::Value(static_cast<int>(Data[idx]));
                            else if (subType == 3) subVal = tinygltf::Value(1.0);
                            else if (subType == 4) subVal = tinygltf::Value("sub");
                            else subVal = tinygltf::Value();
                            arr.push_back(subVal);
                        } else {
                            arr.push_back(tinygltf::Value());
                        }
                    }
                }
                outVal = tinygltf::Value(arr);
                return true;
            }
        case 6: // Object
            {
                tinygltf::Value::Object obj;
                // For objects, we just add a few dummy key-value pairs based on data
                if (remaining > 0) {
                    size_t count = static_cast<size_t>(Data[offset]);
                    if (count > 10) count = 10;
                    for (size_t i = 0; i < count; ++i) {
                        std::string key = "key_" + std::to_string(i);
                        tinygltf::Value val;
                        if (i % 2 == 0) val = tinygltf::Value(true);
                        else val = tinygltf::Value(42);
                        obj[key] = val;
                    }
                }
                outVal = tinygltf::Value(obj);
                return true;
            }
        default:
            // Fallback to null
            outVal = tinygltf::Value();
            return true;
    }
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::Value val;
    if (!ParseFuzzDataToValue(Data, Size, val)) {
        return 0;
    }

    // Fuzz IsString
    bool isStr = val.IsString();
    (void)isStr; // Use result to suppress warnings

    // Fuzz IsObject
    bool isObj = val.IsObject();
    (void)isObj;

    // Fuzz IsNumber
    bool isNum = val.IsNumber();
    (void)isNum;

    // Fuzz IsInt
    bool isInt = val.IsInt();
    (void)isInt;

    // Fuzz GetNumberAsDouble (only if number)
    if (isNum) {
        double d = val.GetNumberAsDouble();
        (void)d;
    }

    // Fuzz Get (only if array)
    if (val.IsArray()) {
        // Try accessing index 0
        // Note: Get() returns a const reference, so we must use const tinygltf::Value&
        const tinygltf::Value &elem = val.Get(0);
        (void)elem;

        // Try accessing a large index (should return null safely)
        const tinygltf::Value &elemOut = val.Get(static_cast<size_t>(-1));
        (void)elemOut;
    }

    return 0;
}