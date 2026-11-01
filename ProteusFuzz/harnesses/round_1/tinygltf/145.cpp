// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <array>

static tinygltf::Value CreateValueFromData(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return tinygltf::Value(); // Null value
    }

    // Interpret first byte to decide type
    uint8_t typeHint = Data[0];
    size_t offset = 1;
    size_t remaining = Size - 1;

    switch (typeHint % 8) {
        case 0: // Null
            return tinygltf::Value();
        case 1: // Boolean
            return tinygltf::Value((remaining > 0 && Data[offset]) ? true : false);
        case 2: // Integer (try to read 4 bytes if available, else 0)
            {
                int val = 0;
                if (remaining >= 4) {
                    std::memcpy(&val, Data + offset, 4);
                }
                return tinygltf::Value(val);
            }
        case 3: // Double (try to read 8 bytes if available, else 0.0)
            {
                double val = 0.0;
                if (remaining >= 8) {
                    std::memcpy(&val, Data + offset, 8);
                }
                return tinygltf::Value(val);
            }
        case 4: // String
            {
                // Use remaining data as string content, ensuring null termination
                std::string str(reinterpret_cast<const char*>(Data + offset), remaining);
                return tinygltf::Value(str);
            }
        case 5: // Array
            {
                tinygltf::Value::Array arr;
                // Create a few dummy values to simulate an array
                // We don't parse the whole buffer recursively to avoid deep recursion in fuzzing
                // Just add a couple of simple values based on data
                if (remaining > 0) arr.push_back(tinygltf::Value((Data[offset] % 2) ? true : false));
                if (remaining > 1) arr.push_back(tinygltf::Value(static_cast<int>(Data[offset + 1])));
                if (remaining > 2) arr.push_back(tinygltf::Value(static_cast<double>(Data[offset + 2])));
                return tinygltf::Value(arr);
            }
        case 6: // Object
            {
                tinygltf::Value::Object obj;
                // Create a few dummy key-value pairs
                if (remaining > 0) {
                    std::string key = "key_" + std::to_string(Data[offset]);
                    obj[key] = tinygltf::Value(true);
                }
                if (remaining > 1) {
                    std::string key = "key_" + std::to_string(Data[offset + 1]);
                    obj[key] = tinygltf::Value(static_cast<int>(Data[offset + 1]));
                }
                if (remaining > 2) {
                    std::string key = "key_" + std::to_string(Data[offset + 2]);
                    obj[key] = tinygltf::Value(static_cast<double>(Data[offset + 2]));
                }
                return tinygltf::Value(obj);
            }
        case 7: // Binary
            {
                std::vector<unsigned char> binData(Data + offset, Data + Size);
                return tinygltf::Value(std::move(binData));
            }
        default:
            return tinygltf::Value();
    }
}

extern "C" int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a Value instance from fuzz input
    tinygltf::Value val = CreateValueFromData(Data, Size);

    // Test IsInt
    bool isInt = val.IsInt();
    (void)isInt; // Suppress unused warning

    // Test IsString
    bool isString = val.IsString();
    (void)isString;

    // Test IsObject
    bool isObject = val.IsObject();
    (void)isObject;

    // Test Has (only meaningful for objects, but safe to call on others)
    std::string testKey = "test_key";
    if (Size > 5) {
        // Use part of input to generate a dynamic key
        testKey = std::string(reinterpret_cast<const char*>(Data + 5), std::min<size_t>(Size - 5, 10));
    }
    bool hasKey = val.Has(testKey);
    (void)hasKey;

    // Test Keys
    std::vector<std::string> keys = val.Keys();
    (void)keys;

    // Test Get (Array access)
    // We try to access index 0, 1, and potentially an out-of-bounds index
    // The Get method is overloaded, so we need to call specific overloads or rely on template deduction
    // Since we cannot easily call the template overloads generically without knowing the type,
    // we will cast to specific types if the value is an array.
    
    if (val.IsArray()) {
        // Access index 0
        const tinygltf::Value &v0 = val.Get(0);
        (void)v0;
        
        // Access index 1
        const tinygltf::Value &v1 = val.Get(1);
        (void)v1;

        // Access out of bounds (should return null)
        const tinygltf::Value &vOut = val.Get(1000);
        (void)vOut;

        // Try to get specific types if the array contains them
        // Note: Get<T> is a template. We can't fuzz the template parameter directly easily.
        // However, we can try to access the raw Value and check its type.
        // The provided signature list shows TINYGLTF_VALUE_GET macros which expand to Get<T>().
        // We will attempt to call Get<int>, Get<double>, Get<bool> if the array is not empty.
        
        // Check if array is not empty using the array_value_ member or by trying to access index 0 safely
        // Since we created the value, we know if it's an array, we can check size via Get(0) existence logic
        // But tinygltf Value doesn't expose size directly in a public API without IsArray check.
        // We rely on the fact that Get(0) returns a valid reference if size > 0, else null.
        // To be safe, we just try to access index 0. If the array was empty in CreateValueFromData, 
        // Get(0) returns a null Value, and Get<int>() on a null Value might behave differently.
        // We assume tinygltf handles null Value access gracefully or asserts.
        
        // We access the internal array_value_ if possible, but it's private.
        // Instead, we just try to extract types from the first element if it exists.
        // We can check if Get(0) is not null by checking its type, but Get(0) returns a reference.
        // Let's just attempt the extraction.
        
        try {
            // We need to check if the array is not empty before accessing.
            // Since we can't easily get size, we rely on the fact that our constructor 
            // only creates arrays with 0-3 elements.
            // If the array is empty, Get(0) returns a static null Value.
            // Calling Get<int>() on a null Value might crash or return 0.
            // We wrap in try-catch to be safe.
            
            // Check if the value at index 0 is actually an integer before casting
            const tinygltf::Value &elem0 = val.Get(0);
            if (elem0.IsInt()) {
                int dummyInt = elem0.Get<int>();
                (void)dummyInt;
            }
            
            if (elem0.IsNumber()) { // IsNumber covers double and int
                 double dummyDouble = elem0.Get<double>();
                 (void)dummyDouble;
            }

            if (elem0.IsBool()) {
                bool dummyBool = elem0.Get<bool>();
                (void)dummyBool;
            }

            if (elem0.IsString()) {
                std::string dummyStr = elem0.Get<std::string>();
                (void)dummyStr;
            }
        } catch (...) {}
    }

    return 0;
}