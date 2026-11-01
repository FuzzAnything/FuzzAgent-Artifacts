// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
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
#include <fstream>

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Target 2: tinygltf::swap4
    // The function is static and defined inside the header. 
    // Since it's static, it's not visible as tinygltf::swap4 in the namespace scope from outside.
    // However, we can define a local wrapper or call it if we can access the internal logic.
    // Given the error, we cannot call tinygltf::swap4 directly.
    // We will skip direct calling of swap4 as it's an internal utility not exposed for external use.
    // Instead, we rely on the other functions which might internally use swap4.
    
    // Prepare common objects for other targets
    tinygltf::Model model;
    std::string err;
    std::string warn;
    tinygltf::TinyGLTF loader;
    const std::string dummy_filename = "./dummy_file";

    // Write input data to a dummy file for file-based loading tests
    WriteDummyFile(dummy_filename, Data, Size);

    // Target 1: LoadBinaryFromFile (Overload 1 with explicit check_sections)
    loader.LoadBinaryFromFile(&model, &err, &warn, dummy_filename, tinygltf::REQUIRE_VERSION);
    
    // Reset model for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // Target 1: LoadBinaryFromFile (Overload 2 with default check_sections)
    loader.LoadBinaryFromFile(&model, &err, &warn, dummy_filename);

    // Reset model for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // Target 3: LoadBinaryFromMemory (Overload 1 with explicit base_dir and check_sections)
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", tinygltf::REQUIRE_VERSION);

    // Reset model for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // Target 3: LoadBinaryFromMemory (Overload 2 with default base_dir and check_sections)
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size);

    // Reset model for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // Target 4: LoadASCIIFromFile
    loader.LoadASCIIFromFile(&model, &err, &warn, dummy_filename, tinygltf::REQUIRE_VERSION);

    // Cleanup is handled by RAII (std::string, tinygltf::Model destructor)
    // No explicit cleanup needed for local variables.

    return 0;
}