// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
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

// Dummy file path for file-based operations
static const char* DUMMY_FILE_PATH = "./dummy_file";

// Helper to create a Model with some fuzzed data to ensure non-empty state
static tinygltf::Model CreateFuzzedModel(const uint8_t* data, size_t size) {
    tinygltf::Model model;
    
    // Add some buffers if data is available
    if (size > 0) {
        tinygltf::Buffer buffer;
        buffer.name = "fuzz_buffer";
        buffer.data.assign(data, data + size);
        model.buffers.push_back(buffer);
    }

    // Add a dummy image
    tinygltf::Image img;
    img.name = "fuzz_img";
    if (size > 0) {
        img.width = 1;
        img.image.assign(data, data + (size < 4 ? size : 4));
    }
    model.images.push_back(img);

    // Add a dummy mesh
    tinygltf::Mesh mesh;
    mesh.name = "fuzz_mesh";
    model.meshes.push_back(mesh);

    // Add a dummy scene
    tinygltf::Scene scene;
    scene.name = "fuzz_scene";
    model.scenes.push_back(scene);

    return model;
}

// Dummy callback implementations for FsCallbacks
static bool ReadWholeFileFunc(std::vector<unsigned char>* out, std::string* err,
                              const std::string& filename, void* user_data) {
    if (err) err->clear();
    if (out) out->clear();
    return false;
}

static bool GetFileSizeInBytesFunc(size_t* size, std::string* err, const std::string& filename, void* user_data) {
    if (size) *size = 0;
    if (err) err->clear();
    return false;
}

// Dummy callback implementations for URICallbacks
static bool URIEncodeFunc(const std::string& input, const std::string& basepath, std::string* out, void* user_data) {
    if (out) *out = input;
    return true;
}

static bool URIDecodeFunc(const std::string& input, std::string* out, void* user_data) {
    if (out) *out = input;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize TinyGLTF instance
    tinygltf::TinyGLTF ctx;

    // Prepare callbacks with correct signatures
    tinygltf::FsCallbacks fs_callbacks = {
        nullptr, // FileExists
        nullptr, // ExpandFilePath
        ReadWholeFileFunc,
        nullptr, // WriteWholeFile
        GetFileSizeInBytesFunc,
        nullptr // user_data
    };

    tinygltf::URICallbacks uri_callbacks = {
        URIEncodeFunc,
        URIDecodeFunc,
        nullptr // user_data
    };

    std::string err;

    // 1. Test SetFsCallbacks (overload with err)
    ctx.SetFsCallbacks(fs_callbacks, &err);

    // 2. Test SetFsCallbacks (overload without err)
    ctx.SetFsCallbacks(fs_callbacks);

    // 3. Test SetURICallbacks
    ctx.SetURICallbacks(uri_callbacks, &err);

    // Create a model with fuzzed data
    tinygltf::Model model = CreateFuzzedModel(Data, Size);

    // 4. Test WriteGltfSceneToStream (Binary)
    {
        std::ostringstream stream;
        ctx.WriteGltfSceneToStream(&model, stream, false, true);
    }

    // 5. Test WriteGltfSceneToStream (JSON)
    {
        std::ostringstream stream;
        ctx.WriteGltfSceneToStream(&model, stream, true, false);
    }

    // 6. Test WriteGltfSceneToFile (Binary)
    ctx.WriteGltfSceneToFile(&model, DUMMY_FILE_PATH, false, false, false, true);

    // 7. Test WriteGltfSceneToFile (JSON)
    ctx.WriteGltfSceneToFile(&model, DUMMY_FILE_PATH, false, false, true, false);

    // 8. Test WriteGltfSceneToFile (Embed Images)
    ctx.WriteGltfSceneToFile(&model, DUMMY_FILE_PATH, true, false, true, false);

    // 9. Test WriteGltfSceneToFile (Embed Buffers)
    ctx.WriteGltfSceneToFile(&model, DUMMY_FILE_PATH, false, true, true, false);

    // 10. Test WriteImageData
    tinygltf::Image img;
    if (Size > 0) {
        img.name = "test_img";
        img.width = 1;
        img.image.assign(Data, Data + (Size < 4 ? Size : 4));
    }

    std::string out_uri;
    
    // WriteImageData requires valid basepath and filename pointers if not embedding.
    // To avoid the crash on nullptr dereference inside tinygltf when checking extensions,
    // we provide valid strings.
    std::string basepath = "./";
    std::string filename = "test.png";

    // WriteImageData with embedImages = true (filename/basepath might be ignored but must be valid)
    tinygltf::WriteImageData(&basepath, &filename, &img, true, &fs_callbacks, &uri_callbacks, &out_uri, nullptr);
    
    // WriteImageData with embedImages = false
    tinygltf::WriteImageData(&basepath, &filename, &img, false, &fs_callbacks, &uri_callbacks, &out_uri, nullptr);

    return 0;
}