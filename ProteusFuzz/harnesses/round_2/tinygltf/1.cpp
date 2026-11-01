// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::WriteGltfSceneToFile at tiny_gltf.h:1460:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
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
#include <sstream>

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Prepare dummy file content
    std::string dummy_file = "./dummy_file";
    WriteDummyFile(dummy_file, Data, Size);

    // 1. SetImagesAsIs
    loader.SetImagesAsIs(true);

    // 2. LoadASCIIFromFile
    loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);

    // 3. WriteGltfSceneToFile (ASCII)
    std::string out_ascii = "./out_ascii.gltf";
    loader.WriteGltfSceneToFile(&model, out_ascii, false, false, true, false);

    // 4. WriteGltfSceneToFile (Binary)
    std::string out_binary = "./out_binary.glb";
    loader.WriteGltfSceneToFile(&model, out_binary, false, false, false, true);

    // Reset model for next load
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // 5. SetImagesAsIs (toggle)
    loader.SetImagesAsIs(false);

    // 6. LoadASCIIFromFile (again)
    loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);

    // 7. WriteGltfSceneToFile (embedded)
    std::string out_embedded = "./out_embedded.gltf";
    loader.WriteGltfSceneToFile(&model, out_embedded, true, true, true, false);

    // Reset model for final load
    model = tinygltf::Model();
    err.clear();
    warn.clear();

    // 8. SetImagesAsIs
    loader.SetImagesAsIs(true);

    // 9. LoadBinaryFromFile
    loader.LoadBinaryFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);

    return 0;
}