// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
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
#include <string>
#include <vector>

static tinygltf::Value createValueFromFuzzData(const uint8_t *data, size_t size) {
    if (size == 0) {
        return tinygltf::Value(std::string("default"));
    }
    
    // Use first byte to determine value type
    uint8_t type_selector = data[0] % 5;
    
    switch (type_selector) {
        case 0: // String
            return tinygltf::Value(std::string(reinterpret_cast<const char*>(data), 
                                             std::min(size, static_cast<size_t>(100))));
        case 1: // Integer
            return tinygltf::Value(static_cast<int>(data[0]));
        case 2: // Real
            return tinygltf::Value(static_cast<double>(data[0]));
        case 3: // Boolean
            return tinygltf::Value((data[0] & 1) != 0);
        case 4: // Array
            {
                tinygltf::Value::Array array;
                for (size_t i = 0; i < std::min(size, static_cast<size_t>(10)); ++i) {
                    array.emplace_back(static_cast<int>(data[i]));
                }
                return tinygltf::Value(array);
            }
        default:
            return tinygltf::Value(std::string("default"));
    }
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create Value object from fuzzer data
    tinygltf::Value value = createValueFromFuzzData(Data, Size);
    
    // Test Type() function
    char type_char = value.Type();
    (void)type_char; // Suppress unused variable warning
    
    // Test IsNumber() function
    bool is_number = value.IsNumber();
    
    // Test IsInt() function
    bool is_int = value.IsInt();
    
    // Test IsReal() function
    bool is_real = value.IsReal();
    
    // Test GetNumberAsDouble() function
    if (is_number) {
        double dbl_val = value.GetNumberAsDouble();
        (void)dbl_val; // Suppress unused variable warning
    }
    
    // Test GetNumberAsInt() function
    if (is_number) {
        int int_val = value.GetNumberAsInt();
        (void)int_val; // Suppress unused variable warning
    }
    
    // Test with different value types by creating additional values
    if (Size > 1) {
        // Create another value with different constructor
        std::string str_data(reinterpret_cast<const char*>(Data), 
                           std::min(Size, static_cast<size_t>(50)));
        tinygltf::Value string_value(std::move(str_data));
        
        // Test all functions on the string value
        char string_type = string_value.Type();
        (void)string_type;
        bool string_is_number = string_value.IsNumber();
        (void)string_is_number;
        bool string_is_int = string_value.IsInt();
        (void)string_is_int;
        bool string_is_real = string_value.IsReal();
        (void)string_is_real;
    }
    
    return 0;
}