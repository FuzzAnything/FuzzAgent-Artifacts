#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_019 {
/*
 * Fuzzing harness for tinygltf library - harness_019.cpp
 * 
 * This harness specifically targets GLTF scene file writing operations with
 * memory-based loading (different from previous file-based loading harnesses):
 * - Primary: tinygltf::TinyGLTF::WriteGltfSceneToFile (110 undiscovered branches, 0% covered)
 * - Secondary: tinygltf::TinyGLTF::WriteGltfSceneToStream (63 undiscovered branches, partially covered)
 * - Required helpers: tinygltf::TinyGLTF::LoadASCIIFromString, 
 *                     tinygltf::TinyGLTF::LoadBinaryFromMemory
 * - Configuration: tinygltf::TinyGLTF::SetParseStrictness
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Memory-based parsing operations but minimal write testing
 * - harness_001: File-based loading (LoadASCIIFromFile, LoadBinaryFromFile) with write operations
 * - harness_006: WriteGltfSceneToFile with embedded images, focuses on image writing paths
 * - harness_017: Configuration API impact on write operations with file-based loading
 * - harness_018: stb_image loading functions with file-based model loading
 * - harness_019: SPECIFIC FOCUS ON MEMORY-BASED LOADING FOR WRITE OPERATIONS:
 *                * Uses LoadASCIIFromString and LoadBinaryFromMemory (not file-based loading)
 *                * Tests WriteGltfSceneToFile with models loaded from memory
 *                * Tests WriteGltfSceneToStream for additional coverage
 *                * Includes SetParseStrictness configuration testing
 *                * Focus on 110+ undiscovered branches in WriteGltfSceneToFile (0% coverage)
 *                  and 63+ in WriteGltfSceneToStream
 *                * Tests both ASCII and binary loading paths before writing
 * 
 * Invocation sequence based on coverage guidance:
 * 1. Create TinyGLTF instance (constructor)
 * 2. Configure parser strictness (SetParseStrictness)
 * 3. Load a Model using LoadASCIIFromString or LoadBinaryFromMemory with fuzzer data
 * 4. Call WriteGltfSceneToFile with the loaded model and output filename
 * 5. Optionally call WriteGltfSceneToStream for additional coverage
 * 6. Let Model destructor clean up automatically
 * 
 * Differentiation: Focus on loading from string/memory rather than files, which may 
 * exercise different code paths. Previous harnesses used LoadASCIIFromFile/LoadBinaryFromFile.
 * This harness uses LoadASCIIFromString and LoadBinaryFromMemory, which may have 
 * different parsing logic. WriteGltfSceneToFile has 110 undiscovered branches (highest 
 * among uncovered APIs) and is completely untouched (0% coverage) despite previous harnesses.
 * Testing these with different load methods might reach different branches.
 * 
 * Uses FuzzedDataProvider to split input for comprehensive testing of different
 * loading methods, write parameter combinations, and strictness configurations.
 */


// Include FuzzedDataProvider for structured input splitting

// TinyGLTF headers - using standard implementation
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// Helper function to create a temporary output file
static std::string create_output_temp_file(const std::string& extension) {
    char temp_filename[64];
    if (extension == ".glb") {
        strcpy(temp_filename, "/tmp/tinygltf_memwrite_XXXXXX.glb");
    } else {
        strcpy(temp_filename, "/tmp/tinygltf_memwrite_XXXXXX.gltf");
    }
    
    int fd = mkstemps(temp_filename, extension == ".glb" ? 4 : 5);
    if (fd == -1) {
        return "";
    }
    close(fd);
    
    return std::string(temp_filename);
}

