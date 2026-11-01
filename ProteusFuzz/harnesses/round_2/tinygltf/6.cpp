// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
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
#include <map>
#include "tiny_gltf.h"

static void TestValueAPIs(const uint8_t *Data, size_t Size) {
    // 1. Test with default constructed value (Null)
    tinygltf::Value val;
    val.IsObject();
    val.IsArray();
    val.ArrayLen();
    // Size() on non-container is undefined but we call it to test behavior
    val.Size();
    // Get on non-array triggers an assert in tinygltf if not checked.
    // We must check IsArray() before calling Get() to avoid the assert failure.
    if (val.IsArray()) {
        val.Get(0);
        val.Get(Size);
    }

    // 2. Test with String type
    if (Size > 0) {
        std::string s(reinterpret_cast<const char*>(Data), Size);
        tinygltf::Value strVal(std::move(s));
        strVal.IsObject();
        strVal.IsArray();
        strVal.ArrayLen();
        strVal.Size();
        if (strVal.IsArray()) {
            strVal.Get(0);
            strVal.Get(Size);
        }
    }

    // 3. Test with Array type
    tinygltf::Value arrVal;
    // Construct an array directly using the vector constructor
    std::vector<tinygltf::Value> arr_elements;
    arrVal = tinygltf::Value(arr_elements);
    
    // Populate array with fuzzed data
    std::vector<tinygltf::Value> new_arr;
    for (size_t i = 0; i < Size; ++i) {
        tinygltf::Value elem;
        // Randomly choose type based on byte
        switch (Data[i] % 4) {
            case 0: 
                // Null
                elem = tinygltf::Value(); 
                break;
            case 1: 
                // Int
                elem = tinygltf::Value(static_cast<int>(i)); 
                break;
            case 2: 
                // Number
                elem = tinygltf::Value(static_cast<double>(i)); 
                break;
            case 3: 
                // Bool
                elem = tinygltf::Value(i % 2 != 0); 
                break;
        }
        new_arr.push_back(elem);
    }
    arrVal = tinygltf::Value(new_arr);

    arrVal.IsObject();
    arrVal.IsArray();
    arrVal.ArrayLen();
    arrVal.Size();
    
    // Test Get with valid and invalid indices only if it is an array
    if (arrVal.IsArray()) {
        if (arrVal.ArrayLen() > 0) {
            arrVal.Get(0);
            arrVal.Get(arrVal.ArrayLen() - 1);
        }
        arrVal.Get(arrVal.ArrayLen()); // Out of bounds (safe in tinygltf)
        arrVal.Get(Size + 100);        // Large out of bounds (safe in tinygltf)
    }

    // 4. Test with Object type
    tinygltf::Value objVal;
    // Construct an object directly
    std::map<std::string, tinygltf::Value> obj_elements;
    objVal = tinygltf::Value(obj_elements);
    
    // Populate object with fuzzed keys/values
    std::map<std::string, tinygltf::Value> new_obj;
    for (size_t i = 0; i < Size / 2; ++i) {
        std::string key = "key_" + std::to_string(i);
        tinygltf::Value valElem;
        valElem = tinygltf::Value(static_cast<double>(i));
        new_obj.insert(std::make_pair(key, valElem));
    }
    objVal = tinygltf::Value(new_obj);

    objVal.IsObject();
    objVal.IsArray();
    objVal.ArrayLen();
    objVal.Size();
    // Get on object is invalid for array index, but we call it to test safety
    // Must check IsArray first to prevent assert
    if (objVal.IsArray()) {
        objVal.Get(0);
        objVal.Get(Size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    TestValueAPIs(Data, Size);

    return 0;
}