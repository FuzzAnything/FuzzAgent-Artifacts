// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
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
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Fuzz SetImagesAsIs
    if (Size > 0) {
        loader.SetImagesAsIs((Data[0] % 2) == 0);
    }

    // Fuzz SetMaxExternalFileSize and GetMaxExternalFileSize
    if (Size > 1) {
        size_t max_size = static_cast<size_t>(Data[1]) * 1024 * 1024; // Scale up for variety
        loader.SetMaxExternalFileSize(max_size);
        (void)loader.GetMaxExternalFileSize(); // Just call to exercise
    }

    // Prepare data for memory loading
    const uint8_t *memory_data = Data;
    size_t memory_size = Size;

    // Fuzz LoadBinaryFromMemory (Overload 1: with base_dir)
    // We use a simple string derived from input as base_dir to avoid external dependencies
    std::string base_dir("./");
    if (Size > 2) {
        // Create a dummy base_dir string from input bytes if possible, but keep it safe
        // Just use a fixed dummy path to avoid path traversal issues in fuzzer
        base_dir = "./";
    }
    loader.LoadBinaryFromMemory(&model, &err, &warn, memory_data, memory_size, base_dir, tinygltf::REQUIRE_VERSION);

    // Fuzz LoadBinaryFromMemory (Overload 2: explicit size_t size parameter)
    // Note: The signature provided in the prompt suggests an overload with `unsigned int size`
    // The implementation usually maps to the same internal logic, but we call it to cover the signature.
    // We cast Size to unsigned int, handling potential overflow by capping if necessary (though fuzzer inputs are usually safe for size_t)
    unsigned int u_size = (Size > 0xFFFFFFFFU) ? 0xFFFFFFFFU : static_cast<unsigned int>(Size);
    loader.LoadBinaryFromMemory(&model, &err, &warn, memory_data, u_size, base_dir, tinygltf::REQUIRE_VERSION);

    // Fuzz LoadBinaryFromFile
    // Write the input data to a dummy file first
    const std::string filename = "./dummy_file.glb";
    WriteDummyFile(filename, Data, Size);

    loader.LoadBinaryFromFile(&model, &err, &warn, filename, tinygltf::REQUIRE_VERSION);

    // Cleanup is handled by RAII (std::string, tinygltf::Model destructor)
    // No explicit cleanup needed for the model or strings in this context.

    return 0;
}