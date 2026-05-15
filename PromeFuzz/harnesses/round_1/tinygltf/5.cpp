// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
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
#include <vector>
#include <string>
#include <fstream>

#include "tiny_gltf.h"

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void testValueFunctions(const tinygltf::Value& value) {
    // Test all the query functions
    bool isBool = value.IsBool();
    bool isBinary = value.IsBinary();
    bool isArray = value.IsArray();
    bool isString = value.IsString();
    bool isReal = value.IsReal();
    
    // Suppress unused variable warnings
    (void)isBool;
    (void)isBinary;
    (void)isArray;
    (void)isString;
    (void)isReal;
    
    // Try to get values based on type
    if (value.IsBool()) {
        bool val = value.Get<bool>();
        (void)val;
    }
    if (value.IsReal()) {
        double val = value.Get<double>();
        (void)val;
    }
    if (value.IsString()) {
        std::string val = value.Get<std::string>();
        (void)val;
    }
    if (value.IsBinary()) {
        std::vector<unsigned char> val = value.Get<std::vector<unsigned char>>();
        (void)val;
    }
    if (value.IsArray()) {
        tinygltf::Value::Array val = value.Get<tinygltf::Value::Array>();
        (void)val;
    }
    
    // Try to get a value that might not match the type
    // This could trigger error conditions
    try {
        if (!value.IsBool()) {
            bool val = value.Get<bool>();
            (void)val;
        }
    } catch (...) {
        // Ignore exceptions
    }
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a dummy file for any potential file operations
    writeDummyFile(Data, Size);
    
    // Test 1: Create Value from string constructor
    try {
        // Use first byte to determine string length (max 255)
        size_t str_len = Data[0] % 256;
        if (str_len > Size - 1) {
            str_len = Size - 1;
        }
        if (str_len > 0) {
            std::string test_str(reinterpret_cast<const char*>(Data + 1), str_len);
            tinygltf::Value value(test_str.c_str());
            testValueFunctions(value);
        }
    } catch (...) {
        // Ignore exceptions during construction
    }
    
    // Test 2: Create different types of values using various constructors
    // We'll use the fuzz data to create different value types
    
    // Create a bool value
    try {
        bool bool_val = (Data[0] & 0x01) != 0;
        tinygltf::Value bool_value(bool_val);
        testValueFunctions(bool_value);
    } catch (...) {
        // Ignore exceptions
    }
    
    // Create an int value
    try {
        int int_val = static_cast<int>(Data[0]);
        tinygltf::Value int_value(int_val);
        testValueFunctions(int_value);
    } catch (...) {
        // Ignore exceptions
    }
    
    // Create a double value
    try {
        double double_val = static_cast<double>(Data[0]);
        tinygltf::Value double_value(double_val);
        testValueFunctions(double_value);
    } catch (...) {
        // Ignore exceptions
    }
    
    // Create a binary value using pointer and size constructor
    try {
        size_t binary_size = Size > 100 ? 100 : Size;
        tinygltf::Value binary_value(Data, binary_size);
        testValueFunctions(binary_value);
    } catch (...) {
        // Ignore exceptions
    }
    
    // Create a binary value using move constructor
    try {
        size_t binary_size = Size > 100 ? 100 : Size;
        std::vector<unsigned char> binary_data(Data, Data + binary_size);
        tinygltf::Value binary_value(std::move(binary_data));
        testValueFunctions(binary_value);
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 3: Create array and object values
    try {
        // Create an empty array
        tinygltf::Value::Array array_val;
        tinygltf::Value array_value(array_val);
        testValueFunctions(array_value);
        
        // Add some elements to the array
        array_val.push_back(tinygltf::Value(static_cast<int>(Data[0])));
        array_val.push_back(tinygltf::Value(static_cast<double>(Data[1])));
        if (Size > 2) {
            std::string str_val(reinterpret_cast<const char*>(Data + 2), 
                               Size > 10 ? 10 : Size - 2);
            array_val.push_back(tinygltf::Value(str_val.c_str()));
        }
        
        tinygltf::Value array_value2(array_val);
        testValueFunctions(array_value2);
        
        // Try to get array elements
        if (array_value2.IsArray()) {
            tinygltf::Value::Array retrieved_array = array_value2.Get<tinygltf::Value::Array>();
            if (!retrieved_array.empty()) {
                // Test getting elements from the array
                const tinygltf::Value& first_elem = retrieved_array[0];
                testValueFunctions(first_elem);
            }
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 4: Create object value
    try {
        tinygltf::Value::Object object_val;
        
        // Add some key-value pairs
        object_val["key1"] = tinygltf::Value(static_cast<int>(Data[0]));
        object_val["key2"] = tinygltf::Value(static_cast<double>(Data[1]));
        
        if (Size > 2) {
            std::string str_key(reinterpret_cast<const char*>(Data + 2), 
                               Size > 20 ? 10 : (Size - 2) / 2);
            std::string str_val(reinterpret_cast<const char*>(Data + 2 + str_key.length()), 
                               Size > 20 ? 10 : (Size - 2 - str_key.length()));
            object_val[str_key] = tinygltf::Value(str_val.c_str());
        }
        
        tinygltf::Value object_value(object_val);
        testValueFunctions(object_value);
        
        // Try to get object elements
        if (object_value.IsObject()) {
            tinygltf::Value::Object retrieved_obj = object_value.Get<tinygltf::Value::Object>();
            if (!retrieved_obj.empty()) {
                // Test getting a value from the object
                auto it = retrieved_obj.find("key1");
                if (it != retrieved_obj.end()) {
                    testValueFunctions(it->second);
                }
            }
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 5: Test with null or empty data
    try {
        // Empty string
        tinygltf::Value empty_str_value("");
        testValueFunctions(empty_str_value);
        
        // Empty binary using pointer and size constructor
        tinygltf::Value empty_binary_value(nullptr, 0);
        testValueFunctions(empty_binary_value);
        
        // Empty binary using move constructor
        std::vector<unsigned char> empty_binary_vec;
        tinygltf::Value empty_binary_value2(std::move(empty_binary_vec));
        testValueFunctions(empty_binary_value2);
        
        // Empty array
        tinygltf::Value::Array empty_array;
        tinygltf::Value empty_array_value(empty_array);
        testValueFunctions(empty_array_value);
        
        // Empty object
        tinygltf::Value::Object empty_object;
        tinygltf::Value empty_object_value(empty_object);
        testValueFunctions(empty_object_value);
    } catch (...) {
        // Ignore exceptions
    }
    
    return 0;
}