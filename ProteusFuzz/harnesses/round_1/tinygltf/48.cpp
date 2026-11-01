// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
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
#include <map>
#include <array>

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::Value value;
    uint8_t op = Data[0] % 10;

    switch (op) {
        case 0: {
            // Test IsInt with integer construction
            int val = 0;
            if (Size > 1) {
                val = static_cast<int>(Data[1]);
            }
            value = tinygltf::Value(val);
            (void)value.IsInt();
            break;
        }
        case 1: {
            // Test IsString with string construction
            std::string str;
            if (Size > 1) {
                str.assign(reinterpret_cast<const char*>(Data + 1), Size - 1);
            }
            value = tinygltf::Value(str);
            (void)value.IsString();
            break;
        }
        case 2: {
            // Test IsBool with boolean construction
            bool b = (Size > 1 && Data[1] != 0);
            value = tinygltf::Value(b);
            (void)value.IsBool();
            break;
        }
        case 3: {
            // Test IsArray with array construction
            std::vector<tinygltf::Value> arr;
            if (Size > 1) {
                // Create a few dummy values based on input size
                for (size_t i = 1; i < Size && i < 10; ++i) {
                    arr.push_back(tinygltf::Value(static_cast<int>(Data[i])));
                }
            }
            value = tinygltf::Value(arr);
            (void)value.IsArray();
            // Test Get on array
            if (Size > 2) {
                size_t idx = Data[1] % 100;
                (void)value.Get(idx);
            }
            break;
        }
        case 4: {
            // Test IsObject with object construction
            std::map<std::string, tinygltf::Value> obj;
            if (Size > 1) {
                obj["key"] = tinygltf::Value(static_cast<int>(Data[1]));
                obj["str"] = tinygltf::Value(std::string(reinterpret_cast<const char*>(Data + 1), Size - 1));
            }
            value = tinygltf::Value(obj);
            (void)value.IsObject();
            break;
        }
        case 5: {
            // Test IsNull (default constructor)
            value = tinygltf::Value();
            (void)value.IsArray();
            (void)value.IsInt();
            (void)value.IsString();
            (void)value.IsBool();
            (void)value.IsObject();
            // Test Get on null (should return null safely)
            (void)value.Get(0);
            break;
        }
        case 6: {
            // Test IsDouble (not explicitly requested but related to Value types)
            double d = 0.0;
            if (Size > 1) {
                d = static_cast<double>(Data[1]);
            }
            value = tinygltf::Value(d);
            (void)value.IsInt(); // Should be false
            (void)value.IsString(); // Should be false
            break;
        }
        case 7: {
            // Test mixed types in array
            std::vector<tinygltf::Value> arr;
            arr.push_back(tinygltf::Value(1));
            arr.push_back(tinygltf::Value(std::string("test")));
            arr.push_back(tinygltf::Value(true));
            value = tinygltf::Value(arr);
            (void)value.IsArray();
            if (Size > 1) {
                size_t idx = Data[1] % 3;
                tinygltf::Value elem = value.Get(idx);
                (void)elem.IsInt();
                (void)elem.IsString();
                (void)elem.IsBool();
            }
            break;
        }
        case 8: {
            // Test out of bounds Get
            std::vector<tinygltf::Value> arr;
            arr.push_back(tinygltf::Value(1));
            value = tinygltf::Value(arr);
            (void)value.Get(9999); // Out of bounds
            break;
        }
        case 9: {
            // Test Get on non-array (should return null safely)
            value = tinygltf::Value(42);
            (void)value.Get(0);
            break;
        }
        default:
            break;
    }

    return 0;
}