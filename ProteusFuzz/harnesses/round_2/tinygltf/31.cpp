// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
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
#include "tiny_gltf.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <array>

static tinygltf::Value CreateValueFromFuzzData(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return tinygltf::Value();
    }

    uint8_t typeSelector = Data[0] % 8;
    tinygltf::Value val;

    switch (typeSelector) {
        case 0: // Null
            val = tinygltf::Value();
            break;
        case 1: // Bool
            if (Size >= 2) {
                val = tinygltf::Value(Data[1] % 2 != 0);
            } else {
                val = tinygltf::Value(false);
            }
            break;
        case 2: // Int
            {
                int intVal = 0;
                if (Size >= 5) {
                    std::memcpy(&intVal, Data + 1, sizeof(int));
                }
                val = tinygltf::Value(intVal);
            }
            break;
        case 3: // Real
            {
                double realVal = 0.0;
                if (Size >= 9) {
                    std::memcpy(&realVal, Data + 1, sizeof(double));
                }
                val = tinygltf::Value(realVal);
            }
            break;
        case 4: // String
            {
                size_t len = 0;
                if (Size > 1) {
                    len = Data[1] % (Size - 1);
                }
                if (Size > len + 1) {
                    std::string str(reinterpret_cast<const char*>(Data + 1), len);
                    val = tinygltf::Value(str);
                } else {
                    val = tinygltf::Value(std::string(""));
                }
            }
            break;
        case 5: // Array
            {
                tinygltf::Value::Array arr;
                size_t count = 0;
                if (Size > 1) {
                    count = Data[1] % 10;
                }
                for (size_t i = 0; i < count; ++i) {
                    int itemVal = 0;
                    size_t offset = 2 + i * 4;
                    if (Size > offset + sizeof(int)) {
                        std::memcpy(&itemVal, Data + offset, sizeof(int));
                    }
                    arr.push_back(tinygltf::Value(itemVal));
                }
                val = tinygltf::Value(std::move(arr));
            }
            break;
        case 6: // Object
            {
                tinygltf::Value::Object obj;
                size_t count = 0;
                if (Size > 1) {
                    count = Data[1] % 5;
                }
                for (size_t i = 0; i < count; ++i) {
                    std::string key = "key_" + std::to_string(i);
                    int valInt = 0;
                    size_t offset = 2 + i * 4;
                    if (Size > offset + sizeof(int)) {
                        std::memcpy(&valInt, Data + offset, sizeof(int));
                    }
                    obj[key] = tinygltf::Value(valInt);
                }
                val = tinygltf::Value(std::move(obj));
            }
            break;
        case 7: // Binary
            {
                size_t len = 0;
                if (Size > 1) {
                    len = Data[1] % (Size - 1);
                }
                if (Size > len + 1) {
                    std::vector<unsigned char> binData(Data + 1, Data + 1 + len);
                    val = tinygltf::Value(std::move(binData));
                } else {
                    val = tinygltf::Value(std::vector<unsigned char>());
                }
            }
            break;
    }
    return val;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a base value from fuzz data
    tinygltf::Value val = CreateValueFromFuzzData(Data, Size);

    // Test IsString
    bool isStr = val.IsString();
    (void)isStr;

    // Test IsNumber
    bool isNum = val.IsNumber();
    (void)isNum;

    // Test IsInt
    bool isInt = val.IsInt();
    (void)isInt;

    // Test IsBool
    bool isBool = val.IsBool();
    (void)isBool;

    // Test Get (Array access)
    // We only call Get if the value is actually an array to avoid assertion failure
    if (val.IsArray()) {
        size_t indexToGet = 0;
        if (Size > 2) {
            indexToGet = Data[2] % 10;
        }
        // Get returns a reference, we just check if it's valid or null
        // Note: tinygltf::Value::Get asserts IsArray() internally, so we guard it here.
        const tinygltf::Value &elem = val.Get(indexToGet);
        (void)elem;
    }

    // Test Has (Object access)
    std::string keyToCheck;
    if (Size > 3) {
        size_t keyLen = Data[3] % 10;
        if (Size > 4 + keyLen) {
            keyToCheck.assign(reinterpret_cast<const char*>(Data + 4), keyLen);
        } else {
            keyToCheck = "test_key";
        }
    } else {
        keyToCheck = "test_key";
    }
    bool hasKey = val.Has(keyToCheck);
    (void)hasKey;

    // Additional state exploration: Create a specific array and test Get bounds
    if (Size > 4) {
        tinygltf::Value::Array arr;
        int count = Data[4] % 5;
        for(int i=0; i<count; ++i) {
            arr.push_back(tinygltf::Value(i));
        }
        tinygltf::Value arrVal(std::move(arr));
        
        // Ensure we only call Get on the array type
        if (arrVal.IsArray()) {
            // Test out of bounds (tinygltf handles this safely by returning null value)
            size_t outOfBoundsIdx = count + 1;
            if (Size > 5) outOfBoundsIdx = Data[5] % 100;
            
            const tinygltf::Value &outElem = arrVal.Get(outOfBoundsIdx);
            (void)outElem;
        }
    }

    return 0;
}