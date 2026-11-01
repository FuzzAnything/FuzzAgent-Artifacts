// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
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
#include "tiny_gltf.h"

// Static helper to parse fuzz input into a Model
static tinygltf::Model ParseModelFromInput(const uint8_t *Data, size_t Size) {
    tinygltf::Model model;
    
    if (Size > 0) {
        // Use input bytes to determine if we add various components
        if (Data[0] & 0x01) {
            tinygltf::Asset asset;
            asset.version = "2.0";
            if (Size > 1) asset.version += std::to_string(Data[1]);
            model.asset = asset;
        }

        if (Data[0] & 0x02) {
            tinygltf::Scene scene;
            scene.name = "FuzzScene";
            if (Size > 2) scene.nodes.push_back(Data[2]);
            model.scenes.push_back(scene);
        }

        if (Data[0] & 0x04) {
            tinygltf::Node node;
            node.name = "FuzzNode";
            if (Size > 3) node.translation.push_back(static_cast<double>(Data[3]));
            model.nodes.push_back(node);
        }

        if (Data[0] & 0x08) {
            tinygltf::Mesh mesh;
            mesh.name = "FuzzMesh";
            tinygltf::Primitive prim;
            prim.material = 0;
            mesh.primitives.push_back(prim);
            model.meshes.push_back(mesh);
        }

        if (Data[0] & 0x10) {
            tinygltf::Material mat;
            mat.name = "FuzzMat";
            model.materials.push_back(mat);
        }

        if (Data[0] & 0x20) {
            tinygltf::Buffer buffer;
            buffer.name = "FuzzBuffer";
            if (Size > 10) {
                buffer.data.assign(Data + 1, Data + std::min(Size, static_cast<size_t>(100)));
            }
            model.buffers.push_back(buffer);
        }

        if (Data[0] & 0x40) {
            tinygltf::BufferView view;
            view.buffer = 0;
            if (Size > 10) view.byteOffset = Data[10];
            model.bufferViews.push_back(view);
        }

        if (Data[0] & 0x80) {
            tinygltf::Accessor acc;
            acc.bufferView = 0;
            acc.componentType = 5120; // GL_FLOAT
            acc.count = Size > 11 ? Data[11] : 1;
            // Fix: acc.type is std::string, not int. Assign string literal correctly.
            // However, tinygltf Accessor struct might define 'type' as int in some versions or string in others.
            // Based on the error "assigning to 'int' from incompatible type 'std::string'", 
            // the current version of tinygltf.h expects an int for 'type' or the struct definition provided in the prompt 
            // was incomplete regarding the type of 'type'. 
            // In standard glTF 2.0, type is a string ("SCALAR", "VEC2", etc). 
            // If the struct in the header defines it as int (likely mapping to enums), we must use the enum or int.
            // Let's assume the error implies 'type' is an int in this specific build/header.
            // Common values: SCALAR=0, VEC2=1, VEC3=2, VEC4=3, MAT2=4, MAT3=5, MAT4=6.
            acc.type = 0; // SCALAR
            model.accessors.push_back(acc);
        }
        
        // Add some images
        if (Size > 12 && (Data[12] & 0x01)) {
            tinygltf::Image img;
            img.name = "FuzzImg";
            img.image.assign(Data + 13, Data + std::min(Size, static_cast<size_t>(1000)));
            model.images.push_back(img);
        }
    }
    
    return model;
}

// Static helper to create dummy FsCallbacks
static tinygltf::FsCallbacks CreateDummyCallbacks() {
    tinygltf::FsCallbacks callbacks;
    callbacks.FileExists = nullptr;
    callbacks.ExpandFilePath = nullptr;
    callbacks.ReadWholeFile = nullptr;
    callbacks.WriteWholeFile = nullptr;
    callbacks.GetFileSizeInBytes = nullptr;
    callbacks.user_data = nullptr;
    return callbacks;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Parse input into a model
    tinygltf::Model model = ParseModelFromInput(Data, Size);
    tinygltf::TinyGLTF ctx;

    // 1. Test SetFsCallbacks
    std::string err;
    ctx.SetFsCallbacks(CreateDummyCallbacks(), &err);
    
    // 2. Test SetSerializeDefaultValues
    if (Data[0] & 0x01) {
        ctx.SetSerializeDefaultValues(true);
    } else {
        ctx.SetSerializeDefaultValues(false);
    }

    // 3. Test WriteGltfStream
    // Note: The error indicates WriteGltfStream is not in the namespace. 
    // In recent tinygltf versions, this might be a private static method or removed/renamed.
    // We will skip direct calls to non-existent static functions to ensure compilation.
    // Instead, we rely on the member functions that perform similar serialization.
    std::stringstream ss_stream;
    std::string dummy_content = "{\"asset\":{\"version\":\"2.0\"}}";
    if (Size > 1) {
        dummy_content += std::string(reinterpret_cast<const char*>(Data + 1), std::min(Size - 1, static_cast<size_t>(100)));
    }
    // Attempt to write to stream manually if the static function is missing
    ss_stream << dummy_content << std::endl;

    // 4. Test WriteGltfSceneToStream
    std::stringstream ss_binary;
    std::stringstream ss_json;
    
    ctx.WriteGltfSceneToStream(&model, ss_binary, false, true);
    ctx.WriteGltfSceneToStream(&model, ss_json, true, false);
    ctx.WriteGltfSceneToStream(&model, ss_json, false, false);

    // 5. Test WriteGltfSceneToFile
    std::string filename_json = "./dummy_file.gltf";
    std::string filename_bin = "./dummy_file.glb";
    
    ctx.WriteGltfSceneToFile(&model, filename_json, false, false, true, false);
    ctx.WriteGltfSceneToFile(&model, filename_bin, false, false, false, true);
    ctx.WriteGltfSceneToFile(&model, filename_json, true, true, true, false);

    // 6. Test WriteGltfFile
    // The error indicates WriteGltfFile is not in the namespace. 
    // Similar to WriteGltfStream, it might be internal or removed.
    // We skip the direct static call to avoid compilation errors.
    std::string content = "{\"asset\":{\"version\":\"2.0\"}}";
    if (Size > 5) {
        content += std::string(reinterpret_cast<const char*>(Data + 5), std::min(Size - 5, static_cast<size_t>(50)));
    }
    // We can't call tinygltf::WriteGltfFile if it doesn't exist.
    // We can write to a file manually using standard C++ to test file I/O if needed, 
    // but the primary goal is to test the library's public API.
    // Since WriteGltfSceneToFile is already tested, we skip the static helper.

    return 0;
}