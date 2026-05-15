#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_000 {
/*
 * Fuzzing harness for tinygltf library - targeting main parsing APIs
 * This harness tests both glTF JSON and GLB binary parsing with various options
 * Uses FuzzedDataProvider for structured input splitting
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;  // Need minimum data for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Consume selector byte to choose parsing path
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume parsing options
    bool use_custom_json = (selector & 0x01) != 0;
    bool check_version = (selector & 0x02) != 0;
    bool check_scene = (selector & 0x04) != 0;
    bool check_all = (selector & 0x08) != 0;
    
    // Build check_sections flag based on consumed bits
    unsigned int check_sections = 0;
    if (check_version) check_sections |= tinygltf::REQUIRE_VERSION;
    if (check_scene) check_sections |= tinygltf::REQUIRE_SCENE;
    if (check_all) check_sections = tinygltf::REQUIRE_ALL;
    
    // Choose parsing method: 0=ASCII, 1=Binary, 2=Auto detect
    uint8_t parse_method = selector % 3;
    
    // Get the remaining data as payload
    std::vector<uint8_t> payload = fdp.ConsumeRemainingBytes<uint8_t>();
    if (payload.empty()) {
        return 0;
    }

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    // Set some parsing options based on consumed data
    loader.SetPreserveImageChannels((selector & 0x10) != 0);
    loader.SetImagesAsIs((selector & 0x20) != 0);
    
    bool ret = false;
    
    switch (parse_method) {
        case 0:  // ASCII/JSON parsing
            if (use_custom_json) {
                // For custom JSON backend, we need to define TINYGLTF_USE_CUSTOM_JSON
                // but for simplicity we'll use standard path
                ret = loader.LoadASCIIFromString(&model, &err, &warn,
                                                 reinterpret_cast<const char*>(payload.data()),
                                                 static_cast<unsigned int>(payload.size()),
                                                 "", check_sections);
            } else {
                ret = loader.LoadASCIIFromString(&model, &err, &warn,
                                                 reinterpret_cast<const char*>(payload.data()),
                                                 static_cast<unsigned int>(payload.size()),
                                                 "", check_sections);
            }
            break;
            
        case 1:  // Binary/GLB parsing
            ret = loader.LoadBinaryFromMemory(&model, &err, &warn,
                                             payload.data(),
                                             static_cast<unsigned int>(payload.size()),
                                             "", check_sections);
            break;
            
        case 2:  // Test both paths
            // Try ASCII first
            (void)loader.LoadASCIIFromString(&model, &err, &warn,
                                            reinterpret_cast<const char*>(payload.data()),
                                            static_cast<unsigned int>(payload.size()),
                                            "", check_sections);
            
            // Clear model and try binary
            model = tinygltf::Model();
            (void)loader.LoadBinaryFromMemory(&model, &err, &warn,
                                             payload.data(),
                                             static_cast<unsigned int>(payload.size()),
                                             "", check_sections);
            ret = true;  // Don't care about actual success
            break;
    }
    
    // The return value is ignored - we're testing for crashes, not correctness
    (void)ret;
    
    return 0;
}

} // namespace harness_000
