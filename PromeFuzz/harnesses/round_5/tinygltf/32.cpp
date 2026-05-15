// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.cc:1105:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
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
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set a minimal asset
    model.asset.version = "2.0";
    
    // Use fuzzer data to populate extras string (may contain invalid UTF-8)
    if (size > 0) {
        // Create a string from the fuzzer data for extras
        std::string fuzz_data(reinterpret_cast<const char*>(data), size);
        
        // Set extras on various components to exercise string handling
        tinygltf::Value extras_value;
        model.asset.extras = tinygltf::Value(fuzz_data);
        
        // Add a simple scene
        tinygltf::Scene scene;
        scene.name = "FuzzScene";
        scene.extras = tinygltf::Value(fuzz_data);
        model.scenes.push_back(scene);
        
        // Add a node
        tinygltf::Node node;
        node.name = "FuzzNode";
        node.extras = tinygltf::Value(fuzz_data);
        model.nodes.push_back(node);
        
        // Add a minimal buffer
        tinygltf::Buffer buffer;
        buffer.name = "FuzzBuffer";
        if (size > 0) {
            buffer.data.assign(data, data + std::min(size, static_cast<size_t>(100)));
        }
        buffer.extras = tinygltf::Value(fuzz_data);
        model.buffers.push_back(buffer);
        
        // Add a buffer view
        tinygltf::BufferView bufferView;
        bufferView.name = "FuzzBufferView";
        bufferView.buffer = 0;
        bufferView.byteOffset = 0;
        if (!buffer.data.empty()) {
            bufferView.byteLength = buffer.data.size();
        }
        bufferView.extras = tinygltf::Value(fuzz_data);
        model.bufferViews.push_back(bufferView);
        
        // Add an accessor
        tinygltf::Accessor accessor;
        accessor.name = "FuzzAccessor";
        accessor.bufferView = 0;
        accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        accessor.type = TINYGLTF_TYPE_VEC3;
        accessor.count = 1;
        accessor.extras = tinygltf::Value(fuzz_data);
        model.accessors.push_back(accessor);
        
        // Add a mesh with primitive
        tinygltf::Mesh mesh;
        mesh.name = "FuzzMesh";
        tinygltf::Primitive primitive;
        primitive.material = -1;
        primitive.mode = TINYGLTF_MODE_TRIANGLES;
        primitive.attributes["POSITION"] = 0;
        primitive.extras = tinygltf::Value(fuzz_data);
        mesh.primitives.push_back(primitive);
        mesh.extras = tinygltf::Value(fuzz_data);
        model.meshes.push_back(mesh);
        
        // Set the default scene
        model.defaultScene = 0;
    }
}

static bool dummyWriteImageData(const std::string* basepath, 
                                const std::string* filename, 
                                const tinygltf::Image* image, 
                                bool embedImages,
                                const tinygltf::FsCallbacks* fs,
                                const tinygltf::URICallbacks* uri,
                                std::string* out_err,
                                void* user_data) {
    // Dummy implementation that does nothing
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create model and populate with fuzzer data
    tinygltf::Model model;
    populateModel(model, Data, Size);
    
    // Create TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Use fuzzer data to determine various boolean parameters
    // Use first byte for control flags
    uint8_t control_flags = Size > 0 ? Data[0] : 0;
    
    // Set various options based on control flags
    loader.SetSerializeDefaultValues((control_flags & 0x01) != 0);
    loader.SetStoreOriginalJSONForExtrasAndExtensions((control_flags & 0x02) != 0);
    
    // Test GetStoreOriginalJSONForExtrasAndExtensions
    bool storeOriginal = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)storeOriginal; // Suppress unused variable warning
    
    // Set a dummy image writer
    loader.SetImageWriter(dummyWriteImageData, nullptr);
    
    // Test WriteGltfSceneToFile with different combinations
    const std::string filename = "./dummy_file";
    
    // Combination 1: JSON format, not embedded, not pretty
    try {
        loader.WriteGltfSceneToFile(&model, filename, 
                                   (control_flags & 0x04) != 0,  // embedImages
                                   (control_flags & 0x08) != 0,  // embedBuffers
                                   (control_flags & 0x10) != 0,  // prettyPrint
                                   (control_flags & 0x20) != 0); // writeBinary
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 in JSON serialization
    }
    
    // Combination 2: Different parameter combination
    try {
        loader.WriteGltfSceneToFile(&model, filename,
                                   (control_flags & 0x40) != 0,
                                   (control_flags & 0x80) != 0,
                                   (control_flags & 0x01) != 0,
                                   (control_flags & 0x02) != 0);
    } catch (...) {
        // Catch any exceptions
    }
    
    // Test WriteGltfSceneToStream
    std::ostringstream stream1;
    try {
        loader.WriteGltfSceneToStream(&model, stream1,
                                     (control_flags & 0x04) != 0,  // prettyPrint
                                     (control_flags & 0x08) != 0); // writeBinary
    } catch (...) {
        // Catch any exceptions from invalid UTF-8
    }
    
    // Test with different stream and parameters
    std::ostringstream stream2;
    try {
        loader.WriteGltfSceneToStream(&model, stream2,
                                     (control_flags & 0x10) != 0,
                                     (control_flags & 0x20) != 0);
    } catch (...) {
        // Catch any exceptions
    }
    
    // Clean up the dummy file if it was created
    std::remove(filename.c_str());
    
    return 0;
}