// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.cc:958:16 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
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
#include <fstream>
#include <string>

static void WriteDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void RemoveDummyFile() {
    remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Use first byte to decide which operation to perform
    uint8_t operation = Data[0] % 6;
    const uint8_t* fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    switch (operation) {
        case 0: {
            // LoadASCIIFromFile
            WriteDummyFile(fuzz_data, fuzz_size);
            loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
            RemoveDummyFile();
            break;
        }
        case 1: {
            // LoadBinaryFromMemory
            if (fuzz_size > 20) {
                loader.LoadBinaryFromMemory(&model, &err, &warn, fuzz_data, fuzz_size);
            }
            break;
        }
        case 2: {
            // RemoveImageLoader
            loader.RemoveImageLoader();
            break;
        }
        case 3: {
            // GetPreserveImageChannels
            bool preserve = loader.GetPreserveImageChannels();
            (void)preserve; // Avoid unused variable warning
            break;
        }
        case 4: {
            // LoadASCIIFromString
            if (fuzz_size > 0) {
                loader.LoadASCIIFromString(&model, &err, &warn, 
                    reinterpret_cast<const char*>(fuzz_data), fuzz_size, "");
            }
            break;
        }
        case 5: {
            // SetImagesAsIs
            bool onoff = fuzz_size % 2 == 0;
            loader.SetImagesAsIs(onoff);
            break;
        }
    }

    // Clean up any remaining dummy file
    RemoveDummyFile();
    return 0;
}