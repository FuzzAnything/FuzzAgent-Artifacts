// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
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

extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::Value val;
    uint8_t op = Data[0] % 7;

    switch (op) {
        case 0: // Test IsString
            if (Size > 1) {
                std::string s(reinterpret_cast<const char*>(Data + 1), Size - 1);
                val = tinygltf::Value(s);
            }
            val.IsString();
            break;

        case 1: // Test IsInt
            if (Size > 1) {
                int i = static_cast<int>(Data[1]);
                val = tinygltf::Value(i);
            }
            val.IsInt();
            break;

        case 2: // Test IsReal
            if (Size > 1) {
                double d = static_cast<double>(Data[1]);
                val = tinygltf::Value(d);
            }
            val.IsReal();
            break;

        case 3: // Test IsBool
            if (Size > 1) {
                bool b = (Data[1] != 0);
                val = tinygltf::Value(b);
            }
            val.IsBool();
            break;

        case 4: // Test IsBinary
            if (Size > 1) {
                // tinygltf::Value has a constructor taking (const unsigned char*, size_t) for binary
                val = tinygltf::Value(Data + 1, Size - 1);
            }
            val.IsBinary();
            break;

        case 5: // Test Get (Array)
            {
                tinygltf::Value::Array arr;
                // Fill array with simple values based on input
                for (size_t i = 1; i < Size; ++i) {
                    arr.push_back(tinygltf::Value(static_cast<int>(Data[i])));
                }
                val = tinygltf::Value(arr);
                
                if (val.IsArray()) {
                    // Try to access various indices
                    size_t idx = (Size > 1) ? Data[1] % (arr.size() + 1) : 0;
                    // Get() returns the array reference, then we index it
                    const auto& array_ref = val.Get<tinygltf::Value::Array>();
                    if (idx < array_ref.size()) {
                        const auto& element = array_ref[idx];
                        (void)element; // Suppress unused warning
                    }
                }
            }
            break;

        case 6: // Test Get (Object)
            {
                tinygltf::Value::Object obj;
                // Create object entries
                for (size_t i = 1; i < Size; ++i) {
                    std::string key = "key_" + std::to_string(i);
                    obj[key] = tinygltf::Value(static_cast<int>(Data[i]));
                }
                val = tinygltf::Value(obj);
                (void)val.IsObject();
            }
            break;
    }

    return 0;
}