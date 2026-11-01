// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Convert fuzz data to a string
    std::string json_str(reinterpret_cast<const char*>(Data), Size);

    // Initialize the loader
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Attempt to load the model from the string.
    // Signature: LoadASCIIFromString(Model*, std::string*, std::string*, const char*, unsigned int, const std::string&, unsigned int)
    // We pass empty base_dir and default check_sections (REQUIRE_VERSION)
    bool success = loader.LoadASCIIFromString(&model, &err, &warn, json_str.c_str(), Size, "", tinygltf::REQUIRE_VERSION);

    (void)success;
    (void)err;
    (void)warn;

    // Additional exploration: Try loading from a file
    const char* dummy_file = "./dummy_file.gltf";
    FILE* f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
        
        tinygltf::Model model2;
        std::string err2;
        std::string warn2;
        loader.LoadASCIIFromFile(&model2, &err2, &warn2, dummy_file);
        
        // Cleanup
        std::remove(dummy_file);
    }

    return 0;
}