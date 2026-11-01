// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
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
#include <map>

static tinygltf::Value CreateValueFromData(const uint8_t *Data, size_t Size) {
    tinygltf::Value val;
    
    if (Size == 0) {
        return val; // Default null value
    }

    uint8_t type = Data[0] % 5; // 0: null, 1: number, 2: string, 3: array, 4: object

    if (type == 0) {
        // Null
        return val;
    } else if (type == 1) {
        // Number (float)
        double num = 0.0;
        if (Size >= 8) {
            memcpy(&num, Data + 1, sizeof(double));
        }
        val = tinygltf::Value(num);
    } else if (type == 2) {
        // String
        if (Size > 1) {
            std::string str(reinterpret_cast<const char*>(Data + 1), Size - 1);
            val = tinygltf::Value(str);
        }
    } else if (type == 3) {
        // Array
        std::vector<tinygltf::Value> arr;
        size_t count = 0;
        if (Size >= 2) {
            count = Data[1];
        }
        // Limit recursion depth and array size to prevent stack overflow or excessive memory
        if (count > 100) count = 100;
        
        for (size_t i = 0; i < count; ++i) {
            // Check bounds before accessing Data
            // We need at least 2 bytes per element in this heuristic: type + value
            // Offset: 2 (header) + i*2 (previous elements) + 1 (current type) + 1 (current value)
            if (Size < 2 + i * 2 + 2) {
                break;
            }
            
            tinygltf::Value sub;
            uint8_t subType = Data[2 + i * 2] % 3;
            
            if (subType == 0) {
                // Number (using single byte as double)
                double d = static_cast<double>(Data[2 + i * 2 + 1]);
                sub = tinygltf::Value(d);
            } else if (subType == 1) {
                // String (single char)
                if (Size >= 2 + i * 2 + 2) {
                    sub = tinygltf::Value(std::string(1, static_cast<char>(Data[2 + i * 2 + 1])));
                } else {
                    sub = tinygltf::Value(std::string(1, static_cast<char>(Data[2 + i * 2])));
                }
            } else {
                // Null
                sub = tinygltf::Value();
            }
            arr.push_back(sub);
        }
        val = tinygltf::Value(arr);
    } else if (type == 4) {
        // Object
        std::map<std::string, tinygltf::Value> obj;
        // Simplified object generation
        if (Size > 1) {
            size_t keyCount = Data[1] % 50;
            for (size_t i = 0; i < keyCount; ++i) {
                std::string key = "key" + std::to_string(i);
                tinygltf::Value sub;
                if (Size > 2 + i) {
                    sub = tinygltf::Value((double)(Data[2 + i]));
                }
                obj[key] = sub;
            }
        }
        val = tinygltf::Value(obj);
    }

    return val;
}

extern "C" int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::Value val = CreateValueFromData(Data, Size);

    // Test Type()
    char typeChar = val.Type();
    (void)typeChar; // Suppress unused warning

    // Test IsArray()
    bool isArray = val.IsArray();
    
    // Test IsObject()
    bool isObject = val.IsObject();

    // Test ArrayLen()
    size_t arrLen = val.ArrayLen();
    (void)arrLen;

    // Test Size()
    size_t valSize = val.Size();
    (void)valSize;

    // Test Get(size_t) only if it is an array
    if (isArray) {
        // Try accessing various indices including out of bounds
        // Use data to determine index to fuzz
        size_t idx = 0;
        if (Size >= 2) {
            idx = Data[1]; 
        }
        
        // Also try a few fixed indices and modulo arithmetic to cover edge cases
        std::vector<size_t> indices;
        indices.push_back(idx);
        if (arrLen > 0) {
            indices.push_back(idx % (arrLen + 1)); // Modulo to ensure some in-bounds and some out-of-bounds
        } else {
            indices.push_back(0);
        }
        indices.push_back(0);
        if (arrLen > 0) indices.push_back(arrLen - 1);
        indices.push_back(arrLen); // Out of bounds
        indices.push_back(arrLen + 1); // Out of bounds

        for (size_t i : indices) {
            const tinygltf::Value &elem = val.Get(i);
            // Perform checks on the returned element to ensure it's valid
            if (i >= arrLen) {
                // Should be null
                if (elem.Type() != 'n') {
                    // This might be an assertion failure or logic error in the library if it doesn't return null
                    // But for fuzzing, we just observe behavior
                }
            } else {
                // Valid access, do some checks
                (void)elem.IsArray();
                (void)elem.IsObject();
                (void)elem.Type();
            }
        }
    }

    // Additional exploration: Create a new value based on the first byte to test different paths
    if (Size > 0) {
        tinygltf::Value val2;
        uint8_t t = Data[0] % 5;
        if (t == 1) val2 = tinygltf::Value(1.0);
        else if (t == 2) val2 = tinygltf::Value("test");
        else if (t == 3) {
            std::vector<tinygltf::Value> v;
            v.push_back(tinygltf::Value(1));
            v.push_back(tinygltf::Value("a"));
            val2 = tinygltf::Value(v);
        } else if (t == 4) {
            std::map<std::string, tinygltf::Value> m;
            m["a"] = tinygltf::Value(1);
            val2 = tinygltf::Value(m);
        }

        (void)val2.Type();
        (void)val2.IsArray();
        (void)val2.IsObject();
        (void)val2.ArrayLen();
        (void)val2.Size();
        
        if (val2.IsArray()) {
            (void)val2.Get(0);
            (void)val2.Get(val2.Size());
            (void)val2.Get(val2.Size() + 1);
        }
    }

    return 0;
}