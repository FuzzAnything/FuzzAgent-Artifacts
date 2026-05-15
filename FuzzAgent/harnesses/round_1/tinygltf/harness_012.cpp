#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

namespace harness_012 {
/*
 * Fuzzing harness for tinygltf library glTF component equality operators
 * 
 * This harness targets 8 high-complexity glTF component equality operators with 160+ undiscovered branches:
 * 1. PositionalEmitter::operator== - 28 undiscovered branches
 * 2. Sampler::operator== - 26 undiscovered branches  
 * 3. Primitive::operator== - 24 undiscovered branches
 * 4. Skin::operator== - 24 undiscovered branches
 * 5. Texture::operator== - 22 undiscovered branches
 * 6. AnimationSampler::operator== - 22 undiscovered branches
 * 7. Buffer::operator== - 22 undiscovered branches
 * 8. AnimationChannel::operator== - 22 undiscovered branches
 * 
 * The harness implements comprehensive equality testing following coverage guidance:
 * 1. Initialize TinyGLTF loader instance
 * 2. Parse glTF model using LoadASCIIFromString/LoadBinaryFromMemory
 * 3. Access various components from Model:
 *    - Positional emitters from audioEmitters
 *    - Samplers from animations
 *    - Primitives from meshes
 *    - Skins from nodes
 *    - Textures from materials
 *    - Buffers and buffer views
 *    - Animation channels and samplers from animations
 * 4. Perform equality comparisons on each component type:
 *    - Compare components with themselves (should return true)
 *    - Compare with default-constructed components (should return false)
 *    - Compare modified components
 * 5. Clean up Model resources
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume selector to choose testing strategy
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume parsing options
    uint8_t check_sections = fdp.ConsumeIntegral<uint8_t>();
    bool store_original_json = fdp.ConsumeBool();
    
    // Consume remaining bytes as glTF data
    std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (gltf_data.empty()) return 0;
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    // Configure parsing options
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
    
    // Try to load the model - ignore failures to allow invalid inputs
    std::string json_str(reinterpret_cast<const char*>(gltf_data.data()), 
                         gltf_data.size());
    
    bool load_success = ctx.LoadASCIIFromString(&model, &err, &warn,
                                               json_str.c_str(),
                                               static_cast<unsigned int>(gltf_data.size()),
                                               /* base_dir */ "",
                                               check_sections);
    
    // If ASCII loading fails, try binary loading
    if (!load_success) {
        load_success = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                               gltf_data.data(),
                                               static_cast<unsigned int>(gltf_data.size()),
                                               /* base_dir */ "",
                                               check_sections);
    }
    
    // Test different equality scenarios based on selector
    switch (selector % 5) {
        case 0: {
            // Strategy 1: Test PositionalEmitter equality operators
            // Access positional emitters from audioEmitters if available
            if (!model.audioEmitters.empty()) {
                for (const auto& audioEmitter : model.audioEmitters) {
                    const auto& positional = audioEmitter.positional;
                    
                    // Test PositionalEmitter::operator==
                    // Compare with itself
                    bool pos_eq_self = (positional == positional);
                    (void)pos_eq_self;
                    
                    // Compare with default-constructed
                    tinygltf::PositionalEmitter default_pos;
                    bool pos_eq_default = (positional == default_pos);
                    (void)pos_eq_default;
                    
                    // Create modified copy
                    tinygltf::PositionalEmitter modified_pos = positional;
                    modified_pos.coneInnerAngle += 0.1;
                    bool pos_eq_modified = (positional == modified_pos);
                    (void)pos_eq_modified;
                }
            }
            
            // Also test standalone PositionalEmitter objects
            tinygltf::PositionalEmitter pos1, pos2;
            bool standalone_pos_eq = (pos1 == pos2);
            (void)standalone_pos_eq;
            
            // Make them different
            pos2.coneOuterAngle = 3.14;
            bool pos_ineq = (pos1 == pos2);
            (void)pos_ineq;
            break;
        }
        
        case 1: {
            // Strategy 2: Test Sampler, Texture, and Buffer equality operators
            // Test Sampler::operator==
            if (!model.samplers.empty()) {
                for (const auto& sampler : model.samplers) {
                    bool sampler_eq_self = (sampler == sampler);
                    (void)sampler_eq_self;
                    
                    tinygltf::Sampler default_sampler;
                    bool sampler_eq_default = (sampler == default_sampler);
                    (void)sampler_eq_default;
                    
                    tinygltf::Sampler modified_sampler = sampler;
                    modified_sampler.minFilter = (sampler.minFilter == -1) ? 9728 : -1;
                    bool sampler_eq_modified = (sampler == modified_sampler);
                    (void)sampler_eq_modified;
                }
            }
            
            // Test Texture::operator==
            if (!model.textures.empty()) {
                for (const auto& texture : model.textures) {
                    bool texture_eq_self = (texture == texture);
                    (void)texture_eq_self;
                    
                    tinygltf::Texture default_texture;
                    bool texture_eq_default = (texture == default_texture);
                    (void)texture_eq_default;
                    
                    tinygltf::Texture modified_texture = texture;
                    modified_texture.sampler = (texture.sampler == -1) ? 0 : -1;
                    bool texture_eq_modified = (texture == modified_texture);
                    (void)texture_eq_modified;
                }
            }
            
            // Test Buffer::operator==
            if (!model.buffers.empty()) {
                for (const auto& buffer : model.buffers) {
                    bool buffer_eq_self = (buffer == buffer);
                    (void)buffer_eq_self;
                    
                    tinygltf::Buffer default_buffer;
                    bool buffer_eq_default = (buffer == default_buffer);
                    (void)buffer_eq_default;
                    
                    tinygltf::Buffer modified_buffer = buffer;
                    if (!modified_buffer.data.empty()) {
                        modified_buffer.data[0] ^= 0xFF;
                    }
                    bool buffer_eq_modified = (buffer == modified_buffer);
                    (void)buffer_eq_modified;
                }
            }
            break;
        }
        
        case 2: {
            // Strategy 3: Test Primitive and Skin equality operators
            // Test Primitive::operator==
            for (const auto& mesh : model.meshes) {
                for (const auto& primitive : mesh.primitives) {
                    bool primitive_eq_self = (primitive == primitive);
                    (void)primitive_eq_self;
                    
                    tinygltf::Primitive default_primitive;
                    bool primitive_eq_default = (primitive == default_primitive);
                    (void)primitive_eq_default;
                    
                    tinygltf::Primitive modified_primitive = primitive;
                    modified_primitive.material = (primitive.material == -1) ? 0 : -1;
                    bool primitive_eq_modified = (primitive == modified_primitive);
                    (void)primitive_eq_modified;
                }
            }
            
            // Test Skin::operator==
            if (!model.skins.empty()) {
                for (const auto& skin : model.skins) {
                    bool skin_eq_self = (skin == skin);
                    (void)skin_eq_self;
                    
                    tinygltf::Skin default_skin;
                    bool skin_eq_default = (skin == default_skin);
                    (void)skin_eq_default;
                    
                    tinygltf::Skin modified_skin = skin;
                    if (!modified_skin.joints.empty()) {
                        modified_skin.joints[0] = (skin.joints[0] == 0) ? 1 : 0;
                    }
                    bool skin_eq_modified = (skin == modified_skin);
                    (void)skin_eq_modified;
                }
            }
            break;
        }
        
        case 3: {
            // Strategy 4: Test AnimationSampler and AnimationChannel equality operators
            if (!model.animations.empty()) {
                for (const auto& animation : model.animations) {
                    // Test AnimationSampler::operator==
                    for (const auto& sampler : animation.samplers) {
                        bool anim_sampler_eq_self = (sampler == sampler);
                        (void)anim_sampler_eq_self;
                        
                        tinygltf::AnimationSampler default_anim_sampler;
                        bool anim_sampler_eq_default = (sampler == default_anim_sampler);
                        (void)anim_sampler_eq_default;
                        
                        tinygltf::AnimationSampler modified_anim_sampler = sampler;
                        modified_anim_sampler.interpolation = "STEP";
                        bool anim_sampler_eq_modified = (sampler == modified_anim_sampler);
                        (void)anim_sampler_eq_modified;
                    }
                    
                    // Test AnimationChannel::operator==
                    for (const auto& channel : animation.channels) {
                        bool anim_channel_eq_self = (channel == channel);
                        (void)anim_channel_eq_self;
                        
                        tinygltf::AnimationChannel default_anim_channel;
                        bool anim_channel_eq_default = (channel == default_anim_channel);
                        (void)anim_channel_eq_default;
                        
                        tinygltf::AnimationChannel modified_anim_channel = channel;
                        modified_anim_channel.target_node = (channel.target_node == -1) ? 0 : -1;
                        bool anim_channel_eq_modified = (channel == modified_anim_channel);
                        (void)anim_channel_eq_modified;
                    }
                }
            }
            
            // Test standalone AnimationSampler and AnimationChannel objects
            tinygltf::AnimationSampler anim_sampler1, anim_sampler2;
            bool standalone_anim_sampler_eq = (anim_sampler1 == anim_sampler2);
            (void)standalone_anim_sampler_eq;
            
            tinygltf::AnimationChannel anim_channel1, anim_channel2;
            bool standalone_anim_channel_eq = (anim_channel1 == anim_channel2);
            (void)standalone_anim_channel_eq;
            break;
        }
        
        case 4: {
            // Strategy 5: Comprehensive testing of all target equality operators together
            // Create default-constructed instances of all target types
            tinygltf::PositionalEmitter pos1, pos2;
            tinygltf::Sampler samp1, samp2;
            tinygltf::Primitive prim1, prim2;
            tinygltf::Skin skin1, skin2;
            tinygltf::Texture tex1, tex2;
            tinygltf::AnimationSampler anim_samp1, anim_samp2;
            tinygltf::Buffer buf1, buf2;
            tinygltf::AnimationChannel anim_chan1, anim_chan2;
            
            // Test all equality operators with default-constructed objects
            bool all_default_eq = true;
            all_default_eq = all_default_eq && (pos1 == pos2);
            all_default_eq = all_default_eq && (samp1 == samp2);
            all_default_eq = all_default_eq && (prim1 == prim2);
            all_default_eq = all_default_eq && (skin1 == skin2);
            all_default_eq = all_default_eq && (tex1 == tex2);
            all_default_eq = all_default_eq && (anim_samp1 == anim_samp2);
            all_default_eq = all_default_eq && (buf1 == buf2);
            all_default_eq = all_default_eq && (anim_chan1 == anim_chan2);
            (void)all_default_eq;
            
            // Modify each object and test inequality
            pos2.coneInnerAngle = 1.0;
            samp2.minFilter = 9728;
            prim2.material = 0;
            if (skin2.joints.empty()) {
                skin2.joints.push_back(0);
            }
            tex2.sampler = 0;
            anim_samp2.interpolation = "STEP";
            if (buf2.data.empty()) {
                buf2.data.push_back(0);
            }
            anim_chan2.target_node = 0;
            
            bool all_modified_ineq = true;
            all_modified_ineq = all_modified_ineq && !(pos1 == pos2);
            all_modified_ineq = all_modified_ineq && !(samp1 == samp2);
            all_modified_ineq = all_modified_ineq && !(prim1 == prim2);
            all_modified_ineq = all_modified_ineq && !(skin1 == skin2);
            all_modified_ineq = all_modified_ineq && !(tex1 == tex2);
            all_modified_ineq = all_modified_ineq && !(anim_samp1 == anim_samp2);
            all_modified_ineq = all_modified_ineq && !(buf1 == buf2);
            all_modified_ineq = all_modified_ineq && !(anim_chan1 == anim_chan2);
            (void)all_modified_ineq;
            
            // Test self-equality for all types
            bool all_self_eq = true;
            all_self_eq = all_self_eq && (pos1 == pos1);
            all_self_eq = all_self_eq && (samp1 == samp1);
            all_self_eq = all_self_eq && (prim1 == prim1);
            all_self_eq = all_self_eq && (skin1 == skin1);
            all_self_eq = all_self_eq && (tex1 == tex1);
            all_self_eq = all_self_eq && (anim_samp1 == anim_samp1);
            all_self_eq = all_self_eq && (buf1 == buf1);
            all_self_eq = all_self_eq && (anim_chan1 == anim_chan1);
            (void)all_self_eq;
            break;
        }
    }
    
    return 0;
}

} // namespace harness_012
