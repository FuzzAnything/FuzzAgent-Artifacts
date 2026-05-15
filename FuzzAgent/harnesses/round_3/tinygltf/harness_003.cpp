#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_003 {
/*
 * Fuzzing harness for tinygltf library targeting equality operators
 * Goal: Exercise high-complexity equality operators with 85.9% uncovered public APIs
 * Targets: Model::eq (40 branches), Material::eq (26 branches), Camera::eq (12 branches), 
 *          Animation::eq (10 branches), Node::eq (28 branches)
 * Strategy: Load two GLTF models (identical or slightly different) and test equality operators
 * Differentiation from previous harnesses:
 *   - harness_000: Basic GLTF parsing (loading/deserialization)
 *   - harness_001: Image loading via embedded images
 *   - harness_002: Image writing using callback APIs
 *   - harness_003: Equality operator testing between loaded models
 */



// Use same includes as other harnesses
// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_IMPLEMENTATION

// Simple GLTF JSON generator for testing - uses fuzzer input
std::string create_minimal_gltf_json(FuzzedDataProvider& fdp, const std::string& asset_name = "TestAsset") {
    std::string generator_name = fdp.ConsumeRandomLengthString(32);
    if (generator_name.empty()) generator_name = "TestHarness";
    
    char buffer[1024];
    snprintf(buffer, sizeof(buffer),
        "{\n"
        "  \"asset\": {\n"
        "    \"version\": \"2.0\",\n"
        "    \"generator\": \"%s\"\n"
        "  }\n"
        "}", generator_name.c_str());
    return std::string(buffer);
}

// Create GLTF with a simple node using fuzzer input
std::string create_gltf_with_node(FuzzedDataProvider& fdp, const std::string& node_name) {
    char buffer[2048];
    snprintf(buffer, sizeof(buffer),
        "{\n"
        "  \"asset\": {\"version\": \"2.0\"},\n"
        "  \"nodes\": [\n"
        "    {\n"
        "      \"name\": \"%s\"\n"
        "    }\n"
        "  ]\n"
        "}", node_name.c_str());
    return std::string(buffer);
}

// Create GLTF with a material using fuzzer input for properties
std::string create_gltf_with_material(FuzzedDataProvider& fdp, const std::string& material_name) {
    // Consume color values from fuzzer input
    float r = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
    float g = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
    float b = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
    float a = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
    float metallic = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
    float roughness = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
    
    char buffer[3072];
    snprintf(buffer, sizeof(buffer),
        "{\n"
        "  \"asset\": {\"version\": \"2.0\"},\n"
        "  \"materials\": [\n"
        "    {\n"
        "      \"name\": \"%s\",\n"
        "      \"pbrMetallicRoughness\": {\n"
        "        \"baseColorFactor\": [%.3f, %.3f, %.3f, %.3f],\n"
        "        \"metallicFactor\": %.3f,\n"
        "        \"roughnessFactor\": %.3f\n"
        "      }\n"
        "    }\n"
        "  ]\n"
        "}", material_name.c_str(), r, g, b, a, metallic, roughness);
    return std::string(buffer);
}

