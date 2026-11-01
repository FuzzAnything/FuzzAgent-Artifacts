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

static void WriteDummyFile(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // 1. SetImagesAsIs
    ctx.SetImagesAsIs(true);

    // Prepare dummy file for ASCII load
    const char *ascii_file = "./dummy_file_ascii.gltf";
    WriteDummyFile(ascii_file, Data, Size);

    // 2. LoadASCIIFromFile
    ctx.LoadASCIIFromFile(&model, &err, &warn, ascii_file, tinygltf::REQUIRE_VERSION);

    // 3. WriteGltfSceneToFile (ASCII)
    const char *out_file_1 = "./output_1.gltf";
    ctx.WriteGltfSceneToFile(&model, out_file_1, false, false, true, false);

    // 4. WriteGltfSceneToFile (Binary)
    const char *out_file_2 = "./output_2.glb";
    ctx.WriteGltfSceneToFile(&model, out_file_2, false, false, false, true);

    // 5. SetImagesAsIs (toggle)
    ctx.SetImagesAsIs(false);

    // Prepare dummy file for ASCII load (reusing data)
    const char *ascii_file_2 = "./dummy_file_ascii2.gltf";
    WriteDummyFile(ascii_file_2, Data, Size);

    // 6. LoadASCIIFromFile
    ctx.LoadASCIIFromFile(&model, &err, &warn, ascii_file_2, tinygltf::REQUIRE_VERSION);

    // 7. WriteGltfSceneToFile (ASCII with embedding)
    const char *out_file_3 = "./output_3.gltf";
    ctx.WriteGltfSceneToFile(&model, out_file_3, true, true, true, false);

    // 8. SetImagesAsIs (toggle back)
    ctx.SetImagesAsIs(true);

    // Prepare dummy file for binary load
    const char *binary_file = "./dummy_file_binary.glb";
    WriteDummyFile(binary_file, Data, Size);

    // 9. LoadBinaryFromFile
    ctx.LoadBinaryFromFile(&model, &err, &warn, binary_file, tinygltf::REQUIRE_VERSION);

    // Cleanup: Remove dummy files
    std::remove(ascii_file);
    std::remove(ascii_file_2);
    std::remove(out_file_1);
    std::remove(out_file_2);
    std::remove(out_file_3);
    std::remove(binary_file);

    return 0;
}