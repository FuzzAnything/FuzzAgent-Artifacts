// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
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

// Helper to generate a random double from input data
static double GetRandomDouble(const uint8_t *Data, size_t Size, size_t &offset) {
    if (offset + 8 > Size) {
        offset = 0;
    }
    if (offset + 8 > Size) return 0.0; 
    uint64_t val = 0;
    std::memcpy(&val, Data + offset, 8);
    offset += 8;
    double d = 0.0;
    std::memcpy(&d, &val, sizeof(double));
    return d;
}

// Helper to generate a random int from input data
static int GetRandomInt(const uint8_t *Data, size_t Size, size_t &offset) {
    if (offset + 4 > Size) {
        offset = 0;
    }
    if (offset + 4 > Size) return 0; 
    int32_t val = 0;
    std::memcpy(&val, Data + offset, 4);
    offset += 4;
    return static_cast<int>(val);
}

// Helper to generate a random bool
static bool GetRandomBool(const uint8_t *Data, size_t Size, size_t &offset) {
    if (offset >= Size) offset = 0;
    if (offset >= Size) return false; 
    return (Data[offset++] & 0x01) != 0;
}

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::Parameter param;
    size_t offset = 0;

    // Test 1: TextureIndex
    int idx = param.TextureIndex();
    (void)idx;

    // Test 2: Factor
    param.number_value = GetRandomDouble(Data, Size, offset);
    double f = param.Factor();
    (void)f;

    // Test 3: TextureStrength
    double s = param.TextureStrength();
    (void)s;

    // Test 4: ColorFactor
    // The library implementation accesses number_array[0], [1], [2] directly.
    // We must ensure the vector has at least 3 elements to avoid out-of-bounds access.
    param.number_array.clear();
    // Always push 3 default values to satisfy the library's direct indexing
    param.number_array.push_back(0.0);
    param.number_array.push_back(0.0);
    param.number_array.push_back(0.0);
    
    // Optionally add a 4th component if data allows
    if (offset + 8 <= Size) {
        param.number_array.push_back(GetRandomDouble(Data, Size, offset));
    } else {
        param.number_array.push_back(1.0); // Default alpha
    }
    
    tinygltf::ColorValue c = param.ColorFactor();
    (void)c;

    // Test 5: TextureScale
    double sc = param.TextureScale();
    (void)sc;

    // Test 6: TextureTexCoord
    int tc = param.TextureTexCoord();
    (void)tc;

    return 0;
}