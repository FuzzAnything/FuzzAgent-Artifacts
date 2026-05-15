#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_013 {
/*
 * Fuzzing harness for tinygltf library targeting glTF object comparison operations
 * Goal: Exercise equality operators for Texture, Scene, SpotLight, PositionalEmitter, Light, and AudioSource objects
 * Primary Targets:
 *   - _ZNK8tinygltf7TextureeqERKS0_ (Texture equality operator: 14 undiscovered branches)
 *   - _ZNK8tinygltf5SceneeqERKS0_ (Scene equality operator: 12 undiscovered branches)
 *   - _ZNK8tinygltf9SpotLighteqERKS0_ (SpotLight equality operator: 12 undiscovered branches)
 *   - _ZNK8tinygltf17PositionalEmittereqERKS0_ (PositionalEmitter equality operator: 20 undiscovered branches)
 *   - _ZNK8tinygltf5LighteqERKS0_ (Light equality operator: 6 undiscovered branches)
 *   - _ZNK8tinygltf11AudioSourceeqERKS0_ (AudioSource equality operator: 4 undiscovered branches)
 * Required Setup: Load glTF models from both JSON string and binary data using TinyGLTF::LoadFromString and LoadBinaryFromMemory
 * Strategy: Create glTF JSON containing textures, scenes, lights, and audio extensions, parse via both loading methods,
 *           compare corresponding objects between loading methods and test equality/inequality cases
 * Differentiation from existing harnesses (harness_000 to harness_012):
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
 *   - harness_012: KHR_audio extension and audio object equality (PositionalEmitter, AudioEmitter, AudioSource, Light)
 *   - harness_013: FOCUSED on Texture, Scene, SpotLight equality + cross-loading method comparison
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

// Generate glTF JSON with textures, scenes, lights, and audio extensions for comprehensive equality testing
std::string generate_gltf_for_comparison(FuzzedDataProvider& fdp) {
    // Consume parameters from fuzzer input
    std::string texture_name = fdp.ConsumeRandomLengthString(32);
    std::string scene_name = fdp.ConsumeRandomLengthString(32);
    std::string light_name = fdp.ConsumeRandomLengthString(32);
    std::string audio_source_name = fdp.ConsumeRandomLengthString(32);
    
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
    std::string light_type = fdp.ConsumeBool() ? "point" : "directional";
    double light_range = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
    
    // SpotLight parameters
    double spot_inner_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 3.14159);
    double spot_outer_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 3.14159);
    
    // Audio source parameters
    std::string audio_uri = fdp.ConsumeRandomLengthString(64);
    int buffer_view = fdp.ConsumeIntegralInRange<int>(-1, 10);
    std::string mime_type = "audio/mpeg";
    
    // Audio emitter parameters (for PositionalEmitter)
    std::string audio_emitter_name = fdp.ConsumeRandomLengthString(32);
    double audio_gain = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    bool audio_loop = fdp.ConsumeBool();
    bool audio_playing = fdp.ConsumeBool();
    std::string audio_type = fdp.ConsumeBool() ? "positional" : "global";
    std::string distance_model = "inverse";
    
    // PositionalEmitter parameters
    double cone_inner_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 6.283185307179586);
    double cone_outer_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 6.283185307179586);
    double cone_outer_gain = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double max_distance = fdp.ConsumeFloatingPointInRange<double>(0.0, 1000.0);
    double ref_distance = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
    double rolloff_factor = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    
    // Build JSON string
    std::ostringstream json;
    json << "{\n";
    json << "  \"asset\": {\"version\": \"2.0\"},\n";
    
    // Extensions used
    json << "  \"extensionsUsed\": [\"KHR_audio\"],\n";
    
    // Textures
    json << "  \"textures\": [\n";
    json << "    {\n";
    json << "      \"name\": \"" << texture_name << "\",\n";
    if (sampler_index >= 0) {
        json << "      \"sampler\": " << sampler_index << ",\n";
    }
    if (source_index >= 0) {
        json << "      \"source\": " << source_index << "\n";
    } else {
        json << "      \"source\": -1\n";
    }
    json << "    }\n";
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
    json << "      \"range\": " << light_range << ",\n";
    json << "      \"spot\": {\n";
    json << "        \"innerConeAngle\": " << spot_inner_angle << ",\n";
    json << "        \"outerConeAngle\": " << spot_outer_angle << "\n";
    json << "      }\n";
    json << "    }\n";
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
    json << "          \"distanceModel\": \"" << distance_model << "\",\n";
    json << "          \"positional\": {\n";
    json << "            \"coneInnerAngle\": " << cone_inner_angle << ",\n";
    json << "            \"coneOuterAngle\": " << cone_outer_angle << ",\n";
    json << "            \"coneOuterGain\": " << cone_outer_gain << ",\n";
    json << "            \"maxDistance\": " << max_distance << ",\n";
    json << "            \"refDistance\": " << ref_distance << ",\n";
    json << "            \"rolloffFactor\": " << rolloff_factor << "\n";
    json << "          },\n";
    json << "          \"source\": 0\n";
    json << "        }\n";
    json << "      ]\n";
    
    json << "    }\n";
    json << "  },\n";
    
    // Default scene
    json << "  \"scene\": 0\n";
    
    json << "}";
    
    return json.str();
}

// Test equality operators across different loading methods
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Generate glTF JSON for testing
    std::string gltf_json = generate_gltf_for_comparison(fdp);
    
    // Create loader instances
    tinygltf::TinyGLTF loader1;
    tinygltf::TinyGLTF loader2;
    
    // Test 1: Load from JSON string using LoadASCIIFromString
    tinygltf::Model model_from_string;
    std::string err1, warn1;
    
    bool load_from_string_success = loader1.LoadASCIIFromString(
        &model_from_string, &err1, &warn1,
        gltf_json.c_str(), 
        static_cast<unsigned int>(gltf_json.size()),
        /* base_dir */ ""
    );
    
    // Test 2: Load from binary data using LoadBinaryFromMemory (treat JSON as binary)
    tinygltf::Model model_from_binary;
    std::string err2, warn2;
    
    bool load_binary_success = loader2.LoadBinaryFromMemory(
        &model_from_binary, &err2, &warn2,
        reinterpret_cast<const unsigned char*>(gltf_json.c_str()),
        static_cast<unsigned int>(gltf_json.size()),
        /* base_dir */ ""
    );
    
    // If either loading method succeeded, test equality operators
    if (load_from_string_success || load_binary_success) {
        // Test Texture equality if textures exist
        if (!model_from_string.textures.empty()) {
            const tinygltf::Texture& texture1 = model_from_string.textures[0];
            tinygltf::Texture texture_copy = texture1;
            bool texture_eq = (texture1 == texture_copy);
            (void)texture_eq;
            
            // Test inequality by creating modified copy
            tinygltf::Texture modified_texture = texture1;
            modified_texture.name += "_modified";
            bool texture_neq = !(texture1 == modified_texture);
            (void)texture_neq;
        }
        
        // Test Scene equality if scenes exist
        if (!model_from_string.scenes.empty()) {
            const tinygltf::Scene& scene1 = model_from_string.scenes[0];
            tinygltf::Scene scene_copy = scene1;
            bool scene_eq = (scene1 == scene_copy);
            (void)scene_eq;
            
            // Test inequality by creating modified copy
            tinygltf::Scene modified_scene = scene1;
            modified_scene.name += "_modified";
            bool scene_neq = !(scene1 == modified_scene);
            (void)scene_neq;
        }
        
        // Test Light equality if lights exist
        if (!model_from_string.lights.empty()) {
            const tinygltf::Light& light1 = model_from_string.lights[0];
            tinygltf::Light light_copy = light1;
            bool light_eq = (light1 == light_copy);
            (void)light_eq;
            
            // Test SpotLight equality (via Light.spot)
            const tinygltf::SpotLight& spotlight1 = light1.spot;
            tinygltf::SpotLight spotlight_copy = spotlight1;
            bool spotlight_eq = (spotlight1 == spotlight_copy);
            (void)spotlight_eq;
            
            // Test inequality by creating modified copy
            tinygltf::Light modified_light = light1;
            modified_light.intensity += 0.5;
            bool light_neq = !(light1 == modified_light);
            (void)light_neq;
        }
        
        // Test AudioSource equality if audio sources exist
        if (!model_from_string.audioSources.empty()) {
            const tinygltf::AudioSource& audio_source1 = model_from_string.audioSources[0];
            tinygltf::AudioSource audio_source_copy = audio_source1;
            bool audio_source_eq = (audio_source1 == audio_source_copy);
            (void)audio_source_eq;
            
            // Test inequality by creating modified copy
            tinygltf::AudioSource modified_audio_source = audio_source1;
            modified_audio_source.name += "_modified";
            bool audio_source_neq = !(audio_source1 == modified_audio_source);
            (void)audio_source_neq;
        }
        
        // Test PositionalEmitter equality if audio emitters exist
        if (!model_from_string.audioEmitters.empty()) {
            const tinygltf::AudioEmitter& audio_emitter1 = model_from_string.audioEmitters[0];
            const tinygltf::PositionalEmitter& positional1 = audio_emitter1.positional;
            tinygltf::PositionalEmitter positional_copy = positional1;
            bool positional_eq = (positional1 == positional_copy);
            (void)positional_eq;
            
            // Test inequality by creating modified copy
            tinygltf::PositionalEmitter modified_positional = positional1;
            modified_positional.coneInnerAngle += 0.1;
            bool positional_neq = !(positional1 == modified_positional);
            (void)positional_neq;
        }
        
        // Cross-loading method comparisons
        if (load_from_string_success && load_binary_success) {
            // Compare objects loaded via different methods
            if (!model_from_string.textures.empty() && !model_from_binary.textures.empty()) {
                const tinygltf::Texture& texture_string = model_from_string.textures[0];
                const tinygltf::Texture& texture_binary = model_from_binary.textures[0];
                bool cross_texture_eq = (texture_string == texture_binary);
                (void)cross_texture_eq;
            }
            
            if (!model_from_string.scenes.empty() && !model_from_binary.scenes.empty()) {
                const tinygltf::Scene& scene_string = model_from_string.scenes[0];
                const tinygltf::Scene& scene_binary = model_from_binary.scenes[0];
                bool cross_scene_eq = (scene_string == scene_binary);
                (void)cross_scene_eq;
            }
            
            if (!model_from_string.lights.empty() && !model_from_binary.lights.empty()) {
                const tinygltf::Light& light_string = model_from_string.lights[0];
                const tinygltf::Light& light_binary = model_from_binary.lights[0];
                bool cross_light_eq = (light_string == light_binary);
                (void)cross_light_eq;
                
                // Compare SpotLight
                bool cross_spotlight_eq = (light_string.spot == light_binary.spot);
                (void)cross_spotlight_eq;
            }
            
            if (!model_from_string.audioSources.empty() && !model_from_binary.audioSources.empty()) {
                const tinygltf::AudioSource& audio_source_string = model_from_string.audioSources[0];
                const tinygltf::AudioSource& audio_source_binary = model_from_binary.audioSources[0];
                bool cross_audio_source_eq = (audio_source_string == audio_source_binary);
                (void)cross_audio_source_eq;
            }
        }
    }
    
    return 0;
}

} // namespace harness_013
