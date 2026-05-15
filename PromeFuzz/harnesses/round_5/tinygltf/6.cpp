// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
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

static void fuzzParameterFunctions(const uint8_t *Data, size_t Size) {
    tinygltf::Parameter param;
    
    // Initialize number_array with at least 3 values as required for ColorFactor
    // Use fuzzer data to populate if available, otherwise use defaults
    if (Size >= 3 * sizeof(double)) {
        // Try to read 3 doubles from fuzzer input
        double values[3];
        size_t bytes_to_copy = std::min(Size, 3 * sizeof(double));
        memcpy(values, Data, bytes_to_copy);
        
        // Add to number_array
        param.number_array.clear();
        for (size_t i = 0; i < 3; ++i) {
            param.number_array.push_back(values[i % (bytes_to_copy / sizeof(double))]);
        }
    } else {
        // Default values
        param.number_array = {1.0, 1.0, 1.0, 1.0};
    }
    
    // Call all target functions
    double scale = param.TextureScale();
    int texCoord = param.TextureTexCoord();
    
    // Only call ColorFactor if number_array has at least 3 elements
    if (param.number_array.size() >= 3) {
        tinygltf::ColorValue color = param.ColorFactor();
        volatile double dummy_color = color[0] + color[1] + color[2] + color[3];
    }
    
    double strength = param.TextureStrength();
    int texIndex = param.TextureIndex();
    double factor = param.Factor();
    
    // Use the results in some way to avoid optimization
    volatile double dummy = scale + strength + factor;
    volatile int dummy_int = texCoord + texIndex;
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Call the main fuzzing function
    fuzzParameterFunctions(Data, Size);
    
    // Also test with different parameter configurations
    tinygltf::Parameter param2;
    
    // Test with empty number_array - skip ColorFactor
    param2.number_array.clear();
    // Don't call ColorFactor on empty array
    param2.TextureScale();
    param2.TextureTexCoord();
    param2.TextureStrength();
    param2.TextureIndex();
    param2.Factor();
    
    // Test with small number_array - skip ColorFactor if less than 3
    param2.number_array = {0.5};
    param2.TextureScale();
    param2.TextureTexCoord();
    param2.TextureStrength();
    param2.TextureIndex();
    param2.Factor();
    if (param2.number_array.size() >= 3) {
        tinygltf::ColorValue color3 = param2.ColorFactor();
    }
    
    // Test with exactly 3 elements
    param2.number_array = {0.1, 0.2, 0.3};
    param2.TextureScale();
    param2.TextureTexCoord();
    param2.TextureStrength();
    param2.TextureIndex();
    param2.Factor();
    if (param2.number_array.size() >= 3) {
        tinygltf::ColorValue color4 = param2.ColorFactor();
    }
    
    // Test with many elements
    param2.number_array.clear();
    for (int i = 0; i < 10; ++i) {
        param2.number_array.push_back(static_cast<double>(i) * 0.1);
    }
    param2.TextureScale();
    param2.TextureTexCoord();
    param2.TextureStrength();
    param2.TextureIndex();
    param2.Factor();
    if (param2.number_array.size() >= 3) {
        tinygltf::ColorValue color5 = param2.ColorFactor();
    }
    
    // Test with exactly 2 elements - skip ColorFactor
    param2.number_array = {0.7, 0.8};
    param2.TextureScale();
    param2.TextureTexCoord();
    param2.TextureStrength();
    param2.TextureIndex();
    param2.Factor();
    // Don't call ColorFactor on array with less than 3 elements
    
    return 0;
}