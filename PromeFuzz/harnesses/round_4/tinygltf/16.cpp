// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
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
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

static void testValueFunctions(const tinygltf::Value& value, const std::string& testKey) {
    // Test IsArray
    bool isArray = value.IsArray();
    (void)isArray;
    
    // Test Has with a key derived from fuzzer input
    bool hasKey = value.Has(testKey);
    (void)hasKey;
    
    // Test ArrayLen
    size_t arrayLen = value.ArrayLen();
    (void)arrayLen;
    
    // Test Keys
    std::vector<std::string> keys = value.Keys();
    (void)keys;
    
    // Test Size
    size_t size = value.Size();
    (void)size;
    
    // Test IsObject
    bool isObject = value.IsObject();
    (void)isObject;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Create test key from fuzzer input (limit to reasonable length)
    std::string testKey;
    if (Size > 0) {
        size_t keyLen = Size > 100 ? 100 : Size;
        testKey = std::string(reinterpret_cast<const char*>(Data), keyLen);
    }
    
    // Test with integer value
    if (Size > 0) {
        int intValue = static_cast<int>(Data[0]);
        tinygltf::Value intVal(intValue);
        testValueFunctions(intVal, testKey);
    }
    
    // Test with object created from glTF file
    if (Size > 0) {
        // Write fuzzer data to dummy file
        std::ofstream file("./dummy_file", std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(Data), Size);
            file.close();
            
            // Try to load glTF file
            tinygltf::TinyGLTF loader;
            tinygltf::Model model;
            std::string err;
            std::string warn;
            
            bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
            if (!ret) {
                ret = loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file");
            }
            
            // If model loaded successfully, test Value functions on model extras
            if (ret) {
                testValueFunctions(model.extras, testKey);
                
                // Also test on node extras if available
                if (!model.nodes.empty()) {
                    testValueFunctions(model.nodes[0].extras, testKey);
                }
                
                // Test on material extras if available
                if (!model.materials.empty()) {
                    testValueFunctions(model.materials[0].extras, testKey);
                }
            }
            
            // Clean up dummy file
            std::remove("./dummy_file");
        }
    }
    
    return 0;
}