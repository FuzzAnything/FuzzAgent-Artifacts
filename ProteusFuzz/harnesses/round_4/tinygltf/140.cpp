// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <random>
#include <fstream>
#include "tiny_gltf.h"

static std::vector<unsigned char> GenerateRandomData(size_t size) {
    std::vector<unsigned char> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (size_t i = 0; i < size; ++i) {
        data[i] = static_cast<unsigned char>(dis(gen));
    }
    return data;
}

static std::string GenerateRandomString(size_t size) {
    std::string str;
    str.reserve(size);
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);
    for (size_t i = 0; i < size; ++i) {
        str += charset[dis(gen)];
    }
    return str;
}

static tinygltf::Buffer CreateRandomBuffer(const uint8_t *Data, size_t Size) {
    tinygltf::Buffer buffer;
    if (Size > 0) {
        buffer.data.assign(Data, Data + Size);
    } else {
        buffer.data = GenerateRandomData(100);
    }
    buffer.name = GenerateRandomString(10);
    return buffer;
}

static tinygltf::Model CreateRandomModel(const uint8_t *Data, size_t Size) {
    tinygltf::Model model;
    model.asset.version = "2.0";
    
    // Add random buffers
    size_t chunkSize = Size / 3;
    if (chunkSize > 0) {
        tinygltf::Buffer buffer1 = CreateRandomBuffer(Data, chunkSize);
        model.buffers.push_back(buffer1);
        
        if (Size > chunkSize) {
            tinygltf::Buffer buffer2 = CreateRandomBuffer(Data + chunkSize, Size - chunkSize);
            model.buffers.push_back(buffer2);
        }
    } else {
        model.buffers.push_back(CreateRandomBuffer(nullptr, 0));
    }
    
    // Add random buffer views
    for (size_t i = 0; i < 3; ++i) {
        tinygltf::BufferView view;
        view.buffer = static_cast<int>(i < model.buffers.size() ? i : 0);
        view.byteOffset = 0;
        model.bufferViews.push_back(view);
    }
    
    // Add random accessors
    for (size_t i = 0; i < 3; ++i) {
        tinygltf::Accessor accessor;
        accessor.bufferView = static_cast<int>(i < model.bufferViews.size() ? i : 0);
        accessor.byteOffset = 0;
        model.accessors.push_back(accessor);
    }
    
    // Add random primitives inside a mesh
    tinygltf::Primitive primitive;
    primitive.material = 0;
    
    tinygltf::Mesh mesh;
    mesh.primitives.push_back(primitive);
    model.meshes.push_back(mesh);
    
    // Add random nodes
    tinygltf::Node node;
    node.mesh = 0;
    model.nodes.push_back(node);
    
    // Add random scene
    tinygltf::Scene scene;
    scene.nodes.push_back(0);
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    return model;
}

extern "C" int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    try {
        // Fuzz WriteGltfSceneToStream (Public Member Function)
        {
            tinygltf::Model model = CreateRandomModel(Data, Size);
            std::stringstream stream;
            tinygltf::TinyGLTF ctx;
            ctx.WriteGltfSceneToStream(&model, stream, true, false);
            ctx.WriteGltfSceneToStream(&model, stream, false, true);
        }

        // Fuzz WriteGltfSceneToFile (Public Member Function)
        {
            tinygltf::Model model = CreateRandomModel(Data, Size);
            tinygltf::TinyGLTF ctx;
            ctx.WriteGltfSceneToFile(&model, "./dummy_file", true, true, true, false);
            ctx.WriteGltfSceneToFile(&model, "./dummy_file", false, false, false, true);
        }
        
        // Since the static functions like SerializeGltfBuffer, WriteGltfStream, WriteBinaryGltfStream, WriteBinaryGltfFile
        // are causing compilation errors due to visibility or namespace issues in the provided context,
        // we focus on fuzzing the public API methods that internally use these functions.
        // The TinyGLTF class methods WriteGltfSceneToStream and WriteGltfSceneToFile are the primary entry points
        // for serialization and will exercise the internal logic.

    } catch (...) {
        // Ignore exceptions during fuzzing
    }

    return 0;
}