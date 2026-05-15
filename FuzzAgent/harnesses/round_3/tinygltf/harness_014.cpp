#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_014 {
/*
 * Fuzzing harness for tinygltf library targeting comprehensive glTF model and component comparison operations
 * Goal: Exercise equality operators through systematic model modification and comparison
 * Primary Targets:
 *   - _ZNK8tinygltf5ModeleqERKS0_ (Model equality operator: 194 undiscovered branches)
 *   - _ZNK8tinygltf17PositionalEmittereqERKS0_ (PositionalEmitter equality operator: 20 undiscovered branches)
 *   - _ZNK8tinygltf7TextureeqERKS0_ (Texture equality operator: 14 undiscovered branches)
 *   - _ZNK8tinygltf5SceneeqERKS0_ (Scene equality operator: 12 undiscovered branches)
 *   - _ZNK8tinygltf9SpotLighteqERKS0_ (SpotLight equality operator: 12 undiscovered branches)
 *   - _ZNK8tinygltf5LighteqERKS0_ (Light equality operator: 6 undiscovered branches)
 *   - _ZNK8tinygltf11AudioSourceeqERKS0_ (AudioSource equality operator: 4 undiscovered branches)
 * Required Setup: Load glTF models using TinyGLTF::LoadASCIIFromString/LoadBinaryFromMemory, create model copies,
 *                 modify specific components, and perform systematic comparisons
 * Strategy: Load a base glTF model, create a copy, systematically modify individual components (textures, scenes, lights, etc.),
 *           and test equality operators after each modification to exercise different code paths
 * Differentiation from existing harnesses (harness_000 to harness_013):
 *   - harness_000: Basic GLTF parsing (loading/deserialization)
 *   - harness_001: Image loading via embedded images
 *   - harness_002: Image writing using callback APIs
 *   - harness_003: General equality operators (Model, Node, Material, Camera, Animation)
 *   - harness_004: File loading callbacks
 *   - harness_005: Specific GLTF component creation
 *   - harness_006: Complete image processing pipeline
 *   - harness_007: Direct STB image loading/writing
 *   - harness_008: 16-bit image loading APIs
 *   - harness_009: GIF loading and zlib decompression
 *   - harness_010: Model equality with buffers and textures
 *   - harness_011: Animation and skinning operations
 *   - harness_012: KHR_audio extension and audio object equality
 *   - harness_013: Texture, Scene, SpotLight equality + cross-loading method comparison
 *   - harness_014: FOCUSED on systematic component modification and comparison testing
 */



// Use same includes as other harnesses
// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_IMPLEMENTATION

