// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
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

static std::string CreatePathFromInput(const uint8_t *Data, size_t Size) {
    std::string path;
    if (Size == 0) return "./dummy_file";
    size_t len = (Size < 256) ? Size : 256;
    for (size_t i = 0; i < len; ++i) {
        uint8_t c = Data[i];
        if (c == 0 || c == '/' || c == '\\' || c == '.') {
            c = '/';
        }
        path += static_cast<char>(c);
    }
    if (path.empty()) path = "./dummy_file";
    return path;
}

static tinygltf::Model CreateModelFromInput(const uint8_t *Data, size_t Size) {
    tinygltf::Model model;
    
    model.asset.version = "2.0";
    
    if (Size > 0) {
        tinygltf::Buffer buffer;
        buffer.data.assign(Data, Data + Size);
        model.buffers.push_back(buffer);
    }
    
    if (Size > 0) {
        tinygltf::BufferView view;
        view.buffer = 0;
        view.byteOffset = 0;
        model.bufferViews.push_back(view);
    }
    
    if (Size > 0) {
        tinygltf::Accessor accessor;
        accessor.bufferView = 0;
        accessor.byteOffset = 0;
        accessor.count = 3;
        accessor.type = TINYGLTF_TYPE_VEC3;
        accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        model.accessors.push_back(accessor);
    }
    
    tinygltf::Mesh mesh;
    mesh.name = "test_mesh";
    tinygltf::Primitive prim;
    prim.attributes["POSITION"] = 0;
    prim.material = 0;
    mesh.primitives.push_back(prim);
    model.meshes.push_back(mesh);
    
    tinygltf::Node node;
    node.mesh = 0;
    model.nodes.push_back(node);
    
    tinygltf::Scene scene;
    scene.nodes.push_back(0);
    model.scenes.push_back(scene);
    model.defaultScene = 0;
    
    tinygltf::Material mat;
    mat.name = "test_material";
    model.materials.push_back(mat);
    
    return model;
}

static std::string CreateJsonContent(const uint8_t *Data, size_t Size) {
    std::ostringstream oss;
    oss << "{\"asset\":{\"version\":\"2.0\"},";
    if (Size > 0) {
        oss << "\"buffers\":[";
        oss << "{\"uri\":\"data.bin\",\"byteLength\":" << Size << "}]";
        oss << ",\"bufferViews\":[{\"buffer\":0,\"byteOffset\":0,\"byteLength\":" << Size << "}]";
        oss << ",\"accessors\":[{\"bufferView\":0,\"componentType\":5126,\"count\":" << Size << ",\"type\":\"SCALAR\"}]";
    }
    oss << "}";
    return oss.str();
}

extern "C" int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string path = CreatePathFromInput(Data, Size);
    tinygltf::Model model = CreateModelFromInput(Data, Size);
    std::string jsonContent = CreateJsonContent(Data, Size);
    std::string err;
    std::vector<unsigned char> binBuffer;
    
    if (Size > 0) {
        binBuffer.assign(Data, Data + Size);
    } else {
        binBuffer.push_back(0);
    }

    // 1. Test FileExists
    // FileExists is a callback function. We call it directly as per the signature provided.
    tinygltf::FileExists(path, nullptr);

    // 2. Test WriteWholeFile
    tinygltf::WriteWholeFile(&err, path, std::vector<unsigned char>(jsonContent.begin(), jsonContent.end()), nullptr);

    // 3. Test WriteGltfSceneToFile
    tinygltf::TinyGLTF ctx;
    ctx.WriteGltfSceneToFile(&model, path + ".gltf", false, false, true, false);
    ctx.WriteGltfSceneToFile(&model, path + "_embed.gltf", true, true, true, false);
    ctx.WriteGltfSceneToFile(&model, path + ".glb", false, false, false, true);

    // 4. Test WriteBinaryGltfFile
    // These are internal utility functions in tinygltf. They are not exported in the public API
    // in all versions or might be static. To ensure compilation, we rely on the public API
    // which internally uses them, or we attempt to call them if they are accessible.
    // Since the compiler reported they are not in the namespace, they are likely static or internal.
    // We can test the functionality via WriteGltfSceneToFile with writeBinary=true (already done above).
    // However, if the user specifically wants to fuzz these internal helpers and they are not visible,
    // we cannot call them directly. 
    // Assuming the provided signature implies they should be callable, but the error says otherwise.
    // We will skip direct calls to these internal static functions as they are not part of the public API
    // in the header provided to the compiler. The WriteGltfSceneToFile covers the logic.

    // 5. Test WriteGltfFile
    // Same as above, internal function. Covered by WriteGltfSceneToFile.

    // 6. Test SerializeGltfBufferData
    // Same as above, internal function.

    // Since the direct calls failed, we rely on the public API which exercises these paths.
    // The WriteGltfSceneToFile with different flags exercises the binary and JSON writing logic.

    return 0;
}