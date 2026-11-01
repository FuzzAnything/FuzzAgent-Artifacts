// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
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

// Helper to create an array Value
static tinygltf::Value CreateArrayValue(const uint8_t *Data, size_t Size) {
    std::vector<tinygltf::Value> elements;
    
    // Ensure we have enough data to create at least one element if Size > 1
    if (Size > 1) {
        size_t count = (Size - 1) > 100 ? 100 : (Size - 1);
        for (size_t i = 0; i < count; ++i) {
            // Create simple int values for array elements
            tinygltf::Value val(static_cast<int>(Data[1 + i]));
            elements.push_back(val);
        }
    }
    // Construct Value from the vector
    return tinygltf::Value(elements);
}

// Helper to create an object Value
static tinygltf::Value CreateObjectValue(const uint8_t *Data, size_t Size) {
    std::map<std::string, tinygltf::Value> objectMap;
    
    if (Size > 1) {
        size_t pairs = (Size - 1) > 50 ? 50 : (Size - 1);
        for (size_t i = 0; i < pairs; ++i) {
            std::string key = "key_" + std::to_string(i);
            tinygltf::Value valElem(static_cast<int>(Data[1 + i]));
            objectMap[key] = valElem;
        }
    }
    // Construct Value from the map
    return tinygltf::Value(objectMap);
}

static tinygltf::Value CreateValueFromFuzzData(const uint8_t *Data, size_t Size) {
    tinygltf::Value val;
    
    if (Size == 0) {
        return val; // Returns null value
    }

    uint8_t typeSelector = Data[0] % 10;

    switch (typeSelector) {
        case 0: // Null
            return val;
        case 1: // Boolean
            // Ensure Size >= 2 before accessing Data[1]
            if (Size >= 2) {
                val = tinygltf::Value(Data[1] % 2 != 0);
            } else {
                val = tinygltf::Value(false);
            }
            break;
        case 2: // Integer
            if (Size >= 5) {
                int32_t intVal = 0;
                std::memcpy(&intVal, Data + 1, sizeof(int32_t));
                val = tinygltf::Value(intVal);
            } else {
                val = tinygltf::Value(0);
            }
            break;
        case 3: // Real (Double)
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
                size_t strLen = (Size - 1) > 1024 ? 1024 : (Size - 1);
                std::string str(reinterpret_cast<const char*>(Data + 1), strLen);
                val = tinygltf::Value(str);
            } else {
                val = tinygltf::Value(std::string(""));
            }
            break;
        case 5: // Array
            return CreateArrayValue(Data, Size);
        case 6: // Object
            return CreateObjectValue(Data, Size);
        default:
            // Fallback to integer
            val = tinygltf::Value(0);
            break;
    }

    return val;
}

extern "C" int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::Value val = CreateValueFromFuzzData(Data, Size);

    // 1. Test Type()
    char type = val.Type();
    (void)type; // Suppress unused warning

    // 2. Test ArrayLen()
    size_t len = val.ArrayLen();
    (void)len;

    // 3. Test Size()
    size_t sz = val.Size();
    (void)sz;

    // 4. Test Keys()
    std::vector<std::string> keys = val.Keys();
    (void)keys;

    // 5. Test Has()
    // Generate a key to check based on fuzz data
    std::string checkKey;
    if (Size > 10) {
        size_t keyLen = (Size - 10) > 20 ? 20 : (Size - 10);
        checkKey = std::string(reinterpret_cast<const char*>(Data + 10), keyLen);
    } else {
        checkKey = "default_key";
    }
    bool hasKey = val.Has(checkKey);
    (void)hasKey;

    // 6. Test Get() with safety check
    if (val.IsArray() && Size > 0) {
        size_t idx = Data[0] % (val.ArrayLen() + 1); // +1 to potentially test out of bounds (safe in tinygltf)
        const tinygltf::Value &elem = val.Get(idx);
        (void)elem;
    }

    // Additional state exploration: Create a specific object to test Has/Keys more robustly
    // Constructing an object manually using the map constructor
    std::map<std::string, tinygltf::Value> testMap;
    testMap["fuzz_key_1"] = tinygltf::Value(1);
    testMap["fuzz_key_2"] = tinygltf::Value(std::string("test"));
    tinygltf::Value testObj(testMap);
    
    bool hasFuzzKey = testObj.Has("fuzz_key_1");
    (void)hasFuzzKey;
    
    std::vector<std::string> objKeys = testObj.Keys();
    (void)objKeys;

    return 0;
}