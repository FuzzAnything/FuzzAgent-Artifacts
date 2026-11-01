// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>

static void WriteDummyFile(const uint8_t *Data, size_t Size, const std::string &filename) {
    FILE *f = fopen(filename.c_str(), "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Fuzz SetStoreOriginalJSONForExtrasAndExtensions and GetStoreOriginalJSONForExtrasAndExtensions
    bool flag = (Data[0] % 2) != 0;
    loader.SetStoreOriginalJSONForExtrasAndExtensions(flag);
    bool currentFlag = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)currentFlag; // Use variable to avoid unused warning

    // Fuzz LoadASCIIFromString (two overloads)
    std::string asciiData(reinterpret_cast<const char*>(Data), Size);
    // Overload 1: const char *str, const unsigned int length
    loader.LoadASCIIFromString(&model, &err, &warn, asciiData.c_str(), static_cast<unsigned int>(Size), "", tinygltf::REQUIRE_VERSION);
    // Overload 2: const char *str, unsigned int length
    loader.LoadASCIIFromString(&model, &err, &warn, asciiData.c_str(), Size, "", tinygltf::REQUIRE_VERSION);

    // Reset model for next tests
    model = tinygltf::Model();

    // Fuzz LoadASCIIFromFile
    WriteDummyFile(Data, Size, "./dummy_file.gltf");
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file.gltf", tinygltf::REQUIRE_VERSION);

    // Reset model for next tests
    model = tinygltf::Model();

    // Fuzz LoadBinaryFromFile
    WriteDummyFile(Data, Size, "./dummy_file.glb");
    loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file.glb", tinygltf::REQUIRE_VERSION);

    // Cleanup
    return 0;
}