// Create GLTF with a camera using fuzzer input for properties
std::string create_gltf_with_camera(FuzzedDataProvider& fdp, const std::string& camera_name, bool perspective = true) {
    if (perspective) {
        // Consume perspective camera values from fuzzer input
        float yfov = fdp.ConsumeFloatingPointInRange<float>(0.1f, 3.0f);
        float znear = fdp.ConsumeFloatingPointInRange<float>(0.01f, 10.0f);
        float zfar = fdp.ConsumeFloatingPointInRange<float>(znear + 0.1f, 1000.0f);
        
        char buffer[3072];
        snprintf(buffer, sizeof(buffer),
            "{\n"
            "  \"asset\": {\"version\": \"2.0\"},\n"
            "  \"cameras\": [\n"
            "    {\n"
            "      \"name\": \"%s\",\n"
            "      \"type\": \"perspective\",\n"
            "      \"perspective\": {\n"
            "        \"yfov\": %.3f,\n"
            "        \"znear\": %.3f,\n"
            "        \"zfar\": %.3f\n"
            "      }\n"
            "    }\n"
            "  ]\n"
            "}", camera_name.c_str(), yfov, znear, zfar);
        return std::string(buffer);
    } else {
        // Consume orthographic camera values from fuzzer input
        float xmag = fdp.ConsumeFloatingPointInRange<float>(0.1f, 10.0f);
        float ymag = fdp.ConsumeFloatingPointInRange<float>(0.1f, 10.0f);
        float znear = fdp.ConsumeFloatingPointInRange<float>(0.01f, 10.0f);
        float zfar = fdp.ConsumeFloatingPointInRange<float>(znear + 0.1f, 1000.0f);
        
        char buffer[3072];
        snprintf(buffer, sizeof(buffer),
            "{\n"
            "  \"asset\": {\"version\": \"2.0\"},\n"
            "  \"cameras\": [\n"
            "    {\n"
            "      \"name\": \"%s\",\n"
            "      \"type\": \"orthographic\",\n"
            "      \"orthographic\": {\n"
            "        \"xmag\": %.3f,\n"
            "        \"ymag\": %.3f,\n"
            "        \"znear\": %.3f,\n"
            "        \"zfar\": %.3f\n"
            "      }\n"
            "    }\n"
            "  ]\n"
            "}", camera_name.c_str(), xmag, ymag, znear, zfar);
        return std::string(buffer);
    }
}

// Create GLTF with an animation using fuzzer input
std::string create_gltf_with_animation(FuzzedDataProvider& fdp, const std::string& anim_name) {
    // Generate some random base64 data from fuzzer input
    std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(120);
    if (random_data.size() < 120) {
        random_data.resize(120, 0);
    }
    
    // Simple base64 encoding of random data
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string base64_data;
    for (size_t i = 0; i < random_data.size(); i += 3) {
        uint32_t triple = (random_data[i] << 16) | 
                         ((i + 1 < random_data.size() ? random_data[i + 1] : 0) << 8) |
                         (i + 2 < random_data.size() ? random_data[i + 2] : 0);
        
        base64_data += base64_chars[(triple >> 18) & 0x3F];
        base64_data += base64_chars[(triple >> 12) & 0x3F];
        base64_data += (i + 1 < random_data.size() ? base64_chars[(triple >> 6) & 0x3F] : '=');
        base64_data += (i + 2 < random_data.size() ? base64_chars[triple & 0x3F] : '=');
    }
    
    char buffer[4096];
    snprintf(buffer, sizeof(buffer),
        "{\n"
        "  \"asset\": {\"version\": \"2.0\"},\n"
        "  \"animations\": [\n"
        "    {\n"
        "      \"name\": \"%s\",\n"
        "      \"channels\": [\n"
        "        {\n"
        "          \"sampler\": 0,\n"
        "          \"target\": {\n"
        "            \"node\": 0,\n"
        "            \"path\": \"translation\"\n"
        "          }\n"
        "        }\n"
        "      ],\n"
        "      \"samplers\": [\n"
        "        {\n"
        "          \"input\": 0,\n"
        "          \"output\": 1,\n"
        "          \"interpolation\": \"LINEAR\"\n"
        "        }\n"
        "      ]\n"
        "    }\n"
        "  ],\n"
        "  \"nodes\": [\n"
        "    {\"name\": \"node0\"}\n"
        "  ],\n"
        "  \"accessors\": [\n"
        "    {\n"
        "      \"bufferView\": 0,\n"
        "      \"componentType\": 5126,\n"
        "      \"count\": 3,\n"
        "      \"type\": \"SCALAR\"\n"
        "    },\n"
        "    {\n"
        "      \"bufferView\": 1,\n"
        "      \"componentType\": 5126,\n"
        "      \"count\": 9,\n"
        "      \"type\": \"VEC3\"\n"
        "    }\n"
        "  ],\n"
        "  \"bufferViews\": [\n"
        "    {\n"
        "      \"buffer\": 0,\n"
        "      \"byteOffset\": 0,\n"
        "      \"byteLength\": 12\n"
        "    },\n"
        "    {\n"
        "      \"buffer\": 0,\n"
        "      \"byteOffset\": 12,\n"
        "      \"byteLength\": 108\n"
        "    }\n"
        "  ],\n"
        "  \"buffers\": [\n"
        "    {\n"
        "      \"uri\": \"data:application/octet-stream;base64,%s\",\n"
        "      \"byteLength\": 120\n"
        "    }\n"
        "  ]\n"
        "}", anim_name.c_str(), base64_data.c_str());
    return std::string(buffer);
}

