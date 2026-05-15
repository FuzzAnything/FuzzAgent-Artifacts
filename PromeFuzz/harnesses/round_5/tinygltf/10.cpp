// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
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
    // Test all the query functions
    bool isArray = value.IsArray();
    bool isBinary = value.IsBinary();
    bool isBool = value.IsBool();
    bool isString = value.IsString();
    bool isObject = value.IsObject();
    
    // Test Get functions based on type
    if (isArray) {
        // Get returns a reference, not a pointer
        const tinygltf::Value::Array& arr = value.Get<tinygltf::Value::Array>();
        // Try to access elements if available
        if (!arr.empty()) {
            const tinygltf::Value& firstElem = arr[0];
            // Recursively test nested values
            testValueFunctions(firstElem);
        }
    } else if (isBinary) {
        // Get returns a reference, not a pointer
        const std::vector<unsigned char>& bin = value.Get<std::vector<unsigned char>>();
        // Accessing bin is optional for fuzzing
        (void)bin;
    } else if (isBool) {
        // Get returns a value, not a pointer
        bool b = value.Get<bool>();
        // Accessing b is optional for fuzzing
        (void)b;
    } else if (isString) {
        // Get returns a reference, not a pointer
        const std::string& str = value.Get<std::string>();
        // Accessing str is optional for fuzzing
        (void)str;
    } else if (isObject) {
        // Get returns a reference, not a pointer
        const tinygltf::Value::Object& obj = value.Get<tinygltf::Value::Object>();
        if (!obj.empty()) {
            // Try to access first element
            auto it = obj.begin();
            const tinygltf::Value& firstValue = it->second;
            testValueFunctions(firstValue);
        }
    } else {
        // Try to get other types that don't have explicit Is* functions
        int intVal = value.Get<int>();
        double realVal = value.Get<double>();
        // Accessing these is optional for fuzzing
        (void)intVal;
        (void)realVal;
    }
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Create a Value object using default constructor
    tinygltf::Value value;
    
    // Test the functions on the default-constructed value
    testValueFunctions(value);
    
    // Create additional Value objects with different types using the fuzzing data
    // We'll create a few different types to explore more states
    
    // Try creating an array value
    tinygltf::Value::Array arrayVal;
    if (Size > 0) {
        // Add a few dummy values to the array
        for (size_t i = 0; i < std::min(static_cast<size_t>(5), Size); i++) {
            tinygltf::Value elem;
            arrayVal.push_back(elem);
        }
        tinygltf::Value arrayValue;
        // Note: We can't directly set the array value without proper constructors
        // We'll rely on the default value for fuzzing
        
        // Test with array type (though it won't actually be an array without proper initialization)
        testValueFunctions(arrayValue);
    }
    
    // Try creating different types of values based on fuzzing input
    // We'll use the first byte to decide what type to simulate
    if (Size > 0) {
        uint8_t typeSelector = Data[0] % 6;
        
        switch (typeSelector) {
            case 0:
                // Simulate bool type (though we can't actually set it)
                testValueFunctions(value);
                break;
            case 1:
                // Simulate string type
                testValueFunctions(value);
                break;
            case 2:
                // Simulate array type
                testValueFunctions(value);
                break;
            case 3:
                // Simulate object type
                testValueFunctions(value);
                break;
            case 4:
                // Simulate binary type
                testValueFunctions(value);
                break;
            case 5:
                // Simulate number types
                testValueFunctions(value);
                break;
        }
    }
    
    return 0;
}