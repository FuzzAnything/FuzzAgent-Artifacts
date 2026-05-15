// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
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
#include <stdint.h>
#include <vector>
#include <string>
#include <map>

static void testValueFunctions(const tinygltf::Value& value) {
    // Test ArrayLen
    size_t array_len = value.ArrayLen();
    
    // Test IsInt
    bool is_int = value.IsInt();
    
    // Test IsNumber
    bool is_number = value.IsNumber();
    
    // Test Type
    char type = value.Type();
    
    // Test Size
    size_t size = value.Size();
    
    // Test Has with a dummy key
    bool has_key = value.Has("test_key");
    
    // Use the results to avoid optimization
    (void)array_len;
    (void)is_int;
    (void)is_number;
    (void)type;
    (void)size;
    (void)has_key;
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Create different types of Value objects to test the functions
    
    // 1. Integer value
    tinygltf::Value int_value(static_cast<int>(Data[0] % 256));
    
    // 2. Double value (if we have at least 8 bytes)
    double double_val = 0.0;
    if (Size >= 8) {
        // Use first 8 bytes to create a double (simplified)
        uint64_t bits = 0;
        for (size_t i = 0; i < 8 && i < Size; i++) {
            bits = (bits << 8) | Data[i];
        }
        memcpy(&double_val, &bits, sizeof(double));
    }
    tinygltf::Value double_value(double_val);
    
    // 3. String value
    std::string str_val;
    if (Size > 0) {
        str_val = std::string(reinterpret_cast<const char*>(Data), 
                             std::min(Size, static_cast<size_t>(100)));
    }
    tinygltf::Value string_value(str_val);
    
    // 4. Boolean value
    tinygltf::Value bool_value(Size % 2 == 0);
    
    // 5. Array value
    std::vector<tinygltf::Value> array_data;
    // Add a few elements to the array
    for (int i = 0; i < 3 && i < static_cast<int>(Size); i++) {
        array_data.push_back(tinygltf::Value(static_cast<int>(Data[i])));
    }
    tinygltf::Value array_value(array_data);
    
    // 6. Object value
    std::map<std::string, tinygltf::Value> object_data;
    if (Size > 0) {
        object_data["key1"] = tinygltf::Value(static_cast<int>(Data[0]));
        if (Size > 1) {
            object_data["key2"] = tinygltf::Value(static_cast<double>(Data[1]));
        }
    }
    tinygltf::Value object_value(object_data);
    
    // Test all value types with the target functions
    testValueFunctions(int_value);
    testValueFunctions(double_value);
    testValueFunctions(string_value);
    testValueFunctions(bool_value);
    testValueFunctions(array_value);
    testValueFunctions(object_value);
    
    return 0;
}