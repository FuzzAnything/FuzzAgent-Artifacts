// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
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
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <functional>
#include "tiny_gltf.h"

// Corrected callback signatures based on tinygltf.h definitions

static bool CustomFileExists(const std::string &filepath, void *user_data) {
    (void)filepath;
    (void)user_data;
    return false;
}

static std::string CustomExpandFilePath(const std::string &relative_path, void *user_data) {
    (void)relative_path;
    (void)user_data;
    return "";
}

static bool CustomReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                                const std::string &filepath, void *user_data) {
    (void)out;
    (void)err;
    (void)filepath;
    (void)user_data;
    return false;
}

static bool CustomWriteWholeFile(std::string *err, const std::string &filepath,
                                 const std::vector<unsigned char> &contents, void *user_data) {
    (void)err;
    (void)filepath;
    (void)contents;
    (void)user_data;
    return false;
}

// Fixed signature: GetFileSizeInBytes takes (size_t*, const std::string*, const std::string&, void*)
// The third argument is the filepath (const std::string&), not a pointer
static bool CustomGetFileSizeInBytes(size_t *size, const std::string *filepath, const std::string &basepath, void *user_data) {
    (void)size;
    (void)filepath;
    (void)basepath;
    (void)user_data;
    return false;
}

// Fixed signature: URIEncode takes (const std::string&, const std::string&, std::string*, void*)
// The second argument is the basepath (const std::string&), not a pointer
static bool CustomURIEncode(const std::string &str, const std::string &basepath, std::string *out, void *user_data) {
    (void)str;
    (void)basepath;
    (void)out;
    (void)user_data;
    return false;
}

// Fixed signature: URIDecode takes (const std::string&, const std::string*, void*)
// The second argument is the basepath (const std::string*), which matches the original assumption
static bool CustomURIDecode(const std::string &str, const std::string *basepath, void *user_data) {
    (void)str;
    (void)basepath;
    (void)user_data;
    return false;
}

static bool CustomWriteImageData(const std::string *basepath, const std::string *filename,
                                 const tinygltf::Image *image, bool embedImages,
                                 const tinygltf::FsCallbacks *fs_cb,
                                 const tinygltf::URICallbacks *uri_cb,
                                 std::string *out_uri, void *user_data) {
    (void)basepath;
    (void)filename;
    (void)image;
    (void)embedImages;
    (void)fs_cb;
    (void)uri_cb;
    (void)out_uri;
    (void)user_data;
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    tinygltf::Image image;
    std::string baseDir = "./dummy_file";
    std::string out_uri;
    std::string err;

    // Initialize callbacks
    tinygltf::FsCallbacks fs_cb;
    fs_cb.FileExists = CustomFileExists;
    fs_cb.ExpandFilePath = CustomExpandFilePath;
    fs_cb.ReadWholeFile = CustomReadWholeFile;
    fs_cb.WriteWholeFile = CustomWriteWholeFile;
    // Fixed assignment for GetFileSizeInBytes with correct signature
    fs_cb.GetFileSizeInBytes = CustomGetFileSizeInBytes;
    fs_cb.user_data = nullptr;

    tinygltf::URICallbacks uri_cb;
    // Fixed assignment for encode with correct signature
    uri_cb.encode = CustomURIEncode;
    // Fixed assignment for decode with correct signature
    uri_cb.decode = CustomURIDecode;
    uri_cb.user_data = nullptr;

    // Populate image with fuzzed data to test UpdateImageObject and WriteImageData
    if (Size > 0) {
        image.image.assign(Data, Data + Size);
        image.width = (Data[0] % 1024) + 1;
        image.height = (Data[0] % 1024) + 1;
        image.name = "fuzz_image";
        image.mimeType = "image/png";
    }

    // Populate model with fuzzed data to test WriteGltfSceneToFile and WriteGltfSceneToStream
    tinygltf::Asset asset;
    asset.version = "2.0";
    model.asset = asset;

    // Add a buffer
    tinygltf::Buffer buffer;
    buffer.data.assign(Data, Data + Size);
    model.buffers.push_back(buffer);

    // Add a buffer view
    tinygltf::BufferView bufferView;
    bufferView.buffer = 0;
    bufferView.byteOffset = 0;
    bufferView.byteLength = Size;
    model.bufferViews.push_back(bufferView);

    // Add an accessor
    tinygltf::Accessor accessor;
    accessor.bufferView = 0;
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
    // Fixed: type is an enum, not a string
    accessor.type = TINYGLTF_TYPE_SCALAR;
    accessor.count = (Size > 0) ? (Data[0] % 100) + 1 : 1;
    model.accessors.push_back(accessor);

    // Add a node
    tinygltf::Node node;
    node.name = "fuzz_node";
    model.nodes.push_back(node);

    // Add a mesh
    tinygltf::Mesh mesh;
    tinygltf::Primitive prim;
    prim.attributes["POSITION"] = 0;
    mesh.primitives.push_back(prim);
    model.meshes.push_back(mesh);

    // Add a scene
    tinygltf::Scene scene;
    scene.nodes.push_back(0);
    model.scenes.push_back(scene);
    model.defaultScene = 0;

    // 2. Test SetFsCallbacks
    ctx.SetFsCallbacks(fs_cb, &err);

    // 3. Test SetURICallbacks
    ctx.SetURICallbacks(uri_cb, &err);

    // 4. Test WriteImageData (Global function)
    std::string dummy_out_uri;
    tinygltf::WriteImageData(&baseDir, &image.name, &image, true, &fs_cb, &uri_cb,
                             &dummy_out_uri, nullptr);
    tinygltf::WriteImageData(&baseDir, &image.name, &image, false, &fs_cb, &uri_cb,
                             &dummy_out_uri, nullptr);

    // 5. Test WriteGltfSceneToFile
    ctx.WriteGltfSceneToFile(&model, "./dummy_file.gltf", true, true, true, false);
    ctx.WriteGltfSceneToFile(&model, "./dummy_file.gltf", false, false, false, true);
    ctx.WriteGltfSceneToFile(&model, "./dummy_file.bin", true, true, true, true);

    // 6. Test WriteGltfSceneToStream
    std::ostringstream oss;
    ctx.WriteGltfSceneToStream(&model, oss, true, false);
    ctx.WriteGltfSceneToStream(&model, oss, false, true);

    return 0;
}