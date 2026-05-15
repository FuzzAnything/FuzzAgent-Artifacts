// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
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
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "tiny_gltf.h"

static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set asset version
    model.asset.version = "2.0";
    
    // Use fuzzer data for extras string (may contain invalid UTF-8)
    std::string fuzz_data(reinterpret_cast<const char*>(data), size);
    
    // Create a simple model with some data
    tinygltf::Buffer buffer;
    buffer.name = "buffer0";
    buffer.data = std::vector<unsigned char>(data, data + (size > 100 ? 100 : size));
    
    // Set extras using a different approach
    tinygltf::Value::Object extras_obj;
    extras_obj["fuzz"] = tinygltf::Value(fuzz_data);
    buffer.extras = tinygltf::Value(extras_obj);
    
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
    accessor.count = 1;
    accessor.type = TINYGLTF_TYPE_VEC3;
    model.accessors.push_back(accessor);
    
    tinygltf::Mesh mesh;
    tinygltf::Primitive primitive;
    primitive.attributes["POSITION"] = 0;
    mesh.primitives.push_back(primitive);
    model.meshes.push_back(mesh);
    
    tinygltf::Node node;
    node.mesh = 0;
    model.nodes.push_back(node);
    
    tinygltf::Scene scene;
    scene.nodes.push_back(0);
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    // Set asset extras
    tinygltf::Value::Object asset_extras_obj;
    asset_extras_obj["fuzz"] = tinygltf::Value(fuzz_data);
    model.asset.extras = tinygltf::Value(asset_extras_obj);
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    
    // Populate model with fuzzer data
    populateModel(model, Data, Size);
    
    // Test SetStoreOriginalJSONForExtrasAndExtensions and GetStoreOriginalJSONForExtrasAndExtensions
    bool storeOriginal = (Data[0] & 0x01) != 0;
    loader.SetStoreOriginalJSONForExtrasAndExtensions(storeOriginal);
    bool getStoreOriginal = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)getStoreOriginal; // Avoid unused variable warning
    
    // Test SetSerializeDefaultValues and GetSerializeDefaultValues
    bool serializeDefaults = (Data[0] & 0x02) != 0;
    loader.SetSerializeDefaultValues(serializeDefaults);
    bool getSerializeDefaults = loader.GetSerializeDefaultValues();
    (void)getSerializeDefaults; // Avoid unused variable warning
    
    // Test WriteGltfSceneToStream with different combinations
    bool prettyPrint = (Data[0] & 0x04) != 0;
    bool writeBinary = (Data[0] & 0x08) != 0;
    
    std::ostringstream stream;
    try {
        loader.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 in JSON serialization
    }
    
    // Test WriteGltfSceneToFile with different combinations
    bool embedImages = (Data[0] & 0x10) != 0;
    bool embedBuffers = (Data[0] & 0x20) != 0;
    prettyPrint = (Data[0] & 0x40) != 0;
    writeBinary = (Data[0] & 0x80) != 0;
    
    const std::string filename = "./dummy_file";
    try {
        loader.WriteGltfSceneToFile(&model, filename, embedImages, embedBuffers, 
                                   prettyPrint, writeBinary);
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or file operations
    }
    
    // Clean up dummy file
    std::remove(filename.c_str());
    
    return 0;
}