#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_012 {
/*
 * Fuzzing harness for tinygltf library targeting glTF audio extension (KHR_audio) functionality
 * Goal: Exercise equality operators for audio-related objects in the KHR_audio extension
 * Primary Targets:
 *   - PositionalEmitter equality operator (_ZNK8tinygltf17PositionalEmittereqERKS0_): 20 branches, 0% coverage
 *   - AudioEmitter equality operator (_ZNK8tinygltf12AudioEmittereqERKS0_): 14 branches, 50% coverage (7 undiscovered)
 *   - AudioSource equality operator (_ZNK8tinygltf11AudioSourceeqERKS0_): 4 branches, 0% coverage
 *   - Light equality operator (_ZNK8tinygltf5LighteqERKS0_): 6 branches, 0% coverage (related to audio context)
 * Required Helpers:
 *   - TinyGLTF::LoadASCIIFromString (parsing glTF with audio extensions)
 *   - TinyGLTF::LoadBinaryFromMemory (alternative parsing method)
 * Strategy: Create glTF JSON containing KHR_audio extension data, parse it, extract audio objects,
 *           perform equality comparisons, and test edge cases with modified copies
 * Differentiation from existing harnesses (harness_000 to harness_011):
 *   - harness_000: Basic GLTF parsing (no extension-specific focus)
 *   - harness_001: Image loading via GLTF embedded images (stb_image focus)
 *   - harness_002: Image writing (stb_image_write focus)
 *   - harness_003-011: Various other APIs (JSON, buffer, mesh, etc.)
 *   - harness_012: FIRST harness targeting KHR_audio extension and audio object equality operators
 */



// #define TINYGLTF_IMPLEMENTATION

// Generate glTF JSON with KHR_audio extension data
std::string generate_gltf_with_audio_extension(FuzzedDataProvider& fdp) {
    // Consume parameters for audio objects from fuzzer input
    std::string audio_name = fdp.ConsumeRandomLengthString(32);
    double audio_gain = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    bool audio_loop = fdp.ConsumeBool();
    bool audio_playing = fdp.ConsumeBool();
    
    // Audio type: "positional" or "global"
    std::string audio_type = fdp.ConsumeBool() ? "positional" : "global";
    
    // Distance model: "linear", "inverse", or "exponential"
    uint8_t dist_model_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
    std::string distance_model;
    switch(dist_model_selector) {
        case 0: distance_model = "linear"; break;
        case 1: distance_model = "inverse"; break;
        case 2: distance_model = "exponential"; break;
    }
    
    // Positional emitter parameters
    double cone_inner_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 6.283185307179586);
    double cone_outer_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 6.283185307179586);
    double cone_outer_gain = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double max_distance = fdp.ConsumeFloatingPointInRange<double>(0.0, 1000.0);
    double ref_distance = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
    double rolloff_factor = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    
    // Audio source parameters
    std::string source_name = fdp.ConsumeRandomLengthString(32);
    std::string source_uri = fdp.ConsumeRandomLengthString(64);
    int buffer_view = fdp.ConsumeIntegralInRange<int>(-1, 100);
    std::string mime_type = "audio/mpeg";
    
    // Light parameters (optional, related context)
    std::string light_name = fdp.ConsumeRandomLengthString(32);
    double light_intensity = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    std::string light_type = fdp.ConsumeBool() ? "point" : "directional";
    
    char buffer[4096];
    snprintf(buffer, sizeof(buffer),
        "{\n"
        "  \"asset\": {\"version\": \"2.0\"},\n"
        "  \"extensionsUsed\": [\"KHR_audio\"],\n"
        "  \"extensions\": {\n"
        "    \"KHR_audio\": {\n"
        "      \"audioSources\": [\n"
        "        {\n"
        "          \"name\": \"%s\",\n"
        "          \"uri\": \"%s\",\n"
        "          \"bufferView\": %d,\n"
        "          \"mimeType\": \"%s\"\n"
        "        }\n"
        "      ],\n"
        "      \"audioEmitters\": [\n"
        "        {\n"
        "          \"name\": \"%s\",\n"
        "          \"gain\": %f,\n"
        "          \"loop\": %s,\n"
        "          \"playing\": %s,\n"
        "          \"type\": \"%s\",\n"
        "          \"distanceModel\": \"%s\",\n"
        "          \"positional\": {\n"
        "            \"coneInnerAngle\": %f,\n"
        "            \"coneOuterAngle\": %f,\n"
        "            \"coneOuterGain\": %f,\n"
        "            \"maxDistance\": %f,\n"
        "            \"refDistance\": %f,\n"
        "            \"rolloffFactor\": %f\n"
        "          },\n"
        "          \"source\": 0\n"
        "        }\n"
        "      ]\n"
        "    }\n"
        "  },\n"
        "  \"nodes\": [\n"
        "    {\n"
        "      \"name\": \"node_with_audio\",\n"
        "      \"extensions\": {\n"
        "        \"KHR_audio\": {\n"
        "          \"emitter\": 0\n"
        "        }\n"
        "      }\n"
        "    }\n"
        "  ],\n"
        "  \"scenes\": [\n"
        "    {\n"
        "      \"nodes\": [0],\n"
        "      \"extensions\": {\n"
        "        \"KHR_audio\": {\n"
        "          \"audioEmitters\": [0]\n"
        "        }\n"
        "      }\n"
        "    }\n"
        "  ],\n"
        "  \"lights\": [\n"
        "    {\n"
        "      \"name\": \"%s\",\n"
        "      \"intensity\": %f,\n"
        "      \"type\": \"%s\"\n"
        "    }\n"
        "  ]\n"
        "}",
        source_name.c_str(), source_uri.c_str(), buffer_view, mime_type.c_str(),
        audio_name.c_str(), audio_gain, audio_loop ? "true" : "false", 
        audio_playing ? "true" : "false", audio_type.c_str(), distance_model.c_str(),
        cone_inner_angle, cone_outer_angle, cone_outer_gain, max_distance, 
        ref_distance, rolloff_factor,
        light_name.c_str(), light_intensity, light_type.c_str());
    
    return std::string(buffer);
}

