// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
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

// Helper to create a valid JSON string from fuzzing input
static std::string MakeValidJSON(const uint8_t *Data, size_t Size) {
    std::string input(reinterpret_cast<const char*>(Data), Size);
    std::string json_str = "{";
    // Add some safe keys to ensure valid structure
    json_str += "\"test_key\": ";
    
    // Try to wrap the raw data in a way that might be parsed
    if (Size > 0 && Data[0] == '{') {
        json_str += input;
        if (Size > 0 && Data[Size-1] != '}') {
            json_str += "}";
        }
    } else if (Size > 0 && Data[0] == '[') {
        json_str += input;
        if (Size > 0 && Data[Size-1] != ']') {
            json_str += "]";
        }
    } else {
        json_str += "\"";
        for (size_t i = 0; i < Size; ++i) {
            unsigned char c = Data[i];
            if (c == '"') json_str += "\\\"";
            else if (c == '\\') json_str += "\\\\";
            else if (c < 32) json_str += "\\u" + std::string(1, (c / 16) + (c >= 10 ? 'a' - 10 : '0')) + std::string(1, (c % 16) + (c >= 10 ? 'a' - 10 : '0'));
            else json_str += c;
        }
        json_str += "\"";
    }
    json_str += "}";
    return json_str;
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment
    tinygltf::Parameter param;
    std::string err;
    
    // Create a JSON object from input
    std::string json_str = MakeValidJSON(Data, Size);
    
    try {
        // Test TextureScale
        double scale = param.TextureScale();
        (void)scale;
        
        // Test ColorFactor
        // Ensure number_array has enough elements before calling ColorFactor to avoid out-of-bounds
        if (param.number_array.size() < 3) {
            param.number_array.resize(3, 0.0);
        }
        tinygltf::ColorValue color = param.ColorFactor();
        (void)color;
        
        // Test Factor
        double factor = param.Factor();
        (void)factor;
        
        // Test TextureStrength
        double strength = param.TextureStrength();
        (void)strength;

        // Strategy 2: Try to parse a file to trigger ParseParameterProperty indirectly
        const char *filename = "./dummy_file";
        FILE *f = fopen(filename, "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
            
            tinygltf::Model model;
            tinygltf::TinyGLTF ctx;
            std::string warn;
            
            bool ret = ctx.LoadASCIIFromFile(&model, &err, &warn, filename);
            (void)ret;
            
            ret = ctx.LoadBinaryFromFile(&model, &err, &warn, filename);
            (void)ret;
        }
        
        // Strategy 3: Directly manipulate Parameter fields to test edge cases
        param.number_value = 1.0;
        factor = param.Factor();
        
        // Set number_array (for ColorFactor) - ensure size is at least 3
        param.number_array = {1.0, 0.0, 0.0};
        color = param.ColorFactor();
        
        param.number_array = {1.0, 0.0, 0.0, 0.5};
        color = param.ColorFactor();
        
        // Set empty array and resize to avoid crash
        param.number_array = {};
        if (param.number_array.size() < 3) {
            param.number_array.resize(3, 0.0);
        }
        color = param.ColorFactor();
        
        param.string_value = "test";
        param.bool_value = true;
        
        param.json_double_value["scale"] = 2.0;
        scale = param.TextureScale();
        
        param.json_double_value["strength"] = 0.5;
        strength = param.TextureStrength();
        
        // Clean up file
        std::remove(filename);
        
    } catch (...) {
        return 0;
    }

    return 0;
}