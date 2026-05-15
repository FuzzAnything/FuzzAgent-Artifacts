// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
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

static void populateModel(tinygltf::Model* model, const uint8_t* data, size_t size) {
    // Initialize basic asset info
    model->asset.version = "2.0";
    
    // Create a simple buffer with fuzzer data
    tinygltf::Buffer buffer;
    buffer.name = "fuzz_buffer";
    if (size > 0) {
        buffer.data.assign(data, data + std::min(size, (size_t)1000));
    }
    model->buffers.push_back(buffer);
    
    // Create a buffer view
    tinygltf::BufferView bufferView;
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = buffer.data.size();
    model->bufferViews.push_back(bufferView);
    
    // Create an accessor
    tinygltf::Accessor accessor;
    accessor.bufferView = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
    accessor.type = TINYGLTF_TYPE_SCALAR;
    accessor.count = buffer.data.size();
    model->accessors.push_back(accessor);
    
    // Create a simple scene
    tinygltf::Scene scene;
    scene.name = "fuzz_scene";
    model->scenes.push_back(scene);
    model->defaultScene = 0;
    
    // Create a node
    tinygltf::Node node;
    node.name = "fuzz_node";
    model->nodes.push_back(node);
    
    // Add node to scene
    model->scenes[0].nodes.push_back(0);
    
    // Set extras from fuzzer data (may contain invalid UTF-8)
    if (size > 0) {
        std::string fuzz_string(reinterpret_cast<const char*>(data), 
                               std::min(size, (size_t)500));
        model->asset.extras = tinygltf::Value(fuzz_string);
    }
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* Data, size_t Size) {
    // Initialize TinyGLTF loader
    tinygltf::TinyGLTF loader;
    
    // Use first byte to determine boolean flags
    uint8_t flags = Size > 0 ? Data[0] : 0;
    bool storeOriginalJSON = flags & 0x01;
    bool serializeDefaults = flags & 0x02;
    bool embedImages = flags & 0x04;
    bool embedBuffers = flags & 0x08;
    bool prettyPrint = flags & 0x10;
    bool writeBinary = flags & 0x20;
    
    // Set loader options
    loader.SetStoreOriginalJSONForExtrasAndExtensions(storeOriginalJSON);
    loader.SetSerializeDefaultValues(serializeDefaults);
    
    // Verify getter matches setter
    bool retrieved = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    if (retrieved != storeOriginalJSON) {
        // Mismatch - could indicate a bug
    }
    
    // Try to load model from fuzzer input
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Use remaining data for LoadASCIIFromString
    const char* gltf_str = Size > 1 ? reinterpret_cast<const char*>(Data + 1) : "";
    size_t gltf_len = Size > 1 ? Size - 1 : 0;
    
    bool load_success = false;
    try {
        load_success = loader.LoadASCIIFromString(&model, &err, &warn, 
                                                 gltf_str, gltf_len, 
                                                 "./dummy_file");
    } catch (...) {
        // Catch any exceptions from invalid input
    }
    
    // If loading failed, create a minimal model
    if (!load_success) {
        model = tinygltf::Model();
        populateModel(&model, Data, Size);
    }
    
    // Write to file
    bool write_success = false;
    try {
        write_success = loader.WriteGltfSceneToFile(&model, "./dummy_file", 
                                                   embedImages, embedBuffers,
                                                   prettyPrint, writeBinary);
    } catch (...) {
        // Catch exceptions from invalid UTF-8 or other serialization errors
    }
    
    // Write to stream
    std::ostringstream stream;
    try {
        loader.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);
    } catch (...) {
        // Catch exceptions from stream writing
    }
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    // Try different combinations of settings
    loader.SetStoreOriginalJSONForExtrasAndExtensions(!storeOriginalJSON);
    loader.SetSerializeDefaultValues(!serializeDefaults);
    
    // Write again with opposite settings
    try {
        loader.WriteGltfSceneToFile(&model, "./dummy_file", 
                                   !embedImages, !embedBuffers,
                                   !prettyPrint, !writeBinary);
    } catch (...) {
        // Ignore exceptions
    }
    
    std::remove("./dummy_file");
    
    return 0;
}