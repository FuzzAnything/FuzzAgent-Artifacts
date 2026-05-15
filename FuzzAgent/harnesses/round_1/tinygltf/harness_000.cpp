#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace harness_000 {
/*
 * Fuzzing harness for tinygltf library (GLTF 2.0 parser)
 * 
 * This harness tests the core loading functions of the tinygltf library:
 * 1. LoadASCIIFromString - JSON glTF parsing
 * 2. LoadBinaryFromMemory - GLB binary parsing
 * 
 * The harness uses FuzzedDataProvider to split input into multiple parts
 * for comprehensive testing of different parsing paths.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful testing
    if (size < 8) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume selector byte to choose which parsing path to test
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume parsing options
    uint8_t check_sections = fdp.ConsumeIntegral<uint8_t>();
    bool store_original_json = fdp.ConsumeBool();
    
    // Consume remaining bytes as the glTF data to parse
    std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    if (gltf_data.empty()) return 0;
    
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    // Configure parsing options
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
    
    // Test different parsing paths based on selector
    switch (selector % 3) {
        case 0: {
            // Test LoadASCIIFromString - JSON parsing
            // Convert data to string for JSON parsing
            std::string json_str(reinterpret_cast<const char*>(gltf_data.data()), 
                                 gltf_data.size());
            bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                               json_str.c_str(), 
                                               static_cast<unsigned int>(gltf_data.size()),
                                               /* base_dir */ "",
                                               check_sections);
            (void)ret; // Result intentionally ignored to allow invalid inputs
            break;
        }
        
        case 1: {
            // Test LoadBinaryFromMemory - GLB parsing
            bool ret = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                gltf_data.data(),
                                                static_cast<unsigned int>(gltf_data.size()),
                                                /* base_dir */ "",
                                                check_sections);
            (void)ret;
            break;
        }
        
        case 2: {
            // Test both ASCII and binary parsing in sequence
            std::string json_str(reinterpret_cast<const char*>(gltf_data.data()), 
                                 gltf_data.size());
            
            // First try ASCII parsing
            bool ascii_ret = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                                    json_str.c_str(), 
                                                    static_cast<unsigned int>(gltf_data.size()),
                                                    /* base_dir */ "",
                                                    check_sections);
            (void)ascii_ret;
            
            // Clear model and errors for next test
            model = tinygltf::Model();
            err.clear();
            warn.clear();
            
            // Then try binary parsing
            bool binary_ret = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                      gltf_data.data(),
                                                      static_cast<unsigned int>(gltf_data.size()),
                                                      /* base_dir */ "",
                                                      check_sections);
            (void)binary_ret;
            break;
        }
    }
    
    return 0;
}

} // namespace harness_000
