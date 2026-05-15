// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
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
    // Set basic asset info
    model.asset.version = "2.0";
    
    // Use fuzzer data for extras string (may contain invalid UTF-8)
    if (size > 0) {
        std::string fuzz_data(reinterpret_cast<const char*>(data), size);
        
        // Set extras on various model components
        model.asset.extras = tinygltf::Value();
        
        // Add a minimal scene
        tinygltf::Scene scene;
        scene.name = "FuzzedScene";
        scene.extras = tinygltf::Value();
        model.scenes.push_back(scene);
        model.defaultScene = 0;
        
        // Add a node
        tinygltf::Node node;
        node.name = "FuzzedNode";
        node.extras = tinygltf::Value();
        model.nodes.push_back(node);
        
        // Add a minimal mesh with primitive
        tinygltf::Mesh mesh;
        mesh.name = "FuzzedMesh";
        
        tinygltf::Primitive primitive;
        primitive.material = -1;
        primitive.mode = 4; // TRIANGLES
        primitive.extras = tinygltf::Value();
        
        // Add some attributes using fuzzer data
        if (size > 10) {
            primitive.attributes["POSITION"] = 0;
        }
        
        mesh.primitives.push_back(primitive);
        model.meshes.push_back(mesh);
        
        // Add a minimal accessor
        tinygltf::Accessor accessor;
        accessor.name = "FuzzedAccessor";
        accessor.bufferView = -1;
        accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        accessor.count = 1;
        accessor.extras = tinygltf::Value();
        model.accessors.push_back(accessor);
        
        // Add a minimal buffer
        tinygltf::Buffer buffer;
        buffer.name = "FuzzedBuffer";
        if (size > 0) {
            buffer.data.assign(data, data + (size > 100 ? 100 : size));
        }
        buffer.extras = tinygltf::Value();
        model.buffers.push_back(buffer);
        
        // Add a buffer view
        tinygltf::BufferView bufferView;
        bufferView.name = "FuzzedBufferView";
        bufferView.buffer = 0;
        bufferView.byteOffset = 0;
        bufferView.byteLength = model.buffers[0].data.size();
        bufferView.extras = tinygltf::Value();
        model.bufferViews.push_back(bufferView);
        
        // Add a material
        tinygltf::Material material;
        material.name = "FuzzedMaterial";
        material.extras = tinygltf::Value();
        model.materials.push_back(material);
    }
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Initialize TinyGLTF object
    tinygltf::TinyGLTF loader;
    
    // Test getters and setters with fuzzer data
    bool serialize_defaults = (Data[0] & 0x01) != 0;
    bool store_original_json = (Data[0] & 0x02) != 0;
    
    loader.SetSerializeDefaultValues(serialize_defaults);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
    
    // Call getters to ensure they work
    (void)loader.GetSerializeDefaultValues();
    (void)loader.GetStoreOriginalJSONForExtrasAndExtensions();
    
    // Create and populate model
    tinygltf::Model model;
    populateModel(model, Data, Size);
    
    // Test WriteGltfSceneToFile with various combinations
    bool embedImages = (Data[0] & 0x04) != 0;
    bool embedBuffers = (Data[0] & 0x08) != 0;
    bool prettyPrint = (Data[0] & 0x10) != 0;
    bool writeBinary = (Data[0] & 0x20) != 0;
    
    const std::string filename = "./dummy_file";
    
    try {
        (void)loader.WriteGltfSceneToFile(&model, filename, 
                                         embedImages, embedBuffers,
                                         prettyPrint, writeBinary);
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Clean up the dummy file
    std::remove(filename.c_str());
    
    // Test WriteGltfSceneToStream
    std::ostringstream stream;
    
    try {
        (void)loader.WriteGltfSceneToStream(&model, stream, 
                                           prettyPrint, writeBinary);
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Test with different serialization settings
    loader.SetSerializeDefaultValues(!serialize_defaults);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(!store_original_json);
    
    // Test again with inverted settings
    try {
        std::ostringstream stream2;
        (void)loader.WriteGltfSceneToStream(&model, stream2, 
                                           !prettyPrint, !writeBinary);
    } catch (...) {
        // Catch any exceptions
    }
    
    return 0;
}