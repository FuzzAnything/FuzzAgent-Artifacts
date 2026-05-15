// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
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

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a string from the fuzzing input
    std::string input_string(reinterpret_cast<const char*>(Data), Size);
    
    // Create a Value object using the string constructor
    tinygltf::Value value(input_string);
    
    // Test all the target API functions
    size_t array_len = value.ArrayLen();
    bool is_array = value.IsArray();
    char type = value.Type();
    size_t size = value.Size();
    std::vector<std::string> keys = value.Keys();
    bool is_object = value.IsObject();
    
    // Use the results in some way to avoid dead code elimination
    // We'll create dummy operations that use the results
    static volatile size_t dummy1 = array_len;
    static volatile bool dummy2 = is_array;
    static volatile char dummy3 = type;
    static volatile size_t dummy4 = size;
    static volatile bool dummy5 = is_object;
    
    // Use the keys vector
    if (!keys.empty()) {
        static volatile char dummy6 = keys[0][0];
    }
    
    return 0;
}