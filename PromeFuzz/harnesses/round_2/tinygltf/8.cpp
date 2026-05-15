// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
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
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <map>

#include "tiny_gltf.h"

static void initializeParameter(tinygltf::Parameter& param, const uint8_t* data, size_t size) {
    // Always ensure number_array has at least 3 elements as required by ColorFactor
    param.number_array = {1.0, 1.0, 1.0, 1.0};
    
    // Use fuzzer data to populate number_array with values from data
    for (size_t i = 0; i < param.number_array.size() && i < size; ++i) {
        param.number_array[i] = static_cast<double>(data[i % size]);
    }
    
    // Populate json_double_value with some entries
    if (size > 10) {
        for (size_t i = 0; i < 3 && (i + 10) < size; ++i) {
            std::string key = "key" + std::to_string(i);
            param.json_double_value[key] = static_cast<double>(data[i + 10]);
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    tinygltf::Parameter param;
    initializeParameter(param, Data, Size);
    
    // Call all target API functions
    int texture_index = param.TextureIndex();
    (void)texture_index;
    
    tinygltf::ColorValue color_factor = param.ColorFactor();
    (void)color_factor;
    
    double texture_strength = param.TextureStrength();
    (void)texture_strength;
    
    double texture_scale = param.TextureScale();
    (void)texture_scale;
    
    int texture_tex_coord = param.TextureTexCoord();
    (void)texture_tex_coord;
    
    double factor = param.Factor();
    (void)factor;
    
    return 0;
}