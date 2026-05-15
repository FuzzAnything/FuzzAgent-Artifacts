// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.cc:1105:16 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
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
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "tiny_gltf.h"

static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set basic asset info
    model.asset.version = "2.0";
    
    // Create a minimal valid model with some data from fuzzer input
    if (size > 0) {
        // Add a buffer with some data
        tinygltf::Buffer buffer;
        buffer.data.assign(data, data + std::min(size, size_t(1024)));
        buffer.name = "fuzz_buffer";
        model.buffers.push_back(buffer);
        
        // Add a buffer view
        tinygltf::BufferView bufferView;
        bufferView.buffer = 0;
        bufferView.byteOffset = 0;
        bufferView.byteLength = std::min(size, size_t(1024));
        model.bufferViews.push_back(bufferView);
        
        // Add an accessor
        tinygltf::Accessor accessor;
        accessor.bufferView = 0;
        accessor.byteOffset = 0;
        accessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
        accessor.count = std::min(size, size_t(1024));
        accessor.type = TINYGLTF_TYPE_SCALAR;
        model.accessors.push_back(accessor);
        
        // Add a node
        tinygltf::Node node;
        node.name = "fuzz_node";
        model.nodes.push_back(node);
        
        // Add a scene
        tinygltf::Scene scene;
        scene.nodes.push_back(0);
        model.scenes.push_back(scene);
        model.defaultScene = 0;
        
        // Add extras with fuzzer data (may contain invalid UTF-8)
        std::string fuzz_data(reinterpret_cast<const char*>(data), std::min(size, size_t(100)));
        model.asset.extras = tinygltf::Value(fuzz_data);
    }
}

static bool dummyImageWriter(const std::string* basepath, const std::string* filename, 
                            const tinygltf::Image* image, bool embedImages,
                            const tinygltf::FsCallbacks* fs, const tinygltf::URICallbacks* uri,
                            std::string* out_uri, void* user_data) {
    if (out_uri && filename) {
        *out_uri = *filename;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Initialize TinyGLTF object
    tinygltf::TinyGLTF loader;
    
    // Create a model and populate it with fuzzer data
    tinygltf::Model model;
    populateModel(model, Data, Size);
    
    // Use fuzzer data to determine various settings
    if (Size > 0) {
        // Set serialization options based on fuzzer input
        bool serializeDefaults = (Data[0] & 0x01) != 0;
        bool storeOriginalJSON = (Data[0] & 0x02) != 0;
        bool useCustomImageWriter = (Data[0] & 0x04) != 0;
        
        loader.SetSerializeDefaultValues(serializeDefaults);
        loader.SetStoreOriginalJSONForExtrasAndExtensions(storeOriginalJSON);
        
        if (useCustomImageWriter) {
            loader.SetImageWriter(dummyImageWriter, nullptr);
        }
        
        // Test GetStoreOriginalJSONForExtrasAndExtensions
        bool getResult = loader.GetStoreOriginalJSONForExtrasAndExtensions();
        (void)getResult; // Suppress unused variable warning
        
        // Determine write parameters from fuzzer data
        bool embedImages = (Data[0] & 0x08) != 0;
        bool embedBuffers = (Data[0] & 0x10) != 0;
        bool prettyPrint = (Data[0] & 0x20) != 0;
        bool writeBinary = (Data[0] & 0x40) != 0;
        
        // Try writing to file
        const std::string filename = "./dummy_file";
        try {
            loader.WriteGltfSceneToFile(&model, filename, embedImages, embedBuffers, 
                                       prettyPrint, writeBinary);
        } catch (...) {
            // Catch any exceptions from invalid UTF-8 or other issues
        }
        
        // Try writing to stream
        std::ostringstream stream;
        try {
            loader.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);
        } catch (...) {
            // Catch any exceptions from invalid UTF-8 or other issues
        }
        
        // Clean up dummy file if it was created
        std::remove(filename.c_str());
    }
    
    return 0;
}