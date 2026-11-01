// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
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
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <string>
#include <array>

static tinygltf::Value CreateValueFromFuzzData(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return tinygltf::Value();
    }

    uint8_t typeSelector = Data[0] % 7;

    switch (typeSelector) {
        case 0: // INT_TYPE
            if (Size >= 5) {
                int32_t intVal = 0;
                std::memcpy(&intVal, Data + 1, 4);
                return tinygltf::Value(static_cast<int>(intVal));
            }
            return tinygltf::Value(0);

        case 1: // REAL_TYPE
            if (Size >= 9) {
                double realVal = 0.0;
                std::memcpy(&realVal, Data + 1, 8);
                return tinygltf::Value(realVal);
            }
            return tinygltf::Value(0.0);

        case 2: // BOOL_TYPE
            if (Size >= 2) {
                return tinygltf::Value(Data[1] != 0);
            }
            return tinygltf::Value(false);

        case 3: // STRING_TYPE
            if (Size >= 2) {
                size_t len = Size - 1;
                std::string str(reinterpret_cast<const char*>(Data + 1), len);
                return tinygltf::Value(str);
            }
            return tinygltf::Value("");

        case 4: // BINARY_TYPE
            if (Size >= 2) {
                std::vector<unsigned char> binData(Data + 1, Data + Size);
                return tinygltf::Value(std::move(binData));
            }
            return tinygltf::Value(std::vector<unsigned char>());

        case 5: // ARRAY_TYPE
            {
                std::vector<tinygltf::Value> arrVec;
                if (Size > 1) {
                    // Recursively create a few values if data permits
                    size_t chunkSize = Size / 2;
                    if (chunkSize > 0) {
                        arrVec.push_back(CreateValueFromFuzzData(Data + 1, chunkSize));
                        arrVec.push_back(CreateValueFromFuzzData(Data + 1 + chunkSize, Size - 1 - chunkSize));
                    }
                }
                return tinygltf::Value(arrVec);
            }

        case 6: // OBJECT_TYPE
            {
                std::map<std::string, tinygltf::Value> objMap;
                if (Size > 1) {
                    std::string key = "key";
                    objMap[key] = CreateValueFromFuzzData(Data + 1, Size - 1);
                }
                return tinygltf::Value(objMap);
            }

        default:
            return tinygltf::Value();
    }
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a Value instance based on fuzzing data
    tinygltf::Value val = CreateValueFromFuzzData(Data, Size);

    // Exercise the target API functions
    // 1. IsBinary
    bool isBinary = val.IsBinary();
    (void)isBinary;

    // 2. IsReal
    bool isReal = val.IsReal();
    (void)isReal;

    // 3. IsString
    bool isString = val.IsString();
    (void)isString;

    // 4. IsBool
    bool isBool = val.IsBool();
    (void)isBool;

    // 5. IsNumber
    bool isNumber = val.IsNumber();
    (void)isNumber;

    // 6. IsInt
    bool isInt = val.IsInt();
    (void)isInt;

    // Additional exploration: Check consistency
    if (val.IsNumber()) {
        if (val.IsInt()) {
            // Safe to cast to int
            (void)val.Get<int>();
        } else if (val.IsReal()) {
            // Safe to cast to double
            (void)val.Get<double>();
        }
    }

    if (val.IsString()) {
        (void)val.Get<std::string>();
    }

    if (val.IsBinary()) {
        (void)val.Get<std::vector<unsigned char>>();
    }

    if (val.IsBool()) {
        (void)val.Get<bool>();
    }

    return 0;
}