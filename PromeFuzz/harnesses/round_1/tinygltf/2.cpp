// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.cc:1105:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.cc:6937:16 in tiny_gltf.h
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
#include <string>
#include <vector>

static bool isValidUTF8(const uint8_t* data, size_t size) {
    size_t i = 0;
    while (i < size) {
        if (data[i] <= 0x7F) {
            i++;
        } else if (data[i] >= 0xC2 && data[i] <= 0xDF) {
            if (i + 1 >= size || (data[i + 1] & 0xC0) != 0x80) return false;
            i += 2;
        } else if (data[i] == 0xE0) {
            if (i + 2 >= size || (data[i + 1] & 0xE0) != 0xA0 || (data[i + 2] & 0xC0) != 0x80) return false;
            i += 3;
        } else if (data[i] >= 0xE1 && data[i] <= 0xEC) {
            if (i + 2 >= size || (data[i + 1] & 0xC0) != 0x80 || (data[i + 2] & 0xC0) != 0x80) return false;
            i += 3;
        } else if (data[i] == 0xED) {
            if (i + 2 >= size || (data[i + 1] & 0xE0) != 0x80 || (data[i + 2] & 0xC0) != 0x80) return false;
            i += 3;
        } else if (data[i] >= 0xEE && data[i] <= 0xEF) {
            if (i + 2 >= size || (data[i + 1] & 0xC0) != 0x80 || (data[i + 2] & 0xC0) != 0x80) return false;
            i += 3;
        } else if (data[i] == 0xF0) {
            if (i + 3 >= size || (data[i + 1] & 0xF0) != 0x90 || (data[i + 2] & 0xC0) != 0x80 || (data[i + 3] & 0xC0) != 0x80) return false;
            i += 4;
        } else if (data[i] >= 0xF1 && data[i] <= 0xF3) {
            if (i + 3 >= size || (data[i + 1] & 0xC0) != 0x80 || (data[i + 2] & 0xC0) != 0x80 || (data[i + 3] & 0xC0) != 0x80) return false;
            i += 4;
        } else if (data[i] == 0xF4) {
            if (i + 3 >= size || (data[i + 1] & 0xF0) != 0x80 || (data[i + 2] & 0xC0) != 0x80 || (data[i + 3] & 0xC0) != 0x80) return false;
            i += 4;
        } else {
            return false;
        }
    }
    return true;
}

