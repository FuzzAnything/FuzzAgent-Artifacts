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
#include <fstream>
#include <stdexcept>
#include "tiny_gltf.h"

// Helper to create a valid JSON object from raw bytes if possible, or a default one.
// We use the public API to construct json objects since detail namespace is internal.
static tinygltf::Model CreateModelFromJsonString(const std::string& json_str) {
    tinygltf::Model model;
    std::string err;
    std::string warn;
    tinygltf::TinyGLTF parser;
    // We don't actually load a file, but we can try to parse the string if the API allows
    // However, TinyGLTF primarily loads from files or buffers. 
    // Since we need to test detail functions which operate on internal json types,
    // we must construct the internal json type directly if possible, or use the parser's internal state.
    // The safest way to get a tinygltf::detail::json is via the parser's internal logic or by 
    // constructing it if the type is exposed. 
    // Given the error, tinygltf::detail::json is not directly accessible as a public type in the way assumed.
    // We need to rely on the fact that tinygltf uses nlohmann::json or RapidJSON internally.
    // If TINYGLTF_USE_RAPIDJSON is not defined, it uses nlohmann::json.
    // The functions we are fuzzing are in the detail namespace.
    // We will try to use the parser to load a dummy file containing the input data to trigger the internal json parsing.
    return model;
}

// Since we cannot directly instantiate tinygltf::detail::json from outside due to visibility,
// we will use the TinyGLTF parser to parse the input data into a model, which internally uses the json types.
// Then we can try to access the internal structures if possible, or just trigger the parsing logic.
// However, the target functions (ArrayEnd, ObjectBegin, etc.) are static helpers for iteration.
// To fuzz them effectively, we need to trigger the code paths that use them.
// The most direct way is to load a glTF file (or JSON) that exercises these iteration helpers.
// We will write the input data to a temporary file and try to load it.

static void WriteInputToFile(const uint8_t *Data, size_t Size, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input to a temporary file
    const char* filename = "./dummy_file";
    WriteInputToFile(Data, Size, filename);

    tinygltf::Model model;
    std::string err;
    std::string warn;
    tinygltf::TinyGLTF parser;

    // Try to load the file as a glTF/glb
    // This will internally parse JSON and use the detail functions we want to fuzz
    bool success = parser.LoadASCIIFromFile(&model, &err, &warn, filename);
    
    // If ASCII load fails, try binary (glb)
    if (!success) {
        success = parser.LoadBinaryFromMemory(&model, &err, &warn, Data, Size);
    }

    // If that fails, try loading as pure JSON (some versions support this via LoadFromString or similar, 
    // but standard API is file/memory based). 
    // The LoadASCIIFromFile should handle JSON content if it's a valid glTF JSON.
    // Even if parsing fails, the internal JSON parser (and thus the iteration helpers) might be exercised 
    // during the error handling or partial parsing.

    // We can also try to trigger specific iteration paths by manipulating the model if loaded
    if (success) {
        // Iterate over buffers, meshes, etc. to ensure internal json iteration helpers are used
        // The model structure itself is C++, but the parsing phase used the detail functions.
        // To further stress test, we could try to serialize back, but that's less relevant for these specific helpers.
        (void)model;
    }

    // Cleanup
    std::remove(filename);

    return 0;
}