// Test 1: Load identical models and test Model equality
static void test_model_equality(FuzzedDataProvider& fdp) {
    // Create minimal GLTF using fuzzer input
    std::string gltf1 = create_minimal_gltf_json(fdp);
    std::string gltf2 = gltf1; // Identical
    
    tinygltf::Model model1, model2;
    tinygltf::TinyGLTF ctx;
    std::string err1, warn1, err2, warn2;
    
    bool ret1 = ctx.LoadASCIIFromString(&model1, &err1, &warn1, gltf1.c_str(),
                                       static_cast<unsigned int>(gltf1.size()),
                                       /* base_dir */ "");
    bool ret2 = ctx.LoadASCIIFromString(&model2, &err2, &warn2, gltf2.c_str(),
                                       static_cast<unsigned int>(gltf2.size()),
                                       /* base_dir */ "");
    
    if (ret1 && ret2) {
        // Test Model equality operator
        bool models_equal = (model1 == model2);
        (void)models_equal;
        
        // Test self-equality
        bool self_equal = (model1 == model1);
        (void)self_equal;
    }
}

// Test 2: Load models with different nodes and test Node equality
static void test_node_equality(FuzzedDataProvider& fdp) {
    // Consume node names from fuzzer input
    std::string node_name1 = fdp.ConsumeRandomLengthString(32);
    std::string node_name2 = fdp.ConsumeRandomLengthString(32);
    
    if (node_name1.empty()) node_name1 = "node1";
    if (node_name2.empty()) node_name2 = "node2";
    
    std::string gltf1 = create_gltf_with_node(fdp, node_name1);
    std::string gltf2 = create_gltf_with_node(fdp, node_name2);
    
    tinygltf::Model model1, model2;
    tinygltf::TinyGLTF ctx;
    std::string err1, warn1, err2, warn2;
    
    bool ret1 = ctx.LoadASCIIFromString(&model1, &err1, &warn1, gltf1.c_str(),
                                       static_cast<unsigned int>(gltf1.size()),
                                       /* base_dir */ "");
    bool ret2 = ctx.LoadASCIIFromString(&model2, &err2, &warn2, gltf2.c_str(),
                                       static_cast<unsigned int>(gltf2.size()),
                                       /* base_dir */ "");
    
    if (ret1 && ret2 && !model1.nodes.empty() && !model2.nodes.empty()) {
        // Test Node equality operators
        bool nodes_equal = (model1.nodes[0] == model2.nodes[0]);
        (void)nodes_equal;
        
        // Test self-equality for each node
        bool node1_self_equal = (model1.nodes[0] == model1.nodes[0]);
        bool node2_self_equal = (model2.nodes[0] == model2.nodes[0]);
        (void)node1_self_equal;
        (void)node2_self_equal;
    }
}

// Test 3: Load models with different materials and test Material equality
static void test_material_equality(FuzzedDataProvider& fdp) {
    // Consume material names and properties from fuzzer input
    std::string material_name1 = fdp.ConsumeRandomLengthString(32);
    std::string material_name2 = fdp.ConsumeRandomLengthString(32);
    
    if (material_name1.empty()) material_name1 = "material1";
    if (material_name2.empty()) material_name2 = "material2";
    
    std::string gltf1 = create_gltf_with_material(fdp, material_name1);
    std::string gltf2 = create_gltf_with_material(fdp, material_name2);
    
    tinygltf::Model model1, model2;
    tinygltf::TinyGLTF ctx;
    std::string err1, warn1, err2, warn2;
    
    bool ret1 = ctx.LoadASCIIFromString(&model1, &err1, &warn1, gltf1.c_str(),
                                       static_cast<unsigned int>(gltf1.size()),
                                       /* base_dir */ "");
    bool ret2 = ctx.LoadASCIIFromString(&model2, &err2, &warn2, gltf2.c_str(),
                                       static_cast<unsigned int>(gltf2.size()),
                                       /* base_dir */ "");
    
    if (ret1 && ret2 && !model1.materials.empty() && !model2.materials.empty()) {
        // Test Material equality operators
        bool materials_equal = (model1.materials[0] == model2.materials[0]);
        (void)materials_equal;
        
        // Test self-equality for each material
        bool material1_self_equal = (model1.materials[0] == model1.materials[0]);
        bool material2_self_equal = (model2.materials[0] == model2.materials[0]);
        (void)material1_self_equal;
        (void)material2_self_equal;
    }
}

