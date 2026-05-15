// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
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
#include "tiny_gltf.h"
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

static bool isValidUTF8(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        uint8_t c = data[i];
        if (c <= 0x7F) continue;
        else if (c >= 0xC2 && c <= 0xDF) {
            if (i + 1 >= size || (data[i + 1] & 0xC0) != 0x80) return false;
            ++i;
        } else if (c >= 0xE0 && c <= 0xEF) {
            if (i + 2 >= size || (data[i + 1] & 0xC0) != 0x80 || (data[i + 2] & 0xC0) != 0x80) return false;
            i += 2;
        } else if (c >= 0xF0 && c <= 0xF4) {
            if (i + 3 >= size || (data[i + 1] & 0xC0) != 0x80 || (data[i + 2] & 0xC0) != 0x80 || (data[i + 3] & 0xC0) != 0x80) return false;
            i += 3;
        } else {
            return false;
        }
    }
    return true;
}

static void populateModel(tinygltf::Model &model, const uint8_t *data, size_t size) {
    // Set basic asset info
    model.asset.version = "2.0";
    
    // Create a simple node
    tinygltf::Node node;
    node.name = "TestNode";
    model.nodes.push_back(node);
    
    // Create a simple scene
    tinygltf::Scene scene;
    scene.name = "TestScene";
    scene.nodes.push_back(0); // Reference the node
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    // Add a simple buffer with fuzzer data
    tinygltf::Buffer buffer;
    buffer.name = "TestBuffer";
    if (size > 0) {
        buffer.data.assign(data, data + std::min(size, static_cast<size_t>(1000)));
    }
    model.buffers.push_back(buffer);
    
    // Add a buffer view
    tinygltf::BufferView bufferView;
    bufferView.name = "TestBufferView";
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    if (!buffer.data.empty()) {
        bufferView.byteLength = buffer.data.size();
    }
    model.bufferViews.push_back(bufferView);
    
    // Add an accessor
    tinygltf::Accessor accessor;
    accessor.name = "TestAccessor";
    accessor.bufferView = 0;
    accessor.byteOffset = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor.type = TINYGLTF_TYPE_VEC3;
    accessor.count = buffer.data.size() / 12; // Rough estimate
    if (accessor.count == 0) accessor.count = 1;
    model.accessors.push_back(accessor);
    
    // Add a simple mesh
    tinygltf::Mesh mesh;
    mesh.name = "TestMesh";
    
    tinygltf::Primitive primitive;
    primitive.material = -1;
    primitive.attributes["POSITION"] = 0; // Reference the accessor
    mesh.primitives.push_back(primitive);
    model.meshes.push_back(mesh);
    
    // Update node to reference mesh
    model.nodes[0].mesh = 0;
    
    // Add extras from fuzzer data (limited to avoid excessive memory)
    if (size > 0) {
        size_t extras_size = std::min(size, static_cast<size_t>(100));
        if (isValidUTF8(data, extras_size)) {
            std::string fuzz_data(reinterpret_cast<const char*>(data), extras_size);
            tinygltf::Value::Object extras_obj;
            extras_obj["fuzz_data"] = tinygltf::Value(fuzz_data);
            model.extras = tinygltf::Value(extras_obj);
        } else {
            std::string safe_data(extras_size, 'A');
            tinygltf::Value::Object extras_obj;
            extras_obj["fuzz_data"] = tinygltf::Value(safe_data);
            model.extras = tinygltf::Value(extras_obj);
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Initialize TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Use fuzzer data to determine boolean parameters
    bool serialize_defaults = false;
    bool store_original_json = false;
    bool embed_images = false;
    bool embed_buffers = false;
    bool pretty_print = false;
    bool write_binary = false;
    
    if (Size > 0) {
        // Use first byte to determine some boolean values
        uint8_t flags = Data[0];
        serialize_defaults = (flags & 0x01) != 0;
        store_original_json = (flags & 0x02) != 0;
        embed_images = (flags & 0x04) != 0;
        embed_buffers = (flags & 0x08) != 0;
        pretty_print = (flags & 0x10) != 0;
        write_binary = (flags & 0x20) != 0;
    }
    
    // Test Set/Get functions
    loader.SetSerializeDefaultValues(serialize_defaults);
    bool get_serialize = loader.GetSerializeDefaultValues();
    (void)get_serialize; // Prevent unused variable warning
    
    loader.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
    bool get_store_json = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)get_store_json; // Prevent unused variable warning
    
    // Create and populate model
    tinygltf::Model model;
    if (Size > 1) {
        populateModel(model, Data + 1, Size - 1);
    } else {
        populateModel(model, Data, Size);
    }
    
    // Test WriteGltfSceneToFile with exception handling
    const std::string filename = "./dummy_file.gltf";
    bool write_result = false;
    try {
        write_result = loader.WriteGltfSceneToFile(&model, filename, 
                                                   embed_images, embed_buffers,
                                                   pretty_print, write_binary);
    } catch (const std::exception& e) {
        // Ignore exceptions during fuzzing
    }
    (void)write_result; // Prevent unused variable warning
    
    // Clean up the file
    std::remove(filename.c_str());
    
    // Test WriteGltfSceneToStream with exception handling
    std::ostringstream stream;
    bool stream_result = false;
    try {
        stream_result = loader.WriteGltfSceneToStream(&model, stream,
                                                      pretty_print, write_binary);
    } catch (const std::exception& e) {
        // Ignore exceptions during fuzzing
    }
    (void)stream_result; // Prevent unused variable warning
    
    // Test with different combinations of parameters
    if (Size > 10) {
        // Try with opposite settings
        loader.SetSerializeDefaultValues(!serialize_defaults);
        loader.SetStoreOriginalJSONForExtrasAndExtensions(!store_original_json);
        
        // Write again with different parameters
        std::ostringstream stream2;
        try {
            loader.WriteGltfSceneToStream(&model, stream2,
                                         !pretty_print, !write_binary);
        } catch (const std::exception& e) {
            // Ignore exceptions during fuzzing
        }
        
        // Write to file with different parameters
        const std::string filename2 = "./dummy_file2.gltf";
        try {
            loader.WriteGltfSceneToFile(&model, filename2,
                                       !embed_images, !embed_buffers,
                                       !pretty_print, !write_binary);
        } catch (const std::exception& e) {
            // Ignore exceptions during fuzzing
        }
        std::remove(filename2.c_str());
    }
    
    return 0;
}