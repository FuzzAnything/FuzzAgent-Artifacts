#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iostream>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_012 {
// Harness 012: JSON floating-point serialization fuzzer for tinygltf
// Targets dtoa_impl functions through WriteGltfSceneToFile/Stream APIs
// Primary uncovered functions: nlohmann::detail::dtoa_impl::grisu2 (76 branches),
// grisu2_digit_gen (60 branches), format_buffer (30 branches) - total 166+ undiscovered branches
// Entry points: WriteGltfSceneToFile (167 undiscovered branches), WriteGltfSceneToStream (130 undiscovered branches)
// Based on coverage guidance in harness_012_guidance.md
// Uses FuzzedDataProvider for structured input splitting to test floating-point serialization


// Include FuzzedDataProvider for structured input splitting

// Include tinygltf header

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for model loading and float injection
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create TinyGLTF loader context
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err, warn;
    
    // Consume operation selector (0-2 for different serialization paths)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume check_sections flag for loading
    unsigned int check_sections = fdp.ConsumeIntegral<uint8_t>() & 0x7F;
    
    // Create a minimal GLTF model for testing
    // We'll create a simple model with cameras, materials, and animations
    // that contain various floating-point values
    
    // 1. Initialize basic model structure
    model.asset.version = "2.0";
    model.asset.generator = "Fuzzer Harness 012";
    
    // 2. Inject floating-point values into cameras
    // Camera 1: Perspective camera with various float values
    {
        tinygltf::Camera cam;
        cam.name = "PerspectiveCamera";
        cam.type = "perspective";
        
        // Generate special floating-point values
        double aspectRatio = 0.0;
        double yfov = 0.0;
        double zfar = 0.0;
        double znear = 0.0;
        
        // Consume float values from fuzzer input
        uint8_t float_selector = fdp.ConsumeIntegral<uint8_t>() % 8;
        switch (float_selector) {
            case 0: // Normal floats
                aspectRatio = fdp.ConsumeFloatingPoint<double>();
                yfov = fdp.ConsumeFloatingPoint<double>();
                zfar = fdp.ConsumeFloatingPoint<double>();
                znear = fdp.ConsumeFloatingPoint<double>();
                break;
            case 1: // Very small floats
                aspectRatio = fdp.ConsumeFloatingPointInRange<double>(std::numeric_limits<double>::min(), 1e-100);
                yfov = fdp.ConsumeFloatingPointInRange<double>(std::numeric_limits<double>::min(), 1e-100);
                zfar = fdp.ConsumeFloatingPointInRange<double>(std::numeric_limits<double>::min(), 1e-100);
                znear = fdp.ConsumeFloatingPointInRange<double>(std::numeric_limits<double>::min(), 1e-100);
                break;
            case 2: // Very large floats
                aspectRatio = fdp.ConsumeFloatingPointInRange<double>(1e100, std::numeric_limits<double>::max());
                yfov = fdp.ConsumeFloatingPointInRange<double>(1e100, std::numeric_limits<double>::max());
                zfar = fdp.ConsumeFloatingPointInRange<double>(1e100, std::numeric_limits<double>::max());
                znear = fdp.ConsumeFloatingPointInRange<double>(1e100, std::numeric_limits<double>::max());
                break;
            case 3: // Denormal floats (very close to zero)
                aspectRatio = std::numeric_limits<double>::denorm_min();
                yfov = std::numeric_limits<double>::denorm_min();
                zfar = std::numeric_limits<double>::denorm_min();
                znear = std::numeric_limits<double>::denorm_min();
                break;
            case 4: // Infinity
                aspectRatio = std::numeric_limits<double>::infinity();
                yfov = -std::numeric_limits<double>::infinity();
                zfar = std::numeric_limits<double>::infinity();
                znear = -std::numeric_limits<double>::infinity();
                break;
            case 5: // NaN
                aspectRatio = std::numeric_limits<double>::quiet_NaN();
                yfov = std::numeric_limits<double>::signaling_NaN();
                zfar = std::numeric_limits<double>::quiet_NaN();
                znear = std::numeric_limits<double>::signaling_NaN();
                break;
            case 6: // Mixed special values
                aspectRatio = std::numeric_limits<double>::infinity();
                yfov = std::numeric_limits<double>::quiet_NaN();
                zfar = -std::numeric_limits<double>::infinity();
                znear = std::numeric_limits<double>::denorm_min();
                break;
            case 7: // Arrays of floats in extras
                // Leave default values, will set in extras
                break;
        }
        
        cam.perspective.aspectRatio = aspectRatio;
        cam.perspective.yfov = yfov;
        cam.perspective.zfar = zfar;
        cam.perspective.znear = znear;
        
        // Add some float arrays to extras
        if (float_selector == 7 && fdp.remaining_bytes() >= 32) {
            std::vector<double> float_array;
            size_t array_size = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            for (size_t i = 0; i < array_size && fdp.remaining_bytes() >= 8; i++) {
                float_array.push_back(fdp.ConsumeFloatingPoint<double>());
            }
            // Note: Would need to create JSON array in extras - simplified for now
        }
        
        model.cameras.push_back(cam);
    }
    
    // Camera 2: Orthographic camera with float values
    {
        tinygltf::Camera cam;
        cam.name = "OrthographicCamera";
        cam.type = "orthographic";
        
        // Consume orthographic camera parameters
        cam.orthographic.xmag = fdp.ConsumeFloatingPoint<double>();
        cam.orthographic.ymag = fdp.ConsumeFloatingPoint<double>();
        cam.orthographic.zfar = fdp.ConsumeFloatingPoint<double>();
        cam.orthographic.znear = fdp.ConsumeFloatingPoint<double>();
        
        model.cameras.push_back(cam);
    }
    
    // 3. Inject floating-point values into materials (metallic/roughness)
    {
        tinygltf::Material mat;
        mat.name = "PBRMaterial";
        
        // PBR metallic-roughness values
        mat.pbrMetallicRoughness.baseColorFactor = {
            fdp.ConsumeFloatingPoint<double>(),  // R
            fdp.ConsumeFloatingPoint<double>(),  // G
            fdp.ConsumeFloatingPoint<double>(),  // B
            fdp.ConsumeFloatingPoint<double>()   // A
        };
        
        // Metallic and roughness factors
        mat.pbrMetallicRoughness.metallicFactor = fdp.ConsumeFloatingPoint<double>();
        mat.pbrMetallicRoughness.roughnessFactor = fdp.ConsumeFloatingPoint<double>();
        
        // Emissive factor (array of 3 floats)
        mat.emissiveFactor = {
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>(),
            fdp.ConsumeFloatingPoint<double>()
        };
        
        // Alpha cutoff
        mat.alphaCutoff = fdp.ConsumeFloatingPoint<double>();
        
        model.materials.push_back(mat);
    }
    
    // 4. Add a scene with a node referencing the camera
    {
        tinygltf::Scene scene;
        scene.name = "TestScene";
        
        tinygltf::Node node;
        node.name = "CameraNode";
        node.camera = 0; // Reference first camera
        
        // Add transformation with float arrays
        if (fdp.remaining_bytes() >= 32) {
            // Rotation quaternion (4 floats)
            node.rotation = {
                fdp.ConsumeFloatingPoint<double>(),
                fdp.ConsumeFloatingPoint<double>(),
                fdp.ConsumeFloatingPoint<double>(),
                fdp.ConsumeFloatingPoint<double>()
            };
            
            // Scale (3 floats)
            node.scale = {
                fdp.ConsumeFloatingPoint<double>(),
                fdp.ConsumeFloatingPoint<double>(),
                fdp.ConsumeFloatingPoint<double>()
            };
            
            // Translation (3 floats)
            node.translation = {
                fdp.ConsumeFloatingPoint<double>(),
                fdp.ConsumeFloatingPoint<double>(),
                fdp.ConsumeFloatingPoint<double>()
            };
        }
        
        model.nodes.push_back(node);
        scene.nodes.push_back(0); // Reference first node
        model.scenes.push_back(scene);
        model.defaultScene = 0;
    }
    
    // 5. Try to load additional data from fuzzer input
    // Use remaining bytes for GLTF loading attempt
    std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    if (!gltf_data.empty()) {
        // Try loading as ASCII first
        bool loaded = ctx.LoadASCIIFromString(&model, &err, &warn,
                                              reinterpret_cast<const char*>(gltf_data.data()),
                                              static_cast<unsigned int>(gltf_data.size()),
                                              "", check_sections);
        
        if (!loaded) {
            // Try as binary
            loaded = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                              gltf_data.data(),
                                              static_cast<unsigned int>(gltf_data.size()),
                                              "", check_sections);
        }
        
        // If loaded, we'll use the loaded model for serialization
        // Otherwise, use our constructed model
    }
    
    // 6. Serialize the model to trigger dtoa_impl functions
    bool serialize_success = false;
    
    if (operation == 0) {
        // Write to stream (in-memory)
        std::ostringstream oss;
        bool prettyPrint = fdp.ConsumeBool();
        bool writeBinary = fdp.ConsumeBool();
        
        serialize_success = ctx.WriteGltfSceneToStream(&model, oss, prettyPrint, writeBinary);
        
        // Use the serialized output to avoid dead code elimination
        std::string serialized = oss.str();
        if (!serialized.empty()) {
            // Touch the serialized data
            (void)serialized.size();
        }
    }
    else if (operation == 1) {
        // Write to temporary file
        std::string temp_filename = "/tmp/fuzz_gltf_012.gltf";
        
        bool embedImages = fdp.ConsumeBool();
        bool embedBuffers = fdp.ConsumeBool();
        bool prettyPrint = fdp.ConsumeBool();
        bool writeBinary = fdp.ConsumeBool();
        
        serialize_success = ctx.WriteGltfSceneToFile(&model, temp_filename, 
                                                     embedImages, embedBuffers,
                                                     prettyPrint, writeBinary);
        
        // Clean up temporary file
        std::remove(temp_filename.c_str());
    }
    else { // operation == 2
        // Test both serialization methods
        std::ostringstream oss;
        bool prettyPrint = fdp.ConsumeBool();
        bool writeBinary = fdp.ConsumeBool();
        
        bool stream_success = ctx.WriteGltfSceneToStream(&model, oss, prettyPrint, writeBinary);
        
        std::string temp_filename = "/tmp/fuzz_gltf_012_b.gltf";
        bool file_success = ctx.WriteGltfSceneToFile(&model, temp_filename,
                                                     false, false, // Don't embed
                                                     prettyPrint, writeBinary);
        
        serialize_success = stream_success || file_success;
        
        // Clean up
        std::remove(temp_filename.c_str());
    }
    
    // 7. Additional float serialization testing through extras and extensions
    // Add special float values to model extras
    if (fdp.remaining_bytes() == 0) { // Only if we have no more input
        // Create a simple JSON with special floats in extras
        // This would be more complex with actual JSON parsing
        // For now, we'll rely on the float values already injected
    }
    
    (void)serialize_success; // Use result to avoid dead code elimination
    
    return 0;
}

} // namespace harness_012
