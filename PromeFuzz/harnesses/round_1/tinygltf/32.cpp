// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
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
#include <cstring>
#include <fstream>

static void createDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void cleanupDummyFile() {
    remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* Data, size_t Size) {
    // Create dummy file with fuzzer data
    createDummyFile(Data, Size);
    
    try {
        // Create a Value object using Object constructor
        tinygltf::Value::Object obj;
        tinygltf::Value value(obj);
        
        // Test Type() function
        char type = value.Type();
        (void)type; // Suppress unused variable warning
        
        // Test IsInt() function
        bool isInt = value.IsInt();
        (void)isInt;
        
        // Test IsNumber() function
        bool isNumber = value.IsNumber();
        (void)isNumber;
        
        // Test IsReal() function
        bool isReal = value.IsReal();
        (void)isReal;
        
        // Test GetNumberAsInt() function
        // Note: This may throw or return default value for non-numeric types
        try {
            int numAsInt = value.GetNumberAsInt();
            (void)numAsInt;
        } catch (...) {
            // Handle any exceptions
        }
        
        // Test GetNumberAsDouble() function
        // Note: This may throw or return default value for non-numeric types
        try {
            double numAsDouble = value.GetNumberAsDouble();
            (void)numAsDouble;
        } catch (...) {
            // Handle any exceptions
        }
        
        // Create additional Value objects with different data to explore more states
        // Using fuzzer data to create different scenarios
        if (Size > 0) {
            // Try to create Value with integer type if we have enough data
            if (Size >= sizeof(int)) {
                int intValue;
                std::memcpy(&intValue, Data, sizeof(int));
                // Note: We need to create a Value with int type
                // Since we don't have the int constructor in the provided info,
                // we'll work with the Object type we can create
            }
            
            // Try different object configurations
            if (Size > 1) {
                // Create another object with different content
                tinygltf::Value::Object obj2;
                std::string key = "test";
                if (Size > key.size() + 1) {
                    // Add some entries to the object
                    tinygltf::Value nestedValue(obj);
                    obj2[key] = nestedValue;
                    
                    tinygltf::Value value2(obj2);
                    
                    // Test functions on the new value
                    value2.Type();
                    value2.IsInt();
                    value2.IsNumber();
                    value2.IsReal();
                    
                    try {
                        value2.GetNumberAsInt();
                    } catch (...) {}
                    
                    try {
                        value2.GetNumberAsDouble();
                    } catch (...) {}
                }
            }
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    // Cleanup
    cleanupDummyFile();
    
    return 0;
}