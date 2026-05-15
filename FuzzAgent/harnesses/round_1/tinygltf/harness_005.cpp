#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <sstream>
#include <memory>
#include <cmath>

namespace harness_005 {
/*
 * Fuzzing harness for tinygltf library JSON number formatting subsystem
 * 
 * This harness specifically targets JSON floating-point number formatting functions
 * with 206+ undiscovered branches across 6 core dtoa_impl functions:
 * - _ZN8nlohmann6detail9dtoa_impl6grisu2EPcRiS3_NS1_5diyfpES4_S4_ (76 undiscovered branches)
 * - _ZN8nlohmann6detail9dtoa_impl16grisu2_digit_genEPcRiS3_NS1_5diyfpES4_S4_ (60 undiscovered branches)
 * - _ZN8nlohmann6detail9dtoa_impl13format_bufferEPciiii (30 undiscovered branches)
 * - _ZN8nlohmann6detail9dtoa_impl18find_largest_pow10EjRj (18 undiscovered branches)
 * - _ZN8nlohmann6detail9dtoa_impl36get_cached_power_for_binary_exponentEi (12 undiscovered branches)
 * - _ZN8nlohmann6detail9dtoa_impl15append_exponentEPci (10 undiscovered branches)
 * 
 * The harness implements comprehensive numeric data generation to trigger
 * floating-point formatting code paths during JSON serialization:
 * 1. Create tinygltf::Model populated with diverse numeric data
 * 2. Include floating-point values in various ranges (very small, normal, very large)
 * 3. Test integers, arrays of mixed numeric types
 * 4. Include camera perspectives with float parameters
 * 5. Include material properties with float values
 * 6. Call WriteGltfSceneToFile or WriteGltfSceneToStream to serialize model to JSON
 * 7. Optionally reload and verify via LoadASCIIFromString
 */



// Mock filesystem for fuzzing - stores files in memory
class MockNumericFileSystem {
public:
    struct FileInfo {
        std::vector<unsigned char> data;
        bool exists;
    };
    
    std::map<std::string, FileInfo> files;
    std::map<std::string, std::stringstream> streams;
    void* user_data;
    
    MockNumericFileSystem() : user_data(nullptr) {}
    
    // FileExists callback implementation
    static bool MockFileExists(const std::string& abs_filename, void* user_data) {
        MockNumericFileSystem* fs = static_cast<MockNumericFileSystem*>(user_data);
        if (!fs) return false;
        
        auto it = fs->files.find(abs_filename);
        return it != fs->files.end() && it->second.exists;
    }
    
    // ExpandFilePath callback implementation (no-op for mocking)
    static std::string MockExpandFilePath(const std::string& filepath, void* user_data) {
        // Return path as-is for mocking
        return filepath;
    }
    
    // ReadWholeFile callback implementation
    static bool MockReadWholeFile(std::vector<unsigned char>* out, 
                                  std::string* err, 
                                  const std::string& filepath, 
                                  void* user_data) {
        MockNumericFileSystem* fs = static_cast<MockNumericFileSystem*>(user_data);
        if (!fs) {
            if (err) *err = "No filesystem context";
            return false;
        }
        
        auto it = fs->files.find(filepath);
        if (it == fs->files.end() || !it->second.exists) {
            if (err) *err = "File not found: " + filepath;
            return false;
        }
        
        *out = it->second.data;
        return true;
    }
    
    // WriteWholeFile callback implementation  
    static bool MockWriteWholeFile(std::string* err, 
                                   const std::string& filepath,
                                   const std::vector<unsigned char>& contents,
                                   void* user_data) {
        MockNumericFileSystem* fs = static_cast<MockNumericFileSystem*>(user_data);
        if (!fs) {
            if (err) *err = "No filesystem context";
            return false;
        }
        
        FileInfo info;
        info.data = contents;
        info.exists = true;
        fs->files[filepath] = info;
        return true;
    }
    
