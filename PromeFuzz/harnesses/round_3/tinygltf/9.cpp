// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>

#include "tiny_gltf.h"

static void fuzzParameterFunctions(const uint8_t* data, size_t size) {
    tinygltf::Parameter param;
    
    // Initialize number_array with at least 3 elements as required by ColorFactor
    param.number_array = {1.0, 1.0, 1.0, 1.0};
    
    // Use fuzz data to modify number_array if we have enough data
    if (size >= sizeof(double) * 4) {
        // Override number_array with fuzz data
        double values[4];
        memcpy(values, data, sizeof(double) * 4);
        param.number_array = {values[0], values[1], values[2], values[3]};
    } else if (size >= sizeof(double) * 3) {
        // At least 3 elements for ColorFactor
        double values[3];
        memcpy(values, data, sizeof(double) * 3);
        param.number_array = {values[0], values[1], values[2], 1.0};
    }
    
    // Call all target API functions
    (void)param.ColorFactor();
    (void)param.TextureScale();
    (void)param.TextureIndex();
    (void)param.TextureTexCoord();
    (void)param.TextureStrength();
    (void)param.Factor();
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    fuzzParameterFunctions(Data, Size);
    return 0;
}