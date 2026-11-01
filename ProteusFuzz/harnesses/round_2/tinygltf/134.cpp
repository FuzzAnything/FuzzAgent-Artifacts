// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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

// Fuzz driver for tinygltf parsing functions
extern "C" int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char* dummy_file = "./dummy_file";
    FILE* f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
        f = nullptr; // Reset pointer to avoid double free
    }

    try {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        // Attempt to load the data as a GLB file using LoadBinaryFromFile or LoadBinary
        bool is_binary = false;
        if (Size >= 4 && Data[0] == 'g' && Data[1] == 'l' && Data[2] == 'T' && Data[3] == 'F') {
            is_binary = true;
        }

        if (is_binary) {
            loader.LoadBinaryFromFile(&model, &err, &warn, dummy_file, 1024);
        } else {
            loader.LoadASCIIFromString(&model, &err, &warn, 
                                      reinterpret_cast<const char*>(Data), 
                                      Size, 
                                      "", 
                                      tinygltf::REQUIRE_VERSION);
        }

        // If the above failed, try loading from file as ASCII
        if (model.nodes.empty() && model.scenes.empty() && model.meshes.empty()) {
            loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);
        }

        // Try with different check_sections flags to explore more states
        if (model.nodes.empty() && model.scenes.empty() && model.meshes.empty()) {
            loader.LoadASCIIFromString(&model, &err, &warn, 
                                      reinterpret_cast<const char*>(Data), 
                                      Size, 
                                      "", 
                                      tinygltf::REQUIRE_VERSION);
        }

        // Try with REQUIRE_VERSION flag (only valid option)
        if (model.nodes.empty() && model.scenes.empty() && model.meshes.empty()) {
            loader.LoadASCIIFromString(&model, &err, &warn, 
                                      reinterpret_cast<const char*>(Data), 
                                      Size, 
                                      "", 
                                      tinygltf::REQUIRE_VERSION);
        }

    } catch (...) {
        // Ignore exceptions during fuzzing
    }

    // Cleanup
    std::remove(dummy_file);

    return 0;
}