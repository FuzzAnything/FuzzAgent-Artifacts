// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <array>
#include <tiny_gltf.h>

static void FuzzValueMethods(const uint8_t *Data, size_t Size) {
    // Create a tinygltf::Value instance.
    // We will construct it as a string initially to test IsString.
    // The fuzzer data will be used to construct the string content.
    std::string str(reinterpret_cast<const char*>(Data), Size);
    tinygltf::Value val(std::move(str));

    // Test 1: IsString
    // Should return true since we constructed it as a string.
    bool isString = val.IsString();
    (void)isString; // Suppress unused variable warning

    // Test 2: IsReal
    // Should return false for a string.
    bool isReal = val.IsReal();
    (void)isReal;

    // Test 3: IsInt
    // Should return false for a string.
    bool isInt = val.IsInt();
    (void)isInt;

    // Test 4: ArrayLen
    // Should return 0 for a string.
    size_t arrLen = val.ArrayLen();
    (void)arrLen;

    // Test 5: Size
    // Behavior for non-container types is undefined per description, 
    // but we call it to exercise the code path.
    size_t sz = val.Size();
    (void)sz;

    // Test 6: Get (Array access)
    // The description says: "Always check if the Value is an array by calling val.IsArray() before invoking val.Get(index)."
    // Since our value is a string, IsArray() should be false.
    // We will not call Get on this specific instance to avoid potential asserts or undefined behavior 
    // if the implementation differs slightly, but we can test the logic flow.
    
    // To test Get properly, we need to construct an array.
    // Let's create a new value that is an array using the fuzzer data.
    tinygltf::Value::Array arr;
    // Fill the array with some dummy values based on input size to vary state
    for (size_t i = 0; i < Size && i < 100; ++i) {
        tinygltf::Value v(static_cast<int>(Data[i]));
        arr.push_back(std::move(v));
    }
    
    tinygltf::Value arrVal(arr);

    // Now test methods on the array value
    bool arrIsString = arrVal.IsString();
    (void)arrIsString;

    bool arrIsReal = arrVal.IsReal();
    (void)arrIsReal;

    bool arrIsInt = arrVal.IsInt();
    (void)arrIsInt;

    size_t arrLenVal = arrVal.ArrayLen();
    (void)arrLenVal;

    size_t arrSize = arrVal.Size();
    (void)arrSize;

    // Test Get on the array
    if (arrVal.IsArray()) {
        // Try to get an element at index 0
        if (arrLenVal > 0) {
            const tinygltf::Value &elem = arrVal.Get(0);
            (void)elem;
        }
        // Try to get an out-of-bounds element (should return null/safe value)
        const tinygltf::Value &outOfBounds = arrVal.Get(arrLenVal + 1);
        (void)outOfBounds;
    }
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Basic sanity check to avoid empty strings causing issues if any
    FuzzValueMethods(Data, Size);

    return 0;
}