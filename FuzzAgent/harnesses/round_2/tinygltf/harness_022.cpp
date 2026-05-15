#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_022 {
// Harness 022: Comprehensive GLTF Light functionality fuzzer for tinygltf
// Targets completely uncovered Light APIs with 0% API coverage and significant undiscovered branch complexity:
// - 6 branches in Light equality operator (Light::operator==)
// - 14 branches in ParseLight function
// - SpotLight parsing and equality (20 undiscovered branches from harness_011 only lightly touched)
// Focus: Light parsing, validation, equality operations, and various light types (directional, point, spot)
// with different parameters and edge cases.
// Differentiates from existing harnesses:
//   - harness_000: Basic glTF loading (not Light-specific)
//   - harness_001: Extended glTF loading (not Light-specific)
//   - harness_002: File I/O operations (not Light-specific)
//   - harness_011: Equality operators for multiple components (includes SpotLight but not comprehensive Light testing)
//   - harness_013-021: Image loading, callbacks, zlib, JSON parsing, image writing, GIF loading, HDR processing (not Light-related)
// Creates a dedicated Light API harness targeting ParseLight, Light::operator==, SpotLight::operator==, 
// and various light configurations for maximum coverage gain on uncovered Light functionality.
// Uses FuzzedDataProvider for structured input splitting to generate diverse light configurations.


// Include FuzzedDataProvider for structured input splitting