    // GetFileSizeInBytes callback implementation
    static bool MockGetFileSizeInBytes(size_t* filesize_out,
                                       std::string* err,
                                       const std::string& filepath,
                                       void* user_data) {
        MockNumericFileSystem* fs = static_cast<MockNumericFileSystem*>(user_data);
        if (!fs) {
            if (err) *err = "No filesystem context";
            return false;
        }
        
        auto it = fs->files.find(filepath);
        if (it == fs->files.end() || !it->second.exists) {
            if (err) *err = "File not found: " + filepath;
            return false;
        }
        
        *filesize_out = it->second.data.size();
        return true;
    }

    
    // Setup filesystem callbacks
    tinygltf::FsCallbacks GetFsCallbacks() {
        tinygltf::FsCallbacks callbacks;
        callbacks.FileExists = &MockNumericFileSystem::MockFileExists;
        callbacks.ExpandFilePath = &MockNumericFileSystem::MockExpandFilePath;
        callbacks.ReadWholeFile = &MockNumericFileSystem::MockReadWholeFile;
        callbacks.WriteWholeFile = &MockNumericFileSystem::MockWriteWholeFile;
        callbacks.GetFileSizeInBytes = &MockNumericFileSystem::MockGetFileSizeInBytes;
        callbacks.user_data = this;
        return callbacks;
    }
};


// Helper function to generate diverse numeric values for testing floating-point formatting
void populateModelWithDiverseNumericData(tinygltf::Model& model, FuzzedDataProvider& fdp) {
    // Clear existing data
    model = tinygltf::Model();
    
    // Set basic asset information
    model.asset.version = "2.0";
    model.asset.generator = "Fuzzer-generated numeric test model";


    // 1. Accessors with diverse numeric component types and values
    for (int i = 0; i < fdp.ConsumeIntegralInRange<int>(1, 5) && fdp.remaining_bytes() > 0; ++i) {
        tinygltf::Accessor accessor;
        accessor.name = "accessor_" + std::to_string(i);
        
        // Choose component type (various numeric types)
        uint8_t comp_type_selector = fdp.ConsumeIntegral<uint8_t>() % 6;
        switch (comp_type_selector) {
            case 0: accessor.componentType = TINYGLTF_COMPONENT_TYPE_BYTE; break;
            case 1: accessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE; break;
            case 2: accessor.componentType = TINYGLTF_COMPONENT_TYPE_SHORT; break;
            case 3: accessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT; break;
            case 4: accessor.componentType = TINYGLTF_COMPONENT_TYPE_INT; break;
            case 5: accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT; break;
        }
        
        // Choose data type (SCALAR, VEC2, VEC3, VEC4, MAT2, MAT3, MAT4)
        uint8_t type_selector = fdp.ConsumeIntegral<uint8_t>() % 7;
        switch (type_selector) {
            case 0: accessor.type = TINYGLTF_TYPE_SCALAR; break;
            case 1: accessor.type = TINYGLTF_TYPE_VEC2; break;
            case 2: accessor.type = TINYGLTF_TYPE_VEC3; break;
            case 3: accessor.type = TINYGLTF_TYPE_VEC4; break;
            case 4: accessor.type = TINYGLTF_TYPE_MAT2; break;
            case 5: accessor.type = TINYGLTF_TYPE_MAT3; break;
            case 6: accessor.type = TINYGLTF_TYPE_MAT4; break;
        }
        
        accessor.count = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        accessor.normalized = fdp.ConsumeBool();
        
        // Generate min/max values with diverse floating-point numbers
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(double) * 2) {
            // Very small numbers (subnormal range)
            double min_val = fdp.ConsumeFloatingPointInRange<double>(1e-308, 1e-100);
            double max_val = fdp.ConsumeFloatingPointInRange<double>(min_val + 1e-100, 1e-50);
            
            accessor.minValues.push_back(min_val);
            accessor.maxValues.push_back(max_val);
            
            // Add more values for VEC types
            if (accessor.type == TINYGLTF_TYPE_VEC2 || accessor.type == TINYGLTF_TYPE_VEC3 || 
                accessor.type == TINYGLTF_TYPE_VEC4) {
                for (int j = 1; j < (accessor.type - TINYGLTF_TYPE_SCALAR); ++j) {
                    if (fdp.remaining_bytes() >= sizeof(double) * 2) {
                        double val1 = fdp.ConsumeFloatingPointInRange<double>(1e-200, 1e-100);
                        double val2 = fdp.ConsumeFloatingPointInRange<double>(val1 + 1e-100, 1e-50);
                        accessor.minValues.push_back(val1);
                        accessor.maxValues.push_back(val2);
                    }
                }
            }
        }
        
        model.accessors.push_back(accessor);
    }
    
    // 2. Materials with diverse floating-point properties
    for (int i = 0; i < fdp.ConsumeIntegralInRange<int>(1, 4) && fdp.remaining_bytes() > 0; ++i) {
        tinygltf::Material material;
        material.name = "material_" + std::to_string(i);
        
        // Generate diverse floating-point values for material properties
        // These will trigger floating-point formatting during JSON serialization
        
        // Emissive factor (3 floats)
        material.emissiveFactor[0] = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        material.emissiveFactor[1] = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        material.emissiveFactor[2] = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        
        // Alpha cutoff (various ranges including very small values)
        double alpha_cutoff = fdp.ConsumeFloatingPoint<double>();
        // Normalize to reasonable range for alpha cutoff
        material.alphaCutoff = std::fmod(std::abs(alpha_cutoff), 1.0);
        
        // PBR metallic roughness properties
        material.pbrMetallicRoughness.baseColorFactor.resize(4);
        for (int j = 0; j < 4 && fdp.remaining_bytes() >= sizeof(float); ++j) {
            // Generate diverse floating-point values including very small numbers
            double val = fdp.ConsumeFloatingPoint<double>();
            material.pbrMetallicRoughness.baseColorFactor[j] = std::fmod(std::abs(val), 1.0);
        }
        
        material.pbrMetallicRoughness.metallicFactor = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        material.pbrMetallicRoughness.roughnessFactor = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        
        // Choose alpha mode based on fuzzed input
        uint8_t alpha_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (alpha_mode) {
            case 0: material.alphaMode = "OPAQUE"; break;
            case 1: material.alphaMode = "MASK"; break;
            case 2: material.alphaMode = "BLEND"; break;
        }
        
        material.doubleSided = fdp.ConsumeBool();
        
        model.materials.push_back(material);
    }
    
