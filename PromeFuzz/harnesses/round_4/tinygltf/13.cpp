// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
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
#include <map>

static void testValueFunctions(const tinygltf::Value& value) {
    // Test IsArray
    bool isArray = value.IsArray();
    
    // Test IsObject
    bool isObject = value.IsObject();
    
    // Test ArrayLen
    size_t arrayLen = value.ArrayLen();
    
    // Test Size
    size_t size = value.Size();
    
    // Test Keys
    std::vector<std::string> keys = value.Keys();
    
    // Test Has with some keys
    if (isObject && !keys.empty()) {
        for (const auto& key : keys) {
            bool hasKey = value.Has(key);
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a string from the fuzzer input
    std::string inputString(reinterpret_cast<const char*>(Data), Size);
    
    // Test with string value using the provided constructor
    tinygltf::Value stringValue(std::move(inputString));
    testValueFunctions(stringValue);
    
    // Create additional test values with different types
    // Create empty string value
    tinygltf::Value emptyStringValue(std::string(""));
    testValueFunctions(emptyStringValue);
    
    // Create a string that looks like JSON
    tinygltf::Value jsonLikeString(std::string("{\"key\": \"value\"}"));
    testValueFunctions(jsonLikeString);
    
    // Create a string that looks like an array
    tinygltf::Value arrayLikeString(std::string("[1, 2, 3]"));
    testValueFunctions(arrayLikeString);
    
    // Create a string with special characters
    tinygltf::Value specialString(std::string("test\n\t\r\b\f\"\\"));
    testValueFunctions(specialString);
    
    // Try to create a TinyGLTF model and parse JSON to get more Value types
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Write data to a dummy file
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
        
        // Try to load the file
        loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", 0);
        
        // Test values from the model if loaded successfully
        // Test extensions (which is a map<string, Value>)
        for (const auto& ext_pair : model.extensions) {
            testValueFunctions(ext_pair.second);
        }
        
        // Test extras (which is a Value)
        if (model.extras.Size() > 0) {
            testValueFunctions(model.extras);
        }
        
        // Test values from nodes
        for (const auto& node : model.nodes) {
            for (const auto& ext_pair : node.extensions) {
                testValueFunctions(ext_pair.second);
            }
            if (node.extras.Size() > 0) {
                testValueFunctions(node.extras);
            }
        }
        
        // Test values from meshes
        for (const auto& mesh : model.meshes) {
            for (const auto& ext_pair : mesh.extensions) {
                testValueFunctions(ext_pair.second);
            }
            if (mesh.extras.Size() > 0) {
                testValueFunctions(mesh.extras);
            }
        }
        
        // Test values from materials
        for (const auto& material : model.materials) {
            for (const auto& ext_pair : material.extensions) {
                testValueFunctions(ext_pair.second);
            }
            if (material.extras.Size() > 0) {
                testValueFunctions(material.extras);
            }
        }
        
        // Remove the dummy file
        remove("./dummy_file");
    }
    
    return 0;
}