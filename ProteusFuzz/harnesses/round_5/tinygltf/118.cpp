// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include "tiny_gltf.h"

// Helper to generate random data from fuzz input
static std::vector<unsigned char> GetRandomData(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return std::vector<unsigned char>();
    }
    return std::vector<unsigned char>(Data, Data + Size);
}

// Helper to generate random string from fuzz input
static std::string GetRandomString(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return std::string();
    }
    return std::string(reinterpret_cast<const char*>(Data), Size);
}

// Helper to generate a simple JSON string for testing
static std::string GenerateSimpleJSON(const uint8_t *Data, size_t Size) {
    std::string content = "{\"asset\":{\"version\":\"2.0\"},";
    if (Size > 0) {
        std::string randomVal;
        for (size_t i = 0; i < Size && i < 100; ++i) {
            randomVal += static_cast<char>(Data[i] % 26 + 'a');
        }
        content += "\"name\":\"" + randomVal + "\"";
    }
    content += "}";
    return content;
}

// Helper to create a minimal Model for testing
static tinygltf::Model CreateMinimalModel(const uint8_t *Data, size_t Size) {
    tinygltf::Model model;
    model.asset.version = "2.0";
    
    if (Size > 0) {
        tinygltf::Buffer buffer;
        buffer.data = GetRandomData(Data, Size);
        buffer.name = "fuzz_buffer";
        model.buffers.push_back(buffer);
    }
    
    if (!model.buffers.empty()) {
        tinygltf::BufferView view;
        view.buffer = 0;
        view.byteOffset = 0;
        model.bufferViews.push_back(view);
    }
    
    if (!model.bufferViews.empty()) {
        tinygltf::Accessor accessor;
        accessor.bufferView = 0;
        accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        accessor.count = 1;
        accessor.type = TINYGLTF_TYPE_SCALAR;
        model.accessors.push_back(accessor);
    }

    return model;
}

extern "C" int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Test tinygltf::WriteGltfStream (via TinyGLTF context or direct call if available)
    // Note: WriteGltfStream is often a static helper inside the implementation.
    // We rely on the public API WriteGltfSceneToStream which uses it internally.
    {
        tinygltf::TinyGLTF ctx;
        tinygltf::Model model = CreateMinimalModel(Data, Size);
        std::ostringstream oss;
        // This internally calls WriteGltfStream
        ctx.WriteGltfSceneToStream(&model, oss, true, false);
    }

    // 2. Test tinygltf::WriteBinaryGltfStream (via TinyGLTF context)
    // Note: WriteBinaryGltfStream is often a static helper.
    // We rely on the public API WriteGltfSceneToStream with writeBinary=true.
    {
        tinygltf::TinyGLTF ctx;
        tinygltf::Model model = CreateMinimalModel(Data, Size);
        std::ostringstream oss;
        // This internally calls WriteBinaryGltfStream
        ctx.WriteGltfSceneToStream(&model, oss, true, true);
    }

    // 3. Test tinygltf::SerializeGltfBuffer (via TinyGLTF context)
    // Note: SerializeGltfBuffer is often a static helper used internally.
    // We trigger it by writing a model with buffers.
    {
        tinygltf::TinyGLTF ctx;
        tinygltf::Model model = CreateMinimalModel(Data, Size);
        std::ostringstream oss;
        // This internally calls SerializeGltfBuffer
        ctx.WriteGltfSceneToStream(&model, oss, true, false);
    }

    // 4. Test tinygltf::WriteBinaryGltfFile (via TinyGLTF context)
    // Note: WriteBinaryGltfFile is often a static helper.
    // We rely on the public API WriteGltfSceneToFile with writeBinary=true.
    {
        tinygltf::TinyGLTF ctx;
        tinygltf::Model model = CreateMinimalModel(Data, Size);
        std::string filename = "./dummy_file.glb";
        // This internally calls WriteBinaryGltfFile
        ctx.WriteGltfSceneToFile(&model, filename, false, false, true, true);
    }

    // 5. Test tinygltf::TinyGLTF::WriteGltfSceneToStream (Public API)
    {
        tinygltf::TinyGLTF ctx;
        tinygltf::Model model = CreateMinimalModel(Data, Size);
        
        std::ostringstream oss;
        ctx.WriteGltfSceneToStream(&model, oss, true, false);
        
        std::ostringstream ossBin;
        ctx.WriteGltfSceneToStream(&model, ossBin, true, true);
    }

    // 6. Test tinygltf::TinyGLTF::WriteGltfSceneToFile (Public API)
    {
        tinygltf::TinyGLTF ctx;
        tinygltf::Model model = CreateMinimalModel(Data, Size);
        
        std::string filename = "./dummy_file.gltf";
        ctx.WriteGltfSceneToFile(&model, filename, false, false, true, false);
        
        std::string filenameBin = "./dummy_file.glb";
        ctx.WriteGltfSceneToFile(&model, filenameBin, false, false, true, true);
        
        ctx.WriteGltfSceneToFile(&model, filename, true, true, true, false);
    }

    return 0;
}