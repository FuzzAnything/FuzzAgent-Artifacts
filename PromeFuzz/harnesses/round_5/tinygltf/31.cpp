// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.cc:1105:16 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
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
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set basic asset info
    model.asset.version = "2.0";
    
    // Use fuzzer data to populate extras string (may contain invalid UTF-8)
    std::string fuzz_data(reinterpret_cast<const char*>(data), size);
    
    // Populate some fields with fuzzer data to increase coverage
    if (size > 0) {
        // Create a minimal valid model structure
        tinygltf::Scene scene;
        scene.name = "FuzzScene";
        
        tinygltf::Node node;
        node.name = "FuzzNode";
        
        tinygltf::Mesh mesh;
        mesh.name = "FuzzMesh";
        
        tinygltf::Primitive primitive;
        primitive.material = 0;
        
        tinygltf::Material material;
        material.name = "FuzzMaterial";
        
        tinygltf::Buffer buffer;
        buffer.name = "FuzzBuffer";
        if (size > 0) {
            buffer.data.assign(data, data + std::min(size, static_cast<size_t>(100)));
        }
        
        tinygltf::BufferView bufferView;
        bufferView.name = "FuzzBufferView";
        bufferView.buffer = 0;
        bufferView.byteOffset = 0;
        if (buffer.data.size() > 0) {
            bufferView.byteLength = buffer.data.size();
        }
        
        tinygltf::Accessor accessor;
        accessor.name = "FuzzAccessor";
        accessor.bufferView = 0;
        accessor.byteOffset = 0;
        accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        accessor.type = TINYGLTF_TYPE_VEC3;
        accessor.count = 1;
        
        // Add objects to model
        model.scenes.push_back(scene);
        model.nodes.push_back(node);
        model.meshes.push_back(mesh);
        model.materials.push_back(material);
        model.buffers.push_back(buffer);
        model.bufferViews.push_back(bufferView);
        model.accessors.push_back(accessor);
        
        // Set scene index
        model.defaultScene = 0;
        model.scenes[0].nodes.push_back(0);
        model.meshes[0].primitives.push_back(primitive);
        
        // Add some dummy extensions data
        model.extensions["KHR_materials_unlit"] = tinygltf::Value();
        
        // Add extras to various objects using string assignment
        model.scenes[0].extras = tinygltf::Value(fuzz_data);
        model.nodes[0].extras = tinygltf::Value(fuzz_data);
        model.meshes[0].extras = tinygltf::Value(fuzz_data);
        model.materials[0].extras = tinygltf::Value(fuzz_data);
        
        // Set extras_json_string for structs that support it
        model.meshes[0].primitives[0].extras_json_string = fuzz_data;
    }
}

static bool dummyWriteImageData(const std::string* basepath, 
                                const std::string* filename, 
                                const tinygltf::Image* image, 
                                bool embedImages,
                                const tinygltf::FsCallbacks* fsCallbacks,
                                const tinygltf::URICallbacks* uriCallbacks,
                                std::string* out_uri,
                                void* user_data) {
    // Dummy implementation for image writing callback
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Initialize TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Create and populate model
    tinygltf::Model model;
    populateModel(model, Data, Size);
    
    // Use fuzzer data to determine various boolean flags
    // Use first byte for flags, rest for data
    uint8_t flags = Size > 0 ? Data[0] : 0;
    bool embedImages = (flags & 0x01) != 0;
    bool embedBuffers = (flags & 0x02) != 0;
    bool prettyPrint = (flags & 0x04) != 0;
    bool writeBinary = (flags & 0x08) != 0;
    bool serializeDefaults = (flags & 0x10) != 0;
    bool storeOriginalJSON = (flags & 0x20) != 0;
    
    // Set various options based on fuzzer input
    loader.SetSerializeDefaultValues(serializeDefaults);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(storeOriginalJSON);
    
    // Set dummy image writer callback
    loader.SetImageWriter(dummyWriteImageData, nullptr);
    
    // Check GetStoreOriginalJSONForExtrasAndExtensions
    bool getStoreOriginal = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)getStoreOriginal; // Mark as used
    
    // Write to file
    const std::string filename = "./dummy_file";
    try {
        bool success = loader.WriteGltfSceneToFile(&model, filename, 
                                                   embedImages, embedBuffers,
                                                   prettyPrint, writeBinary);
        (void)success; // Mark as used
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Write to stream
    try {
        std::ostringstream stream;
        bool success = loader.WriteGltfSceneToStream(&model, stream, 
                                                     prettyPrint, writeBinary);
        (void)success; // Mark as used
        
        // Also test with stringstream
        std::stringstream sstream;
        bool success2 = loader.WriteGltfSceneToStream(&model, sstream,
                                                      prettyPrint, writeBinary);
        (void)success2; // Mark as used
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Test with different combinations of parameters
    // Vary the boolean parameters to explore more states
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            try {
                std::ostringstream testStream;
                bool testSuccess = loader.WriteGltfSceneToStream(&model, testStream,
                                                                 i == 0, j == 0);
                (void)testSuccess; // Mark as used
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // Clean up dummy file if it was created
    std::remove(filename.c_str());
    
    return 0;
}