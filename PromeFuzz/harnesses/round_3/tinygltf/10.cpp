// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
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
#include <cstring>
#include <string>
#include <vector>
#include <array>

static void test_value_apis(const uint8_t *data, size_t size) {
    // Create an object with some random data
    tinygltf::Value::Object obj;
    
    // Add some keys with different value types based on fuzzer input
    if (size > 0) {
        // Create a string key from first byte
        std::string key1 = "key" + std::to_string(data[0] % 10);
        
        // Add a boolean value
        obj[key1] = tinygltf::Value(static_cast<bool>(data[0] & 1));
    }
    
    if (size > 1) {
        std::string key2 = "key" + std::to_string(data[1] % 10);
        
        // Add an integer value
        obj[key2] = tinygltf::Value(static_cast<int>(data[1]));
    }
    
    if (size > 2) {
        std::string key3 = "key" + std::to_string(data[2] % 10);
        
        // Add a double value
        obj[key3] = tinygltf::Value(static_cast<double>(data[2]));
    }
    
    if (size > 3) {
        std::string key4 = "key" + std::to_string(data[3] % 10);
        
        // Add a string value
        std::string str_val = "str" + std::to_string(data[3]);
        obj[key4] = tinygltf::Value(str_val);
    }
    
    if (size > 4) {
        std::string key5 = "key" + std::to_string(data[4] % 10);
        
        // Add a binary value - need to use move constructor
        std::vector<unsigned char> binary_data;
        size_t binary_size = size > 100 ? 100 : size;
        for (size_t i = 0; i < binary_size && i < size; i++) {
            binary_data.push_back(data[i]);
        }
        obj[key5] = tinygltf::Value(std::move(binary_data));
    }
    
    if (size > 5) {
        std::string key6 = "key" + std::to_string(data[5] % 10);
        
        // Add an array value
        tinygltf::Value::Array arr;
        size_t arr_size = (data[5] % 5) + 1;
        for (size_t i = 0; i < arr_size; i++) {
            arr.push_back(tinygltf::Value(static_cast<int>(i)));
        }
        obj[key6] = tinygltf::Value(std::move(arr));
    }
    
    if (size > 6) {
        std::string key7 = "key" + std::to_string(data[6] % 10);
        
        // Add a nested object
        tinygltf::Value::Object nested_obj;
        nested_obj["nested_key"] = tinygltf::Value("nested_value");
        obj[key7] = tinygltf::Value(std::move(nested_obj));
    }
    
    // Create Value object using the provided constructor
    tinygltf::Value value(std::move(obj));
    
    // Test IsObject()
    bool is_obj = value.IsObject();
    
    // Test Has() for various keys
    for (int i = 0; i < 10 && i < static_cast<int>(size); i++) {
        std::string test_key = "key" + std::to_string(i);
        bool has_key = value.Has(test_key);
    }
    
    // Test ArrayLen() - should return 0 for object
    size_t array_len = value.ArrayLen();
    
    // Test IsArray() - should return false for object
    bool is_array = value.IsArray();
    
    // Test typed Get() functions
    try {
        // Try to get as boolean
        if (size > 0) {
            std::string bool_key = "key" + std::to_string(data[0] % 10);
            if (value.Has(bool_key)) {
                bool bool_val = value.Get(bool_key).Get<bool>();
            }
        }
        
        // Try to get as integer
        if (size > 1) {
            std::string int_key = "key" + std::to_string(data[1] % 10);
            if (value.Has(int_key)) {
                int int_val = value.Get(int_key).Get<int>();
            }
        }
        
        // Try to get as double
        if (size > 2) {
            std::string double_key = "key" + std::to_string(data[2] % 10);
            if (value.Has(double_key)) {
                double double_val = value.Get(double_key).Get<double>();
            }
        }
        
        // Try to get as string
        if (size > 3) {
            std::string str_key = "key" + std::to_string(data[3] % 10);
            if (value.Has(str_key)) {
                std::string str_val = value.Get(str_key).Get<std::string>();
            }
        }
        
        // Try to get as binary
        if (size > 4) {
            std::string bin_key = "key" + std::to_string(data[4] % 10);
            if (value.Has(bin_key)) {
                std::vector<unsigned char> bin_val = value.Get(bin_key).Get<std::vector<unsigned char>>();
            }
        }
        
        // Try to get as array
        if (size > 5) {
            std::string arr_key = "key" + std::to_string(data[5] % 10);
            if (value.Has(arr_key)) {
                tinygltf::Value::Array arr_val = value.Get(arr_key).Get<tinygltf::Value::Array>();
                
                // Test array operations on the array value
                if (!arr_val.empty()) {
                    const tinygltf::Value& arr_elem = value.Get(arr_key);
                    bool arr_is_array = arr_elem.IsArray();
                    size_t arr_len = arr_elem.ArrayLen();
                    
                    if (arr_len > 0) {
                        size_t arr_idx = static_cast<size_t>(data[5]) % arr_len;
                        const tinygltf::Value& elem = arr_elem.Get(arr_idx);
                    }
                }
            }
        }
        
        // Try to get as object
        if (size > 6) {
            std::string obj_key = "key" + std::to_string(data[6] % 10);
            if (value.Has(obj_key)) {
                tinygltf::Value::Object obj_val = value.Get(obj_key).Get<tinygltf::Value::Object>();
            }
        }
    } catch (...) {
        // Ignore exceptions from invalid type conversions
    }
    
    // Create an array value to test array-specific functions
    tinygltf::Value::Array test_array;
    size_t test_arr_size = (size % 10) + 1;
    for (size_t i = 0; i < test_arr_size; i++) {
        int val = static_cast<int>((i < size ? data[i] : 0) + i);
        test_array.push_back(tinygltf::Value(val));
    }
    
    tinygltf::Value array_value(std::move(test_array));
    
    // Test array-specific functions
    bool array_is_array = array_value.IsArray();
    bool array_is_object = array_value.IsObject();
    size_t actual_array_len = array_value.ArrayLen();
    
    // Test Get() with various indices on array - only if it's an array
    if (array_is_array && actual_array_len > 0) {
        for (size_t i = 0; i < actual_array_len && i < 5; i++) {
            const tinygltf::Value& elem = array_value.Get(i);
            
            // Try to get the integer value
            try {
                int int_elem = elem.Get<int>();
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // Test Has() on array (should return false)
    if (size > 0) {
        std::string test_key = "test" + std::to_string(data[0] % 10);
        bool has_on_array = array_value.Has(test_key);
    }
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    test_value_apis(Data, Size);
    
    return 0;
}