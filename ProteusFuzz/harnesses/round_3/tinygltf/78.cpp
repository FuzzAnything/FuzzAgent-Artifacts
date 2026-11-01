// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToStream at tiny_gltf.h:1454:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
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
#include <fstream>
#include <functional>
#include "tiny_gltf.h"

// Forward declarations for function pointers to match tinygltf types
// Based on the error messages, the expected signatures are:
// ReadWholeFile: bool(std::vector<unsigned char>*, std::string*, const std::string&, void*)
// GetFileSizeInBytes: bool(size_t*, std::string*, const std::string&, void*)
// URIEncode: bool(const std::string&, const std::string&, std::string*, void*)
// URIDecode: bool(const std::string&, std::string*, void*) - Note: Decode has different signature than Encode

static bool FuzzFileExists(const std::string &filepath, void *) {
    return true;
}

static std::string FuzzExpandFilePath(const std::string &filepath, void *) {
    return filepath;
}

// Corrected: std::vector<unsigned char>* comes first, then std::string* for err
static bool FuzzReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                              const std::string &filepath, void *) {
    if (err) err->clear();
    if (out) out->clear();
    return true;
}

// Corrected: std::string* (err) is the second argument
static bool FuzzWriteWholeFile(std::string *err, const std::string &filepath,
                               const std::vector<unsigned char> &contents, void *) {
    if (err) err->clear();
    std::ofstream ofs(filepath, std::ios::binary);
    if (!ofs) {
        if (err) *err = "Failed to open file for writing";
        return false;
    }
    ofs.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    return true;
}

// Corrected: size_t*, std::string* (err), const std::string& (filepath), void*
static bool FuzzGetFileSizeInBytes(size_t *out, std::string *err, const std::string &filepath, void *) {
    if (out) *out = 0;
    if (err) err->clear();
    return true;
}

// Corrected: const std::string& (uri), const std::string& (context), std::string* (out), void*
static bool FuzzURIEncode(const std::string &uri, const std::string &context, std::string *out, void *) {
    if (out) *out = uri;
    return true;
}

// Corrected: const std::string& (uri), std::string* (out), void*
// The error message indicates URIDecode expects: bool(const std::string&, std::string*, void*)
static bool FuzzURIDecode(const std::string &uri, std::string *out, void *) {
    if (out) *out = uri;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF ctx;
    std::string err;

    // Prepare FsCallbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = FuzzFileExists;
    fs_callbacks.ExpandFilePath = FuzzExpandFilePath;
    fs_callbacks.ReadWholeFile = FuzzReadWholeFile;
    fs_callbacks.WriteWholeFile = FuzzWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = FuzzGetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;

    // Prepare URICallbacks
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = FuzzURIEncode;
    uri_callbacks.decode = FuzzURIDecode;
    uri_callbacks.user_data = nullptr;

    // 1. Test SetFsCallbacks (with err pointer)
    ctx.SetFsCallbacks(fs_callbacks, &err);

    // 1. Test SetFsCallbacks (without err pointer)
    ctx.SetFsCallbacks(fs_callbacks, nullptr);

    // 2. Test SetURICallbacks
    ctx.SetURICallbacks(uri_callbacks, &err);

    // 3. Test WriteWholeFile directly
    std::vector<unsigned char> dummy_contents(Size);
    for (size_t i = 0; i < Size; ++i) {
        dummy_contents[i] = Data[i];
    }
    std::string write_err;
    tinygltf::WriteWholeFile(&write_err, "./dummy_file", dummy_contents, nullptr);

    // 4. Test WriteGltfSceneToStream
    tinygltf::Model model;
    std::ostringstream stream;
    ctx.WriteGltfSceneToStream(&model, stream, true, false); // JSON
    ctx.WriteGltfSceneToStream(&model, stream, false, true); // Binary

    // 5. Test WriteGltfSceneToFile
    ctx.WriteGltfSceneToFile(&model, "./dummy_file.gltf", false, false, true, false);
    ctx.WriteGltfSceneToFile(&model, "./dummy_file.glb", true, true, false, true);

    return 0;
}