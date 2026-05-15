// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.cc:1105:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
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

// Custom image writer callback for testing with correct signature
static bool dummyImageWriter(const std::string* basepath,
                            const std::string* filename,
                            const tinygltf::Image* image,
                            bool embedImages,
                            const tinygltf::FsCallbacks* fs_cb,
                            const tinygltf::URICallbacks* uri_cb,
                            std::string* out_uri,
                            void* user_data) {
    // Return success regardless of actual writing
    if (out_uri) {
        *out_uri = "dummy_uri";
    }
    return true;
}

// Helper function to create a minimal valid Model
static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set basic asset info
    model.asset.version = "2.0";
    
    // Create a minimal node
    tinygltf::Node node;
    node.name = "TestNode";
    
    // Add some extras data using fuzzer input (limited to avoid invalid UTF-8)
    std::string fuzz_data;
    if (size > 0) {
        // Take a small portion to avoid huge strings
        size_t str_size = size > 100 ? 100 : size;
        fuzz_data.assign(reinterpret_cast<const char*>(data), str_size);
        
        // Ensure it's valid UTF-8 by replacing non-ASCII with safe characters
        for (char& c : fuzz_data) {
            if (static_cast<unsigned char>(c) > 127) {
                c = '?';
            }
        }
    }
    
    // Create extras value
    tinygltf::Value::Object extras_obj;
    extras_obj["fuzz_data"] = tinygltf::Value(fuzz_data);
    node.extras = tinygltf::Value(std::move(extras_obj));
    
    model.nodes.push_back(node);
    
    // Create a minimal scene
    tinygltf::Scene scene;
    scene.name = "TestScene";
    scene.nodes.push_back(0); // Reference the node we just added
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    // Create a minimal buffer
    tinygltf::Buffer buffer;
    buffer.name = "TestBuffer";
    if (size > 0) {
        buffer.data.assign(data, data + (size > 1024 ? 1024 : size));
    }
    model.buffers.push_back(buffer);
    
    // Create a minimal buffer view
    tinygltf::BufferView bufferView;
    bufferView.name = "TestBufferView";
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = buffer.data.size();
    model.bufferViews.push_back(bufferView);
    
    // Create a minimal accessor
    tinygltf::Accessor accessor;
    accessor.name = "TestAccessor";
    accessor.bufferView = 0;
    accessor.byteOffset = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor.type = TINYGLTF_TYPE_VEC3;
    accessor.count = 1;
    model.accessors.push_back(accessor);
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // Initialize TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Test SetImageWriter with correct signature
    loader.SetImageWriter(dummyImageWriter, nullptr);
    
    // Test SetStoreOriginalJSONForExtrasAndExtensions with fuzzer-controlled value
    bool store_json = false;
    if (Size > 0) {
        store_json = (Data[0] & 0x01) != 0;
    }
    loader.SetStoreOriginalJSONForExtrasAndExtensions(store_json);
    
    // Test GetStoreOriginalJSONForExtrasAndExtensions
    bool retrieved = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)retrieved; // Use variable to avoid unused warning
    
    // Test SetSerializeDefaultValues with fuzzer-controlled value
    bool serialize_defaults = false;
    if (Size > 1) {
        serialize_defaults = (Data[1] & 0x01) != 0;
    }
    loader.SetSerializeDefaultValues(serialize_defaults);
    
    // Create and populate a model
    tinygltf::Model model;
    populateModel(model, Data, Size);
    
    // Test WriteGltfSceneToFile with various combinations
    const std::string filename = "./dummy_file";
    
    // Generate parameters from fuzzer data
    bool embedImages = false;
    bool embedBuffers = false;
    bool prettyPrint = false;
    bool writeBinary = false;
    
    if (Size > 2) {
        embedImages = (Data[2] & 0x01) != 0;
        embedBuffers = (Data[2] & 0x02) != 0;
        prettyPrint = (Data[2] & 0x04) != 0;
        writeBinary = (Data[2] & 0x08) != 0;
    }
    
    // Try to write to file, catching any exceptions
    try {
        loader.WriteGltfSceneToFile(&model, filename, embedImages, embedBuffers, 
                                   prettyPrint, writeBinary);
    } catch (...) {
        // Ignore exceptions from invalid UTF-8 or other issues
    }
    
    // Test WriteGltfSceneToStream with various combinations
    std::ostringstream stream;
    
    // Use different parameters for stream writing
    bool streamPrettyPrint = false;
    bool streamWriteBinary = false;
    
    if (Size > 3) {
        streamPrettyPrint = (Data[3] & 0x01) != 0;
        streamWriteBinary = (Data[3] & 0x02) != 0;
    }
    
    // Try to write to stream, catching any exceptions
    try {
        loader.WriteGltfSceneToStream(&model, stream, streamPrettyPrint, streamWriteBinary);
    } catch (...) {
        // Ignore exceptions from invalid UTF-8 or other issues
    }
    
    // Clean up dummy file if it was created
    std::remove(filename.c_str());
    
    return 0;
}