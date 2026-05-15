// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <map>

static void test_value_functions(const tinygltf::Value& value) {
    // Test ArrayLen
    size_t array_len = value.ArrayLen();
    (void)array_len;
    
    // Test Type
    char type = value.Type();
    (void)type;
    
    // Test Size
    size_t size = value.Size();
    (void)size;
    
    // Test IsObject
    bool is_object = value.IsObject();
    (void)is_object;
    
    // Test Keys
    std::vector<std::string> keys = value.Keys();
    (void)keys;
    
    // Test Has for some sample keys
    if (is_object && !keys.empty()) {
        for (const auto& key : keys) {
            bool has_key = value.Has(key);
            (void)has_key;
        }
    }
    
    // Test Has for a non-existent key
    bool has_nonexistent = value.Has("nonexistent_key");
    (void)has_nonexistent;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Create different types of Value objects to test
    
    // 1. Create an empty object
    tinygltf::Value::Object empty_obj;
    tinygltf::Value empty_obj_value(std::move(empty_obj));
    test_value_functions(empty_obj_value);
    
    // 2. Create an object with some random keys using fuzzer data
    if (Size > 0) {
        tinygltf::Value::Object obj;
        
        // Use fuzzer data to create keys and values
        size_t offset = 0;
        while (offset + 2 < Size) {
            // Extract key length (1 byte)
            uint8_t key_len = Data[offset] % 16 + 1; // 1-16 chars
            offset++;
            
            if (offset + key_len >= Size) break;
            
            // Create key from fuzzer data
            std::string key(reinterpret_cast<const char*>(Data + offset), 
                           std::min(key_len, static_cast<uint8_t>(Size - offset)));
            offset += key_len;
            
            if (offset >= Size) break;
            
            // Create a simple integer value
            int int_val = static_cast<int>(Data[offset]);
            offset++;
            
            // Add to object
            obj[key] = tinygltf::Value(int_val);
        }
        
        tinygltf::Value obj_value(std::move(obj));
        test_value_functions(obj_value);
    }
    
    // 3. Create nested objects
    if (Size > 0) {
        tinygltf::Value::Object nested_obj;
        
        // Create a nested object
        tinygltf::Value::Object inner_obj;
        if (Size > 1) {
            inner_obj["inner_key"] = tinygltf::Value(static_cast<int>(Data[0]));
        }
        
        nested_obj["nested"] = tinygltf::Value(std::move(inner_obj));
        
        if (Size > 2) {
            nested_obj["simple"] = tinygltf::Value(static_cast<int>(Data[1]));
        }
        
        tinygltf::Value nested_obj_value(std::move(nested_obj));
        test_value_functions(nested_obj_value);
    }
    
    // 4. Create object with array values
    if (Size > 0) {
        tinygltf::Value::Object obj_with_array;
        
        // Create an array
        tinygltf::Value::Array array;
        for (size_t i = 0; i < std::min(static_cast<size_t>(5), Size); i++) {
            array.push_back(tinygltf::Value(static_cast<int>(Data[i])));
        }
        
        obj_with_array["array"] = tinygltf::Value(std::move(array));
        
        if (Size > 1) {
            obj_with_array["number"] = tinygltf::Value(static_cast<int>(Data[0]));
        }
        
        tinygltf::Value array_obj_value(std::move(obj_with_array));
        test_value_functions(array_obj_value);
    }
    
    // 5. Create object with boolean values
    if (Size > 0) {
        tinygltf::Value::Object bool_obj;
        
        bool_obj["true_val"] = tinygltf::Value(true);
        bool_obj["false_val"] = tinygltf::Value(false);
        
        if (Size > 1) {
            bool_obj["dynamic_bool"] = tinygltf::Value(Data[0] % 2 == 0);
        }
        
        tinygltf::Value bool_obj_value(std::move(bool_obj));
        test_value_functions(bool_obj_value);
    }
    
    // 6. Create object with string values
    if (Size > 0) {
        tinygltf::Value::Object string_obj;
        
        // Use fuzzer data to create string values
        size_t str_len = std::min(static_cast<size_t>(32), Size);
        std::string fuzz_str(reinterpret_cast<const char*>(Data), str_len);
        
        string_obj["fuzz_string"] = tinygltf::Value(fuzz_str);
        string_obj["empty_string"] = tinygltf::Value(std::string(""));
        string_obj["static_string"] = tinygltf::Value(std::string("test"));
        
        tinygltf::Value string_obj_value(std::move(string_obj));
        test_value_functions(string_obj_value);
    }
    
    // 7. Create object with double values
    if (Size > 0) {
        tinygltf::Value::Object double_obj;
        
        double_obj["zero"] = tinygltf::Value(0.0);
        double_obj["one"] = tinygltf::Value(1.0);
        
        if (Size > 1) {
            double fuzz_double = static_cast<double>(Data[0]) / 256.0;
            double_obj["fuzz_double"] = tinygltf::Value(fuzz_double);
        }
        
        tinygltf::Value double_obj_value(std::move(double_obj));
        test_value_functions(double_obj_value);
    }
    
    // 8. Create object with mixed types
    if (Size > 0) {
        tinygltf::Value::Object mixed_obj;
        
        mixed_obj["int"] = tinygltf::Value(42);
        mixed_obj["double"] = tinygltf::Value(3.14);
        mixed_obj["bool"] = tinygltf::Value(true);
        mixed_obj["string"] = tinygltf::Value(std::string("mixed"));
        
        // Add nested object
        tinygltf::Value::Object inner_mixed;
        inner_mixed["inner_int"] = tinygltf::Value(100);
        mixed_obj["nested"] = tinygltf::Value(std::move(inner_mixed));
        
        // Add array
        tinygltf::Value::Array mixed_array;
        mixed_array.push_back(tinygltf::Value(1));
        mixed_array.push_back(tinygltf::Value(2.0));
        mixed_array.push_back(tinygltf::Value(std::string("three")));
        mixed_obj["mixed_array"] = tinygltf::Value(std::move(mixed_array));
        
        tinygltf::Value mixed_obj_value(std::move(mixed_obj));
        test_value_functions(mixed_obj_value);
    }
    
    return 0;
}