// Helper function to consume a valid UTF-8 string from fuzzer input
// Generates ASCII-only strings (0x20-0x7E) which are always valid UTF-8
static std::string consume_valid_utf8_string(FuzzedDataProvider& fdp, size_t max_length) {
    if (fdp.remaining_bytes() == 0 || max_length == 0) {
        return "";
    }
    
    // Determine actual length to consume
    size_t length = fdp.ConsumeIntegralInRange<size_t>(0, std::min(max_length, fdp.remaining_bytes()));
    if (length == 0) {
        return "";
    }
    
    // Consume raw bytes
    std::vector<uint8_t> bytes = fdp.ConsumeBytes<uint8_t>(length);
    
    // Convert to valid UTF-8 (ASCII subset: printable characters 32-126)
    std::string result;
    result.reserve(bytes.size());
    
    for (uint8_t byte : bytes) {
        // Map byte to ASCII printable range (32-126)
        // This ensures valid UTF-8 while preserving some variability
        char c = 32 + (byte % 95);  // 95 printable ASCII chars (32-126)
        result.push_back(c);
    }
    
    return result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need at least 16 bytes for meaningful testing
    // (selector + minimal data for loading + filename)
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume selector bytes to control different aspects of testing
    uint8_t selector1 = fdp.ConsumeIntegral<uint8_t>();
    uint8_t selector2 = fdp.ConsumeIntegral<uint8_t>();
    
    // Use selector bits to control different aspects:
    // selector1 bit 0: ASCII vs Binary for loading
    // selector1 bit 1: Strict vs Permissive parsing
    // selector1 bit 2: Use base_dir or empty for loading
    // selector1 bit 3: Write binary (GLB) vs ASCII (glTF)
    // selector2 bit 0-1: embedImages (00=false, 01=true, 10=random)
    // selector2 bit 2-3: embedBuffers (00=false, 01=true, 10=random)
    // selector2 bit 4: prettyPrint for writing
    
    bool load_ascii = (selector1 & 0x01) == 0;
    bool strict_parsing = (selector1 & 0x02) != 0;
    bool use_base_dir = (selector1 & 0x04) != 0;
    bool write_binary = (selector1 & 0x08) != 0;
    
    bool embed_images = false;
    switch ((selector2 >> 0) & 0x03) {
        case 0: embed_images = false; break;
        case 1: embed_images = true; break;
        case 2: embed_images = (fdp.ConsumeIntegral<uint8_t>() % 2) == 0; break;
        default: embed_images = false; break;
    }
    
    bool embed_buffers = false;
    switch ((selector2 >> 2) & 0x03) {
        case 0: embed_buffers = false; break;
        case 1: embed_buffers = true; break;
        case 2: embed_buffers = (fdp.ConsumeIntegral<uint8_t>() % 2) == 0; break;
        default: embed_buffers = false; break;
    }
    
    bool pretty_print = (selector2 & 0x10) != 0;
    
    // Initialize TinyGLTF context
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Configure parser strictness based on selector
    if (strict_parsing) {
        ctx.SetParseStrictness(tinygltf::ParseStrictness::Strict);
    } else {
        ctx.SetParseStrictness(tinygltf::ParseStrictness::Permissive);
    }
    
    // Determine base directory for loading (if any)
    std::string base_dir = use_base_dir ? "/tmp" : "";
    
    // Load model from memory (either ASCII or Binary)
    bool load_success = false;
    
    if (load_ascii) {
        // ASCII/JSON loading path
        // Need to ensure we have enough data for JSON parsing
        if (fdp.remaining_bytes() < 4) {
            return 0;
        }
        
        // Consume data for JSON string
        size_t json_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), size_t(4096)));
        std::string json_data = fdp.ConsumeBytesAsString(json_size);
        
        // Try to load ASCII from string
        load_success = ctx.LoadASCIIFromString(&model, &err, &warn,
                                               json_data.c_str(),
                                               static_cast<unsigned int>(json_data.size()),
                                               base_dir);
    } else {
        // Binary/GLB loading path
        // Need to ensure we have enough data for binary parsing
        if (fdp.remaining_bytes() < 4) {
            return 0;
        }
        
        // Consume data for binary buffer
        size_t binary_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), size_t(4096)));
        std::vector<uint8_t> binary_data = fdp.ConsumeBytes<uint8_t>(binary_size);
        
        // Try to load binary from memory
        load_success = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                binary_data.data(),
                                                static_cast<unsigned int>(binary_data.size()),
                                                base_dir);
    }
    
    // If loading failed, we might still have partial model data
    // We'll proceed with write testing regardless to exercise error paths
    // in the write functions
    
    // Create temporary output file
    std::string output_ext = write_binary ? ".glb" : ".gltf";
    std::string output_filename = create_output_temp_file(output_ext);
    if (output_filename.empty()) {
        // Couldn't create temp file, skip write testing
        return 0;
    }
    
    // Test WriteGltfSceneToFile - primary target with 110 undiscovered branches
    bool write_file_success = false;
    if (!model.meshes.empty() || !model.nodes.empty() || !model.accessors.empty()) {
        // Only attempt to write if we have some model data
        write_file_success = ctx.WriteGltfSceneToFile(&model, output_filename,
                                                      embed_images, embed_buffers,
                                                      pretty_print, write_binary);
        
        // Clean up the output file if it was created
        unlink(output_filename.c_str());
    }
    
    // Test WriteGltfSceneToStream - secondary target with 63 undiscovered branches
    // Use a small subset of fuzzer input for stream testing if we have enough data
    if (fdp.remaining_bytes() >= 4) {
        // Create a stringstream for output
        std::ostringstream oss;
        
        // Determine if we should test stream writing
        uint8_t stream_selector = fdp.ConsumeIntegral<uint8_t>();
        bool test_stream = (stream_selector % 4) == 0; // 25% chance
        
        if (test_stream && (!model.meshes.empty() || !model.nodes.empty() || !model.accessors.empty())) {
            bool stream_write_binary = (stream_selector & 0x01) != 0;
            bool stream_pretty_print = (stream_selector & 0x02) != 0;
            
            bool stream_success = ctx.WriteGltfSceneToStream(&model, oss,
                                                             stream_pretty_print, 
                                                             stream_write_binary);
            
            // Verify stream actually wrote something (optional)
            std::string stream_output = oss.str();
            (void)stream_output; // Mark as used
        }
    }
    
    // Additional testing: Try with different strictness settings
    // after initial load to test state persistence
    if (fdp.remaining_bytes() >= 1) {
        uint8_t reconfig_selector = fdp.ConsumeIntegral<uint8_t>();
        if ((reconfig_selector & 0x01) != 0) {
            // Toggle strictness
            if (strict_parsing) {
                ctx.SetParseStrictness(tinygltf::ParseStrictness::Permissive);
            } else {
                ctx.SetParseStrictness(tinygltf::ParseStrictness::Strict);
            }
            
            // Create another output file with different name
            std::string output_filename2 = create_output_temp_file(output_ext);
            if (!output_filename2.empty()) {
                if (!model.meshes.empty() || !model.nodes.empty() || !model.accessors.empty()) {
                    bool write_success2 = ctx.WriteGltfSceneToFile(&model, output_filename2,
                                                                   embed_images, embed_buffers,
                                                                   pretty_print, write_binary);
                    (void)write_success2; // Mark as used
                    
                    // Clean up
                    unlink(output_filename2.c_str());
                }
            }
        }
    }
    
    // The results are intentionally ignored as we're testing for crashes,
    // not correctness. Both success and failure paths are valuable for coverage.
    (void)load_success;
    (void)write_file_success;
    
    return 0;
}

} // namespace harness_019
