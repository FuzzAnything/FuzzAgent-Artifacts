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
#include "tiny_gltf.h"

static void fuzzParameterFunctions(const uint8_t *data, size_t size) {
    // Create a Parameter object
    tinygltf::Parameter param;
    
    // Initialize number_array with at least 3 values as required for ColorFactor
    // Use fuzzer data if available, otherwise default values
    param.number_array = {1.0, 1.0, 1.0, 1.0};
    
    // If we have enough data, populate number_array with fuzzer values
    if (size >= sizeof(double) * 3) {
        size_t num_doubles = std::min(size / sizeof(double), static_cast<size_t>(4));
        param.number_array.resize(num_doubles);
        for (size_t i = 0; i < num_doubles; ++i) {
            double value;
            memcpy(&value, data + i * sizeof(double), sizeof(double));
            param.number_array[i] = value;
        }
    }
    
    // Set other parameter fields that might be used by the functions
    if (size > 0) {
        param.has_number_value = true;
        param.number_value = static_cast<double>(data[0]);
        
        param.bool_value = (size > 1) ? (data[1] != 0) : false;
        
        // Set texture index related values
        if (size > 2) {
            // Use data[2] for texture index
            // Use data[3] for texture coordinate
            // The actual Parameter class might store these differently,
            // but we set what we can based on available public members
        }
    }
    
    // Call all target API functions
    // 1. ColorFactor
    try {
        auto color = param.ColorFactor();
        (void)color; // Use result to avoid unused variable warning
    } catch (...) {
        // Ignore exceptions
    }
    
    // 2. TextureScale
    try {
        double scale = param.TextureScale();
        (void)scale;
    } catch (...) {
        // Ignore exceptions
    }
    
    // 3. TextureIndex
    try {
        int texIndex = param.TextureIndex();
        (void)texIndex;
    } catch (...) {
        // Ignore exceptions
    }
    
    // 4. TextureTexCoord
    try {
        int texCoord = param.TextureTexCoord();
        (void)texCoord;
    } catch (...) {
        // Ignore exceptions
    }
    
    // 5. TextureStrength
    try {
        double strength = param.TextureStrength();
        (void)strength;
    } catch (...) {
        // Ignore exceptions
    }
    
    // 6. Factor
    try {
        double factor = param.Factor();
        (void)factor;
    } catch (...) {
        // Ignore exceptions
    }
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Main fuzzer entry point
    fuzzParameterFunctions(Data, Size);
    
    // Return 0 indicates successful execution
    return 0;
}