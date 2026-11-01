// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
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

    // Step 1: LoadBinaryFromFile
    // Write input data to a dummy binary file
    WriteDummyFile("./dummy_file.glb", Data, Size);
    loader.LoadBinaryFromFile(&model, &err, &warn, "./dummy_file.glb", tinygltf::REQUIRE_VERSION);

    // Step 2: LoadASCIIFromFile
    // Write input data to a dummy ASCII file (treating binary as text for fuzzing robustness)
    WriteDummyFile("./dummy_file.gltf", Data, Size);
    loader.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file.gltf", tinygltf::REQUIRE_VERSION);

    // Step 3 & 4: Accessor::ByteStride
    // We need to access Accessors from the loaded model.
    // If the model loaded successfully, iterate through accessors.
    // If not, we can still test with default constructed objects to trigger edge cases.
    
    if (!model.accessors.empty()) {
        // Test with actual loaded accessors and buffer views
        for (const auto &accessor : model.accessors) {
            if (accessor.bufferView >= 0 && static_cast<size_t>(accessor.bufferView) < model.bufferViews.size()) {
                const tinygltf::BufferView &bv = model.bufferViews[accessor.bufferView];
                // Invoke ByteStride twice as requested
                int stride1 = accessor.ByteStride(bv);
                int stride2 = accessor.ByteStride(bv);
                (void)stride1;
                (void)stride2;
            }
        }
    }

    // Explore more states: Test with a default constructed BufferView
    // This tests the logic when bufferView is invalid or empty
    tinygltf::Accessor defaultAccessor;
    tinygltf::BufferView defaultBufferView;
    
    // Set some fields to trigger different code paths in ByteStride
    defaultBufferView.byteOffset = 0;
    // ByteStride relies on accessor type and bufferView.byteStride
    // Since we can't easily set the type without a full JSON parse, 
    // we rely on the default state which should be handled gracefully.
    
    int stride3 = defaultAccessor.ByteStride(defaultBufferView);
    int stride4 = defaultAccessor.ByteStride(defaultBufferView);
    (void)stride3;
    (void)stride4;

    return 0;
}