#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <sys/types.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_015 {
/*
 * Fuzzing harness for tinygltf library targeting GLTF object equality testing via file-based loading
 * Goal: Exercise equality operators for PositionalEmitter, Texture, Scene, SpotLight, Light, and AudioSource objects
 *       using file-based loading methods (LoadASCIIFromFile, LoadBinaryFromFile)
 * Primary Targets:
 *   - _ZNK8tinygltf17PositionalEmittereqERKS0_ (PositionalEmitter equality operator: 20 undiscovered branches)
 *   - _ZNK8tinygltf7TextureeqERKS0_ (Texture equality operator: 14 undiscovered branches)
 *   - _ZNK8tinygltf5SceneeqERKS0_ (Scene equality operator: 12 undiscovered branches)
 *   - _ZNK8tinygltf9SpotLighteqERKS0_ (SpotLight equality operator: 12 undiscovered branches)
 *   - _ZNK8tinygltf5LighteqERKS0_ (Light equality operator: 6 undiscovered branches)
 *   - _ZNK8tinygltf11AudioSourceeqERKS0_ (AudioSource equality operator: 4 undiscovered branches)
 * Required Setup: Create temporary GLTF files, load using TinyGLTF::LoadASCIIFromFile/LoadBinaryFromFile,
 *                 create Model objects, and perform equality comparisons
 * Strategy: Generate GLTF data with target objects, write to temporary files, load via both file-based methods,
 *           test equality operators between corresponding objects from different loading methods
 * Differentiation from existing harnesses (harness_000 to harness_014):
 *   - harness_013: Tests equality operators using LoadASCIIFromString and LoadBinaryFromMemory (memory-based)
 *   - harness_014: Tests equality via systematic component modification and comparison
 *   - harness_015: FOCUSED on file-based loading (LoadASCIIFromFile, LoadBinaryFromFile) for equality testing
 */



// Use same includes as other harnesses
// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_IMPLEMENTATION

