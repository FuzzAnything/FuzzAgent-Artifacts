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
#include <fstream>
#include <string>
#include <vector>

static void WriteDummyFile(const std::string& filename, const uint8_t* data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Step 1: Prepare file for LoadBinaryFromFile
    const std::string binary_filename = "./dummy_file.glb";
    WriteDummyFile(binary_filename, Data, Size);

    // Invoke LoadBinaryFromFile
    loader.LoadBinaryFromFile(&model, &err, &warn, binary_filename, tinygltf::REQUIRE_VERSION);

    // Step 2: Prepare file for LoadASCIIFromFile (reuse data, assuming it might be ASCII or binary)
    const std::string ascii_filename = "./dummy_file.gltf";
    WriteDummyFile(ascii_filename, Data, Size);

    // Invoke LoadASCIIFromFile
    loader.LoadASCIIFromFile(&model, &err, &warn, ascii_filename, tinygltf::REQUIRE_VERSION);

    // Step 3 & 4: Invoke Accessor::ByteStride diversely
    // We need an Accessor and a BufferView to call ByteStride.
    // Since the model might be empty or invalid, we create local instances to ensure coverage.
    
    tinygltf::Accessor accessor;
    tinygltf::BufferView bufferView;

    // Default call
    accessor.ByteStride(bufferView);

    // Vary bufferView properties to explore different states
    bufferView.byteOffset = 10;
    accessor.ByteStride(bufferView);

    bufferView.byteOffset = 0;
    accessor.ByteStride(bufferView);

    // Try with different byteOffset values from the input data if possible
    if (Size >= 4) {
        uint32_t offset_val = *reinterpret_cast<const uint32_t*>(Data);
        bufferView.byteOffset = offset_val;
        accessor.ByteStride(bufferView);
    }

    // Cleanup is minimal as objects are stack-allocated and destructors handle memory.
    // No file cleanup required for dummy files in fuzzing context typically, 
    // but in a real scenario one might remove them. Here we leave them for simplicity.

    return 0;
}