// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
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
#include "code/tiny_gltf.h"
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Create a double from the fuzzer input
    double input_value = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&input_value, Data, sizeof(double));
    } else {
        // If not enough bytes, use the available bytes and pad with zeros
        uint8_t buffer[sizeof(double)] = {0};
        for (size_t i = 0; i < Size && i < sizeof(double); ++i) {
            buffer[i] = Data[i];
        }
        memcpy(&input_value, buffer, sizeof(double));
    }

    try {
        // Create Value object with the double
        tinygltf::Value val(input_value);
        
        // Test all the target functions
        bool is_real = val.IsReal();
        double as_double = val.GetNumberAsDouble();
        bool is_number = val.IsNumber();
        bool is_int = val.IsInt();
        char type = val.Type();
        int as_int = val.GetNumberAsInt();
        
        // Use the results to avoid dead code elimination
        static volatile bool dummy_bool = false;
        static volatile double dummy_double = 0.0;
        static volatile char dummy_char = 0;
        static volatile int dummy_int = 0;
        
        dummy_bool = is_real || is_number || is_int;
        dummy_double = as_double;
        dummy_char = type;
        dummy_int = as_int;
        
    } catch (...) {
        // Catch any exceptions to prevent crashes from propagating
    }
    
    return 0;
}