// Simple base64 encoding for image/buffer data (copied from other harnesses)
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
std::string generate_gltf_for_file_testing(FuzzedDataProvider& fdp) {
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
    
    // Light parameters
    std::vector<double> light_color = {
        fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0),
        fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0),
        fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0)
    };
    double light_intensity = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    std::string light_type = fdp.ConsumeBool() ? "spot" : "point";
    double light_range = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
    double spot_inner_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 3.14159);
    double spot_outer_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 3.14159);
    
    // Audio parameters
    std::string audio_uri = fdp.ConsumeRandomLengthString(64);
    int buffer_view = fdp.ConsumeIntegralInRange<int>(-1, 10);
    std::string mime_type = fdp.PickValueInArray({"audio/mpeg", "audio/wav", "audio/ogg"});
    double audio_gain = fdp.ConsumeFloatingPointInRange<double>(0.0, 2.0);
    bool audio_loop = fdp.ConsumeBool();
    bool audio_playing = fdp.ConsumeBool();
    std::string audio_type = fdp.ConsumeBool() ? "positional" : "global";
    std::string distance_model = fdp.PickValueInArray({"linear", "inverse", "exponential"});
    double cone_inner_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 6.28318);
    double cone_outer_angle = fdp.ConsumeFloatingPointInRange<double>(0.0, 6.28318);
    double cone_outer_gain = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double max_distance = fdp.ConsumeFloatingPointInRange<double>(0.0, 1000.0);
    double ref_distance = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
    double rolloff_factor = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    
    // Generate simple image data for texture
    std::vector<uint8_t> image_data;
    size_t image_size = fdp.ConsumeIntegralInRange<size_t>(16, 256);
    for (size_t i = 0; i < image_size; i++) {
        image_data.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    std::string image_base64 = base64_encode(image_data.data(), image_data.size());
    
    std::ostringstream json;
    json << "{\n";
    json << "  \"asset\": {\n";
    json << "    \"version\": \"2.0\",\n";
    json << "    \"generator\": \"fuzzer\"\n";
    json << "  },\n";
    
    // Buffers and buffer views for images and audio
    json << "  \"buffers\": [\n";
    json << "    {\n";
    json << "      \"uri\": \"data:image/png;base64," << image_base64 << "\",\n";
    json << "      \"byteLength\": " << image_data.size() << "\n";
    json << "    }\n";
    json << "  ],\n";
    
    json << "  \"bufferViews\": [\n";
    json << "    {\n";
    json << "      \"buffer\": 0,\n";
    json << "      \"byteOffset\": 0,\n";
    json << "      \"byteLength\": " << image_data.size() << "\n";
    json << "    }\n";
    json << "  ],\n";
    
    // Images
    json << "  \"images\": [\n";
    json << "    {\n";
    json << "      \"uri\": \"data:image/png;base64," << image_base64 << "\"\n";
    json << "    }\n";
    json << "  ],\n";
    
    // Textures
    json << "  \"textures\": [\n";
    json << "    {\n";
    json << "      \"name\": \"" << texture_name << "\"";
    if (sampler_index >= 0) {
        json << ",\n      \"sampler\": " << sampler_index;
    }
    if (source_index >= 0) {
        json << ",\n      \"source\": " << source_index;
    }
    json << "\n    }\n";
    json << "  ],\n";
    
    // Nodes for scene
    json << "  \"nodes\": [\n";
    for (int i = 0; i < scene_node_count; i++) {
        json << "    {\n";
        json << "      \"name\": \"node" << i << "\"\n";
        json << "    }";
        if (i < scene_node_count - 1) json << ",";
        json << "\n";
    }
    json << "  ],\n";
    
    // Scenes
    json << "  \"scenes\": [\n";
    json << "    {\n";
    json << "      \"name\": \"" << scene_name << "\",\n";
    json << "      \"nodes\": [";
    for (int i = 0; i < scene_node_count; i++) {
        json << i;
        if (i < scene_node_count - 1) json << ", ";
    }
    json << "]\n";
    json << "    }\n";
    json << "  ],\n";
    
    // Lights (KHR_lights_punctual extension)
    json << "  \"extensionsUsed\": [\"KHR_lights_punctual\", \"KHR_audio\"],\n";
    json << "  \"extensions\": {\n";
    json << "    \"KHR_lights_punctual\": {\n";
    json << "      \"lights\": [\n";
    json << "        {\n";
    json << "          \"name\": \"" << light_name << "\",\n";
    json << "          \"color\": [" << light_color[0] << ", " << light_color[1] << ", " << light_color[2] << "],\n";
    json << "          \"intensity\": " << light_intensity << ",\n";
    json << "          \"type\": \"" << light_type << "\",\n";
    json << "          \"range\": " << light_range << ",\n";
    json << "          \"spot\": {\n";
    json << "            \"innerConeAngle\": " << spot_inner_angle << ",\n";
    json << "            \"outerConeAngle\": " << spot_outer_angle << "\n";
    json << "          }\n";
    json << "        }\n";
    json << "      ]\n";
    json << "    },\n";
    
    // KHR_audio extension
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

// Create a temporary file with given content and return its path
std::string create_temp_file(const std::string& content, const std::string& suffix = ".gltf") {
    char template_path[] = "/tmp/tinygltf_fuzz_XXXXXX";
    int fd = mkstemp(template_path);
    if (fd == -1) {
        return "";
    }
    
    // Write content to file
    ssize_t written = write(fd, content.c_str(), content.size());
    close(fd);
    
    if (written != static_cast<ssize_t>(content.size())) {
        unlink(template_path);
        return "";
    }
    
    return template_path;
}

// Test equality operators using file-based loading methods
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Generate glTF JSON for testing
    std::string gltf_json = generate_gltf_for_file_testing(fdp);
    
    // Create temporary files
    std::string ascii_file = create_temp_file(gltf_json, ".gltf");
    std::string binary_file = create_temp_file(gltf_json, ".glb");
    
    if (ascii_file.empty() || binary_file.empty()) {
        // Clean up any created files
        if (!ascii_file.empty()) unlink(ascii_file.c_str());
        if (!binary_file.empty()) unlink(binary_file.c_str());
        return 0;
    }
    
    // Create loader instances
    tinygltf::TinyGLTF loader1;
    tinygltf::TinyGLTF loader2;
    
    // Test 1: Load from ASCII file using LoadASCIIFromFile
    tinygltf::Model model_from_ascii_file;
    std::string err1, warn1;
    
    bool load_ascii_file_success = loader1.LoadASCIIFromFile(
        &model_from_ascii_file, &err1, &warn1,
        ascii_file
    );
    
    // Test 2: Load from binary file using LoadBinaryFromFile
    tinygltf::Model model_from_binary_file;
    std::string err2, warn2;
    
    bool load_binary_file_success = loader2.LoadBinaryFromFile(
        &model_from_binary_file, &err2, &warn2,
        binary_file
    );
    
    // Clean up temporary files
    unlink(ascii_file.c_str());
    unlink(binary_file.c_str());
    
    // If either loading method succeeded, test equality operators
    if (load_ascii_file_success || load_binary_file_success) {
        // Test Texture equality if textures exist
        if (!model_from_ascii_file.textures.empty() && !model_from_binary_file.textures.empty()) {
            const tinygltf::Texture& texture1 = model_from_ascii_file.textures[0];
            const tinygltf::Texture& texture2 = model_from_binary_file.textures[0];
            
            // Test equality operator
            bool texture_equal = (texture1 == texture2);
            // Test self-equality (should always be true)
            bool texture_self_equal = (texture1 == texture1);
            (void)texture_equal;
            (void)texture_self_equal;
        }
        
        // Test Scene equality if scenes exist
        if (!model_from_ascii_file.scenes.empty() && !model_from_binary_file.scenes.empty()) {
            const tinygltf::Scene& scene1 = model_from_ascii_file.scenes[0];
            const tinygltf::Scene& scene2 = model_from_binary_file.scenes[0];
            
            // Test equality operator
            bool scene_equal = (scene1 == scene2);
            // Test self-equality (should always be true)
            bool scene_self_equal = (scene1 == scene1);
            (void)scene_equal;
            (void)scene_self_equal;
        }
        
        // Test Light equality if lights exist (via extensions)
        if (model_from_ascii_file.lights.size() > 0 && model_from_binary_file.lights.size() > 0) {
            const tinygltf::Light& light1 = model_from_ascii_file.lights[0];
            const tinygltf::Light& light2 = model_from_binary_file.lights[0];
            
            // Test equality operator
            bool light_equal = (light1 == light2);
            // Test self-equality (should always be true)
            bool light_self_equal = (light1 == light1);
            (void)light_equal;
            (void)light_self_equal;
            
            // If it's a spot light, test SpotLight equality
            if (light1.type == "spot" && light2.type == "spot") {
                // Note: Need to access spot light specific equality operator
                // The equality operator for Light should handle spot light comparison
            }
        }
        
        // Test AudioSource equality if audio sources exist
        if (model_from_ascii_file.audioSources.size() > 0 && model_from_binary_file.audioSources.size() > 0) {
            const tinygltf::AudioSource& audio1 = model_from_ascii_file.audioSources[0];
            const tinygltf::AudioSource& audio2 = model_from_binary_file.audioSources[0];
            
            // Test equality operator
            bool audio_equal = (audio1 == audio2);
            // Test self-equality (should always be true)
            bool audio_self_equal = (audio1 == audio1);
            (void)audio_equal;
            (void)audio_self_equal;
        }
        
        // Test PositionalEmitter equality if audio emitters exist
        if (model_from_ascii_file.audioEmitters.size() > 0 && model_from_binary_file.audioEmitters.size() > 0) {
            const tinygltf::AudioEmitter& emitter1 = model_from_ascii_file.audioEmitters[0];
            const tinygltf::AudioEmitter& emitter2 = model_from_binary_file.audioEmitters[0];
            
            // Test PositionalEmitter equality (within audio emitter)
            const tinygltf::PositionalEmitter& positional1 = emitter1.positional;
            const tinygltf::PositionalEmitter& positional2 = emitter2.positional;
            
            // Test equality operator
            bool positional_equal = (positional1 == positional2);
            // Test self-equality (should always be true)
            bool positional_self_equal = (positional1 == positional1);
            (void)positional_equal;
            (void)positional_self_equal;
        }
    }
    
    return 0;
}

} // namespace harness_015
