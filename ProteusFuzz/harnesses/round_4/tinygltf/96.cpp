// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.h:1483:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include "tiny_gltf.h"

// Custom callback implementations for fuzzing matching tinygltf signatures

// Dummy image loader matching LoadImageDataFunction
static bool FuzzImageLoader(tinygltf::Image *image,
                            const int image_idx,
                            const std::string *ext,
                            std::string *err,
                            int width, int height,
                            const unsigned char *bytes,
                            int size,
                            void *user_data) {
    (void)image_idx; (void)ext; (void)width; (void)height; (void)user_data;
    if (image && size > 0) {
        image->image.assign(bytes, bytes + size);
        image->width = width;
        image->height = height;
    }
    return true;
}

// Dummy image writer matching WriteImageDataFunction
static bool FuzzImageWriter(const std::string *imagepath,
                            const std::string *basepath,
                            const tinygltf::Image *image,
                            bool embedImages,
                            const tinygltf::FsCallbacks *fs,
                            const tinygltf::URICallbacks *uri,
                            std::string *err,
                            void *user_data) {
    (void)imagepath; (void)basepath; (void)image; (void)embedImages;
    (void)fs; (void)uri; (void)err; (void)user_data;
    return true;
}

// Dummy URI encoder matching URICallbacks::encode signature
static bool FuzzURIEncode(const std::string &in,
                          const std::string &uri,
                          std::string *out,
                          void *user_data) {
    (void)in; (void)uri; (void)user_data;
    if (out) {
        *out = uri;
    }
    return true;
}

// Dummy URI decoder matching URICallbacks::decode signature
static bool FuzzURIDecode(const std::string &in,
                          std::string *out,
                          void *user_data) {
    (void)in; (void)user_data;
    if (out) {
        *out = in;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;

    // 1. SetImagesAsIs
    bool imagesAsIs = (Data[0] & 0x01) != 0;
    ctx.SetImagesAsIs(imagesAsIs);

    // 2. SetImageLoader
    if (Data[0] & 0x02) {
        ctx.SetImageLoader(FuzzImageLoader, nullptr);
    } else {
        ctx.SetImageLoader(nullptr, nullptr);
    }

    // 3. SetImageWriter
    ctx.SetImageWriter(FuzzImageWriter, nullptr);

    // 4. SetURICallbacks
    tinygltf::URICallbacks callbacks;
    callbacks.encode = FuzzURIEncode;
    callbacks.decode = FuzzURIDecode;
    callbacks.user_data = nullptr;

    // Test with valid callbacks
    bool uriResult = ctx.SetURICallbacks(callbacks, &err);
    (void)uriResult;

    // Test with invalid callbacks (null functions)
    tinygltf::URICallbacks invalidCallbacks;
    invalidCallbacks.encode = nullptr;
    invalidCallbacks.decode = nullptr;
    invalidCallbacks.user_data = nullptr;
    ctx.SetURICallbacks(invalidCallbacks, &err);

    // 5. WriteGltfSceneToStream
    std::ostringstream stream;
    bool prettyPrint = (Data[0] & 0x04) != 0;
    bool writeBinary = (Data[0] & 0x08) != 0;

    ctx.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);

    // 6. WriteGltfSceneToFile
    const char *filename = "./dummy_file";
    bool embedImages = (Data[0] & 0x10) != 0;
    bool embedBuffers = (Data[0] & 0x20) != 0;

    ctx.WriteGltfSceneToFile(&model, filename, embedImages, embedBuffers, prettyPrint, writeBinary);

    // Cleanup: Remove file if it exists
    std::remove(filename);

    return 0;
}