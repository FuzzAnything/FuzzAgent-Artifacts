// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
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
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <random>
#include <cstring>

// Helper to generate random data
static std::vector<uint8_t> GenerateRandomData(size_t size) {
    std::vector<uint8_t> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (size_t i = 0; i < size; ++i) {
        data[i] = static_cast<uint8_t>(dis(gen));
    }
    return data;
}

// Helper to generate a random string
static std::string GenerateRandomString(size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-.";
    std::string str;
    str.reserve(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);
    for (size_t i = 0; i < size; ++i) {
        str += charset[dis(gen)];
    }
    return str;
}

// Custom FsCallbacks implementation for fuzzing
// Note: Function signatures must match tinygltf definitions exactly
static bool FuzzFileExists(const std::string &absfilename, void *user_data) {
    return false; // Assume file doesn't exist to trigger write paths
}

static std::string FuzzExpandFilePath(const std::string &filepath, void *user_data) {
    return filepath;
}

// Signature: bool ReadWholeFileFunction(std::vector<unsigned char> *out, std::string *err, const std::string &filepath, void *user_data)
static bool FuzzReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                              const std::string &filepath, void *user_data) {
    if (err) *err = "File not found in fuzz mode";
    return false;
}

// Signature: bool WriteWholeFileFunction(std::string *err, const std::string &filepath, const std::vector<unsigned char> &contents, void *user_data)
static bool FuzzWriteWholeFile(std::string *err, const std::string &filepath,
                               const std::vector<unsigned char> &contents, void *user_data) {
    // Try to write to a dummy file or ignore
    std::ofstream ofs("./dummy_file", std::ios::binary);
    if (!ofs) {
        if (err) *err = "Failed to open dummy file";
        return false;
    }
    ofs.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    ofs.close();
    return true;
}

// Signature: bool GetFileSizeFunction(size_t *size, std::string *err, const std::string &filepath, void *user_data)
static bool FuzzGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filepath, void *user_data) {
    if (size) *size = 0;
    return true;
}

// Custom URICallbacks implementation for fuzzing
// Signature: bool URIEncodeFunction(const std::string &basepath, const std::string &uri, std::string *out, void *user_data)
static bool FuzzURIEncode(const std::string &basepath, const std::string &uri, std::string *out, void *user_data) {
    // Simple passthrough or basic encoding
    if (out) *out = uri;
    return true;
}

// Signature: bool URIDecodeFunction(const std::string &uri, std::string *out, void *user_data)
static bool FuzzURIDecode(const std::string &uri, std::string *out, void *user_data) {
    // Simple passthrough
    if (out) *out = uri;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Parse input to determine which function to test
    uint8_t mode = Data[0] % 6; // 0-5 for the 6 target functions

    // Prepare common data
    tinygltf::Model model;
    tinygltf::Image image;
    
    // Initialize FsCallbacks with correct signatures
    tinygltf::FsCallbacks fs_cb = {
        FuzzFileExists,
        FuzzExpandFilePath,
        FuzzReadWholeFile,
        FuzzWriteWholeFile,
        FuzzGetFileSizeInBytes,
        nullptr
    };
    
    // Initialize URICallbacks with correct signatures
    tinygltf::URICallbacks uri_cb = {
        FuzzURIEncode,
        FuzzURIDecode,
        nullptr
    };
    
    std::string baseDir = "./";
    std::string out_uri;
    std::string err;
    bool embedImages = (Data[0] & 0x01) != 0;
    bool embedBuffers = (Data[0] & 0x02) != 0;
    bool prettyPrint = (Data[0] & 0x04) != 0;
    bool writeBinary = (Data[0] & 0x08) != 0;

    // Populate model with random data to ensure non-trivial state
    if (Size > 1) {
        size_t num_nodes = Data[1] % 10;
        for (size_t i = 0; i < num_nodes; ++i) {
            tinygltf::Node node;
            node.name = GenerateRandomString(5);
            model.nodes.push_back(node);
        }
    }

    // Populate image with random data
    if (Size > 2) {
        size_t img_size = Data[2] * 100;
        if (img_size > 0) {
            image.image = GenerateRandomData(img_size);
            image.width = Data[2];
            image.name = "fuzz_image";
        }
    }

    switch (mode) {
        case 0: {
            // Test UpdateImageObject
            // Note: UpdateImageObject is a static function in the tinygltf namespace
            // It is not exposed as a public API in some versions, but if available:
            // tinygltf::UpdateImageObject(image, baseDir, 0, embedImages, &fs_cb, &uri_cb, nullptr, nullptr, &out_uri);
            // Since the signature in the prompt suggests it exists, we attempt to call it.
            // If it's not in the public header, this might fail to compile, but the prompt implies it's targetable.
            // However, based on common tinygltf versions, this is often internal. 
            // We will skip direct call if not strictly public, but the prompt asks to fuzz it.
            // Assuming it is accessible as per prompt description:
            // tinygltf::UpdateImageObject(image, baseDir, 0, embedImages, &fs_cb, &uri_cb, nullptr, nullptr, &out_uri);
            // To avoid compilation error if it's internal, we wrap in a try-catch or just skip if not found.
            // Given the error log says "no member named 'UpdateImageObject'", it's likely internal.
            // We will comment out the direct call to UpdateImageObject if it causes issues, 
            // but the prompt specifically lists it. Let's assume the user has a version where it's accessible 
            // or we need to call it via the internal logic if possible. 
            // Since the error explicitly says it's not found, we cannot call it directly.
            // We will skip this case or rely on other functions that internally use it.
            // For the sake of the fuzz driver structure, we leave a placeholder or skip.
            break;
        }
        case 1: {
            // Test WriteGltfSceneToFile
            tinygltf::TinyGLTF ctx;
            // Set callbacks to use our dummy implementation
            ctx.SetFsCallbacks(fs_cb, &err);
            ctx.SetURICallbacks(uri_cb, &err);
            ctx.WriteGltfSceneToFile(&model, "./dummy_file", embedImages, embedBuffers, prettyPrint, writeBinary);
            break;
        }
        case 2: {
            // Test WriteGltfSceneToStream
            tinygltf::TinyGLTF ctx;
            ctx.SetFsCallbacks(fs_cb, &err);
            ctx.SetURICallbacks(uri_cb, &err);
            std::ostringstream stream;
            ctx.WriteGltfSceneToStream(&model, stream, prettyPrint, writeBinary);
            break;
        }
        case 3: {
            // Test SetFsCallbacks
            tinygltf::TinyGLTF ctx;
            ctx.SetFsCallbacks(fs_cb, &err);
            break;
        }
        case 4: {
            // Test WriteImageData
            std::string basepath = "./";
            std::string filename = "test.png";
            tinygltf::WriteImageData(&basepath, &filename, &image, embedImages,
                                     &fs_cb, &uri_cb, &out_uri, nullptr);
            break;
        }
        case 5: {
            // Test SetURICallbacks
            tinygltf::TinyGLTF ctx;
            ctx.SetURICallbacks(uri_cb, &err);
            break;
        }
        default:
            break;
    }

    return 0;
}