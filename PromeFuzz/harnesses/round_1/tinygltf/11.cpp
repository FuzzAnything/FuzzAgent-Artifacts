// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.cc:1105:16 in tiny_gltf.h
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

static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set asset version
    model.asset.version = "2.0";
    
    // Create a buffer with some data
    tinygltf::Buffer buffer;
    buffer.name = "test_buffer";
    buffer.data.resize(std::min(size, static_cast<size_t>(100)));
    if (size > 0) {
        memcpy(buffer.data.data(), data, std::min(size, static_cast<size_t>(100)));
    }
    model.buffers.push_back(buffer);
    
    // Create a buffer view
    tinygltf::BufferView bufferView;
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = buffer.data.size();
    model.bufferViews.push_back(bufferView);
    
    // Create an accessor
    tinygltf::Accessor accessor;
    accessor.bufferView = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
    accessor.count = buffer.data.size();
    accessor.type = TINYGLTF_TYPE_SCALAR;
    model.accessors.push_back(accessor);
    
    // Create an image with some data
    tinygltf::Image image;
    image.name = "test_image";
    image.width = 16;
    image.height = 16;
    image.component = 4;
    image.bits = 8;
    image.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
    image.image.resize(16 * 16 * 4);
    if (size > 0) {
        for (size_t i = 0; i < image.image.size() && i < size; ++i) {
            image.image[i] = data[i % size];
        }
    }
    model.images.push_back(image);
    
    // Create a scene
    tinygltf::Scene scene;
    scene.name = "test_scene";
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    // Set extras with fuzzer data (may contain invalid UTF-8)
    std::string fuzz_data(data, data + std::min(size, static_cast<size_t>(100)));
    model.asset.extras = tinygltf::Value(std::move(fuzz_data));
}

static bool dummyImageWriter(const std::string* basepath, const std::string* filename,
                             const tinygltf::Image* image, bool embedImages,
                             const tinygltf::FsCallbacks* fs, const tinygltf::URICallbacks* uri_cb,
                             std::string* out_uri, void* user_data) {
    // Dummy implementation that does nothing
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    
    // Populate model with fuzzer data
    populateModel(model, Data, Size);
    
    // Use fuzzer data to determine various boolean parameters
    uint8_t flags = Data[0];
    bool embedImages = (flags & 0x01) != 0;
    bool embedBuffers = (flags & 0x02) != 0;
    bool prettyPrint = (flags & 0x04) != 0;
    bool writeBinary = (flags & 0x08) != 0;
    bool serializeDefaults = (flags & 0x10) != 0;
    bool storeOriginalJSON = (flags & 0x20) != 0;
    
    // Test SetSerializeDefaultValues
    loader.SetSerializeDefaultValues(serializeDefaults);
    
    // Test SetStoreOriginalJSONForExtrasAndExtensions
    loader.SetStoreOriginalJSONForExtrasAndExtensions(storeOriginalJSON);
    
    // Test GetStoreOriginalJSONForExtrasAndExtensions
    bool retrieved = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)retrieved; // Use variable to avoid unused warning
    
    // Test SetImageWriter
    void* user_data = nullptr;
    loader.SetImageWriter(dummyImageWriter, user_data);
    
    // Test WriteGltfSceneToFile
    const std::string filename = "./dummy_file";
    bool success = loader.WriteGltfSceneToFile(&model, filename, 
                                              embedImages, embedBuffers,
                                              prettyPrint, writeBinary);
    (void)success; // Use variable to avoid unused warning
    
    // Clean up the dummy file
    std::remove(filename.c_str());
    
    // Test WriteGltfSceneToStream
    std::ostringstream stream;
    bool streamSuccess = loader.WriteGltfSceneToStream(&model, stream,
                                                      prettyPrint, writeBinary);
    (void)streamSuccess; // Use variable to avoid unused warning
    
    return 0;
}