    // 3. Cameras with diverse floating-point parameters
    for (int i = 0; i < fdp.ConsumeIntegralInRange<int>(1, 3) && fdp.remaining_bytes() > 0; ++i) {
        tinygltf::Camera camera;
        camera.name = "camera_" + std::to_string(i);
        
        // Choose camera type
        if (fdp.ConsumeBool()) {
            camera.type = "perspective";
            
            // Generate diverse floating-point values for perspective camera
            // These include very small, normal, and very large values
            camera.perspective.aspectRatio = fdp.ConsumeFloatingPointInRange<double>(0.1, 10.0);
            
            // Yfov with diverse values
            double yfov = fdp.ConsumeFloatingPoint<double>();
            camera.perspective.yfov = std::fmod(std::abs(yfov), 3.14159 * 2.0);
            
            // Znear and zfar with diverse ranges including very small values
            camera.perspective.znear = fdp.ConsumeFloatingPointInRange<double>(0.001, 100.0);
            camera.perspective.zfar = fdp.ConsumeFloatingPointInRange<double>(
                camera.perspective.znear + 0.1, 10000.0);
        } else {
            camera.type = "orthographic";
            
            // Generate diverse floating-point values for orthographic camera
            camera.orthographic.xmag = fdp.ConsumeFloatingPointInRange<double>(0.1, 100.0);
            camera.orthographic.ymag = fdp.ConsumeFloatingPointInRange<double>(0.1, 100.0);
            camera.orthographic.znear = fdp.ConsumeFloatingPointInRange<double>(0.001, 10.0);
            camera.orthographic.zfar = fdp.ConsumeFloatingPointInRange<double>(
                camera.orthographic.znear + 0.1, 1000.0);
        }
        
        model.cameras.push_back(camera);
    }
    