// Simple base64 encoding for image/buffer data
static const char base64_chars[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode(const uint8_t* data, size_t len) {
    std::string ret;
    int i = 0;
    uint8_t char_array_3[3];
    uint8_t char_array_4[4];

    while (len--) {
        char_array_3[i++] = *(data++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; i < 4; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(int j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (int j = 0; j < i + 1; j++)
            ret += base64_chars[char_array_4[j]];

        while(i++ < 3)
            ret += '=';
    }

    return ret;
}

// Generate comprehensive glTF JSON with all target components for equality testing
std::string generate_comprehensive_gltf(FuzzedDataProvider& fdp) {
    // Consume parameters from fuzzer input
    std::string texture_name = fdp.ConsumeRandomLengthString(32);
    std::string scene_name = fdp.ConsumeRandomLengthString(32);
    std::string light_name = fdp.ConsumeRandomLengthString(32);
    std::string audio_source_name = fdp.ConsumeRandomLengthString(32);
    std::string audio_emitter_name = fdp.ConsumeRandomLengthString(32);
    
    if (texture_name.empty()) texture_name = "texture";
    if (scene_name.empty()) scene_name = "scene";
    if (light_name.empty()) light_name = "light";
    if (audio_source_name.empty()) audio_source_name = "audio_source";
    if (audio_emitter_name.empty()) audio_emitter_name = "audio_emitter";
    
    // Texture parameters
    int sampler_index = fdp.ConsumeIntegralInRange<int>(-1, 10);
    int source_index = fdp.ConsumeIntegralInRange<int>(-1, 10);
    
    // Scene parameters
    int node_count = fdp.ConsumeIntegralInRange<int>(0, 5);
    std::vector<int> nodes;
    for (int i = 0; i < node_count; i++) {
        nodes.push_back(fdp.ConsumeIntegralInRange<int>(0, 10));
    }
    
    // Light parameters
    std::vector<double> light_color = {
        fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0),
        fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0),
        fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0)
    };
    double light_intensity = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    std::string light_type = fdp.PickValueInArray({"point", "directional", "spot"});
    double light_range = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
    
    // SpotLight parameters
    double spot_inner_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 3.14159);
    double spot_outer_angle = fdp.ConsumeFloatingPointInRange<double>(spot_inner_angle, 3.14159);
    
    // Audio source parameters
    std::string audio_uri = fdp.ConsumeRandomLengthString(64);
    int buffer_view = fdp.ConsumeIntegralInRange<int>(-1, 10);
    std::string mime_type = "audio/mpeg";
    
    // Audio emitter parameters (for PositionalEmitter)
    double audio_gain = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    bool audio_loop = fdp.ConsumeBool();
    bool audio_playing = fdp.ConsumeBool();
    std::string audio_type = fdp.ConsumeBool() ? "positional" : "global";
    std::string distance_model = fdp.PickValueInArray({"inverse", "linear", "exponential"});
    
    // PositionalEmitter parameters
    double cone_inner_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 6.283185307179586);
    double cone_outer_angle = fdp.ConsumeFloatingPointInRange<double>(cone_inner_angle, 6.283185307179586);
    double cone_outer_gain = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double max_distance = fdp.ConsumeFloatingPointInRange<double>(0.0, 1000.0);
    double ref_distance = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
    double rolloff_factor = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    
    // Build comprehensive JSON string
    std::ostringstream json;
    json << "{\n";
    json << "  \"asset\": {\"version\": \"2.0\"},\n";
    
    // Extensions used
    json << "  \"extensionsUsed\": [\"KHR_audio\"],\n";
    
    // Textures
    json << "  \"textures\": [\n";
    json << "    {\n";
    json << "      \"name\": \"" << texture_name << "\"";
    if (sampler_index >= 0) {
        json << ",\n      \"sampler\": " << sampler_index;
    }
    if (source_index >= 0) {
        json << ",\n      \"source\": " << source_index;
    } else {
        json << ",\n      \"source\": -1";
    }
    json << "\n    }\n";
    json << "  ],\n";
    
    // Scenes
    json << "  \"scenes\": [\n";
    json << "    {\n";
    json << "      \"name\": \"" << scene_name << "\"";
    if (!nodes.empty()) {
        json << ",\n      \"nodes\": [";
        for (size_t i = 0; i < nodes.size(); i++) {
            json << nodes[i];
            if (i < nodes.size() - 1) json << ", ";
        }
        json << "]";
    }
    json << "\n    }\n";
    json << "  ],\n";
    
    // Lights
    json << "  \"lights\": [\n";
    json << "    {\n";
    json << "      \"name\": \"" << light_name << "\",\n";
    json << "      \"color\": [" << light_color[0] << ", " << light_color[1] << ", " << light_color[2] << "],\n";
    json << "      \"intensity\": " << light_intensity << ",\n";
    json << "      \"type\": \"" << light_type << "\",\n";
    json << "      \"range\": " << light_range;
    if (light_type == "spot") {
        json << ",\n      \"spot\": {\n";
        json << "        \"innerConeAngle\": " << spot_inner_angle << ",\n";
        json << "        \"outerConeAngle\": " << spot_outer_angle << "\n";
        json << "      }";
    }
    json << "\n    }\n";
    json << "  ],\n";
    
    // KHR_audio extension
    json << "  \"extensions\": {\n";
    json << "    \"KHR_audio\": {\n";
    
    // Audio sources
    json << "      \"audioSources\": [\n";
    json << "        {\n";
    json << "          \"name\": \"" << audio_source_name << "\",\n";
    json << "          \"uri\": \"" << audio_uri << "\"";
    if (buffer_view >= 0) {
        json << ",\n          \"bufferView\": " << buffer_view;
    }
    json << ",\n          \"mimeType\": \"" << mime_type << "\"\n";
    json << "        }\n";
    json << "      ],\n";
    
    // Audio emitters
    json << "      \"audioEmitters\": [\n";
    json << "        {\n";
    json << "          \"name\": \"" << audio_emitter_name << "\",\n";
    json << "          \"gain\": " << audio_gain << ",\n";
    json << "          \"loop\": " << (audio_loop ? "true" : "false") << ",\n";
    json << "          \"playing\": " << (audio_playing ? "true" : "false") << ",\n";
    json << "          \"type\": \"" << audio_type << "\",\n";
    json << "          \"distanceModel\": \"" << distance_model << "\"";
    if (audio_type == "positional") {
        json << ",\n          \"positional\": {\n";
        json << "            \"coneInnerAngle\": " << cone_inner_angle << ",\n";
        json << "            \"coneOuterAngle\": " << cone_outer_angle << ",\n";
        json << "            \"coneOuterGain\": " << cone_outer_gain << ",\n";
        json << "            \"maxDistance\": " << max_distance << ",\n";
        json << "            \"refDistance\": " << ref_distance << ",\n";
        json << "            \"rolloffFactor\": " << rolloff_factor << "\n";
        json << "          }";
    }
    json << ",\n          \"source\": 0\n";
    json << "        }\n";
    json << "      ]\n";
    
    json << "    }\n";
    json << "  },\n";
    
    // Default scene
    json << "  \"scene\": 0\n";
    
    json << "}";
    
    return json.str();
}