static std::string sanitizeUTF8(const uint8_t* data, size_t size, size_t max_len) {
    std::string result;
    result.reserve(std::min(size, max_len));
    
    for (size_t i = 0; i < std::min(size, max_len); i++) {
        uint8_t c = data[i];
        if (c <= 0x7F) {
            result.push_back(static_cast<char>(c));
        } else if (c >= 0xC2 && c <= 0xDF) {
            if (i + 1 < std::min(size, max_len) && (data[i + 1] & 0xC0) == 0x80) {
                result.push_back(static_cast<char>(c));
                result.push_back(static_cast<char>(data[i + 1]));
                i++;
            } else {
                result.push_back('?');
            }
        } else if (c == 0xE0) {
            if (i + 2 < std::min(size, max_len) && (data[i + 1] & 0xE0) == 0xA0 && (data[i + 2] & 0xC0) == 0x80) {
                result.push_back(static_cast<char>(c));
                result.push_back(static_cast<char>(data[i + 1]));
                result.push_back(static_cast<char>(data[i + 2]));
                i += 2;
            } else {
                result.push_back('?');
            }
        } else if (c >= 0xE1 && c <= 0xEC) {
            if (i + 2 < std::min(size, max_len) && (data[i + 1] & 0xC0) == 0x80 && (data[i + 2] & 0xC0) == 0x80) {
                result.push_back(static_cast<char>(c));
                result.push_back(static_cast<char>(data[i + 1]));
                result.push_back(static_cast<char>(data[i + 2]));
                i += 2;
            } else {
                result.push_back('?');
            }
        } else if (c == 0xED) {
            if (i + 2 < std::min(size, max_len) && (data[i + 1] & 0xE0) == 0x80 && (data[i + 2] & 0xC0) == 0x80) {
                result.push_back(static_cast<char>(c));
                result.push_back(static_cast<char>(data[i + 1]));
                result.push_back(static_cast<char>(data[i + 2]));
                i += 2;
            } else {
                result.push_back('?');
            }
        } else if (c >= 0xEE && c <= 0xEF) {
            if (i + 2 < std::min(size, max_len) && (data[i + 1] & 0xC0) == 0x80 && (data[i + 2] & 0xC0) == 0x80) {
                result.push_back(static_cast<char>(c));
                result.push_back(static_cast<char>(data[i + 1]));
                result.push_back(static_cast<char>(data[i + 2]));
                i += 2;
            } else {
                result.push_back('?');
            }
        } else if (c == 0xF0) {
            if (i + 3 < std::min(size, max_len) && (data[i + 1] & 0xF0) == 0x90 && 
                (data[i + 2] & 0xC0) == 0x80 && (data[i + 3] & 0xC0) == 0x80) {
                result.push_back(static_cast<char>(c));
                result.push_back(static_cast<char>(data[i + 1]));
                result.push_back(static_cast<char>(data[i + 2]));
                result.push_back(static_cast<char>(data[i + 3]));
                i += 3;
            } else {
                result.push_back('?');
            }
        } else if (c >= 0xF1 && c <= 0xF3) {
            if (i + 3 < std::min(size, max_len) && (data[i + 1] & 0xC0) == 0x80 && 
                (data[i + 2] & 0xC0) == 0x80 && (data[i + 3] & 0xC0) == 0x80) {
                result.push_back(static_cast<char>(c));
                result.push_back(static_cast<char>(data[i + 1]));
                result.push_back(static_cast<char>(data[i + 2]));
                result.push_back(static_cast<char>(data[i + 3]));
                i += 3;
            } else {
                result.push_back('?');
            }
        } else if (c == 0xF4) {
            if (i + 3 < std::min(size, max_len) && (data[i + 1] & 0xF0) == 0x80 && 
                (data[i + 2] & 0xC0) == 0x80 && (data[i + 3] & 0xC0) == 0x80) {
                result.push_back(static_cast<char>(c));
                result.push_back(static_cast<char>(data[i + 1]));
                result.push_back(static_cast<char>(data[i + 2]));
                result.push_back(static_cast<char>(data[i + 3]));
                i += 3;
            } else {
                result.push_back('?');
            }
        } else {
            result.push_back('?');
        }
    }
    
    return result;
}

static void populateModel(tinygltf::Model& model, const uint8_t* data, size_t size) {
    // Set asset version
    model.asset.version = "2.0";
    
    // Sanitize UTF-8 for string fields
    std::string safe_string = sanitizeUTF8(data, size, 100);
    
    // Populate some basic fields with fuzzer data
    if (size > 0) {
        // Create a buffer with some data
        tinygltf::Buffer buffer;
        buffer.name = "test_buffer";
        buffer.data.assign(data, data + std::min(size, size_t(1000)));
        model.buffers.push_back(buffer);
        
        // Create a buffer view
        tinygltf::BufferView bufferView;
        bufferView.buffer = 0;
        bufferView.byteOffset = 0;
        bufferView.byteLength = std::min(size, size_t(1000));
        model.bufferViews.push_back(bufferView);
        
        // Create an accessor
        tinygltf::Accessor accessor;
        accessor.bufferView = 0;
        accessor.byteOffset = 0;
        accessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
        accessor.count = std::min(size, size_t(1000));
        accessor.type = TINYGLTF_TYPE_SCALAR;
        model.accessors.push_back(accessor);
        
        // Create an image with some data
        tinygltf::Image image;
        image.name = "test_image";
        image.width = 16;
        image.height = 16;
        image.image.assign(data, data + std::min(size, size_t(256)));
        model.images.push_back(image);
        
        // Create a scene
        tinygltf::Scene scene;
        scene.name = "test_scene";
        model.scenes.push_back(scene);
        model.defaultScene = 0;
        
        // Create a node
        tinygltf::Node node;
        node.name = "test_node";
        model.nodes.push_back(node);
        
        // Create a mesh with a primitive
        tinygltf::Mesh mesh;
        mesh.name = "test_mesh";
        tinygltf::Primitive primitive;
        primitive.material = -1;
        primitive.mode = TINYGLTF_MODE_TRIANGLES;
        mesh.primitives.push_back(primitive);
        model.meshes.push_back(mesh);
        
        // Set extras with sanitized UTF-8 string
        if (!safe_string.empty() && isValidUTF8(reinterpret_cast<const uint8_t*>(safe_string.data()), safe_string.size())) {
            model.extras = tinygltf::Value(safe_string);
        }
    }
}

