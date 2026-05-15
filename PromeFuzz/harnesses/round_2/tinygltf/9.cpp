// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set basic asset info
    model.asset.version = "2.0";
    
    // Create a minimal scene
    tinygltf::Scene scene;
    scene.name = "TestScene";
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    // Create a node
    tinygltf::Node node;
    node.name = "TestNode";
    model.nodes.push_back(node);
    
    // Add node to scene
    model.scenes[0].nodes.push_back(0);
    
    // Create a simple mesh with primitive
    tinygltf::Mesh mesh;
    mesh.name = "TestMesh";
    
    tinygltf::Primitive primitive;
    primitive.material = 0;
    
    // Add some attributes
    primitive.attributes["POSITION"] = 0;
    
    mesh.primitives.push_back(primitive);
    model.meshes.push_back(mesh);
    
    // Create a material
    tinygltf::Material material;
    material.name = "TestMaterial";
    model.materials.push_back(material);
    
    // Create accessor for position
    tinygltf::Accessor accessor;
    accessor.bufferView = 0;
    accessor.byteOffset = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor.count = 3;
    accessor.type = TINYGLTF_TYPE_VEC3;
    model.accessors.push_back(accessor);
    
    // Create buffer view
    tinygltf::BufferView bufferView;
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = 36; // 3 vertices * 3 floats * 4 bytes
    model.bufferViews.push_back(bufferView);
    
    // Create buffer with some dummy data
    tinygltf::Buffer buffer;
    buffer.data = std::vector<unsigned char>(36, 0);
    model.buffers.push_back(buffer);
    
    // Use fuzzer data for extras strings if available
    if (size > 0) {
        // Limit string length to avoid excessive memory usage
        size_t str_len = size > 1000 ? 1000 : size;
        std::string fuzz_data(reinterpret_cast<const char*>(data), str_len);
        
        // Set extras on various objects
        model.asset.extras = tinygltf::Value();
        model.scenes[0].extras = tinygltf::Value();
        model.nodes[0].extras = tinygltf::Value();
        
        // Set extras_json_string if the struct has it
        if (!model.meshes[0].primitives.empty()) {
            model.meshes[0].primitives[0].extras_json_string = fuzz_data;
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* Data, size_t Size) {
    // Initialize tinygltf object
    tinygltf::TinyGLTF loader;
    
    // Test GetSerializeDefaultValues (should return default)
    bool defaultSerialize = loader.GetSerializeDefaultValues();
    (void)defaultSerialize; // Mark as used
    
    // Test GetStoreOriginalJSONForExtrasAndExtensions (should return default)
    bool defaultStoreJSON = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)defaultStoreJSON; // Mark as used
    
    // Use fuzzer data to determine boolean parameters
    bool serializeDefaults = false;
    bool storeJSON = false;
    bool embedImages = false;
    bool embedBuffers = false;
    bool prettyPrint = false;
    bool writeBinary = false;
    
    if (Size > 0) {
        // Use first byte to determine some boolean values
        uint8_t flags = Data[0];
        serializeDefaults = (flags & 0x01) != 0;
        storeJSON = (flags & 0x02) != 0;
        embedImages = (flags & 0x04) != 0;
        embedBuffers = (flags & 0x08) != 0;
        prettyPrint = (flags & 0x10) != 0;
        writeBinary = (flags & 0x20) != 0;
    }
    
    // Set parameters based on fuzzer input
    loader.SetSerializeDefaultValues(serializeDefaults);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(storeJSON);
    
    // Verify getters return what we set
    bool gotSerialize = loader.GetSerializeDefaultValues();
    bool gotStoreJSON = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    
    // These should match what we set (unless there's some constraint)
    (void)gotSerialize;
    (void)gotStoreJSON;
    
    // Create and populate a model
    tinygltf::Model model;
    populateModel(model, Data, Size);
    
    // Try to write to file
    try {
        bool writeSuccess = loader.WriteGltfSceneToFile(
            &model, 
            "./dummy_file",
            embedImages,
            embedBuffers,
            prettyPrint,
            writeBinary
        );
        (void)writeSuccess; // Mark as used
        
        // Clean up the dummy file
        std::remove("./dummy_file");
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other issues
        std::remove("./dummy_file");
    }
    
    // Try to write to stream
    try {
        std::ostringstream stream;
        bool streamSuccess = loader.WriteGltfSceneToStream(
            &model,
            stream,
            prettyPrint,
            writeBinary
        );
        (void)streamSuccess; // Mark as used
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other issues
    }
    
    return 0;
}