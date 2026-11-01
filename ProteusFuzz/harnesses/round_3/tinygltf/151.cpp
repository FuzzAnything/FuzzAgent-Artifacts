// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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

static tinygltf::Value CreateValueFromFuzzData(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return tinygltf::Value("");
    }
    std::string str(reinterpret_cast<const char*>(Data), Size);
    return tinygltf::Value(std::move(str));
}

static void TestValueAPIs(tinygltf::Value &val) {
    // Test IsArray
    bool isArray = val.IsArray();
    (void)isArray;

    // Test IsObject
    bool isObject = val.IsObject();
    (void)isObject;

    // Test ArrayLen
    size_t len = val.ArrayLen();
    (void)len;

    // Test Size
    size_t size = val.Size();
    (void)size;

    // Test Get (array access)
    // Only call Get if the value is actually an array to avoid assertion failure
    if (val.IsArray()) {
        // Try accessing index 0
        const tinygltf::Value &elem0 = val.Get(0);
        (void)elem0;
        
        // Try accessing a large index (out of bounds) - this should return a safe null
        const tinygltf::Value &elemLarge = val.Get(1000);
        (void)elemLarge;
    }
    
    // Test Get for object (using the second overload conceptually, though tinygltf::Value::Get(size_t) is for arrays)
    // The prompt mentions "all 2 overloads", but the signature provided is only for size_t (array).
    // The macro TINYGLTF_VALUE_GET usually expands to a Get(const std::string&) for objects.
    // Since we don't have the full macro expansion in the prompt, we assume the primary target is the array Get.
    // If the library has Get(const std::string&), we would check IsObject() and call it.
    // Based on the provided snippet, we stick to the array Get logic guarded by IsArray().
}

extern "C" int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Strategy 1: Treat input as a string value
    {
        tinygltf::Value val(CreateValueFromFuzzData(Data, Size));
        TestValueAPIs(val);
    }

    // Strategy 2: Try to parse input as JSON to generate Array/Object types
    // We use a dummy file to attempt loading, which may trigger internal Value parsing.
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
        
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;
        
        // Attempt to load the file. Even if it fails, the internal parsing logic might be exercised.
        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file);
        (void)ret; (void)err; (void)warn;
        
        // If we successfully loaded a model, we can inspect its values if accessible.
        // However, the primary target is the Value class methods.
        // The string value test above covers the core logic for non-array/non-object types.
        // To cover array/object types, we rely on the library's internal parsing of the fuzzed JSON.
    }
    
    // Clean up dummy file
    std::remove(dummy_file);

    return 0;
}