static bool dummyWriteImageData(const std::string* basepath,
                                const std::string* filename,
                                const tinygltf::Image* image,
                                bool embedImages,
                                const tinygltf::FsCallbacks* fs,
                                const tinygltf::URICallbacks* uri,
                                std::string* out_uri,
                                void* user_data) {
    // Dummy implementation that does nothing
    if (out_uri && filename) {
        *out_uri = *filename;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* Data, size_t Size) {
    // Limit input size to avoid excessive memory usage
    if (Size > 10000) {
        return 0;
    }
    
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    
    // Populate the model with fuzzer data
    populateModel(model, Data, Size);
    
    // Use fuzzer data to determine various boolean parameters
    // Use first byte for configuration flags
    uint8_t config = Size > 0 ? Data[0] : 0;
    
    bool embedImages = (config & 0x01) != 0;
    bool embedBuffers = (config & 0x02) != 0;
    bool prettyPrint = (config & 0x04) != 0;
    bool writeBinary = (config & 0x08) != 0;
    bool serializeDefaults = (config & 0x10) != 0;
    bool storeOriginalJSON = (config & 0x20) != 0;
    bool useCustomImageWriter = (config & 0x40) != 0;
    
    // Set various options based on configuration
    loader.SetSerializeDefaultValues(serializeDefaults);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(storeOriginalJSON);
    
    // Test GetStoreOriginalJSONForExtrasAndExtensions
    bool getStoreOriginal = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)getStoreOriginal; // Suppress unused variable warning
    
    // Set custom image writer if configured
    if (useCustomImageWriter) {
        loader.SetImageWriter(dummyWriteImageData, nullptr);
    }
    
    // Test WriteGltfSceneToFile
    const std::string filename = "./dummy_file.gltf";
    bool result = false;
    try {
        result = loader.WriteGltfSceneToFile(&model, filename,
                                             embedImages, embedBuffers,
                                             prettyPrint, writeBinary);
    } catch (...) {
        // Catch any exceptions to avoid crashes
    }
    (void)result; // Suppress unused variable warning
    
    // Clean up the file
    std::remove(filename.c_str());
    
    // Test WriteGltfSceneToStream
    std::ostringstream stream;
    bool streamResult = false;
    try {
        streamResult = loader.WriteGltfSceneToStream(&model, stream,
                                                     prettyPrint, writeBinary);
    } catch (...) {
        // Catch any exceptions to avoid crashes
    }
    (void)streamResult; // Suppress unused variable warning
    
    // Test with different combinations
    // Try with opposite settings
    if (Size > 1) {
        uint8_t altConfig = Data[1 % Size];
        bool altEmbedImages = (altConfig & 0x01) != 0;
        bool altEmbedBuffers = (altConfig & 0x02) != 0;
        bool altPrettyPrint = (altConfig & 0x04) != 0;
        bool altWriteBinary = (altConfig & 0x08) != 0;
        
        std::ostringstream altStream;
        try {
            loader.WriteGltfSceneToStream(&model, altStream,
                                         altPrettyPrint, altWriteBinary);
        } catch (...) {
            // Catch any exceptions
        }
        
        const std::string altFilename = "./dummy_file_alt.gltf";
        try {
            loader.WriteGltfSceneToFile(&model, altFilename,
                                       altEmbedImages, altEmbedBuffers,
                                       altPrettyPrint, altWriteBinary);
        } catch (...) {
            // Catch any exceptions
        }
        std::remove(altFilename.c_str());
    }
    
    return 0;
}