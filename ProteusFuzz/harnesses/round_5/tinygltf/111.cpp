// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteWholeFile at tiny_gltf.h:1347:6 in tiny_gltf.h
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
#include "tiny_gltf.h"

// Helper to generate a random string from fuzz input
static std::string GenerateString(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "";
    std::string result(reinterpret_cast<const char*>(Data), Size);
    for (char &c : result) {
        if (c == '\0') c = ' ';
    }
    return result;
}

// Helper to generate a random buffer from fuzz input
static std::vector<unsigned char> GenerateBuffer(const uint8_t *Data, size_t Size) {
    if (Size == 0) return std::vector<unsigned char>();
    return std::vector<unsigned char>(Data, Data + Size);
}

// Helper to create a tinygltf::Buffer from fuzz input
static tinygltf::Buffer CreateBuffer(const uint8_t *Data, size_t Size) {
    tinygltf::Buffer buffer;
    buffer.data = GenerateBuffer(Data, Size);
    buffer.name = GenerateString(Data, Size);
    buffer.extras = tinygltf::Value(42);
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    size_t chunkSize = Size / 6;
    if (chunkSize == 0) chunkSize = 1;

    // --- Test 2: WriteWholeFile ---
    // This function is typically exposed in the tinygltf namespace.
    {
        std::string err;
        std::string filepath = "./dummy_file_whole";
        
        // Ensure we don't read past the buffer
        size_t offset = 3 * chunkSize;
        size_t len = chunkSize;
        
        if (offset + len > Size) {
            offset = 0;
            len = Size;
        }
        
        std::vector<unsigned char> contents = GenerateBuffer(Data + offset, len);
        
        tinygltf::WriteWholeFile(&err, filepath, contents, nullptr);
        std::remove(filepath.c_str());
    }

    return 0;
}