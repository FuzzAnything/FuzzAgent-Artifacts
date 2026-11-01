// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <functional>
#include "tiny_gltf.h"
#include <fstream>

// Dummy filesystem callbacks for fuzzing
// Note: Argument order must match tinygltf.h definitions exactly
static bool DummyReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &filepath, void *user_data) {
    if (err) err->clear();
    if (out) out->clear();
    return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filepath,
                                const std::vector<unsigned char> &contents, void *user_data) {
    if (err) err->clear();
    std::ofstream ofs(filepath, std::ios::binary);
    if (!ofs) {
        if (err) *err = "Failed to open file";
        return false;
    }
    ofs.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    ofs.close();
    return true;
}

static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filepath, void *user_data) {
    if (err) err->clear();
    if (size) *size = 0;
    return false;
}

// Dummy URI callbacks
// Note: Argument order must match tinygltf.h definitions exactly
// The signature in tinygltf.h for URIEncodeFunction is:
// typedef std::function<bool(const std::string &uri, const std::string &baseDir, std::string *out, void *user_data)> URIEncodeFunction;
// And for URIDecodeFunction:
// typedef std::function<bool(const std::string &uri, std::string *out, void *user_data)> URIDecodeFunction;
// We need to match these signatures exactly.

static bool DummyURIEncode(const std::string &uri, const std::string &baseDir, std::string *out, void *user_data) {
    if (out) *out = uri;
    return true;
}

static bool DummyURIDecode(const std::string &uri, std::string *out, void *user_data) {
    if (out) *out = uri;
    return true;
}

// Dummy WriteImageDataFunction
static bool DummyWriteImageData(const tinygltf::Image *image, const std::string *baseDir,
                                const std::string &filename, void *user_data) {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) return false;
    if (image && !image->image.empty()) {
        ofs.write(reinterpret_cast<const char*>(image->image.data()), image->image.size());
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;

    // 1. Fuzz SetFsCallbacks and SetURICallbacks
    tinygltf::FsCallbacks fs_cb;
    fs_cb.FileExists = nullptr; 
    fs_cb.ExpandFilePath = nullptr;
    fs_cb.ReadWholeFile = DummyReadWholeFile;
    fs_cb.WriteWholeFile = DummyWriteWholeFile;
    fs_cb.GetFileSizeInBytes = DummyGetFileSizeInBytes;
    fs_cb.user_data = nullptr;

    tinygltf::URICallbacks uri_cb;
    uri_cb.encode = DummyURIEncode;
    uri_cb.decode = DummyURIDecode;
    uri_cb.user_data = nullptr;

    std::string err;
    ctx.SetFsCallbacks(fs_cb, &err);
    ctx.SetURICallbacks(uri_cb, &err);

    // 2. Fuzz WriteWholeFile directly
    std::string filepath = "./dummy_file";
    std::vector<unsigned char> contents(Data, Data + Size);
    std::string write_err;
    tinygltf::WriteWholeFile(&write_err, filepath, contents, nullptr);

    // 3. Fuzz UpdateImageObject logic indirectly via WriteGltfSceneToFile
    tinygltf::Image image;
    image.name = "dummy_image";
    image.width = 64;
    image.image.assign(Data, Data + Size);
    
    model.images.push_back(image);

    // 4. Fuzz WriteGltfSceneToStream
    tinygltf::Asset asset;
    asset.version = "2.0";
    model.asset = asset;

    tinygltf::Scene scene;
    scene.name = "fuzz_scene";
    model.scenes.push_back(scene);
    model.defaultScene = 0;

    tinygltf::Node node;
    node.name = "fuzz_node";
    model.nodes.push_back(node);
    scene.nodes.push_back(0);

    tinygltf::Mesh mesh;
    mesh.name = "fuzz_mesh";
    tinygltf::Primitive prim;
    prim.material = -1;
    mesh.primitives.push_back(prim);
    model.meshes.push_back(mesh);
    node.mesh = 0;

    tinygltf::Buffer buffer;
    buffer.name = "fuzz_buffer";
    buffer.data.assign(Data, Data + Size);
    model.buffers.push_back(buffer);

    tinygltf::BufferView bufferView;
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = Size;
    model.bufferViews.push_back(bufferView);

    tinygltf::Accessor accessor;
    accessor.bufferView = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor.count = Size / 4;
    accessor.type = TINYGLTF_TYPE_SCALAR;
    model.accessors.push_back(accessor);

    std::ostringstream stream;
    bool stream_result = ctx.WriteGltfSceneToStream(&model, stream, true, false);
    (void)stream_result;

    // 5. Fuzz WriteGltfSceneToFile
    std::string filename = "./dummy_output.gltf";
    bool file_result = ctx.WriteGltfSceneToFile(&model, filename, false, false, true, false);
    (void)file_result;

    std::string filename_bin = "./dummy_output.glb";
    bool file_result_bin = ctx.WriteGltfSceneToFile(&model, filename_bin, false, false, true, true);
    (void)file_result_bin;

    return 0;
}