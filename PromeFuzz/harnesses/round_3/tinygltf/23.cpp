// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <array>

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Create a string from the input data
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Test 1: Create a Value object from string constructor
    tinygltf::Value string_value(input_str.c_str());
    
    // Test IsString on the string value
    bool is_string = string_value.IsString();
    
    // Test other type checks on the string value
    bool is_array = string_value.IsArray();
    bool is_bool = string_value.IsBool();
    bool is_real = string_value.IsReal();
    bool is_binary = string_value.IsBinary();
    
    // Try to get the string value
    if (is_string) {
        try {
            std::string retrieved_str = string_value.Get<std::string>();
        } catch (...) {
            // Ignore exceptions during fuzzing
        }
    }
    
    // Test 2: Create different types of Value objects to test more states
    // Create a boolean value
    tinygltf::Value bool_value(true);
    bool bool_is_bool = bool_value.IsBool();
    bool bool_is_real = bool_value.IsReal();
    
    if (bool_is_bool) {
        try {
            bool retrieved_bool = bool_value.Get<bool>();
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Create a real (double) value
    tinygltf::Value real_value(3.14159);
    bool real_is_real = real_value.IsReal();
    bool real_is_bool = real_value.IsBool();
    
    if (real_is_real) {
        try {
            double retrieved_real = real_value.Get<double>();
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Create an integer value
    tinygltf::Value int_value(42);
    bool int_is_real = int_value.IsReal();
    
    if (!int_is_real) { // Integer is not REAL_TYPE
        try {
            int retrieved_int = int_value.Get<int>();
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 3: Create an array value
    tinygltf::Value::Array array_data;
    // Add some values to the array
    array_data.push_back(tinygltf::Value(1));
    array_data.push_back(tinygltf::Value(2.0));
    array_data.push_back(tinygltf::Value("test"));
    
    tinygltf::Value array_value(array_data);
    bool array_is_array = array_value.IsArray();
    
    if (array_is_array) {
        try {
            // Try to get the array
            tinygltf::Value::Array retrieved_array = array_value.Get<tinygltf::Value::Array>();
            
            // Try to access elements in the array
            if (retrieved_array.size() > 0) {
                // Access first element
                tinygltf::Value first_element = retrieved_array[0];
                bool first_is_int = first_element.IsReal();
                
                // Try to get the integer value
                if (!first_is_int) { // Integer is not REAL_TYPE
                    int first_int = first_element.Get<int>();
                }
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 4: Create an object value
    tinygltf::Value::Object object_data;
    object_data["key1"] = tinygltf::Value("value1");
    object_data["key2"] = tinygltf::Value(123);
    
    tinygltf::Value object_value(object_data);
    bool object_is_array = object_value.IsArray();
    
    if (!object_is_array) { // Object is not an array
        try {
            // Try to get the object
            tinygltf::Value::Object retrieved_object = object_value.Get<tinygltf::Value::Object>();
            
            // Try to access values in the object
            if (retrieved_object.find("key1") != retrieved_object.end()) {
                tinygltf::Value key1_value = retrieved_object["key1"];
                bool key1_is_string = key1_value.IsString();
                
                if (key1_is_string) {
                    std::string key1_str = key1_value.Get<std::string>();
                }
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 5: Create a binary value - FIXED: Use the correct constructor
    std::vector<unsigned char> binary_data;
    if (Size > 0) {
        // Use the input data as binary data
        binary_data.assign(Data, Data + Size);
    }
    
    // Create binary value using the correct constructor
    tinygltf::Value binary_value(binary_data.data(), binary_data.size());
    bool binary_is_binary = binary_value.IsBinary();
    
    if (binary_is_binary) {
        try {
            std::vector<unsigned char> retrieved_binary = binary_value.Get<std::vector<unsigned char>>();
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 6: Test ColorValue type (array of 4 doubles)
    tinygltf::ColorValue color_data = {1.0, 0.5, 0.0, 1.0};
    // Note: ColorValue is just an alias for std::array<double, 4>
    // We need to wrap it in a Value object
    tinygltf::Value::Array color_array;
    for (double val : color_data) {
        color_array.push_back(tinygltf::Value(val));
    }
    tinygltf::Value color_value(color_array);
    
    bool color_is_array = color_value.IsArray();
    if (color_is_array) {
        try {
            tinygltf::Value::Array retrieved_color_array = color_value.Get<tinygltf::Value::Array>();
            if (retrieved_color_array.size() >= 4) {
                // Check if elements are real numbers
                for (size_t i = 0; i < 4; i++) {
                    bool is_real_val = retrieved_color_array[i].IsReal();
                    if (is_real_val) {
                        double color_component = retrieved_color_array[i].Get<double>();
                    }
                }
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    return 0;
}