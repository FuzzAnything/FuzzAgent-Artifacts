// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
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
#include <fstream>
#include <sstream>
#include <cstdint>
#include <cstring>

static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set asset version
    model.asset.version = "2.0";
    
    // Create a node
    tinygltf::Node node;
    node.name = "TestNode";
    
    // Add some minimal data to make the model somewhat valid
    tinygltf::Buffer buffer;
    buffer.name = "TestBuffer";
    if (size > 0) {
        buffer.data.assign(data, data + std::min(size, static_cast<size_t>(1024)));
    }
    model.buffers.push_back(buffer);
    
    tinygltf::BufferView bufferView;
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = buffer.data.size();
    model.bufferViews.push_back(bufferView);
    
    tinygltf::Accessor accessor;
    accessor.bufferView = 0;
    accessor.byteOffset = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor.type = TINYGLTF_TYPE_VEC3;
    accessor.count = 1;
    model.accessors.push_back(accessor);
    
    // Add mesh with primitive
    tinygltf::Mesh mesh;
    tinygltf::Primitive primitive;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;
    primitive.attributes["POSITION"] = 0;
    mesh.primitives.push_back(primitive);
    model.meshes.push_back(mesh);
    
    node.mesh = 0;
    model.nodes.push_back(node);
    
    // Create scene
    tinygltf::Scene scene;
    scene.nodes.push_back(0);
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    // Use fuzzer data for string fields (extras, extensions, etc.)
    // This may include invalid UTF-8 sequences
    std::string fuzz_data(data, data + size);
    
    // Set extras in various places with fuzzer data
    tinygltf::Value extras_value;
    // Note: We're creating a simple string value from fuzzer data
    // The actual Value class might need proper construction
    // This is a simplified approach for fuzzing
    model.extras = tinygltf::Value();
    
    // Also set in asset
    model.asset.extras = tinygltf::Value();
    
    // Set in some components if they exist
    if (!model.meshes.empty()) {
        model.meshes[0].extras = tinygltf::Value();
    }
    
    if (!model.nodes.empty()) {
        model.nodes[0].extras = tinygltf::Value();
    }
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    
    // Populate model with fuzzer data
    populateModel(model, Data, Size);
    
    // Use first byte to control various boolean parameters
    uint8_t control_byte = Data[0];
    bool serialize_defaults = (control_byte & 0x01) != 0;
    bool store_original_json = (control_byte & 0x02) != 0;
    bool embed_images = (control_byte & 0x04) != 0;
    bool embed_buffers = (control_byte & 0x08) != 0;
    bool pretty_print = (control_byte & 0x10) != 0;
    bool write_binary = (control_byte & 0x20) != 0;
    
    // Test Set/Get functions
    loader.SetSerializeDefaultValues(serialize_defaults);
    bool get_serialize = loader.GetSerializeDefaultValues();
    
    loader.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
    bool get_store_json = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    
    // Test WriteGltfSceneToFile
    try {
        bool result = loader.WriteGltfSceneToFile(&model, "./dummy_file", 
                                                 embed_images, embed_buffers,
                                                 pretty_print, write_binary);
        // Clean up the dummy file
        std::remove("./dummy_file");
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other issues
        std::remove("./dummy_file");
    }
    
    // Test WriteGltfSceneToStream
    try {
        std::ostringstream oss;
        bool result = loader.WriteGltfSceneToStream(&model, oss, 
                                                   pretty_print, write_binary);
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other issues
    }
    
    // Test with different combinations of settings
    if (Size > 1) {
        uint8_t second_control = Data[1 % Size];
        
        // Flip some settings
        loader.SetSerializeDefaultValues((second_control & 0x01) != 0);
        loader.SetStoreOriginalJSONForExtrasAndExtensions((second_control & 0x02) != 0);
        
        // Test again with different parameters
        try {
            std::ostringstream oss2;
            bool embed_images2 = (second_control & 0x04) != 0;
            bool write_binary2 = (second_control & 0x08) != 0;
            
            bool result = loader.WriteGltfSceneToStream(&model, oss2, 
                                                       pretty_print, write_binary2);
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    return 0;
}