// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.h:1478:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
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

static void WriteDummyFile(const std::string &filename, const uint8_t *Data, size_t Size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

static bool DummyEncode(const std::string &in, const std::string &out, std::string *err, void *user_data) {
    return true;
}

static bool DummyDecode(const std::string &in, std::string *out, void *user_data) {
    if (out) *out = in;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF parser;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Fuzz SetPreserveImageChannels
    if (Size > 0) {
        parser.SetPreserveImageChannels(Data[0] % 2 != 0);
    }

    // Fuzz SetURICallbacks
    tinygltf::URICallbacks callbacks;
    callbacks.encode = DummyEncode;
    callbacks.decode = DummyDecode;
    callbacks.user_data = nullptr;
    parser.SetURICallbacks(callbacks, &err);

    // Fuzz RemoveImageLoader
    parser.RemoveImageLoader();

    // Write data to dummy files for file-based loaders
    WriteDummyFile("./dummy_ascii.gltf", Data, Size);
    WriteDummyFile("./dummy_binary.glb", Data, Size);

    // Fuzz LoadASCIIFromFile
    parser.LoadASCIIFromFile(&model, &err, &warn, "./dummy_ascii.gltf", tinygltf::REQUIRE_VERSION);
    parser.LoadASCIIFromFile(&model, &err, &warn, "./dummy_ascii.gltf", tinygltf::REQUIRE_VERSION | tinygltf::REQUIRE_SCENE);
    parser.LoadASCIIFromFile(&model, &err, &warn, "./dummy_ascii.gltf", 0);

    // Fuzz LoadBinaryFromFile
    parser.LoadBinaryFromFile(&model, &err, &warn, "./dummy_binary.glb", tinygltf::REQUIRE_VERSION);
    parser.LoadBinaryFromFile(&model, &err, &warn, "./dummy_binary.glb", tinygltf::REQUIRE_VERSION | tinygltf::REQUIRE_SCENE);
    parser.LoadBinaryFromFile(&model, &err, &warn, "./dummy_binary.glb", 0);

    // Fuzz LoadASCIIFromString
    const char *str = reinterpret_cast<const char*>(Data);
    parser.LoadASCIIFromString(&model, &err, &warn, str, Size, "./", tinygltf::REQUIRE_VERSION);
    parser.LoadASCIIFromString(&model, &err, &warn, str, Size, "./", tinygltf::REQUIRE_VERSION | tinygltf::REQUIRE_SCENE);
    parser.LoadASCIIFromString(&model, &err, &warn, str, Size, "./", 0);

    // Fuzz LoadASCIIFromString with truncated data
    if (Size > 10) {
        parser.LoadASCIIFromString(&model, &err, &warn, str, 10, "./", tinygltf::REQUIRE_VERSION);
    }

    return 0;
}