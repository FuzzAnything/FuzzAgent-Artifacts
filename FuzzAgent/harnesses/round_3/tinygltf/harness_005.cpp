#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_005 {
/*
 * Fuzzing harness for tinygltf library targeting comprehensive GLTF component equality operators
 * Goal: Exercise 350+ undiscovered branches across major GLTF component equality operators
 * Targets:
 *   - Camera equality operator (40 branches)
 *   - Node equality operator (32 branches)
 *   - Mesh equality operator (26 branches)
 *   - Accessor equality operator (26 branches)
 *   - Image equality operator (24 branches)
 *   - BufferView equality operator (22 branches)
 *   - Sampler equality operator (18 branches)
 *   - Primitive equality operator (16 branches)
 *   - Skin equality operator (16 branches)
 * Strategy: Load GLTF models using custom filesystem callbacks, extract components,
 *           and test equality operators between identical/different/empty objects
 * Differentiation from previous harnesses:
 *   - harness_000: Basic GLTF parsing (string/memory loading)
 *   - harness_001: Image loading via embedded images
 *   - harness_002: Image writing callbacks
 *   - harness_003: Equality testing (limited scope - Model, Material, Camera, Animation, Node)
 *   - harness_004: File-based loading with stb_image targeting
 *   - harness_005: FOCUSED equality operator coverage across all major GLTF components
 */



// Use same includes as other harnesses
// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_IMPLEMENTATION

// Custom filesystem callbacks that read from in-memory file database
struct MemoryFileSystem {
    // Map from filename to file content
    std::map<std::string, std::vector<unsigned char>> files;
    
    // Track which files were accessed
    std::vector<std::string> accessed_files;
    
    void clear() {
        files.clear();
        accessed_files.clear();
    }
    
    void add_file(const std::string& filename, const std::vector<unsigned char>& data) {
        files[filename] = data;
    }
};

// Global memory filesystem for callbacks
static MemoryFileSystem g_memory_fs;

// Custom FileExists callback
bool memory_file_exists(const std::string& abs_filename, void* user_data) {
    (void)user_data;
    return g_memory_fs.files.find(abs_filename) != g_memory_fs.files.end();
}

// Custom ExpandFilePath callback - just returns path as-is for fuzzing
std::string memory_expand_file_path(const std::string& filepath, void* user_data) {
    (void)user_data;
    return filepath;
}

// Custom ReadWholeFile callback - reads from memory filesystem
bool memory_read_whole_file(std::vector<unsigned char>* out, std::string* err, 
                           const std::string& filepath, void* user_data) {
    (void)user_data;
    
    auto it = g_memory_fs.files.find(filepath);
    if (it == g_memory_fs.files.end()) {
        if (err) *err = "File not found: " + filepath;
        return false;
    }
    
    g_memory_fs.accessed_files.push_back(filepath);
    *out = it->second;
    return true;
}

// Custom WriteWholeFile callback - not needed for fuzzing but required by API
bool memory_write_whole_file(std::string* err, const std::string& filepath,
                            const std::vector<unsigned char>& contents, void* user_data) {
    (void)err; (void)filepath; (void)contents; (void)user_data;
    // Not implemented for fuzzing - return success
    return true;
}

// Custom GetFileSizeInBytes callback
bool memory_get_file_size_in_bytes(size_t* filesize_out, std::string* err,
                                   const std::string& filepath, void* user_data) {
    (void)user_data;
    
    auto it = g_memory_fs.files.find(filepath);
    if (it == g_memory_fs.files.end()) {
        if (err) *err = "File not found: " + filepath;
        return false;
    }
    
    *filesize_out = it->second.size();
    return true;
}

// Setup custom filesystem callbacks
tinygltf::FsCallbacks setup_custom_fs_callbacks() {
    tinygltf::FsCallbacks callbacks;
    callbacks.FileExists = memory_file_exists;
    callbacks.ExpandFilePath = memory_expand_file_path;
    callbacks.ReadWholeFile = memory_read_whole_file;
    callbacks.WriteWholeFile = memory_write_whole_file;
    callbacks.GetFileSizeInBytes = memory_get_file_size_in_bytes;
    callbacks.user_data = nullptr;
    return callbacks;
}

