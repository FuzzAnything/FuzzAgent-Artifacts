// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
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
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

static void FillModelWithFuzzData(tinygltf::Model *model, const uint8_t *data, size_t size) {
    if (!model || !data || size == 0) return;

    // Create a simple asset
    tinygltf::Asset asset;
    asset.version = "2.0";
    model->asset = asset;

    // Parse data to generate random structures
    size_t idx = 0;
    auto get_val = [&]() -> uint8_t {
        if (idx >= size) return 0;
        return data[idx++];
    };
    auto get_int = [&]() -> int {
        if (idx + 3 >= size) return 0;
        int val = 0;
        val |= data[idx++];
        val |= (data[idx++] << 8);
        val |= (data[idx++] << 16);
        val |= (data[idx++] << 24);
        return val;
    };

    // Add random buffers
    for (int i = 0; i < 5 && idx < size; ++i) {
        tinygltf::Buffer buf;
        buf.name = "buffer_" + std::to_string(i);
        int len = get_int() % 1024;
        if (len > 0) {
            buf.data.resize(len);
            for (int j = 0; j < len && idx < size; ++j) {
                buf.data[j] = data[idx++];
            }
        }
        model->buffers.push_back(buf);
    }

    // Add random buffer views
    for (int i = 0; i < 5 && idx < size; ++i) {
        tinygltf::BufferView bv;
        bv.name = "view_" + std::to_string(i);
        bv.buffer = get_int() % static_cast<int>(model->buffers.size());
        bv.byteOffset = get_int() % 100;
        model->bufferViews.push_back(bv);
    }

    // Add random accessors
    for (int i = 0; i < 5 && idx < size; ++i) {
        tinygltf::Accessor acc;
        acc.name = "acc_" + std::to_string(i);
        acc.bufferView = get_int() % static_cast<int>(model->bufferViews.size());
        acc.byteOffset = get_int() % 50;
        acc.minValues.push_back(static_cast<double>(get_val()));
        model->accessors.push_back(acc);
    }

    // Add random meshes
    for (int i = 0; i < 3 && idx < size; ++i) {
        tinygltf::Mesh mesh;
        mesh.name = "mesh_" + std::to_string(i);
        
        tinygltf::Primitive prim;
        prim.material = get_int() % 10; // dummy material index
        // Add attributes
        prim.attributes["POSITION"] = get_int() % static_cast<int>(model->accessors.size());
        if (get_val() % 2) prim.attributes["NORMAL"] = get_int() % static_cast<int>(model->accessors.size());
        
        mesh.primitives.push_back(prim);
        model->meshes.push_back(mesh);
    }

    // Add random materials
    for (int i = 0; i < 3 && idx < size; ++i) {
        tinygltf::Material mat;
        mat.name = "mat_" + std::to_string(i);
        mat.emissiveFactor = {static_cast<double>(get_val()) / 255.0, static_cast<double>(get_val()) / 255.0, static_cast<double>(get_val()) / 255.0};
        mat.alphaCutoff = static_cast<double>(get_val()) / 255.0;
        model->materials.push_back(mat);
    }

    // Add random nodes
    for (int i = 0; i < 5 && idx < size; ++i) {
        tinygltf::Node node;
        node.name = "node_" + std::to_string(i);
        node.mesh = get_int() % static_cast<int>(model->meshes.size());
        model->nodes.push_back(node);
    }

    // Add random scenes
    tinygltf::Scene scene;
    scene.name = "default_scene";
    for (const auto &node : model->nodes) {
        scene.nodes.push_back(static_cast<int>(node.name.size())); // dummy index
    }
    model->scenes.push_back(scene);
    model->defaultScene = 0;

    // Add random images
    for (int i = 0; i < 2 && idx < size; ++i) {
        tinygltf::Image img;
        img.name = "img_" + std::to_string(i);
        img.width = get_int() % 100;
        int imgDataLen = get_int() % 100;
        if (imgDataLen > 0) {
            img.image.resize(imgDataLen);
            for (int j = 0; j < imgDataLen && idx < size; ++j) {
                img.image[j] = data[idx++];
            }
        }
        model->images.push_back(img);
    }

    // Add random textures
    for (int i = 0; i < 2 && idx < size; ++i) {
        tinygltf::Texture tex;
        tex.name = "tex_" + std::to_string(i);
        tex.sampler = get_int() % 10;
        model->textures.push_back(tex);
    }

    // Add random samplers
    for (int i = 0; i < 2 && idx < size; ++i) {
        tinygltf::Sampler samp;
        samp.name = "samp_" + std::to_string(i);
        samp.minFilter = get_int() % 10;
        model->samplers.push_back(samp);
    }
}

static uint8_t SafeGetByte(const uint8_t *Data, size_t Size, size_t index) {
    if (index < Size) {
        return Data[index];
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;

    // Fill model with fuzzed data
    FillModelWithFuzzData(&model, Data, Size);

    // Fuzz GetSerializeDefaultValues and SetSerializeDefaultValues
    bool currentVal = ctx.GetSerializeDefaultValues();
    (void)currentVal; // Use the value to avoid unused warning
    
    // Toggle the setting based on input
    if (SafeGetByte(Data, Size, 0) % 2 == 0) {
        ctx.SetSerializeDefaultValues(true);
    } else {
        ctx.SetSerializeDefaultValues(false);
    }
    
    // Verify the setting
    currentVal = ctx.GetSerializeDefaultValues();
    (void)currentVal;

    // Fuzz WriteGltfSceneToStream
    {
        std::ostringstream stream;
        bool success = ctx.WriteGltfSceneToStream(&model, stream, 
                                                  SafeGetByte(Data, Size, 0) % 2, 
                                                  SafeGetByte(Data, Size, 1) % 2);
        (void)success;
    }

    // Fuzz WriteGltfSceneToFile
    // Use a dummy file path
    std::string filename = "./dummy_file.gltf";
    if (SafeGetByte(Data, Size, 2) % 2 == 0) {
        filename = "./dummy_file.glb";
    }
    
    // Write to file
    bool fileSuccess = ctx.WriteGltfSceneToFile(
        &model, 
        filename, 
        SafeGetByte(Data, Size, 3) % 2, // embedImages
        SafeGetByte(Data, Size, 4) % 2, // embedBuffers
        SafeGetByte(Data, Size, 5) % 2, // prettyPrint
        SafeGetByte(Data, Size, 6) % 2  // writeBinary
    );
    (void)fileSuccess;

    return 0;
}