    // 4. Nodes with transformation matrices containing diverse floating-point values
    for (int i = 0; i < fdp.ConsumeIntegralInRange<int>(1, 4) && fdp.remaining_bytes() > 0; ++i) {
        tinygltf::Node node;
        node.name = "node_" + std::to_string(i);
        
        // Generate transformation arrays with diverse floating-point values
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(double) * 4) {
            // Rotation quaternion (4 doubles)
            for (int j = 0; j < 4; ++j) {
                double rot_val = fdp.ConsumeFloatingPoint<double>();
                node.rotation.push_back(std::fmod(rot_val, 1.0));
            }
        }
        
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(double) * 3) {
            // Scale vector (3 doubles)
            for (int j = 0; j < 3; ++j) {
                double scale_val = fdp.ConsumeFloatingPointInRange<double>(0.01, 10.0);
                node.scale.push_back(scale_val);
            }
        }
        
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(double) * 3) {
            // Translation vector (3 doubles)
            for (int j = 0; j < 3; ++j) {
                double trans_val = fdp.ConsumeFloatingPointInRange<double>(-100.0, 100.0);
                node.translation.push_back(trans_val);
            }
        }
        
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(double) * 16) {
            // Matrix (16 doubles) - includes diverse floating-point patterns
            for (int j = 0; j < 16; ++j) {
                double matrix_val = fdp.ConsumeFloatingPoint<double>();
                node.matrix.push_back(matrix_val);
            }
        }
        
        // Assign camera if available
        if (!model.cameras.empty()) {
            node.camera = fdp.ConsumeIntegralInRange<int>(0, static_cast<int>(model.cameras.size()) - 1);
        }
        
        model.nodes.push_back(node);
    }
    
    // 5. Scenes
    if (!model.nodes.empty()) {
        tinygltf::Scene scene;
        scene.name = "fuzzer_scene";
        
        // Add some nodes to the scene
        int num_nodes = std::min(static_cast<int>(model.nodes.size()), 
                                 fdp.ConsumeIntegralInRange<int>(1, 3));
        for (int i = 0; i < num_nodes; ++i) {
            scene.nodes.push_back(i);
        }
        
        model.scenes.push_back(scene);
        model.defaultScene = 0;
    }
    
    // 6. Buffers and BufferViews (minimal for valid glTF)
    tinygltf::Buffer buffer;
    buffer.name = "fuzzer_buffer";
    buffer.data.resize(fdp.ConsumeIntegralInRange<size_t>(1, 100));
    for (size_t i = 0; i < buffer.data.size() && fdp.remaining_bytes() > 0; ++i) {
        buffer.data[i] = fdp.ConsumeIntegral<uint8_t>();
    }
    model.buffers.push_back(buffer);
    
    if (!model.accessors.empty() && !model.buffers.empty()) {
        tinygltf::BufferView bufferView;
        bufferView.name = "fuzzer_bufferview";
        bufferView.buffer = 0;
        bufferView.byteOffset = 0;
        bufferView.byteLength = buffer.data.size();
        bufferView.byteStride = 0;
        model.bufferViews.push_back(bufferView);
        
        // Update accessors to reference buffer view
        for (size_t i = 0; i < model.accessors.size(); ++i) {
            model.accessors[i].bufferView = 0;
            model.accessors[i].byteOffset = 0;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful numeric data testing
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create mock filesystem
    MockNumericFileSystem mockFs;
    
    // Set up TinyGLTF context
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    // Configure filesystem callbacks for the mock
    tinygltf::FsCallbacks fsCallbacks = mockFs.GetFsCallbacks();
    
    if (!ctx.SetFsCallbacks(fsCallbacks, &err)) {
        // Failed to set filesystem callbacks, but continue anyway
        // to test other code paths
    }
    // Create and populate model with diverse numeric data
    tinygltf::Model model;
    populateModelWithDiverseNumericData(model, fdp);
    
    // Choose which serialization method to test
    uint8_t serialization_method = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    bool serialize_result = false;
    
    switch (serialization_method) {
        case 0: {
            // Test WriteGltfSceneToFile with various configurations
            std::string output_filename = "test_output_" + std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ".gltf";
            
            // Test different combinations of serialization options
            // These will trigger different JSON formatting paths
            bool embedImages = fdp.ConsumeBool();
            bool embedBuffers = fdp.ConsumeBool();
            bool prettyPrint = fdp.ConsumeBool();
            bool writeBinary = fdp.ConsumeBool();
            
            serialize_result = ctx.WriteGltfSceneToFile(&model, output_filename,
                                                       embedImages, embedBuffers,
                                                       prettyPrint, writeBinary);
            break;
        }
        
        case 1: {
            // Test WriteGltfSceneToStream (ASCII)
            std::stringstream stream;
            bool prettyPrint = fdp.ConsumeBool();
            bool writeBinary = false; // ASCII mode
            
            serialize_result = ctx.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);
            
            // Optionally try to reload the serialized JSON to verify
            if (serialize_result && fdp.ConsumeBool()) {
                std::string serialized_json = stream.str();
                if (!serialized_json.empty()) {
                    tinygltf::Model reloaded_model;
                    std::string reload_err;
                    std::string reload_warn;
                    
                    // Try to reload the serialized JSON
                    bool reload_result = ctx.LoadASCIIFromString(&reloaded_model, &reload_err, &reload_warn,
                                                                serialized_json.c_str(),
                                                                static_cast<unsigned int>(serialized_json.size()),
                                                                /* base_dir */ "",
                                                                tinygltf::REQUIRE_VERSION);
                    (void)reload_result; // Result intentionally ignored
                }
            }
            break;
        }
        
        case 2: {
            // Test WriteGltfSceneToStream (Binary GLB)
            std::stringstream stream;
            bool prettyPrint = fdp.ConsumeBool();
            bool writeBinary = true; // Binary mode
            
            serialize_result = ctx.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);
            break;
        }
        
        case 3: {
            // Test multiple serialization methods in sequence
            std::string output_filename = "test_multi_" + std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ".gltf";
            
            // First ASCII file
            bool result1 = ctx.WriteGltfSceneToFile(&model, output_filename,
                                                   false, false, true, false);
            
            // Then binary file
            std::string binary_filename = "test_multi_" + std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ".glb";
            bool result2 = ctx.WriteGltfSceneToFile(&model, binary_filename,
                                                   false, false, false, true);
            
            // Then ASCII stream
            std::stringstream ascii_stream;
            bool result3 = ctx.WriteGltfSceneToStream(&model, ascii_stream, true, false);
            
            // Then binary stream
            std::stringstream binary_stream;
            bool result4 = ctx.WriteGltfSceneToStream(&model, binary_stream, false, true);
            
            serialize_result = result1 || result2 || result3 || result4;
            break;
        }
    }
    
    // Clean up mock files
    mockFs.files.clear();
    
    // Return 0 regardless of serialization result to allow testing error paths
    return 0;
}

} // namespace harness_005
