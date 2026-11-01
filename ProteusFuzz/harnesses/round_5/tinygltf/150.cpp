// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::FileExists at tiny_gltf.h:1333:6 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
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
#include <algorithm>
#include "tiny_gltf.h"

static void FuzzDataToBuffer(const uint8_t *Data, size_t Size, std::vector<unsigned char> &buffer) {
    buffer.clear();
    if (Size > 0) {
        buffer.assign(Data, Data + Size);
    }
}

static std::string FuzzDataToPath(const uint8_t *Data, size_t Size) {
    std::string path = "./dummy_file";
    for (size_t i = 0; i < Size; ++i) {
        if (Data[i] == '\0') continue;
        if ((Data[i] >= 'a' && Data[i] <= 'z') || 
            (Data[i] >= 'A' && Data[i] <= 'Z') || 
            (Data[i] >= '0' && Data[i] <= '9') ||
            Data[i] == '.' || Data[i] == '_') {
            path += static_cast<char>(Data[i]);
        }
    }
    return path;
}

extern "C" int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare data
    std::vector<unsigned char> buffer;
    FuzzDataToBuffer(Data, Size, buffer);
    std::string path = FuzzDataToPath(Data, Size);
    std::string json_content;
    
    // Fix: Use size_t for both arguments to std::min
    size_t json_len = std::min(Size, static_cast<size_t>(1000));
    if (json_len > 0) {
        json_content.assign(reinterpret_cast<const char*>(Data), json_len);
    }

    // 1. Fuzz FileExists
    bool file_exists = tinygltf::FileExists(path, nullptr);
    (void)file_exists;

    // 2. Fuzz WriteWholeFile
    std::string err;
    bool write_whole_result = tinygltf::WriteWholeFile(&err, path, buffer, nullptr);
    (void)write_whole_result;

    // 3. Fuzz WriteGltfSceneToFile
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    
    tinygltf::Asset asset;
    asset.version = "2.0";
    model.asset = asset;

    tinygltf::Buffer buf;
    buf.data = buffer;
    model.buffers.push_back(buf);

    tinygltf::BufferView view;
    view.buffer = 0;
    view.byteOffset = 0;
    model.bufferViews.push_back(view);

    tinygltf::Accessor acc;
    acc.bufferView = 0;
    model.accessors.push_back(acc);

    tinygltf::Scene scene;
    model.scenes.push_back(scene);
    model.defaultScene = 0;

    tinygltf::Node node;
    model.nodes.push_back(node);

    tinygltf::Mesh mesh;
    model.meshes.push_back(mesh);

    bool write_gltf_result = ctx.WriteGltfSceneToFile(
        &model, path + ".gltf",
        (Size % 2 == 0), 
        (Size % 3 == 0), 
        (Size % 4 == 0), 
        (Size % 5 == 0)
    );
    (void)write_gltf_result;

    // 4. Fuzz WriteBinaryGltfFile
    // Note: WriteBinaryGltfFile is a static helper inside TinyGLTF in some versions,
    // but often not exposed in the global namespace or requires specific includes.
    // If it's not in tinygltf namespace, we skip it or call via instance if possible.
    // Based on the error, it's not in the global namespace. 
    // In tinygltf, this is often a static member of TinyGLTF or internal.
    // We will attempt to call it if it exists as a static member of TinyGLTF.
    // However, the prompt says "static bool WriteBinaryGltfFile" in tinygltf namespace.
    // If the header doesn't expose it, we skip to avoid compilation error.
    // Let's try calling it as a static member of TinyGLTF if the global one fails.
    // Actually, looking at common tinygltf implementations, these are often internal.
    // We will wrap in a check or simply comment out if not accessible.
    // But to follow instructions, we assume the API exists as described.
    // If the compiler says it's missing, it might be a version mismatch.
    // We will try to call it, but if it's truly missing, we must remove it.
    // Given the error "no member named ... in namespace", we must remove or adjust.
    // Since I cannot change the library, I will remove the calls that are not found.
    
    // Re-evaluating based on error: The functions WriteBinaryGltfFile, SerializeGltfBufferData, 
    // WriteGltfFile are not found in the namespace. 
    // They might be internal static functions not exposed in the public header 
    // or the specific version used by the fuzzer doesn't expose them globally.
    // I will remove the calls to these specific functions to ensure compilation.
    
    // 5. & 6. Removed due to compilation errors regarding missing symbols.
    // We rely on WriteGltfSceneToFile and WriteWholeFile which are confirmed to exist.

    return 0;
}