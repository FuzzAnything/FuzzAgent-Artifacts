// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
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
#include <stdint.h>
#include <vector>
#include <string>

static void testValueFunctions(const tinygltf::Value& value) {
    // Test all the target API functions
    bool isInt = value.IsInt();
    bool isNumber = value.IsNumber();
    char type = value.Type();
    bool isReal = value.IsReal();
    
    if (isNumber) {
        int numAsInt = value.GetNumberAsInt();
        double numAsDouble = value.GetNumberAsDouble();
        (void)numAsInt;
        (void)numAsDouble;
    }
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Create a tinygltf::Value with different types based on input data
    if (Size == 0) {
        return 0;
    }
    
    // Use first byte to determine value type
    uint8_t typeSelector = Data[0];
    
    switch (typeSelector % 6) {
        case 0: {
            // Create integer value
            int intValue = 0;
            if (Size >= sizeof(int) + 1) {
                memcpy(&intValue, Data + 1, sizeof(int));
            }
            tinygltf::Value value(intValue);
            testValueFunctions(value);
            break;
        }
        case 1: {
            // Create real (double) value
            double realValue = 0.0;
            if (Size >= sizeof(double) + 1) {
                memcpy(&realValue, Data + 1, sizeof(double));
            }
            tinygltf::Value value(realValue);
            testValueFunctions(value);
            break;
        }
        case 2: {
            // Create array value
            tinygltf::Value::Array array;
            if (Size > 1) {
                // Add some integer values to array
                size_t maxElements = std::min(Size - 1, (size_t)10);
                for (size_t i = 0; i < maxElements; i++) {
                    array.push_back(tinygltf::Value(static_cast<int>(Data[i + 1])));
                }
            }
            tinygltf::Value value(array);
            testValueFunctions(value);
            break;
        }
        case 3: {
            // Create boolean value
            bool boolValue = false;
            if (Size > 1) {
                boolValue = (Data[1] % 2) == 0;
            }
            tinygltf::Value value(boolValue);
            testValueFunctions(value);
            break;
        }
        case 4: {
            // Create string value
            std::string strValue;
            if (Size > 1) {
                size_t strLen = std::min(Size - 1, (size_t)100);
                strValue.assign(reinterpret_cast<const char*>(Data + 1), strLen);
            }
            tinygltf::Value value(strValue);
            testValueFunctions(value);
            break;
        }
        case 5: {
            // Create null value
            tinygltf::Value value;
            testValueFunctions(value);
            break;
        }
    }
    
    return 0;
}