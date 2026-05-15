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
#include <cstdint>
#include <fstream>
#include <string>

static void writeDummyFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(Data), Size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    writeDummyFile(Data, Size);
    
    // Initialize required objects
    tinygltf::TinyGLTF loader;
    tinygltf::Model model_binary;
    tinygltf::Model model_ascii;
    std::string err;
    std::string warn;
    
    // Call LoadBinaryFromFile
    bool result_binary = loader.LoadBinaryFromFile(&model_binary, &err, &warn, "./dummy_file");
    
    // Call LoadASCIIFromFile
    bool result_ascii = loader.LoadASCIIFromFile(&model_ascii, &err, &warn, "./dummy_file");
    
    // Prepare for ByteStride calls
    tinygltf::Accessor accessor;
    tinygltf::BufferView bufferView;
    
    // First ByteStride call
    int stride1 = accessor.ByteStride(bufferView);
    
    // Second ByteStride call
    int stride2 = accessor.ByteStride(bufferView);
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}