// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <tiny_gltf.h>

// Fuzz driver for tinygltf::Parameter methods
// Targets: TextureScale, TextureTexCoord, ColorFactor, Factor, TextureStrength, TextureIndex

static void FuzzParameterMethods(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    // Create a Parameter instance
    tinygltf::Parameter param;

    // 1. Fuzz number_value (used by Factor())
    if (Size >= sizeof(double)) {
        double val;
        memcpy(&val, Data, sizeof(double));
        param.number_value = val;
    }

    // 2. Fuzz number_array (used by ColorFactor())
    param.number_array.clear();
    
    size_t offset = 0;
    if (Size >= sizeof(double)) {
        // Try to parse as many doubles as possible
        while (offset + sizeof(double) <= Size) {
            double val;
            memcpy(&val, Data + offset, sizeof(double));
            param.number_array.push_back(val);
            offset += sizeof(double);
            
            // Limit the number of elements to prevent excessive memory usage or logic loops
            if (param.number_array.size() >= 10) break;
        }
    }
    
    // 3. Fuzz string_value
    if (Size > 0) {
        size_t str_len = 0;
        while (str_len < Size && Data[str_len] != 0) {
            str_len++;
        }
        if (str_len > 0) {
            param.string_value.assign(reinterpret_cast<const char*>(Data), str_len);
        }
    }

    // Invoke the target functions
    
    // 1. TextureScale
    double scale = param.TextureScale();
    (void)scale;

    // 2. TextureTexCoord
    int texCoord = param.TextureTexCoord();
    (void)texCoord;

    // 3. ColorFactor
    // The library implementation accesses number_array[0], [1], [2] without bounds checking.
    // To prevent a crash in debug mode (assertion failure) or undefined behavior in release,
    // we must ensure the vector has at least 3 elements before calling this method.
    if (param.number_array.size() >= 3) {
        tinygltf::ColorValue color = param.ColorFactor();
        (void)color;
    }

    // 4. Factor
    double factor = param.Factor();
    (void)factor;

    // 5. TextureStrength
    double strength = param.TextureStrength();
    (void)strength;

    // 6. TextureIndex
    int texIndex = param.TextureIndex();
    (void)texIndex;
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Wrap logic to handle potential exceptions or crashes
    try {
        FuzzParameterMethods(Data, Size);
    } catch (...) {
        // Catch any unexpected exceptions
        return 0;
    }
    
    return 0;
}