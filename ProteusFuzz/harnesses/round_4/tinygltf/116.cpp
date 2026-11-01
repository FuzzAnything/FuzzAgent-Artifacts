// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
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

// Dummy functions matching the expected signatures in tinygltf
// ReadWholeFile: bool (std::vector<unsigned char>*, std::string*, const std::string&, void*)
static bool DummyReadWholeFile(std::vector<unsigned char> *buf, std::string *err,
                               const std::string &filename, void *user_data) {
    (void)user_data;
    if (err) err->clear();
    if (buf) buf->clear();
    return false;
}

// WriteWholeFile: bool (std::string*, const std::string&, const std::vector<unsigned char>&, void*)
// Note: The 3rd argument is const reference, not pointer, based on the error message analysis
// Error message: function<bool (basic_string<char> *, const basic_string<char> &, const vector<unsigned char> &, void *)>
static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents, void *user_data) {
    (void)user_data;
    if (err) err->clear();
    return false;
}

// GetFileSizeInBytes: bool (size_t*, std::string*, const std::string&, void*)
static bool DummyGetFileSizeInBytes(size_t *size, std::string *err,
                                    const std::string &filename, void *user_data) {
    (void)user_data;
    if (err) err->clear();
    if (size) *size = 0;
    return false;
}

// URIEncode: bool (const std::string&, const std::string&, std::string*, void*)
static bool DummyURIEncode(const std::string &str, const std::string &encoding, std::string *out, void *user_data) {
    (void)user_data;
    (void)encoding;
    if (out) *out = str;
    return true;
}

// URIDecode: bool (const std::string&, std::string*, void*)
static bool DummyURIDecode(const std::string &str, std::string *out, void *user_data) {
    (void)user_data;
    if (out) *out = str;
    return true;
}

static bool DummyFileExists(const std::string &filename, void *user_data) {
    (void)user_data;
    return false;
}

static std::string DummyExpandFilePath(const std::string &filepath, void *user_data) {
    (void)user_data;
    return filepath;
}

extern "C" int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Fuzz LoadBinaryFromMemory
    ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, static_cast<unsigned int>(Size), "", 0);
    model = tinygltf::Model(); // Reset

    // Fuzz LoadASCIIFromString
    ctx.LoadASCIIFromString(&model, &err, &warn, reinterpret_cast<const char*>(Data), Size, "", 0);
    model = tinygltf::Model(); // Reset

    // Fuzz SetFsCallbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = &DummyFileExists;
    fs_callbacks.ExpandFilePath = &DummyExpandFilePath;
    fs_callbacks.ReadWholeFile = &DummyReadWholeFile;
    fs_callbacks.WriteWholeFile = &DummyWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = &DummyGetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;
    ctx.SetFsCallbacks(fs_callbacks, &err);

    // Fuzz SetURICallbacks
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = &DummyURIEncode;
    uri_callbacks.decode = &DummyURIDecode;
    uri_callbacks.user_data = nullptr;
    ctx.SetURICallbacks(uri_callbacks, &err);

    // Fuzz LoadBinaryFromFile and LoadASCIIFromFile using a dummy file
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);

        ctx.LoadBinaryFromFile(&model, &err, &warn, dummy_file, 0);
        model = tinygltf::Model(); // Reset

        ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file, 0);
        model = tinygltf::Model(); // Reset

        // Cleanup
        std::remove(dummy_file);
    }

    return 0;
}