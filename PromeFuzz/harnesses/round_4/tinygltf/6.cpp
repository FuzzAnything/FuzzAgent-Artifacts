// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
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
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>

#include "code/tiny_gltf.h"

static void fuzzParameterFunctions(const uint8_t *data, size_t size) {
    tinygltf::Parameter param;
    
    // Initialize number_array with at least 3 values as required by ColorFactor
    param.number_array = {1.0, 1.0, 1.0, 1.0};
    
    // Use fuzzer data to potentially populate number_array with fuzzer values
    if (size > 0) {
        size_t num_elements = std::min(size, static_cast<size_t>(4));
        for (size_t i = 0; i < num_elements && i < param.number_array.size(); ++i) {
            // Convert byte to double in a reasonable range
            param.number_array[i] = static_cast<double>(data[i % size]) / 255.0;
        }
    }
    
    // Call all target API functions
    (void)param.TextureIndex();
    (void)param.ColorFactor();
    (void)param.TextureScale();
    (void)param.TextureStrength();
    (void)param.Factor();
    (void)param.TextureTexCoord();
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *data, size_t size) {
    // Call the function that tests all Parameter API functions
    fuzzParameterFunctions(data, size);
    
    return 0;
}