// Include tinygltf header - DO NOT define TINYGLTF_IMPLEMENTATION
// since we're linking against libtinygltf.a which already has implementations

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful Light testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector (0-3 for different testing strategies)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Create TinyGLTF loader context
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err, warn;
    
    // Path 0: Direct Light equality operator testing with fuzzed Light objects
    if (operation == 0) {
        // Create multiple Light objects with fuzzed parameters
        tinygltf::Light light1, light2, light3;
        
        // Consume light type for light1
        uint8_t type_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (type_selector) {
            case 0: light1.type = "directional"; break;
            case 1: light1.type = "point"; break;
            case 2: light1.type = "spot"; break;
        }
        
        // Consume light type for light2 (could be same or different)
        type_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (type_selector) {
            case 0: light2.type = "directional"; break;
            case 1: light2.type = "point"; break;
            case 2: light2.type = "spot"; break;
        }
        
        // Consume names
        light1.name = fdp.ConsumeRandomLengthString(32);
        light2.name = fdp.ConsumeRandomLengthString(32);
        light3.name = fdp.ConsumeRandomLengthString(32);
        
        // Consume color arrays (RGB)
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
            light1.color.push_back(fdp.ConsumeFloatingPoint<double>());
        }
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
            light2.color.push_back(fdp.ConsumeFloatingPoint<double>());
        }
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
            light3.color.push_back(fdp.ConsumeFloatingPoint<double>());
        }
        
        // Consume intensity values
        light1.intensity = fdp.ConsumeFloatingPoint<double>();
        light2.intensity = fdp.ConsumeFloatingPoint<double>();
        light3.intensity = fdp.ConsumeFloatingPoint<double>();
        
        // Consume range values
        light1.range = fdp.ConsumeFloatingPoint<double>();
        light2.range = fdp.ConsumeFloatingPoint<double>();
        light3.range = fdp.ConsumeFloatingPoint<double>();
        
        // Setup spot light parameters if type is "spot"
        if (light1.type == "spot" && fdp.remaining_bytes() >= 16) {
            light1.spot.innerConeAngle = fdp.ConsumeFloatingPoint<double>();
            light1.spot.outerConeAngle = fdp.ConsumeFloatingPoint<double>();
        }
        if (light2.type == "spot" && fdp.remaining_bytes() >= 16) {
            light2.spot.innerConeAngle = fdp.ConsumeFloatingPoint<double>();
            light2.spot.outerConeAngle = fdp.ConsumeFloatingPoint<double>();
        }
        
        // Test Light equality operators (6 undiscovered branches)
        bool eq1 = (light1 == light2);
        bool eq2 = (light2 == light1);
        bool eq3 = (light1 == light1);  // Self-equality should be true
        bool eq4 = (light2 == light2);  // Self-equality should be true
        bool eq5 = (light3 == light3);  // Self-equality should be true
        bool eq6 = (light1 == light3);
        bool eq7 = (light3 == light2);
        
        // Use results to avoid dead code elimination
        (void)eq1; (void)eq2; (void)eq3; (void)eq4; (void)eq5; (void)eq6; (void)eq7;
        
        // Test SpotLight equality operators if applicable
        if (light1.type == "spot" && light2.type == "spot") {
            bool spot_eq1 = (light1.spot == light2.spot);
            bool spot_eq2 = (light1.spot == light1.spot);  // Self-equality
            bool spot_eq3 = (light2.spot == light2.spot);  // Self-equality
            (void)spot_eq1; (void)spot_eq2; (void)spot_eq3;
        }
        
        // Test with modified copies
        tinygltf::Light light1_copy = light1;
        bool copy_eq = (light1 == light1_copy);
        (void)copy_eq;
        
        // Modify copy and test inequality
        if (!light1_copy.color.empty()) {
            light1_copy.color[0] += 1.0;
            bool modified_eq = (light1 == light1_copy);
            (void)modified_eq;
        }
        
        return 0;
    }
    
    // Path 1: ParseLight function testing with JSON input
    else if (operation == 1) {
        // Create JSON string that represents a light object
        std::string json_str = "{";
        
        // Consume light type
        uint8_t type_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
        std::string light_type;
        switch (type_selector) {
            case 0: light_type = "directional"; break;
            case 1: light_type = "point"; break;
            case 2: light_type = "spot"; break;
        }
        json_str += "\"type\":\"" + light_type + "\",";
        
        // Consume name (optional)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            std::string name = fdp.ConsumeRandomLengthString(32);
            // Escape quotes in name
            std::string escaped_name;
            for (char c : name) {
                if (c == '"' || c == '\\') escaped_name += '\\';
                escaped_name += c;
            }
            json_str += "\"name\":\"" + escaped_name + "\",";
        }
        
        // Consume color array (optional, 3 floats)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 24) {
            double r = fdp.ConsumeFloatingPoint<double>();
            double g = fdp.ConsumeFloatingPoint<double>();
            double b = fdp.ConsumeFloatingPoint<double>();
            json_str += "\"color\":[" + std::to_string(r) + "," + 
                                     std::to_string(g) + "," + 
                                     std::to_string(b) + "],";
        }
        
        // Consume intensity (optional)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 8) {
            double intensity = fdp.ConsumeFloatingPoint<double>();
            json_str += "\"intensity\":" + std::to_string(intensity) + ",";
        }
        
        // Consume range (optional)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 8) {
            double range = fdp.ConsumeFloatingPoint<double>();
            json_str += "\"range\":" + std::to_string(range) + ",";
        }
        
        // Add spot object if type is "spot"
        if (light_type == "spot" && fdp.remaining_bytes() >= 16) {
            double innerAngle = fdp.ConsumeFloatingPoint<double>();
            double outerAngle = fdp.ConsumeFloatingPoint<double>();
            json_str += "\"spot\":{";
            json_str += "\"innerConeAngle\":" + std::to_string(innerAngle) + ",";
            json_str += "\"outerConeAngle\":" + std::to_string(outerAngle);
            json_str += "},";
        }
        
        // Remove trailing comma if exists
        if (json_str.back() == ',') {
            json_str.pop_back();
        }
        json_str += "}";
        
        // Parse the JSON to test ParseLight indirectly through model loading
        bool loaded = ctx.LoadASCIIFromString(&model, &err, &warn,
                                              json_str.c_str(),
                                              static_cast<unsigned int>(json_str.size()),
                                              "", 0);
        (void)loaded; // We're testing the parsing, not the loading success
        
        // If lights were parsed, test their properties
        if (!model.lights.empty()) {
            for (const auto& light : model.lights) {
                // Access all light properties to ensure code paths are exercised
                (void)light.name;
                (void)light.type;
                (void)light.color.size();
                (void)light.intensity;
                (void)light.range;
                
                // Test equality with itself
                bool self_eq = (light == light);
                (void)self_eq;
                
                // Test SpotLight if applicable
                if (light.type == "spot") {
                    (void)light.spot.innerConeAngle;
                    (void)light.spot.outerConeAngle;
                    
                    // Test SpotLight equality
                    bool spot_self_eq = (light.spot == light.spot);
                    (void)spot_self_eq;
                }
            }
        }
        
        return 0;
    }
    
    // Path 2: Multiple lights in model with various configurations
    else if (operation == 2) {
        // Create a JSON with multiple lights
        std::string json_str = "{\"lights\":[";
        
        // Determine number of lights to create (1-5)
        int num_lights = fdp.ConsumeIntegralInRange<int>(1, 5);
        
        for (int i = 0; i < num_lights && fdp.remaining_bytes() > 50; i++) {
            json_str += "{";
            
            // Light type
            uint8_t type_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
            std::string light_type;
            switch (type_selector) {
                case 0: light_type = "directional"; break;
                case 1: light_type = "point"; break;
                case 2: light_type = "spot"; break;
            }
            json_str += "\"type\":\"" + light_type + "\",";
            
            // Name with index
            json_str += "\"name\":\"light_" + std::to_string(i) + "\",";
            
            // Color (always include for testing)
            if (fdp.remaining_bytes() >= 24) {
                double r = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
                double g = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
                double b = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
                json_str += "\"color\":[" + std::to_string(r) + "," + 
                                         std::to_string(g) + "," + 
                                         std::to_string(b) + "],";
            }
            
            // Intensity
            if (fdp.remaining_bytes() >= 8) {
                double intensity = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
                json_str += "\"intensity\":" + std::to_string(intensity) + ",";
            }
            
            // Range (for point and spot lights)
            if ((light_type == "point" || light_type == "spot") && fdp.remaining_bytes() >= 8) {
                double range = fdp.ConsumeFloatingPointInRange<double>(0.0, 1000.0);
                json_str += "\"range\":" + std::to_string(range) + ",";
            }
            
            // Spot parameters
            if (light_type == "spot" && fdp.remaining_bytes() >= 16) {
                double innerAngle = fdp.ConsumeFloatingPointInRange<double>(0.0, 3.14159);
                double outerAngle = fdp.ConsumeFloatingPointInRange<double>(innerAngle, 3.14159);
                json_str += "\"spot\":{";
                json_str += "\"innerConeAngle\":" + std::to_string(innerAngle) + ",";
                json_str += "\"outerConeAngle\":" + std::to_string(outerAngle);
                json_str += "},";
            }
            
            // Remove trailing comma
            if (json_str.back() == ',') {
                json_str.pop_back();
            }
            
            json_str += "}";
            if (i < num_lights - 1) {
                json_str += ",";
            }
        }
        
        json_str += "]}";
        
        // Load the model
        bool loaded = ctx.LoadASCIIFromString(&model, &err, &warn,
                                              json_str.c_str(),
                                              static_cast<unsigned int>(json_str.size()),
                                              "", 0);
        (void)loaded;
        
        // Test all light equality combinations
        if (!model.lights.empty()) {
            for (size_t i = 0; i < model.lights.size() && i < 3; i++) {
                for (size_t j = 0; j < model.lights.size() && j < 3; j++) {
                    bool light_eq = (model.lights[i] == model.lights[j]);
                    (void)light_eq;
                }
            }
            
            // Test SpotLight equality for spot lights
            for (size_t i = 0; i < model.lights.size(); i++) {
                if (model.lights[i].type == "spot") {
                    for (size_t j = 0; j < model.lights.size(); j++) {
                        if (model.lights[j].type == "spot") {
                            bool spot_eq = (model.lights[i].spot == model.lights[j].spot);
                            (void)spot_eq;
                        }
                    }
                }
            }
        }
        
        return 0;
    }
    
    // Path 3: Edge case and error testing for Light parsing
    else {
        // Test various edge cases and error conditions
        
        // Case 3a: Missing required "type" field
        if (fdp.ConsumeBool()) {
            std::string json_str = "{\"name\":\"test_light\",\"color\":[1,0,0]}";
            tinygltf::Model model2;
            std::string err2, warn2;
            bool loaded = ctx.LoadASCIIFromString(&model2, &err2, &warn2,
                                                  json_str.c_str(),
                                                  static_cast<unsigned int>(json_str.size()),
                                                  "", 0);
            (void)loaded;
        }
        
        // Case 3b: Invalid light type
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            std::string invalid_type = fdp.ConsumeRandomLengthString(10);
            std::string json_str = "{\"type\":\"" + invalid_type + "\"}";
            tinygltf::Model model2;
            std::string err2, warn2;
            bool loaded = ctx.LoadASCIIFromString(&model2, &err2, &warn2,
                                                  json_str.c_str(),
                                                  static_cast<unsigned int>(json_str.size()),
                                                  "", 0);
            (void)loaded;
        }
        
        // Case 3c: Spot light without "spot" object
        if (fdp.ConsumeBool()) {
            std::string json_str = "{\"type\":\"spot\",\"name\":\"invalid_spot\"}";
            tinygltf::Model model2;
            std::string err2, warn2;
            bool loaded = ctx.LoadASCIIFromString(&model2, &err2, &warn2,
                                                  json_str.c_str(),
                                                  static_cast<unsigned int>(json_str.size()),
                                                  "", 0);
            (void)loaded;
        }
        
        // Case 3d: Malformed color array
        if (fdp.ConsumeBool()) {
            std::string json_str = "{\"type\":\"point\",\"color\":\"not_an_array\"}";
            tinygltf::Model model2;
            std::string err2, warn2;
            bool loaded = ctx.LoadASCIIFromString(&model2, &err2, &warn2,
                                                  json_str.c_str(),
                                                  static_cast<unsigned int>(json_str.size()),
                                                  "", 0);
            (void)loaded;
        }
        
        // Case 3e: Empty color array
        if (fdp.ConsumeBool()) {
            std::string json_str = "{\"type\":\"directional\",\"color\":[]}";
            tinygltf::Model model2;
            std::string err2, warn2;
            bool loaded = ctx.LoadASCIIFromString(&model2, &err2, &warn2,
                                                  json_str.c_str(),
                                                  static_cast<unsigned int>(json_str.size()),
                                                  "", 0);
            (void)loaded;
        }
        
        // Case 3f: Extremely large values
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 24) {
            double huge_val = fdp.ConsumeFloatingPoint<double>();
            double tiny_val = fdp.ConsumeFloatingPoint<double>();
            double neg_val = fdp.ConsumeFloatingPoint<double>();
            
            std::string json_str = "{";
            json_str += "\"type\":\"point\",";
            json_str += "\"intensity\":" + std::to_string(huge_val) + ",";
            json_str += "\"range\":" + std::to_string(tiny_val) + ",";
            json_str += "\"color\":[" + std::to_string(neg_val) + "," + 
                                     std::to_string(huge_val) + "," + 
                                     std::to_string(tiny_val) + "]";
            json_str += "}";
            
            tinygltf::Model model2;
            std::string err2, warn2;
            bool loaded = ctx.LoadASCIIFromString(&model2, &err2, &warn2,
                                                  json_str.c_str(),
                                                  static_cast<unsigned int>(json_str.size()),
                                                  "", 0);
            (void)loaded;
        }
        
        // Case 3g: Valid spot light with nested extras/extensions simulation
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            std::string json_str = "{";
            json_str += "\"type\":\"spot\",";
            json_str += "\"name\":\"spot_with_extras\",";
            json_str += "\"intensity\":2.5,";
            json_str += "\"range\":50.0,";
            json_str += "\"color\":[1.0,0.8,0.6],";
            json_str += "\"spot\":{";
            json_str += "\"innerConeAngle\":0.5,";
            json_str += "\"outerConeAngle\":1.0,";
            json_str += "\"extras\":{\"custom\":\"data\"}";
            json_str += "},";
            json_str += "\"extras\":{\"author\":\"fuzzer\"}";
            json_str += "}";
            
            tinygltf::Model model2;
            std::string err2, warn2;
            bool loaded = ctx.LoadASCIIFromString(&model2, &err2, &warn2,
                                                  json_str.c_str(),
                                                  static_cast<unsigned int>(json_str.size()),
                                                  "", 0);
            (void)loaded;
            
            // Test equality if loaded successfully
            if (!model2.lights.empty()) {
                bool self_eq = (model2.lights[0] == model2.lights[0]);
                (void)self_eq;
                
                if (model2.lights[0].type == "spot") {
                    bool spot_self_eq = (model2.lights[0].spot == model2.lights[0].spot);
                    (void)spot_self_eq;
                }
            }
        }
        
        return 0;
    }
}

} // namespace harness_022
