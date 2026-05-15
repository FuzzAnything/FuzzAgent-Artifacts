// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::GetMaxExternalFileSize at tiny_gltf.h:1555:10 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
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
#include <string>
#include <fstream>

static void writeDummyFile(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Initialize objects
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Test GetMaxExternalFileSize
    size_t maxSize = loader.GetMaxExternalFileSize();
    (void)maxSize; // Use variable to avoid unused warning
    
    // Test GetPreserveImageChannels
    bool preserveChannels = loader.GetPreserveImageChannels();
    (void)preserveChannels;
    
    // Test SetMaxExternalFileSize with fuzzed size
    if (Size > 0) {
        size_t newSize = static_cast<size_t>(Data[0]) | (static_cast<size_t>(Data[Size > 1 ? 1 : 0]) << 8);
        loader.SetMaxExternalFileSize(newSize);
    }
    
    // Test LoadBinaryFromMemory if we have enough data
    if (Size > 20) {
        loader.LoadBinaryFromMemory(&model, &err, &warn, Data, static_cast<unsigned int>(Size), "");
        
        // Try with different check_sections values
        if (Size > 40) {
            unsigned int checkSections = static_cast<unsigned int>(Data[0]) % 3;
            loader.LoadBinaryFromMemory(&model, &err, &warn, Data, static_cast<unsigned int>(Size), ".", checkSections);
        }
    }
    
    // Test LoadASCIIFromFile
    writeDummyFile(Data, Size);
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
    
    // Try with different check_sections values
    if (Size > 0) {
        unsigned int checkSections = static_cast<unsigned int>(Data[0]) % 3;
        loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", checkSections);
    }
    
    // Test LoadBinaryFromFile
    writeDummyFile(Data, Size);
    loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file");
    
    // Try with different check_sections values
    if (Size > 0) {
        unsigned int checkSections = static_cast<unsigned int>(Data[0]) % 3;
        loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file", checkSections);
    }
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}