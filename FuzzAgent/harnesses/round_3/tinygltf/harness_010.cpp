#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_010 {
/*
 * Fuzzing harness for tinygltf library targeting GLTF Model comparison and equality testing operations
 * Goal: Exercise high-complexity Model equality operator (199 undiscovered branches) and related structure equality operators
 * Primary Target: _ZNK8tinygltf5ModeleqERKS0_ (Model equality operator - 199 undiscovered branches)
 * Secondary Targets: 
 *   - _ZNK8tinygltf17PositionalEmittereqERKS0_ (20 undiscovered branches)
 *   - _ZNK8tinygltf6BuffereqERKS0_ (14 undiscovered branches)
 *   - _ZNK8tinygltf7TextureeqERKS0_ (14 undiscovered branches)
 * Required Setup: Load/create GLTF Model objects using tinygltf::TinyGLTF::LoadASCIIFromString/LoadBinaryFromMemory
 * Strategy: Create both identical and differing GLTF models and test equality operators comprehensively
 * Differentiation from existing harnesses (harness_000 to harness_009):
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
 *   - harness_010: FOCUSED on high-complexity Model equality (199 branches) + PositionalEmitter/Buffer/Texture equality
 */



// Use same includes as other harnesses
// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_IMPLEMENTATION

// Simple base64 encoding for buffer data
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

// Create minimal GLTF with asset information
std::string create_minimal_gltf(FuzzedDataProvider& fdp, const std::string& asset_name) {
    std::string generator = fdp.ConsumeRandomLengthString(32);
    if (generator.empty()) generator = "TestHarness_010";
    
    char buffer[512];
    snprintf(buffer, sizeof(buffer),
        "{\n"
        "  \"asset\": {\n"
        "    \"version\": \"2.0\",\n"
        "    \"generator\": \"%s\"\n"
        "  }\n"
        "}", generator.c_str());
    return std::string(buffer);
}

// Create GLTF with buffer data for testing Buffer equality
std::string create_gltf_with_buffer(FuzzedDataProvider& fdp, const std::string& buffer_name, 
                                   size_t buffer_size, const uint8_t* buffer_data) {
    std::string base64_data = base64_encode(buffer_data, buffer_size);
    
    char buffer_json[2048];
    snprintf(buffer_json, sizeof(buffer_json),
        "{\n"
        "  \"asset\": {\"version\": \"2.0\"},\n"
        "  \"buffers\": [\n"
        "    {\n"
        "      \"name\": \"%s\",\n"
        "      \"uri\": \"data:application/octet-stream;base64,%s\",\n"
        "      \"byteLength\": %zu\n"
        "    }\n"
        "  ]\n"
        "}", buffer_name.c_str(), base64_data.c_str(), buffer_size);
    return std::string(buffer_json);
}
// Create GLTF with texture data for testing Texture equality
std::string create_gltf_with_texture(FuzzedDataProvider& fdp, const std::string& texture_name,
                                    const std::string& image_name, int sampler_index) {
    // Generate minimal PNG-like data from fuzzer input
    // Use a small fixed header and variable body from fuzzer input
    size_t image_data_size = fdp.ConsumeIntegralInRange<size_t>(16, 128);
    std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_data_size);
    
    // Ensure we have at least some data
    if (image_data.empty()) {
        image_data.push_back(0x89); image_data.push_back('P'); image_data.push_back('N'); image_data.push_back('G');
        image_data.push_back(0x0D); image_data.push_back(0x0A); image_data.push_back(0x1A); image_data.push_back(0x0A);
    }
    
    std::string base64_image = base64_encode(image_data.data(), image_data.size());
    
    char buffer[4096];
    snprintf(buffer, sizeof(buffer),
        "{\n"
        "  \"asset\": {\"version\": \"2.0\"},\n"
        "  \"images\": [\n"
        "    {\n"
        "      \"name\": \"%s\",\n"
        "      \"uri\": \"data:image/png;base64,%s\"\n"
        "    }\n"
        "  ],\n"
        "  \"textures\": [\n"
        "    {\n"
        "      \"name\": \"%s\",\n"
        "      \"source\": 0,\n"
        "      \"sampler\": %d\n"
        "    }\n"
        "  ],\n"
        "  \"samplers\": [\n"
        "    {\n"
        "      \"magFilter\": 9729,\n"
        "      \"minFilter\": 9987\n"
        "    }\n"
        "  ]\n"
        "}", image_name.c_str(), base64_image.c_str(), texture_name.c_str(), sampler_index);
    return std::string(buffer);
}
// Create GLTF with audio emitter for testing PositionalEmitter equality
std::string create_gltf_with_audio_emitter(FuzzedDataProvider& fdp, const std::string& emitter_name,
                                          double cone_inner, double cone_outer, double cone_gain) {
    char buffer[3072];
    snprintf(buffer, sizeof(buffer),
        "{\n"
        "  \"asset\": {\"version\": \"2.0\"},\n"
        "  \"extensions\": {\n"
        "    \"KHR_audio\": {\n"
        "      \"emitters\": [\n"
        "        {\n"
        "          \"name\": \"%s\",\n"
        "          \"type\": \"positional\",\n"
        "          \"positional\": {\n"
        "            \"coneInnerAngle\": %.6f,\n"
        "            \"coneOuterAngle\": %.6f,\n"
        "            \"coneOuterGain\": %.6f\n"
        "          }\n"
        "        }\n"
        "      ]\n"
        "    }\n"
        "  },\n"
        "  \"nodes\": [\n"
        "    {\n"
        "      \"extensions\": {\n"
        "        \"KHR_audio\": {\n"
        "          \"emitter\": 0\n"
        "        }\n"
        "      }\n"
        "    }\n"
        "  ]\n"
        "}", emitter_name.c_str(), cone_inner, cone_outer, cone_gain);
    return std::string(buffer);
}

