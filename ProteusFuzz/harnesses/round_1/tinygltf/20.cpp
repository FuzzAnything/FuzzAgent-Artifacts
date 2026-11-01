// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

// Helper to create a dummy JSON file from fuzzing input
static void WriteDummyFile(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Load JSON from fuzzing input
    // We use a dummy file approach as requested, or direct string parsing if the library allows.
    // Since the target functions take a `detail::json` object, we need to parse the input first.
    // tinygltf usually uses RapidJSON or nlohmann/json. We will attempt to parse the input
    // as a JSON string using the library's internal parser if possible, or via a Model loader
    // to trigger the internal json parsing logic.
    
    // However, the target functions are static and operate on `detail::json`.
    // We cannot directly construct `detail::json` from outside the namespace easily without
    // knowing the exact backend (RapidJSON vs nlohmann).
    // The most robust way to fuzz these specific parsing functions is to trigger them
    // through the main model loading process which eventually calls them.
    
    // Strategy:
    // 1. Write input to a dummy file.
    // 2. Use tinygltf::Model to load the file. The loading process will internally
    //    parse JSON and call the target functions (ParseUnsignedProperty, etc.)
    //    when parsing specific glTF nodes, audio emitters, etc.
    
    WriteDummyFile(Data, Size);

    tinygltf::Model model;
    std::string err;
    std::string warn;
    tinygltf::TinyGLTF ctx;

    // Try to load the dummy file. This will exercise the parsing logic.
    // We set flags to try to trigger various code paths.
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(true);
    
    bool success = ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");
    
    // We don't strictly care if it succeeds or fails, as long as the parsing logic runs.
    // The fuzzer will explore valid and invalid JSON structures.
    
    // Additional exploration: Try loading as binary if ASCII fails or to trigger different paths
    if (!success) {
        // Attempt binary load just in case the input was binary glTF (glb)
        // Note: The input is raw bytes, so it might be a valid GLB header.
        ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size);
    }

    // Cleanup is handled by RAII (std::string, tinygltf::Model)
    
    return 0;
}