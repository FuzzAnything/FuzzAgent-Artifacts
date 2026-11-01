// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
// tinygltf::Accessor::ByteStride at tiny_gltf.h:862:7 in tiny_gltf.h
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
#include <fstream>
#include <string>

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char *>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Prepare dummy file for binary loading
    WriteDummyFile("./dummy_file.glb", Data, Size);
    loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file.glb", tinygltf::REQUIRE_VERSION);

    // Prepare dummy file for ASCII loading (reuse data, though it might be invalid JSON)
    WriteDummyFile("./dummy_file.gltf", Data, Size);
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file.gltf", tinygltf::REQUIRE_VERSION);

    // Accessor::ByteStride exploration
    // We need an Accessor and a BufferView to call ByteStride
    tinygltf::Accessor accessor;
    tinygltf::BufferView bufferView;

    // Initialize bufferView with some fuzzed data if possible, or default
    // Since we can't easily extract a valid BufferView from the loaded model without complex parsing,
    // we will create a synthetic one or use the default constructed one to test edge cases.
    
    // Test 1: Default constructed objects
    int stride1 = accessor.ByteStride(bufferView);
    (void)stride1; // Suppress unused variable warning

    // Test 2: Modify bufferView properties to explore different states
    // byteStride is a key parameter for ByteStride calculation
    bufferView.byteStride = 0;
    int stride2 = accessor.ByteStride(bufferView);
    (void)stride2;

    // Try to set a non-zero byteStride
    if (Size > 0) {
        bufferView.byteStride = Data[0];
        int stride3 = accessor.ByteStride(bufferView);
        (void)stride3;
    }

    // Test 3: Attempt to use accessors from the loaded model if any exist
    // This explores the path where the model was successfully parsed
    if (!model.accessors.empty()) {
        for (const auto &acc : model.accessors) {
            if (acc.bufferView >= 0 && static_cast<size_t>(acc.bufferView) < model.bufferViews.size()) {
                const tinygltf::BufferView &bv = model.bufferViews[acc.bufferView];
                int stride4 = acc.ByteStride(bv);
                (void)stride4;
            }
        }
    }

    // Test 4: Second call to ByteStride as per requirement sequence
    // Re-using the first accessor or a new one
    if (!model.accessors.empty()) {
        const tinygltf::Accessor &acc = model.accessors[0];
        if (acc.bufferView >= 0 && static_cast<size_t>(acc.bufferView) < model.bufferViews.size()) {
            const tinygltf::BufferView &bv = model.bufferViews[acc.bufferView];
            int stride5 = acc.ByteStride(bv);
            (void)stride5;
        }
    } else {
        // Fallback if no accessors loaded
        int stride6 = accessor.ByteStride(bufferView);
        (void)stride6;
    }

    return 0;
}