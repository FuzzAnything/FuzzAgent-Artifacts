// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
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

static void writeDummyFile(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(Data, 1, Size, f);
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    writeDummyFile(Data, Size);
    
    // Initialize TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Prepare model and error/warning strings
    tinygltf::Model model1, model2;
    std::string err, warn;
    
    // Call LoadBinaryFromFile
    loader.LoadBinaryFromFile(&model1, &err, &warn, "./dummy_file");
    
    // Call LoadASCIIFromFile
    loader.LoadASCIIFromFile(&model2, &err, &warn, "./dummy_file");
    
    // Prepare Accessor and BufferView for ByteStride calls
    tinygltf::Accessor accessor;
    tinygltf::BufferView bufferView;
    
    // First ByteStride call
    accessor.ByteStride(bufferView);
    
    // Second ByteStride call
    accessor.ByteStride(bufferView);
    
    return 0;
}