// Test 1: Comprehensive Model equality testing with identical models
static void test_model_equality_identical(FuzzedDataProvider& fdp) {
    // Create identical GLTF models using fuzzer input
    std::string gltf_json = create_minimal_gltf(fdp, "TestAsset");
    
    tinygltf::Model model1, model2;
    tinygltf::TinyGLTF ctx;
    std::string err1, warn1, err2, warn2;
    
    // Load identical models
    bool ret1 = ctx.LoadASCIIFromString(&model1, &err1, &warn1, gltf_json.c_str(),
                                       static_cast<unsigned int>(gltf_json.size()),
                                       /* base_dir */ "");
    bool ret2 = ctx.LoadASCIIFromString(&model2, &err2, &warn2, gltf_json.c_str(),
                                       static_cast<unsigned int>(gltf_json.size()),
                                       /* base_dir */ "");
    
    if (ret1 && ret2) {
        // Test Model equality operator (primary target - 199 branches)
        bool models_equal = (model1 == model2);
        (void)models_equal;
        
        // Test self-equality
        bool self_equal1 = (model1 == model1);
        bool self_equal2 = (model2 == model2);
        (void)self_equal1;
        (void)self_equal2;
    }
}

// Test 2: Model equality testing with differing models
static void test_model_equality_differing(FuzzedDataProvider& fdp) {
    // Create two different GLTF models
    std::string gltf1 = create_minimal_gltf(fdp, "Model1");
    
    // Consume some bytes to make input different for second model
    fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(1, 16));
    std::string gltf2 = create_minimal_gltf(fdp, "Model2");
    
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
        // Test Model equality operator with differing models
        bool models_equal = (model1 == model2);
        (void)models_equal;
    }
}

