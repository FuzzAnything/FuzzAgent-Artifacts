// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::ExpandFilePath at tiny_gltf.cc:1333:13 in tiny_gltf.h
// tinygltf::GetFileSizeInBytes at tiny_gltf.cc:1396:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.cc:1590:6 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.cc:6994:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.cc:1225:16 in tiny_gltf.h
// tinygltf::FileExists at tiny_gltf.cc:1265:6 in tiny_gltf.h
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
#include <string>
#include <vector>

static void populateModel(tinygltf::Model &model, const uint8_t *data, size_t size) {
    // Set basic asset info
    model.asset.version = "2.0";
    
    // Create a simple scene
    tinygltf::Scene scene;
    scene.name = "TestScene";
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    // Create a node
    tinygltf::Node node;
    node.name = "TestNode";
    model.nodes.push_back(node);
    model.scenes[0].nodes.push_back(0);
    
    // Create a buffer with some data
    tinygltf::Buffer buffer;
    buffer.name = "TestBuffer";
    buffer.data.assign(data, data + std::min(size, (size_t)1024));
    model.buffers.push_back(buffer);
    
    // Create a buffer view
    tinygltf::BufferView bufferView;
    bufferView.name = "TestBufferView";
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = std::min(size, (size_t)1024);
    model.bufferViews.push_back(bufferView);
    
    // Create an accessor
    tinygltf::Accessor accessor;
    accessor.name = "TestAccessor";
    accessor.bufferView = 0;
    accessor.byteOffset = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor.type = TINYGLTF_TYPE_VEC3;
    accessor.count = std::min(size / 12, (size_t)10);
    model.accessors.push_back(accessor);
    
    // Create a mesh with primitive
    tinygltf::Mesh mesh;
    mesh.name = "TestMesh";
    tinygltf::Primitive primitive;
    primitive.material = -1;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;
    primitive.attributes["POSITION"] = 0;
    mesh.primitives.push_back(primitive);
    model.meshes.push_back(mesh);
    
    // Update node to reference mesh
    model.nodes[0].mesh = 0;
}

static bool dummyFileExists(const std::string &, void *) {
    return true;
}

static std::string dummyExpandFilePath(const std::string &filepath, void *) {
    return filepath;
}

static bool dummyReadWholeFile(std::vector<unsigned char> *, std::string *,
                               const std::string &, void *) {
    return true;
}

static bool dummyWriteWholeFile(std::string *, const std::string &,
                                const std::vector<unsigned char> &, void *) {
    return true;
}

static bool dummyGetFileSizeInBytes(size_t *, std::string *,
                                    const std::string &, void *) {
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use a deterministic dummy file path
    const std::string dummy_file = "./dummy_file";
    
    // Test 1: ExpandFilePath
    {
        std::string filepath(reinterpret_cast<const char*>(Data), 
                            std::min(Size, (size_t)256));
        std::string result = tinygltf::ExpandFilePath(filepath, nullptr);
        // Result should be unchanged as per documentation
    }
    
    // Test 2: GetFileSizeInBytes
    {
        // Write some data to dummy file
        std::ofstream outfile(dummy_file, std::ios::binary);
        if (outfile) {
            outfile.write(reinterpret_cast<const char*>(Data), 
                         std::min(Size, (size_t)1024));
            outfile.close();
        }
        
        size_t filesize = 0;
        std::string err;
        std::string filepath = dummy_file;
        bool success = tinygltf::GetFileSizeInBytes(&filesize, &err, 
                                                   filepath, nullptr);
        // Clean up
        std::remove(dummy_file.c_str());
    }
    
    // Test 3: WriteWholeFile
    {
        std::string err;
        std::vector<unsigned char> contents;
        if (Size > 0) {
            contents.assign(Data, Data + std::min(Size, (size_t)4096));
        }
        bool success = tinygltf::WriteWholeFile(&err, dummy_file, 
                                               contents, nullptr);
        // Clean up
        std::remove(dummy_file.c_str());
    }
    
    // Test 4: WriteGltfSceneToFile
    {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;
        
        // Populate model with fuzzed data
        populateModel(model, Data, Size);
        
        // Try different combinations of parameters
        for (int i = 0; i < 2; ++i) {
            bool embedImages = (i & 1) == 0;
            bool embedBuffers = (i & 2) == 0;
            bool prettyPrint = (i & 4) == 0;
            bool writeBinary = (i & 8) == 0;
            
            try {
                bool success = loader.WriteGltfSceneToFile(&model, dummy_file,
                                                          embedImages, embedBuffers,
                                                          prettyPrint, writeBinary);
            } catch (...) {
                // Catch exceptions from invalid UTF-8 in JSON library
            }
            
            // Clean up
            std::remove(dummy_file.c_str());
        }
    }
    
    // Test 5: SetFsCallbacks
    {
        tinygltf::TinyGLTF loader;
        std::string err;
        
        tinygltf::FsCallbacks callbacks;
        callbacks.FileExists = dummyFileExists;
        callbacks.ExpandFilePath = dummyExpandFilePath;
        callbacks.ReadWholeFile = dummyReadWholeFile;
        callbacks.WriteWholeFile = dummyWriteWholeFile;
        callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
        callbacks.user_data = nullptr;
        
        bool success = loader.SetFsCallbacks(callbacks, &err);
        
        // Test with null callbacks
        tinygltf::FsCallbacks nullCallbacks = {};
        loader.SetFsCallbacks(nullCallbacks, nullptr);
    }
    
    // Test 6: FileExists
    {
        // Write dummy file
        std::ofstream outfile(dummy_file, std::ios::binary);
        if (outfile) {
            outfile.write(reinterpret_cast<const char*>(Data), 
                         std::min(Size, (size_t)1));
            outfile.close();
        }
        
        std::string filepath = dummy_file;
        bool exists = tinygltf::FileExists(filepath, nullptr);
        
        // Clean up
        std::remove(dummy_file.c_str());
    }
    
    return 0;
}