// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <array>
#include <tiny_gltf.h>

static void ConstructRandomValue(tinygltf::Value& val, const uint8_t* data, size_t size) {
    if (size == 0) {
        val = tinygltf::Value(0.0); // Default to real
        return;
    }

    uint8_t type_selector = data[0] % 7; // 0-6

    switch (type_selector) {
        case 0: // Null
            val = tinygltf::Value();
            break;
        case 1: // Bool
            if (size >= 2) {
                val = tinygltf::Value(data[1] % 2 != 0);
            } else {
                val = tinygltf::Value(false);
            }
            break;
        case 2: // Int
            if (size >= 5) {
                int32_t int_val = 0;
                std::memcpy(&int_val, &data[1], sizeof(int32_t));
                val = tinygltf::Value(int_val);
            } else {
                val = tinygltf::Value(0);
            }
            break;
        case 3: // Real
            if (size >= 9) {
                double real_val = 0.0;
                std::memcpy(&real_val, &data[1], sizeof(double));
                val = tinygltf::Value(real_val);
            } else {
                val = tinygltf::Value(0.0);
            }
            break;
        case 4: // String
            if (size > 1) {
                size_t len = (size - 1) % 1024;
                if (len > 0) {
                    std::string str(reinterpret_cast<const char*>(data + 1), len);
                    val = tinygltf::Value(str);
                } else {
                    val = tinygltf::Value(std::string());
                }
            } else {
                val = tinygltf::Value(std::string());
            }
            break;
        case 5: // Array
            {
                tinygltf::Value::Array arr;
                if (size > 1) {
                    size_t count = (size - 1) % 20;
                    for (size_t i = 0; i < count; ++i) {
                        tinygltf::Value child;
                        // Recursively construct with shifted data or simple types
                        uint8_t child_type = data[1 + i] % 3;
                        if (child_type == 0) child = tinygltf::Value(0);
                        else if (child_type == 1) child = tinygltf::Value(1.0);
                        else child = tinygltf::Value(std::string("item"));
                        arr.push_back(child);
                    }
                }
                val = tinygltf::Value(arr);
                break;
            }
        case 6: // Object
            {
                tinygltf::Value::Object obj;
                if (size > 1) {
                    size_t count = (size - 1) % 10;
                    for (size_t i = 0; i < count; ++i) {
                        std::string key = "key_" + std::to_string(i);
                        tinygltf::Value val_child;
                        uint8_t child_type = data[1 + i] % 3;
                        if (child_type == 0) val_child = tinygltf::Value(0);
                        else if (child_type == 1) val_child = tinygltf::Value(1.0);
                        else val_child = tinygltf::Value(std::string("val"));
                        obj[key] = val_child;
                    }
                }
                val = tinygltf::Value(obj);
                break;
            }
    }
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::Value val;
    ConstructRandomValue(val, Data, Size);

    // 1. Test IsNumber
    bool is_num = val.IsNumber();
    (void)is_num;

    // 2. Test IsBool
    bool is_bool = val.IsBool();
    (void)is_bool;

    // 3. Test IsInt
    bool is_int = val.IsInt();
    (void)is_int;

    // 4. Test IsString
    bool is_str = val.IsString();
    (void)is_str;

    // 5. Test Get (Array access)
    // We try to access index 0 and potentially out of bounds based on input
    if (val.IsArray()) {
        size_t index = (Size > 0) ? (Data[0] % 100) : 0;
        // Safe access via Get
        const tinygltf::Value& elem = val.Get(index);
        // Check if the returned value is the null sentinel for out of bounds
        (void)elem;
    }

    // 6. Test Has (Object access)
    // Generate a key from input data
    if (Size > 0) {
        std::string key;
        size_t key_len = (Data[0] % 10) + 1;
        if (Size > 1) {
            // Ensure we don't read past the buffer
            if (key_len > Size - 1) key_len = Size - 1;
            key.assign(reinterpret_cast<const char*>(Data + 1), key_len);
        } else {
            key = "test_key";
        }
        bool has_key = val.Has(key);
        (void)has_key;
    }

    // Additional stress: Construct a specific array to test Get with various indices
    tinygltf::Value::Array test_arr;
    test_arr.push_back(tinygltf::Value(1));
    test_arr.push_back(tinygltf::Value(2.0));
    test_arr.push_back(tinygltf::Value(std::string("test")));
    tinygltf::Value arr_val(test_arr);

    // Test Get on known array
    const tinygltf::Value& v0 = arr_val.Get(0);
    const tinygltf::Value& v1 = arr_val.Get(1);
    const tinygltf::Value& v_out = arr_val.Get(100); // Out of bounds

    // Verify types of retrieved values
    if (v0.IsInt()) { /* valid */ }
    if (v1.IsNumber()) { /* valid */ }
    if (v_out.IsNumber() == false) { /* expected null */ }

    return 0;
}