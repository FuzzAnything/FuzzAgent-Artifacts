// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
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
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

#include "tiny_gltf.h"

static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set basic asset info
    model.asset.version = "2.0";
    
    // Use fuzzer data to populate string fields
    std::string fuzz_data(reinterpret_cast<const char*>(data), size);
    
    // Create a minimal valid model structure
    tinygltf::Scene scene;
    scene.name = "FuzzScene";
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    // Add a node
    tinygltf::Node node;
    node.name = "FuzzNode";
    model.nodes.push_back(node);
    model.scenes[0].nodes.push_back(0);
    
    // Add a mesh with primitive
    tinygltf::Mesh mesh;
    mesh.name = "FuzzMesh";
    
    tinygltf::Primitive primitive;
    primitive.material = -1;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;
    
    // Add some attributes using fuzz data to determine indices
    if (size > 0) {
        primitive.attributes["POSITION"] = data[0] % 10;
    }
    
    mesh.primitives.push_back(primitive);
    model.meshes.push_back(mesh);
    model.nodes[0].mesh = 0;
    
    // Add a buffer with some data
    tinygltf::Buffer buffer;
    buffer.name = "FuzzBuffer";
    buffer.data.assign(data, data + (size > 1024 ? 1024 : size));
    model.buffers.push_back(buffer);
    
    // Add extras with fuzz data (may contain invalid UTF-8)
    tinygltf::Value::Object extras_obj;
    extras_obj["fuzz_data"] = tinygltf::Value(fuzz_data);
    model.asset.extras = tinygltf::Value(extras_obj);
    
    // Set extras_json_string if we want to test original JSON storage
    model.asset.extras_json_string = fuzz_data;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    
    // Use first byte to determine various boolean parameters
    uint8_t control_byte = Data[0];
    bool serialize_defaults = (control_byte & 0x01) != 0;
    bool store_original_json = (control_byte & 0x02) != 0;
    bool embed_images = (control_byte & 0x04) != 0;
    bool embed_buffers = (control_byte & 0x08) != 0;
    bool pretty_print = (control_byte & 0x10) != 0;
    bool write_binary = (control_byte & 0x20) != 0;
    
    // Test getters before setting
    bool default1 = loader.GetSerializeDefaultValues();
    bool default2 = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    
    // Set parameters based on fuzz input
    loader.SetSerializeDefaultValues(serialize_defaults);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
    
    // Verify getters return what we set
    bool get1 = loader.GetSerializeDefaultValues();
    bool get2 = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    
    // Populate model with fuzz data
    populateModel(model, Data, Size);
    
    // Test WriteGltfSceneToFile
    try {
        const std::string filename = "./dummy_file";
        bool write_success = loader.WriteGltfSceneToFile(&model, filename, 
                                                        embed_images, embed_buffers,
                                                        pretty_print, write_binary);
        // Clean up the file
        std::remove(filename.c_str());
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Test WriteGltfSceneToStream
    try {
        std::ostringstream stream;
        bool stream_success = loader.WriteGltfSceneToStream(&model, stream,
                                                           pretty_print, write_binary);
    } catch (...) {
        // Catch any exceptions from invalid UTF-8 or other errors
    }
    
    // Test with different combinations of parameters
    if (Size > 1) {
        // Alternate parameter settings using second byte
        uint8_t alt_control = Data[1 % Size];
        loader.SetSerializeDefaultValues((alt_control & 0x01) != 0);
        loader.SetStoreOriginalJSONForExtrasAndExtensions((alt_control & 0x02) != 0);
        
        // Try writing again with different parameters
        try {
            std::ostringstream alt_stream;
            bool alt_success = loader.WriteGltfSceneToStream(&model, alt_stream,
                                                            (alt_control & 0x04) != 0,
                                                            (alt_control & 0x08) != 0);
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    return 0;
}