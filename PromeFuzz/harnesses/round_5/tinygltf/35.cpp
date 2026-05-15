// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.cc:1105:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
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
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

static void populateModel(tinygltf::Model &model, const uint8_t *data, size_t size) {
    // Initialize basic asset information
    model.asset.version = "2.0";
    model.asset.generator = "fuzzer";
    
    // Create a simple scene
    tinygltf::Scene scene;
    scene.name = "TestScene";
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    // Create a node
    tinygltf::Node node;
    node.name = "TestNode";
    model.nodes.push_back(node);
    model.scenes[0].nodes.push_back(0);
    
    // Create a buffer with some data
    tinygltf::Buffer buffer;
    buffer.name = "TestBuffer";
    buffer.data.assign(data, data + (size > 1024 ? 1024 : size)); // Limit size
    model.buffers.push_back(buffer);
    
    // Create a buffer view
    tinygltf::BufferView bufferView;
    bufferView.name = "TestBufferView";
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = buffer.data.size();
    model.bufferViews.push_back(bufferView);
    
    // Create an accessor
    tinygltf::Accessor accessor;
    accessor.name = "TestAccessor";
    accessor.bufferView = 0;
    accessor.byteOffset = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor.count = buffer.data.size() / 16; // Rough estimate
    accessor.type = TINYGLTF_TYPE_VEC4;
    model.accessors.push_back(accessor);
    
    // Create a mesh with primitive
    tinygltf::Mesh mesh;
    mesh.name = "TestMesh";
    tinygltf::Primitive primitive;
    primitive.material = 0;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;
    primitive.attributes["POSITION"] = 0;
    mesh.primitives.push_back(primitive);
    model.meshes.push_back(mesh);
    model.nodes[0].mesh = 0;
    
    // Create a material
    tinygltf::Material material;
    material.name = "TestMaterial";
    material.pbrMetallicRoughness.baseColorFactor = {1.0, 1.0, 1.0, 1.0};
    material.pbrMetallicRoughness.metallicFactor = 0.5;
    material.pbrMetallicRoughness.roughnessFactor = 0.5;
    model.materials.push_back(material);
    
    // Set extras from fuzzer data (may contain invalid UTF-8)
    if (size > 0) {
        std::string extras_str(reinterpret_cast<const char*>(data), 
                              size > 100 ? 100 : size);
        model.extras = tinygltf::Value(extras_str);
    }
}

static bool dummyImageWriter(const std::string* basepath, 
                            const std::string* filename, 
                            const tinygltf::Image* image, 
                            bool embedImages,
                            const tinygltf::FsCallbacks* fs_callbacks,
                            const tinygltf::URICallbacks* uri_callbacks,
                            std::string* out_buffer,
                            void* user_data) {
    // Do nothing, just return success
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Initialize model
    tinygltf::Model model;
    populateModel(model, Data, Size);
    
    // Create TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Use fuzzer data to determine various boolean parameters
    uint8_t flags = Size > 0 ? Data[0] : 0;
    
    // Set serialization options based on fuzzer input
    loader.SetSerializeDefaultValues((flags & 0x01) != 0);
    loader.SetStoreOriginalJSONForExtrasAndExtensions((flags & 0x02) != 0);
    
    // Test GetStoreOriginalJSONForExtrasAndExtensions
    bool storeOriginal = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)storeOriginal; // Prevent unused variable warning
    
    // Set image writer callback
    void* user_data = nullptr;
    loader.SetImageWriter(dummyImageWriter, user_data);
    
    // Prepare output file path
    const std::string filename = "./dummy_file";
    
    // Try different combinations of parameters
    bool embedImages = (flags & 0x04) != 0;
    bool embedBuffers = (flags & 0x08) != 0;
    bool prettyPrint = (flags & 0x10) != 0;
    bool writeBinary = (flags & 0x20) != 0;
    
    // Test WriteGltfSceneToFile
    try {
        loader.WriteGltfSceneToFile(&model, filename, 
                                   embedImages, embedBuffers, 
                                   prettyPrint, writeBinary);
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Test WriteGltfSceneToStream with different parameters
    std::ostringstream stream1;
    try {
        loader.WriteGltfSceneToStream(&model, stream1, 
                                     prettyPrint, writeBinary);
    } catch (...) {
        // Catch exceptions
    }
    
    // Test with opposite parameters
    std::ostringstream stream2;
    try {
        loader.WriteGltfSceneToStream(&model, stream2, 
                                     !prettyPrint, !writeBinary);
    } catch (...) {
        // Catch exceptions
    }
    
    // Clean up dummy file
    std::remove(filename.c_str());
    
    return 0;
}