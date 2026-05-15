#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_015 {
/*
 * Fuzzing harness for tinygltf equality operators comprehensive testing
 * 
 * This harness targets 29+ equality operators for GLTF data structures with
 * 386+ undiscovered branches. Focuses on implementing the invocation sequence:
 * 1) Parse GLTF input (ASCII or binary) into Model objects
 * 2) Extract specific object types from Model (scenes, lights, cameras, etc.)
 * 3) For each object type, get two instances and call equality operator: object1 == object2
 * 4) Test both identical objects and different objects
 * 
 * Semantically different from existing harnesses by focusing on object comparison logic
 * rather than parsing, targeting the completely untouched equality operator cluster.
 * 
 * Target equality operators (from coverage guidance):
 * - Scene::operator== (20 branches)
 * - SpotLight::operator== (20 branches)
 * - Parameter::operator== (18 branches)
 * - AudioEmitter::operator== (14 branches)
 * - Light::operator== (6 branches)
 * - Camera::operator== (12 branches)
 * - Node::operator== (28 branches)
 * - Animation::operator== (10 branches)
 * - Accessor::operator== (22 branches)
 * - Image::operator== (20 branches)
 * - Mesh::operator== (10 branches)
 * - BufferView::operator== (18 branches)
 * - Plus 17+ additional equality operators for other GLTF types
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful testing: selector + parsing options + some GLTF data
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume selector for testing strategy
    uint8_t strategy = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume parsing options
    uint8_t check_sections = fdp.ConsumeIntegral<uint8_t>();
    bool store_original_json = fdp.ConsumeBool();
    // Consume object type selector (which object type to test equality for)
    uint8_t object_type_selector = fdp.ConsumeIntegral<uint8_t>() % 16;
    
    // Consume remaining bytes as GLTF data for parsing
    std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (gltf_data.empty()) return 0;
    
    tinygltf::Model model1, model2;
    tinygltf::TinyGLTF ctx;
    std::string err1, err2, warn1, warn2;
    
    // Configure parsing options
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
    
    bool parse_success1 = false;
    bool parse_success2 = false;
    
    // Parse the same GLTF data into two models (identical objects case)
    switch (strategy) {
        case 0: {
            // ASCII parsing for both models
            std::string json_str(reinterpret_cast<const char*>(gltf_data.data()), 
                                 gltf_data.size());
            parse_success1 = ctx.LoadASCIIFromString(&model1, &err1, &warn1, 
                                                     json_str.c_str(), 
                                                     static_cast<unsigned int>(gltf_data.size()),
                                                     /* base_dir */ "",
                                                     check_sections);
            parse_success2 = ctx.LoadASCIIFromString(&model2, &err2, &warn2, 
                                                     json_str.c_str(), 
                                                     static_cast<unsigned int>(gltf_data.size()),
                                                     /* base_dir */ "",
                                                     check_sections);
            break;
        }
        
        case 1: {
            // Binary parsing for both models
            parse_success1 = ctx.LoadBinaryFromMemory(&model1, &err1, &warn1,
                                                      gltf_data.data(),
                                                      static_cast<unsigned int>(gltf_data.size()),
                                                      /* base_dir */ "",
                                                      check_sections);
            parse_success2 = ctx.LoadBinaryFromMemory(&model2, &err2, &warn2,
                                                      gltf_data.data(),
                                                      static_cast<unsigned int>(gltf_data.size()),
                                                      /* base_dir */ "",
                                                      check_sections);
            break;
        }
        
        case 2: {
            // Mixed parsing: ASCII for model1, Binary for model2 (different objects case)
            std::string json_str(reinterpret_cast<const char*>(gltf_data.data()), 
                                 gltf_data.size());
            parse_success1 = ctx.LoadASCIIFromString(&model1, &err1, &warn1, 
                                                     json_str.c_str(), 
                                                     static_cast<unsigned int>(gltf_data.size()),
                                                     /* base_dir */ "",
                                                     check_sections);
            parse_success2 = ctx.LoadBinaryFromMemory(&model2, &err2, &warn2,
                                                      gltf_data.data(),
                                                      static_cast<unsigned int>(gltf_data.size()),
                                                      /* base_dir */ "",
                                                      check_sections);
            break;
        }
    }
    
    // If parsing failed for both models, return early
    if (!parse_success1 && !parse_success2) return 0;
    
    // Test equality operators based on object type selector
    switch (object_type_selector) {
        case 0: {
            // Test Scene equality operators
            if (!model1.scenes.empty() && !model2.scenes.empty()) {
                // Compare first scenes from each model
                bool equal = model1.scenes[0] == model2.scenes[0];
                (void)equal; // Result intentionally ignored
                
                // Also test with identical scenes from same model if available
                if (model1.scenes.size() > 1) {
                    bool identical_equal = model1.scenes[0] == model1.scenes[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        
        case 1: {
            // Test Light equality operators
            if (!model1.lights.empty() && !model2.lights.empty()) {
                bool equal = model1.lights[0] == model2.lights[0];
                (void)equal;
                
                if (model1.lights.size() > 1) {
                    bool identical_equal = model1.lights[0] == model1.lights[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        
        case 2: {
            // Test Camera equality operators
            if (!model1.cameras.empty() && !model2.cameras.empty()) {
                bool equal = model1.cameras[0] == model2.cameras[0];
                (void)equal;
                
                if (model1.cameras.size() > 1) {
                    bool identical_equal = model1.cameras[0] == model1.cameras[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        
        case 3: {
            // Test Node equality operators
            if (!model1.nodes.empty() && !model2.nodes.empty()) {
                bool equal = model1.nodes[0] == model2.nodes[0];
                (void)equal;
                
                if (model1.nodes.size() > 1) {
                    bool identical_equal = model1.nodes[0] == model1.nodes[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        
        case 4: {
            // Test Animation equality operators
            if (!model1.animations.empty() && !model2.animations.empty()) {
                bool equal = model1.animations[0] == model2.animations[0];
                (void)equal;
                
                if (model1.animations.size() > 1) {
                    bool identical_equal = model1.animations[0] == model1.animations[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        
        case 5: {
            // Test Accessor equality operators
            if (!model1.accessors.empty() && !model2.accessors.empty()) {
                bool equal = model1.accessors[0] == model2.accessors[0];
                (void)equal;
                
                if (model1.accessors.size() > 1) {
                    bool identical_equal = model1.accessors[0] == model1.accessors[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        
        case 6: {
            // Test Image equality operators
            if (!model1.images.empty() && !model2.images.empty()) {
                bool equal = model1.images[0] == model2.images[0];
                (void)equal;
                
                if (model1.images.size() > 1) {
                    bool identical_equal = model1.images[0] == model1.images[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        
        case 7: {
            // Test Mesh equality operators
            if (!model1.meshes.empty() && !model2.meshes.empty()) {
                bool equal = model1.meshes[0] == model2.meshes[0];
                (void)equal;
                
                if (model1.meshes.size() > 1) {
                    bool identical_equal = model1.meshes[0] == model1.meshes[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        
        case 8: {
            // Test BufferView equality operators
            if (!model1.bufferViews.empty() && !model2.bufferViews.empty()) {
                bool equal = model1.bufferViews[0] == model2.bufferViews[0];
                (void)equal;
                
                if (model1.bufferViews.size() > 1) {
                    bool identical_equal = model1.bufferViews[0] == model1.bufferViews[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        
        case 9: {
            // Test Material equality operators
            if (!model1.materials.empty() && !model2.materials.empty()) {
                bool equal = model1.materials[0] == model2.materials[0];
                (void)equal;
                
                if (model1.materials.size() > 1) {
                    bool identical_equal = model1.materials[0] == model1.materials[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        
        case 10: {
            // Test Texture equality operators
            if (!model1.textures.empty() && !model2.textures.empty()) {
                bool equal = model1.textures[0] == model2.textures[0];
                (void)equal;
                
                if (model1.textures.size() > 1) {
                    bool identical_equal = model1.textures[0] == model1.textures[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        case 11: {
            // Test Sampler equality operators
            if (!model1.samplers.empty() && !model2.samplers.empty()) {
                bool equal = model1.samplers[0] == model2.samplers[0];
                (void)equal;
                
                if (model1.samplers.size() > 1) {
                    bool identical_equal = model1.samplers[0] == model1.samplers[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        
        case 12: {
            // Test AudioEmitter equality operators
            if (!model1.audioEmitters.empty() && !model2.audioEmitters.empty()) {
                bool equal = model1.audioEmitters[0] == model2.audioEmitters[0];
                (void)equal;
                
                if (model1.audioEmitters.size() > 1) {
                    bool identical_equal = model1.audioEmitters[0] == model1.audioEmitters[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        
        case 13: {
            // Test Skin equality operators
            if (!model1.skins.empty() && !model2.skins.empty()) {
                bool equal = model1.skins[0] == model2.skins[0];
                (void)equal;
                
                if (model1.skins.size() > 1) {
                    bool identical_equal = model1.skins[0] == model1.skins[0];
                    (void)identical_equal;
                }
            }
            break;
        }
        case 14: {
            // Test Parameter equality operators (legacy interface)
            // Create Parameter objects directly for testing
            tinygltf::Parameter param1, param2;
            bool param_equal = param1 == param2;
            (void)param_equal;
            
            // Also test with modified parameters
            param1.bool_value = true;
            param2.bool_value = false;
            bool param_not_equal = param1 == param2;
            (void)param_not_equal;
            break;
        }
        
        case 15: {
            // Test SpotLight equality operators
            // SpotLight is a member of Light struct, so we need to access it from lights
            if (!model1.lights.empty() && !model2.lights.empty()) {
                const auto& spot1 = model1.lights[0].spot;
                const auto& spot2 = model2.lights[0].spot;
                bool spot_equal = spot1 == spot2;
                (void)spot_equal;
                
                // Also test with identical spot lights
                bool identical_spot_equal = spot1 == spot1;
                (void)identical_spot_equal;
            }
            break;
        }
        
        default: {
            // Test Model equality operator itself
            bool model_equal = model1 == model2;
            (void)model_equal;
            break;
        }
    }
    
    // This ensures we test as many equality operators as possible
    
    // Test Scene equality if both models have scenes
    if (!model1.scenes.empty() && !model2.scenes.empty()) {
        // Compare first element from each
        bool scene_equal = model1.scenes[0] == model2.scenes[0];
        (void)scene_equal;
    }
    
    // Test Light equality
    if (!model1.lights.empty() && !model2.lights.empty()) {
        bool light_equal = model1.lights[0] == model2.lights[0];
        (void)light_equal;
    }
    
    // Test Camera equality
    if (!model1.cameras.empty() && !model2.cameras.empty()) {
        bool camera_equal = model1.cameras[0] == model2.cameras[0];
        (void)camera_equal;
    }
    
    // Test Node equality
    if (!model1.nodes.empty() && !model2.nodes.empty()) {
        bool node_equal = model1.nodes[0] == model2.nodes[0];
        (void)node_equal;
    }
    
    // Test Animation equality
    if (!model1.animations.empty() && !model2.animations.empty()) {
        bool animation_equal = model1.animations[0] == model2.animations[0];
        (void)animation_equal;
    }
    
    // Test Accessor equality
    if (!model1.accessors.empty() && !model2.accessors.empty()) {
        bool accessor_equal = model1.accessors[0] == model2.accessors[0];
        (void)accessor_equal;
    }
    
    // Test Image equality
    if (!model1.images.empty() && !model2.images.empty()) {
        bool image_equal = model1.images[0] == model2.images[0];
        (void)image_equal;
    }
    
    // Test Mesh equality
    if (!model1.meshes.empty() && !model2.meshes.empty()) {
        bool mesh_equal = model1.meshes[0] == model2.meshes[0];
        (void)mesh_equal;
    }
    
    // Test BufferView equality
    if (!model1.bufferViews.empty() && !model2.bufferViews.empty()) {
        bool bufferView_equal = model1.bufferViews[0] == model2.bufferViews[0];
        (void)bufferView_equal;
    }
    
    // Test Material equality
    if (!model1.materials.empty() && !model2.materials.empty()) {
        bool material_equal = model1.materials[0] == model2.materials[0];
        (void)material_equal;
    }
    
    // Test Texture equality
    if (!model1.textures.empty() && !model2.textures.empty()) {
        bool texture_equal = model1.textures[0] == model2.textures[0];
        (void)texture_equal;
    }
    
    // Test Sampler equality
    if (!model1.samplers.empty() && !model2.samplers.empty()) {
        bool sampler_equal = model1.samplers[0] == model2.samplers[0];
        (void)sampler_equal;
    }
    
    // Test Skin equality if present
    if (!model1.skins.empty() && !model2.skins.empty()) {
        bool skin_equal = model1.skins[0] == model2.skins[0];
        (void)skin_equal;
    }
    
    // Test AudioEmitter equality if present (from KHR_audio extension)
    if (!model1.audioEmitters.empty() && !model2.audioEmitters.empty()) {
        bool audioEmitter_equal = model1.audioEmitters[0] == model2.audioEmitters[0];
        (void)audioEmitter_equal;
    }
    
    // Test AudioSource equality if present
    if (!model1.audioSources.empty() && !model2.audioSources.empty()) {
        bool audioSource_equal = model1.audioSources[0] == model2.audioSources[0];
        (void)audioSource_equal;
    }
    
    return 0;
}

} // namespace harness_015
