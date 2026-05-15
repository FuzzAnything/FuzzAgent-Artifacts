// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.cc:958:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.cc:958:16 in tiny_gltf.h
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
#include <fstream>
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void removeDummyFile() {
    std::remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* Data, size_t Size) {
    // Initialize TinyGLTF object
    tinygltf::TinyGLTF loader;
    
    // Test SetImagesAsIs with different values
    if (Size > 0) {
        loader.SetImagesAsIs(Data[0] & 1);
    }
    
    // Test GetPreserveImageChannels
    bool preserveChannels = loader.GetPreserveImageChannels();
    (void)preserveChannels; // Avoid unused variable warning
    
    // Test RemoveImageLoader
    loader.RemoveImageLoader();
    
    // Test LoadASCIIFromString
    {
        tinygltf::Model model;
        std::string err;
        std::string warn;
        
        // Use the input data as the ASCII string
        if (Size > 0) {
            const char* str = reinterpret_cast<const char*>(Data);
            unsigned int length = static_cast<unsigned int>(Size);
            if (length > Size) length = static_cast<unsigned int>(Size);
            
            loader.LoadASCIIFromString(&model, &err, &warn, str, length, ".");
        }
    }
    
    // Test LoadASCIIFromFile
    {
        tinygltf::Model model;
        std::string err;
        std::string warn;
        
        // Write input data to file and try to load it
        writeDummyFile(Data, Size);
        loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
        removeDummyFile();
    }
    
    // Test LoadBinaryFromMemory
    {
        tinygltf::Model model;
        std::string err;
        std::string warn;
        
        if (Size > 0) {
            loader.LoadBinaryFromMemory(&model, &err, &warn, Data, 
                                        static_cast<unsigned int>(Size), ".");
        }
    }
    
    // Test SetImagesAsIs again with opposite value
    if (Size > 0) {
        loader.SetImagesAsIs(!(Data[0] & 1));
    }
    
    // Test GetPreserveImageChannels again
    preserveChannels = loader.GetPreserveImageChannels();
    (void)preserveChannels;
    
    // Test RemoveImageLoader again
    loader.RemoveImageLoader();
    
    return 0;
}