// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
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
#include "tiny_gltf.h"
#include <fstream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char* dummy_file = "./dummy_file";
    std::ofstream out(dummy_file, std::ios::binary);
    if (out) {
        out.write(reinterpret_cast<const char*>(Data), Size);
        out.close();
    }

    std::string json_str(reinterpret_cast<const char*>(Data), Size);
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    std::string base_dir = "";

    // LoadASCIIFromString requires base_dir and check_sections arguments
    ctx.LoadASCIIFromString(&model, &err, &warn, json_str.c_str(), Size, base_dir, 0);

    // LoadASCIIFromFile
    ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_file);

    // LoadBinaryFromFile
    ctx.LoadBinaryFromFile(&model, &err, &warn, dummy_file);

    return 0;
}

#pragma GCC diagnostic pop