// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
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

static void test_value_functions(tinygltf::Value& value) {
    // Test Type() function
    char type = value.Type();
    
    // Test ArrayLen() function
    size_t array_len = value.ArrayLen();
    
    // Test Keys() function - only meaningful for objects
    std::vector<std::string> keys = value.Keys();
    
    // Test Has() function - only meaningful for objects
    if (value.Type() == 'o') { // 'o' indicates object type
        for (const auto& key : keys) {
            bool has_key = value.Has(key);
            // Also test with a non-existent key
            bool has_fake = value.Has("__fake_key_that_should_not_exist__");
        }
    }
    
    // Test Get() functions
    // First overload: Get(const std::string& key) for objects
    if (value.Type() == 'o') {
        for (const auto& key : keys) {
            const tinygltf::Value& retrieved = value.Get(key);
            // Access the retrieved value's type to ensure it's used
            char retrieved_type = retrieved.Type();
        }
    }
    
    // Second overload: Get() for array access
    if (value.Type() == 'a') { // 'a' indicates array type
        for (size_t i = 0; i < array_len && i < 10; ++i) {
            // Try to get by index (using the array Get overload)
            // Note: tinygltf::Value has Get(int index) for arrays
            const tinygltf::Value& array_elem = value.Get(static_cast<int>(i));
            char elem_type = array_elem.Type();
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Create various types of Value objects to test the functions
    
    // 1. Create an array Value
    tinygltf::Value::Array array_val;
    // Add some dummy values to the array
    array_val.push_back(tinygltf::Value(42));
    array_val.push_back(tinygltf::Value(3.14));
    array_val.push_back(tinygltf::Value("test_string"));
    
    tinygltf::Value array_value(array_val);
    test_value_functions(array_value);
    
    // 2. Create an object Value
    tinygltf::Value::Object object_val;
    object_val["integer"] = tinygltf::Value(100);
    object_val["double"] = tinygltf::Value(2.718);
    object_val["string"] = tinygltf::Value("hello");
    
    // Create an inner array for nested testing
    tinygltf::Value::Array inner_array;
    inner_array.push_back(tinygltf::Value(1));
    inner_array.push_back(tinygltf::Value(2));
    object_val["nested_array"] = tinygltf::Value(inner_array);
    
    tinygltf::Value object_value(object_val);
    test_value_functions(object_value);
    
    // 3. Test with primitive types as well
    tinygltf::Value int_value(123);
    test_value_functions(int_value);
    
    tinygltf::Value double_value(45.67);
    test_value_functions(double_value);
    
    tinygltf::Value string_value("fuzz_test");
    test_value_functions(string_value);
    
    tinygltf::Value bool_value(true);
    test_value_functions(bool_value);
    
    // 4. Create an empty array and object
    tinygltf::Value::Array empty_array;
    tinygltf::Value empty_array_value(empty_array);
    test_value_functions(empty_array_value);
    
    tinygltf::Value::Object empty_object;
    tinygltf::Value empty_object_value(empty_object);
    test_value_functions(empty_object_value);
    
    // 5. Use fuzzer data to create more diverse values
    if (Size > 0) {
        // Use first byte to determine value type
        uint8_t value_type = Data[0] % 6;
        
        switch (value_type) {
            case 0: {
                // Create integer from data
                int int_val = Size > 1 ? static_cast<int>(Data[1]) : 0;
                tinygltf::Value fuzz_int_value(int_val);
                test_value_functions(fuzz_int_value);
                break;
            }
            case 1: {
                // Create double from data
                double double_val = Size > 1 ? static_cast<double>(Data[1]) / 255.0 : 0.0;
                tinygltf::Value fuzz_double_value(double_val);
                test_value_functions(fuzz_double_value);
                break;
            }
            case 2: {
                // Create string from data (limited to safe length)
                size_t str_len = Size > 1 ? std::min(static_cast<size_t>(Data[1]), Size - 2) : 0;
                if (str_len > 0 && 2 + str_len <= Size) {
                    std::string fuzz_str(reinterpret_cast<const char*>(Data + 2), str_len);
                    tinygltf::Value fuzz_string_value(fuzz_str);
                    test_value_functions(fuzz_string_value);
                }
                break;
            }
            case 3: {
                // Create array with fuzz data
                tinygltf::Value::Array fuzz_array;
                size_t num_elements = Size > 1 ? std::min(static_cast<size_t>(Data[1]), 10UL) : 0;
                for (size_t i = 0; i < num_elements && 2 + i < Size; ++i) {
                    fuzz_array.push_back(tinygltf::Value(static_cast<int>(Data[2 + i])));
                }
                tinygltf::Value fuzz_array_value(fuzz_array);
                test_value_functions(fuzz_array_value);
                break;
            }
            case 4: {
                // Create object with fuzz data
                tinygltf::Value::Object fuzz_object;
                size_t num_pairs = Size > 1 ? std::min(static_cast<size_t>(Data[1]), 5UL) : 0;
                for (size_t i = 0; i < num_pairs; ++i) {
                    size_t key_offset = 2 + i * 3;
                    size_t val_offset = key_offset + 1;
                    
                    if (val_offset + 1 < Size) {
                        std::string key = "key" + std::to_string(Data[key_offset]);
                        int value = static_cast<int>(Data[val_offset]);
                        fuzz_object[key] = tinygltf::Value(value);
                    }
                }
                tinygltf::Value fuzz_object_value(fuzz_object);
                test_value_functions(fuzz_object_value);
                break;
            }
            case 5: {
                // Create boolean from data
                bool bool_val = Size > 1 ? (Data[1] % 2 == 0) : false;
                tinygltf::Value fuzz_bool_value(bool_val);
                test_value_functions(fuzz_bool_value);
                break;
            }
        }
    }
    
    return 0;
}