// Test 3: Buffer equality testing
static void test_buffer_equality(FuzzedDataProvider& fdp) {
    // Create buffer data from fuzzer input
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(16, 256);
    std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
    
    if (buffer_data.size() < 16) {
        // Pad with zeros if needed
        buffer_data.resize(16, 0);
    }
    
    // Create GLTF with identical buffers
    std::string gltf_json = create_gltf_with_buffer(fdp, "TestBuffer", buffer_data.size(), buffer_data.data());
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err, warn;
    
    bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, gltf_json.c_str(),
                                      static_cast<unsigned int>(gltf_json.size()),
                                      /* base_dir */ "");
    
    if (ret && !model.buffers.empty()) {
        // Test Buffer equality operator (secondary target - 14 branches)
        tinygltf::Buffer& buffer = model.buffers[0];
        bool buffer_self_equal = (buffer == buffer);
        (void)buffer_self_equal;
        
        // Create a copy and test equality
        tinygltf::Buffer buffer_copy = buffer;
        bool buffers_equal = (buffer == buffer_copy);
        (void)buffers_equal;
    }
}

// Test 4: Texture equality testing
static void test_texture_equality(FuzzedDataProvider& fdp) {
    // Create GLTF with texture
    std::string texture_name = fdp.ConsumeRandomLengthString(16);
    std::string image_name = fdp.ConsumeRandomLengthString(16);
    int sampler_index = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    if (texture_name.empty()) texture_name = "texture1";
    if (image_name.empty()) image_name = "image1";
    
    std::string gltf_json = create_gltf_with_texture(fdp, texture_name, image_name, sampler_index);
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err, warn;
    
    bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, gltf_json.c_str(),
                                      static_cast<unsigned int>(gltf_json.size()),
                                      /* base_dir */ "");
    
    if (ret && !model.textures.empty()) {
        // Test Texture equality operator (secondary target - 14 branches)
        tinygltf::Texture& texture = model.textures[0];
        bool texture_self_equal = (texture == texture);
        (void)texture_self_equal;
        
        // Create a copy and test equality
        tinygltf::Texture texture_copy = texture;
        bool textures_equal = (texture == texture_copy);
        (void)textures_equal;
    }
}

// Test 5: PositionalEmitter equality testing via audio emitters
static void test_positional_emitter_equality(FuzzedDataProvider& fdp) {
    // Create GLTF with audio emitter containing PositionalEmitter
    std::string emitter_name = fdp.ConsumeRandomLengthString(16);
    double cone_inner = fdp.ConsumeFloatingPointInRange<double>(0.0, 6.283185307179586);
    double cone_outer = fdp.ConsumeFloatingPointInRange<double>(cone_inner, 6.283185307179586);
    double cone_gain = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    
    if (emitter_name.empty()) emitter_name = "emitter1";
    
    std::string gltf_json = create_gltf_with_audio_emitter(fdp, emitter_name, cone_inner, cone_outer, cone_gain);
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err, warn;
    
    // Note: This requires KHR_audio extension support
    // Even if loading fails, we exercise error paths
    bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, gltf_json.c_str(),
                                      static_cast<unsigned int>(gltf_json.size()),
                                      /* base_dir */ "");
    
    // If we have audio emitters with positional data, test equality
    if (ret && !model.audioEmitters.empty()) {
        tinygltf::AudioEmitter& audio_emitter = model.audioEmitters[0];
        
        // Test AudioEmitter equality (which internally tests PositionalEmitter equality)
        bool emitter_self_equal = (audio_emitter == audio_emitter);
        (void)emitter_self_equal;
        
        // Create a copy and test equality
        tinygltf::AudioEmitter emitter_copy = audio_emitter;
        bool emitters_equal = (audio_emitter == emitter_copy);
        (void)emitters_equal;
    }
}

