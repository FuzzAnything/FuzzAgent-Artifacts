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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename.c_str(), "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
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

    // 1. SetImagesAsIs
    loader.SetImagesAsIs(true);

    // Write data to dummy file for ASCII load
    WriteDummyFile("./dummy_file", Data, Size);

    // 2. LoadASCIIFromFile
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", tinygltf::REQUIRE_VERSION);

    // 3. WriteGltfSceneToFile (ASCII, no embed)
    loader.WriteGltfSceneToFile(&model, "./output1.gltf", false, false, true, false);

    // 4. WriteGltfSceneToFile (Binary, embed)
    loader.WriteGltfSceneToFile(&model, "./output2.glb", true, true, false, true);

    // 5. SetImagesAsIs (toggle)
    loader.SetImagesAsIs(false);

    // 6. LoadASCIIFromFile again
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file", tinygltf::REQUIRE_VERSION);

    // 7. WriteGltfSceneToFile again
    loader.WriteGltfSceneToFile(&model, "./output3.gltf", false, true, false, false);

    // 8. SetImagesAsIs (toggle back)
    loader.SetImagesAsIs(true);

    // 9. LoadBinaryFromFile
    // Reuse the dummy file as a potential binary blob
    loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file", tinygltf::REQUIRE_VERSION);

    return 0;
}