// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
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
#include <map>

static void test_value_operations(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create various Value objects to test
    tinygltf::Value null_value;
    
    // Boolean value
    tinygltf::Value bool_value(static_cast<bool>(data[0] & 1));
    
    // Integer value
    int int_val = 0;
    if (size > 1) int_val = static_cast<int>(data[1]);
    tinygltf::Value int_value(int_val);
    
    // Real value
    double real_val = 0.0;
    if (size > 2) real_val = static_cast<double>(data[2]);
    tinygltf::Value real_value(real_val);
    
    // String value
    std::string str_val;
    if (size > 3) {
        str_val = std::string(reinterpret_cast<const char*>(data), 
                             std::min(size, static_cast<size_t>(100)));
    }
    tinygltf::Value string_value(str_val);
    
    // Binary value - use the constructor that takes pointer and size
    std::vector<unsigned char> binary_val;
    if (size > 4) {
        binary_val.assign(data, data + std::min(size, static_cast<size_t>(50)));
    }
    tinygltf::Value binary_value(binary_val.data(), binary_val.size());
    
    // Array value
    tinygltf::Value::Array array_val;
    array_val.push_back(int_value);
    array_val.push_back(real_value);
    array_val.push_back(string_value);
    tinygltf::Value array_value(std::move(array_val));
    
    // Object value
    tinygltf::Value::Object obj_val;
    obj_val["key1"] = int_value;
    obj_val["key2"] = string_value;
    obj_val["key3"] = array_value;
    tinygltf::Value object_value(std::move(obj_val));
    
    // Test IsArray on different value types
    bool is_array1 = null_value.IsArray();
    bool is_array2 = bool_value.IsArray();
    bool is_array3 = int_value.IsArray();
    bool is_array4 = real_value.IsArray();
    bool is_array5 = string_value.IsArray();
    bool is_array6 = binary_value.IsArray();
    bool is_array7 = array_value.IsArray();
    bool is_array8 = object_value.IsArray();
    
    // Test IsString on different value types
    bool is_string1 = null_value.IsString();
    bool is_string2 = bool_value.IsString();
    bool is_string3 = int_value.IsString();
    bool is_string4 = real_value.IsString();
    bool is_string5 = string_value.IsString();
    bool is_string6 = binary_value.IsString();
    bool is_string7 = array_value.IsString();
    bool is_string8 = object_value.IsString();
    
    // Test IsObject on different value types
    bool is_object1 = null_value.IsObject();
    bool is_object2 = bool_value.IsObject();
    bool is_object3 = int_value.IsObject();
    bool is_object4 = real_value.IsObject();
    bool is_object5 = string_value.IsObject();
    bool is_object6 = binary_value.IsObject();
    bool is_object7 = array_value.IsObject();
    bool is_object8 = object_value.IsObject();
    
    // Test Get on array_value (if it's an array)
    if (array_value.IsArray()) {
        // Test Get with valid and invalid indices
        array_value.Get(0);
        array_value.Get(1);
        array_value.Get(2);
        array_value.Get(100); // Out of bounds
        array_value.Get(-1); // Invalid index
    }
    
    // Test Get on object_value (if it's an object)
    if (object_value.IsObject()) {
        // Test Get with existing and non-existing keys
        object_value.Get("key1");
        object_value.Get("key2");
        object_value.Get("key3");
        object_value.Get("nonexistent");
    }
    
    // Test Has on object_value (if it's an object)
    bool has_key1 = false, has_key2 = false, has_key3 = false, has_invalid = false, has_empty = false;
    if (object_value.IsObject()) {
        has_key1 = object_value.Has("key1");
        has_key2 = object_value.Has("key2");
        has_key3 = object_value.Has("key3");
        has_invalid = object_value.Has("nonexistent");
        has_empty = object_value.Has("");
    }
    
    // Test Get on other value types (should return null references)
    bool_value.Get(0); // Not an array/object
    int_value.Get("key"); // Not an object
    string_value.Get(0); // Not an array
    real_value.Get("test"); // Not an object
    
    // Test Has on non-object values (should return false)
    bool has_on_non_object1 = bool_value.Has("key");
    bool has_on_non_object2 = int_value.Has("key");
    bool has_on_non_object3 = string_value.Has("key");
    bool has_on_non_object4 = array_value.Has("key");
    
    // Prevent unused variable warnings
    (void)is_array1; (void)is_array2; (void)is_array3; (void)is_array4;
    (void)is_array5; (void)is_array6; (void)is_array7; (void)is_array8;
    (void)is_string1; (void)is_string2; (void)is_string3; (void)is_string4;
    (void)is_string5; (void)is_string6; (void)is_string7; (void)is_string8;
    (void)is_object1; (void)is_object2; (void)is_object3; (void)is_object4;
    (void)is_object5; (void)is_object6; (void)is_object7; (void)is_object8;
    (void)has_key1; (void)has_key2; (void)has_key3; (void)has_invalid; (void)has_empty;
    (void)has_on_non_object1; (void)has_on_non_object2; 
    (void)has_on_non_object3; (void)has_on_non_object4;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    test_value_operations(Data, Size);
    return 0;
}