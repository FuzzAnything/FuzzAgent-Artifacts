// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:409:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:408:1 in tiny_gltf.h
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

static void testValueFunctions(tinygltf::Value& value) {
    // Test type checking functions
    bool isBool = value.IsBool();
    bool isArray = value.IsArray();
    bool isString = value.IsString();
    bool isBinary = value.IsBinary();
    char type = value.Type();
    
    // Use volatile to prevent optimization
    volatile bool dummy1 = isBool;
    volatile bool dummy2 = isArray;
    volatile bool dummy3 = isString;
    volatile bool dummy4 = isBinary;
    volatile char dummy5 = type;
    (void)dummy1; (void)dummy2; (void)dummy3; (void)dummy4; (void)dummy5;
    
    // Test Get functions based on type
    if (value.IsBool()) {
        const bool& boolVal = value.Get<bool>();
        volatile bool dummy = boolVal;
        (void)dummy;
    }
    
    if (value.IsArray()) {
        const tinygltf::Value::Array& arr = value.Get<tinygltf::Value::Array>();
        volatile size_t dummy = arr.size();
        (void)dummy;
    }
    
    if (value.IsString()) {
        const std::string& str = value.Get<std::string>();
        volatile size_t dummy = str.size();
        (void)dummy;
    }
    
    if (value.IsBinary()) {
        const std::vector<unsigned char>& bin = value.Get<std::vector<unsigned char>>();
        volatile size_t dummy = bin.size();
        (void)dummy;
    }
    
    // Try to get other types regardless of actual type
    // These will return null references if type doesn't match
    const double& dbl = value.Get<double>();
    const int& intVal = value.Get<int>();
    const tinygltf::Value::Object& obj = value.Get<tinygltf::Value::Object>();
    
    volatile double dummy6 = dbl;
    volatile int dummy7 = intVal;
    volatile size_t dummy8 = obj.size();
    (void)dummy6; (void)dummy7; (void)dummy8;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Create a binary value from fuzzer input
    std::vector<unsigned char> binaryData(Data, Data + Size);
    tinygltf::Value binaryValue(std::move(binaryData));
    
    // Test functions on binary value
    testValueFunctions(binaryValue);
    
    // Create other types of values to test more states
    // Bool value
    tinygltf::Value boolValue(true);
    testValueFunctions(boolValue);
    
    // Int value
    tinygltf::Value intValue(42);
    testValueFunctions(intValue);
    
    // Double value
    tinygltf::Value doubleValue(3.14);
    testValueFunctions(doubleValue);
    
    // String value
    tinygltf::Value stringValue("test");
    testValueFunctions(stringValue);
    
    // Array value
    tinygltf::Value::Array array;
    array.push_back(tinygltf::Value(1));
    array.push_back(tinygltf::Value(2));
    tinygltf::Value arrayValue(std::move(array));
    testValueFunctions(arrayValue);
    
    // Object value
    tinygltf::Value::Object obj;
    obj["key1"] = tinygltf::Value("value1");
    obj["key2"] = tinygltf::Value(123);
    tinygltf::Value objectValue(std::move(obj));
    testValueFunctions(objectValue);
    
    // Test with empty values
    tinygltf::Value emptyArrayValue(tinygltf::Value::Array{});
    testValueFunctions(emptyArrayValue);
    
    tinygltf::Value emptyObjectValue(tinygltf::Value::Object{});
    testValueFunctions(emptyObjectValue);
    
    // Test with empty binary
    tinygltf::Value emptyBinaryValue(std::vector<unsigned char>{});
    testValueFunctions(emptyBinaryValue);
    
    return 0;
}