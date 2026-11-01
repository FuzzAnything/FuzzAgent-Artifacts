// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
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
#include <fstream>
#include "tiny_gltf.h"

static void WriteDummyFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

static void local_swap4(unsigned int *val) {
    #ifdef TINYGLTF_LITTLE_ENDIAN
        // Do nothing on little-endian
        (void)val;
    #else
        // Swap bytes on big-endian
        unsigned int v = *val;
        *val = ((v & 0x000000FF) << 24) |
               ((v & 0x0000FF00) << 8) |
               ((v & 0x00FF0000) >> 8) |
               ((v & 0xFF000000) >> 24);
    #endif
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Prepare data for memory-based loading
    const unsigned char *bytes = Data;
    unsigned int length = static_cast<unsigned int>(Size);
    const std::string base_dir = "";

    // 1. LoadBinaryFromMemory (Overload 1: with check_sections)
    ctx.LoadBinaryFromMemory(&model, &err, &warn, bytes, length, base_dir, 
                             tinygltf::REQUIRE_VERSION);
    ctx.LoadBinaryFromMemory(&model, &err, &warn, bytes, length, base_dir, 
                             tinygltf::REQUIRE_VERSION);
    ctx.LoadBinaryFromMemory(&model, &err, &warn, bytes, length, base_dir, 
                             tinygltf::REQUIRE_VERSION);
    ctx.LoadBinaryFromMemory(&model, &err, &warn, bytes, length, base_dir, 
                             tinygltf::REQUIRE_VERSION);
    ctx.LoadBinaryFromMemory(&model, &err, &warn, bytes, length, base_dir, 
                             tinygltf::REQUIRE_VERSION);

    // 2. LoadBinaryFromMemory (Overload 2: default check_sections)
    ctx.LoadBinaryFromMemory(&model, &err, &warn, bytes, length, base_dir);

    // 3. Write to dummy file for file-based loading
    WriteDummyFile(Data, Size);

    // 4. LoadBinaryFromFile (Overload 1: with check_sections)
    ctx.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file", 
                           tinygltf::REQUIRE_VERSION);
    ctx.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file", 
                           tinygltf::REQUIRE_VERSION);
    ctx.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file", 
                           tinygltf::REQUIRE_VERSION);

    // 5. LoadBinaryFromFile (Overload 2: default check_sections)
    ctx.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file");

    // 6. LoadASCIIFromFile (Overload 1: with check_sections)
    ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", 
                          tinygltf::REQUIRE_VERSION);
    ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", 
                          tinygltf::REQUIRE_VERSION);

    // 7. LoadASCIIFromFile (Overload 2: default check_sections)
    ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");

    // 8. swap4
    unsigned int val = 0x12345678;
    local_swap4(&val);
    local_swap4(&val);
    local_swap4(&val);

    // Cleanup: Remove dummy file if it exists
    std::remove("./dummy_file");

    return 0;
}