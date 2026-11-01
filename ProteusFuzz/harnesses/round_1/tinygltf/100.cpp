// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
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

// Helper to create a dummy file if needed
static void CreateDummyFile(const uint8_t *Data, size_t Size) {
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create dummy file for potential side effects
    CreateDummyFile(Data, Size);

    // Initialize TinyGLTF context
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;

    // LoadFile is the correct method to load from a file path.
    // The signature is typically: LoadFile(Model*, std::string*, std::string*, const std::string&, int)
    // We pass the dummy file path.
    // Note: In some versions of tinygltf, the method might be named differently or have different parameters.
    // Assuming the standard API:
    bool success = ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");

    // If LoadASCIIFromFile fails, we might still want to try parsing if the library exposes a public JSON parser.
    // However, LoadASCIIFromFile is the standard way to feed data into the parser.
    // The fuzzer input is written to "./dummy_file" and parsed by the library's internal logic.

    return 0;
}