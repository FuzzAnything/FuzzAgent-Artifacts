// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.h:1467:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.h:1467:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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

extern "C" int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a tinygltf::TinyGLTF instance
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Convert input data to a string for JSON parsing
    std::string json_str(reinterpret_cast<const char*>(Data), Size);

    // Fuzz SetStoreOriginalJSONForExtrasAndExtensions
    // Toggle based on first byte to explore different states
    bool enable_json_storage = (Data[0] % 2 == 0);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(enable_json_storage);

    // Fuzz SetParseStrictness
    // Use second byte to determine strictness, check size first
    tinygltf::ParseStrictness strictness = tinygltf::Permissive;
    if (Size > 1) {
        strictness = (Data[1] % 2 == 0) ? tinygltf::Permissive : tinygltf::Strict;
    }
    loader.SetParseStrictness(strictness);

    // LoadFromString is private and requires 7 arguments.
    // Instead, we use LoadFromString which is public.
    // We write the fuzz input to a temporary file and load it.
    const char* dummy_file = "./dummy_file";
    FILE* f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
        
        // Try loading from file
        // LoadFromFile signature: bool LoadFromFile(Model* model, std::string* err, std::string* warn, const std::string& filename);
        bool file_success = loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file);
        // Clean up
        remove(dummy_file);
    }

    // Additional fuzzing: Try to call SetStoreOriginalJSONForExtrasAndExtensions and SetParseStrictness
    // with different values in a loop to ensure state changes are handled.
    for (int i = 0; i < 10; ++i) {
        loader.SetStoreOriginalJSONForExtrasAndExtensions((i % 2) == 0);
        loader.SetParseStrictness((i % 2) == 0 ? tinygltf::Permissive : tinygltf::Strict);
    }

    // Try to parse a minimal valid glTF string by writing to file
    std::string valid_gltf = R"({"asset":{"version":"2.0"}})";
    f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(valid_gltf.c_str(), 1, valid_gltf.size(), f);
        fclose(f);
        loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file);
        remove(dummy_file);
    }

    // Try to parse an invalid glTF string
    std::string invalid_gltf = "not json";
    f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(invalid_gltf.c_str(), 1, invalid_gltf.size(), f);
        fclose(f);
        loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file);
        remove(dummy_file);
    }

    // Try to parse a glTF with a mesh
    std::string mesh_gltf = R"({"asset":{"version":"2.0"},"meshes":[{"primitives":[{"attributes":{"POSITION":0}}]}]})";
    f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(mesh_gltf.c_str(), 1, mesh_gltf.size(), f);
        fclose(f);
        loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file);
        remove(dummy_file);
    }

    // Try to parse a glTF with extras and extensions
    std::string extras_gltf = R"({"asset":{"version":"2.0","extras":{"key":"value"}},"extensionsUsed":["EXT_mesh_gpu_instancing"]})";
    f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(extras_gltf.c_str(), 1, extras_gltf.size(), f);
        fclose(f);
        loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file);
        remove(dummy_file);
    }

    return 0;
}