// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
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
#include <cstdint>
#include <cstring>
#include <vector>

static void testValueFunctions(const tinygltf::Value& value) {
    // Test all the API functions on the given Value object
    bool isReal = value.IsReal();
    bool isNumber = value.IsNumber();
    bool isInt = value.IsInt();
    char type = value.Type();
    
    // Only call GetNumberAsDouble and GetNumberAsInt if it's a number type
    // to avoid potential exceptions or undefined behavior
    if (isNumber) {
        double asDouble = value.GetNumberAsDouble();
        int asInt = value.GetNumberAsInt();
        // Use volatile to prevent optimization
        volatile double d = asDouble;
        volatile int i = asInt;
        (void)d;
        (void)i;
    }
    
    // Use volatile to prevent optimization
    volatile bool b1 = isReal;
    volatile bool b2 = isNumber;
    volatile bool b3 = isInt;
    volatile char c = type;
    (void)b1;
    (void)b2;
    (void)b3;
    (void)c;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Use first byte to determine what type of Value to create
    uint8_t typeSelector = Data[0];
    
    // Create different types of Value objects based on input
    if (typeSelector % 4 == 0) {
        // Create integer Value
        int intValue = 0;
        if (Size >= 1 + sizeof(int)) {
            memcpy(&intValue, Data + 1, sizeof(int));
        }
        tinygltf::Value value(intValue);
        testValueFunctions(value);
    }
    else if (typeSelector % 4 == 1) {
        // Create double Value
        double doubleValue = 0.0;
        if (Size >= 1 + sizeof(double)) {
            memcpy(&doubleValue, Data + 1, sizeof(double));
        }
        tinygltf::Value value(doubleValue);
        testValueFunctions(value);
    }
    else if (typeSelector % 4 == 2) {
        // Create array Value
        std::vector<tinygltf::Value> arrayValues;
        
        // Try to create some array elements from the input data
        size_t offset = 1;
        while (offset + sizeof(int) <= Size && arrayValues.size() < 10) {
            int elemValue = 0;
            memcpy(&elemValue, Data + offset, sizeof(int));
            arrayValues.push_back(tinygltf::Value(elemValue));
            offset += sizeof(int);
        }
        
        tinygltf::Value value(std::move(arrayValues));
        testValueFunctions(value);
    }
    else {
        // Create boolean Value
        bool boolValue = (typeSelector % 2 == 0);
        tinygltf::Value value(boolValue);
        testValueFunctions(value);
    }
    
    return 0;
}