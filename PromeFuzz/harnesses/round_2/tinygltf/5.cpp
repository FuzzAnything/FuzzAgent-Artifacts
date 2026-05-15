// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsInt at tiny_gltf.h:321:7 in tiny_gltf.h
// tinygltf::Value::IsBool at tiny_gltf.h:294:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:406:1 in tiny_gltf.h
// tinygltf::Value::IsReal at tiny_gltf.h:300:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:407:1 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:411:1 in tiny_gltf.h
// tinygltf::Value::IsBinary at tiny_gltf.h:304:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:410:1 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "tiny_gltf.h"

static void test_value_functions(const tinygltf::Value& value) {
    // Test type checking functions
    bool is_real = value.IsReal();
    bool is_array = value.IsArray();
    bool is_bool = value.IsBool();
    bool is_binary = value.IsBinary();
    
    // Test GetNumberAsInt
    int number_as_int = value.GetNumberAsInt();
    
    // Test Get functions based on type
    if (value.IsBool()) {
        bool bool_val = value.Get<bool>();
    }
    if (value.IsReal()) {
        double real_val = value.Get<double>();
    }
    if (value.IsArray()) {
        const tinygltf::Value::Array& arr = value.Get<tinygltf::Value::Array>();
    }
    if (value.IsBinary()) {
        const std::vector<unsigned char>& bin = value.Get<std::vector<unsigned char>>();
    }
}

static void test_extensions_map(const tinygltf::ExtensionMap& extensions) {
    for (const auto& ext : extensions) {
        test_value_functions(ext.second);
    }
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Create various types of Value objects to test
    std::vector<tinygltf::Value> test_values;
    
    // Create boolean values
    test_values.emplace_back(true);
    test_values.emplace_back(false);
    
    // Try to create values from the fuzzer input
    if (Size > 0) {
        // Use first byte to decide value type
        uint8_t type_selector = Data[0];
        
        switch (type_selector % 4) {
            case 0: {
                // Boolean from fuzzer data
                bool b = (Size > 1 && (Data[1] & 0x1) != 0);
                test_values.emplace_back(b);
                break;
            }
            case 1: {
                // Create an array value
                tinygltf::Value::Array arr;
                if (Size > 2) {
                    size_t arr_size = Data[1] % 10; // Small array size
                    for (size_t i = 0; i < arr_size && i + 2 < Size; i++) {
                        bool elem = (Data[2 + i] & 0x1) != 0;
                        arr.emplace_back(elem);
                    }
                }
                break;
            }
            default:
                // Default boolean
                test_values.emplace_back((type_selector & 0x1) != 0);
                break;
        }
    }
    
    // Test all created values
    for (const auto& val : test_values) {
        test_value_functions(val);
    }
    
    // Additional test: Try to load a glTF file with fuzzer data
    // Write fuzzer data to a dummy file
    std::ofstream outfile("./dummy_file", std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(reinterpret_cast<const char*>(Data), Size);
        outfile.close();
        
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;
        
        // Try loading as both ASCII and binary
        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
        if (!ret) {
            ret = loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file");
        }
        
        // If loaded successfully, test values from the model
        if (ret) {
            // Test model extras and extensions
            test_value_functions(model.extras);
            test_extensions_map(model.extensions);
            
            // Test values from materials
            for (const auto& material : model.materials) {
                test_value_functions(material.extras);
                test_extensions_map(material.extensions);
            }
            
            // Test values from nodes
            for (const auto& node : model.nodes) {
                test_value_functions(node.extras);
                test_extensions_map(node.extensions);
            }
            
            // Test values from scenes
            for (const auto& scene : model.scenes) {
                test_value_functions(scene.extras);
                test_extensions_map(scene.extensions);
            }
            
            // Test values from animations
            for (const auto& animation : model.animations) {
                test_value_functions(animation.extras);
                test_extensions_map(animation.extensions);
            }
            
            // Test values from cameras
            for (const auto& camera : model.cameras) {
                test_value_functions(camera.extras);
                test_extensions_map(camera.extensions);
            }
            
            // Test values from meshes
            for (const auto& mesh : model.meshes) {
                test_value_functions(mesh.extras);
                test_extensions_map(mesh.extensions);
            }
            
            // Test values from textures
            for (const auto& texture : model.textures) {
                test_value_functions(texture.extras);
                test_extensions_map(texture.extensions);
            }
            
            // Test values from images
            for (const auto& image : model.images) {
                test_value_functions(image.extras);
                test_extensions_map(image.extensions);
            }
            
            // Test values from samplers
            for (const auto& sampler : model.samplers) {
                test_value_functions(sampler.extras);
                test_extensions_map(sampler.extensions);
            }
            
            // Test values from buffers
            for (const auto& buffer : model.buffers) {
                test_value_functions(buffer.extras);
                test_extensions_map(buffer.extensions);
            }
            
            // Test values from bufferViews
            for (const auto& bufferView : model.bufferViews) {
                test_value_functions(bufferView.extras);
                test_extensions_map(bufferView.extensions);
            }
            
            // Test values from accessors
            for (const auto& accessor : model.accessors) {
                test_value_functions(accessor.extras);
                test_extensions_map(accessor.extensions);
            }
        }
        
        // Clean up dummy file
        std::remove("./dummy_file");
    }
    
    return 0;
}