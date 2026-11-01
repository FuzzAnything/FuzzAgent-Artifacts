// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
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
    tinygltf::Value val;

    if (Size == 0) {
        return val; // Default constructed (null)
    }

    // Use the first byte to determine the type to construct
    uint8_t typeSelector = Data[0];
    size_t remainingSize = Size > 1 ? Size - 1 : 0;
    const uint8_t *payload = Size > 1 ? Data + 1 : nullptr;

    switch (typeSelector % 8) {
        case 0: // INT_TYPE
            if (remainingSize >= sizeof(int)) {
                int intVal = 0;
                std::memcpy(&intVal, payload, sizeof(int));
                val = tinygltf::Value(intVal);
            } else {
                val = tinygltf::Value(0);
            }
            break;
        case 1: // REAL_TYPE
            if (remainingSize >= sizeof(double)) {
                double realVal = 0.0;
                std::memcpy(&realVal, payload, sizeof(double));
                val = tinygltf::Value(realVal);
            } else {
                val = tinygltf::Value(0.0);
            }
            break;
        case 2: // STRING_TYPE
            if (remainingSize > 0) {
                std::string str(reinterpret_cast<const char*>(payload), remainingSize);
                val = tinygltf::Value(str);
            } else {
                val = tinygltf::Value("");
            }
            break;
        case 3: // BOOL_TYPE
            val = tinygltf::Value(payload != nullptr && payload[0] != 0);
            break;
        case 4: // ARRAY_TYPE
            {
                tinygltf::Value::Array arr;
                if (remainingSize > 0) {
                    // Interpret payload as a count of elements (capped)
                    int count = 0;
                    if (remainingSize >= sizeof(int)) {
                        std::memcpy(&count, payload, sizeof(int));
                    }
                    // Limit array size to prevent OOM
                    int safeCount = (count > 1000) ? 1000 : count;
                    if (safeCount < 0) safeCount = 0;
                    
                    for (int i = 0; i < safeCount; ++i) {
                        // Recursively create simple values for the array
                        // We just push integers based on payload bytes to keep it simple
                        if (i < remainingSize) {
                            arr.push_back(tinygltf::Value(static_cast<int>(payload[i])));
                        } else {
                            arr.push_back(tinygltf::Value(0));
                        }
                    }
                }
                val = tinygltf::Value(arr);
            }
            break;
        case 5: // OBJECT_TYPE
            {
                tinygltf::Value::Object obj;
                if (remainingSize > 0) {
                    int count = 0;
                    if (remainingSize >= sizeof(int)) {
                        std::memcpy(&count, payload, sizeof(int));
                    }
                    int safeCount = (count > 100) ? 100 : count;
                    if (safeCount < 0) safeCount = 0;

                    for (int i = 0; i < safeCount; ++i) {
                        std::string key = "key_" + std::to_string(i);
                        // Create a simple value for the object
                        obj[key] = tinygltf::Value(static_cast<int>(i));
                    }
                }
                val = tinygltf::Value(obj);
            }
            break;
        case 6: // BINARY_TYPE
            if (remainingSize > 0) {
                // tinygltf::Value has a constructor taking (const unsigned char*, size_t)
                val = tinygltf::Value(payload, remainingSize);
            } else {
                val = tinygltf::Value(static_cast<const unsigned char*>(nullptr), 0);
            }
            break;
        case 7: // NULL_TYPE (default)
            // val remains default constructed (null)
            break;
    }

    return val;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a Value instance from fuzz data
    tinygltf::Value val = CreateValueFromFuzzData(Data, Size);

    // Fuzz IsNumber
    bool isNumber = val.IsNumber();
    (void)isNumber; // Suppress unused warning

    // Fuzz IsReal
    bool isReal = val.IsReal();
    (void)isReal;

    // Fuzz IsArray
    bool isArray = val.IsArray();
    (void)isArray;

    // Fuzz IsObject
    bool isObject = val.IsObject();
    (void)isObject;

    // Fuzz IsBinary
    bool isBinary = val.IsBinary();
    (void)isBinary;

    // Fuzz Get methods (Type specific)
    // We attempt to get values regardless of type to test internal safety checks
    
    // Get boolean
    bool boolVal = val.Get<bool>();
    (void)boolVal;

    // Get real (double)
    double realVal = val.Get<double>();
    (void)realVal;

    // Get int
    int intVal = val.Get<int>();
    (void)intVal;

    // Get string
    std::string strVal = val.Get<std::string>();
    (void)strVal;

    // Get binary
    std::vector<unsigned char> binVal = val.Get<std::vector<unsigned char>>();
    (void)binVal;

    // Get array
    tinygltf::Value::Array arrVal = val.Get<tinygltf::Value::Array>();
    (void)arrVal;

    // Get object
    tinygltf::Value::Object objVal = val.Get<tinygltf::Value::Object>();
    (void)objVal;

    // Specific test for array indexing if it is an array
    if (val.IsArray()) {
        // Use fuzz data to determine index
        size_t idx = 0;
        if (Size > 2) {
            idx = static_cast<size_t>(Data[1]) + static_cast<size_t>(Data[2]) * 256;
        }
        // Access element safely using the array's operator[] or Get with index if available
        // tinygltf Value::Get(size_t) is not a template, but we can access via array_value_ if needed
        // However, the API usually provides Get() for the array type, then index it.
        // Let's try to get the array and index it manually to be safe with the API version
        tinygltf::Value::Array arr = val.Get<tinygltf::Value::Array>();
        if (idx < arr.size()) {
            tinygltf::Value elem = arr[idx];
            (void)elem;
        } else {
            // Access out of bounds to trigger safety checks if any
            // This might crash if not handled, but fuzzing aims to find such issues.
            // We rely on the library's internal checks or undefined behavior to be caught by sanitizers.
            // To be safe and avoid immediate crash in the fuzzer harness, we skip if out of bounds
            // unless we want to test the library's robustness against OOB.
            // Let's just access it to see if the library handles it.
            // Note: tinygltf Value::Get(size_t) is not standard in all versions, 
            // so accessing arr[idx] is the standard way.
            // If the library doesn't check bounds here, it might crash, which is a valid bug finding.
            // But to prevent fuzzer from dying instantly on every OOB, we check.
            // However, for fuzzing, we might want to let it crash to find the bug.
            // Let's assume the library is safe or we want to find the crash.
            // We will not access if out of bounds to keep the fuzzer running for other paths.
        }
    }

    return 0;
}