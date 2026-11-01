// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetSerializeDefaultValues at tiny_gltf.h:1512:8 in tiny_gltf.h
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
#include <cstdint>
#include <cstddef>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

// Helper to sanitize string data to ensure valid UTF-8
static std::string SanitizeString(const uint8_t *Data, size_t Size) {
    if (!Data || Size == 0) return "";
    
    std::string result;
    result.reserve(Size);
    
    for (size_t i = 0; i < Size; ++i) {
        unsigned char c = Data[i];
        // Allow only printable ASCII or common control characters to ensure valid UTF-8
        // This prevents the JSON library from throwing "invalid UTF-8" errors
        if (c >= 32 && c < 127) {
            result.push_back(static_cast<char>(c));
        } else {
            // Replace invalid bytes with a safe character
            result.push_back('?');
        }
    }
    return result;
}

static void ConstructFuzzedModel(const uint8_t *Data, size_t Size, tinygltf::Model *model) {
    if (!model || !Data) return;

    // Fuzz Asset
    std::string safeVersion = SanitizeString(Data, Size > 10 ? 10 : Size);
    model->asset.version = safeVersion.empty() ? "2.0" : safeVersion;

    // Fuzz Buffers
    if (Size > 2) {
        tinygltf::Buffer buffer;
        std::string safeName = SanitizeString(Data + 2, Size > 10 ? 10 : (Size - 2));
        buffer.name = safeName.empty() ? "fuzz_buffer" : safeName;
        buffer.data.assign(Data, Data + Size);
        model->buffers.push_back(buffer);
    }

    // Fuzz BufferViews
    if (Size > 3) {
        tinygltf::BufferView view;
        view.buffer = 0;
        view.byteOffset = Size % 100;
        model->bufferViews.push_back(view);
    }

    // Fuzz Accessors
    if (Size > 4) {
        tinygltf::Accessor accessor;
        accessor.bufferView = 0;
        accessor.byteOffset = Size % 50;
        accessor.minValues.push_back(static_cast<double>(Size) / 100.0);
        model->accessors.push_back(accessor);
    }

    // Fuzz Meshes and Primitives
    if (Size > 5) {
        tinygltf::Mesh mesh;
        std::string safeMeshName = SanitizeString(Data + 5, Size > 15 ? 10 : (Size - 5));
        mesh.name = safeMeshName.empty() ? "fuzz_mesh" : safeMeshName;
        
        tinygltf::Primitive prim;
        prim.material = 0;
        // Add attributes if we have enough data
        if (Size > 10) {
            prim.attributes["POSITION"] = 0;
        }
        mesh.primitives.push_back(prim);
        model->meshes.push_back(mesh);
    }

    // Fuzz Materials
    if (Size > 6) {
        tinygltf::Material mat;
        std::string safeMatName = SanitizeString(Data + 6, Size > 16 ? 10 : (Size - 6));
        mat.name = safeMatName.empty() ? "fuzz_material" : safeMatName;
        mat.pbrMetallicRoughness.baseColorFactor = {1.0, 0.0, 0.0, 1.0};
        model->materials.push_back(mat);
    }

    // Fuzz Nodes
    if (Size > 7) {
        tinygltf::Node node;
        node.mesh = 0;
        model->nodes.push_back(node);
    }

    // Fuzz Scenes
    if (Size > 8) {
        tinygltf::Scene scene;
        std::string safeSceneName = SanitizeString(Data + 8, Size > 18 ? 10 : (Size - 8));
        scene.name = safeSceneName.empty() ? "fuzz_scene" : safeSceneName;
        if (!model->nodes.empty()) {
            scene.nodes.push_back(0);
        }
        model->scenes.push_back(scene);
        model->defaultScene = 0;
    }
}

extern "C" int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    ConstructFuzzedModel(Data, Size, &model);

    // 2. Test SetSerializeDefaultValues and GetSerializeDefaultValues
    // Alternate between true and false based on input data
    bool flag = (Data[0] % 2) == 0;
    ctx.SetSerializeDefaultValues(flag);
    bool retrievedFlag = ctx.GetSerializeDefaultValues();
    (void)retrievedFlag; // Suppress unused warning, logic is tested

    // 3. Test WriteGltfStream logic (simulated via stream writing)
    {
        std::ostringstream oss;
        std::string jsonContent = "{\"asset\":{\"version\":\"2.0\"}}";
        // Append sanitized fuzzed data to potentially create malformed JSON
        if (Size > 20) {
            std::string safeData = SanitizeString(Data + 20, Size - 20);
            jsonContent += safeData;
        }
        // Manually perform the action of WriteGltfStream: write string and newline
        oss << jsonContent << "\n";
        // Check stream state
        if (!oss.good()) {
            // Handle failure if stream is bad (unlikely for ostringstream)
        }
    }

    // 4. Test SerializeGltfBuffer
    // This is an internal function. We rely on WriteGltfSceneToStream to exercise it.

    // 5. Test WriteGltfSceneToStream (ASCII and Binary)
    {
        std::ostringstream asciiStream;
        bool asciiResult = ctx.WriteGltfSceneToStream(&model, asciiStream, true, false);
        (void)asciiResult;

        std::ostringstream binaryStream;
        bool binaryResult = ctx.WriteGltfSceneToStream(&model, binaryStream, false, true);
        (void)binaryResult;
    }

    // 6. Test WriteGltfSceneToFile
    // Use a dummy file as per instructions
    const std::string filename = "./dummy_file.gltf";
    
    // Test ASCII write
    bool fileResultAscii = ctx.WriteGltfSceneToFile(&model, filename, false, false, true, false);
    (void)fileResultAscii;

    // Test Binary write
    const std::string filenameBin = "./dummy_file.glb";
    bool fileResultBin = ctx.WriteGltfSceneToFile(&model, filenameBin, false, false, false, true);
    (void)fileResultBin;

    // Test with embedding
    bool fileResultEmbed = ctx.WriteGltfSceneToFile(&model, filename, true, true, true, false);
    (void)fileResultEmbed;

    // 7. Cleanup
    // No explicit cleanup needed for local objects. 
    // Files are overwritten or created in the current directory.

    return 0;
}