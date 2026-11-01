// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
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

static void FuzzValueFromData(const uint8_t *Data, size_t Size, tinygltf::Value &val) {
    if (Size == 0) {
        val = tinygltf::Value("");
        return;
    }

    // Use the first byte to determine the type to fuzz
    uint8_t typeSelector = Data[0] % 7;

    switch (typeSelector) {
        case 0: // INT_TYPE
            if (Size >= 4) {
                int intVal = *reinterpret_cast<const int*>(Data + 1);
                val = tinygltf::Value(intVal);
            } else {
                val = tinygltf::Value(0);
            }
            break;
        case 1: // REAL_TYPE
            if (Size >= 8) {
                double realVal = *reinterpret_cast<const double*>(Data + 1);
                val = tinygltf::Value(realVal);
            } else {
                val = tinygltf::Value(0.0);
            }
            break;
        case 2: // BOOL_TYPE
            val = tinygltf::Value((Data[1] & 1) != 0);
            break;
        case 3: // STRING_TYPE
            {
                std::string str(reinterpret_cast<const char*>(Data + 1), Size - 1);
                val = tinygltf::Value(str);
            }
            break;
        case 4: // BINARY_TYPE
            {
                // tinygltf::Value constructor for binary takes (const unsigned char*, size_t)
                // or rvalue vector. We use the pointer constructor.
                val = tinygltf::Value(Data + 1, Size - 1);
            }
            break;
        case 5: // ARRAY_TYPE
            {
                tinygltf::Value::Array arr;
                size_t chunkSize = (Size - 1) / 4;
                if (chunkSize > 100) chunkSize = 100;
                for (size_t i = 0; i < chunkSize; ++i) {
                    uint8_t subType = Data[1 + i] % 3;
                    if (subType == 0) {
                        // Explicitly cast to int to avoid ambiguity with bool/double
                        arr.push_back(tinygltf::Value(static_cast<int>(i)));
                    } else if (subType == 1) {
                        arr.push_back(tinygltf::Value(static_cast<double>(i)));
                    } else {
                        arr.push_back(tinygltf::Value("item"));
                    }
                }
                val = tinygltf::Value(arr);
            }
            break;
        case 6: // OBJECT_TYPE
            {
                tinygltf::Value::Object obj;
                size_t chunkSize = (Size - 1) / 4;
                if (chunkSize > 50) chunkSize = 50;
                for (size_t i = 0; i < chunkSize; ++i) {
                    std::string key = "key" + std::to_string(i);
                    uint8_t valType = Data[1 + i] % 3;
                    if (valType == 0) {
                        obj[key] = tinygltf::Value(static_cast<int>(i));
                    } else if (valType == 1) {
                        obj[key] = tinygltf::Value(static_cast<double>(i));
                    } else {
                        obj[key] = tinygltf::Value("val");
                    }
                }
                val = tinygltf::Value(obj);
            }
            break;
        default:
            val = tinygltf::Value("");
            break;
    }
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::Value val;
    FuzzValueFromData(Data, Size, val);

    // 1. Test IsBinary
    bool isBinary = val.IsBinary();
    if (isBinary) {
        // Use Get template to access binary data safely
        const std::vector<unsigned char> &bin = val.Get<std::vector<unsigned char>>();
        (void)bin;
    }

    // 2. Test IsReal
    bool isReal = val.IsReal();
    if (isReal) {
        double r = val.Get<double>();
        (void)r;
    }

    // 3. Test IsNumber
    bool isNum = val.IsNumber();
    (void)isNum;

    // 4. Test IsInt
    bool isInt = val.IsInt();
    if (isInt) {
        int i = val.Get<int>();
        (void)i;
    }

    // 5. Test Get (Array access)
    if (val.IsArray()) {
        // Access index 0
        const tinygltf::Value &elem = val.Get(0);
        (void)elem;
        
        // Try out of bounds
        const tinygltf::Value &elemOob = val.Get(Size + 100);
        (void)elemOob;
    }

    // 6. Test IsBool
    bool isBool = val.IsBool();
    if (isBool) {
        bool b = val.Get<bool>();
        (void)b;
    }

    // Additional exploration: Test Get with specific types if types match
    if (val.IsInt()) {
        (void)val.Get<int>();
    }
    if (val.IsReal()) {
        (void)val.Get<double>();
    }
    if (val.IsBool()) {
        (void)val.Get<bool>();
    }
    if (val.IsString()) {
        (void)val.Get<std::string>();
    }
    if (val.IsBinary()) {
        (void)val.Get<std::vector<unsigned char>>();
    }
    if (val.IsArray()) {
        (void)val.Get<tinygltf::Value::Array>();
    }
    if (val.IsObject()) {
        (void)val.Get<tinygltf::Value::Object>();
    }

    return 0;
}