// Create minimal GLTF JSON with specified components
std::string create_minimal_gltf_json(FuzzedDataProvider& fdp, bool include_camera = false, 
                                     bool include_node = false, bool include_mesh = false,
                                     bool include_accessor = false, bool include_image = false,
                                     bool include_bufferview = false, bool include_sampler = false,
                                     bool include_primitive = false, bool include_skin = false) {
    std::string json = "{\n  \"asset\": {\n    \"version\": \"2.0\",\n    \"generator\": \"TestHarness\"\n  }";
    
    if (include_camera) {
        json += ",\n  \"cameras\": [\n    {\n      \"type\": \"perspective\",\n      \"name\": \"Camera1\",\n      \"perspective\": {\n        \"yfov\": 0.785398,\n        \"znear\": 0.01,\n        \"zfar\": 1000.0\n      }\n    }";
        // Add a second camera for equality testing
        json += ",\n    {\n      \"type\": \"orthographic\",\n      \"name\": \"Camera2\",\n      \"orthographic\": {\n        \"xmag\": 1.0,\n        \"ymag\": 1.0,\n        \"zfar\": 100.0,\n        \"znear\": 0.1\n      }\n    }";
        json += "\n  ]";
    }
    
    if (include_node) {
        json += ",\n  \"nodes\": [\n    {\n      \"name\": \"Node1\",\n      \"translation\": [1.0, 2.0, 3.0]\n    }";
        json += ",\n    {\n      \"name\": \"Node2\",\n      \"rotation\": [0.0, 0.0, 0.0, 1.0],\n      \"scale\": [1.0, 1.0, 1.0]\n    }";
        if (include_camera) {
            json += ",\n    {\n      \"name\": \"NodeWithCamera\",\n      \"camera\": 0\n    }";
        }
        json += "\n  ]";
    }
    
    if (include_mesh) {
        json += ",\n  \"meshes\": [\n    {\n      \"name\": \"Mesh1\",\n      \"primitives\": [\n        {\n          \"attributes\": {\n            \"POSITION\": 0\n          },\n          \"mode\": 4\n        }\n      ]\n    }";
        json += ",\n    {\n      \"name\": \"Mesh2\",\n      \"weights\": [1.0, 0.5]\n    }";
        json += "\n  ]";
    }
    
    if (include_accessor && include_bufferview) {
        json += ",\n  \"bufferViews\": [\n    {\n      \"buffer\": 0,\n      \"byteOffset\": 0,\n      \"byteLength\": 36\n    }\n  ]";
        json += ",\n  \"accessors\": [\n    {\n      \"bufferView\": 0,\n      \"byteOffset\": 0,\n      \"componentType\": 5126,\n      \"count\": 3,\n      \"type\": \"VEC3\",\n      \"max\": [1.0, 1.0, 1.0],\n      \"min\": [0.0, 0.0, 0.0]\n    }";
        json += ",\n    {\n      \"bufferView\": 0,\n      \"byteOffset\": 36,\n      \"componentType\": 5123,\n      \"count\": 3,\n      \"type\": \"SCALAR\",\n      \"normalized\": false\n    }";
        json += "\n  ]";
        json += ",\n  \"buffers\": [\n    {\n      \"uri\": \"data:application/octet-stream;base64,AAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAQAAAAAAAAAAAAAA\",\n      \"byteLength\": 72\n    }\n  ]";
    }
    
    if (include_image) {
        json += ",\n  \"images\": [\n    {\n      \"uri\": \"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mNkYPhfDwAChwGA60e6kgAAAABJRU5ErkJggg==\",\n      \"mimeType\": \"image/png\",\n      \"name\": \"Image1\"\n    }";
        json += ",\n    {\n      \"bufferView\": 0,\n      \"mimeType\": \"image/jpeg\",\n      \"name\": \"Image2\"\n    }";
        json += "\n  ]";
    }
    
    if (include_sampler) {
        json += ",\n  \"samplers\": [\n    {\n      \"magFilter\": 9729,\n      \"minFilter\": 9987,\n      \"wrapS\": 33648,\n      \"wrapT\": 33648,\n      \"name\": \"Sampler1\"\n    }";
        json += ",\n    {\n      \"magFilter\": 9728,\n      \"minFilter\": 9728,\n      \"wrapS\": 33071,\n      \"wrapT\": 33071,\n      \"name\": \"Sampler2\"\n    }";
        json += "\n  ]";
    }
    
    if (include_skin) {
        json += ",\n  \"skins\": [\n    {\n      \"inverseBindMatrices\": 0,\n      \"skeleton\": 0,\n      \"joints\": [0, 1],\n      \"name\": \"Skin1\"\n    }";
        json += ",\n    {\n      \"joints\": [0],\n      \"name\": \"Skin2\"\n    }";
        json += "\n  ]";
    }
    
    json += "\n}";
    return json;
}

