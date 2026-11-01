// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
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

// Helper to safely call functions that might be in a different namespace or scope
// depending on the build configuration of tinygltf.
// Since the target functions are static and inside the tinygltf namespace (or detail),
// and we cannot directly access private details without the correct backend headers,
// we will attempt to use the public API to trigger the logic indirectly if possible,
// or we will try to access the functions if they are exposed in the current build.

// However, the error indicates that 'detail' is not visible.
// This usually means the internal namespace is hidden or the backend is not configured.
// But the task requires fuzzing these specific functions.
// If the functions are static and internal, they might not be accessible from outside.
// Let's check if tinygltf exposes a way to parse JSON directly or if we can use the Model loader.
// The Model loader uses these functions internally.
// We can try to construct a minimal glTF JSON string and load it.
// This will exercise the parsing logic internally.

extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write the input to a dummy file to simulate a glTF file
    const char* dummy_file = "./dummy_file";
    FILE* f = fopen(dummy_file, "wb");
    if (!f) return 0;
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Use the public API to load the file. This will internally call the target functions
    // if the JSON structure matches what they expect (e.g., lights, cameras).
    tinygltf::TinyGLTF loader;
    std::string load_err;
    std::string load_warn;
    tinygltf::Model model;

    // Try to load as ASCII
    bool ret = loader.LoadASCIIFromFile(&model, &load_err, &load_warn, dummy_file);
    (void)ret; // Ignore return, we just want to exercise the code path

    // Also try to load as binary if the first attempt fails or to cover more paths
    // We reuse the same file, assuming it might be binary or ASCII
    ret = loader.LoadBinaryFromMemory(&model, &load_err, &load_warn, Data, Size);
    (void)ret;

    // If the library is built with specific backends, we might be able to access the internal functions.
    // However, since the compiler error says 'detail' is not found, we cannot directly call them.
    // The best approach is to rely on the public API to trigger the internal logic.
    // The Load functions will parse the JSON and call ParseLight, ParseCamera, etc. if the JSON contains them.
    // The fuzzer will generate various JSON structures, some of which will trigger these parsers.

    // Additionally, we can try to create a minimal valid glTF JSON with specific extensions
    // to ensure the parsers are called. But since the input is arbitrary, we rely on the fuzzer.
    
    // If the user expects direct calls, and the functions are not accessible,
    // we might need to check if there's a way to expose them or if the build flags are different.
    // But based on the error, we cannot access them directly.
    // So we stick to the file loading approach.

    return 0;
}