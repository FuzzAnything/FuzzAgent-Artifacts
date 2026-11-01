// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
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
#include <array>
#include "tiny_gltf.h"

static tinygltf::Value CreateValueFromFuzzData(const uint8_t *Data, size_t Size) {
    // Fuzzing strategy:
    // - If Size is 0, return a null value.
    // - If Size > 0, use the first byte to determine the type of Value to construct.
    // - Use the remaining bytes to populate the value content if applicable.
    
    if (Size == 0) {
        return tinygltf::Value(); // Null value
    }

    uint8_t typeSelector = Data[0];
    size_t offset = 1;
    size_t remaining = Size - offset;

    switch (typeSelector % 7) {
        case 0: // Null
            return tinygltf::Value();
        case 1: // Bool
            return tinygltf::Value(remaining > 0 ? (Data[offset] % 2 == 1) : false);
        case 2: // Int
            if (remaining >= 4) {
                int32_t intVal = 0;
                std::memcpy(&intVal, Data + offset, 4);
                return tinygltf::Value(static_cast<int>(intVal));
            }
            if (remaining > 0) {
                return tinygltf::Value(static_cast<int>(Data[offset]));
            }
            return tinygltf::Value(0);
        case 3: // Real (Double)
            if (remaining >= 8) {
                double realVal = 0.0;
                std::memcpy(&realVal, Data + offset, 8);
                return tinygltf::Value(realVal);
            }
            if (remaining > 0) {
                return tinygltf::Value(static_cast<double>(Data[offset]));
            }
            return tinygltf::Value(0.0);
        case 4: // String
            {
                // Find null terminator or use remaining length
                size_t strLen = remaining;
                for (size_t i = 0; i < remaining; ++i) {
                    if (Data[offset + i] == 0) {
                        strLen = i;
                        break;
                    }
                }
                return tinygltf::Value(std::string(reinterpret_cast<const char*>(Data + offset), strLen));
            }
        case 5: // Array
            {
                tinygltf::Value::Array arr;
                // Simple heuristic: create elements based on chunks of data
                // If data is small, just add a few primitive values
                for (size_t i = 0; i < remaining && i < 100; ++i) {
                    uint8_t val = Data[offset + i];
                    if (val % 3 == 0) arr.push_back(tinygltf::Value(static_cast<int>(val)));
                    else if (val % 3 == 1) arr.push_back(tinygltf::Value(static_cast<double>(val)));
                    else arr.push_back(tinygltf::Value(val % 2 == 0));
                }
                return tinygltf::Value(arr);
            }
        case 6: // Object
            {
                tinygltf::Value::Object obj;
                // Create a few key-value pairs
                for (size_t i = 0; i < remaining && i < 50; ++i) {
                    std::string key = "key_" + std::to_string(i);
                    uint8_t val = Data[offset + i];
                    if (val % 3 == 0) obj[key] = tinygltf::Value(static_cast<int>(val));
                    else if (val % 3 == 1) obj[key] = tinygltf::Value(static_cast<double>(val));
                    else obj[key] = tinygltf::Value(val % 2 == 0);
                }
                return tinygltf::Value(obj);
            }
        default:
            return tinygltf::Value();
    }
}

extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    // Create a Value instance based on fuzz input
    tinygltf::Value val = CreateValueFromFuzzData(Data, Size);

    // 2. Conduct tests on the target API functions
    // We will invoke the functions based on the type of the value to explore different states.
    
    // Test IsObject
    if (val.IsObject()) {
        // Safe to access object members if needed, but here we just check the type
        // No specific action required other than confirming the type check works
    }

    // Test IsNumber
    if (val.IsNumber()) {
        // If it's a number, we can try to get it as int or double depending on subtype
        if (val.IsInt()) {
            int intVal = val.Get<int>();
            (void)intVal; // Suppress unused warning
        }
        if (val.IsNumber() && !val.IsInt()) { // It's a double
            double realVal = val.Get<double>();
            (void)realVal;
        }
    }

    // Test IsBool
    if (val.IsBool()) {
        bool boolVal = val.Get<bool>();
        (void)boolVal;
    }

    // Test IsBinary
    if (val.IsBinary()) {
        std::vector<unsigned char> binaryVal = val.Get<std::vector<unsigned char>>();
        (void)binaryVal;
    }

    // Test IsInt (redundant with IsNumber check above but explicit for coverage)
    if (val.IsInt()) {
        int intVal = val.Get<int>();
        (void)intVal;
    }

    // Test Get on Array
    // Only call Get on arrays to avoid assertion failures or undefined behavior
    if (val.IsArray()) {
        const tinygltf::Value::Array &arr = val.Get<tinygltf::Value::Array>();
        size_t arrSize = arr.size();
        
        // Fuzz index selection: try 0, size-1, and a random index from data
        std::vector<size_t> indices;
        indices.push_back(0);
        if (arrSize > 1) indices.push_back(arrSize - 1);
        
        // Use data to pick an index if available
        if (Size > 1) {
            size_t fuzzIndex = static_cast<size_t>(Data[1]) % (arrSize + 1); // +1 to test out of bounds
            indices.push_back(fuzzIndex);
        }

        for (size_t idx : indices) {
            // Get returns a reference; if out of bounds, it returns a static null Value
            // This is safe to call even if idx is out of bounds
            const tinygltf::Value &elem = val.Get(idx);
            (void)elem;
        }
    }

    // Test Get on Object (not directly supported by Get(size_t) but IsObject is checked)
    // The prompt specifically asks about Get(size_t) for arrays.
    // However, we can verify IsObject logic is robust.
    
    // 3. Cleanup
    // tinygltf::Value uses RAII, so no explicit cleanup is needed.
    // Return 0 to indicate success (fuzzing engine continues)
    return 0;
}