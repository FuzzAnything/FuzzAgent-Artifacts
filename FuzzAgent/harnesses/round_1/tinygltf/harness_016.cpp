#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_016 {
/*
 * Fuzzing harness for tinygltf JSON Value manipulation functions
 * 
 * This harness targets Value object access functions with 26+ undiscovered branches:
 * 1) Value::Size() - 8 branches
 * 2) Value::Keys() - 4 branches  
 * 3) Value::Has("key") - 4 branches
 * 4) Value::Get("key") - 4 branches
 * 5) Value::Get(index) - 4 branches
 * 6) Value::ArrayLen() - 2 branches
 * 
 * Implementation sequence:
 * 1) Create tinygltf::TinyGLTF loader instance
 * 2) Call LoadASCIIFromString or LoadBinaryFromMemory to parse GLTF data
 * 3) Traverse Model hierarchy to find Value objects (in extras, extensions)
 * 4) For each Value found, call target functions
 * 
 * Semantically different from existing harnesses by focusing on JSON Value access patterns
 * rather than parsing or equality testing, targeting completely uncovered Value API cluster.
 */


// Helper function to recursively traverse and test Value objects
static void testValue(const tinygltf::Value& value, FuzzedDataProvider& fdp) {
    if (value.Type() == tinygltf::NULL_TYPE) return;
    
    // Test Value::Size() - works for both objects and arrays
    size_t obj_size = value.Size();
    (void)obj_size; // Use result to avoid unused variable warning
    
    // Test Value::ArrayLen() - only meaningful for arrays
    size_t array_len = value.ArrayLen();
    (void)array_len;
    
    if (value.IsObject()) {
        // Test Value::Keys() for objects
        std::vector<std::string> keys = value.Keys();
        
        if (!keys.empty()) {
            // Test Value::Has() with keys from the object
            for (const auto& key : keys) {
                bool has_key = value.Has(key);
                (void)has_key;
                
                // Test Value::Get("key") for existing keys
                const tinygltf::Value& nested_value = value.Get(key);
                // Recursively test nested values
                testValue(nested_value, fdp);
            }
            
            // Also test Value::Has() with fuzzer-generated keys (may not exist)
            size_t num_test_keys = fdp.ConsumeIntegralInRange<size_t>(0, 5);
            for (size_t i = 0; i < num_test_keys; ++i) {
                std::string test_key = fdp.ConsumeRandomLengthString(32);
                bool has_test_key = value.Has(test_key);
                (void)has_test_key;
                
                // Even if key doesn't exist, Get returns null_value
                const tinygltf::Value& test_get = value.Get(test_key);
                (void)test_get;
            }
        }
    } else if (value.IsArray()) {
        // Test Value::Get(index) for arrays
        if (array_len > 0) {
            // Test valid indices
            size_t test_idx = fdp.ConsumeIntegralInRange<size_t>(0, array_len - 1);
            const tinygltf::Value& elem = value.Get(test_idx);
            testValue(elem, fdp);
            
            // Also test out-of-bounds index (should return null_value)
            size_t oob_idx = array_len + fdp.ConsumeIntegralInRange<size_t>(0, 10);
            const tinygltf::Value& oob_elem = value.Get(oob_idx);
            (void)oob_elem;
        }
    }
}

// Helper to traverse Model and find Value objects
static void traverseModelForValues(const tinygltf::Model& model, FuzzedDataProvider& fdp) {
    // Test extras Value at model level
    testValue(model.extras, fdp);
    
    // Test extensions (ExtensionMap is map<string, Value>)
    for (const auto& ext_pair : model.extensions) {
        testValue(ext_pair.second, fdp);
    }
    
    // Test extras and extensions in various model components
    // These components often have Value fields for custom data
    
    // Accessors
    for (const auto& accessor : model.accessors) {
        testValue(accessor.extras, fdp);
        for (const auto& ext_pair : accessor.extensions) {
            testValue(ext_pair.second, fdp);
        }
    }
    
    // Animations
    for (const auto& animation : model.animations) {
        testValue(animation.extras, fdp);
        for (const auto& ext_pair : animation.extensions) {
            testValue(ext_pair.second, fdp);
        }
    }
    
    // Buffers
    for (const auto& buffer : model.buffers) {
        testValue(buffer.extras, fdp);
        for (const auto& ext_pair : buffer.extensions) {
            testValue(ext_pair.second, fdp);
        }
    }
    
    // BufferViews
    for (const auto& bufferView : model.bufferViews) {
        testValue(bufferView.extras, fdp);
        for (const auto& ext_pair : bufferView.extensions) {
            testValue(ext_pair.second, fdp);
        }
    }
    
    // Materials
    for (const auto& material : model.materials) {
        testValue(material.extras, fdp);
        for (const auto& ext_pair : material.extensions) {
            testValue(ext_pair.second, fdp);
        }
    }
    
    // Meshes
    for (const auto& mesh : model.meshes) {
        testValue(mesh.extras, fdp);
        for (const auto& ext_pair : mesh.extensions) {
            testValue(ext_pair.second, fdp);
        }
    }
    
    // Nodes
    for (const auto& node : model.nodes) {
        testValue(node.extras, fdp);
        for (const auto& ext_pair : node.extensions) {
            testValue(ext_pair.second, fdp);
        }
    }
    
    // Scenes
    for (const auto& scene : model.scenes) {
        testValue(scene.extras, fdp);
        for (const auto& ext_pair : scene.extensions) {
            testValue(ext_pair.second, fdp);
        }
    }
    
    // Textures
    for (const auto& texture : model.textures) {
        testValue(texture.extras, fdp);
        for (const auto& ext_pair : texture.extensions) {
            testValue(ext_pair.second, fdp);
        }
    }
    
    // Images
    for (const auto& image : model.images) {
        testValue(image.extras, fdp);
        for (const auto& ext_pair : image.extensions) {
            testValue(ext_pair.second, fdp);
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful testing: selector + parsing options + GLTF data
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parsing strategy selector
    uint8_t strategy = fdp.ConsumeIntegral<uint8_t>() % 2; // 0=ASCII, 1=Binary
    
    // Consume parsing options
    uint8_t check_sections = fdp.ConsumeIntegral<uint8_t>();
    bool store_original_json = fdp.ConsumeBool();
    
    // Consume remaining bytes as GLTF data for parsing
    std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (gltf_data.empty()) return 0;
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err, warn;
    
    // Configure parsing options
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
    
    bool parse_success = false;
    
    // Parse GLTF data based on strategy
    if (strategy == 0) {
        // ASCII parsing
        std::string json_str(reinterpret_cast<const char*>(gltf_data.data()), 
                             gltf_data.size());
        parse_success = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                               json_str.c_str(), 
                                               static_cast<unsigned int>(gltf_data.size()),
                                               /* base_dir */ "",
                                               check_sections);
    } else {
        // Binary parsing
        parse_success = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                gltf_data.data(),
                                                static_cast<unsigned int>(gltf_data.size()),
                                                /* base_dir */ "",
                                                check_sections);
    }
    
    // Even if parsing failed, we might have partial model with some Value objects
    // Always attempt to traverse whatever model state we have
    
    // Traverse model to find and test Value objects
    traverseModelForValues(model, fdp);
    
    return 0;
}

} // namespace harness_016
