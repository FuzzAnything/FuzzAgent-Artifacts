// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
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
#include <algorithm>
#include "tiny_gltf.h"

static tinygltf::Value CreateValueFromFuzzData(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return tinygltf::Value();
    }

    uint8_t typeSelector = Data[0];
    size_t offset = 1;
    size_t remaining = Size - 1;

    switch (typeSelector % 7) {
        case 0: // INT_TYPE
            if (remaining >= sizeof(int)) {
                int val = 0;
                std::memcpy(&val, Data + offset, sizeof(int));
                return tinygltf::Value(val);
            }
            return tinygltf::Value(0);

        case 1: // REAL_TYPE
            if (remaining >= sizeof(double)) {
                double val = 0.0;
                std::memcpy(&val, Data + offset, sizeof(double));
                return tinygltf::Value(val);
            }
            return tinygltf::Value(0.0);

        case 2: // BOOL_TYPE
            {
                bool val = (remaining > 0 && Data[offset] != 0);
                return tinygltf::Value(val);
            }

        case 3: // STRING_TYPE
            {
                size_t strLen = 0;
                if (remaining > 0) {
                    const char* start = reinterpret_cast<const char*>(Data + offset);
                    const char* end = std::find(start, start + remaining, '\0');
                    strLen = static_cast<size_t>(end - start);
                }
                return tinygltf::Value(std::string(reinterpret_cast<const char*>(Data + offset), strLen));
            }

        case 4: // BINARY_TYPE
            {
                if (remaining > 0) {
                    return tinygltf::Value(Data + offset, remaining);
                }
                const unsigned char* empty_ptr = nullptr;
                return tinygltf::Value(empty_ptr, 0);
            }

        case 5: // ARRAY_TYPE
            {
                tinygltf::Value::Array arr;
                if (remaining >= sizeof(size_t)) {
                    size_t count = 0;
                    std::memcpy(&count, Data + offset, sizeof(size_t));
                    offset += sizeof(size_t);
                    remaining -= sizeof(size_t);
                    
                    if (count > 100) count = 100;
                    
                    for (size_t i = 0; i < count && remaining > 0; ++i) {
                        if (Data[offset] % 2 == 0) {
                            arr.push_back(tinygltf::Value(static_cast<int>(Data[offset])));
                        } else {
                            arr.push_back(tinygltf::Value(std::string("item")));
                        }
                        offset++;
                        remaining--;
                    }
                }
                return tinygltf::Value(arr);
            }

        case 6: // OBJECT_TYPE
            {
                tinygltf::Value::Object obj;
                if (remaining >= sizeof(size_t)) {
                    size_t count = 0;
                    std::memcpy(&count, Data + offset, sizeof(size_t));
                    offset += sizeof(size_t);
                    remaining -= sizeof(size_t);
                    
                    if (count > 50) count = 50;
                    
                    for (size_t i = 0; i < count && remaining > 0; ++i) {
                        size_t keyLen = 0;
                        if (remaining > 0) {
                            keyLen = Data[offset] % 20;
                            offset++;
                            remaining--;
                        }
                        if (remaining >= keyLen) {
                            std::string key(reinterpret_cast<const char*>(Data + offset), keyLen);
                            offset += keyLen;
                            remaining -= keyLen;
                            
                            if (remaining > 0) {
                                int val = Data[offset];
                                offset++;
                                remaining--;
                                obj[key] = tinygltf::Value(val);
                            }
                        }
                    }
                }
                return tinygltf::Value(obj);
            }

        default:
            return tinygltf::Value();
    }
}

extern "C" int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::Value val = CreateValueFromFuzzData(Data, Size);

    // Test IsNumber
    bool isNum = val.IsNumber();
    (void)isNum;

    // Test IsBool
    bool isBool = val.IsBool();
    (void)isBool;

    // Test IsString
    bool isStr = val.IsString();
    (void)isStr;

    // Test IsInt
    bool isInt = val.IsInt();
    (void)isInt;

    // Test Has (only meaningful for objects)
    if (val.IsObject()) {
        std::string key;
        if (Size > 1) {
            key = std::string(reinterpret_cast<const char*>(Data + 1), Size > 2 ? Size - 1 : 1);
        }
        bool hasKey = val.Has(key);
        (void)hasKey;
    }

    // Test Get (only meaningful for arrays)
    if (val.IsArray()) {
        size_t index = 0;
        if (Size > 0) {
            index = Data[0] % 100;
        }
        
        tinygltf::Value retrieved = val.Get(index);
        (void)retrieved;
        
        if (retrieved.IsInt()) {
            int intVal = retrieved.Get<int>();
            (void)intVal;
        }
        if (retrieved.IsNumber()) {
            double realVal = retrieved.Get<double>();
            (void)realVal;
        }
        if (retrieved.IsString()) {
            std::string strVal = retrieved.Get<std::string>();
            (void)strVal;
        }
        if (retrieved.IsBool()) {
            bool boolVal = retrieved.Get<bool>();
            (void)boolVal;
        }
    }

    return 0;
}