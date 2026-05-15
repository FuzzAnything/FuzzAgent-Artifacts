#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_011 {
// Harness 011: Targeted equality operator fuzzer for uncovered GLTF components
// Specifically targets equality operators for components with 0% coverage identified in guidance:
// BufferView (30 undiscovered branches), Sampler (26 undiscovered branches), Skin (24 undiscovered branches),
// AnimationSampler (22 undiscovered branches), Texture (22 undiscovered branches), 
// Scene (20 undiscovered branches), SpotLight (20 undiscovered branches)
// Total: 160+ points of undiscovered complexity
// Based on coverage guidance in harness_011_guidance.md
// Uses FuzzedDataProvider for structured input splitting to test various comparison scenarios


// Include FuzzedDataProvider for structured input splitting

// Include tinygltf header

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create TinyGLTF loader context
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err, warn;
    
    // Consume operation selector (0-3 for different testing strategies)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume check_sections flag for loading
    unsigned int check_sections = fdp.ConsumeIntegral<uint8_t>() & 0x7F;
    
    // Consume base path (empty for memory loading)
    std::string base_path = "";
    
    // Path 0: Load model and test equality operators on components from loaded model
    if (operation == 0) {
        // Use all remaining data for loading
        std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (!gltf_data.empty()) {
            // Try loading as ASCII first
            bool loaded = ctx.LoadASCIIFromString(&model, &err, &warn,
                                                  reinterpret_cast<const char*>(gltf_data.data()),
                                                  static_cast<unsigned int>(gltf_data.size()),
                                                  base_path, check_sections);
            
            if (!loaded) {
                // Try as binary
                loaded = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                  gltf_data.data(),
                                                  static_cast<unsigned int>(gltf_data.size()),
                                                  base_path, check_sections);
            }
            
            if (loaded) {
                // Test BufferView equality operators (30 undiscovered branches)
                for (size_t i = 0; i < model.bufferViews.size() && i < 10; i++) {
                    for (size_t j = 0; j < model.bufferViews.size() && j < 10; j++) {
                        bool bufferView_equal = (model.bufferViews[i] == model.bufferViews[j]);
                        (void)bufferView_equal; // Use result to avoid dead code elimination
                        
                        // Test self-equality (should always be true)
                        if (i == j) {
                            bool self_equal = (model.bufferViews[i] == model.bufferViews[i]);
                            (void)self_equal;
                        }
                    }
                }
                
                // Test Sampler equality operators (26 undiscovered branches)
                for (size_t i = 0; i < model.samplers.size() && i < 10; i++) {
                    for (size_t j = 0; j < model.samplers.size() && j < 10; j++) {
                        bool sampler_equal = (model.samplers[i] == model.samplers[j]);
                        (void)sampler_equal;
                        
                        // Test self-equality
                        if (i == j) {
                            bool self_equal = (model.samplers[i] == model.samplers[i]);
                            (void)self_equal;
                        }
                    }
                }
                
                // Test Skin equality operators (24 undiscovered branches)
                for (size_t i = 0; i < model.skins.size() && i < 10; i++) {
                    for (size_t j = 0; j < model.skins.size() && j < 10; j++) {
                        bool skin_equal = (model.skins[i] == model.skins[j]);
                        (void)skin_equal;
                        
                        // Test self-equality
                        if (i == j) {
                            bool self_equal = (model.skins[i] == model.skins[i]);
                            (void)self_equal;
                        }
                    }
                }
                
                // Test AnimationSampler equality operators (22 undiscovered branches)
                for (size_t anim_idx = 0; anim_idx < model.animations.size() && anim_idx < 5; anim_idx++) {
                    const auto& animation = model.animations[anim_idx];
                    for (size_t i = 0; i < animation.samplers.size() && i < 5; i++) {
                        for (size_t j = 0; j < animation.samplers.size() && j < 5; j++) {
                            bool animSampler_equal = (animation.samplers[i] == animation.samplers[j]);
                            (void)animSampler_equal;
                            
                            // Test self-equality
                            if (i == j) {
                                bool self_equal = (animation.samplers[i] == animation.samplers[i]);
                                (void)self_equal;
                            }
                        }
                    }
                }
                
                // Test Texture equality operators (22 undiscovered branches)
                for (size_t i = 0; i < model.textures.size() && i < 10; i++) {
                    for (size_t j = 0; j < model.textures.size() && j < 10; j++) {
                        bool texture_equal = (model.textures[i] == model.textures[j]);
                        (void)texture_equal;
                        
                        // Test self-equality
                        if (i == j) {
                            bool self_equal = (model.textures[i] == model.textures[i]);
                            (void)self_equal;
                        }
                    }
                }
                
                // Test Scene equality operators (20 undiscovered branches)
                for (size_t i = 0; i < model.scenes.size() && i < 10; i++) {
                    for (size_t j = 0; j < model.scenes.size() && j < 10; j++) {
                        bool scene_equal = (model.scenes[i] == model.scenes[j]);
                        (void)scene_equal;
                        
                        // Test self-equality
                        if (i == j) {
                            bool self_equal = (model.scenes[i] == model.scenes[i]);
                            (void)self_equal;
                        }
                    }
                }
                
                // Test SpotLight equality operators (20 undiscovered branches)
                for (size_t i = 0; i < model.lights.size() && i < 10; i++) {
                    const auto& light = model.lights[i];
                    // Test SpotLight equality if the light has a spot component
                    bool spotLight_equal = (light.spot == light.spot);
                    (void)spotLight_equal;
                    
                    // Compare with other lights' spot components
                    for (size_t j = 0; j < model.lights.size() && j < 10; j++) {
                        const auto& other_light = model.lights[j];
                        bool cross_spot_equal = (light.spot == other_light.spot);
                        (void)cross_spot_equal;
                    }
                }
            }
        }
    }
    // Path 1: Test default-constructed components vs loaded components
    else if (operation == 1) {
        // Use all remaining data for loading
        std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (!gltf_data.empty()) {
            // Try loading as ASCII first
            bool loaded = ctx.LoadASCIIFromString(&model, &err, &warn,
                                                  reinterpret_cast<const char*>(gltf_data.data()),
                                                  static_cast<unsigned int>(gltf_data.size()),
                                                  base_path, check_sections);
            
            if (!loaded) {
                // Try as binary
                loaded = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                  gltf_data.data(),
                                                  static_cast<unsigned int>(gltf_data.size()),
                                                  base_path, check_sections);
            }
            
            if (loaded) {
                // Create default-constructed instances
                tinygltf::BufferView defaultBufferView;
                tinygltf::Sampler defaultSampler;
                tinygltf::Skin defaultSkin;
                tinygltf::AnimationSampler defaultAnimSampler;
                tinygltf::Texture defaultTexture;
                tinygltf::Scene defaultScene;
                tinygltf::SpotLight defaultSpotLight;
                
                // Compare default instances with loaded components
                // BufferView comparisons
                if (!model.bufferViews.empty()) {
                    for (size_t i = 0; i < model.bufferViews.size() && i < 5; i++) {
                        bool bufView_eq_default = (model.bufferViews[i] == defaultBufferView);
                        bool default_eq_bufView = (defaultBufferView == model.bufferViews[i]);
                        (void)bufView_eq_default;
                        (void)default_eq_bufView;
                    }
                }
                
                // Sampler comparisons
                if (!model.samplers.empty()) {
                    for (size_t i = 0; i < model.samplers.size() && i < 5; i++) {
                        bool sampler_eq_default = (model.samplers[i] == defaultSampler);
                        bool default_eq_sampler = (defaultSampler == model.samplers[i]);
                        (void)sampler_eq_default;
                        (void)default_eq_sampler;
                    }
                }
                
                // Skin comparisons
                if (!model.skins.empty()) {
                    for (size_t i = 0; i < model.skins.size() && i < 5; i++) {
                        bool skin_eq_default = (model.skins[i] == defaultSkin);
                        bool default_eq_skin = (defaultSkin == model.skins[i]);
                        (void)skin_eq_default;
                        (void)default_eq_skin;
                    }
                }
                
                // AnimationSampler comparisons
                for (size_t anim_idx = 0; anim_idx < model.animations.size() && anim_idx < 3; anim_idx++) {
                    const auto& animation = model.animations[anim_idx];
                    for (size_t i = 0; i < animation.samplers.size() && i < 3; i++) {
                        bool animSampler_eq_default = (animation.samplers[i] == defaultAnimSampler);
                        bool default_eq_animSampler = (defaultAnimSampler == animation.samplers[i]);
                        (void)animSampler_eq_default;
                        (void)default_eq_animSampler;
                    }
                }
                
                // Texture comparisons
                if (!model.textures.empty()) {
                    for (size_t i = 0; i < model.textures.size() && i < 5; i++) {
                        bool texture_eq_default = (model.textures[i] == defaultTexture);
                        bool default_eq_texture = (defaultTexture == model.textures[i]);
                        (void)texture_eq_default;
                        (void)default_eq_texture;
                    }
                }
                
                // Scene comparisons
                if (!model.scenes.empty()) {
                    for (size_t i = 0; i < model.scenes.size() && i < 5; i++) {
                        bool scene_eq_default = (model.scenes[i] == defaultScene);
                        bool default_eq_scene = (defaultScene == model.scenes[i]);
                        (void)scene_eq_default;
                        (void)default_eq_scene;
                    }
                }
                
                // SpotLight comparisons
                if (!model.lights.empty()) {
                    for (size_t i = 0; i < model.lights.size() && i < 5; i++) {
                        const auto& light = model.lights[i];
                        bool spot_eq_default = (light.spot == defaultSpotLight);
                        bool default_eq_spot = (defaultSpotLight == light.spot);
                        (void)spot_eq_default;
                        (void)default_eq_spot;
                    }
                }
            }
        }
    }
    // Path 2: Test edge cases with modified components
    else if (operation == 2) {
        // Use all remaining data for loading
        std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (!gltf_data.empty()) {
            // Try loading as ASCII first
            bool loaded = ctx.LoadASCIIFromString(&model, &err, &warn,
                                                  reinterpret_cast<const char*>(gltf_data.data()),
                                                  static_cast<unsigned int>(gltf_data.size()),
                                                  base_path, check_sections);
            
            if (!loaded) {
                // Try as binary
                loaded = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                  gltf_data.data(),
                                                  static_cast<unsigned int>(gltf_data.size()),
                                                  base_path, check_sections);
            }
            
            if (loaded) {
                // Create modified copies and test equality
                // BufferView modification
                if (!model.bufferViews.empty()) {
                    tinygltf::BufferView modified = model.bufferViews[0];
                    modified.byteLength += 1; // Modify a field
                    bool bufView_eq_modified = (model.bufferViews[0] == modified);
                    (void)bufView_eq_modified;
                }
                
                // Sampler modification
                if (!model.samplers.empty()) {
                    tinygltf::Sampler modified = model.samplers[0];
                    modified.minFilter = (modified.minFilter == -1) ? 9728 : -1; // Toggle filter
                    bool sampler_eq_modified = (model.samplers[0] == modified);
                    (void)sampler_eq_modified;
                }
                
                // Skin modification
                if (!model.skins.empty()) {
                    tinygltf::Skin modified = model.skins[0];
                    modified.name = "modified_" + modified.name;
                    bool skin_eq_modified = (model.skins[0] == modified);
                    (void)skin_eq_modified;
                }
                
                // AnimationSampler modification
                if (!model.animations.empty() && !model.animations[0].samplers.empty()) {
                    tinygltf::AnimationSampler modified = model.animations[0].samplers[0];
                    modified.interpolation = "modified";
                    bool animSampler_eq_modified = (model.animations[0].samplers[0] == modified);
                    (void)animSampler_eq_modified;
                }
                
                // Texture modification
                if (!model.textures.empty()) {
                    tinygltf::Texture modified = model.textures[0];
                    modified.sampler = (modified.sampler == -1) ? 0 : -1; // Toggle sampler index
                    bool texture_eq_modified = (model.textures[0] == modified);
                    (void)texture_eq_modified;
                }
                
                // Scene modification
                if (!model.scenes.empty()) {
                    tinygltf::Scene modified = model.scenes[0];
                    if (!modified.nodes.empty()) {
                        modified.nodes[0] = (modified.nodes[0] == -1) ? 0 : -1; // Modify a node index
                    }
                    bool scene_eq_modified = (model.scenes[0] == modified);
                    (void)scene_eq_modified;
                }
                
                // SpotLight modification
                if (!model.lights.empty()) {
                    tinygltf::SpotLight modified = model.lights[0].spot;
                    modified.innerConeAngle += 0.1; // Modify angle
                    bool spotLight_eq_modified = (model.lights[0].spot == modified);
                    (void)spotLight_eq_modified;
                }
            }
        }
    }
    // Path 3: Test empty vs populated component comparisons
    else if (operation == 3) {
        // Create empty vectors of components
        std::vector<tinygltf::BufferView> emptyBufferViews;
        std::vector<tinygltf::Sampler> emptySamplers;
        std::vector<tinygltf::Skin> emptySkins;
        std::vector<tinygltf::AnimationSampler> emptyAnimSamplers;
        std::vector<tinygltf::Texture> emptyTextures;
        std::vector<tinygltf::Scene> emptyScenes;
        
        // Create default instances
        tinygltf::BufferView defaultBufView;
        tinygltf::Sampler defaultSampler;
        tinygltf::Skin defaultSkin;
        tinygltf::AnimationSampler defaultAnimSampler;
        tinygltf::Texture defaultTexture;
        tinygltf::Scene defaultScene;
        tinygltf::SpotLight defaultSpotLight;
        
        // Test equality between default instances
        bool bufView_self = (defaultBufView == defaultBufView);
        bool sampler_self = (defaultSampler == defaultSampler);
        bool skin_self = (defaultSkin == defaultSkin);
        bool animSampler_self = (defaultAnimSampler == defaultAnimSampler);
        bool texture_self = (defaultTexture == defaultTexture);
        bool scene_self = (defaultScene == defaultScene);
        bool spotLight_self = (defaultSpotLight == defaultSpotLight);
        
        (void)bufView_self;
        (void)sampler_self;
        (void)skin_self;
        (void)animSampler_self;
        (void)texture_self;
        (void)scene_self;
        (void)spotLight_self;
        
        // Also load a model if there's data
        std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (!gltf_data.empty()) {
            // Try loading as ASCII
            bool loaded = ctx.LoadASCIIFromString(&model, &err, &warn,
                                                  reinterpret_cast<const char*>(gltf_data.data()),
                                                  static_cast<unsigned int>(gltf_data.size()),
                                                  base_path, check_sections);
            
            if (!loaded) {
                // Try as binary
                loaded = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                  gltf_data.data(),
                                                  static_cast<unsigned int>(gltf_data.size()),
                                                  base_path, check_sections);
            }
        }
    }
    
    // Model cleanup happens automatically via destructor
    return 0;
}

} // namespace harness_011