// Test 4: Load models with different cameras and test Camera equality
static void test_camera_equality(FuzzedDataProvider& fdp) {
    // Consume camera names and types from fuzzer input
    std::string camera_name1 = fdp.ConsumeRandomLengthString(32);
    std::string camera_name2 = fdp.ConsumeRandomLengthString(32);
    bool perspective1 = fdp.ConsumeBool();
    bool perspective2 = fdp.ConsumeBool();
    
    if (camera_name1.empty()) camera_name1 = "camera1";
    if (camera_name2.empty()) camera_name2 = "camera2";
    
    std::string gltf1 = create_gltf_with_camera(fdp, camera_name1, perspective1);
    std::string gltf2 = create_gltf_with_camera(fdp, camera_name2, perspective2);
    
    tinygltf::Model model1, model2;
    tinygltf::TinyGLTF ctx;
    std::string err1, warn1, err2, warn2;
    
    bool ret1 = ctx.LoadASCIIFromString(&model1, &err1, &warn1, gltf1.c_str(),
                                       static_cast<unsigned int>(gltf1.size()),
                                       /* base_dir */ "");
    bool ret2 = ctx.LoadASCIIFromString(&model2, &err2, &warn2, gltf2.c_str(),
                                       static_cast<unsigned int>(gltf2.size()),
                                       /* base_dir */ "");
    
    if (ret1 && ret2 && !model1.cameras.empty() && !model2.cameras.empty()) {
        // Test Camera equality operators
        bool cameras_equal = (model1.cameras[0] == model2.cameras[0]);
        (void)cameras_equal;
        
        // Test self-equality for each camera
        bool camera1_self_equal = (model1.cameras[0] == model1.cameras[0]);
        bool camera2_self_equal = (model2.cameras[0] == model2.cameras[0]);
        (void)camera1_self_equal;
        (void)camera2_self_equal;
    }
}

// Test 5: Load models with different animations and test Animation equality
static void test_animation_equality(FuzzedDataProvider& fdp) {
    // Consume animation names from fuzzer input
    std::string anim_name1 = fdp.ConsumeRandomLengthString(32);
    std::string anim_name2 = fdp.ConsumeRandomLengthString(32);
    
    if (anim_name1.empty()) anim_name1 = "animation1";
    if (anim_name2.empty()) anim_name2 = "animation2";
    
    std::string gltf1 = create_gltf_with_animation(fdp, anim_name1);
    std::string gltf2 = create_gltf_with_animation(fdp, anim_name2);
    
    tinygltf::Model model1, model2;
    tinygltf::TinyGLTF ctx;
    std::string err1, warn1, err2, warn2;
    
    bool ret1 = ctx.LoadASCIIFromString(&model1, &err1, &warn1, gltf1.c_str(),
                                       static_cast<unsigned int>(gltf1.size()),
                                       /* base_dir */ "");
    bool ret2 = ctx.LoadASCIIFromString(&model2, &err2, &warn2, gltf2.c_str(),
                                       static_cast<unsigned int>(gltf2.size()),
                                       /* base_dir */ "");
    
    if (ret1 && ret2 && !model1.animations.empty() && !model2.animations.empty()) {
        // Test Animation equality operators
        bool animations_equal = (model1.animations[0] == model2.animations[0]);
        (void)animations_equal;
        
        // Test self-equality for each animation
        bool anim1_self_equal = (model1.animations[0] == model1.animations[0]);
        bool anim2_self_equal = (model2.animations[0] == model2.animations[0]);
        (void)anim1_self_equal;
        (void)anim2_self_equal;
    }
}

