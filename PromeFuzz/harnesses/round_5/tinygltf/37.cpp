// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.cc:1105:16 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "tiny_gltf.h"

// Custom image writer callback for testing - corrected signature
static bool dummyImageWriter(const std::string* basepath, const std::string* uri,
                             const tinygltf::Image* image, bool embedImages,
                             const tinygltf::FsCallbacks* fs,
                             const tinygltf::URICallbacks* uri_cb,
                             std::string* out_uri, void* user_data) {
    // Always return true to simulate successful image writing
    return true;
}

// Helper function to populate a minimal Model with fuzzed data
static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set basic asset info
    model.asset.version = "2.0";
    model.asset.generator = "Fuzzer";
    
    // Create a minimal buffer with fuzzed data
    if (size > 0) {
        tinygltf::Buffer buffer;
        buffer.name = "fuzz_buffer";
        buffer.data.assign(data, data + std::min(size, static_cast<size_t>(1024)));
        model.buffers.push_back(buffer);
        
        // Create a buffer view
        tinygltf::BufferView bufferView;
        bufferView.name = "fuzz_buffer_view";
        bufferView.buffer = 0;
        bufferView.byteOffset = 0;
        bufferView.byteLength = std::min(size, static_cast<size_t>(1024));
        model.bufferViews.push_back(bufferView);
        
        // Create an accessor
        tinygltf::Accessor accessor;
        accessor.name = "fuzz_accessor";
        accessor.bufferView = 0;
        accessor.byteOffset = 0;
        accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        accessor.type = TINYGLTF_TYPE_VEC3;
        accessor.count = std::min(size / 12, static_cast<size_t>(10)); // Approximate count
        model.accessors.push_back(accessor);
    }
    
    // Create a node
    tinygltf::Node node;
    node.name = "fuzz_node";
    if (!model.accessors.empty()) {
        node.mesh = 0;
    }
    model.nodes.push_back(node);
    
    // Create a mesh with a primitive
    tinygltf::Mesh mesh;
    mesh.name = "fuzz_mesh";
    if (!model.accessors.empty()) {
        tinygltf::Primitive primitive;
        primitive.material = 0;
        primitive.mode = TINYGLTF_MODE_TRIANGLES;
        primitive.attributes["POSITION"] = 0;
        mesh.primitives.push_back(primitive);
    }
    model.meshes.push_back(mesh);
    
    // Create a material
    tinygltf::Material material;
    material.name = "fuzz_material";
    material.pbrMetallicRoughness.baseColorFactor = {1.0, 0.0, 0.0, 1.0};
    material.pbrMetallicRoughness.metallicFactor = 0.5;
    material.pbrMetallicRoughness.roughnessFactor = 0.5;
    model.materials.push_back(material);
    
    // Create a scene
    tinygltf::Scene scene;
    scene.name = "fuzz_scene";
    if (!model.nodes.empty()) {
        scene.nodes.push_back(0);
    }
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    // Add some string data from fuzzer input (may contain invalid UTF-8)
    if (size > 0) {
        std::string fuzz_data(reinterpret_cast<const char*>(data), 
                             std::min(size, static_cast<size_t>(256)));
        
        // Add to asset extras
        tinygltf::Value::Object extras_obj;
        extras_obj["fuzz_key"] = tinygltf::Value(fuzz_data);
        model.asset.extras = tinygltf::Value(extras_obj);
        
        // Add to material extras
        tinygltf::Value::Object mat_extras;
        mat_extras["fuzz_mat_data"] = tinygltf::Value(fuzz_data.substr(0, std::min(size, static_cast<size_t>(128))));
        model.materials[0].extras = tinygltf::Value(mat_extras);
    }
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* data, size_t size) {
    // Initialize random seed based on input data
    unsigned int seed = 0;
    if (size >= sizeof(seed)) {
        seed = *reinterpret_cast<const unsigned int*>(data);
    } else if (size > 0) {
        seed = data[0];
    }
    srand(seed);
    
    // Create TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Set random configuration options
    loader.SetSerializeDefaultValues(rand() % 2);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(rand() % 2);
    
    // Set custom image writer 50% of the time
    if (rand() % 2) {
        loader.SetImageWriter(dummyImageWriter, nullptr);
    }
    
    // Create and populate model
    tinygltf::Model model;
    populateModel(model, data, size);
    
    // Get configuration state (test GetStoreOriginalJSONForExtrasAndExtensions)
    bool storeOriginalJSON = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)storeOriginalJSON; // Mark as used
    
    // Test WriteGltfSceneToFile with various configurations
    const std::string filename = "./dummy_file.gltf";
    
    // Try different combinations of parameters
    bool embedImages = rand() % 2;
    bool embedBuffers = rand() % 2;
    bool prettyPrint = rand() % 2;
    bool writeBinary = rand() % 2;
    
    try {
        loader.WriteGltfSceneToFile(&model, filename, embedImages, embedBuffers, 
                                   prettyPrint, writeBinary);
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Clean up the file
    std::remove(filename.c_str());
    
    // Test WriteGltfSceneToStream with various configurations
    std::ostringstream stream;
    
    prettyPrint = rand() % 2;
    writeBinary = rand() % 2;
    
    try {
        loader.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Test with stringstream as well
    std::stringstream string_stream;
    
    try {
        loader.WriteGltfSceneToStream(&model, string_stream, rand() % 2, rand() % 2);
    } catch (...) {
        // Catch any exceptions
    }
    
    // Test with different model states
    // Create an empty model
    tinygltf::Model emptyModel;
    emptyModel.asset.version = "2.0";
    
    std::ostringstream emptyStream;
    try {
        loader.WriteGltfSceneToStream(&emptyModel, emptyStream, true, false);
    } catch (...) {
        // Catch any exceptions
    }
    
    // Test with model containing only scenes
    tinygltf::Model sceneOnlyModel;
    sceneOnlyModel.asset.version = "2.0";
    tinygltf::Scene scene;
    scene.name = "test_scene";
    sceneOnlyModel.scenes.push_back(scene);
    sceneOnlyModel.defaultScene = 0;
    
    try {
        loader.WriteGltfSceneToFile(&sceneOnlyModel, "./scene_only.gltf", 
                                   false, false, true, false);
    } catch (...) {
        // Catch any exceptions
    }
    std::remove("./scene_only.gltf");
    
    // Test with binary output to file
    try {
        loader.WriteGltfSceneToFile(&model, "./binary_output.glb", 
                                   true, true, false, true);
    } catch (...) {
        // Catch any exceptions
    }
    std::remove("./binary_output.glb");
    
    return 0;
}