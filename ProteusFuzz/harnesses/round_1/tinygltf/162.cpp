// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
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
#include <cstring>
#include <vector>
#include <string>
#include <cstdlib>

static tinygltf::Value CreateValueFromFuzzData(const uint8_t *Data, size_t Size) {
    // Helper to create a Value based on the first byte of input to diversify types
    if (Size == 0) {
        return tinygltf::Value(); // Null value
    }

    uint8_t typeSelector = Data[0] % 8;

    switch (typeSelector) {
        case 0: // BOOL_TYPE
            return tinygltf::Value(Size > 1 ? (Data[1] % 2 != 0) : false);
        case 1: // INT_TYPE
            if (Size >= 5) {
                int32_t val = 0;
                std::memcpy(&val, Data + 1, sizeof(int32_t));
                return tinygltf::Value(val);
            }
            return tinygltf::Value(0);
        case 2: // REAL_TYPE
            if (Size >= 9) {
                double val = 0.0;
                std::memcpy(&val, Data + 1, sizeof(double));
                return tinygltf::Value(val);
            }
            return tinygltf::Value(0.0);
        case 3: // STRING_TYPE
            if (Size > 1) {
                // Create a string from remaining data
                size_t strLen = Size - 1;
                // Avoid excessively large strings to prevent OOM in fuzzer
                if (strLen > 1024) strLen = 1024;
                return tinygltf::Value(std::string(reinterpret_cast<const char*>(Data + 1), strLen));
            }
            return tinygltf::Value(std::string(""));
        case 4: // ARRAY_TYPE
            {
                tinygltf::Value::Array arr;
                // Create a small array of nested values
                size_t count = (Size > 1) ? (Data[1] % 5) : 0;
                for (size_t i = 0; i < count; ++i) {
                    // Recursively create simple values for the array
                    uint8_t subType = (Size > 2 + i) ? (Data[2 + i] % 2) : 0;
                    if (subType == 0) {
                        arr.push_back(tinygltf::Value(true));
                    } else {
                        arr.push_back(tinygltf::Value(42));
                    }
                }
                return tinygltf::Value(arr);
            }
        case 5: // OBJECT_TYPE
            {
                tinygltf::Value::Object obj;
                size_t count = (Size > 1) ? (Data[1] % 5) : 0;
                for (size_t i = 0; i < count; ++i) {
                    std::string key = "key_" + std::to_string(i);
                    obj[key] = tinygltf::Value(true);
                }
                return tinygltf::Value(obj);
            }
        case 6: // BINARY_TYPE
            {
                if (Size > 1) {
                    size_t len = Size - 1;
                    if (len > 1024) len = 1024;
                    // Use the constructor taking pointer and size for binary data
                    return tinygltf::Value(Data + 1, len);
                }
                return tinygltf::Value(static_cast<const unsigned char*>(nullptr), 0);
            }
        default: // NULL_TYPE
            return tinygltf::Value();
    }
}

static void FuzzValueAPIs(tinygltf::Value &val, const uint8_t *Data, size_t Size) {
    // 1. IsBool
    bool isBool = val.IsBool();
    (void)isBool; // Suppress unused warning

    // 2. Has (Only safe for objects, but calling on non-objects is defined to return false)
    std::string key = "test_key";
    if (Size > 10) {
        key.assign(reinterpret_cast<const char*>(Data + 1), std::min(Size - 1, size_t(10)));
    }
    bool hasKey = val.Has(key);
    (void)hasKey;

    // 3. IsInt
    bool isInt = val.IsInt();
    (void)isInt;

    // 4. IsNumber
    bool isNumber = val.IsNumber();
    (void)isNumber;

    // 5. IsString
    bool isString = val.IsString();
    (void)isString;

    // 6. Get (Array access)
    // Ensure we only call Get on arrays to avoid asserts in debug builds or undefined behavior
    if (val.IsArray()) {
        // Generate an index from fuzz data
        size_t index = 0;
        if (Size >= 2) {
            index = static_cast<size_t>(Data[1]) % 10;
        }
        // Access element safely
        const tinygltf::Value &elem = val.Get(index);
        (void)elem;
    }
}

extern "C" int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a Value based on fuzz data
    tinygltf::Value val = CreateValueFromFuzzData(Data, Size);

    // Fuzz the target API functions
    FuzzValueAPIs(val, Data, Size);

    return 0;
}