// Test equality operators for all GLTF components
void test_component_equality_operators(tinygltf::Model& model, FuzzedDataProvider& fdp) {
    // Test Camera equality operators
    if (!model.cameras.empty()) {
        for (size_t i = 0; i < model.cameras.size(); ++i) {
            for (size_t j = 0; j < model.cameras.size(); ++j) {
                bool equal = (model.cameras[i] == model.cameras[j]);
                (void)equal; // Use result to avoid unused variable warning
                
                // Test with default constructed camera
                tinygltf::Camera default_camera;
                bool not_equal_to_default = (model.cameras[i] == default_camera);
                (void)not_equal_to_default;
                
                // Test self-equality
                bool self_equal = (model.cameras[i] == model.cameras[i]);
                (void)self_equal;
            }
        }
    }
    
    // Test Node equality operators
    if (!model.nodes.empty()) {
        for (size_t i = 0; i < model.nodes.size(); ++i) {
            for (size_t j = 0; j < model.nodes.size(); ++j) {
                bool equal = (model.nodes[i] == model.nodes[j]);
                (void)equal;
                
                // Test with default constructed node
                tinygltf::Node default_node;
                bool not_equal_to_default = (model.nodes[i] == default_node);
                (void)not_equal_to_default;
                
                // Test self-equality
                bool self_equal = (model.nodes[i] == model.nodes[i]);
                (void)self_equal;
            }
        }
    }
    
    // Test Mesh equality operators
    if (!model.meshes.empty()) {
        for (size_t i = 0; i < model.meshes.size(); ++i) {
            for (size_t j = 0; j < model.meshes.size(); ++j) {
                bool equal = (model.meshes[i] == model.meshes[j]);
                (void)equal;
                
                // Test with default constructed mesh
                tinygltf::Mesh default_mesh;
                bool not_equal_to_default = (model.meshes[i] == default_mesh);
                (void)not_equal_to_default;
                
                // Test self-equality
                bool self_equal = (model.meshes[i] == model.meshes[i]);
                (void)self_equal;
            }
        }
    }
    
    // Test Accessor equality operators
    if (!model.accessors.empty()) {
        for (size_t i = 0; i < model.accessors.size(); ++i) {
            for (size_t j = 0; j < model.accessors.size(); ++j) {
                bool equal = (model.accessors[i] == model.accessors[j]);
                (void)equal;
                
                // Test with default constructed accessor
                tinygltf::Accessor default_accessor;
                bool not_equal_to_default = (model.accessors[i] == default_accessor);
                (void)not_equal_to_default;
                
                // Test self-equality
                bool self_equal = (model.accessors[i] == model.accessors[i]);
                (void)self_equal;
            }
        }
    }
    
    // Test Image equality operators
    if (!model.images.empty()) {
        for (size_t i = 0; i < model.images.size(); ++i) {
            for (size_t j = 0; j < model.images.size(); ++j) {
                bool equal = (model.images[i] == model.images[j]);
                (void)equal;
                
                // Test with default constructed image
                tinygltf::Image default_image;
                bool not_equal_to_default = (model.images[i] == default_image);
                (void)not_equal_to_default;
                
                // Test self-equality
                bool self_equal = (model.images[i] == model.images[i]);
                (void)self_equal;
            }
        }
    }
    
    // Test BufferView equality operators
    if (!model.bufferViews.empty()) {
        for (size_t i = 0; i < model.bufferViews.size(); ++i) {
            for (size_t j = 0; j < model.bufferViews.size(); ++j) {
                bool equal = (model.bufferViews[i] == model.bufferViews[j]);
                (void)equal;
                
                // Test with default constructed bufferView
                tinygltf::BufferView default_bufferview;
                bool not_equal_to_default = (model.bufferViews[i] == default_bufferview);
                (void)not_equal_to_default;
                
                // Test self-equality
                bool self_equal = (model.bufferViews[i] == model.bufferViews[i]);
                (void)self_equal;
            }
        }
    }
    
    // Test Sampler equality operators
    if (!model.samplers.empty()) {
        for (size_t i = 0; i < model.samplers.size(); ++i) {
            for (size_t j = 0; j < model.samplers.size(); ++j) {
                bool equal = (model.samplers[i] == model.samplers[j]);
                (void)equal;
                
                // Test with default constructed sampler
                tinygltf::Sampler default_sampler;
                bool not_equal_to_default = (model.samplers[i] == default_sampler);
                (void)not_equal_to_default;
                
                // Test self-equality
                bool self_equal = (model.samplers[i] == model.samplers[i]);
                (void)self_equal;
            }
        }
    }
    
    // Test Primitive equality operators (via Mesh primitives)
    if (!model.meshes.empty()) {
        for (auto& mesh : model.meshes) {
            if (!mesh.primitives.empty()) {
                for (size_t i = 0; i < mesh.primitives.size(); ++i) {
                    for (size_t j = 0; j < mesh.primitives.size(); ++j) {
                        bool equal = (mesh.primitives[i] == mesh.primitives[j]);
                        (void)equal;
                        
                        // Test with default constructed primitive
                        tinygltf::Primitive default_primitive;
                        bool not_equal_to_default = (mesh.primitives[i] == default_primitive);
                        (void)not_equal_to_default;
                        
                        // Test self-equality
                        bool self_equal = (mesh.primitives[i] == mesh.primitives[i]);
                        (void)self_equal;
                    }
                }
            }
        }
    }
    
    // Test Skin equality operators
    if (!model.skins.empty()) {
        for (size_t i = 0; i < model.skins.size(); ++i) {
            for (size_t j = 0; j < model.skins.size(); ++j) {
                bool equal = (model.skins[i] == model.skins[j]);
                (void)equal;
                
                // Test with default constructed skin
                tinygltf::Skin default_skin;
                bool not_equal_to_default = (model.skins[i] == default_skin);
                (void)not_equal_to_default;
                
                // Test self-equality
                bool self_equal = (model.skins[i] == model.skins[i]);
                (void)self_equal;
            }
        }
    }
    
    // Test Model equality operator itself
    bool model_self_equal = (model == model);
    (void)model_self_equal;
    
    // Test with default constructed model
    tinygltf::Model default_model;
    bool model_not_equal_to_default = (model == default_model);
    (void)model_not_equal_to_default;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        return 0; // Need minimum input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Clear global memory filesystem
    g_memory_fs.clear();
    
    // Setup GLTF loader with custom filesystem callbacks
    tinygltf::TinyGLTF loader;
    tinygltf::FsCallbacks custom_callbacks = setup_custom_fs_callbacks();
    std::string err;
    if (!loader.SetFsCallbacks(custom_callbacks, &err)) {
        return 0;
    }
    
    // Consume fuzzer input to determine which components to include
    uint8_t component_mask = fdp.ConsumeIntegral<uint8_t>();
    bool include_camera = (component_mask & 0x01) != 0;
    bool include_node = (component_mask & 0x02) != 0;
    bool include_mesh = (component_mask & 0x04) != 0;
    bool include_accessor = (component_mask & 0x08) != 0;
    bool include_image = (component_mask & 0x10) != 0;
    bool include_bufferview = (component_mask & 0x20) != 0;
    bool include_sampler = (component_mask & 0x40) != 0;
    bool include_skin = (component_mask & 0x80) != 0;
    
    // Always include primitive testing if mesh is included
    bool include_primitive = include_mesh;
    
    // Generate GLTF JSON with selected components
    std::string gltf_json = create_minimal_gltf_json(
        fdp, include_camera, include_node, include_mesh, include_accessor,
        include_image, include_bufferview, include_sampler, include_primitive, include_skin
    );
    
    // Convert JSON to binary data for memory filesystem
    std::vector<unsigned char> gltf_data(gltf_json.begin(), gltf_json.end());
    
    // Add GLTF file to memory filesystem
    std::string gltf_filename = "test.gltf";
    g_memory_fs.add_file(gltf_filename, gltf_data);
    
    // Load GLTF model using file-based loading (ASCII)
    tinygltf::Model model;
    std::string warn;
    
    bool load_success = loader.LoadASCIIFromFile(&model, &err, &warn, gltf_filename);
    
    if (!load_success) {
        // Try binary loading instead
        std::string gltf_bin_filename = "test.glb";
        g_memory_fs.add_file(gltf_bin_filename, gltf_data);
        
        load_success = loader.LoadBinaryFromFile(&model, &err, &warn, gltf_bin_filename);
    }
    
    if (load_success) {
        // Test all equality operators for components in the loaded model
        test_component_equality_operators(model, fdp);
    }
    
    // Also test with empty/default constructed components
    // This exercises equality operators for empty objects
    {
        tinygltf::Camera camera1, camera2;
        bool camera_eq = (camera1 == camera2);
        (void)camera_eq;
        
        tinygltf::Node node1, node2;
        bool node_eq = (node1 == node2);
        (void)node_eq;
        
        tinygltf::Mesh mesh1, mesh2;
        bool mesh_eq = (mesh1 == mesh2);
        (void)mesh_eq;
        
        tinygltf::Accessor accessor1, accessor2;
        bool accessor_eq = (accessor1 == accessor2);
        (void)accessor_eq;
        
        tinygltf::Image image1, image2;
        bool image_eq = (image1 == image2);
        (void)image_eq;
        
        tinygltf::BufferView bufferview1, bufferview2;
        bool bufferview_eq = (bufferview1 == bufferview2);
        (void)bufferview_eq;
        
        tinygltf::Sampler sampler1, sampler2;
        bool sampler_eq = (sampler1 == sampler2);
        (void)sampler_eq;
        
        tinygltf::Primitive primitive1, primitive2;
        bool primitive_eq = (primitive1 == primitive2);
        (void)primitive_eq;
        
        tinygltf::Skin skin1, skin2;
        bool skin_eq = (skin1 == skin2);
        (void)skin_eq;
        
        tinygltf::Model empty_model1, empty_model2;
        bool model_eq = (empty_model1 == empty_model2);
        (void)model_eq;
    }
    
    return 0;
}

} // namespace harness_005
