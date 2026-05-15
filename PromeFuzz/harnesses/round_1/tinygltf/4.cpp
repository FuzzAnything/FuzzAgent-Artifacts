// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::TextureTexCoord at tiny_gltf.h:453:7 in tiny_gltf.h
// tinygltf::Parameter::TextureIndex at tiny_gltf.h:442:7 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
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
#include <cstring>
#include <vector>
#include <map>
#include <string>

// Include the tinygltf header with the specified path
#include "tiny_gltf.h"

// Helper function to write data to a dummy file
static void writeToDummyFile(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

// Helper function to create a Parameter with random data from fuzzer input
static tinygltf::Parameter createParameterFromData(const uint8_t* data, size_t size, size_t& offset) {
    tinygltf::Parameter param;
    
    // Use fuzzer data to populate parameter fields
    if (offset + sizeof(bool) <= size) {
        bool has_number_array;
        memcpy(&has_number_array, data + offset, sizeof(bool));
        offset += sizeof(bool);
        
        if (has_number_array) {
            // Ensure we have at least 3 values for ColorFactor safety
            size_t array_size = 4; // Use 4 for RGBA default
            for (size_t i = 0; i < array_size && offset + sizeof(double) <= size; i++) {
                double value;
                memcpy(&value, data + offset, sizeof(double));
                offset += sizeof(double);
                param.number_array.push_back(value);
            }
            
            // Ensure minimum 3 elements for ColorFactor
            while (param.number_array.size() < 3) {
                param.number_array.push_back(1.0);
            }
        }
    }
    
    if (offset + sizeof(int) <= size) {
        int int_value;
        memcpy(&int_value, data + offset, sizeof(int));
        offset += sizeof(int);
        param.has_number_value = true;
        param.number_value = int_value;
    }
    
    if (offset + sizeof(double) <= size) {
        double double_value;
        memcpy(&double_value, data + offset, sizeof(double));
        offset += sizeof(double);
        param.has_number_value = true;
        param.number_value = double_value;
    }
    
    return param;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for potential file-based operations
    writeToDummyFile(Data, Size);
    
    // Create multiple Parameter objects with different configurations
    std::vector<tinygltf::Parameter> params;
    size_t offset = 0;
    
    // Create several parameters with different data from fuzzer input
    for (int i = 0; i < 5 && offset < Size; i++) {
        params.push_back(createParameterFromData(Data, Size, offset));
    }
    
    // If we couldn't create any parameters, create a default one
    if (params.empty()) {
        params.push_back(tinygltf::Parameter());
        // Ensure number_array has at least 3 elements for ColorFactor
        params[0].number_array = {1.0, 1.0, 1.0, 1.0};
    }
    
    // Test all target API functions on each parameter
    for (auto& param : params) {
        // 1. TextureTexCoord
        int texCoord = param.TextureTexCoord();
        (void)texCoord; // Use result to avoid unused variable warning
        
        // 2. TextureIndex
        int texIndex = param.TextureIndex();
        (void)texIndex;
        
        // 3. TextureScale
        double texScale = param.TextureScale();
        (void)texScale;
        
        // 4. ColorFactor - ensure number_array has at least 3 elements
        if (param.number_array.size() >= 3) {
            tinygltf::ColorValue color = param.ColorFactor();
            (void)color;
        }
        
        // 5. TextureStrength
        double texStrength = param.TextureStrength();
        (void)texStrength;
        
        // 6. Factor
        double factor = param.Factor();
        (void)factor;
    }
    
    // Create additional test cases with specific configurations
    // Test with empty parameter
    tinygltf::Parameter emptyParam;
    emptyParam.TextureTexCoord();
    emptyParam.TextureIndex();
    emptyParam.TextureScale();
    // Ensure number_array has elements before calling ColorFactor
    emptyParam.number_array = {0.5, 0.5, 0.5};
    emptyParam.ColorFactor();
    emptyParam.TextureStrength();
    emptyParam.Factor();
    
    // Test with parameter having only number_value
    tinygltf::Parameter numberParam;
    numberParam.has_number_value = true;
    numberParam.number_value = 42.0;
    numberParam.number_array = {1.0, 2.0, 3.0}; // Ensure ColorFactor safety
    numberParam.TextureTexCoord();
    numberParam.TextureIndex();
    numberParam.TextureScale();
    numberParam.ColorFactor();
    numberParam.TextureStrength();
    numberParam.Factor();
    
    // Test with parameter having only number_array
    tinygltf::Parameter arrayParam;
    arrayParam.number_array = {0.1, 0.2, 0.3, 0.4, 0.5};
    arrayParam.TextureTexCoord();
    arrayParam.TextureIndex();
    arrayParam.TextureScale();
    arrayParam.ColorFactor();
    arrayParam.TextureStrength();
    arrayParam.Factor();
    
    return 0;
}