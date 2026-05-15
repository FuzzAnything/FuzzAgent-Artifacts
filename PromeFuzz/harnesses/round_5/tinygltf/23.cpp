// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
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
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Use the first byte to determine the integer value for the Value constructor
    int int_value = 0;
    if (Size > 0) {
        // Use the first byte as the integer value
        int_value = static_cast<int>(Data[0]);
    }
    
    // Create a tinygltf::Value object with the integer constructor
    tinygltf::Value value(int_value);
    
    // Test all the target API functions
    double double_result = value.GetNumberAsDouble();
    bool is_int_result = value.IsInt();
    int int_result = value.GetNumberAsInt();
    char type_result = value.Type();
    bool is_number_result = value.IsNumber();
    bool is_real_result = value.IsReal();
    
    // Use volatile to prevent optimization
    volatile double v_double = double_result;
    volatile bool v_is_int = is_int_result;
    volatile int v_int = int_result;
    volatile char v_type = type_result;
    volatile bool v_is_number = is_number_result;
    volatile bool v_is_real = is_real_result;
    
    // Use the results in a way that can't be optimized away
    if (v_double > 1000.0 || v_int > 1000 || v_type == 'x') {
        // This branch is unlikely to be taken, but prevents dead code elimination
        __builtin_trap();
    }
    
    return 0;
}