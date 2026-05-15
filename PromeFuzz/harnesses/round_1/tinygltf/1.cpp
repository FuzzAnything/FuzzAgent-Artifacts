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
#include <cstring>
#include <fstream>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    static const std::string dummy_file = "./dummy_file";
    
    // Write fuzzer input to dummy file
    std::ofstream outfile(dummy_file, std::ios::binary);
    if (!outfile.is_open()) {
        return 0;
    }
    outfile.write(reinterpret_cast<const char*>(Data), Size);
    outfile.close();
    
    // Initialize TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Initialize models for loading
    tinygltf::Model model_binary;
    tinygltf::Model model_ascii;
    
    // Initialize error/warning strings
    std::string err;
    std::string warn;
    
    // Call LoadBinaryFromFile
    bool binary_result = loader.LoadBinaryFromFile(&model_binary, &err, &warn, dummy_file);
    
    // Call LoadASCIIFromFile
    bool ascii_result = loader.LoadASCIIFromFile(&model_ascii, &err, &warn, dummy_file);
    
    // Create default Accessor and BufferView objects for ByteStride calls
    tinygltf::Accessor accessor1;
    tinygltf::Accessor accessor2;
    tinygltf::BufferView bufferView1;
    tinygltf::BufferView bufferView2;
    
    // Initialize buffer views with some data
    bufferView1.buffer = 0;
    bufferView1.byteOffset = 0;
    
    bufferView2.buffer = 1;
    bufferView2.byteOffset = 100;
    
    // First ByteStride call
    int stride1 = accessor1.ByteStride(bufferView1);
    
    // Second ByteStride call
    int stride2 = accessor2.ByteStride(bufferView2);
    
    // Clean up dummy file
    std::remove(dummy_file.c_str());
    
    return 0;
}