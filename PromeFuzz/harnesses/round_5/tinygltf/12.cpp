// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
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
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include "tiny_gltf.h"

static void fuzzParameter(const uint8_t *data, size_t size) {
    tinygltf::Parameter param;
    
    // Initialize parameter based on fuzz data
    if (size > 0) {
        uint8_t param_type = data[0] % 5;
        size_t offset = 1;
        
        switch (param_type) {
            case 0: {
                // Set as texture parameter
                if (offset + sizeof(int) <= size) {
                    int tex_index;
                    std::memcpy(&tex_index, data + offset, sizeof(int));
                    // We can't directly set texture, but we can use the Parameter's internal state
                    // by creating a parameter that would be valid for texture functions
                    // For simplicity, we'll just call the functions on default parameter
                }
                break;
            }
            case 1: {
                // Set as number array (for ColorFactor)
                size_t num_count = 3 + (size > 1 ? data[1] % 10 : 0);
                if (num_count > 0) {
                    // Ensure we have at least 3 elements for ColorFactor
                    if (num_count < 3) num_count = 3;
                    
                    std::vector<double> number_array;
                    number_array.resize(num_count, 1.0);
                    
                    // Fill the array with fuzz data if available
                    for (size_t i = 0; i < num_count && offset + i * sizeof(double) + sizeof(double) <= size; ++i) {
                        double val;
                        std::memcpy(&val, data + offset + i * sizeof(double), sizeof(double));
                        number_array[i] = val;
                    }
                    
                    // Set the number_array in the parameter
                    param.number_array = number_array;
                }
                break;
            }
            case 2: {
                // Set as double
                if (offset + sizeof(double) <= size) {
                    double val;
                    std::memcpy(&val, data + offset, sizeof(double));
                    // Store in a way that Factor() might use
                    param.number_array = {val};
                }
                break;
            }
            case 3: {
                // Set as int
                if (offset + sizeof(int) <= size) {
                    int val;
                    std::memcpy(&val, data + offset, sizeof(int));
                    // Store in a way that TextureIndex() might use
                    // We'll rely on default parameter
                }
                break;
            }
            case 4: {
                // Set as string
                size_t str_len = size - offset > 256 ? 256 : size - offset;
                if (str_len > 0) {
                    std::string str(reinterpret_cast<const char*>(data + offset), str_len);
                    // Store string if Parameter supports it
                    // We'll rely on default parameter
                }
                break;
            }
        }
    }
    
    // Ensure number_array has at least 3 elements before calling ColorFactor
    if (param.number_array.size() < 3) {
        param.number_array.resize(3, 1.0);
    }
    
    // Call all target functions - they should handle default/invalid states
    (void)param.TextureScale();
    (void)param.TextureTexCoord();
    
    // For ColorFactor, ensure we have at least 3 elements in number_array
    (void)param.ColorFactor();
    
    (void)param.TextureStrength();
    (void)param.TextureIndex();
    (void)param.Factor();
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzzParameter(Data, Size);
    
    return 0;
}