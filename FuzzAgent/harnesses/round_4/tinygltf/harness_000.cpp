#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_000 {
/*
 * Fuzzing harness for tinygltf library - harness_000.cpp
 * 
 * This harness targets the main API entry points of tinygltf library:
 * - LoadASCIIFromString: Parses glTF JSON data from memory
 * - LoadBinaryFromMemory: Parses GLB binary data from memory
 * 
 * Uses FuzzedDataProvider to split input for comprehensive testing
 * of different parsing paths and API variations.
 */


// Include FuzzedDataProvider for structured input splitting

// TinyGLTF headers - using standard implementation
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need at least 2 bytes for selector and some data
    if (size < 2) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume a selector byte to choose different parsing paths
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Get the remaining data for parsing
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    if (remaining_data.empty()) {
        return 0;
    }
    
    // Initialize TinyGLTF context
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Use selector bits to choose different parsing strategies
    // Bit 0: ASCII vs Binary
    // Bit 1: Use base_dir or empty
    // Bit 2: Use check_sections flag variations
    bool use_ascii = (selector & 0x01) == 0;
    bool use_base_dir = (selector & 0x02) != 0;
    unsigned int check_sections = (selector & 0x04) ? tinygltf::REQUIRE_VERSION : 0;
    
    bool parse_result = false;
    
    if (use_ascii) {
        // ASCII/JSON parsing path
        std::string json_data(reinterpret_cast<const char*>(remaining_data.data()), 
                              remaining_data.size());
        
        std::string base_dir = use_base_dir ? "/tmp" : "";
        
        parse_result = ctx.LoadASCIIFromString(&model, &err, &warn,
                                               json_data.c_str(),
                                               static_cast<unsigned int>(json_data.size()),
                                               base_dir,
                                               check_sections);
    } else {
        // Binary/GLB parsing path
        std::string base_dir = use_base_dir ? "/tmp" : "";
        
        parse_result = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                remaining_data.data(),
                                                static_cast<unsigned int>(remaining_data.size()),
                                                base_dir,
                                                check_sections);
    }
    
    // The result is intentionally ignored as we're testing for crashes,
    // not correctness. Both success and failure paths are valuable for coverage.
    (void)parse_result;
    
    // Additional testing: Try serialization APIs if parsing succeeded
    if (parse_result && !model.meshes.empty()) {
        // Test writing to stream (both ASCII and binary formats)
        bool write_binary = (selector & 0x08) != 0;
        bool pretty_print = (selector & 0x10) != 0;
        
        // Create an output string stream
        std::ostringstream oss;
        
        // Note: WriteGltfSceneToStream is commented out as it may require
        // additional setup or have dependencies. We'll focus on parsing for now.
        // bool write_result = ctx.WriteGltfSceneToStream(&model, oss, pretty_print, write_binary);
        // (void)write_result;
    }
    
    // Test other API variations with a small probability
    if ((selector & 0x20) != 0 && remaining_data.size() > 100) {
        // Try parsing with different strictness settings (Permissive or Strict)
        tinygltf::ParseStrictness strictness = 
            (selector & 0x40) ? tinygltf::Strict : tinygltf::Permissive;
        ctx.SetParseStrictness(strictness);
        
        // Create a new model for this test
        tinygltf::Model model2;
        std::string err2, warn2;
        
        bool result2 = false;
        if (use_ascii) {
            std::string json_data2(reinterpret_cast<const char*>(remaining_data.data()), 
                                   std::min(remaining_data.size(), size_t(1000)));
            result2 = ctx.LoadASCIIFromString(&model2, &err2, &warn2,
                                              json_data2.c_str(),
                                              static_cast<unsigned int>(json_data2.size()),
                                              "",
                                              check_sections);
        } else {
            std::vector<uint8_t> small_data(remaining_data.begin(), 
                                           remaining_data.begin() + std::min(remaining_data.size(), size_t(1000)));
            result2 = ctx.LoadBinaryFromMemory(&model2, &err2, &warn2,
                                               small_data.data(),
                                               static_cast<unsigned int>(small_data.size()),
                                               "",
                                               check_sections);
        }
        (void)result2;
    }
    
    return 0;
}

} // namespace harness_000
