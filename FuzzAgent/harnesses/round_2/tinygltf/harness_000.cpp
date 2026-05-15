#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_000 {
// Harness 000: Comprehensive glTF loading fuzzer for tinygltf
// Targets main API entry points for glTF loading and parsing
// Uses FuzzedDataProvider for structured input splitting


// Include FuzzedDataProvider for structured input splitting

// Define implementations for stb_image and tinygltf
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 2) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume selector byte to choose different fuzzing paths
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Create model and context
    tinygltf::Model model;
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    // Path 0: LoadASCIIFromString (JSON parsing)
    if ((selector & 0x03) == 0) {
        // Use remaining bytes as JSON string
        std::string json_str = fdp.ConsumeRemainingBytesAsString();
        
        if (!json_str.empty()) {
            bool ret = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                               json_str.c_str(), 
                                               static_cast<unsigned int>(json_str.size()),
                                               /* base_dir */ "");
            (void)ret; // Ignore return value, we're fuzzing
            
            // Try to access some model data to exercise more code paths
            if (!model.accessors.empty()) {
                // Access first accessor if available
                (void)model.accessors[0].name;
            }
            if (!model.meshes.empty()) {
                // Access first mesh if available
                (void)model.meshes[0].name;
            }
        }
    }
    // Path 1: LoadBinaryFromMemory (GLB parsing)
    else if ((selector & 0x03) == 1) {
        // Use remaining bytes as binary GLB data
        std::vector<uint8_t> binary_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (!binary_data.empty()) {
            bool ret = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                binary_data.data(),
                                                static_cast<unsigned int>(binary_data.size()),
                                                /* base_dir */ "");
            (void)ret;
            
            // Try to access some model data
            if (!model.buffers.empty()) {
                (void)model.buffers[0].name;
            }
            if (!model.images.empty()) {
                (void)model.images[0].name;
            }
        }
    }
    // Path 2: LoadASCIIFromString with different check_sections flags
    else if ((selector & 0x03) == 2) {
        // Consume check_sections flag from input
        unsigned int check_sections = fdp.ConsumeIntegral<uint8_t>() & 0x7F;
        
        std::string json_str = fdp.ConsumeRemainingBytesAsString();
        
        if (!json_str.empty()) {
            bool ret = ctx.LoadASCIIFromString(&model, &err, &warn,
                                               json_str.c_str(),
                                               static_cast<unsigned int>(json_str.size()),
                                               /* base_dir */ "",
                                               check_sections);
            (void)ret;
            
            // Access asset info
            (void)model.asset.version;
            (void)model.asset.generator;
        }
    }
    // Path 3: LoadBinaryFromMemory with different check_sections flags
    else {
        // Consume check_sections flag from input
        unsigned int check_sections = fdp.ConsumeIntegral<uint8_t>() & 0x7F;
        
        std::vector<uint8_t> binary_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (!binary_data.empty()) {
            bool ret = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                binary_data.data(),
                                                static_cast<unsigned int>(binary_data.size()),
                                                /* base_dir */ "",
                                                check_sections);
            (void)ret;
            
            // Access extensions
            if (!model.extensionsUsed.empty()) {
                (void)model.extensionsUsed[0];
            }
            if (!model.extensionsRequired.empty()) {
                (void)model.extensionsRequired[0];
            }
        }
    }
    
    return 0;
}

} // namespace harness_000
