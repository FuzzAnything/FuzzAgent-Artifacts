// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
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
#include "tiny_gltf.h"

static tinygltf::Model CreateFuzzedModel(const uint8_t *Data, size_t Size) {
    tinygltf::Model model;
    
    if (Size == 0) {
        return model;
    }

    // Populate model with fuzzed data to trigger various code paths
    // Add nodes
    size_t numNodes = (Size > 100) ? 100 : Size;
    model.nodes.resize(numNodes);
    for (size_t i = 0; i < numNodes; ++i) {
        tinygltf::Node node;
        node.translation = {
            static_cast<double>(Data[i % Size]),
            static_cast<double>(Data[(i + 1) % Size]),
            static_cast<double>(Data[(i + 2) % Size])
        };
        node.rotation = {
            static_cast<double>(Data[(i + 3) % Size]),
            static_cast<double>(Data[(i + 4) % Size]),
            static_cast<double>(Data[(i + 5) % Size]),
            static_cast<double>(Data[(i + 6) % Size])
        };
        node.scale = {
            static_cast<double>(Data[(i + 7) % Size]),
            static_cast<double>(Data[(i + 8) % Size]),
            static_cast<double>(Data[(i + 9) % Size])
        };
        model.nodes[i] = node;
    }

    // Add meshes
    size_t numMeshes = (Size > 50) ? 50 : Size;
    model.meshes.resize(numMeshes);
    for (size_t i = 0; i < numMeshes; ++i) {
        tinygltf::Mesh mesh;
        mesh.name = "mesh_" + std::to_string(i);
        model.meshes[i] = mesh;
    }

    // Add materials
    size_t numMaterials = (Size > 50) ? 50 : Size;
    model.materials.resize(numMaterials);
    for (size_t i = 0; i < numMaterials; ++i) {
        tinygltf::Material mat;
        mat.name = "material_" + std::to_string(i);
        model.materials[i] = mat;
    }

    // Add buffers
    size_t numBuffers = (Size > 20) ? 20 : Size;
    model.buffers.resize(numBuffers);
    for (size_t i = 0; i < numBuffers; ++i) {
        tinygltf::Buffer buffer;
        buffer.name = "buffer_" + std::to_string(i);
        size_t bufSize = (Size > 1000) ? 1000 : Size;
        buffer.data.resize(bufSize);
        for (size_t j = 0; j < bufSize; ++j) {
            buffer.data[j] = Data[(i * 100 + j) % Size];
        }
        model.buffers[i] = buffer;
    }

    // Add scenes
    size_t numScenes = (Size > 10) ? 10 : Size;
    model.scenes.resize(numScenes);
    for (size_t i = 0; i < numScenes; ++i) {
        tinygltf::Scene scene;
        scene.name = "scene_" + std::to_string(i);
        model.scenes[i] = scene;
    }

    // Add asset
    tinygltf::Asset asset;
    asset.version = "2.0";
    model.asset = asset;

    return model;
}

static bool SafeReadBool(const uint8_t *Data, size_t Size, size_t index) {
    if (Size <= index) return false;
    return (Data[index] % 2) != 0;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    try {
        tinygltf::TinyGLTF ctx;
        tinygltf::Model model = CreateFuzzedModel(Data, Size);

        // Test 1: GetSerializeDefaultValues
        bool serializeDefaults = ctx.GetSerializeDefaultValues();
        (void)serializeDefaults; // Suppress unused variable warning

        // Test 2: SetSerializeDefaultValues with fuzzed boolean
        bool setDefault = SafeReadBool(Data, Size, 0);
        ctx.SetSerializeDefaultValues(setDefault);
        bool checkDefault = ctx.GetSerializeDefaultValues();
        (void)checkDefault;

        // Test 3: WriteGltfSceneToStream (binary and ASCII)
        std::ostringstream stream;
        bool writeBinary = SafeReadBool(Data, Size, 1);
        bool prettyPrint = SafeReadBool(Data, Size, 2);
        bool resultStream = ctx.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);
        (void)resultStream;

        // Test 4: WriteGltfSceneToFile
        std::string filename = "./dummy_file";
        bool embedImages = SafeReadBool(Data, Size, 3);
        bool embedBuffers = SafeReadBool(Data, Size, 4);
        bool resultFile = ctx.WriteGltfSceneToFile(&model, filename, embedImages, embedBuffers, prettyPrint, writeBinary);
        (void)resultFile;

    } catch (...) {
        // Catch any exceptions to prevent fuzzer crashes
        return 0;
    }

    return 0;
}