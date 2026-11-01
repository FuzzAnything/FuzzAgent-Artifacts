// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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
#include <fstream>
#include <stdexcept>

// Include the target library
#include "tiny_gltf.h"

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string dummy_path = "./dummy_file";
    std::ofstream out(dummy_path, std::ios::binary);
    if (!out.is_open()) {
        return 0;
    }
    out.write(reinterpret_cast<const char*>(Data), Size);
    out.close();

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string load_err;
    std::string load_warn;

    // Try loading as ASCII (JSON)
    // This will trigger internal JSON parsing and the target functions
    bool loaded_ascii = loader.LoadASCIIFromFile(&model, &load_err, &load_warn, dummy_path);
    
    // Reset model by reassigning a new empty model
    model = tinygltf::Model();

    // Try loading as Binary (glb)
    // This also triggers parsing logic
    bool loaded_binary = loader.LoadBinaryFromFile(&model, &load_err, &load_warn, dummy_path);

    // Reset model again
    model = tinygltf::Model();

    return 0;
}