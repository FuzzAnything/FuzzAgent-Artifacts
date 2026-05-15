// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
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
#include <cstddef>
#include <string>
#include <vector>
#include <algorithm>

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Create a tinygltf::Value object using the string constructor
    std::string input_string(reinterpret_cast<const char*>(Data), Size);
    tinygltf::Value value(std::move(input_string));
    
    // Test Size() method
    static_cast<void>(value.Size());
    
    // Test Has() method with various keys
    // Use different key strategies to explore more states
    if (Size > 0) {
        // Try first byte as single character key
        std::string key1(1, static_cast<char>(Data[0]));
        static_cast<void>(value.Has(key1));
        
        // Try a fixed key
        static_cast<void>(value.Has("test_key"));
        
        // Try empty key
        static_cast<void>(value.Has(""));
        
        // Try key from input data if enough bytes available
        if (Size > 10) {
            std::string key2(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(10)));
            static_cast<void>(value.Has(key2));
        }
    }
    
    // Test ArrayLen() method
    static_cast<void>(value.ArrayLen());
    
    // Test IsInt() method
    static_cast<void>(value.IsInt());
    
    // Test IsNumber() method
    static_cast<void>(value.IsNumber());
    
    // Test Type() method
    static_cast<void>(value.Type());
    
    // Create additional values with different content to explore more states
    if (Size > 1) {
        // Create value with substring
        std::string sub_string(reinterpret_cast<const char*>(Data), Size/2);
        tinygltf::Value value2(std::move(sub_string));
        
        // Test all methods on the second value
        static_cast<void>(value2.Size());
        static_cast<void>(value2.Has("key"));
        static_cast<void>(value2.ArrayLen());
        static_cast<void>(value2.IsInt());
        static_cast<void>(value2.IsNumber());
        static_cast<void>(value2.Type());
    }
    
    // Test with empty string value
    tinygltf::Value empty_value(std::string(""));
    static_cast<void>(empty_value.Size());
    static_cast<void>(empty_value.Has(""));
    static_cast<void>(empty_value.ArrayLen());
    static_cast<void>(empty_value.IsInt());
    static_cast<void>(empty_value.IsNumber());
    static_cast<void>(empty_value.Type());
    
    // Test with special characters in string
    std::string special_chars = "{\"key\": \"value\"}";
    tinygltf::Value json_like_value(std::move(special_chars));
    static_cast<void>(json_like_value.Size());
    static_cast<void>(json_like_value.Has("key"));
    static_cast<void>(json_like_value.ArrayLen());
    static_cast<void>(json_like_value.IsInt());
    static_cast<void>(json_like_value.IsNumber());
    static_cast<void>(json_like_value.Type());
    
    return 0;
}