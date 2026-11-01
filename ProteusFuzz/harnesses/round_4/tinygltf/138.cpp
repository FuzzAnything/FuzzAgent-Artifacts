// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
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

// Static helper functions to avoid name conflicts and encapsulate logic
// Function signatures must match the typedefs in tiny_gltf.h exactly.

static bool FileExistsFunc(const std::string& filename, void* user_data) {
    return false;
}

static std::string ExpandFilePathFunc(const std::string& filepath, void* user_data) {
    return "";
}

static bool ReadWholeFileFunc(std::vector<unsigned char>* out, std::string* err,
                              const std::string& filename, void* user_data) {
    return false;
}

static bool WriteWholeFileFunc(std::string* err, const std::string& filename,
                               const std::vector<unsigned char>& contents, void* user_data) {
    return false;
}

static bool GetFileSizeInBytesFunc(unsigned long* size, std::string* err,
                                   const std::string& filename, void* user_data) {
    return false;
}

// URICallbacks signatures in tinygltf are:
// bool (const std::string&, const std::string&, std::string*, void*) for encode
// bool (const std::string&, std::string*, void*) for decode
// Note: The exact signature depends on the version, but typically they return bool
// and take string references.
// Based on common tinygltf implementations:
// typedef bool (*URIEncodeFunction)(const std::string& uri, const std::string& base_dir, std::string* out, void* user_data);
// typedef bool (*URIDecodeFunction)(const std::string& uri, std::string* out, void* user_data);

static bool URICodeFunc(const std::string& uri, const std::string& base_dir, std::string* out, void* user_data) {
    if (out) *out = uri;
    return true;
}

static bool URIDecodeFunc(const std::string& uri, std::string* out, void* user_data) {
    if (out) *out = uri;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Prepare data for callbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = FileExistsFunc;
    fs_callbacks.ExpandFilePath = ExpandFilePathFunc;
    fs_callbacks.ReadWholeFile = ReadWholeFileFunc;
    fs_callbacks.WriteWholeFile = WriteWholeFileFunc;
    fs_callbacks.GetFileSizeInBytes = GetFileSizeInBytesFunc;
    fs_callbacks.user_data = nullptr;

    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = URICodeFunc;
    uri_callbacks.decode = URIDecodeFunc;
    uri_callbacks.user_data = nullptr;

    // Fuzz SetFsCallbacks
    ctx.SetFsCallbacks(fs_callbacks, &err);

    // Fuzz SetURICallbacks
    ctx.SetURICallbacks(uri_callbacks, &err);

    // Fuzz LoadBinaryFromMemory
    // Pass the raw input data as binary glTF
    ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 0);

    // Reset model for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // Fuzz LoadASCIIFromString
    // Treat input as ASCII JSON string (might fail, which is expected for fuzzing)
    ctx.LoadASCIIFromString(&model, &err, &warn, reinterpret_cast<const char*>(Data), Size, "", 0);

    // Reset model for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // Fuzz LoadBinaryFromFile and LoadASCIIFromFile
    // Write input data to a dummy file to test file-based loaders
    const char *dummy_file = "./dummy_file";
    std::ofstream ofs(dummy_file, std::ios::binary);
    if (ofs.is_open()) {
        ofs.write(reinterpret_cast<const char*>(Data), Size);
        ofs.close();

        // Fuzz LoadBinaryFromFile
        ctx.LoadBinaryFromFile(&model, &err, &warn, dummy_file, 0);

        // Reset model
        model = tinygltf::Model();
        err.clear();
        warn.clear();

        // Fuzz LoadASCIIFromFile
        ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file, 0);
    }

    // Cleanup
    // Model destructor handles cleanup
    return 0;
}