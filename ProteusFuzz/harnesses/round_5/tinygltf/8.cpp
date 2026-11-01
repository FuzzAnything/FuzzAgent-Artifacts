// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
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
#include <array>

static tinygltf::Value CreateValueFromFuzzData(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return tinygltf::Value(false);
    }

    uint8_t type = Data[0] % 8; // 0: BOOL, 1: REAL, 2: INT, 3: STRING, 4: BINARY, 5: ARRAY, 6: OBJECT, 7: NULL
    size_t offset = 1;
    size_t remaining = Size - offset;

    switch (type) {
        case 0: // BOOL
            return tinygltf::Value(remaining > 0 ? (Data[offset] % 2 == 1) : false);
        case 1: // REAL
            if (remaining >= sizeof(double)) {
                double val;
                std::memcpy(&val, &Data[offset], sizeof(double));
                return tinygltf::Value(val);
            }
            return tinygltf::Value(0.0);
        case 2: // INT
            if (remaining >= sizeof(int)) {
                int val;
                std::memcpy(&val, &Data[offset], sizeof(int));
                return tinygltf::Value(val);
            }
            return tinygltf::Value(0);
        case 3: // STRING
            if (remaining > 0) {
                size_t len = remaining;
                if (len > 1024) len = 1024; // Limit string size
                std::string s(reinterpret_cast<const char*>(&Data[offset]), len);
                return tinygltf::Value(s);
            }
            return tinygltf::Value(std::string());
        case 4: // BINARY
            if (remaining > 0) {
                size_t len = remaining;
                if (len > 1024) len = 1024;
                std::vector<unsigned char> bin(&Data[offset], &Data[offset] + len);
                // Use rvalue reference constructor for binary data
                return tinygltf::Value(std::move(bin));
            }
            return tinygltf::Value(std::vector<unsigned char>());
        case 5: // ARRAY
            {
                tinygltf::Value::Array arr;
                size_t count = remaining > 0 ? (Data[offset] % 10) + 1 : 0;
                offset++;
                remaining--;
                for (size_t i = 0; i < count && remaining > 0; ++i) {
                    arr.push_back(CreateValueFromFuzzData(&Data[offset], remaining));
                    size_t used = 1 + (remaining > 0 ? (Data[offset] % 10) + 1 : 0);
                    if (used > remaining) break;
                    offset += used;
                    remaining -= used;
                }
                return tinygltf::Value(arr);
            }
        case 6: // OBJECT
            {
                tinygltf::Value::Object obj;
                size_t count = remaining > 0 ? (Data[offset] % 5) + 1 : 0;
                offset++;
                remaining--;
                for (size_t i = 0; i < count && remaining > 0; ++i) {
                    std::string key = "key" + std::to_string(i);
                    obj[key] = CreateValueFromFuzzData(&Data[offset], remaining);
                    size_t used = 1 + (remaining > 0 ? (Data[offset] % 10) + 1 : 0);
                    if (used > remaining) break;
                    offset += used;
                    remaining -= used;
                }
                return tinygltf::Value(obj);
            }
        default: // NULL
            return tinygltf::Value();
    }
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a Value from fuzz data
    tinygltf::Value val = CreateValueFromFuzzData(Data, Size);

    // Test IsBinary
    bool isBinary = val.IsBinary();
    (void)isBinary;

    // Test IsObject
    bool isObject = val.IsObject();
    (void)isObject;

    // Test IsString
    bool isString = val.IsString();
    (void)isString;

    // Test IsInt
    bool isInt = val.IsInt();
    (void)isInt;

    // Test IsArray
    bool isArray = val.IsArray();
    (void)isArray;

    // Test Get on array if applicable
    if (val.IsArray()) {
        // Try to access a few indices based on input data
        size_t idx = 0;
        if (Size > 0) idx = Data[0] % 10;
        if (Size > 1) idx = (idx + Data[1]) % 10;
        
        const tinygltf::Value &elem = val.Get(idx);
        (void)elem;
        
        // Also try out of bounds
        // Use Get<tinygltf::Value::Array>() to access the underlying array to get size
        const auto &arr = val.Get<tinygltf::Value::Array>();
        size_t arrSize = arr.size();
        const tinygltf::Value &outOfBounds = val.Get(arrSize + 1);
        (void)outOfBounds;
    }

    // Additional checks to ensure types are correctly identified using Get<T>
    if (val.IsBinary()) {
        const auto &bin = val.Get<std::vector<unsigned char>>();
        (void)bin;
    }
    
    if (val.IsObject()) {
        const auto &obj = val.Get<tinygltf::Value::Object>();
        (void)obj;
    }

    if (val.IsString()) {
        const auto &str = val.Get<std::string>();
        (void)str;
    }

    if (val.IsInt()) {
        const int i = val.Get<int>();
        (void)i;
    }

    if (val.IsArray()) {
        const auto &arr = val.Get<tinygltf::Value::Array>();
        (void)arr;
    }

    return 0;
}