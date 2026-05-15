#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_002 {
// Harness 002: GLTF file I/O operations fuzzer for tinygltf
// Targets file-based loading and writing APIs: LoadBinaryFromFile, LoadASCIIFromFile, WriteGltfSceneToFile
// Implements Load -> Write -> Load lifecycle sequence as specified in coverage guidance
// Uses FuzzedDataProvider for structured input splitting


// Include FuzzedDataProvider for structured input splitting

// Define implementations for stb_image and tinygltf
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// Helper function to create a deterministic temporary filename based on input
std::string create_temp_filename(uint32_t seed, const std::string& suffix) {
    // Use deterministic hash-based approach for fuzzing reproducibility
    uint32_t hash = seed * 0x5bd1e995;
    hash ^= hash >> 15;
    hash *= 0x5bd1e995;
    
    std::string filename = "/tmp/tinygltf_fuzz_" + std::to_string(hash) + suffix;
    return filename;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create TinyGLTF context and model objects
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Consume operation selector (0-3)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume a filename from input (max 128 chars)
    std::string filename = fdp.ConsumeRandomLengthString(128);
    if (filename.empty()) {
        filename = "test.gltf"; // Fallback name
    }
    
    // Consume seeds for deterministic temporary filenames
    uint32_t seed1 = fdp.ConsumeIntegral<uint32_t>();
    uint32_t seed2 = fdp.ConsumeIntegral<uint32_t>();
    
    // Create temporary file paths
    std::string temp_file1 = create_temp_filename(seed1, ".gltf");
    std::string temp_file2 = create_temp_filename(seed2, ".glb");
    // Operation 0: LoadASCIIFromFile -> WriteGltfSceneToFile -> LoadBinaryFromFile
    if (operation == 0) {
        // First, create some GLTF content from remaining input
        std::string gltf_content = fdp.ConsumeRemainingBytesAsString();
        if (gltf_content.empty()) return 0;
        
        // Write GLTF content to first temporary file
        {
            std::ofstream out_file(temp_file1);
            if (!out_file) return 0;
            out_file.write(gltf_content.c_str(), gltf_content.size());
            out_file.close();
        }
        
        // Step 1: Load from ASCII file
        bool load_ascii_result = ctx.LoadASCIIFromFile(&model, &err, &warn, temp_file1);
        (void)load_ascii_result; // Ignore return value for fuzzing
        
        // Step 2: Write to second file (embed images and buffers, pretty print)
        if (!model.accessors.empty() || !model.meshes.empty()) {
            bool write_result = ctx.WriteGltfSceneToFile(&model, temp_file2, 
                                                        fdp.ConsumeBool(),  // embedImages
                                                        fdp.ConsumeBool(),  // embedBuffers
                                                        fdp.ConsumeBool(),  // prettyPrint
                                                        fdp.ConsumeBool()); // writeBinary
            (void)write_result;
            
            // Step 3: Load back from binary file (if written as binary)
            // Check if file exists and has content
            std::ifstream test_file(temp_file2);
            if (test_file.good()) {
                // Create a new model for loading
                tinygltf::Model model2;
                std::string err2, warn2;
                
                // Try to load as binary
                bool load_binary_result = ctx.LoadBinaryFromFile(&model2, &err2, &warn2, temp_file2);
                (void)load_binary_result;
            }
        }
    }
    // Operation 1: LoadBinaryFromFile with various check_sections flags
    else if (operation == 1) {
        // Create binary GLTF content from input
        std::vector<uint8_t> binary_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (binary_data.empty()) return 0;
        
        // Write binary data to temporary file
        {
            std::ofstream out_file(temp_file1, std::ios::binary);
            if (!out_file) return 0;
            out_file.write(reinterpret_cast<const char*>(binary_data.data()), binary_data.size());
            out_file.close();
        }
        
        // Consume check_sections flag from remaining input
        unsigned int check_sections = fdp.ConsumeIntegral<uint8_t>() & 0x7F;
        
        // Load binary from file with specified check_sections
        bool load_result = ctx.LoadBinaryFromFile(&model, &err, &warn, temp_file1, check_sections);
        (void)load_result;
        
        // Access model components to exercise more code paths
        if (!model.buffers.empty()) {
            (void)model.buffers[0].name;
            (void)model.buffers[0].data.size();
        }
        if (!model.images.empty()) {
            (void)model.images[0].name;
            (void)model.images[0].width;
            (void)model.images[0].height;
        }
    }
    // Operation 2: WriteGltfSceneToFile with different configuration combinations
    else if (operation == 2) {
        // First create a simple model from input
        std::string json_content = fdp.ConsumeRemainingBytesAsString();
        if (json_content.empty()) return 0;
        
        // Load model from string first
        bool load_result = ctx.LoadASCIIFromString(&model, &err, &warn, 
                                                  json_content.c_str(),
                                                  static_cast<unsigned int>(json_content.size()),
                                                  "");
        (void)load_result;
        
        // Generate all possible combinations of WriteGltfSceneToFile parameters
        // from fuzzer input
        bool embedImages = fdp.ConsumeBool();
        bool embedBuffers = fdp.ConsumeBool();
        bool prettyPrint = fdp.ConsumeBool();
        bool writeBinary = fdp.ConsumeBool();
        
        // Write model to file with various configurations
        bool write_result = ctx.WriteGltfSceneToFile(&model, temp_file1,
                                                    embedImages, embedBuffers,
                                                    prettyPrint, writeBinary);
        (void)write_result;
        
        // Try to load the written file back
        if (writeBinary) {
            // Try loading as binary
            tinygltf::Model model2;
            std::string err2, warn2;
            bool load_binary_result = ctx.LoadBinaryFromFile(&model2, &err2, &warn2, temp_file1);
            (void)load_binary_result;
        } else {
            // Try loading as ASCII
            tinygltf::Model model2;
            std::string err2, warn2;
            bool load_ascii_result = ctx.LoadASCIIFromFile(&model2, &err2, &warn2, temp_file1);
            (void)load_ascii_result;
        }
    }
    // Operation 3: Comprehensive file I/O lifecycle test
    else {
        // Create GLTF content from input
        std::string gltf_content = fdp.ConsumeRemainingBytesAsString();
        if (gltf_content.empty()) return 0;
        
        // Write to first temporary file
        {
            std::ofstream out_file(temp_file1);
            if (!out_file) return 0;
            out_file.write(gltf_content.c_str(), gltf_content.size());
            out_file.close();
        }
        
        // Test 1: LoadASCIIFromFile
        tinygltf::Model model1;
        bool load_ascii_result = ctx.LoadASCIIFromFile(&model1, &err, &warn, temp_file1);
        (void)load_ascii_result;
        
        // Test 2: Write to second file (if model has content)
        if (!model1.accessors.empty() || !model1.meshes.empty()) {
            bool write_result = ctx.WriteGltfSceneToFile(&model1, temp_file2, 
                                                        true, true, true, false);
            (void)write_result;
            
            // Test 3: LoadBinaryFromFile (if file was created)
            std::ifstream test_file(temp_file2);
            if (test_file.good()) {
                tinygltf::Model model2;
                std::string err2, warn2;
                bool load_binary_result = ctx.LoadBinaryFromFile(&model2, &err2, &warn2, temp_file2);
                (void)load_binary_result;
                
                // Test 4: Write again with different parameters
                if (!model2.accessors.empty()) {
                    bool write_result2 = ctx.WriteGltfSceneToFile(&model2, temp_file1,
                                                                 false, false, false, true);
                    (void)write_result2;
                }
            }
        }
    }
    
    // Clean up temporary files
    std::remove(temp_file1.c_str());
    std::remove(temp_file2.c_str());
    
    return 0;
}

} // namespace harness_002