// Test equality operators for audio-related objects
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed: we need enough for FuzzedDataProvider to work
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Generate glTF JSON with audio extension
    std::string gltf_json = generate_gltf_with_audio_extension(fdp);
    
    // Test ASCII parsing
    {
        tinygltf::Model model1;
        tinygltf::TinyGLTF ctx;
        std::string err1, warn1;
        
        bool ret1 = ctx.LoadASCIIFromString(&model1, &err1, &warn1, 
                                           gltf_json.c_str(), 
                                           static_cast<unsigned int>(gltf_json.size()),
                                           /* base_dir */ "");
        
        // If parsing succeeded, test equality operators
        if (ret1 && !model1.audioEmitters.empty() && !model1.audioSources.empty()) {
            // Test AudioEmitter equality
            const tinygltf::AudioEmitter& emitter1 = model1.audioEmitters[0];
            tinygltf::AudioEmitter emitter_copy = emitter1; // Copy constructor
            bool emitter_eq = (emitter1 == emitter_copy);
            (void)emitter_eq;
            
            // Test AudioSource equality  
            const tinygltf::AudioSource& source1 = model1.audioSources[0];
            tinygltf::AudioSource source_copy = source1;
            bool source_eq = (source1 == source_copy);
            (void)source_eq;
            
            // Test PositionalEmitter equality (via AudioEmitter)
            const tinygltf::PositionalEmitter& positional1 = emitter1.positional;
            tinygltf::PositionalEmitter positional_copy = positional1;
            bool positional_eq = (positional1 == positional_copy);
            (void)positional_eq;
            
            // Test Light equality if present
            if (!model1.lights.empty()) {
                const tinygltf::Light& light1 = model1.lights[0];
                tinygltf::Light light_copy = light1;
                bool light_eq = (light1 == light_copy);
                (void)light_eq;
            }
            
            // Test inequality cases by creating modified copies
            if (!model1.audioEmitters.empty()) {
                tinygltf::AudioEmitter modified_emitter = emitter1;
                modified_emitter.gain += 0.1; // Modify to create inequality
                bool emitter_neq = !(emitter1 == modified_emitter);
                (void)emitter_neq;
            }
            
            if (!model1.audioSources.empty()) {
                tinygltf::AudioSource modified_source = source1;
                modified_source.name += "_modified"; // Modify to create inequality
                bool source_neq = !(source1 == modified_source);
                (void)source_neq;
            }
        }
    }
    
    // Also test binary parsing with the same data
    // Convert JSON to binary format (simplified - just use the string as binary)
    {
        tinygltf::Model model2;
        tinygltf::TinyGLTF ctx2;
        std::string err2, warn2;
        
        bool ret2 = ctx2.LoadBinaryFromMemory(&model2, &err2, &warn2,
                                             reinterpret_cast<const unsigned char*>(gltf_json.c_str()),
                                             static_cast<unsigned int>(gltf_json.size()),
                                             /* base_dir */ "");
        
        // Test equality operators on binary-parsed model if successful
        if (ret2 && !model2.audioEmitters.empty() && !model2.audioSources.empty()) {
            // Similar equality tests as above
            const tinygltf::AudioEmitter& emitter2 = model2.audioEmitters[0];
            const tinygltf::AudioSource& source2 = model2.audioSources[0];
            
            tinygltf::AudioEmitter emitter2_copy = emitter2;
            bool emitter2_eq = (emitter2 == emitter2_copy);
            (void)emitter2_eq;
            
            tinygltf::AudioSource source2_copy = source2;
            bool source2_eq = (source2 == source2_copy);
            (void)source2_eq;
            
            // Compare objects from ASCII and binary parsing (they should be equal if parsing succeeded)
            // Note: This is an interesting edge case for the fuzzer to explore
        }
    }
    
    // Test with minimal valid JSON (edge case)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 32) {
        std::string minimal_json = "{\"asset\":{\"version\":\"2.0\"}}";
        tinygltf::Model model3;
        tinygltf::TinyGLTF ctx3;
        std::string err3, warn3;
        
        bool ret3 = ctx3.LoadASCIIFromString(&model3, &err3, &warn3,
                                            minimal_json.c_str(),
                                            static_cast<unsigned int>(minimal_json.size()),
                                            /* base_dir */ "");
        (void)ret3;
    }
    
    return 0;
}

} // namespace harness_012
