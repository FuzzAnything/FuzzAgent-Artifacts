// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.cc:958:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
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
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* Data, size_t Size) {
    // Initialize required objects
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Test LoadBinaryFromMemory
    if (Size > 20) {  // Minimum required size for binary glTF header
        loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", tinygltf::REQUIRE_VERSION);
    }
    
    // Test SetImagesAsIs with different states
    loader.SetImagesAsIs(true);
    loader.SetImagesAsIs(false);
    
    // Test LoadASCIIFromFile
    writeDummyFile(Data, Size);
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", tinygltf::REQUIRE_VERSION);
    
    // Test GetPreserveImageChannels
    bool preserve = loader.GetPreserveImageChannels();
    (void)preserve;  // Use variable to avoid unused warning
    
    // Test RemoveImageLoader
    loader.RemoveImageLoader();
    
    // Test LoadASCIIFromString
    if (Size > 0) {
        const char* str = reinterpret_cast<const char*>(Data);
        loader.LoadASCIIFromString(&model, &err, &warn, str, Size, "", tinygltf::REQUIRE_VERSION);
    }
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}