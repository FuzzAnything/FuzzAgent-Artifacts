// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // 1. Fuzz LoadBinaryFromMemory (Overload 1: with check_sections)
    // Using the full buffer as binary data
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "./dummy_file", tinygltf::REQUIRE_VERSION);
    // Reset model by reassigning a default constructed instance
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // 2. Fuzz LoadBinaryFromMemory (Overload 2: default check_sections)
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "./dummy_file");
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // 3. Fuzz LoadASCIIFromFile
    // Write data to a dummy file to simulate ASCII input
    WriteDummyFile("./dummy_file.json", Data, Size);
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file.json", tinygltf::REQUIRE_VERSION);
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // 4. Fuzz LoadBinaryFromFile
    // Write data to a dummy file to simulate binary input
    WriteDummyFile("./dummy_file.glb", Data, Size);
    loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file.glb", tinygltf::REQUIRE_VERSION);
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // 5. Fuzz SetPreserveImageChannels and GetImagesAsIs
    // Toggle state based on input data to explore different branches
    if (Size > 0) {
        loader.SetPreserveImageChannels((Data[0] % 2) == 1);
        loader.GetImagesAsIs();
    }
    
    // Try another toggle
    loader.SetPreserveImageChannels((Data[0] % 2) == 0);
    loader.GetImagesAsIs();

    // Cleanup is handled by RAII (std::string, tinygltf::Model)
    return 0;
}