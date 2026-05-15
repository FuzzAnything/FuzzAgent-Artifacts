// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::ArrayLen at tiny_gltf.h:352:10 in tiny_gltf.h
// tinygltf::Value::Keys at tiny_gltf.h:365:28 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Has at tiny_gltf.h:358:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
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
#include <vector>
#include <string>
#include <map>

static void test_value_functions(const tinygltf::Value& value) {
    // Test IsObject
    bool is_object = value.IsObject();
    
    // Test ArrayLen
    size_t array_len = value.ArrayLen();
    
    // Test Keys (only meaningful for objects)
    std::vector<std::string> keys = value.Keys();
    
    // Test Has and Get(const std::string&) for objects
    if (is_object) {
        // Test with existing keys from Keys()
        for (const auto& key : keys) {
            bool has_key = value.Has(key);
            const tinygltf::Value& retrieved = value.Get(key);
            (void)has_key; // Avoid unused variable warning
            (void)retrieved; // Avoid unused variable warning
        }
        
        // Test with a dummy key
        bool has_dummy = value.Has("dummy_key");
        const tinygltf::Value& dummy_retrieved = value.Get("dummy_key");
        (void)has_dummy;
        (void)dummy_retrieved;
    }
    
    // Test Get(size_t) for arrays
    if (array_len > 0) {
        // Test valid indices
        for (size_t i = 0; i < array_len && i < 10; ++i) { // Limit to 10 for performance
            const tinygltf::Value& elem = value.Get(i);
            (void)elem;
        }
        
        // Test out-of-bounds index
        if (array_len < SIZE_MAX) {
            const tinygltf::Value& oob = value.Get(array_len);
            (void)oob;
        }
    }
}

static void test_extension_map(const std::map<std::string, tinygltf::Value>& extensions) {
    for (const auto& pair : extensions) {
        test_value_functions(pair.second);
    }
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Create a dummy file with fuzzer data
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Initialize TinyGLTF loader
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Try to load the file (both JSON and binary)
    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
    if (!ret) {
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file");
    }
    
    // Test Value functions on various parts of the model
    if (ret) {
        // Test on model extensions (map of string to Value)
        test_extension_map(model.extensions);
        
        // Test on model extras (Value)
        test_value_functions(model.extras);
        
        // Test on mesh extras and extensions
        for (const auto& mesh : model.meshes) {
            test_extension_map(mesh.extensions);
            test_value_functions(mesh.extras);
        }
        
        // Test on node extras and extensions
        for (const auto& node : model.nodes) {
            test_extension_map(node.extensions);
            test_value_functions(node.extras);
        }
        
        // Test on material extras and extensions
        for (const auto& material : model.materials) {
            test_extension_map(material.extensions);
            test_value_functions(material.extras);
        }
        
        // Test on animation extras and extensions
        for (const auto& animation : model.animations) {
            test_extension_map(animation.extensions);
            test_value_functions(animation.extras);
        }
        
        // Test on camera extras and extensions
        for (const auto& camera : model.cameras) {
            test_extension_map(camera.extensions);
            test_value_functions(camera.extras);
        }
        
        // Test on skin extras and extensions
        for (const auto& skin : model.skins) {
            test_extension_map(skin.extensions);
            test_value_functions(skin.extras);
        }
        
        // Test on texture extras and extensions
        for (const auto& texture : model.textures) {
            test_extension_map(texture.extensions);
            test_value_functions(texture.extras);
        }
        
        // Test on image extras and extensions
        for (const auto& image : model.images) {
            test_extension_map(image.extensions);
            test_value_functions(image.extras);
        }
        
        // Test on buffer extras and extensions
        for (const auto& buffer : model.buffers) {
            test_extension_map(buffer.extensions);
            test_value_functions(buffer.extras);
        }
        
        // Test on bufferView extras and extensions
        for (const auto& bufferView : model.bufferViews) {
            test_extension_map(bufferView.extensions);
            test_value_functions(bufferView.extras);
        }
        
        // Test on accessor extras and extensions
        for (const auto& accessor : model.accessors) {
            test_extension_map(accessor.extensions);
            test_value_functions(accessor.extras);
        }
        
        // Test on sampler extras and extensions
        for (const auto& sampler : model.samplers) {
            test_extension_map(sampler.extensions);
            test_value_functions(sampler.extras);
        }
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}