// Test 6: Test equality of multiple components together
static void test_comprehensive_equality(FuzzedDataProvider& fdp) {
    // Create a more complex GLTF with multiple components using fuzzer input
    std::string node_name1 = fdp.ConsumeRandomLengthString(16);
    std::string node_name2 = fdp.ConsumeRandomLengthString(16);
    std::string mat_name1 = fdp.ConsumeRandomLengthString(16);
    std::string mat_name2 = fdp.ConsumeRandomLengthString(16);
    std::string cam_name = fdp.ConsumeRandomLengthString(16);
    
    if (node_name1.empty()) node_name1 = "node1";
    if (node_name2.empty()) node_name2 = "node2";
    if (mat_name1.empty()) mat_name1 = "mat1";
    if (mat_name2.empty()) mat_name2 = "mat2";
    if (cam_name.empty()) cam_name = "cam1";
    
    char complex_gltf[8192];
    snprintf(complex_gltf, sizeof(complex_gltf),
        "{\n"
        "  \"asset\": {\"version\": \"2.0\"},\n"
        "  \"nodes\": [\n"
        "    {\"name\": \"%s\"},\n"
        "    {\"name\": \"%s\"}\n"
        "  ],\n"
        "  \"materials\": [\n"
        "    {\n"
        "      \"name\": \"%s\",\n"
        "      \"pbrMetallicRoughness\": {\n"
        "        \"baseColorFactor\": [1.0, 0.0, 0.0, 1.0]\n"
        "      }\n"
        "    },\n"
        "    {\n"
        "      \"name\": \"%s\",\n"
        "      \"pbrMetallicRoughness\": {\n"
        "        \"baseColorFactor\": [0.0, 1.0, 0.0, 1.0]\n"
        "      }\n"
        "    }\n"
        "  ],\n"
        "  \"cameras\": [\n"
        "    {\n"
        "      \"name\": \"%s\",\n"
        "      \"type\": \"perspective\",\n"
        "      \"perspective\": {\n"
        "        \"yfov\": 0.75,\n"
        "        \"znear\": 0.1\n"
        "      }\n"
        "    }\n"
        "  ]\n"
        "}", node_name1.c_str(), node_name2.c_str(), mat_name1.c_str(), mat_name2.c_str(), cam_name.c_str());
    
    std::string gltf1 = complex_gltf;
    std::string gltf2 = complex_gltf; // Identical
    
    tinygltf::Model model1, model2;
    tinygltf::TinyGLTF ctx;
    std::string err1, warn1, err2, warn2;
    
    bool ret1 = ctx.LoadASCIIFromString(&model1, &err1, &warn1, gltf1.c_str(),
                                       static_cast<unsigned int>(gltf1.size()),
                                       /* base_dir */ "");
    bool ret2 = ctx.LoadASCIIFromString(&model2, &err2, &warn2, gltf2.c_str(),
                                       static_cast<unsigned int>(gltf2.size()),
                                       /* base_dir */ "");
    
    if (ret1 && ret2) {
        // Test Model equality
        bool models_equal = (model1 == model2);
        (void)models_equal;
        
        // Test equality of individual components if they exist
        if (!model1.nodes.empty() && !model2.nodes.empty()) {
            bool nodes_equal = (model1.nodes[0] == model2.nodes[0]);
            (void)nodes_equal;
        }
        
        if (!model1.materials.empty() && !model2.materials.empty()) {
            bool materials_equal = (model1.materials[0] == model2.materials[0]);
            (void)materials_equal;
        }
        
        if (!model1.cameras.empty() && !model2.cameras.empty()) {
            bool cameras_equal = (model1.cameras[0] == model2.cameras[0]);
            (void)cameras_equal;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) return 0;  // Need minimum input for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Use first byte to select test case (similar to harness_000 pattern)
    uint8_t test_case = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    try {
        switch (test_case) {
            case 0:
                test_model_equality(fdp);
                break;
            case 1:
                test_node_equality(fdp);
                break;
            case 2:
                test_material_equality(fdp);
                break;
            case 3:
                test_camera_equality(fdp);
                break;
            case 4:
                test_animation_equality(fdp);
                break;
            case 5:
                test_comprehensive_equality(fdp);
                break;
        }
    } catch (...) {
        // Catch any exceptions to prevent crashes from malformed input
    }
    
    return 0;
}

} // namespace harness_003
