// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
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
#include <utility>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <map>

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Use the input data to create a Value object with different types
    if (Size == 0) {
        return 0;
    }
    
    // Use first byte to determine the type of Value to create
    uint8_t type_selector = Data[0] % 8;
    
    try {
        tinygltf::Value value;
        
        switch (type_selector) {
            case 0: {
                // Create a boolean value
                bool bool_val = false;
                if (Size > 1) {
                    bool_val = (Data[1] % 2) == 0;
                }
                value = tinygltf::Value(bool_val);
                break;
            }
            case 1: {
                // Create a binary value
                std::vector<unsigned char> binary_data;
                if (Size > 1) {
                    size_t binary_size = std::min(Size - 1, static_cast<size_t>(100));
                    binary_data.assign(Data + 1, Data + 1 + binary_size);
                }
                // Use move constructor for binary data
                value = tinygltf::Value(std::move(binary_data));
                break;
            }
            case 2: {
                // Create an array value
                tinygltf::Value::Array array_val;
                if (Size > 1) {
                    // Add a few dummy values to the array
                    size_t array_size = std::min(Size - 1, static_cast<size_t>(5));
                    for (size_t i = 0; i < array_size; i++) {
                        array_val.push_back(tinygltf::Value(static_cast<int>(Data[1 + i])));
                    }
                }
                value = tinygltf::Value(std::move(array_val));
                break;
            }
            case 3: {
                // Create a string value
                std::string str_val;
                if (Size > 1) {
                    size_t str_size = std::min(Size - 1, static_cast<size_t>(100));
                    str_val.assign(reinterpret_cast<const char*>(Data + 1), str_size);
                }
                value = tinygltf::Value(std::move(str_val));
                break;
            }
            case 4: {
                // Create an object value
                tinygltf::Value::Object obj_val;
                if (Size > 1) {
                    // Add a few key-value pairs
                    size_t obj_size = std::min(Size - 1, static_cast<size_t>(5));
                    for (size_t i = 0; i < obj_size; i++) {
                        std::string key = "key" + std::to_string(i);
                        obj_val[key] = tinygltf::Value(static_cast<int>(Data[1 + i]));
                    }
                }
                value = tinygltf::Value(std::move(obj_val));
                break;
            }
            case 5: {
                // Create an integer value
                int int_val = 0;
                if (Size > 1) {
                    int_val = static_cast<int>(Data[1]);
                }
                value = tinygltf::Value(int_val);
                break;
            }
            case 6: {
                // Create a real (double) value
                double real_val = 0.0;
                if (Size > 1) {
                    real_val = static_cast<double>(Data[1]);
                }
                value = tinygltf::Value(real_val);
                break;
            }
            case 7: {
                // Create a null value (default constructor)
                value = tinygltf::Value();
                break;
            }
        }
        
        // Test the API functions
        bool is_bool = value.IsBool();
        bool is_binary = value.IsBinary();
        bool is_array = value.IsArray();
        bool is_string = value.IsString();
        bool is_object = value.IsObject();
        
        // Try to get values based on type checks
        if (is_bool) {
            bool bool_result = value.Get<bool>();
            (void)bool_result;
        }
        
        if (is_binary) {
            const std::vector<unsigned char>& binary_result = value.Get<std::vector<unsigned char>>();
            (void)binary_result;
        }
        
        if (is_array) {
            const tinygltf::Value::Array& array_result = value.Get<tinygltf::Value::Array>();
            (void)array_result;
            
            // Try to get elements from the array if it's not empty
            if (!array_result.empty()) {
                const tinygltf::Value& elem = value.Get(0);
                (void)elem;
            }
        }
        
        if (is_string) {
            const std::string& string_result = value.Get<std::string>();
            (void)string_result;
        }
        
        if (is_object) {
            const tinygltf::Value::Object& object_result = value.Get<tinygltf::Value::Object>();
            (void)object_result;
            
            // Try to get a value by key if object is not empty
            if (!object_result.empty()) {
                // Use a key that might exist
                const tinygltf::Value& obj_elem = value.Get("key0");
                (void)obj_elem;
            }
        }
        
        // Also try to get other types (might fail, but that's OK for fuzzing)
        try {
            int int_result = value.Get<int>();
            (void)int_result;
        } catch (...) {}
        
        try {
            double real_result = value.Get<double>();
            (void)real_result;
        } catch (...) {}
        
    } catch (...) {
        // Catch any exceptions to prevent crashes from propagating
    }
    
    return 0;
}