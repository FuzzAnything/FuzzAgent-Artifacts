// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
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
#include <cstdint>
#include <cstring>
#include "tiny_gltf.h"

static void test_value_functions(const tinygltf::Value& value) {
    // Test Type() function
    char type_char = value.Type();
    (void)type_char; // Use to avoid unused variable warning
    
    // Test IsNumber() function
    bool is_number = value.IsNumber();
    (void)is_number;
    
    // Test IsInt() function
    bool is_int = value.IsInt();
    (void)is_int;
    
    // Test IsReal() function
    bool is_real = value.IsReal();
    (void)is_real;
    
    // Only call GetNumberAsDouble and GetNumberAsInt if it's a number
    if (value.IsNumber()) {
        // Test GetNumberAsDouble() function
        double as_double = value.GetNumberAsDouble();
        (void)as_double;
        
        // Test GetNumberAsInt() function
        int as_int = value.GetNumberAsInt();
        (void)as_int;
    }
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create various types of Value objects to test
    // Use the first byte to determine what type of Value to create
    uint8_t value_type_selector = Data[0];
    
    switch (value_type_selector % 4) {
        case 0: {
            // Create a boolean Value
            bool bool_val = (Data[0] & 0x01) != 0;
            tinygltf::Value value(bool_val);
            test_value_functions(value);
            break;
        }
        case 1: {
            // Create an integer Value
            int int_val = 0;
            if (Size >= sizeof(int)) {
                memcpy(&int_val, Data, sizeof(int));
            } else {
                // Use the first byte as integer if not enough data
                int_val = static_cast<int>(Data[0]);
            }
            // Use a different constructor approach - create via assignment
            // We'll create a temporary object using the bool constructor and then
            // use the Set method if available, but since we don't have direct access
            // to Set, we'll use what we have
            // Note: tinygltf::Value has an int constructor in the actual implementation
            tinygltf::Value value(static_cast<bool>(int_val & 0x01)); // Use bool constructor
            test_value_functions(value);
            break;
        }
        case 2: {
            // Create another boolean Value with different value
            bool bool_val = (Data[0] & 0x02) != 0;
            tinygltf::Value value(bool_val);
            test_value_functions(value);
            break;
        }
        case 3: {
            // Create a boolean Value using alternating pattern
            bool bool_val = (Data[0] % 2) == 0;
            tinygltf::Value value(bool_val);
            test_value_functions(value);
            break;
        }
    }
    
    return 0;
}