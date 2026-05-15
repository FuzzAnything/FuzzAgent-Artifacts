// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
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
#include <vector>
#include <string>
#include <cstring>

static tinygltf::Value createValueFromFuzzData(const uint8_t *Data, size_t Size) {
    // Use fuzz data to create different Value types
    if (Size == 0) {
        // Return default constructed Value (null type)
        return tinygltf::Value();
    }
    
    // Use first byte to determine type
    uint8_t type_selector = Data[0];
    
    switch (type_selector % 6) {
        case 0: {
            // Integer type
            int int_val = 0;
            if (Size >= 1 + sizeof(int)) {
                memcpy(&int_val, Data + 1, sizeof(int));
            }
            return tinygltf::Value(int_val);
        }
        case 1: {
            // Real type (double)
            double real_val = 0.0;
            if (Size >= 1 + sizeof(double)) {
                memcpy(&real_val, Data + 1, sizeof(double));
            }
            return tinygltf::Value(real_val);
        }
        case 2: {
            // Bool type - check bounds before accessing Data[1]
            bool bool_val = false;
            if (Size >= 2) {
                bool_val = (Data[1] & 0x01) != 0;
            }
            return tinygltf::Value(bool_val);
        }
        case 3: {
            // String type - check bounds carefully
            std::string str_val;
            if (Size >= 2) {
                size_t str_len = Data[1] % (Size - 1);  // Size-1 is at least 1
                if (str_len > 0 && Size >= 2 + str_len) {
                    str_val.assign(reinterpret_cast<const char*>(Data + 2), str_len);
                } else {
                    str_val = "test";
                }
            } else {
                str_val = "test";
            }
            return tinygltf::Value(str_val);
        }
        case 4: {
            // Array type - using provided constructor
            tinygltf::Value::Array array;
            // Add some values to array
            array.push_back(tinygltf::Value(42));
            array.push_back(tinygltf::Value(3.14));
            array.push_back(tinygltf::Value(true));
            return tinygltf::Value(std::move(array));
        }
        case 5: {
            // Empty array
            tinygltf::Value::Array array;
            return tinygltf::Value(std::move(array));
        }
        default:
            return tinygltf::Value();
    }
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Create Value from fuzz data
    tinygltf::Value value = createValueFromFuzzData(Data, Size);
    
    // Test all target API functions
    bool is_int = value.IsInt();
    bool is_number = value.IsNumber();
    bool is_real = value.IsReal();
    char type_char = value.Type();
    
    // Only call GetNumberAsInt if it's a number type
    if (is_number) {
        int num_as_int = value.GetNumberAsInt();
        (void)num_as_int; // Avoid unused variable warning
    }
    
    // Only call GetNumberAsDouble if it's a number type
    if (is_number) {
        double num_as_double = value.GetNumberAsDouble();
        (void)num_as_double; // Avoid unused variable warning
    }
    
    // Create more diverse test cases
    // Test with different Value constructions
    static int counter = 0;
    counter++;
    
    // Test with specific values based on counter
    switch (counter % 5) {
        case 0: {
            tinygltf::Value int_val(100);
            (void)int_val.IsInt();
            (void)int_val.GetNumberAsInt();
            (void)int_val.IsNumber();
            (void)int_val.Type();
            (void)int_val.IsReal();
            (void)int_val.GetNumberAsDouble();
            break;
        }
        case 1: {
            tinygltf::Value real_val(3.14159);
            (void)real_val.IsInt();
            (void)real_val.GetNumberAsInt();
            (void)real_val.IsNumber();
            (void)real_val.Type();
            (void)real_val.IsReal();
            (void)real_val.GetNumberAsDouble();
            break;
        }
        case 2: {
            tinygltf::Value bool_val(true);
            (void)bool_val.IsInt();
            (void)bool_val.IsNumber();
            (void)bool_val.Type();
            (void)bool_val.IsReal();
            break;
        }
        case 3: {
            tinygltf::Value::Array array;
            array.push_back(tinygltf::Value(1));
            array.push_back(tinygltf::Value(2.0));
            tinygltf::Value array_val(std::move(array));
            (void)array_val.IsInt();
            (void)array_val.IsNumber();
            (void)array_val.Type();
            (void)array_val.IsReal();
            break;
        }
        case 4: {
            std::string str = "test_string";
            tinygltf::Value str_val(str);
            (void)str_val.IsInt();
            (void)str_val.IsNumber();
            (void)str_val.Type();
            (void)str_val.IsReal();
            break;
        }
    }
    
    return 0;
}