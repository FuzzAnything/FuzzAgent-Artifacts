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
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>

#include "tiny_gltf.h"

// Dummy image writer callback for SetImageWriter with correct signature
static bool dummyImageWriter(const std::string* basepath,
                             const std::string* filename,
                             const tinygltf::Image* image,
                             bool embedImages,
                             const tinygltf::FsCallbacks* fs,
                             const tinygltf::URICallbacks* uri,
                             std::string* out_uri,
                             void* user_data) {
    // Do nothing, just return success
    return true;
}

// Helper function to populate a minimal Model with fuzzer data
static void populateModel(tinygltf::Model* model, const uint8_t* data, size_t size) {
    // Set basic asset info
    model->asset.version = "2.0";
    
    // Create a minimal scene
    tinygltf::Scene scene;
    scene.name = "TestScene";
    model->scenes.push_back(scene);
    model->defaultScene = 0;
    
    // Create a node
    tinygltf::Node node;
    node.name = "TestNode";
    model->nodes.push_back(node);
    
    // Add the node to the scene
    model->scenes[0].nodes.push_back(0);
    
    // Create a simple buffer with fuzzer data (limited to avoid excessive memory)
    tinygltf::Buffer buffer;
    buffer.name = "TestBuffer";
    size_t bufferSize = size > 1024 ? 1024 : size; // Limit buffer size
    buffer.data.assign(data, data + bufferSize);
    model->buffers.push_back(buffer);
    
    // Create a buffer view
    tinygltf::BufferView bufferView;
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = bufferSize;
    model->bufferViews.push_back(bufferView);
    
    // Create an accessor
    tinygltf::Accessor accessor;
    accessor.bufferView = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor.type = TINYGLTF_TYPE_VEC3;
    accessor.count = bufferSize / (3 * sizeof(float));
    if (accessor.count == 0) accessor.count = 1;
    model->accessors.push_back(accessor);
    
    // Create a mesh with a primitive
    tinygltf::Mesh mesh;
    mesh.name = "TestMesh";
    
    tinygltf::Primitive primitive;
    primitive.material = 0;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;
    primitive.attributes["POSITION"] = 0;
    mesh.primitives.push_back(primitive);
    
    model->meshes.push_back(mesh);
    
    // Create a material
    tinygltf::Material material;
    material.name = "TestMaterial";
    material.pbrMetallicRoughness.baseColorFactor = {1.0, 1.0, 1.0, 1.0};
    material.pbrMetallicRoughness.metallicFactor = 0.0;
    material.pbrMetallicRoughness.roughnessFactor = 1.0;
    model->materials.push_back(material);
    
    // Add some fuzzer data to extras (may contain invalid UTF-8)
    if (size > 0) {
        // Use a portion of fuzzer data as string for extras
        size_t strSize = size > 256 ? 256 : size;
        std::string fuzz_str(reinterpret_cast<const char*>(data), strSize);
        
        // Add to asset extras
        tinygltf::Value::Object extras_obj;
        extras_obj["fuzz_data"] = tinygltf::Value(fuzz_str);
        model->asset.extras = tinygltf::Value(extras_obj);
        
        // Also add to scene extras
        tinygltf::Value::Object scene_extras;
        scene_extras["test"] = tinygltf::Value(fuzz_str);
        model->scenes[0].extras = tinygltf::Value(scene_extras);
    }
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Initialize TinyGLTF object
    tinygltf::TinyGLTF loader;
    
    // Use first byte to determine configuration flags
    uint8_t config = Size > 0 ? Data[0] : 0;
    const uint8_t* modelData = Size > 1 ? Data + 1 : Data;
    size_t modelDataSize = Size > 1 ? Size - 1 : Size;
    
    // Set configuration based on fuzzer input
    bool storeOriginalJSON = (config & 0x01) != 0;
    bool serializeDefaults = (config & 0x02) != 0;
    
    loader.SetStoreOriginalJSONForExtrasAndExtensions(storeOriginalJSON);
    loader.SetSerializeDefaultValues(serializeDefaults);
    
    // Set dummy image writer
    loader.SetImageWriter(dummyImageWriter, nullptr);
    
    // Verify GetStoreOriginalJSONForExtrasAndExtensions returns what we set
    bool retrieved = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    if (retrieved != storeOriginalJSON) {
        // Mismatch, but continue fuzzing
    }
    
    // Create and populate a model
    tinygltf::Model model;
    populateModel(&model, modelData, modelDataSize);
    
    // Prepare file path for WriteGltfSceneToFile
    const std::string filename = "./dummy_file";
    
    // Use bits from config to determine write parameters
    bool embedImages = (config & 0x04) != 0;
    bool embedBuffers = (config & 0x08) != 0;
    bool prettyPrint = (config & 0x10) != 0;
    bool writeBinary = (config & 0x20) != 0;
    
    // Try WriteGltfSceneToFile - catch exceptions from JSON library
    try {
        bool writeSuccess = loader.WriteGltfSceneToFile(&model, filename,
                                                        embedImages, embedBuffers,
                                                        prettyPrint, writeBinary);
        // Ignore result - we're just fuzzing
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Try WriteGltfSceneToStream - also catch exceptions
    try {
        std::ostringstream stream;
        bool streamSuccess = loader.WriteGltfSceneToStream(&model, stream,
                                                           prettyPrint, writeBinary);
        // Ignore result - we're just fuzzing
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Clean up: remove the dummy file if it was created
    std::remove(filename.c_str());
    
    return 0;
}