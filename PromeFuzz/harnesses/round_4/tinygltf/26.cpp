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
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

static bool dummyImageWriter(const std::string* basepath, const std::string* filename,
                             const tinygltf::Image* image, bool embedImages,
                             const tinygltf::FsCallbacks* fs, const tinygltf::URICallbacks* uri,
                             std::string* out_uri, void* user_data) {
    // Return true to indicate success
    return true;
}

static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set basic asset info
    model.asset.version = "2.0";
    
    // Create a simple buffer with fuzzer data
    tinygltf::Buffer buffer;
    buffer.name = "fuzz_buffer";
    buffer.data.assign(data, data + (size > 1024 ? 1024 : size)); // Limit size
    
    // Create buffer view
    tinygltf::BufferView bufferView;
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = buffer.data.size();
    
    // Create accessor
    tinygltf::Accessor accessor;
    accessor.bufferView = 0;
    accessor.byteOffset = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_BYTE;
    accessor.count = buffer.data.size();
    accessor.type = TINYGLTF_TYPE_SCALAR;
    
    // Create primitive
    tinygltf::Primitive primitive;
    primitive.material = -1;
    primitive.mode = TINYGLTF_MODE_POINTS;
    
    // Create mesh
    tinygltf::Mesh mesh;
    mesh.name = "fuzz_mesh";
    mesh.primitives.push_back(primitive);
    
    // Create node
    tinygltf::Node node;
    node.mesh = 0;
    
    // Create scene
    tinygltf::Scene scene;
    scene.name = "fuzz_scene";
    scene.nodes.push_back(0);
    
    // Add extras as binary data (vector of unsigned char)
    size_t extras_len = size > 256 ? 256 : size;
    std::vector<unsigned char> extras_data(data, data + extras_len);
    
    // Set extras on various components using binary data
    // Use the correct constructor for binary data
    model.extras = tinygltf::Value(std::move(extras_data));
    
    // Create new vectors for buffer and mesh extras
    std::vector<unsigned char> buffer_extras_data(data, data + (extras_len > 128 ? 128 : extras_len));
    std::vector<unsigned char> mesh_extras_data(data + (extras_len > 128 ? 128 : 0), 
                                               data + (extras_len > 256 ? 256 : extras_len));
    
    buffer.extras = tinygltf::Value(std::move(buffer_extras_data));
    mesh.extras = tinygltf::Value(std::move(mesh_extras_data));
    
    // Add components to model
    model.buffers.push_back(buffer);
    model.bufferViews.push_back(bufferView);
    model.accessors.push_back(accessor);
    model.meshes.push_back(mesh);
    model.nodes.push_back(node);
    model.scenes.push_back(scene);
    model.defaultScene = 0;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    tinygltf::TinyGLTF loader;
    
    // Use first byte to control boolean parameters
    uint8_t control_byte = Data[0];
    bool store_json = (control_byte & 0x01) != 0;
    bool serialize_defaults = (control_byte & 0x02) != 0;
    bool embed_images = (control_byte & 0x04) != 0;
    bool embed_buffers = (control_byte & 0x08) != 0;
    bool pretty_print = (control_byte & 0x10) != 0;
    bool write_binary = (control_byte & 0x20) != 0;
    
    // Set various options
    loader.SetStoreOriginalJSONForExtrasAndExtensions(store_json);
    loader.SetSerializeDefaultValues(serialize_defaults);
    
    // Set dummy image writer
    loader.SetImageWriter(dummyImageWriter, nullptr);
    
    // Check current setting
    bool current_setting = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)current_setting; // Use result to avoid unused variable warning
    
    // Create and populate model
    tinygltf::Model model;
    populateModel(model, Data, Size);
    
    // Try writing to file
    const std::string filename = "./dummy_file";
    
    try {
        bool success = loader.WriteGltfSceneToFile(&model, filename,
                                                  embed_images, embed_buffers,
                                                  pretty_print, write_binary);
        (void)success; // Use result to avoid unused variable warning
        
        // Clean up the file if it was created
        std::remove(filename.c_str());
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
        std::remove(filename.c_str());
    }
    
    // Try writing to stream
    try {
        std::ostringstream oss;
        bool stream_success = loader.WriteGltfSceneToStream(&model, oss,
                                                           pretty_print, write_binary);
        (void)stream_success; // Use result to avoid unused variable warning
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    return 0;
}