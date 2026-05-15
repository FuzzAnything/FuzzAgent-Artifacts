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
#include "tiny_gltf.h"

static void testValueFunctions(const tinygltf::Value& value) {
    // Test Type() function
    char type = value.Type();
    (void)type;
    
    // Test IsNumber() function
    bool isNumber = value.IsNumber();
    (void)isNumber;
    
    // Test IsInt() function
    bool isInt = value.IsInt();
    (void)isInt;
    
    // Test IsReal() function
    bool isReal = value.IsReal();
    (void)isReal;
    
    // Only call numeric conversion functions if it's a number
    if (value.IsNumber()) {
        // Test GetNumberAsDouble() function
        double asDouble = value.GetNumberAsDouble();
        (void)asDouble;
        
        // Test GetNumberAsInt() function
        int asInt = value.GetNumberAsInt();
        (void)asInt;
    }
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Create different types of Value objects using the available constructor
    // We'll use the int constructor as provided, but also try to create other types
    // using the fuzzer data to explore different states
    
    // Test 1: Create Value with integer from fuzzer data
    if (Size >= sizeof(int)) {
        int intValue;
        // Use first 4 bytes as int
        memcpy(&intValue, Data, sizeof(int));
        tinygltf::Value value1(intValue);
        testValueFunctions(value1);
    }
    
    // Test 2: Create Value with different integer values
    // Use various positions in the data to create different ints
    for (size_t i = 0; i + sizeof(int) <= Size; i += sizeof(int)) {
        int intValue;
        memcpy(&intValue, Data + i, sizeof(int));
        tinygltf::Value value2(intValue);
        testValueFunctions(value2);
    }
    
    // Test 3: Create Value with a simple int from the size
    tinygltf::Value value3(static_cast<int>(Size));
    testValueFunctions(value3);
    
    // Test 4: Create multiple Values with different ints
    // Use the first byte as int if available
    if (Size > 0) {
        tinygltf::Value value4(static_cast<int>(Data[0]));
        testValueFunctions(value4);
    }
    
    // Test 5: Create Value with 0
    tinygltf::Value value5(0);
    testValueFunctions(value5);
    
    // Test 6: Create Value with -1
    tinygltf::Value value6(-1);
    testValueFunctions(value6);
    
    // Test 7: Create Value with maximum int
    tinygltf::Value value7(std::numeric_limits<int>::max());
    testValueFunctions(value7);
    
    // Test 8: Create Value with minimum int
    tinygltf::Value value8(std::numeric_limits<int>::min());
    testValueFunctions(value8);
    
    return 0;
}