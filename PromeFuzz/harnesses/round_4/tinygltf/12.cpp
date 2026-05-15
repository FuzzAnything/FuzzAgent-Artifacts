// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
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
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

#include "tiny_gltf.h"

static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Initialize required asset field
    model.asset.version = "2.0";
    
    // Create a minimal valid model with some data
    tinygltf::Scene scene;
    scene.name = "TestScene";
    model.scenes.push_back(scene);
    
    tinygltf::Node node;
    node.name = "TestNode";
    model.nodes.push_back(node);
    
    model.scenes[0].nodes.push_back(0);
    
    // Add a buffer with some data
    tinygltf::Buffer buffer;
    buffer.name = "TestBuffer";
    if (size > 0) {
        buffer.data.assign(data, data + std::min(size, static_cast<size_t>(1024)));
    } else {
        buffer.data = {0x00, 0x01, 0x02, 0x03};
    }
    model.buffers.push_back(buffer);
    
    // Add a buffer view
    tinygltf::BufferView bufferView;
    bufferView.name = "TestBufferView";
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = model.buffers[0].data.size();
    model.bufferViews.push_back(bufferView);
    
    // Add an accessor
    tinygltf::Accessor accessor;
    accessor.name = "TestAccessor";
    accessor.bufferView = 0;
    accessor.byteOffset = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor.type = TINYGLTF_TYPE_VEC3;
    accessor.count = 1;
    model.accessors.push_back(accessor);
    
    // Add a mesh with primitive
    tinygltf::Mesh mesh;
    mesh.name = "TestMesh";
    
    tinygltf::Primitive primitive;
    primitive.material = -1;
    primitive.attributes["POSITION"] = 0;
    mesh.primitives.push_back(primitive);
    
    model.meshes.push_back(mesh);
    model.nodes[0].mesh = 0;
    
    // Try to set some extras from fuzzer data (may contain invalid UTF-8)
    if (size > 0) {
        std::string fuzz_data(reinterpret_cast<const char*>(data), std::min(size, static_cast<size_t>(256)));
        
        // Set extras on various components (may contain invalid UTF-8)
        model.asset.extras = tinygltf::Value(fuzz_data.c_str());
        model.scenes[0].extras = tinygltf::Value(fuzz_data.c_str());
        model.nodes[0].extras = tinygltf::Value(fuzz_data.c_str());
        
        // Also set extras_json_string if the feature is enabled
        model.scenes[0].extras_json_string = fuzz_data;
    }
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Initialize TinyGLTF object
    tinygltf::TinyGLTF loader;
    
    // Use fuzzer data to determine boolean states
    bool serializeDefaults = false;
    bool storeOriginalJSON = false;
    bool embedImages = false;
    bool embedBuffers = false;
    bool prettyPrint = false;
    bool writeBinary = false;
    
    if (Size >= 1) {
        uint8_t flags = Data[0];
        serializeDefaults = (flags & 0x01) != 0;
        storeOriginalJSON = (flags & 0x02) != 0;
        embedImages = (flags & 0x04) != 0;
        embedBuffers = (flags & 0x08) != 0;
        prettyPrint = (flags & 0x10) != 0;
        writeBinary = (flags & 0x20) != 0;
    }
    
    // Set the states based on fuzzer input
    loader.SetSerializeDefaultValues(serializeDefaults);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(storeOriginalJSON);
    
    // Get the states back
    bool gotSerializeDefaults = loader.GetSerializeDefaultValues();
    bool gotStoreOriginalJSON = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    
    // Create a model and populate it with fuzzer data
    tinygltf::Model model;
    populateModel(model, Data, Size);
    
    // Try to write to file
    const std::string filename = "./dummy_file.gltf";
    try {
        loader.WriteGltfSceneToFile(&model, filename, embedImages, embedBuffers, prettyPrint, writeBinary);
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Clean up the file
    std::remove(filename.c_str());
    
    // Try to write to stream
    std::ostringstream stream;
    try {
        loader.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Test with different combinations of parameters
    if (Size >= 2) {
        uint8_t altFlags = Data[1 % Size];
        bool altEmbedImages = (altFlags & 0x01) != 0;
        bool altEmbedBuffers = (altFlags & 0x02) != 0;
        bool altPrettyPrint = (altFlags & 0x04) != 0;
        bool altWriteBinary = (altFlags & 0x08) != 0;
        
        const std::string altFilename = "./dummy_file_alt.gltf";
        try {
            loader.WriteGltfSceneToFile(&model, altFilename, altEmbedImages, altEmbedBuffers, altPrettyPrint, altWriteBinary);
        } catch (...) {
            // Catch exceptions
        }
        std::remove(altFilename.c_str());
        
        std::ostringstream altStream;
        try {
            loader.WriteGltfSceneToStream(&model, altStream, altPrettyPrint, altWriteBinary);
        } catch (...) {
            // Catch exceptions
        }
    }
    
    // Toggle states and test again
    loader.SetSerializeDefaultValues(!serializeDefaults);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(!storeOriginalJSON);
    
    // Get the toggled states
    bool toggledSerializeDefaults = loader.GetSerializeDefaultValues();
    bool toggledStoreOriginalJSON = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    
    // Try one more write with toggled states
    const std::string toggledFilename = "./dummy_file_toggled.gltf";
    try {
        loader.WriteGltfSceneToFile(&model, toggledFilename, !embedImages, !embedBuffers, !prettyPrint, !writeBinary);
    } catch (...) {
        // Catch exceptions
    }
    std::remove(toggledFilename.c_str());
    
    return 0;
}