// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
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
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <functional>

// Dummy filesystem callbacks for testing SetFsCallbacks
// Note: Function signatures must match tinygltf.h exactly.
// Based on error messages, ReadWholeFile expects: bool(std::vector<unsigned char>*, std::string*, const std::string&, void*)
// GetFileSizeInBytes expects: bool(size_t*, std::string*, const std::string&, void*)
// FileExists expects: bool(const std::string&, void*)
// ExpandFilePath expects: std::string(const std::string&, void*)
// WriteWholeFile expects: bool(std::string*, const std::string&, const std::vector<unsigned char>&, void*)

static bool DummyFileExists(const std::string &filename, void *) {
    (void)filename;
    return false;
}

static std::string DummyExpandFilePath(const std::string &filepath, void *) {
    (void)filepath;
    return "";
}

static bool DummyReadWholeFile(std::vector<unsigned char> *buf, std::string *err,
                               const std::string &filename, void *) {
    (void)filename;
    (void)buf;
    (void)err;
    return false;
}

static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents, void *) {
    (void)err;
    (void)filename;
    (void)contents;
    return false;
}

// Fixed signature: added std::string* err parameter based on compiler error
static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *) {
    (void)size;
    (void)err;
    (void)filename;
    return false;
}

// Dummy URI callbacks for testing SetURICallbacks
// Based on error messages, encode expects: bool(const std::string&, const std::string&, std::string*, void*)
// decode expects: bool(const std::string&, std::string*, void*)

static bool DummyURIEncode(const std::string &uri, const std::string & /*unused*/, std::string *out, void *) {
    if (out) *out = uri;
    return true;
}

static bool DummyURIDecode(const std::string &uri, std::string *out, void *) {
    if (out) *out = uri;
    return true;
}

static void WriteDummyFile(const uint8_t *Data, size_t Size) {
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Fuzz SetFsCallbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = DummyFileExists;
    fs_callbacks.ExpandFilePath = DummyExpandFilePath;
    fs_callbacks.ReadWholeFile = DummyReadWholeFile;
    fs_callbacks.WriteWholeFile = DummyWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = DummyGetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;

    // Try with valid callbacks
    loader.SetFsCallbacks(fs_callbacks, &err);

    // Try with null callbacks (should fail)
    tinygltf::FsCallbacks null_fs_callbacks;
    null_fs_callbacks.FileExists = nullptr;
    null_fs_callbacks.ExpandFilePath = nullptr;
    null_fs_callbacks.ReadWholeFile = nullptr;
    null_fs_callbacks.WriteWholeFile = nullptr;
    null_fs_callbacks.GetFileSizeInBytes = nullptr;
    null_fs_callbacks.user_data = nullptr;
    loader.SetFsCallbacks(null_fs_callbacks, &err);

    // Fuzz SetURICallbacks
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = DummyURIEncode;
    uri_callbacks.decode = DummyURIDecode;
    uri_callbacks.user_data = nullptr;

    loader.SetURICallbacks(uri_callbacks, &err);

    // Try with null callbacks
    tinygltf::URICallbacks null_uri_callbacks;
    null_uri_callbacks.encode = nullptr;
    null_uri_callbacks.decode = nullptr;
    null_uri_callbacks.user_data = nullptr;
    loader.SetURICallbacks(null_uri_callbacks, &err);

    // Fuzz LoadBinaryFromMemory
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 0);
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 1);
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 2);

    // Fuzz LoadASCIIFromString
    // Treat input as string (may contain nulls, but function takes const char*)
    loader.LoadASCIIFromString(&model, &err, &warn, reinterpret_cast<const char*>(Data), Size, "", 0);
    loader.LoadASCIIFromString(&model, &err, &warn, reinterpret_cast<const char*>(Data), Size, "", 1);
    loader.LoadASCIIFromString(&model, &err, &warn, reinterpret_cast<const char*>(Data), Size, "", 2);

    // Fuzz LoadASCIIFromFile and LoadBinaryFromFile
    // Write data to a dummy file first
    WriteDummyFile(Data, Size);

    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", 0);
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", 1);
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", 2);

    loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file", 0);
    loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file", 1);
    loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file", 2);

    return 0;
}