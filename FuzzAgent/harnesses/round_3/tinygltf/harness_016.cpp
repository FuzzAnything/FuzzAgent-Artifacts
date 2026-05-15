#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_016 {
/*
 * Fuzzing harness for tinygltf library targeting glTF struct equality operators via randomized property modification
 * Goal: Exercise equality operators for PositionalEmitter, Texture, SpotLight, Scene, Light, and AudioSource objects
 *       through systematic random modification of individual properties to test all comparison code paths
 * Primary Targets:
 *   - _ZNK8tinygltf17PositionalEmittereqERKS0_ (PositionalEmitter equality operator: 20 undiscovered branches)
 *   - _ZNK8tinygltf7TextureeqERKS0_ (Texture equality operator: 14 undiscovered branches)
 *   - _ZNK8tinygltf9SpotLighteqERKS0_ (SpotLight equality operator: 12 undiscovered branches)
 *   - _ZNK8tinygltf5SceneeqERKS0_ (Scene equality operator: 12 undiscovered branches)
 *   - _ZNK8tinygltf5LighteqERKS0_ (Light equality operator: 6 undiscovered branches)
 *   - _ZNK8tinygltf11AudioSourceeqERKS0_ (AudioSource equality operator: 4 undiscovered branches)
 * Required Setup: Load glTF data containing audio emitters, textures, lights, scenes using TinyGLTF::LoadASCIIFromString/LoadBinaryFromMemory
 * Strategy: Load base glTF model, create copies, randomly modify specific properties of each target struct,
 *           test equality/inequality after each modification to exercise different comparison code paths
 * Differentiation from existing harnesses (harness_000 to harness_015):
 *   - harness_012: KHR_audio extension and audio object equality
 *   - harness_013: Texture, Scene, SpotLight equality + cross-loading method comparison
 *   - harness_014: Systematic component modification and comparison testing
 *   - harness_015: File-based loading (LoadASCIIFromFile, LoadBinaryFromFile) for equality testing
 *   - harness_016: FOCUSED on randomized property modification to test all fields of equality operators individually
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

// Generate comprehensive glTF JSON with all target components for property modification testing
std::string generate_gltf_for_property_modification(FuzzedDataProvider& fdp) {
    // Consume parameters from fuzzer input
    std::string texture_name = fdp.ConsumeRandomLengthString(32);
    std::string scene_name = fdp.ConsumeRandomLengthString(32);
    std::string light_name = fdp.ConsumeRandomLengthString(32);
    std::string audio_source_name = fdp.ConsumeRandomLengthString(32);
    std::string audio_emitter_name = fdp.ConsumeRandomLengthString(32);
    
    // Texture parameters
    int sampler_index = fdp.ConsumeIntegralInRange<int>(-1, 10);
    int source_index = fdp.ConsumeIntegralInRange<int>(-1, 10);
    
    // Scene parameters
    int scene_node_count = fdp.ConsumeIntegralInRange<int>(0, 5);
    std::vector<int> scene_nodes;
    for (int i = 0; i < scene_node_count; i++) {
        scene_nodes.push_back(fdp.ConsumeIntegralInRange<int>(0, 10));
    }
    
    // Light parameters
    std::string light_type = fdp.ConsumeBool() ? "point" : "directional";
    double light_intensity = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    std::array<double, 3> light_color = {
        fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0),
        fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0),
        fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0)
    };
    
    // SpotLight parameters
    double spot_inner_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 3.14159);
    double spot_outer_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 3.14159);
    
    // AudioSource parameters
    std::string audio_source_uri = fdp.ConsumeRandomLengthString(64);
    int audio_source_buffer_view = fdp.ConsumeIntegralInRange<int>(-1, 10);
    
    // AudioEmitter parameters with PositionalEmitter
    double audio_gain = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    bool audio_loop = fdp.ConsumeBool();
    bool audio_playing = fdp.ConsumeBool();
    std::string audio_type = fdp.ConsumeBool() ? "positional" : "global";
    std::string distance_model = fdp.ConsumeBool() ? "linear" : "inverse";
    
    // PositionalEmitter parameters
    double cone_inner_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 6.283185307179586);
    double cone_outer_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 6.283185307179586);
    double cone_outer_gain = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double max_distance = fdp.ConsumeFloatingPointInRange<double>(0.0, 1000.0);
    double ref_distance = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
    double rolloff_factor = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    
    // Build scene nodes list JSON
    std::string scene_nodes_json;
    if (!scene_nodes.empty()) {
        scene_nodes_json = "\"nodes\": [";
        for (size_t i = 0; i < scene_nodes.size(); i++) {
            scene_nodes_json += std::to_string(scene_nodes[i]);
            if (i < scene_nodes.size() - 1) {
                scene_nodes_json += ", ";
            }
        }
        scene_nodes_json += "]";
    }
    
    // Build light color array JSON
    std::string light_color_json = "[" + 
        std::to_string(light_color[0]) + ", " +
        std::to_string(light_color[1]) + ", " +
        std::to_string(light_color[2]) + "]";
    
    char buffer[4096];
    snprintf(buffer, sizeof(buffer),
        "{\n"
        "  \"asset\": {\"version\": \"2.0\"},\n"
        "  \"extensionsUsed\": [\"KHR_lights_punctual\", \"KHR_audio\"],\n"
        "  \"extensions\": {\n"
        "    \"KHR_lights_punctual\": {\n"
        "      \"lights\": [\n"
        "        {\n"
        "          \"name\": \"%s\",\n"
        "          \"type\": \"%s\",\n"
        "          \"intensity\": %f,\n"
        "          \"color\": %s,\n"
        "          \"spot\": {\n"
        "            \"innerConeAngle\": %f,\n"
        "            \"outerConeAngle\": %f\n"
        "          }\n"
        "        }\n"
        "      ]\n"
        "    },\n"
        "    \"KHR_audio\": {\n"
        "      \"audioSources\": [\n"
        "        {\n"
        "          \"name\": \"%s\",\n"
        "          \"uri\": \"%s\",\n"
        "          \"bufferView\": %d\n"
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
        "          }\n"
        "        }\n"
        "      ]\n"
        "    }\n"
        "  },\n"
        "  \"textures\": [\n"
        "    {\n"
        "      \"name\": \"%s\",\n"
        "      \"sampler\": %d,\n"
        "      \"source\": %d\n"
        "    }\n"
        "  ],\n"
        "  \"scenes\": [\n"
        "    {\n"
        "      \"name\": \"%s\"%s%s\n"
        "    }\n"
        "  ]\n"
        "}",
        light_name.c_str(), light_type.c_str(), light_intensity, light_color_json.c_str(),
        spot_inner_angle, spot_outer_angle,
        audio_source_name.c_str(), audio_source_uri.c_str(), audio_source_buffer_view,
        audio_emitter_name.c_str(), audio_gain, audio_loop ? "true" : "false",
        audio_playing ? "true" : "false", audio_type.c_str(), distance_model.c_str(),
        cone_inner_angle, cone_outer_angle, cone_outer_gain,
        max_distance, ref_distance, rolloff_factor,
        texture_name.c_str(), sampler_index, source_index,
        scene_name.c_str(), scene_nodes_json.empty() ? "" : ", ", scene_nodes_json.c_str()
    );
    
    return std::string(buffer);
}

// Function to test PositionalEmitter equality with random property modifications
void test_positional_emitter_equality(FuzzedDataProvider& fdp, const tinygltf::PositionalEmitter& base) {
    // Create multiple copies with different modifications
    const int num_tests = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < num_tests; i++) {
        if (fdp.remaining_bytes() < 10) break;
        
        tinygltf::PositionalEmitter modified = base;
        
        // Choose which property to modify
        uint8_t property_to_modify = fdp.ConsumeIntegral<uint8_t>() % 7;
        
        switch (property_to_modify) {
            case 0:
                modified.coneInnerAngle = fdp.ConsumeFloatingPoint<double>();
                break;
            case 1:
                modified.coneOuterAngle = fdp.ConsumeFloatingPoint<double>();
                break;
            case 2:
                modified.coneOuterGain = fdp.ConsumeFloatingPoint<double>();
                break;
            case 3:
                modified.maxDistance = fdp.ConsumeFloatingPoint<double>();
                break;
            case 4:
                modified.refDistance = fdp.ConsumeFloatingPoint<double>();
                break;
            case 5:
                modified.rolloffFactor = fdp.ConsumeFloatingPoint<double>();
                break;
            case 6:
                // Modify extensions
                modified.extensions["test_mod"] = tinygltf::Value(fdp.ConsumeIntegral<int>());
                break;
        }
        
        // Test equality
        bool equal_result = (base == modified);
        bool self_equal = (modified == modified);
        (void)equal_result;
        (void)self_equal;
        
        // Test with exact copy
        tinygltf::PositionalEmitter exact_copy = modified;
        bool exact_equal = (modified == exact_copy);
        (void)exact_equal;
    }
}

// Function to test Texture equality with random property modifications
void test_texture_equality(FuzzedDataProvider& fdp, const tinygltf::Texture& base) {
    const int num_tests = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < num_tests; i++) {
        if (fdp.remaining_bytes() < 10) break;
        
        tinygltf::Texture modified = base;
        
        uint8_t property_to_modify = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (property_to_modify) {
            case 0:
                modified.name += "_modified_" + std::to_string(i);
                break;
            case 1:
                modified.sampler = fdp.ConsumeIntegral<int>();
                break;
            case 2:
                modified.source = fdp.ConsumeIntegral<int>();
                break;
            case 3:
                modified.extensions["test_mod"] = tinygltf::Value(fdp.ConsumeRandomLengthString(16));
                break;
        }
        
        // Test equality
        bool equal_result = (base == modified);
        bool self_equal = (modified == modified);
        (void)equal_result;
        (void)self_equal;
    }
}

// Function to test SpotLight equality with random property modifications
void test_spotlight_equality(FuzzedDataProvider& fdp, const tinygltf::SpotLight& base) {
    const int num_tests = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < num_tests; i++) {
        if (fdp.remaining_bytes() < 10) break;
        
        tinygltf::SpotLight modified = base;
        
        uint8_t property_to_modify = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (property_to_modify) {
            case 0:
                modified.innerConeAngle = fdp.ConsumeFloatingPoint<double>();
                break;
            case 1:
                modified.outerConeAngle = fdp.ConsumeFloatingPoint<double>();
                break;
            case 2:
                modified.extensions["test_mod"] = tinygltf::Value(fdp.ConsumeIntegral<int>());
                break;
        }
        
        // Test equality
        bool equal_result = (base == modified);
        bool self_equal = (modified == modified);
        (void)equal_result;
        (void)self_equal;
    }
}

// Function to test Scene equality with random property modifications
void test_scene_equality(FuzzedDataProvider& fdp, const tinygltf::Scene& base) {
    const int num_tests = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < num_tests; i++) {
        if (fdp.remaining_bytes() < 10) break;
        
        tinygltf::Scene modified = base;
        
        uint8_t property_to_modify = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (property_to_modify) {
            case 0:
                modified.name += "_modified_" + std::to_string(i);
                break;
            case 1:
                // Modify nodes vector
                if (!modified.nodes.empty() && fdp.ConsumeBool()) {
                    modified.nodes.pop_back();
                } else {
                    modified.nodes.push_back(fdp.ConsumeIntegral<int>());
                }
                break;
            case 2:
                modified.extensions["test_mod"] = tinygltf::Value(fdp.ConsumeRandomLengthString(16));
                break;
        }
        
        // Test equality
        bool equal_result = (base == modified);
        bool self_equal = (modified == modified);
        (void)equal_result;
        (void)self_equal;
    }
}

// Function to test Light equality with random property modifications
void test_light_equality(FuzzedDataProvider& fdp, const tinygltf::Light& base) {
    const int num_tests = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < num_tests; i++) {
        if (fdp.remaining_bytes() < 10) break;
        
        tinygltf::Light modified = base;
        
        uint8_t property_to_modify = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (property_to_modify) {
            case 0:
                modified.name += "_modified_" + std::to_string(i);
                break;
            case 1:
                modified.type = fdp.ConsumeBool() ? "point" : "directional";
                break;
            case 2:
                // Modify color array
                if (modified.color.size() >= 3) {
                    modified.color[0] = fdp.ConsumeFloatingPoint<double>();
                    modified.color[1] = fdp.ConsumeFloatingPoint<double>();
                    modified.color[2] = fdp.ConsumeFloatingPoint<double>();
                }
                break;
        }
        
        // Test equality
        bool equal_result = (base == modified);
        bool self_equal = (modified == modified);
        (void)equal_result;
        (void)self_equal;
    }
}

// Function to test AudioSource equality with random property modifications
void test_audio_source_equality(FuzzedDataProvider& fdp, const tinygltf::AudioSource& base) {
    const int num_tests = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < num_tests; i++) {
        if (fdp.remaining_bytes() < 10) break;
        
        tinygltf::AudioSource modified = base;
        
        uint8_t property_to_modify = fdp.ConsumeIntegral<uint8_t>() % 2;
        
        switch (property_to_modify) {
            case 0:
                modified.name += "_modified_" + std::to_string(i);
                break;
            case 1:
                modified.uri = fdp.ConsumeRandomLengthString(64);
                break;
        }
        
        // Note: AudioSource equality operator only compares name and uri
        // Test equality
        bool equal_result = (base == modified);
        bool self_equal = (modified == modified);
        (void)equal_result;
        (void)self_equal;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need enough data for meaningful testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Generate glTF JSON with all target components
    std::string gltf_json = generate_gltf_for_property_modification(fdp);
    
    // Choose loading method based on fuzzer input
    bool use_ascii = fdp.ConsumeBool();
    
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    bool load_success = false;
    
    if (use_ascii) {
        // Load from ASCII/JSON string
        load_success = loader.LoadASCIIFromString(
            &model, &err, &warn,
            gltf_json.c_str(),
            static_cast<unsigned int>(gltf_json.size()),
            /* base_dir */ ""
        );
    } else {
        // Load from binary data using LoadBinaryFromMemory
        // Note: We're using JSON as binary data, which should fail or parse differently
        load_success = loader.LoadBinaryFromMemory(
            &model, &err, &warn,
            reinterpret_cast<const unsigned char*>(gltf_json.c_str()),
            static_cast<unsigned int>(gltf_json.size()),
            /* base_dir */ ""
        );
    }
    
    if (!load_success) {
        // Even if loading fails, we might have exercised some error paths
        return 0;
    }
    
    // Test 1: PositionalEmitter equality (from AudioEmitter)
    if (!model.audioEmitters.empty()) {
        const tinygltf::AudioEmitter& audio_emitter = model.audioEmitters[0];
        const tinygltf::PositionalEmitter& positional_emitter = audio_emitter.positional;
        
        test_positional_emitter_equality(fdp, positional_emitter);
    }
    
    // Test 2: Texture equality
    if (!model.textures.empty()) {
        const tinygltf::Texture& texture = model.textures[0];
        test_texture_equality(fdp, texture);
    }
    
    // Test 3: SpotLight equality (from Light extensions)
    if (!model.lights.empty()) {
        const tinygltf::Light& light = model.lights[0];
        const tinygltf::SpotLight& spotlight = light.spot;
        
        test_spotlight_equality(fdp, spotlight);
    }
    
    // Test 4: Scene equality
    if (!model.scenes.empty()) {
        const tinygltf::Scene& scene = model.scenes[0];
        test_scene_equality(fdp, scene);
    }
    
    // Test 5: Light equality
    if (!model.lights.empty()) {
        const tinygltf::Light& light = model.lights[0];
        test_light_equality(fdp, light);
    }
    
    // Test 6: AudioSource equality
    if (!model.audioSources.empty()) {
        const tinygltf::AudioSource& audio_source = model.audioSources[0];
        test_audio_source_equality(fdp, audio_source);
    }
    
    // Test 7: Cross-object comparisons (different object types should not be equal)
    // This tests that equality operators return false for different types
    if (!model.textures.empty() && !model.scenes.empty()) {
        // These are different types, can't compare directly, but we can test
        // that they're not accidentally comparable
        const tinygltf::Texture& texture = model.textures[0];
        const tinygltf::Scene& scene = model.scenes[0];
        
        // Test self-equality again
        bool texture_self_eq = (texture == texture);
        bool scene_self_eq = (scene == scene);
        (void)texture_self_eq;
        (void)scene_self_eq;
    }
    
    // Test 8: Equality with exact copies
    if (!model.textures.empty()) {
        tinygltf::Texture exact_copy = model.textures[0];
        bool exact_eq = (model.textures[0] == exact_copy);
        (void)exact_eq;
    }
    
    if (!model.scenes.empty()) {
        tinygltf::Scene exact_copy = model.scenes[0];
        bool exact_eq = (model.scenes[0] == exact_copy);
        (void)exact_eq;
    }
    
    if (!model.lights.empty()) {
        tinygltf::Light exact_copy = model.lights[0];
        bool exact_eq = (model.lights[0] == exact_copy);
        (void)exact_eq;
    }
    
    if (!model.audioSources.empty()) {
        tinygltf::AudioSource exact_copy = model.audioSources[0];
        bool exact_eq = (model.audioSources[0] == exact_copy);
        (void)exact_eq;
    }
    
    return 0;
}

} // namespace harness_016
