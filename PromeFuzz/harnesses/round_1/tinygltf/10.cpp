// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
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
#include <cstring>
#include <fstream>
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Test SetMaxExternalFileSize and GetMaxExternalFileSize
    if (Size > 0) {
        size_t max_size = static_cast<size_t>(Data[0]) * 1024;
        loader.SetMaxExternalFileSize(max_size);
    }
    size_t current_max = loader.GetMaxExternalFileSize();
    (void)current_max;

    // Test GetPreserveImageChannels
    bool preserve = loader.GetPreserveImageChannels();
    (void)preserve;

    // Test LoadBinaryFromMemory - requires at least 20 bytes for header validation
    if (Size >= 20) {
        loader.LoadBinaryFromMemory(&model, &err, &warn, Data, static_cast<unsigned int>(Size), "", tinygltf::REQUIRE_VERSION);
    }

    // Test LoadASCIIFromFile
    writeDummyFile(Data, Size);
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", tinygltf::REQUIRE_VERSION);

    // Test LoadBinaryFromFile
    writeDummyFile(Data, Size);
    loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file", tinygltf::REQUIRE_VERSION);

    // Clean up dummy file
    std::remove("./dummy_file");

    return 0;
}