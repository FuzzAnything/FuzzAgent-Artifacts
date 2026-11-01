// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <tiny_gltf.h>

static void FuzzValue(tinygltf::Value &val, const uint8_t *Data, size_t Size) {
    // Test IsNumber
    (void)val.IsNumber();

    // Test IsBool
    (void)val.IsBool();

    // Test IsInt
    (void)val.IsInt();

    // Test IsString
    (void)val.IsString();

    // Test Get for different types
    // Array access: Only attempt if the value is actually an array
    if (Size > 0 && val.IsArray()) {
        size_t index = Data[0];
        (void)val.Get(index);
    }

    // Object access via Has
    if (Size > 1 && val.IsObject()) {
        std::string key(reinterpret_cast<const char*>(Data), Size);
        (void)val.Has(key);
    }
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file if needed (not strictly needed for Value tests, but good practice)
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Fuzz different Value types
    // 1. String Value
    {
        std::string str(reinterpret_cast<const char*>(Data), Size);
        tinygltf::Value val(std::move(str));
        FuzzValue(val, Data, Size);
    }

    // 2. Integer Value
    {
        int int_val = 0;
        if (Size >= sizeof(int)) {
            std::memcpy(&int_val, Data, sizeof(int));
        }
        tinygltf::Value val(int_val);
        FuzzValue(val, Data, Size);
    }

    // 3. Real (Float) Value
    {
        double real_val = 0.0;
        if (Size >= sizeof(double)) {
            std::memcpy(&real_val, Data, sizeof(double));
        }
        tinygltf::Value val(real_val);
        FuzzValue(val, Data, Size);
    }

    // 4. Boolean Value
    {
        bool bool_val = (Size > 0 && Data[0] != 0);
        tinygltf::Value val(bool_val);
        FuzzValue(val, Data, Size);
    }

    // 5. Array Value
    {
        tinygltf::Value::Array arr;
        // Create a few values to populate the array
        for (size_t i = 0; i < Size && i < 10; ++i) {
            if (i % 2 == 0) {
                arr.push_back(tinygltf::Value(static_cast<int>(Data[i])));
            } else {
                arr.push_back(tinygltf::Value(static_cast<double>(Data[i])));
            }
        }
        tinygltf::Value val(arr);
        FuzzValue(val, Data, Size);
    }

    // 6. Object Value
    {
        tinygltf::Value::Object obj;
        // Create a few key-value pairs
        for (size_t i = 0; i < Size && i < 10; ++i) {
            std::string key = "key_" + std::to_string(i);
            obj[key] = tinygltf::Value(static_cast<int>(Data[i]));
        }
        tinygltf::Value val(obj);
        FuzzValue(val, Data, Size);
    }

    // 7. Binary Value
    {
        tinygltf::Value val(Data, Size);
        FuzzValue(val, Data, Size);
    }

    // 8. Null Value
    {
        tinygltf::Value val;
        FuzzValue(val, Data, Size);
    }

    // Cleanup dummy file
    std::remove(dummy_file);

    return 0;
}