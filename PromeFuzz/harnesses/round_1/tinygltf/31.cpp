// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
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
#include <cstddef>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    // Use first byte to determine value type to create
    if (Size == 0) return 0;
    
    uint8_t value_type = Data[0] % 7; // 0-6 for different value types
    
    tinygltf::Value value;
    
    // Create different types of values based on input
    switch (value_type) {
        case 0: {
            // Create integer value
            int int_val = 42;
            if (Size >= 5) {
                // Use some bytes to create an integer
                memcpy(&int_val, Data + 1, sizeof(int));
            }
            value = tinygltf::Value(int_val);
            break;
        }
        case 1: {
            // Create real (double) value
            double real_val = 3.14159;
            if (Size >= 9) {
                memcpy(&real_val, Data + 1, sizeof(double));
            }
            value = tinygltf::Value(real_val);
            break;
        }
        case 2: {
            // Create boolean value - check we have at least 2 bytes
            bool bool_val = false;
            if (Size >= 2) {
                bool_val = (Data[1] % 2) == 0;
            }
            value = tinygltf::Value(bool_val);
            break;
        }
        case 3: {
            // Create string value
            std::string str_val = "test";
            if (Size > 1) {
                // Use up to 100 bytes for string
                size_t str_len = (Size - 1) > 100 ? 100 : (Size - 1);
                str_val.assign(reinterpret_cast<const char*>(Data + 1), str_len);
            }
            value = tinygltf::Value(str_val);
            break;
        }
        case 4: {
            // Create array value
            std::vector<tinygltf::Value> array_val;
            // Add some dummy values to array
            array_val.push_back(tinygltf::Value(1));
            array_val.push_back(tinygltf::Value(2.0));
            array_val.push_back(tinygltf::Value(true));
            value = tinygltf::Value(array_val);
            break;
        }
        case 5: {
            // Create object value
            tinygltf::Value::Object obj_val;
            obj_val["int_key"] = tinygltf::Value(123);
            obj_val["real_key"] = tinygltf::Value(45.67);
            value = tinygltf::Value(std::move(obj_val));
            break;
        }
        case 6: {
            // Create null value
            value = tinygltf::Value();
            break;
        }
    }
    
    // Test all the target API functions
    bool is_int = value.IsInt();
    bool is_number = value.IsNumber();
    bool is_real = value.IsReal();
    char type_char = value.Type();
    
    // Only call GetNumberAsInt and GetNumberAsDouble if it's a number type
    if (is_number) {
        int num_as_int = value.GetNumberAsInt();
        double num_as_double = value.GetNumberAsDouble();
        (void)num_as_int; // Avoid unused variable warning
        (void)num_as_double; // Avoid unused variable warning
    }
    
    // Avoid unused variable warnings
    (void)is_int;
    (void)is_number;
    (void)is_real;
    (void)type_char;
    
    return 0;
}