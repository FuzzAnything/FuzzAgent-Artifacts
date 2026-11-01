// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::RemoveImageLoader at tiny_gltf.h:1478:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
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

// Static helper to write input data to a dummy file
static void WriteDummyFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

// Static helper to write input data to a dummy JSON file
static void WriteDummyJsonFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file.json", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

// Static helper to write input data to a dummy binary glb file
static void WriteDummyGlbFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file.glb", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

// Helper to reset the model by reassigning a default constructed one
static void ResetModel(tinygltf::Model &model) {
    model = tinygltf::Model();
}

extern "C" int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // 1. Test LoadASCIIFromString with raw input as JSON string
    // We cast data to const char*. Note: this might not be valid JSON, which is good for fuzzing.
    ctx.LoadASCIIFromString(&model, &err, &warn,
                            reinterpret_cast<const char*>(Data),
                            Size,
                            "", // base_dir
                            tinygltf::REQUIRE_VERSION);
    ResetModel(model); // Reset model for next test

    // 2. Test SetFsCallbacks with custom callbacks (using null pointers to test error handling or valid stubs)
    tinygltf::FsCallbacks fs_callbacks;
    // Initialize with nulls to test error path, or we could implement stubs.
    // For fuzzing, passing nulls often triggers validation logic.
    fs_callbacks.FileExists = nullptr;
    fs_callbacks.ExpandFilePath = nullptr;
    fs_callbacks.ReadWholeFile = nullptr;
    fs_callbacks.WriteWholeFile = nullptr;
    fs_callbacks.GetFileSizeInBytes = nullptr;
    fs_callbacks.user_data = nullptr;
    ctx.SetFsCallbacks(fs_callbacks, &err);
    
    // Reset callbacks to default to allow file loading later
    // Since we can't easily reset to default without re-instantiating, 
    // we will just proceed. The next calls might fail if callbacks are null, which is expected.
    
    // 3. Test LoadBinaryFromFile
    // Write data to dummy file and try to load it as binary GLB
    WriteDummyGlbFile(Data, Size);
    ctx.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file.glb", tinygltf::REQUIRE_VERSION);
    ResetModel(model);

    // 4. Test LoadASCIIFromFile
    // Write data to dummy JSON file and try to load it as ASCII JSON
    WriteDummyJsonFile(Data, Size);
    ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file.json", tinygltf::REQUIRE_VERSION);
    ResetModel(model);

    // 5. Test SetURICallbacks
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = nullptr;
    uri_callbacks.decode = nullptr;
    uri_callbacks.user_data = nullptr;
    ctx.SetURICallbacks(uri_callbacks, &err);

    // 6. Test RemoveImageLoader
    ctx.RemoveImageLoader();

    // Additional exploration: Try with different check_sections flags
    // REQUIRE_VERSION (default), REQUIRE_VERSION_AND_CHECK_SCHEMA (if available), or 0
    // We'll try 0 (no check) if the enum allows, or just stick to standard constants.
    // Assuming REQUIRE_VERSION is 1, let's try 0 if it's a valid int.
    ctx.LoadASCIIFromString(&model, &err, &warn,
                            reinterpret_cast<const char*>(Data),
                            Size,
                            "",
                            0); // No checks
    ResetModel(model);

    // Try loading with a very short string to test boundary conditions
    if (Size > 0) {
        ctx.LoadASCIIFromString(&model, &err, &warn,
                                reinterpret_cast<const char*>(Data),
                                1, // Only first byte
                                "",
                                tinygltf::REQUIRE_VERSION);
        ResetModel(model);
    }

    return 0;
}