// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::IsInt at tiny_gltf.h:296:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Type at tiny_gltf.h:292:8 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Size at tiny_gltf.h:377:10 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "tiny_gltf.h"

static void test_value_functions(const tinygltf::Value& value) {
    // Test IsNumber
    bool is_number = value.IsNumber();
    (void)is_number;
    
    // Test ArrayLen
    size_t array_len = value.ArrayLen();
    (void)array_len;
    
    // Test IsInt
    bool is_int = value.IsInt();
    (void)is_int;
    
    // Test Type
    char type = value.Type();
    (void)type;
    
    // Test Has - only valid for object types
    if (value.Type() == tinygltf::OBJECT_TYPE) {
        bool has_key = value.Has("test_key");
        (void)has_key;
    }
    
    // Test Size
    size_t size = value.Size();
    (void)size;
}

static tinygltf::Value create_value_from_data(const uint8_t* data, size_t size) {
    // Create a binary value using the provided constructor
    return tinygltf::Value(data, size);
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* Data, size_t Size) {
    // Limit input size to avoid excessive memory usage
    if (Size > 1024 * 1024) {
        return 0;
    }
    
    // Test 1: Create a Value directly from fuzzer input
    try {
        tinygltf::Value value1 = create_value_from_data(Data, Size);
        test_value_functions(value1);
    } catch (...) {
        // Ignore exceptions during value creation
    }
    
    // Test 2: Try to load a glTF model from file
    std::string filename = "./dummy_file";
    
    // Write fuzzer data to file
    std::ofstream outfile(filename, std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(reinterpret_cast<const char*>(Data), Size);
        outfile.close();
    } else {
        return 0;
    }
    
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    // Try loading as binary glTF
    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    
    if (!ret) {
        // If binary fails, try loading as ASCII glTF
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    }
    
    if (ret) {
        // Test values from the loaded model
        // Access various parts of the model that contain Value objects
        
        // Test values in extensions
        for (const auto& ext_pair : model.extensions) {
            test_value_functions(ext_pair.second);
        }
        
        // Test values in extras
        test_value_functions(model.extras);
        
        // Test values in nodes
        for (const auto& node : model.nodes) {
            test_value_functions(node.extras);
            for (const auto& ext_pair : node.extensions) {
                test_value_functions(ext_pair.second);
            }
        }
        
        // Test values in meshes
        for (const auto& mesh : model.meshes) {
            test_value_functions(mesh.extras);
            for (const auto& ext_pair : mesh.extensions) {
                test_value_functions(ext_pair.second);
            }
            
            // Test values in primitives
            for (const auto& primitive : mesh.primitives) {
                test_value_functions(primitive.extras);
                for (const auto& ext_pair : primitive.extensions) {
                    test_value_functions(ext_pair.second);
                }
            }
        }
        
        // Test values in materials
        for (const auto& material : model.materials) {
            test_value_functions(material.extras);
            for (const auto& ext_pair : material.extensions) {
                test_value_functions(ext_pair.second);
            }
        }
    }
    
    // Clean up the dummy file
    std::remove(filename.c_str());
    
    return 0;
}