// Test systematic component modification and comparison
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for comprehensive testing
    if (size < 200) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Generate comprehensive glTF JSON for testing
    std::string gltf_json = generate_comprehensive_gltf(fdp);
    
    // Choose loading method: ASCII or Binary
    bool use_ascii = fdp.ConsumeBool();
    
    // Create loader instance
    tinygltf::TinyGLTF loader;
    tinygltf::Model base_model;
    std::string err, warn;
    
    bool load_success = false;
    
    if (use_ascii) {
        // Load from JSON string using LoadASCIIFromString
        load_success = loader.LoadASCIIFromString(
            &base_model, &err, &warn,
            gltf_json.c_str(),
            static_cast<unsigned int>(gltf_json.size()),
            /* base_dir */ ""
        );
    } else {
        // Load from binary data using LoadBinaryFromMemory
        load_success = loader.LoadBinaryFromMemory(
            &base_model, &err, &warn,
            reinterpret_cast<const unsigned char*>(gltf_json.c_str()),
            static_cast<unsigned int>(gltf_json.size()),
            /* base_dir */ ""
        );
    }
    
    if (!load_success) {
        // Even if loading fails, we might have exercised some error paths
        return 0;
    }
    
    // Test 1: Model equality with itself (should always be true)
    bool model_self_equal = (base_model == base_model);
    (void)model_self_equal;
    
    // Test 2: Create exact copy and compare (should be true)
    tinygltf::Model model_copy = base_model;
    bool models_equal = (base_model == model_copy);
    (void)models_equal;
    
    // Test 3: Systematic component modification and comparison
    // Choose which component to modify based on fuzzer input
    uint8_t component_to_modify = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Create a fresh copy for modification
    tinygltf::Model modified_model = base_model;
    
    switch (component_to_modify) {
        case 0: // Modify Texture
            if (!modified_model.textures.empty()) {
                tinygltf::Texture& texture = modified_model.textures[0];
                
                // Apply different modifications based on fuzzer input
                uint8_t texture_mod = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (texture_mod) {
                    case 0:
                        texture.name += "_modified";
                        break;
                    case 1:
                        texture.sampler = fdp.ConsumeIntegralInRange<int>(-1, 100);
                        break;
                    case 2:
                        texture.source = fdp.ConsumeIntegralInRange<int>(-1, 100);
                        break;
                    case 3:
                        // Clear extensions
                        texture.extensions.clear();
                        break;
                }
                
                // Test Texture equality
                if (!base_model.textures.empty()) {
                    const tinygltf::Texture& base_texture = base_model.textures[0];
                    bool textures_equal = (base_texture == texture);
                    bool textures_not_equal = !(base_texture == texture);
                    (void)textures_equal;
                    (void)textures_not_equal;
                }
                
                // Test self-equality of modified texture
                bool modified_texture_self_equal = (texture == texture);
                (void)modified_texture_self_equal;
            }
            break;
            
        case 1: // Modify Scene
            if (!modified_model.scenes.empty()) {
                tinygltf::Scene& scene = modified_model.scenes[0];
                
                uint8_t scene_mod = fdp.ConsumeIntegral<uint8_t>() % 3;
                switch (scene_mod) {
                    case 0:
                        scene.name += "_modified";
                        break;
                    case 1:
                        // Add or remove nodes
                        if (!scene.nodes.empty()) {
                            scene.nodes.pop_back();
                        } else {
                            scene.nodes.push_back(fdp.ConsumeIntegral<int>());
                        }
                        break;
                    case 2:
                        // Modify extensions
                        scene.extensions["test_extension"] = tinygltf::Value();
                        break;
                }
                
                // Test Scene equality
                if (!base_model.scenes.empty()) {
                    const tinygltf::Scene& base_scene = base_model.scenes[0];
                    bool scenes_equal = (base_scene == scene);
                    bool scenes_not_equal = !(base_scene == scene);
                    (void)scenes_equal;
                    (void)scenes_not_equal;
                }
            }
            break;
            
        case 2: // Modify Light (and SpotLight)
            if (!modified_model.lights.empty()) {
                tinygltf::Light& light = modified_model.lights[0];
                
                uint8_t light_mod = fdp.ConsumeIntegral<uint8_t>() % 5;
                switch (light_mod) {
                    case 0:
                        light.name += "_modified";
                        break;
                    case 1:
                        light.color[0] = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
                        light.color[1] = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
                        light.color[2] = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
                        break;
                    case 2:
                        light.intensity = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
                        break;
                    case 3:
                        light.type = fdp.PickValueInArray({"point", "directional", "spot"});
                        break;
                    case 4:
                        // Modify SpotLight
                        light.spot.innerConeAngle = fdp.ConsumeFloatingPointInRange<double>(0.0, 3.14159);
                        light.spot.outerConeAngle = fdp.ConsumeFloatingPointInRange<double>(light.spot.innerConeAngle, 3.14159);
                        break;
                }
                
                // Test Light equality
                if (!base_model.lights.empty()) {
                    const tinygltf::Light& base_light = base_model.lights[0];
                    bool lights_equal = (base_light == light);
                    bool lights_not_equal = !(base_light == light);
                    (void)lights_equal;
                    (void)lights_not_equal;
                    
                    // Test SpotLight equality
                    bool spotlights_equal = (base_light.spot == light.spot);
                    (void)spotlights_equal;
                }
            }
            break;
            
        case 3: // Modify AudioSource
            if (!modified_model.audioSources.empty()) {
                tinygltf::AudioSource& audio_source = modified_model.audioSources[0];
                
                uint8_t audio_mod = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (audio_mod) {
                    case 0:
                        audio_source.name += "_modified";
                        break;
                    case 1:
                        audio_source.uri = fdp.ConsumeRandomLengthString(128);
                        break;
                    case 2:
                        audio_source.bufferView = fdp.ConsumeIntegralInRange<int>(-1, 100);
                        break;
                    case 3:
                        audio_source.mimeType = fdp.PickValueInArray({"audio/mpeg", "audio/wav", "audio/ogg"});
                        break;
                }
                
                // Test AudioSource equality
                if (!base_model.audioSources.empty()) {
                    const tinygltf::AudioSource& base_audio_source = base_model.audioSources[0];
                    bool audio_sources_equal = (base_audio_source == audio_source);
                    bool audio_sources_not_equal = !(base_audio_source == audio_source);
                    (void)audio_sources_equal;
                    (void)audio_sources_not_equal;
                }
            }
            break;
            
        case 4: // Modify PositionalEmitter (via AudioEmitter)
            if (!modified_model.audioEmitters.empty()) {
                tinygltf::AudioEmitter& audio_emitter = modified_model.audioEmitters[0];
                
                // Only modify if it's a positional emitter
                if (audio_emitter.type == "positional") {
                    uint8_t positional_mod = fdp.ConsumeIntegral<uint8_t>() % 6;
                    switch (positional_mod) {
                        case 0:
                            audio_emitter.positional.coneInnerAngle = fdp.ConsumeFloatingPointInRange<double>(0.0, 6.283185307179586);
                            break;
                        case 1:
                            audio_emitter.positional.coneOuterAngle = fdp.ConsumeFloatingPointInRange<double>(
                                audio_emitter.positional.coneInnerAngle, 6.283185307179586);
                            break;
                        case 2:
                            audio_emitter.positional.coneOuterGain = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
                            break;
                        case 3:
                            audio_emitter.positional.maxDistance = fdp.ConsumeFloatingPointInRange<double>(0.0, 10000.0);
                            break;
                        case 4:
                            audio_emitter.positional.refDistance = fdp.ConsumeFloatingPointInRange<double>(0.0, 1000.0);
                            break;
                        case 5:
                            audio_emitter.positional.rolloffFactor = fdp.ConsumeFloatingPointInRange<double>(0.0, 20.0);
                            break;
                    }
                    
                    // Test PositionalEmitter equality
                    if (!base_model.audioEmitters.empty() && base_model.audioEmitters[0].type == "positional") {
                        const tinygltf::PositionalEmitter& base_positional = base_model.audioEmitters[0].positional;
                        bool positionals_equal = (base_positional == audio_emitter.positional);
                        bool positionals_not_equal = !(base_positional == audio_emitter.positional);
                        (void)positionals_equal;
                        (void)positionals_not_equal;
                    }
                }
            }
            break;
            
        case 5: // Modify multiple components
            {
                // Modify texture if exists
                if (!modified_model.textures.empty()) {
                    modified_model.textures[0].name += "_multi_mod";
                }
                
                // Modify scene if exists
                if (!modified_model.scenes.empty()) {
                    modified_model.scenes[0].name += "_multi_mod";
                }
                
                // Modify light if exists
                if (!modified_model.lights.empty()) {
                    modified_model.lights[0].intensity *= 2.0;
                }
                
                // Test overall model equality after multiple modifications
                bool models_equal_after_multi_mod = (base_model == modified_model);
                (void)models_equal_after_multi_mod;
            }
            break;
    }
    
    // After modifications, test full model equality
    bool models_equal_after_mod = (base_model == modified_model);
    (void)models_equal_after_mod;
    
    // Test 4: Edge cases - empty/default components
    // Test with empty textures if they exist
    if (!base_model.textures.empty()) {
        tinygltf::Texture empty_texture;
        const tinygltf::Texture& base_texture = base_model.textures[0];
        bool empty_vs_base = (empty_texture == base_texture);
        bool base_vs_empty = (base_texture == empty_texture);
        bool empty_self = (empty_texture == empty_texture);
        (void)empty_vs_base;
        (void)base_vs_empty;
        (void)empty_self;
    }
    
    // Test with empty scenes if they exist
    if (!base_model.scenes.empty()) {
        tinygltf::Scene empty_scene;
        const tinygltf::Scene& base_scene = base_model.scenes[0];
        bool empty_vs_base = (empty_scene == base_scene);
        bool base_vs_empty = (base_scene == empty_scene);
        bool empty_self = (empty_scene == empty_scene);
        (void)empty_vs_base;
        (void)base_vs_empty;
        (void)empty_self;
    }
    
    // Test with empty lights if they exist
    if (!base_model.lights.empty()) {
        tinygltf::Light empty_light;
        const tinygltf::Light& base_light = base_model.lights[0];
        bool empty_vs_base = (empty_light == base_light);
        bool base_vs_empty = (base_light == empty_light);
        bool empty_self = (empty_light == empty_light);
        (void)empty_vs_base;
        (void)base_vs_empty;
        (void)empty_self;
        
        // Test SpotLight edge cases
        tinygltf::SpotLight empty_spotlight;
        bool empty_spot_self = (empty_spotlight == empty_spotlight);
        (void)empty_spot_self;
    }
    
    // Test with empty audio sources if they exist
    if (!base_model.audioSources.empty()) {
        tinygltf::AudioSource empty_audio_source;
        const tinygltf::AudioSource& base_audio_source = base_model.audioSources[0];
        bool empty_vs_base = (empty_audio_source == base_audio_source);
        bool base_vs_empty = (base_audio_source == empty_audio_source);
        bool empty_self = (empty_audio_source == empty_audio_source);
        (void)empty_vs_base;
        (void)base_vs_empty;
        (void)empty_self;
    }
    
    // Test with empty positional emitter
    tinygltf::PositionalEmitter empty_positional;
    bool empty_positional_self = (empty_positional == empty_positional);
    (void)empty_positional_self;
    
    return 0;
}

} // namespace harness_014
