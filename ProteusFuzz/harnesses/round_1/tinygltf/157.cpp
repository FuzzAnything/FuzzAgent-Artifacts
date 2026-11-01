// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
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
#include <fstream>
#include <memory>

// Dummy callback implementations for FsCallbacks
static bool DummyReadWholeFile(std::vector<unsigned char> *contents, std::string *err, const std::string &filepath, void *) {
    if (err) *err = "DummyReadWholeFile not implemented";
    return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filepath, const std::vector<unsigned char> &contents, void *) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "Failed to open file for writing";
        return false;
    }
    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    return true;
}

static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filepath, void *) {
    if (size) *size = 0;
    return false;
}

// Dummy callback implementations for URICallbacks
static bool DummyURIEncode(const std::string &uri, const std::string &baseDir, std::string *out_uri, void *) {
    if (out_uri) *out_uri = uri;
    return true;
}

static bool DummyURIDecode(const std::string &uri, std::string *out_uri, void *) {
    if (out_uri) *out_uri = uri;
    return true;
}

// Dummy WriteImageDataFunction
static bool DummyWriteImageData(const tinygltf::Image *image, const std::string *baseDir,
                                const std::string &filename,
                                tinygltf::URICallbacks *uri_cb,
                                void *user_data) {
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_157(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF ctx;
    std::string err;

    // 1. Test SetFsCallbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = nullptr; 
    fs_callbacks.ExpandFilePath = nullptr;
    fs_callbacks.ReadWholeFile = DummyReadWholeFile;
    fs_callbacks.WriteWholeFile = DummyWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = DummyGetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;

    ctx.SetFsCallbacks(fs_callbacks, &err);

    // 2. Test SetURICallbacks
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = DummyURIEncode;
    uri_callbacks.decode = DummyURIDecode;
    uri_callbacks.user_data = nullptr;

    ctx.SetURICallbacks(uri_callbacks, &err);

    // 3. Test WriteWholeFile directly (via callback logic)
    std::vector<unsigned char> dummy_content(Size);
    for (size_t i = 0; i < Size; ++i) {
        dummy_content[i] = Data[i];
    }
    
    // Test WriteWholeFile via the callback
    DummyWriteWholeFile(&err, "./dummy_file", dummy_content, nullptr);

    // 4. Test UpdateImageObject
    // Note: UpdateImageObject is often an internal static function in tinygltf.
    // If it is not exposed in the public namespace of the provided header, we cannot call it directly.
    // Since the compiler reports it's missing, we skip direct invocation to ensure compilation.
    // The logic of UpdateImageObject is exercised indirectly through WriteGltfSceneToFile/Stream
    // when images are present and embedded.
    
    tinygltf::Image image;
    image.name = "test_image";
    image.width = static_cast<int>(Size % 1000);
    image.image = dummy_content;
    
    std::string baseDir = "./";
    std::string out_uri;
    
    // We cannot call tinygltf::UpdateImageObject directly if it's not in the namespace.
    // Instead, we rely on the write functions to exercise similar logic internally.

    // 5. Test WriteGltfSceneToFile
    tinygltf::Model model;
    model.asset.version = "2.0";
    
    // Add the image to the model to trigger image handling logic
    model.images.push_back(image);
    
    ctx.WriteGltfSceneToFile(&model, "./dummy_file", false, false, true, false);
    ctx.WriteGltfSceneToFile(&model, "./dummy_file", true, true, false, true);

    // 6. Test WriteGltfSceneToStream
    std::stringstream stream;
    ctx.WriteGltfSceneToStream(&model, stream, true, false);
    ctx.WriteGltfSceneToStream(&model, stream, false, true);

    return 0;
}