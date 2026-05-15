#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_010 {
// Harness 010: Targeted equality operator fuzzer for tinygltf
// Specifically targets equality operators for GLTF model components with 0% coverage
// Based on coverage guidance: Model equality (40 branches, 287 undiscovered), Accessor equality (22 branches),
// Image equality (20 branches), BufferView equality (18 branches), Sampler equality (14 branches),
// Skin equality (12 branches), Asset equality (12 branches)
// Supporting APIs: Value equality, Value::Get, Value::Size (uncovered)
// Uses FuzzedDataProvider for structured input splitting to test various comparison scenarios


// Include FuzzedDataProvider for structured input splitting

// Include tinygltf header without implementation macro

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create TinyGLTF loader context
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model1, model2;
    std::string err1, err2, warn1, warn2;
    
    // Consume operation selector (0-4 for different comparison paths)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Consume check_sections flag for loading
    unsigned int check_sections = fdp.ConsumeIntegral<uint8_t>() & 0x7F;
    
    // Path 0: Load two models from same data and compare (should be equal)
    if (operation == 0) {
        // Split input into two parts for loading
        size_t split_point = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes() - 1);
        std::vector<uint8_t> part1 = fdp.ConsumeBytes<uint8_t>(split_point);
        std::vector<uint8_t> part2 = fdp.ConsumeRemainingBytes<uint8_t>();
        
        // Try to load model1 from part1 as ASCII
        bool loaded1 = ctx.LoadASCIIFromString(&model1, &err1, &warn1,
                                               reinterpret_cast<const char*>(part1.data()),
                                               static_cast<unsigned int>(part1.size()),
                                               "", check_sections);
        
        // Try to load model2 from part2 as binary
        bool loaded2 = ctx.LoadBinaryFromMemory(&model2, &err2, &warn2,
                                                part2.data(),
                                                static_cast<unsigned int>(part2.size()),
                                                "", check_sections);
        
        // Test Model equality operator if both loaded
        if (loaded1 && loaded2) {
            bool models_equal = (model1 == model2);
            (void)models_equal; // Use result to avoid dead code elimination
        }
        
        // Also test self-equality (should always be true)
        if (loaded1) {
            bool self_equal = (model1 == model1);
            (void)self_equal;
        }
        if (loaded2) {
            bool self_equal = (model2 == model2);
            (void)self_equal;
        }
    }
    // Path 1: Compare individual components from loaded models
    else if (operation == 1) {
        // Use all remaining data for loading
        std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (!gltf_data.empty()) {
            // Try loading as ASCII first
            bool loaded = ctx.LoadASCIIFromString(&model1, &err1, &warn1,
                                                  reinterpret_cast<const char*>(gltf_data.data()),
                                                  static_cast<unsigned int>(gltf_data.size()),
                                                  "", check_sections);
            
            if (!loaded) {
                // Try as binary
                loaded = ctx.LoadBinaryFromMemory(&model1, &err1, &warn1,
                                                  gltf_data.data(),
                                                  static_cast<unsigned int>(gltf_data.size()),
                                                  "", check_sections);
            }
            
            if (loaded) {
                // Test component equality operators
                
                // Asset equality
                if (!model1.asset.version.empty()) {
                    bool asset_equal = (model1.asset == model1.asset);
                    (void)asset_equal;
                }
                
                // Accessor equality
                for (size_t i = 0; i < model1.accessors.size() && i < 5; i++) {
                    for (size_t j = 0; j < model1.accessors.size() && j < 5; j++) {
                        bool accessor_equal = (model1.accessors[i] == model1.accessors[j]);
                        (void)accessor_equal;
                    }
                }
                
                // Image equality
                for (size_t i = 0; i < model1.images.size() && i < 5; i++) {
                    for (size_t j = 0; j < model1.images.size() && j < 5; j++) {
                        bool image_equal = (model1.images[i] == model1.images[j]);
                        (void)image_equal;
                    }
                }
                
                // BufferView equality
                for (size_t i = 0; i < model1.bufferViews.size() && i < 5; i++) {
                    for (size_t j = 0; j < model1.bufferViews.size() && j < 5; j++) {
                        bool bufferView_equal = (model1.bufferViews[i] == model1.bufferViews[j]);
                        (void)bufferView_equal;
                    }
                }
                
                // Sampler equality
                for (size_t i = 0; i < model1.samplers.size() && i < 5; i++) {
                    for (size_t j = 0; j < model1.samplers.size() && j < 5; j++) {
                        bool sampler_equal = (model1.samplers[i] == model1.samplers[j]);
                        (void)sampler_equal;
                    }
                }
                
                // Skin equality
                for (size_t i = 0; i < model1.skins.size() && i < 5; i++) {
                    for (size_t j = 0; j < model1.skins.size() && j < 5; j++) {
                        bool skin_equal = (model1.skins[i] == model1.skins[j]);
                        (void)skin_equal;
                    }
                }
            }
        }
    }
    // Path 2: Test Value operations on model properties
    else if (operation == 2) {
        // Use all remaining data for loading
        std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (!gltf_data.empty()) {
            tinygltf::Model model;
            std::string err, warn;
            
            bool loaded = ctx.LoadASCIIFromString(&model, &err, &warn,
                                                  reinterpret_cast<const char*>(gltf_data.data()),
                                                  static_cast<unsigned int>(gltf_data.size()),
                                                  "", check_sections);
            
            if (!loaded) {
                loaded = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                  gltf_data.data(),
                                                  static_cast<unsigned int>(gltf_data.size()),
                                                  "", check_sections);
            }
            
            if (loaded) {
                // Test Value::Size() on extras (Value object)
                size_t extras_size = model.extras.Size();
                (void)extras_size;
                
                // Test Value::Get() if extras has content
                if (model.extras.IsObject()) {
                    auto keys = model.extras.Keys();
                    if (!keys.empty()) {
                        // Try to get first key's value
                        try {
                            const tinygltf::Value& val = model.extras.Get(keys[0]);
                            (void)val;
                        } catch (...) {
                            // Ignore exceptions
                        }
                    }
                }
                
                // Test Asset equality with Value operations
                // model.asset.extensions is an ExtensionMap (std::map)
                if (!model.asset.extensions.empty()) {
                    // Test extension map size
                    size_t asset_ext_size = model.asset.extensions.size();
                    (void)asset_ext_size;
                    
                    // Test equality of extension maps (map comparison)
                    bool extensions_equal = (model.asset.extensions == model.asset.extensions);
                    (void)extensions_equal;
                }
                
                // Test Value equality on material extras
                for (size_t i = 0; i < model.materials.size() && i < 3; i++) {
                    // Check if extras is not null by checking type
                    if (model.materials[i].extras.Type() != tinygltf::NULL_TYPE) {
                        bool extras_equal = (model.materials[i].extras == model.materials[i].extras);
                        (void)extras_equal;
                    }
                }
            }
        }
    }
    // Path 3: Create synthetic models for equality testing
    else if (operation == 3) {
        // Consume parameters for synthetic model creation
        bool create_asset = fdp.ConsumeBool();
        bool create_accessor = fdp.ConsumeBool();
        bool create_image = fdp.ConsumeBool();
        bool create_bufferView = fdp.ConsumeBool();
        bool create_sampler = fdp.ConsumeBool();
        bool create_skin = fdp.ConsumeBool();
        
        // Create two empty models
        tinygltf::Model model1, model2;
        
        // Add synthetic Asset if requested
        if (create_asset) {
            tinygltf::Asset asset1, asset2;
            asset1.version = "2.0";
            asset2.version = "2.0";
            asset1.generator = "Fuzzer";
            asset2.generator = "Fuzzer";
            
            // Add some differences based on fuzzer input
            if (fdp.ConsumeBool()) {
                asset1.minVersion = "2.0";
                asset2.minVersion = "2.0";
            }
            
            model1.asset = asset1;
            model2.asset = asset2;
            
            // Test Asset equality
            bool asset_equal = (model1.asset == model2.asset);
            (void)asset_equal;
        }
        
        // Add synthetic Accessor if requested
        if (create_accessor && fdp.remaining_bytes() >= 8) {
            tinygltf::Accessor acc1, acc2;
            acc1.bufferView = fdp.ConsumeIntegral<int>();
            acc2.bufferView = fdp.ConsumeIntegral<int>();
            acc1.byteOffset = fdp.ConsumeIntegral<int>();
            acc2.byteOffset = fdp.ConsumeIntegral<int>();
            acc1.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
            acc2.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
            acc1.count = 1;
            acc2.count = 1;
            acc1.type = TINYGLTF_TYPE_SCALAR;
            acc2.type = TINYGLTF_TYPE_SCALAR;
            
            model1.accessors.push_back(acc1);
            model2.accessors.push_back(acc2);
            
            // Test Accessor equality
            if (!model1.accessors.empty() && !model2.accessors.empty()) {
                bool accessor_equal = (model1.accessors[0] == model2.accessors[0]);
                (void)accessor_equal;
            }
        }
        
        // Add synthetic Image if requested
        if (create_image && fdp.remaining_bytes() >= 4) {
            tinygltf::Image img1, img2;
            img1.width = fdp.ConsumeIntegralInRange<int>(1, 1024);
            img2.width = img1.width;
            img1.height = fdp.ConsumeIntegralInRange<int>(1, 1024);
            img2.height = img1.height;
            img1.component = 4; // RGBA
            img2.component = 4;
            img1.bits = 8;
            img2.bits = 8;
            
            model1.images.push_back(img1);
            model2.images.push_back(img2);
            
            // Test Image equality
            if (!model1.images.empty() && !model2.images.empty()) {
                bool image_equal = (model1.images[0] == model2.images[0]);
                (void)image_equal;
            }
        }
        
        // Test overall Model equality with synthetic components
        bool synthetic_models_equal = (model1 == model2);
        (void)synthetic_models_equal;
    }
    // Path 4: Edge case testing
    else {
        // Test empty model equality
        tinygltf::Model empty1, empty2;
        bool empty_equal = (empty1 == empty2);
        (void)empty_equal;
        
        // Test self-equality for empty model
        bool empty_self_equal = (empty1 == empty1);
        (void)empty_self_equal;
        
        // Test with partial data from remaining input
        if (fdp.remaining_bytes() > 0) {
            // Try to load a model
            std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
            
            if (!gltf_data.empty()) {
                tinygltf::Model model;
                std::string err, warn;
                
                bool loaded = ctx.LoadASCIIFromString(&model, &err, &warn,
                                                      reinterpret_cast<const char*>(gltf_data.data()),
                                                      static_cast<unsigned int>(gltf_data.size()),
                                                      "", check_sections);
                
                if (loaded) {
                    // Compare with empty model (should be false if model has content)
                    bool with_empty_equal = (model == empty1);
                    (void)with_empty_equal;
                    
                    // Test component-by-component comparisons
                    if (!model.accessors.empty()) {
                        // Compare first accessor with itself
                        bool accessor_self = (model.accessors[0] == model.accessors[0]);
                        (void)accessor_self;
                    }
                    
                    if (!model.images.empty()) {
                        // Compare first image with itself
                        bool image_self = (model.images[0] == model.images[0]);
                        (void)image_self;
                    }
                    
                    // Test Value operations on model extensions (ExtensionMap)
                    if (!model.extensions.empty()) {
                        size_t ext_size = model.extensions.size();
                        (void)ext_size;
                        
                        // Test equality of extension maps
                        bool ext_equal = (model.extensions == model.extensions);
                        (void)ext_equal;
                    }
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_010
