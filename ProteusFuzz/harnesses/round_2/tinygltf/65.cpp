// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
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

// Custom image loader callback for fuzzing
static bool FuzzImageLoader(tinygltf::Image *image,
                            const int width,
                            std::string *ext,
                            std::string *mime,
                            int requested_components,
                            int size,
                            const unsigned char *bytes,
                            int size2,
                            void *user_data) {
    if (!image) return false;
    image->width = 1;
    image->height = 1;
    image->image.resize(4);
    for (int i = 0; i < 4; ++i) image->image[i] = 0;
    image->as_is = false;
    if (ext) *ext = "";
    if (mime) *mime = "";
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 7) return 0;

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;

    // Use input data to drive configuration flags
    bool preserveChannels = (Data[0] % 2) == 0;
    bool imagesAsIs = (Data[1] % 2) == 0;
    bool useCustomLoader = (Data[2] % 2) == 0;
    bool embedImages = (Data[3] % 2) == 0;
    bool embedBuffers = (Data[4] % 2) == 0;
    bool prettyPrint = (Data[5] % 2) == 0;
    bool writeBinary = (Data[6] % 2) == 0;

    // Set configuration
    ctx.SetPreserveImageChannels(preserveChannels);
    ctx.SetImagesAsIs(imagesAsIs);

    if (useCustomLoader) {
        ctx.SetImageLoader(FuzzImageLoader, nullptr);
    } else {
        ctx.SetImageLoader(nullptr, nullptr);
    }

    // Query state
    bool currentPreserve = ctx.GetPreserveImageChannels();
    bool currentAsIs = ctx.GetImagesAsIs();
    (void)currentPreserve;
    (void)currentAsIs;

    // Populate model with some fuzzed data to ensure WriteGltfSceneToFile has content
    model.asset.version = "2.0";

    // Add a buffer with fuzzed data
    if (Size > 10) {
        tinygltf::Buffer buffer;
        buffer.name = "fuzz_buffer";
        buffer.data.assign(Data + 10, Data + Size);
        model.buffers.push_back(buffer);
    }

    // Add a buffer view
    if (model.buffers.size() > 0) {
        tinygltf::BufferView view;
        view.buffer = 0;
        view.byteOffset = 0;
        model.bufferViews.push_back(view);
    }

    // Add an accessor
    if (model.bufferViews.size() > 0) {
        tinygltf::Accessor acc;
        acc.bufferView = 0;
        acc.componentType = 5125; // UNSIGNED_INT
        acc.count = 3;
        // In tinygltf, Accessor::type is an int enum (SCALAR, VEC2, VEC3, etc.)
        // 4 corresponds to VEC3
        acc.type = 4; 
        model.accessors.push_back(acc);
    }

    // Add a primitive and mesh
    if (model.accessors.size() > 0) {
        tinygltf::Primitive prim;
        prim.attributes["POSITION"] = 0;
        tinygltf::Mesh mesh;
        mesh.primitives.push_back(prim);
        model.meshes.push_back(mesh);
    }

    // Add a node and scene
    if (model.meshes.size() > 0) {
        tinygltf::Node node;
        node.mesh = 0;
        model.nodes.push_back(node);
        
        tinygltf::Scene scene;
        scene.nodes.push_back(0);
        model.scenes.push_back(scene);
        model.defaultScene = 0;
    }

    // Write to file
    const std::string filename = "./dummy_file";
    bool success = ctx.WriteGltfSceneToFile(
        &model,
        filename,
        embedImages,
        embedBuffers,
        prettyPrint,
        writeBinary
    );

    return success ? 1 : 0;
}