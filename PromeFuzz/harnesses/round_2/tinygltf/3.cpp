// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:412:1 in tiny_gltf.h
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
#include <vector>
#include <string>
#include <array>

static void fuzzValueCreation(const uint8_t* data, size_t size) {
    if (size < 1) return;
    
    // Create different types of Value objects based on input data
    uint8_t type_selector = data[0] % 7;
    
    switch (type_selector) {
        case 0: {
            // Integer value
            int int_val = static_cast<int>(data[0]);
            tinygltf::Value val(int_val);
            
            // Test Is* methods
            bool is_obj = val.IsObject();
            bool is_arr = val.IsArray();
            bool is_bool = val.IsBool();
            bool is_binary = val.IsBinary();
            bool is_string = val.IsString();
            
            // Try to get value (should work for int)
            if (val.Get<int>() == int_val) {
                // Successfully retrieved integer
            }
            break;
        }
        case 1: {
            // Boolean value (simulated)
            bool bool_val = (data[0] & 0x01) != 0;
            // Note: We don't have boolean constructor in provided info
            // Using integer constructor as workaround
            tinygltf::Value val(static_cast<int>(bool_val));
            
            // Test Is* methods
            val.IsObject();
            val.IsArray();
            val.IsBool();
            val.IsBinary();
            val.IsString();
            break;
        }
        case 2: {
            // Create an array Value
            tinygltf::Value::Array arr;
            if (size > 1) {
                for (size_t i = 1; i < std::min(size, static_cast<size_t>(10)); i++) {
                    arr.push_back(tinygltf::Value(static_cast<int>(data[i])));
                }
            }
            tinygltf::Value val(arr);
            
            // Test Is* methods
            bool is_arr = val.IsArray();
            bool is_obj = val.IsObject();
            
            // Try to get array if it's an array
            if (is_arr) {
                const tinygltf::Value::Array& retrieved_arr = val.Get<tinygltf::Value::Array>();
                if (!retrieved_arr.empty()) {
                    // Try to access first element
                    const tinygltf::Value& first_elem = retrieved_arr[0];
                    first_elem.IsObject();
                    first_elem.IsArray();
                }
            }
            break;
        }
        case 3: {
            // Create an object Value
            tinygltf::Value::Object obj;
            if (size > 2) {
                std::string key = "key" + std::to_string(data[1]);
                obj[key] = tinygltf::Value(static_cast<int>(data[2]));
            }
            tinygltf::Value val(obj);
            
            // Test Is* methods
            bool is_obj = val.IsObject();
            bool is_arr = val.IsArray();
            
            // Try to get object if it's an object
            if (is_obj) {
                const tinygltf::Value::Object& retrieved_obj = val.Get<tinygltf::Value::Object>();
                if (!retrieved_obj.empty()) {
                    // Try to access first element
                    auto it = retrieved_obj.begin();
                    const tinygltf::Value& first_val = it->second;
                    first_val.IsObject();
                    first_val.IsArray();
                }
            }
            break;
        }
        case 4: {
            // String value (simulated via integer constructor)
            tinygltf::Value val(static_cast<int>(data[0]));
            
            // Test all Is* methods
            val.IsObject();
            val.IsArray();
            val.IsBool();
            val.IsBinary();
            val.IsString();
            
            // Try to get as different types (some will fail gracefully)
            try {
                val.Get<int>();
            } catch (...) {}
            break;
        }
        case 5: {
            // Binary value (simulated via integer constructor)
            tinygltf::Value val(static_cast<int>(data[0]));
            
            // Test Is* methods
            val.IsObject();
            val.IsArray();
            val.IsBool();
            val.IsBinary();
            val.IsString();
            break;
        }
        case 6: {
            // Real/double value (simulated via integer constructor)
            tinygltf::Value val(static_cast<int>(data[0]));
            
            // Test Is* methods
            val.IsObject();
            val.IsArray();
            val.IsBool();
            val.IsBinary();
            val.IsString();
            
            // Try to get as different types
            try {
                val.Get<double>();
            } catch (...) {
                // Some types might throw or return default
            }
            break;
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test the Value class methods with various inputs
    fuzzValueCreation(Data, Size);
    
    return 0;
}