// Test 6: Comprehensive equality testing across all target types
static void test_comprehensive_equality(FuzzedDataProvider& fdp) {
    // Create a complex GLTF with multiple components
    std::string gltf_json = create_minimal_gltf(fdp, "ComprehensiveTest");
    
    tinygltf::Model model1, model2;
    tinygltf::TinyGLTF ctx;
    std::string err1, warn1, err2, warn2;
    
    // Load identical models
    bool ret1 = ctx.LoadASCIIFromString(&model1, &err1, &warn1, gltf_json.c_str(),
                                       static_cast<unsigned int>(gltf_json.size()),
                                       /* base_dir */ "");
    bool ret2 = ctx.LoadASCIIFromString(&model2, &err2, &warn2, gltf_json.c_str(),
                                       static_cast<unsigned int>(gltf_json.size()),
                                       /* base_dir */ "");
    
    if (ret1 && ret2) {
        // 1. Test Model equality (primary target)
        bool model_equality = (model1 == model2);
        (void)model_equality;
        
        // 2. Test equality of various components if they exist
        
        // Test Buffer equality if buffers exist
        if (!model1.buffers.empty() && !model2.buffers.empty()) {
            for (size_t i = 0; i < std::min(model1.buffers.size(), model2.buffers.size()); ++i) {
                bool buffer_eq = (model1.buffers[i] == model2.buffers[i]);
                (void)buffer_eq;
            }
        }
        
        // Test Texture equality if textures exist
        if (!model1.textures.empty() && !model2.textures.empty()) {
            for (size_t i = 0; i < std::min(model1.textures.size(), model2.textures.size()); ++i) {
                bool texture_eq = (model1.textures[i] == model2.textures[i]);
                (void)texture_eq;
            }
        }
        
        // Test AudioEmitter equality if audio emitters exist
        if (!model1.audioEmitters.empty() && !model2.audioEmitters.empty()) {
            for (size_t i = 0; i < std::min(model1.audioEmitters.size(), model2.audioEmitters.size()); ++i) {
                bool audio_emitter_eq = (model1.audioEmitters[i] == model2.audioEmitters[i]);
                (void)audio_emitter_eq;
            }
        }
    }
}

// Test 7: Binary model loading and equality testing
static void test_binary_model_equality(FuzzedDataProvider& fdp) {
    // Create binary GLTF data (simplified - just use ASCII as binary for testing)
    std::string gltf_json = create_minimal_gltf(fdp, "BinaryTest");
    
    // Convert to binary-like data
    std::vector<uint8_t> binary_data(gltf_json.begin(), gltf_json.end());
    
    tinygltf::Model model1, model2;
    tinygltf::TinyGLTF ctx;
    std::string err1, warn1, err2, warn2;
    
    // Load from binary memory (using ASCII data as binary for testing)
    bool ret1 = ctx.LoadBinaryFromMemory(&model1, &err1, &warn1, binary_data.data(),
                                        static_cast<unsigned int>(binary_data.size()),
                                        /* base_dir */ "");
    
    // Load second model from same binary data
    bool ret2 = ctx.LoadBinaryFromMemory(&model2, &err2, &warn2, binary_data.data(),
                                        static_cast<unsigned int>(binary_data.size()),
                                        /* base_dir */ "");
    
    if (ret1 && ret2) {
        // Test Model equality operator with binary-loaded models
        bool models_equal = (model1 == model2);
        (void)models_equal;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    // We need enough data for: GLTF creation + various parameters + buffer data
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector from fuzzer input
    uint8_t operation = 0;
    if (fdp.remaining_bytes() >= 1) {
        operation = fdp.ConsumeIntegral<uint8_t>() % 7;
    }
    
    // Execute different equality tests based on operation selector
    switch (operation) {
        case 0:
            test_model_equality_identical(fdp);
            break;
        case 1:
            test_model_equality_differing(fdp);
            break;
        case 2:
            test_buffer_equality(fdp);
            break;
        case 3:
            test_texture_equality(fdp);
            break;
        case 4:
            test_positional_emitter_equality(fdp);
            break;
        case 5:
            test_comprehensive_equality(fdp);
            break;
        case 6:
            test_binary_model_equality(fdp);
            break;
        default:
            // Default to comprehensive test
            test_comprehensive_equality(fdp);
            break;
    }
    
    return 0;
}

} // namespace harness_010
