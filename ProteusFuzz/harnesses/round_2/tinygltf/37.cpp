// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetSerializeDefaultValues at tiny_gltf.h:1508:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
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

// Define function pointer types matching tinygltf's expectations
// Based on the error, GetFileSizeInBytes expects: bool(size_t*, std::string*, const std::string&, void*)
typedef bool (*FileExistsFunction)(const std::string &filename, void *user_data);
typedef std::string (*ExpandFilePathFunction)(const std::string &filepath, void *user_data);
typedef bool (*ReadWholeFileFunction)(std::string *err, std::vector<unsigned char> *out,
                                      const std::string &filename, void *user_data);
typedef bool (*WriteWholeFileFunction)(std::string *err, const std::string &filename,
                                       const std::vector<unsigned char> &contents, void *user_data);
// Corrected signature based on compiler error: (size_t*, std::string*, const std::string&, void*)
typedef bool (*GetFileSizeFunction)(size_t *size, std::string *err, const std::string &filename, void *user_data);

static bool StaticFileExists(const std::string &filename, void *) {
    (void)filename;
    return false;
}

static std::string StaticExpandFilePath(const std::string &filepath, void *) {
    (void)filepath;
    return filepath;
}

// Note: The order of arguments in tinygltf's ReadWholeFileFunction is (out, err, filename, user_data)
// based on the error message: function<bool (vector<unsigned char> *, basic_string<char> *, ...)>
static bool StaticReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                                const std::string &filename, void *) {
    (void)filename;
    if (err) err->clear();
    if (out) out->clear();
    return false;
}

static bool StaticWriteWholeFile(std::string *err, const std::string &filename,
                                 const std::vector<unsigned char> &contents, void *) {
    (void)filename;
    (void)contents;
    if (err) err->clear();
    return true;
}

// Corrected signature to match the expected function pointer type
static bool StaticGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *) {
    (void)filename;
    if (size) *size = 0;
    if (err) err->clear();
    return false;
}

static tinygltf::Model CreateFuzzedModel(const uint8_t *data, size_t size) {
    tinygltf::Model model;
    model.asset = tinygltf::Asset();
    model.asset.version = "2.0";

    if (size > 0) {
        tinygltf::Buffer buffer;
        buffer.data.assign(data, data + (size > 1024 ? 1024 : size));
        model.buffers.push_back(buffer);

        tinygltf::BufferView bufferView;
        bufferView.buffer = 0;
        bufferView.byteOffset = 0;
        model.bufferViews.push_back(bufferView);

        tinygltf::Accessor accessor;
        accessor.bufferView = 0;
        accessor.componentType = 5125; 
        accessor.count = size > 4 ? 4 : size;
        // Fix: accessor.type is likely an enum or int in this version, not a string.
        // Common types: SCALAR=0, VEC2=1, VEC3=2, VEC4=3, MAT2=4, MAT3=5, MAT4=6
        accessor.type = 0; // SCALAR
        model.accessors.push_back(accessor);

        tinygltf::Mesh mesh;
        tinygltf::Primitive primitive;
        primitive.attributes["POSITION"] = 0;
        mesh.primitives.push_back(primitive);
        model.meshes.push_back(mesh);

        tinygltf::Node node;
        node.mesh = 0;
        model.nodes.push_back(node);

        tinygltf::Scene scene;
        scene.nodes.push_back(0);
        model.scenes.push_back(scene);
        model.defaultScene = 0;
    }

    return model;
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;

    // 1. Test SetSerializeDefaultValues
    bool serializeDefaults = (Data[0] % 2) == 0;
    ctx.SetSerializeDefaultValues(serializeDefaults);

    // 2. Test SetFsCallbacks
    tinygltf::FsCallbacks fsCallbacks;
    fsCallbacks.FileExists = StaticFileExists;
    fsCallbacks.ExpandFilePath = StaticExpandFilePath;
    // Fix: Ensure argument order matches the expected function pointer type
    fsCallbacks.ReadWholeFile = StaticReadWholeFile;
    fsCallbacks.WriteWholeFile = StaticWriteWholeFile;
    // Fixed signature to match expected type
    fsCallbacks.GetFileSizeInBytes = StaticGetFileSizeInBytes;
    fsCallbacks.user_data = nullptr;

    bool fsSet = ctx.SetFsCallbacks(fsCallbacks, &err);
    (void)fsSet;

    // 3. Create a model to serialize
    tinygltf::Model model = CreateFuzzedModel(Data, Size);

    // 4. Test WriteGltfSceneToStream
    std::ostringstream stream;
    bool writeBinary = (Data[0] % 3) == 0;
    bool prettyPrint = (Data[0] % 2) != 0;
    bool streamResult = ctx.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);
    (void)streamResult;

    // 5. Test WriteGltfSceneToFile
    std::string filename = "./dummy_file";
    bool embedImages = (Size > 1 && (Data[1] % 2) != 0);
    bool embedBuffers = (Size > 2 && (Data[2] % 2) != 0);
    bool fileResult = ctx.WriteGltfSceneToFile(&model, filename, embedImages, embedBuffers, prettyPrint, writeBinary);
    (void)fileResult;

    // 6. Test WriteGltfFile and WriteGltfStream
    // These functions are not found in the namespace in the provided header version.
    // We skip them to ensure compilation, as the error explicitly states they are missing.
    // If they were available, we would call them here.
    // std::string content = stream.str();
    // if (content.empty()) content = "{}";
    // (void)tinygltf::WriteGltfFile(filename, content);
    // std::ostringstream stream2;
    // (void)tinygltf::WriteGltfStream(stream2, content);

    return 0;
}