// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
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
#include <cstdlib>
#include <vector>
#include <string>
#include <array>

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Use a small portion of the input to decide which operations to perform
    if (Size == 0) return 0;
    
    // Create various Value objects to test
    std::string input_str(reinterpret_cast<const char*>(Data), Size % 1024); // Limit size
    
    // Test 1: Create a string Value and test its properties
    tinygltf::Value str_value(input_str);
    
    // Test Type() function
    char type = str_value.Type();
    
    // Test IsString() function
    bool is_string = str_value.IsString();
    
    // Test IsArray() function
    bool is_array = str_value.IsArray();
    
    // Test IsBool() function
    bool is_bool = str_value.IsBool();
    
    // Test IsBinary() function
    bool is_binary = str_value.IsBinary();
    
    // Try to Get the string value
    if (str_value.IsString()) {
        try {
            const std::string& retrieved_str = str_value.Get<std::string>();
            (void)retrieved_str; // Use to avoid unused variable warning
        } catch (...) {
            // Handle any exceptions
        }
    }
    
    // Test 2: Create other types of Value objects using available constructors
    // Create a bool Value
    bool bool_val = (Size % 2) == 0;
    tinygltf::Value bool_value(bool_val);
    
    // Create an int Value
    int int_val = static_cast<int>(Size);
    tinygltf::Value int_value(int_val);
    
    // Create a double Value
    double double_val = static_cast<double>(Size);
    tinygltf::Value double_value(double_val);
    
    // Test 3: Create an array Value
    tinygltf::Value::Array array_val;
    // Add some values to the array
    array_val.push_back(tinygltf::Value(input_str.substr(0, std::min<size_t>(10, input_str.size()))));
    
    // Create a Value from the array
    tinygltf::Value array_value(array_val);
    
    // Test array properties
    bool array_is_array = array_value.IsArray();
    bool array_is_string = array_value.IsString();
    
    // Try to get array elements
    if (array_value.IsArray()) {
        try {
            const tinygltf::Value::Array& retrieved_array = array_value.Get<tinygltf::Value::Array>();
            if (!retrieved_array.empty()) {
                const tinygltf::Value& first_elem = retrieved_array[0];
                (void)first_elem; // Use to avoid unused variable warning
            }
        } catch (...) {
            // Handle any exceptions
        }
    }
    
    // Test 4: Create an object Value
    tinygltf::Value::Object object_val;
    object_val["test_key"] = tinygltf::Value(input_str.substr(0, std::min<size_t>(5, input_str.size())));
    
    // Create a Value from the object
    tinygltf::Value object_value(object_val);
    
    // Try to get object values
    if (object_value.Type() == 'o') { // Object type
        try {
            const tinygltf::Value::Object& retrieved_object = object_value.Get<tinygltf::Value::Object>();
            auto it = retrieved_object.find("test_key");
            if (it != retrieved_object.end()) {
                (void)it->second; // Use to avoid unused variable warning
            }
        } catch (...) {
            // Handle any exceptions
        }
    }
    
    // Test 5: Create binary data Value using the correct constructor
    std::vector<unsigned char> binary_data;
    if (Size > 0) {
        size_t binary_size = std::min<size_t>(Size, 100);
        binary_data.assign(Data, Data + binary_size);
        // Use the constructor that takes a pointer and size
        tinygltf::Value binary_value(Data, binary_size);
        
        // Test binary properties
        bool binary_is_binary = binary_value.IsBinary();
        
        // Try to get binary data
        if (binary_value.IsBinary()) {
            try {
                const std::vector<unsigned char>& retrieved_binary = binary_value.Get<std::vector<unsigned char>>();
                (void)retrieved_binary; // Use to avoid unused variable warning
            } catch (...) {
                // Handle any exceptions
            }
        }
    }
    
    // Test 6: Create binary data Value using move constructor
    std::vector<unsigned char> binary_data2;
    if (Size > 0) {
        size_t binary_size2 = std::min<size_t>(Size, 50);
        binary_data2.assign(Data, Data + binary_size2);
        // Use the move constructor
        tinygltf::Value binary_value2(std::move(binary_data2));
        
        // Test binary properties
        bool binary_is_binary2 = binary_value2.IsBinary();
        
        // Try to get binary data
        if (binary_value2.IsBinary()) {
            try {
                const std::vector<unsigned char>& retrieved_binary2 = binary_value2.Get<std::vector<unsigned char>>();
                (void)retrieved_binary2; // Use to avoid unused variable warning
            } catch (...) {
                // Handle any exceptions
            }
        }
    }
    
    // Test with different input patterns
    uint8_t selector = Data[0];
    
    // Use selector to test different code paths
    switch (selector % 8) {
        case 0: {
            // Test empty string Value
            tinygltf::Value empty_str_value("");
            (void)empty_str_value.IsString();
            (void)empty_str_value.Type();
            break;
        }
            
        case 1: {
            // Test with longer string
            std::string long_str(1000, 'A');
            tinygltf::Value long_str_value(long_str);
            if (long_str_value.IsString()) {
                try {
                    (void)long_str_value.Get<std::string>();
                } catch (...) {
                    // Handle exceptions
                }
            }
            break;
        }
            
        case 2: {
            // Test array operations more thoroughly
            if (array_value.IsArray()) {
                try {
                    (void)array_value.Get<tinygltf::Value::Array>();
                } catch (...) {
                    // Handle exceptions
                }
            }
            break;
        }
            
        case 3: {
            // Test object operations more thoroughly
            if (object_value.Type() == 'o') {
                try {
                    (void)object_value.Get<tinygltf::Value::Object>();
                } catch (...) {
                    // Handle exceptions
                }
            }
            break;
        }
            
        case 4: {
            // Test binary operations more thoroughly
            if (Size > 0) {
                tinygltf::Value binary_test_value(Data, std::min<size_t>(Size, 10));
                if (binary_test_value.IsBinary()) {
                    try {
                        (void)binary_test_value.Get<std::vector<unsigned char>>();
                    } catch (...) {
                        // Handle exceptions
                    }
                }
            }
            break;
        }
            
        case 5: {
            // Test all Is* functions on different types
            (void)str_value.IsArray();
            (void)str_value.IsBool();
            (void)str_value.IsString();
            (void)str_value.IsBinary();
            (void)str_value.Type();
            
            (void)array_value.IsArray();
            (void)array_value.IsBool();
            (void)array_value.IsString();
            (void)array_value.IsBinary();
            (void)array_value.Type();
            break;
        }
            
        case 6: {
            // Test Get on mismatched types (should handle gracefully)
            if (!str_value.IsArray()) {
                try {
                    // This should not crash but might throw or return default
                    (void)str_value.Get<tinygltf::Value::Array>();
                } catch (...) {
                    // Expected to catch exceptions
                }
            }
            break;
        }
            
        case 7: {
            // Test with nested structures
            tinygltf::Value::Array nested_array;
            tinygltf::Value::Object nested_object;
            nested_object["nested"] = tinygltf::Value("nested_value");
            nested_array.push_back(tinygltf::Value(nested_object));
            
            tinygltf::Value nested_value(nested_array);
            if (nested_value.IsArray()) {
                try {
                    const tinygltf::Value::Array& arr = nested_value.Get<tinygltf::Value::Array>();
                    if (!arr.empty() && arr[0].Type() == 'o') {
                        const tinygltf::Value::Object& obj = arr[0].Get<tinygltf::Value::Object>();
                        auto it = obj.find("nested");
                        if (it != obj.end() && it->second.IsString()) {
                            (void)it->second.Get<std::string>();
                        }
                    }
                } catch (...) {
                    // Handle exceptions
                }
            }
            break;
        }
    }
    
    return 0;
}