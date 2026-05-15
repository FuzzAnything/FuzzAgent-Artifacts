// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.cc:1105:16 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
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

static bool dummyImageWriter(const std::string* basepath,
                             const std::string* filename,
                             const tinygltf::Image* image,
                             bool embedImages,
                             const tinygltf::FsCallbacks* fs,
                             const tinygltf::URICallbacks* uri,
                             std::string* out_uri,
                             void* user_data) {
    // Dummy implementation for fuzzing
    if (out_uri) {
        *out_uri = "dummy_uri";
    }
    return true;
}

static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set basic asset info
    model.asset.version = "2.0";
    
    // Create a minimal valid scene
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
    buffer.data.assign(data, data + std::min(size, size_t(1024)));
    model.buffers.push_back(buffer);
    
    // Create a buffer view
    tinygltf::BufferView bufferView;
    bufferView.name = "TestBufferView";
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = std::min(size, size_t(1024));
    model.bufferViews.push_back(bufferView);
    
    // Create an accessor
    tinygltf::Accessor accessor;
    accessor.name = "TestAccessor";
    accessor.bufferView = 0;
    accessor.byteOffset = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor.count = std::min(size / 16, size_t(64)); // Approximate
    accessor.type = TINYGLTF_TYPE_VEC3;
    model.accessors.push_back(accessor);
    
    // Create a mesh with primitive
    tinygltf::Mesh mesh;
    mesh.name = "TestMesh";
    tinygltf::Primitive primitive;
    primitive.material = 0;
    primitive.attributes["POSITION"] = 0;
    mesh.primitives.push_back(primitive);
    model.meshes.push_back(mesh);
    model.nodes[0].mesh = 0;
    
    // Create a material
    tinygltf::Material material;
    material.name = "TestMaterial";
    model.materials.push_back(material);
    
    // Add extras string from fuzzer data (may contain invalid UTF-8)
    std::string fuzz_data(reinterpret_cast<const char*>(data), std::min(size, size_t(256)));
    model.extras = tinygltf::Value(fuzz_data);
    
    // Also set extras in some nested structures
    model.materials[0].extras = tinygltf::Value(fuzz_data);
    if (!model.meshes.empty() && !model.meshes[0].primitives.empty()) {
        model.meshes[0].primitives[0].extras = tinygltf::Value(fuzz_data);
    }
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine boolean parameters
    uint8_t flags = Data[0];
    bool storeOriginalJSON = (flags & 0x01) != 0;
    bool serializeDefaults = (flags & 0x02) != 0;
    bool embedImages = (flags & 0x04) != 0;
    bool embedBuffers = (flags & 0x08) != 0;
    bool prettyPrint = (flags & 0x10) != 0;
    bool writeBinary = (flags & 0x20) != 0;
    
    // Skip first byte for model data
    const uint8_t* modelData = Data + 1;
    size_t modelSize = Size - 1;
    
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    
    // Configure loader based on fuzzer input
    loader.SetStoreOriginalJSONForExtrasAndExtensions(storeOriginalJSON);
    loader.SetSerializeDefaultValues(serializeDefaults);
    
    // Set dummy image writer
    loader.SetImageWriter(dummyImageWriter, nullptr);
    
    // Check getter
    bool currentStoreSetting = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)currentStoreSetting; // Use result to avoid unused variable warning
    
    // Populate model with fuzzer data
    populateModel(model, modelData, modelSize);
    
    // Try writing to file
    try {
        const std::string filename = "./dummy_file";
        bool success = loader.WriteGltfSceneToFile(&model, filename,
                                                   embedImages, embedBuffers,
                                                   prettyPrint, writeBinary);
        (void)success; // Use result to avoid unused variable warning
        
        // Clean up the dummy file
        std::remove(filename.c_str());
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Try writing to stream
    try {
        std::ostringstream oss;
        bool success = loader.WriteGltfSceneToStream(&model, oss,
                                                     prettyPrint, writeBinary);
        (void)success; // Use result to avoid unused variable warning
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    return 0;
}