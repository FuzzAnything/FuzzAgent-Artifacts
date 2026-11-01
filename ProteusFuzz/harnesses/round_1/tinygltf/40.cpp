// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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
#include <fstream>

// tinygltf is a single-header library.
#include "tiny_gltf.h"

static void WriteDummyFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write data to dummy file
    WriteDummyFile(Data, Size);

    std::string err;
    std::string warn;
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;

    // Convert raw bytes to string.
    std::string json_str(reinterpret_cast<const char*>(Data), Size);
    
    // LoadFromString is private in some versions or requires specific arguments.
    // We use the public API LoadASCIIFromFile which internally calls the parsing logic.
    // We pass the base_dir as empty and check_sections as 0.
    // The function signature for LoadASCIIFromFile is typically:
    // bool LoadASCIIFromFile(Model *model, std::string *err, std::string *warn, 
    //                        const std::string &filename, unsigned int check_sections);
    
    // 1. Fuzz LoadASCIIFromFile
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", 0);

    // 2. Try to load with a different model instance
    tinygltf::Model model2;
    loader.LoadASCIIFromFile(&model2, &err, &warn, "./dummy_file", 0);

    // 3. Try to load as binary if the data starts with glTF magic
    tinygltf::Model model3;
    if (Size >= 4) {
        uint32_t magic = 0;
        std::memcpy(&magic, Data, 4);
        if (magic == 0x46546C67) { // "glTF" magic number
             // LoadBinaryFromMemory signature:
             // bool LoadBinaryFromMemory(Model *model, std::string *err, std::string *warn, 
             //                           const unsigned char *bytes, unsigned int size, 
             //                           const std::string &base_dir, unsigned int check_sections);
             loader.LoadBinaryFromMemory(&model3, &err, &warn, Data, Size, "", 0);
        }
    }

    // 4. Try to access specific properties if the model was loaded successfully
    // This indirectly tests the parsing of Accessor, Texture, AudioSource, etc.
    if (!model.accessors.empty()) {
        const auto& acc = model.accessors[0];
        (void)acc; 
    }
    if (!model.textures.empty()) {
        const auto& tex = model.textures[0];
        (void)tex;
    }
    if (!model.audioSources.empty()) {
        const auto& audio = model.audioSources[0];
        (void)audio;
    }

    return 0;
}