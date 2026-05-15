// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
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
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "tiny_gltf.h"

static void populateModel(tinygltf::Model* model, const uint8_t* data, size_t size) {
    // Initialize some basic fields with fuzzer data
    if (size > 0) {
        // Use first byte to determine some configurations
        uint8_t config = data[0];
        
        // Set asset version
        model->asset.version = "2.0";
        
        // Create a minimal buffer with fuzzer data
        if (size > 1) {
            tinygltf::Buffer buffer;
            buffer.name = "fuzz_buffer";
            buffer.data.assign(data + 1, data + std::min(size, static_cast<size_t>(1024)));
            model->buffers.push_back(buffer);
            
            // Create a buffer view
            tinygltf::BufferView bufferView;
            bufferView.name = "fuzz_buffer_view";
            bufferView.buffer = 0;
            bufferView.byteOffset = 0;
            bufferView.byteLength = buffer.data.size();
            model->bufferViews.push_back(bufferView);
            
            // Create an accessor
            tinygltf::Accessor accessor;
            accessor.name = "fuzz_accessor";
            accessor.bufferView = 0;
            accessor.byteOffset = 0;
            accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
            accessor.count = std::min(static_cast<size_t>(256), buffer.data.size() / 4);
            accessor.type = TINYGLTF_TYPE_VEC3;
            model->accessors.push_back(accessor);
            
            // Create a node with the accessor
            tinygltf::Node node;
            node.name = "fuzz_node";
            node.mesh = 0;
            model->nodes.push_back(node);
            
            // Create a scene with the node
            tinygltf::Scene scene;
            scene.name = "fuzz_scene";
            scene.nodes.push_back(0);
            model->scenes.push_back(scene);
            model->defaultScene = 0;
            
            // Create a mesh with a primitive
            tinygltf::Mesh mesh;
            mesh.name = "fuzz_mesh";
            tinygltf::Primitive primitive;
            primitive.material = 0;
            primitive.mode = TINYGLTF_MODE_TRIANGLES;
            primitive.attributes["POSITION"] = 0;
            mesh.primitives.push_back(primitive);
            model->meshes.push_back(mesh);
            
            // Create a material
            tinygltf::Material material;
            material.name = "fuzz_material";
            material.doubleSided = (config & 0x01) != 0;
            material.alphaCutoff = 0.5;
            material.emissiveFactor = {0.0, 0.0, 0.0};
            model->materials.push_back(material);
            
            // Add some extras data from fuzzer input
            if (size > 100) {
                std::string extras_str(reinterpret_cast<const char*>(data + 50), 
                                      std::min(size - 50, static_cast<size_t>(50)));
                model->extras = tinygltf::Value(extras_str);
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    try {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;
        std::string err;
        std::string warn;
        
        // Use first byte for configuration
        uint8_t config = Data[0];
        const uint8_t* gltf_data = Data + 1;
        size_t gltf_size = Size - 1;
        
        if (gltf_size == 0) {
            return 0;
        }
        
        // Set various options based on configuration bits
        bool serialize_defaults = (config & 0x01) != 0;
        bool store_original_json = (config & 0x02) != 0;
        bool embed_images = (config & 0x04) != 0;
        bool embed_buffers = (config & 0x08) != 0;
        bool pretty_print = (config & 0x10) != 0;
        bool write_binary = (config & 0x20) != 0;
        
        loader.SetSerializeDefaultValues(serialize_defaults);
        loader.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
        
        // Test GetStoreOriginalJSONForExtrasAndExtensions
        bool get_store = loader.GetStoreOriginalJSONForExtrasAndExtensions();
        (void)get_store; // Suppress unused variable warning
        
        // Try to load ASCII glTF from string
        std::string gltf_string(reinterpret_cast<const char*>(gltf_data), gltf_size);
        bool load_result = loader.LoadASCIIFromString(&model, &err, &warn, 
                                                     gltf_string.c_str(), 
                                                     static_cast<unsigned int>(gltf_string.size()),
                                                     ".", 
                                                     tinygltf::REQUIRE_VERSION);
        
        // If loading failed, create a minimal model for writing tests
        if (!load_result) {
            // Create a fresh model with fuzzer data
            tinygltf::Model fresh_model;
            populateModel(&fresh_model, Data, Size);
            
            // Test WriteGltfSceneToFile with dummy file
            const std::string filename = "./dummy_file.gltf";
            bool write_result = loader.WriteGltfSceneToFile(&fresh_model, filename,
                                                           embed_images, embed_buffers,
                                                           pretty_print, write_binary);
            (void)write_result; // Suppress unused variable warning
            
            // Test WriteGltfSceneToStream
            std::ostringstream stream;
            bool stream_result = loader.WriteGltfSceneToStream(&fresh_model, stream,
                                                              pretty_print, write_binary);
            (void)stream_result; // Suppress unused variable warning
            
            // Clean up dummy file
            std::remove(filename.c_str());
        } else {
            // Loading succeeded, test writing functions with loaded model
            
            // Test WriteGltfSceneToFile with dummy file
            const std::string filename = "./dummy_file.gltf";
            bool write_result = loader.WriteGltfSceneToFile(&model, filename,
                                                           embed_images, embed_buffers,
                                                           pretty_print, write_binary);
            (void)write_result; // Suppress unused variable warning
            
            // Test WriteGltfSceneToStream
            std::ostringstream stream;
            bool stream_result = loader.WriteGltfSceneToStream(&model, stream,
                                                              pretty_print, write_binary);
            (void)stream_result; // Suppress unused variable warning
            
            // Clean up dummy file
            std::remove(filename.c_str());
        }
        
        // Test with different configurations
        loader.SetSerializeDefaultValues(!serialize_defaults);
        loader.SetStoreOriginalJSONForExtrasAndExtensions(!store_original_json);
        
        // Create another model for additional testing
        tinygltf::Model model2;
        populateModel(&model2, Data, Size);
        
        // Test writing with opposite configuration
        const std::string filename2 = "./dummy_file2.gltf";
        loader.WriteGltfSceneToFile(&model2, filename2,
                                   !embed_images, !embed_buffers,
                                   !pretty_print, !write_binary);
        
        std::ostringstream stream2;
        loader.WriteGltfSceneToStream(&model2, stream2,
                                     !pretty_print, !write_binary);
        
        // Clean up second dummy file
        std::remove(filename2.c_str());
        
    } catch (...) {
        // Catch all exceptions to prevent crashes from propagating
    }
    
    return 0;
}