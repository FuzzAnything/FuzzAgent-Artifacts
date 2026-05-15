#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_020 {
/*
 * Fuzzing harness for tinygltf library targeting GLTF object comparison operators (eq functions)
 * Goal: Exercise comparison operators for various GLTF structures to cover 100+ undiscovered branches
 * Primary Target APIs (with undiscovered branch complexity from coverage analysis):
 *   - PositionalEmitter::operator== (20 undiscovered branches)
 *   - AudioSource::operator== (4 undiscovered branches)  
 *   - AnimationSampler::operator== (14 undiscovered branches)
 *   - Animation::operator== (34 undiscovered branches)
 *   - AnimationChannel::operator== (14 undiscovered branches)
 *   - SpotLight::operator==
 *   - Material::operator==
 * Strategy: Load-Compare Pipeline
 *   1. Initialization Phase: Create TinyGLTF loader instance
 *   2. Loading Phase: Load GLTF model from fuzzer input (ASCII or binary based on input)
 *   3. Extraction Phase: Extract various objects from loaded Model (cameras, lights, animations, materials, etc.)
 *   4. Comparison Phase: Perform comparisons between objects using operator==
 *   5. Edge Case Testing: Test null/empty/partial objects and self-comparisons
 * Dependency Chain: 
 *   - Initialize TinyGLTF loader
 *   - Load GLTF model using LoadASCIIFromString or LoadBinaryFromMemory
 *   - Access Model collections to get objects for comparison
 *   - Call operator== on target structures
 *   - Automatic cleanup via destructors
 * Differentiation from existing harnesses:
 *   - harness_000: Basic loading only, no object comparisons
 *   - harness_001-019: Various loading/parsing/image operations, not focused on comparison operators
 *   - harness_020: FOCUSED ON COMPARISON OPERATORS for GLTF structures
 * Unique Value: Targets largest cluster of uncovered APIs (40+ comparison operators) with 100+ undiscovered branches,
 *               exercises fundamental GLTF object equality logic, tests comparison paths rarely hit by existing fuzzing.
 */



