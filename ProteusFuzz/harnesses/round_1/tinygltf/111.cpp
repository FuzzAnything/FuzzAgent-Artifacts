// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
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
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char *>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Fuzz SetStoreOriginalJSONForExtrasAndExtensions and GetStoreOriginalJSONForExtrasAndExtensions
    bool enable_original_json = (Data[0] % 2) != 0;
    loader.SetStoreOriginalJSONForExtrasAndExtensions(enable_original_json);
    bool current_state = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)current_state; // Use to avoid unused variable warning

    // Fuzz GetImagesAsIs (requires SetImagesAsIs to change state, but we can check default)
    bool images_as_is = loader.GetImagesAsIs();
    (void)images_as_is;

    // Prepare dummy file content
    const std::string dummy_filename = "./dummy_file";
    WriteDummyFile(dummy_filename, Data, Size);

    // Fuzz LoadBinaryFromFile (.glb)
    // We use the raw data as a potential binary glb file
    loader.LoadBinaryFromFile(&model, &err, &warn, dummy_filename, 0);
    loader.LoadBinaryFromFile(&model, &err, &warn, dummy_filename, 1);
    loader.LoadBinaryFromFile(&model, &err, &warn, dummy_filename, 2);
    loader.LoadBinaryFromFile(&model, &err, &warn, dummy_filename, 3);

    // Fuzz LoadASCIIFromFile (.gltf)
    // We use the raw data as a potential ASCII gltf file
    loader.LoadASCIIFromFile(&model, &err, &warn, dummy_filename, 0);
    loader.LoadASCIIFromFile(&model, &err, &warn, dummy_filename, 1);
    loader.LoadASCIIFromFile(&model, &err, &warn, dummy_filename, 2);
    loader.LoadASCIIFromFile(&model, &err, &warn, dummy_filename, 3);

    // Fuzz LoadBinaryFromMemory
    // We use the raw data directly as memory buffer
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 0);
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 1);
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 2);
    loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 3);
    
    // Try with a base_dir if Size allows
    if (Size > 10) {
        std::string base_dir(reinterpret_cast<const char*>(Data + Size - 10), 10);
        loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, base_dir, 0);
    }

    // Reset model for potential next iteration (though not strictly needed as it's re-declared or cleared by loader logic usually)
    model = tinygltf::Model();

    return 0;
}