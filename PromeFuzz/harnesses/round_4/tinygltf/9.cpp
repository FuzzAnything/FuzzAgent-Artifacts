// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
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
#include <map>

static void testValueFunctions(const tinygltf::Value& value, const std::string& key) {
    // Test IsArray
    bool isArray = value.IsArray();
    
    // Test IsObject
    bool isObject = value.IsObject();
    
    // Test IsString
    bool isString = value.IsString();
    
    // Test Has (only meaningful for objects)
    bool hasKey = value.Has(key);
    
    // Test Get with string key (for objects)
    if (isObject) {
        const tinygltf::Value& retrieved = value.Get(key);
        (void)retrieved; // Avoid unused variable warning
    }
    
    // Test template Get functions for various types
    // Note: These will return default values if type doesn't match
    if (value.IsBool()) {
        bool b = value.Get<bool>();
        (void)b;
    }
    
    if (value.IsNumber()) {
        double d = value.Get<double>();
        (void)d;
        
        int i = value.Get<int>();
        (void)i;
    }
    
    if (isString) {
        std::string s = value.Get<std::string>();
        (void)s;
    }
    
    if (value.IsBinary()) {
        std::vector<unsigned char> bin = value.Get<std::vector<unsigned char>>();
        (void)bin;
    }
    
    if (isArray) {
        tinygltf::Value::Array arr = value.Get<tinygltf::Value::Array>();
        (void)arr;
    }
    
    if (isObject) {
        tinygltf::Value::Object obj = value.Get<tinygltf::Value::Object>();
        (void)obj;
    }
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a key from the first part of the data
    std::string key;
    size_t key_len = Size > 100 ? 100 : Size;
    for (size_t i = 0; i < key_len && i < Size; i++) {
        char c = static_cast<char>(Data[i] % 128);
        if (c >= 32 && c <= 126) { // Printable ASCII
            key.push_back(c);
        } else {
            key.push_back('a' + (c % 26));
        }
    }
    if (key.empty()) {
        key = "default_key";
    }
    
    // Create different types of Value objects to test
    
    // 1. Integer value
    int int_val = static_cast<int>(Data[0]) * 256 + (Size > 1 ? Data[1] : 0);
    tinygltf::Value intValue(int_val);
    testValueFunctions(intValue, key);
    
    // 2. Create an array value
    tinygltf::Value::Array array;
    for (size_t i = 0; i < 3 && i * 2 + 1 < Size; i++) {
        int elem = static_cast<int>(Data[i * 2]) * 256 + 
                   (i * 2 + 1 < Size ? Data[i * 2 + 1] : 0);
        array.push_back(tinygltf::Value(elem));
    }
    tinygltf::Value arrayValue(array);
    testValueFunctions(arrayValue, key);
    
    // 3. Create an object value
    tinygltf::Value::Object object;
    for (size_t i = 0; i < 2 && i * 4 + 3 < Size; i++) {
        std::string obj_key = "key" + std::to_string(i);
        int obj_val = static_cast<int>(Data[i * 4]) * 16777216 +
                      (i * 4 + 1 < Size ? Data[i * 4 + 1] * 65536 : 0) +
                      (i * 4 + 2 < Size ? Data[i * 4 + 2] * 256 : 0) +
                      (i * 4 + 3 < Size ? Data[i * 4 + 3] : 0);
        object[obj_key] = tinygltf::Value(obj_val);
    }
    tinygltf::Value objectValue(object);
    testValueFunctions(objectValue, key);
    
    // 4. Create a string value
    std::string str_val;
    size_t str_len = Size > 50 ? 50 : Size;
    for (size_t i = 0; i < str_len && i < Size; i++) {
        char c = static_cast<char>(Data[i] % 128);
        if (c >= 32 && c <= 126) {
            str_val.push_back(c);
        } else {
            str_val.push_back('A' + (c % 26));
        }
    }
    if (str_val.empty()) {
        str_val = "default_string";
    }
    
    // Note: We don't have direct string constructor, so we'll skip this
    // since we need to parse JSON or use other methods to create string Value
    
    // 5. Test with null value (default constructor)
    tinygltf::Value nullValue;
    testValueFunctions(nullValue, key);
    
    return 0;
}