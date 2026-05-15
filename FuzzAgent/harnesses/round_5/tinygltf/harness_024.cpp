#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_024 {
/*
 * Fuzzing harness for tinygltf library - specifically targeting LoadASCIIFromString/LoadFromString
 * with 62 undiscovered branches and 0% coverage.
 * 
 * This harness focuses on exploring error handling paths and edge cases in the ASCII loading pipeline:
 * 1. LoadASCIIFromString -> LoadFromString -> JSON parsing -> validation
 * 2. Comprehensive error path coverage: JSON parsing failures, version validation, scene validation
 * 3. Edge cases: empty strings, short strings, malformed JSON, special characters
 * 4. Various check_sections flag combinations
 * 5. Different base_dir scenarios
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: Tests LoadASCIIFromString as part of a switch statement
 * - harness_012.cpp: Tests LoadASCIIFromString with WriteGltfSceneToStream
 * - harness_014.cpp: Tests LoadASCIIFromString with configuration callbacks
 * - This harness: Focuses exclusively on error paths and edge cases in LoadASCIIFromString/LoadFromString
 * 
 * Strategy: Use FuzzedDataProvider to generate diverse input that triggers different error conditions
 * and validation paths in the ASCII loading code.
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need at least some bytes for meaningful testing
    if (size < 1) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Consume configuration bytes
    uint8_t config_byte = fdp.ConsumeIntegral<uint8_t>();
    
    // Parse check_sections flags - these control validation strictness
    unsigned int check_sections = 0;
    bool check_version = (config_byte & 0x01) != 0;
    bool check_scene = (config_byte & 0x02) != 0;
    bool check_scenes = (config_byte & 0x04) != 0;
    bool check_all = (config_byte & 0x08) != 0;
    
    if (check_version) check_sections |= tinygltf::REQUIRE_VERSION;
    if (check_scene) check_sections |= tinygltf::REQUIRE_SCENE;
    if (check_scenes) check_sections |= tinygltf::REQUIRE_SCENES;
    if (check_all) check_sections = tinygltf::REQUIRE_ALL;
    
    // Consume parse strictness mode
    uint8_t strictness_mode = (config_byte >> 4) & 0x03;
    
    // Consume base_dir type (0-3 for different base_dir strategies)
    uint8_t base_dir_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Determine how to generate the JSON string
    uint8_t json_gen_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    std::string ascii_content;
    std::string base_dir;
    
    // Generate base_dir based on type
    switch (base_dir_type) {
        case 0:
            base_dir = "";  // Empty base_dir
            break;
        case 1:
            base_dir = "/tmp";  // Simple path
            break;
        case 2:
            base_dir = "/home/user/documents";  // Longer path
            break;
        case 3:
            // Consume random string for base_dir (could be invalid path)
            if (fdp.remaining_bytes() > 0) {
                base_dir = fdp.ConsumeRandomLengthString(128);
            }
            break;
    }
    
    // Generate ASCII content based on json_gen_type
    switch (json_gen_type) {
        case 0: {
            // Empty or very short string (triggers "JSON string too short" error)
            ascii_content = fdp.ConsumeBytesAsString(fdp.ConsumeIntegralInRange<size_t>(0, 3));
            break;
        }
        
        case 1: {
            // Use fuzzer input to construct a JSON-like string
            std::string json_start = "{\"";
            std::string json_middle = "\":\"";
            std::string json_end = "\"}";
            
            if (fdp.remaining_bytes() >= 4) {
                std::string key = fdp.ConsumeRandomLengthString(32);
                std::string value = fdp.ConsumeRandomLengthString(64);
                ascii_content = json_start + key + json_middle + value + json_end;
            } else {
                ascii_content = "{}";
            }
            break;
        }
        
        case 2: {
            // Construct more complex JSON structure from fuzzer input
            std::stringstream json_stream;
            json_stream << "{";
            
            // Add asset object
            json_stream << "\"asset\":{";
            json_stream << "\"version\":\"";
            if (fdp.remaining_bytes() > 0) {
                json_stream << fdp.ConsumeRandomLengthString(16);
            } else {
                json_stream << "2.0";
            }
            json_stream << "\",\"generator\":\"";
            if (fdp.remaining_bytes() > 0) {
                json_stream << fdp.ConsumeRandomLengthString(32);
            } else {
                json_stream << "fuzzer";
            }
            json_stream << "\"}";
            
            // Add optional arrays based on remaining input
            if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
                json_stream << ",\"scenes\":[]";
            }
            if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
                json_stream << ",\"nodes\":[]";
            }
            
            json_stream << "}";
            ascii_content = json_stream.str();
            break;
        }
        
        case 3: {
            // Malformed JSON - generate broken structure
            std::string json_start = "{\"";
            std::string json_middle = "\":\"";
            
            if (fdp.remaining_bytes() >= 2) {
                std::string key = fdp.ConsumeRandomLengthString(16);
                std::string value = fdp.ConsumeRandomLengthString(32);
                ascii_content = json_start + key + json_middle + value;
                // Missing closing braces intentionally
            } else {
                ascii_content = "{";
            }
            break;
        }
        
        case 4: {
            // JSON with content from fuzzer input
            std::string json_start = "{\"asset\":{\"version\":\"2.0\",\"name\":\"";
            if (fdp.remaining_bytes() > 0) {
                ascii_content = json_start + fdp.ConsumeRandomLengthString(100);
            } else {
                ascii_content = json_start;
            }
            ascii_content += "\"}}";
            break;
        }
        
        case 5: {
            // JSON array instead of object (should fail)
            ascii_content = "[";
            size_t array_items = fdp.ConsumeIntegralInRange<size_t>(0, 10);
            for (size_t i = 0; i < array_items && fdp.remaining_bytes() > 0; ++i) {
                if (i > 0) ascii_content += ",";
                ascii_content += std::to_string(fdp.ConsumeIntegral<int>());
            }
            ascii_content += "]";
            break;
        }
        
        case 6: {
            // Very long string (use remaining bytes)
            if (fdp.remaining_bytes() > 0) {
                ascii_content = fdp.ConsumeRemainingBytesAsString();
            } else {
                ascii_content = "{}";
            }
            break;
        }
        
        case 7: {
            // Binary data disguised as text
            size_t binary_size = fdp.ConsumeIntegralInRange<size_t>(0, std::min((size_t)1024, fdp.remaining_bytes()));
            if (binary_size > 0) {
                std::vector<uint8_t> binary_data = fdp.ConsumeBytes<uint8_t>(binary_size);
                ascii_content.assign(reinterpret_cast<const char*>(binary_data.data()), binary_data.size());
            } else {
                ascii_content = "{}";
            }
            break;
        }
    }

    
    // Create loader and model
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    

    switch (strictness_mode) {
        case 0:
            loader.SetParseStrictness(tinygltf::Permissive);
            break;
        case 1:
            loader.SetParseStrictness(tinygltf::Strict);
            break;
        default:
            // Use default
            break;
    }
    
    // Set some loader options based on config
    loader.SetPreserveImageChannels((config_byte & 0x10) != 0);
    loader.SetImagesAsIs((config_byte & 0x20) != 0);
    
    // Try loading with different parameter variations
    bool load_success = false;
    
    // Test 1: Normal LoadASCIIFromString call
    load_success = loader.LoadASCIIFromString(&model, &err, &warn,
                                              ascii_content.c_str(),
                                              static_cast<unsigned int>(ascii_content.size()),
                                              base_dir,
                                              check_sections);
    
    // Clear model and errors for next test
    model = tinygltf::Model();
    err.clear();
    warn.clear();
    
    // Test 2: Call with null error/warn pointers (tests different code path)
    if (fdp.ConsumeBool() && ascii_content.size() > 0) {
        (void)loader.LoadASCIIFromString(&model, nullptr, nullptr,
                                         ascii_content.c_str(),
                                         static_cast<unsigned int>(ascii_content.size()),
                                         base_dir,
                                         check_sections);
    }
    
    // Test 3: If we have remaining bytes, try with modified content
    if (fdp.remaining_bytes() > 0) {
        model = tinygltf::Model();
        err.clear();
        warn.clear();
        
        // Modify the string slightly
        std::string modified_content = ascii_content;
        if (!modified_content.empty() && fdp.remaining_bytes() > 0) {
            // Insert some random bytes at a random position
            std::string insert_str = fdp.ConsumeRandomLengthString(std::min((size_t)64, fdp.remaining_bytes()));
            if (modified_content.size() > 10 && !insert_str.empty()) {
                size_t insert_pos = modified_content.size() / 2;
                modified_content.insert(insert_pos, insert_str);
            }
        }
        
        (void)loader.LoadASCIIFromString(&model, &err, &warn,
                                         modified_content.c_str(),
                                         static_cast<unsigned int>(modified_content.size()),
                                         base_dir,
                                         check_sections);
    }
    
    // Test 4: Edge case - extremely large length value (potential overflow)
    if (fdp.ConsumeBool() && ascii_content.size() > 0) {
        model = tinygltf::Model();
        err.clear();
        warn.clear();
        
        // Use a very large length value (but actual string is shorter)
        unsigned int large_length = static_cast<unsigned int>(
            fdp.ConsumeIntegralInRange<size_t>(ascii_content.size(), ascii_content.size() * 2 + 1000));
        
        (void)loader.LoadASCIIFromString(&model, &err, &warn,
                                         ascii_content.c_str(),
                                         large_length,
                                         base_dir,
                                         check_sections);
    }
    
    // Test 5: Test with empty base_dir but valid JSON
    if (fdp.ConsumeBool() && ascii_content.size() > 4) {
        model = tinygltf::Model();
        err.clear();
        warn.clear();
        
        (void)loader.LoadASCIIFromString(&model, &err, &warn,
                                         ascii_content.c_str(),
                                         static_cast<unsigned int>(ascii_content.size()),
                                         "",
                                         check_sections);
    }
    
    return 0;
}

} // namespace harness_024
