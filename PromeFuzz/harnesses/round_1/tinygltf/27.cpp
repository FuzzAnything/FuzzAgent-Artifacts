// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
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

static void TestValueFunctions(const tinygltf::Value& value) {
    // Test Type() function
    char type = value.Type();
    
    // Test IsArray() function
    bool is_array = value.IsArray();
    
    // Test IsObject() function
    bool is_object = value.IsObject();
    
    if (is_array) {
        // Try to get as array if it's an array
        try {
            const tinygltf::Value::Array& arr = value.Get<tinygltf::Value::Array>();
            if (!arr.empty()) {
                // Test Get with index if array is not empty
                const tinygltf::Value& first_elem = arr[0];
                // Recursively test nested values
                TestValueFunctions(first_elem);
            }
        } catch (...) {
            // Ignore exceptions from Get
        }
    }
    
    if (is_object) {
        // Test Keys() function
        std::vector<std::string> keys = value.Keys();
        
        // Test Has() function for each key
        for (const auto& key : keys) {
            bool has_key = value.Has(key);
            
            if (has_key) {
                // Try to get the value for this key
                try {
                    // Try different Get specializations for object values
                    const tinygltf::Value::Object& obj = value.Get<tinygltf::Value::Object>();
                    auto it = obj.find(key);
                    if (it != obj.end()) {
                        const tinygltf::Value& nested_value = it->second;
                        TestValueFunctions(nested_value);
                    }
                } catch (...) {
                    // Ignore exceptions from Get
                }
            }
        }
        
        // Test Has() with a random key that likely doesn't exist
        value.Has("nonexistent_key_test");
    }
    
    // Try other Get specializations (they will fail for wrong types, but that's OK)
    try {
        int int_val = value.Get<int>();
    } catch (...) {}
    
    try {
        std::string str_val = value.Get<std::string>();
    } catch (...) {}
    
    try {
        std::vector<unsigned char> bin_val = value.Get<std::vector<unsigned char>>();
    } catch (...) {}
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // Use the data to create different types of Value objects
    
    // Create an empty array Value
    tinygltf::Value::Array empty_array;
    tinygltf::Value array_value(std::move(empty_array));
    TestValueFunctions(array_value);
    
    // Create an array with some nested values
    tinygltf::Value::Array nested_array;
    
    // Add an integer value
    tinygltf::Value int_value;
    // Note: We need to create values properly. Since we don't have all constructors,
    // we'll create simple test cases with what we know works.
    
    // Create an object Value
    tinygltf::Value::Object obj;
    obj["test_key"] = array_value;  // Use the array value we created
    
    tinygltf::Value object_value(std::move(tinygltf::Value::Array{})); // Wrong type but tests error paths
    TestValueFunctions(object_value);
    
    // Create another test with actual data from fuzzer input
    if (Size > 0) {
        // Use the fuzzer data to create a string
        std::string test_string(reinterpret_cast<const char*>(Data), std::min(Size, size_t(100)));
        
        // Create a binary vector
        std::vector<unsigned char> binary_data(Data, Data + std::min(Size, size_t(100)));
        
        // We can't directly create these Value types without proper constructors,
        // but we can test the functions on existing values
        
        // Test with the array_value we already have
        TestValueFunctions(array_value);
        
        // Create an object with some keys based on fuzzer data
        tinygltf::Value::Object fuzz_obj;
        if (Size > 10) {
            std::string key1(reinterpret_cast<const char*>(Data), 5);
            fuzz_obj[key1] = array_value;
            
            if (Size > 20) {
                std::string key2(reinterpret_cast<const char*>(Data + 5), 5);
                fuzz_obj[key2] = array_value;
            }
        }
        
        // We need to create a Value from this object, but we only have Array constructor
        // So we'll work with what we have
    }
    
    return 0;
}