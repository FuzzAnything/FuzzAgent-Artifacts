// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsString at tiny_gltf.h:302:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
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
#include <cstdlib>
#include <vector>
#include <string>

static void testValueFunctions(const tinygltf::Value& value, const std::string& key) {
    // Test IsBool
    (void)value.IsBool();
    
    // Test Get with provided key
    const tinygltf::Value& retrieved = value.Get(key);
    // Test functions on retrieved value as well
    (void)retrieved.IsBool();
    (void)retrieved.IsReal();
    (void)retrieved.IsString();
    (void)retrieved.IsBinary();
    (void)retrieved.Type();
    
    // Test IsReal
    (void)value.IsReal();
    
    // Test IsString
    (void)value.IsString();
    
    // Test IsBinary
    (void)value.IsBinary();
    
    // Test Type
    (void)value.Type();
}

static void createAndTestBinaryValue(const uint8_t* data, size_t size, const std::string& key) {
    // Create binary vector from fuzzer input
    std::vector<unsigned char> binary_data(data, data + size);
    
    // Create Value object using binary constructor
    tinygltf::Value binary_value(std::move(binary_data));
    
    // Test all functions on the binary value
    testValueFunctions(binary_value, key);
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Extract a key from the input data (first null-terminated string or first few bytes)
    std::string key;
    size_t key_len = 0;
    while (key_len < Size && Data[key_len] != 0 && key_len < 100) {
        key_len++;
    }
    if (key_len > 0) {
        key = std::string(reinterpret_cast<const char*>(Data), key_len);
    } else {
        key = "test_key";
    }
    
    // Test 1: Create binary value and test functions
    createAndTestBinaryValue(Data, Size, key);
    
    // Test 2: Try to load a glTF file from the fuzzer input
    // Write data to dummy file
    FILE* file = fopen("./dummy_file", "wb");
    if (file) {
        fwrite(Data, 1, Size, file);
        fclose(file);
    }
    
    // Attempt to load the file using tinygltf
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    // Try loading as both JSON and binary formats
    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
    if (!ret) {
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file");
    }
    
    // If model loaded successfully, traverse and test Value objects
    if (ret) {
        // Test values from model extensions if present
        if (model.extensions.size() > 0) {
            for (const auto& ext_pair : model.extensions) {
                testValueFunctions(ext_pair.second, key);
            }
        }
        
        // Test values from model extras (check type first to avoid null values)
        if (model.extras.Type() != 0) {  // Assuming 0 is null type
            testValueFunctions(model.extras, key);
        }
        
        // Test values from scenes
        for (const auto& scene : model.scenes) {
            if (scene.extras.Type() != 0) {
                testValueFunctions(scene.extras, key);
            }
            if (scene.extensions.size() > 0) {
                for (const auto& ext_pair : scene.extensions) {
                    testValueFunctions(ext_pair.second, key);
                }
            }
        }
        
        // Test values from nodes
        for (const auto& node : model.nodes) {
            if (node.extras.Type() != 0) {
                testValueFunctions(node.extras, key);
            }
            if (node.extensions.size() > 0) {
                for (const auto& ext_pair : node.extensions) {
                    testValueFunctions(ext_pair.second, key);
                }
            }
        }
        
        // Test values from meshes
        for (const auto& mesh : model.meshes) {
            if (mesh.extras.Type() != 0) {
                testValueFunctions(mesh.extras, key);
            }
            if (mesh.extensions.size() > 0) {
                for (const auto& ext_pair : mesh.extensions) {
                    testValueFunctions(ext_pair.second, key);
                }
            }
        }
        
        // Test values from materials
        for (const auto& material : model.materials) {
            if (material.extras.Type() != 0) {
                testValueFunctions(material.extras, key);
            }
            if (material.extensions.size() > 0) {
                for (const auto& ext_pair : material.extensions) {
                    testValueFunctions(ext_pair.second, key);
                }
            }
        }
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}