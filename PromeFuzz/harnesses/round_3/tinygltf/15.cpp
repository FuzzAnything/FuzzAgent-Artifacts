// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
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
#include <map>
#include <string>

#include "tiny_gltf.h"

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Use a static dummy file path
    static const char* dummy_file = "./dummy_file";
    
    // Write fuzzer data to the dummy file
    FILE* f = fopen(dummy_file, "wb");
    if (!f) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);
    
    // Create Parameter objects
    tinygltf::Parameter param1;
    tinygltf::Parameter param2;
    tinygltf::Parameter param3;
    
    // ALWAYS initialize number_array with at least 3 elements before calling ColorFactor
    // This is critical to prevent the segfault
    param1.number_array = {1.0, 1.0, 1.0, 1.0};  // 4 elements
    param2.number_array = {1.0, 1.0, 1.0, 1.0};  // Always initialize with default
    param3.number_array = {1.0, 1.0, 1.0, 1.0};  // Always initialize with default
    
    // Initialize with fuzzer data if available, but keep at least 3 elements
    if (Size >= 12) {
        param2.number_array = {
            static_cast<double>(Data[0]),
            static_cast<double>(Data[1]),
            static_cast<double>(Data[2]),
            static_cast<double>(Data[3])
        };
    }
    
    // Initialize json_double_value for factor and scale functions
    if (Size >= 4) {
        param1.json_double_value["scale"] = static_cast<double>(Data[0] % 100);
        param1.json_double_value["strength"] = static_cast<double>(Data[1] % 100);
        param1.json_double_value["factor"] = static_cast<double>(Data[2] % 100);
        
        param2.json_double_value["scale"] = static_cast<double>(Data[3] % 100);
        param2.json_double_value["strength"] = static_cast<double>(Data[0] % 100);
        param2.json_double_value["factor"] = static_cast<double>(Data[1] % 100);
        
        param3.json_double_value["scale"] = static_cast<double>(Data[2] % 100);
        param3.json_double_value["strength"] = static_cast<double>(Data[3] % 100);
        param3.json_double_value["factor"] = static_cast<double>(Data[0] % 100);
    }
    
    // Initialize string_value for texture type identification
    // Set string_value to indicate texture type
    if (Size >= 2) {
        // Use fuzzer data to determine what type of texture parameter this is
        uint8_t texture_type = Data[0] % 4;
        switch (texture_type) {
            case 0:
                param1.string_value = "normalTexture";
                break;
            case 1:
                param1.string_value = "occlusionTexture";
                break;
            case 2:
                param1.string_value = "baseColorTexture";
                break;
            case 3:
                param1.string_value = "metallicRoughnessTexture";
                break;
        }
        
        uint8_t texture_type2 = Data[1] % 4;
        switch (texture_type2) {
            case 0:
                param2.string_value = "normalTexture";
                break;
            case 1:
                param2.string_value = "occlusionTexture";
                break;
            case 2:
                param2.string_value = "baseColorTexture";
                break;
            case 3:
                param2.string_value = "metallicRoughnessTexture";
                break;
        }
        
        // Also set for param3
        uint8_t texture_type3 = (Size >= 3) ? (Data[2] % 4) : 0;
        switch (texture_type3) {
            case 0:
                param3.string_value = "normalTexture";
                break;
            case 1:
                param3.string_value = "occlusionTexture";
                break;
            case 2:
                param3.string_value = "baseColorTexture";
                break;
            case 3:
                param3.string_value = "metallicRoughnessTexture";
                break;
        }
    }
    
    // Call all target API functions on different parameter objects
    // 1. ColorFactor - Now safe because number_array always has at least 3 elements
    auto color1 = param1.ColorFactor();
    (void)color1;
    
    auto color2 = param2.ColorFactor();
    (void)color2;
    
    auto color3 = param3.ColorFactor();
    (void)color3;
    
    // 2. TextureScale
    double scale1 = param1.TextureScale();
    (void)scale1;
    
    double scale2 = param2.TextureScale();
    (void)scale2;
    
    double scale3 = param3.TextureScale();
    (void)scale3;
    
    // 3. TextureIndex
    int index1 = param1.TextureIndex();
    (void)index1;
    
    int index2 = param2.TextureIndex();
    (void)index2;
    
    int index3 = param3.TextureIndex();
    (void)index3;
    
    // 4. TextureTexCoord
    int texCoord1 = param1.TextureTexCoord();
    (void)texCoord1;
    
    int texCoord2 = param2.TextureTexCoord();
    (void)texCoord2;
    
    int texCoord3 = param3.TextureTexCoord();
    (void)texCoord3;
    
    // 5. TextureStrength
    double strength1 = param1.TextureStrength();
    (void)strength1;
    
    double strength2 = param2.TextureStrength();
    (void)strength2;
    
    double strength3 = param3.TextureStrength();
    (void)strength3;
    
    // 6. Factor
    double factor1 = param1.Factor();
    (void)factor1;
    
    double factor2 = param2.Factor();
    (void)factor2;
    
    double factor3 = param3.Factor();
    (void)factor3;
    
    // Clean up dummy file
    remove(dummy_file);
    
    return 0;
}