// Include tinygltf header

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough data for meaningful GLTF testing
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t loader_type = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0 = ASCII, 1 = Binary
    uint8_t comparison_strategy = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool test_empty_objects = fdp.ConsumeBool();
    bool test_self_comparison = fdp.ConsumeBool();
    bool test_partial_objects = fdp.ConsumeBool();
    
    // Get the remaining bytes as GLTF data
    std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (gltf_data.empty()) {
        return 0;
    }
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    bool load_success = false;
    
    // Try loading based on loader_type
    if (loader_type == 0) {
        // ASCII loading
        std::string ascii_str(reinterpret_cast<const char*>(gltf_data.data()), 
                              std::min<size_t>(gltf_data.size(), 1024 * 1024)); // Limit to 1MB
        load_success = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                              ascii_str.c_str(), 
                                              static_cast<unsigned int>(ascii_str.size()),
                                              /* base_dir */ "");
    } else {
        // Binary loading
        load_success = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                               gltf_data.data(),
                                               static_cast<unsigned int>(gltf_data.size()),
                                               /* base_dir */ "");
    }
    
    // Even if loading fails, we may have partial model data to test
    // We'll proceed with comparison testing on whatever data we have
    
    // ============================================
    // COMPARISON OPERATOR TESTING
    // ============================================
    
    // 1. PositionalEmitter comparisons
    if (!model.audioEmitters.empty()) {
        for (size_t i = 0; i < model.audioEmitters.size(); i++) {
            const auto& emitter = model.audioEmitters[i];
            
            // Test self-comparison (should always be true)
            if (test_self_comparison) {
                bool self_eq = (emitter.positional == emitter.positional);
                (void)self_eq;
            }
            
            // Test comparisons with other emitters
            if (i + 1 < model.audioEmitters.size()) {
                bool cross_eq = (emitter.positional == model.audioEmitters[i + 1].positional);
                (void)cross_eq;
            }
        }
    }
    
    // 2. AudioSource comparisons
    if (!model.audioSources.empty()) {
        for (size_t i = 0; i < model.audioSources.size(); i++) {
            const auto& source = model.audioSources[i];
            
            // Test self-comparison
            if (test_self_comparison) {
                bool self_eq = (source == source);
                (void)self_eq;
            }
            
            // Test comparisons with other sources
            if (i + 1 < model.audioSources.size()) {
                bool cross_eq = (source == model.audioSources[i + 1]);
                (void)cross_eq;
            }
        }
    }
    
    // 3. AnimationSampler comparisons
    if (!model.animations.empty()) {
        for (const auto& animation : model.animations) {
            if (!animation.samplers.empty()) {
                for (size_t i = 0; i < animation.samplers.size(); i++) {
                    const auto& sampler = animation.samplers[i];
                    
                    // Test self-comparison
                    if (test_self_comparison) {
                        bool self_eq = (sampler == sampler);
                        (void)self_eq;
                    }
                    
                    // Test comparisons with other samplers in same animation
                    if (i + 1 < animation.samplers.size()) {
                        bool cross_eq = (sampler == animation.samplers[i + 1]);
                        (void)cross_eq;
                    }
                }
            }
        }
    }
    
    // 4. Animation comparisons
    if (!model.animations.empty()) {
        for (size_t i = 0; i < model.animations.size(); i++) {
            const auto& animation = model.animations[i];
            
            // Test self-comparison
            if (test_self_comparison) {
                bool self_eq = (animation == animation);
                (void)self_eq;
            }
            
            // Test comparisons with other animations
            if (i + 1 < model.animations.size()) {
                bool cross_eq = (animation == model.animations[i + 1]);
                (void)cross_eq;
            }
        }
    }
    
    // 5. AnimationChannel comparisons
    if (!model.animations.empty()) {
        for (const auto& animation : model.animations) {
            if (!animation.channels.empty()) {
                for (size_t i = 0; i < animation.channels.size(); i++) {
                    const auto& channel = animation.channels[i];
                    
                    // Test self-comparison
                    if (test_self_comparison) {
                        bool self_eq = (channel == channel);
                        (void)self_eq;
                    }
                    
                    // Test comparisons with other channels in same animation
                    if (i + 1 < animation.channels.size()) {
                        bool cross_eq = (channel == animation.channels[i + 1]);
                        (void)cross_eq;
                    }
                }
            }
        }
    }
    
    // 6. SpotLight comparisons (via Light structures)
    if (!model.lights.empty()) {
        for (size_t i = 0; i < model.lights.size(); i++) {
            const auto& light = model.lights[i];
            
            // Test self-comparison
            if (test_self_comparison) {
                bool self_eq = (light == light);
                (void)self_eq;
            }
            
            // Test comparisons with other lights
            if (i + 1 < model.lights.size()) {
                bool cross_eq = (light == model.lights[i + 1]);
                (void)cross_eq;
            }
            
            // Specifically test SpotLight comparison if this is a spot light
            // (Note: SpotLight is a member of Light struct)
            if (light.type == "spot") {
                // Test spot.spot comparison
                bool spot_eq = (light.spot == light.spot);
                (void)spot_eq;
                
                if (i + 1 < model.lights.size() && model.lights[i + 1].type == "spot") {
                    bool cross_spot_eq = (light.spot == model.lights[i + 1].spot);
                    (void)cross_spot_eq;
                }
            }
        }
    }
    
    // 7. Material comparisons
    if (!model.materials.empty()) {
        for (size_t i = 0; i < model.materials.size(); i++) {
            const auto& material = model.materials[i];
            
            // Test self-comparison
            if (test_self_comparison) {
                bool self_eq = (material == material);
                (void)self_eq;
            }
            
            // Test comparisons with other materials
            if (i + 1 < model.materials.size()) {
                bool cross_eq = (material == model.materials[i + 1]);
                (void)cross_eq;
            }
        }
    }
    
    // 8. Test edge cases with empty/default constructed objects
    if (test_empty_objects) {
        // Create default objects and compare them
        tinygltf::PositionalEmitter empty_pos_emitter1, empty_pos_emitter2;
        bool empty_pos_eq = (empty_pos_emitter1 == empty_pos_emitter2);
        (void)empty_pos_eq;
        
        tinygltf::AudioSource empty_audio_source1, empty_audio_source2;
        bool empty_audio_eq = (empty_audio_source1 == empty_audio_source2);
        (void)empty_audio_eq;
        
        tinygltf::AnimationSampler empty_sampler1, empty_sampler2;
        bool empty_sampler_eq = (empty_sampler1 == empty_sampler2);
        (void)empty_sampler_eq;
        
        tinygltf::Animation empty_animation1, empty_animation2;
        bool empty_animation_eq = (empty_animation1 == empty_animation2);
        (void)empty_animation_eq;
        
        tinygltf::AnimationChannel empty_channel1, empty_channel2;
        bool empty_channel_eq = (empty_channel1 == empty_channel2);
        (void)empty_channel_eq;
        
        tinygltf::SpotLight empty_spotlight1, empty_spotlight2;
        bool empty_spotlight_eq = (empty_spotlight1 == empty_spotlight2);
        (void)empty_spotlight_eq;
        
        tinygltf::Material empty_material1, empty_material2;
        bool empty_material_eq = (empty_material1 == empty_material2);
        (void)empty_material_eq;
    }
    
    // 9. Test partial object comparisons (if we have partial model data)
    if (test_partial_objects && load_success) {
        // Compare model with itself
        bool model_self_eq = (model == model);
        (void)model_self_eq;
        
        // Compare various other structures in the model
        if (!model.cameras.empty()) {
            for (size_t i = 0; i < model.cameras.size(); i++) {
                bool camera_eq = (model.cameras[i] == model.cameras[i]);
                (void)camera_eq;
                
                if (i + 1 < model.cameras.size()) {
                    bool cross_camera_eq = (model.cameras[i] == model.cameras[i + 1]);
                    (void)cross_camera_eq;
                }
            }
        }
        
        if (!model.meshes.empty()) {
            for (size_t i = 0; i < model.meshes.size(); i++) {
                bool mesh_eq = (model.meshes[i] == model.meshes[i]);
                (void)mesh_eq;
                
                if (i + 1 < model.meshes.size()) {
                    bool cross_mesh_eq = (model.meshes[i] == model.meshes[i + 1]);
                    (void)cross_mesh_eq;
                }
            }
        }
        
        if (!model.nodes.empty()) {
            for (size_t i = 0; i < model.nodes.size(); i++) {
                bool node_eq = (model.nodes[i] == model.nodes[i]);
                (void)node_eq;
                
                if (i + 1 < model.nodes.size()) {
                    bool cross_node_eq = (model.nodes[i] == model.nodes[i + 1]);
                    (void)cross_node_eq;
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_020
