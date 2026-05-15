// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
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

static void testValueFunctions(const tinygltf::Value& value) {
    // Test Type() function
    char type = value.Type();
    (void)type;
    
    // Test IsReal() function
    bool isReal = value.IsReal();
    (void)isReal;
    
    // Test IsInt() function
    bool isInt = value.IsInt();
    (void)isInt;
    
    // Test IsNumber() function
    bool isNumber = value.IsNumber();
    (void)isNumber;
    
    // Test GetNumberAsDouble() function
    if (value.IsNumber()) {
        double dblVal = value.GetNumberAsDouble();
        (void)dblVal;
    }
    
    // Test GetNumberAsInt() function
    if (value.IsNumber()) {
        int intVal = value.GetNumberAsInt();
        (void)intVal;
    }
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Test with binary type constructor
    if (Size > 0) {
        tinygltf::Value binaryValue(Data, Size);
        testValueFunctions(binaryValue);
    }
    
    // Test with integer type
    if (Size >= sizeof(int)) {
        int intValue;
        memcpy(&intValue, Data, sizeof(int));
        tinygltf::Value intVal(intValue);
        testValueFunctions(intVal);
    }
    
    // Test with double type
    if (Size >= sizeof(double)) {
        double doubleValue;
        memcpy(&doubleValue, Data, sizeof(double));
        tinygltf::Value doubleVal(doubleValue);
        testValueFunctions(doubleVal);
    }
    
    // Test with boolean type
    if (Size > 0) {
        bool boolValue = Data[0] & 1;
        tinygltf::Value boolVal(boolValue);
        testValueFunctions(boolVal);
    }
    
    // Test with string type
    if (Size > 0) {
        std::string strValue(reinterpret_cast<const char*>(Data), 
                            Size > 100 ? 100 : Size);
        tinygltf::Value strVal(strValue);
        testValueFunctions(strVal);
    }
    
    // Test with array type
    if (Size > 0) {
        tinygltf::Value::Array arrayValue;
        for (size_t i = 0; i < 3 && i < Size; i++) {
            arrayValue.push_back(tinygltf::Value(static_cast<int>(Data[i])));
        }
        tinygltf::Value arrayVal(arrayValue);
        testValueFunctions(arrayVal);
    }
    
    // Test with empty value
    tinygltf::Value emptyValue;
    testValueFunctions(emptyValue);
    
    return 0;
}