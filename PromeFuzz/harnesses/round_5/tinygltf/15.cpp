// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
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
#include "tiny_gltf.h"

static void fuzzParameterFunctions(const uint8_t *data, size_t size) {
    // Create a default Parameter object
    tinygltf::Parameter param;
    
    // Initialize number_array with at least 3 values as required for ColorFactor
    // We'll use some values from the fuzzer input if available
    if (size >= 12) {  // At least 3 floats (12 bytes)
        param.number_array.clear();
        for (size_t i = 0; i < 3 && (i * 4) < size; i++) {
            float val = 1.0f;
            if ((i * 4 + 3) < size) {
                // Construct a float from bytes (simple approach)
                uint32_t bytes = 0;
                for (int j = 0; j < 4 && (i * 4 + j) < size; j++) {
                    bytes |= (data[i * 4 + j] << (j * 8));
                }
                val = *reinterpret_cast<float*>(&bytes);
            }
            param.number_array.push_back(val);
        }
    } else {
        // Default values as suggested
        param.number_array = {1.0, 1.0, 1.0, 1.0};
    }
    
    // Call all target functions
    double scale = param.TextureScale();
    int texCoord = param.TextureTexCoord();
    tinygltf::ColorValue color = param.ColorFactor();
    double strength = param.TextureStrength();
    int texIndex = param.TextureIndex();
    double factor = param.Factor();
    
    // Use the results to avoid compiler optimization
    (void)scale;
    (void)texCoord;
    (void)color;
    (void)strength;
    (void)texIndex;
    (void)factor;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Call the parameter functions fuzzer
    fuzzParameterFunctions(Data, Size);
    
    return 0;
}