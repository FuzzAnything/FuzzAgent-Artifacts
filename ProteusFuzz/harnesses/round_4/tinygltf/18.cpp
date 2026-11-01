// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetMaxExternalFileSize at tiny_gltf.h:1551:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
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

// Custom URI callbacks for fuzzing
// The signatures must match tinygltf's expected function pointer types.
// Based on tiny_gltf.h, URICallbacks expects:
// typedef std::function<bool(const std::string&, const std::string&, std::string*, void*)> URICallbacks::encode;
// typedef std::function<bool(const std::string&, std::string*, void*)> URICallbacks::decode;
// Wait, looking at the error message, the types are:
// encode: function<bool (const string &, const string &, string *, void *)>
// decode: function<bool (const string &, string *, void *)>
// Let's define them to match these signatures exactly.

static bool FuzzURIEncode(const std::string &in, const std::string &out, std::string *result, void *user_data) {
    (void)user_data;
    // Simple passthrough: copy input to output
    if (result) {
        *result = in;
    }
    return true;
}

static bool FuzzURIDecode(const std::string &in, std::string *result, void *user_data) {
    (void)user_data;
    // Simple passthrough
    if (result) {
        *result = in;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // 1. Test SetImagesAsIs
    // Toggle based on first byte
    ctx.SetImagesAsIs(Data[0] % 2 != 0);

    // 2. Test SetMaxExternalFileSize
    // Use next 4 bytes as size, ensure it's not 0 to avoid trivial cases, 
    // but allow small sizes to test limits.
    size_t max_size = 0;
    if (Size >= 5) {
        max_size = static_cast<size_t>(Data[1]) | 
                   (static_cast<size_t>(Data[2]) << 8) | 
                   (static_cast<size_t>(Data[3]) << 16) | 
                   (static_cast<size_t>(Data[4]) << 24);
        // Prevent 0 which might be a valid default but we want to test setting
        if (max_size == 0) max_size = 1; 
    } else {
        max_size = 1024;
    }
    ctx.SetMaxExternalFileSize(max_size);

    // 3. Test SetURICallbacks
    tinygltf::URICallbacks callbacks;
    callbacks.encode = FuzzURIEncode;
    callbacks.decode = FuzzURIDecode;
    callbacks.user_data = nullptr;
    // Try to set callbacks
    ctx.SetURICallbacks(callbacks, &err);

    // 4. Test LoadBinaryFromMemory
    // Use the entire input as binary data
    ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", tinygltf::REQUIRE_VERSION);
    // Reset model by reassigning a default constructed model
    model = tinygltf::Model();

    // 5. Test LoadASCIIFromString
    // Interpret input as ASCII (char*)
    // We cast Data to const char*. Note: Data might contain null bytes, 
    // but LoadASCIIFromString takes length, so it should be safe.
    // However, if the data is not valid JSON, it should fail gracefully.
    const char *ascii_str = reinterpret_cast<const char*>(Data);
    ctx.LoadASCIIFromString(&model, &err, &warn, ascii_str, Size, "", tinygltf::REQUIRE_VERSION);
    // Reset model
    model = tinygltf::Model();

    // 6. Test LoadBinaryFromFile
    // Write the input data to a dummy file and try to load it as a binary glTF
    const std::string dummy_file = "./dummy_file.glb";
    {
        std::ofstream ofs(dummy_file, std::ios::binary);
        if (ofs) {
            ofs.write(reinterpret_cast<const char*>(Data), Size);
            ofs.close();
        }
    }
    
    // Attempt to load the file
    ctx.LoadBinaryFromFile(&model, &err, &warn, dummy_file, tinygltf::REQUIRE_VERSION);
    
    // Cleanup: Remove the dummy file
    std::remove(dummy_file.c_str());

    return 0;
}