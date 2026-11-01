// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
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
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

static void WriteDummyFile(const std::string& filename, const uint8_t* data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Test SetPreserveImageChannels with different values
    // Use the first byte to determine the boolean value
    bool preserve_channels = (Data[0] % 2 == 0);
    loader.SetPreserveImageChannels(preserve_channels);

    // Test GetImagesAsIs
    // This checks the state before loading
    bool images_as_is_before = loader.GetImagesAsIs();
    (void)images_as_is_before; // Suppress unused variable warning

    // Prepare file path
    const std::string filename = "./dummy_file";

    // Write the fuzz input to a file to test file loading functions
    WriteDummyFile(filename, Data, Size);

    // Test LoadASCIIFromFile
    // Try with different check_sections values based on input
    unsigned int check_sections_ascii = (Size > 1) ? (Data[1] % 4) : 0;
    loader.LoadASCIIFromFile(&model, &err, &warn, filename, check_sections_ascii);

    // Reset model for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // Test LoadBinaryFromFile
    // Try with different check_sections values
    unsigned int check_sections_binary = (Size > 2) ? (Data[2] % 4) : 0;
    loader.LoadBinaryFromFile(&model, &err, &warn, filename, check_sections_binary);

    // Reset model for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // Test LoadBinaryFromMemory (Overload 1: with base_dir and check_sections)
    // Use a portion of the data as the base_dir if possible, otherwise empty
    std::string base_dir;
    if (Size > 4) {
        // Take a small chunk as base_dir, ensuring it's null-terminated or just a path string
        // For safety, we'll just use a static string or empty if data looks binary
        // Here we just use empty string to avoid path issues with binary data
        base_dir = "";
    }
    unsigned int check_sections_mem1 = (Size > 3) ? (Data[3] % 4) : 0;
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, base_dir, check_sections_mem1);

    // Reset model for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // Test LoadBinaryFromMemory (Overload 2: with default base_dir and check_sections)
    // This overload uses defaults for base_dir and check_sections
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size);

    // Final check of GetImagesAsIs after loading attempts
    bool images_as_is_after = loader.GetImagesAsIs();
    (void)images_as_is_after;

    // Cleanup is handled by RAII for std::string and tinygltf::Model
    // No explicit cleanup needed for these types

    return 0;
}