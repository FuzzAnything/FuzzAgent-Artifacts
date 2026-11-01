// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
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

static tinygltf::Value CreateRandomValue(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return tinygltf::Value(); // Null value
    }

    int typeChoice = Data[0] % 7;
    size_t offset = 1;
    size_t remaining = Size - offset;

    switch (typeChoice) {
        case 0: // Null
            return tinygltf::Value();
        case 1: // Bool
            if (remaining > 0) {
                return tinygltf::Value(Data[offset] % 2 != 0);
            }
            return tinygltf::Value(false);
        case 2: // Int
            if (remaining >= 4) {
                int val = 0;
                std::memcpy(&val, Data + offset, 4);
                return tinygltf::Value(val);
            }
            return tinygltf::Value(0);
        case 3: // Real (Double)
            if (remaining >= 8) {
                double val = 0.0;
                std::memcpy(&val, Data + offset, 8);
                return tinygltf::Value(val);
            }
            return tinygltf::Value(0.0);
        case 4: // String
            // Find a null terminator or use max length
            {
                size_t strLen = 0;
                while (offset + strLen < Size && Data[offset + strLen] != 0 && strLen < 1024) {
                    strLen++;
                }
                if (strLen > 0) {
                    std::string s(reinterpret_cast<const char*>(Data + offset), strLen);
                    return tinygltf::Value(s);
                }
                return tinygltf::Value(std::string());
            }
        case 5: // Binary
            // Use remaining data as binary
            {
                std::vector<unsigned char> binData;
                if (remaining > 0) {
                    binData.assign(Data + offset, Data + Size);
                }
                // tinygltf::Value requires rvalue for vector<unsigned char>
                return tinygltf::Value(std::move(binData));
            }
        case 6: // Array or Object (Recursive)
            if (remaining < 2) {
                return tinygltf::Value();
            }
            {
                bool isArray = (Data[offset] % 2) != 0;
                offset++;
                remaining--;
                
                // Determine size of array/object
                size_t count = 0;
                if (remaining >= 2) {
                    count = (Data[offset] * 256 + Data[offset+1]) % 10; // Limit to 10 elements to prevent stack overflow
                    offset += 2;
                    remaining -= 2;
                }

                if (isArray) {
                    tinygltf::Value::Array arr;
                    for (size_t i = 0; i < count && remaining > 0; ++i) {
                        arr.push_back(CreateRandomValue(Data + offset, remaining));
                        // Advance data pointer roughly
                        size_t step = (remaining > 10) ? 10 : remaining;
                        offset += step;
                        remaining -= step;
                    }
                    return tinygltf::Value(arr);
                } else {
                    tinygltf::Value::Object obj;
                    for (size_t i = 0; i < count && remaining > 0; ++i) {
                        // Generate a key
                        std::string key = "key_" + std::to_string(i);
                        obj[key] = CreateRandomValue(Data + offset, remaining);
                        size_t step = (remaining > 10) ? 10 : remaining;
                        offset += step;
                        remaining -= step;
                    }
                    return tinygltf::Value(obj);
                }
            }
    }
    return tinygltf::Value();
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a random Value based on input data
    tinygltf::Value val = CreateRandomValue(Data, Size);

    // Test IsString
    bool isString = val.IsString();
    (void)isString; // Suppress unused warning

    // Test IsReal
    bool isReal = val.IsReal();
    (void)isReal;

    // Test IsObject
    bool isObject = val.IsObject();
    (void)isObject;

    // Test IsArray
    bool isArray = val.IsArray();
    (void)isArray;

    // Test IsBool
    bool isBool = val.IsBool();
    (void)isBool;

    // Test Get methods based on type checks
    // Only access specific getters if the type matches to avoid undefined behavior or assertions
    // Note: The prompt mentions Get(size_t) for arrays, but also lists TINYGLTF_VALUE_GET macros.
    // We will test the array Get(size_t) specifically as requested in the description.

    if (isArray) {
        // Try to access elements
        // Use input data to determine index, ensuring it's somewhat random but safe
        size_t index = 0;
        if (Size > 0) {
            index = Data[0] % 100; // Limit index range
        }
        
        // Get returns a reference. We just need to call it to trigger the logic.
        // The function returns a reference to a static null Value if out of bounds.
        const tinygltf::Value &elem = val.Get(index);
        (void)elem;
    }

    // Additional exploration: Try to force specific types and check consistency
    // This helps cover branches where type checks return false for expected types
    tinygltf::Value boolVal = tinygltf::Value(true);
    (void)boolVal.IsBool();
    (void)boolVal.IsString(); // Should be false
    (void)boolVal.IsReal();   // Should be false

    tinygltf::Value intVal = tinygltf::Value(42);
    (void)intVal.IsReal(); // Note: In tinygltf, int is distinct from real usually, but check implementation
    (void)intVal.IsBool();

    tinygltf::Value realVal = tinygltf::Value(3.14);
    (void)realVal.IsReal();
    (void)realVal.IsBool();

    tinygltf::Value strVal = tinygltf::Value(std::string("test"));
    (void)strVal.IsString();
    (void)strVal.IsArray();

    tinygltf::Value arrVal = tinygltf::Value(tinygltf::Value::Array{tinygltf::Value(1), tinygltf::Value(2)});
    (void)arrVal.IsArray();
    // Access valid index
    if (arrVal.IsArray()) {
        const tinygltf::Value &v = arrVal.Get(0);
        (void)v;
        // Access invalid index
        const tinygltf::Value &v2 = arrVal.Get(999);
        (void)v2;
    }

    tinygltf::Value objVal = tinygltf::Value(tinygltf::Value::Object{{"key", tinygltf::Value("val")}});
    (void)objVal.IsObject();
    (void)objVal.IsArray();

    return 0;
}