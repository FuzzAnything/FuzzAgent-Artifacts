// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Parameter::Factor at tiny_gltf.h:489:10 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
// tinygltf::Parameter::TextureStrength at tiny_gltf.h:477:10 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::Parameter::ColorFactor at tiny_gltf.h:494:14 in tiny_gltf.h
// tinygltf::Parameter::TextureScale at tiny_gltf.h:465:10 in tiny_gltf.h
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

// Fuzzing entry point
extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Interpret input as a potential JSON string
    std::string json_str(reinterpret_cast<const char*>(Data), Size);

    // Write input to a dummy file to test loading
    const char* dummy_file = "./dummy_file";
    FILE* f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    } else {
        return 0;
    }

    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Use TinyGLTF loader class which is the standard API
    tinygltf::TinyGLTF loader;
    
    // Try to load ASCII file
    bool loaded = loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file, 0);
    
    // If that fails, try binary (some files might be binary glTF)
    if (!loaded) {
        loaded = loader.LoadBinaryFromFile(&model, &err, &warn, dummy_file, 0);
    }

    // If we have materials, we can test the Parameter methods.
    for (auto& mat : model.materials) {
        // Access values map (Parameter map)
        // tinygltf::Material has 'values' which is a map<string, Parameter>
        for (auto& kv : mat.values) {
            tinygltf::Parameter& param = kv.second;
            
            // Test ColorFactor
            tinygltf::ColorValue color = param.ColorFactor();
            (void)color;
            
            // Test TextureScale
            double scale = param.TextureScale();
            (void)scale;
            
            // Test TextureStrength
            double strength = param.TextureStrength();
            (void)strength;
            
            // Test Factor
            double factor = param.Factor();
            (void)factor;
        }
    }

    // To test ParseParameterProperty, we need a detail::json object.
    // Since we cannot easily construct the internal JSON type without knowing the backend (RapidJSON vs nlohmann),
    // and direct construction is complex, we rely on the loader to populate the model.
    // However, to specifically test ParseParameterProperty, we can try to construct a simple JSON object
    // if the backend is nlohmann (as suggested by the prompt's data types).
    
    // We will attempt to include nlohmann/json.hpp if not already included by tiny_gltf.h
    // and try to construct a JSON object to test ParseParameterProperty.
    // Note: This part might not compile if the backend is RapidJSON, but we try.
    
    // We use a try-catch to handle potential runtime errors during JSON parsing.
    try {
        // Attempt to parse the input string as JSON using nlohmann if available.
        // We use a macro or check if nlohmann is available.
        // Since we can't check at compile time easily here, we try to use it.
        // If the compilation fails, it means nlohmann is not the backend or not included.
        
        // We will try to use the tinygltf::detail namespace if it exposes the json type.
        // If the backend is nlohmann, tinygltf::detail::json is likely nlohmann::json.
        
        // We try to construct a JSON object from the input string.
        // We assume the input might be a valid JSON object.
        
        // Since we cannot guarantee the backend, we skip direct JSON construction for ParseParameterProperty
        // if it leads to compilation errors. Instead, we rely on the loader's internal parsing.
        // The loader internally calls ParseParameterProperty when parsing materials.
        // Thus, loading the file already exercises ParseParameterProperty indirectly.
        
        // If we want to test ParseParameterProperty directly, we need to access the internal JSON.
        // This is often not possible without exposing the internal types.
        // We will assume the indirect testing via LoadASCIIFromFile is sufficient for the fuzz driver.
        
        // However, to be thorough, we try to construct a Parameter and call ParseParameterProperty
        // if we can access the internal JSON type.
        
        // We will skip direct testing of ParseParameterProperty if the internal JSON type is not accessible.
        // The loader's LoadASCIIFromFile already tests the parsing logic including ParseParameterProperty.
        
    } catch (...) {
        // Ignore exceptions
    }

    // Test GetNumber if we can access the internal JSON.
    // Similar to ParseParameterProperty, we rely on the loader.
    
    // We also test the Parameter methods on manually constructed Parameters to ensure robustness.
    tinygltf::Parameter p1;
    p1.number_value = 1.0;
    (void)p1.Factor();
    
    tinygltf::Parameter p2;
    p2.number_array = {1.0, 2.0, 3.0, 4.0};
    (void)p2.ColorFactor();
    
    tinygltf::Parameter p3;
    p3.json_double_value["scale"] = 2.0;
    (void)p3.TextureScale();
    
    tinygltf::Parameter p4;
    p4.json_double_value["strength"] = 0.5;
    (void)p4.TextureStrength();

    return 0;
}