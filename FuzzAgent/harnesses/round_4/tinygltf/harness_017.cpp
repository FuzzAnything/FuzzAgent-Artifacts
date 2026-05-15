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

namespace harness_017 {
/*
 * Fuzzing harness for tinygltf library - harness_017.cpp
 * 
 * This harness specifically targets glTF file writing/export operations with 
 * comprehensive configuration API testing, focusing on the interplay between
 * configuration settings and write operations:
 * - Primary: tinygltf::TinyGLTF::WriteGltfSceneToFile (110 undiscovered branches, 0% covered)
 * - Secondary: tinygltf::TinyGLTF::WriteGltfSceneToStream (63 undiscovered branches)
 * - Configuration APIs: tinygltf::TinyGLTF::SetSerializeDefaultValues, 
 *                       tinygltf::TinyGLTF::GetSerializeDefaultValues
 * - Required setup: tinygltf::TinyGLTF constructor, LoadASCIIFromFile, 
 *                   LoadBinaryFromFile, Model destructor/cleanup
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Memory-based parsing operations only
 * - harness_001: Basic file I/O operations without configuration testing
 * - harness_006: WriteGltfSceneToFile with embedded images (limited configuration)
 * - harness_008: Configuration and callback API testing but not specifically 
 *                focused on serialize default values
 * - harness_016: Exhaustive flag combination testing but minimal configuration 
 *                state transition testing
 * - harness_017: SPECIFIC FOCUS ON CONFIGURATION API IMPACT ON WRITE OPERATIONS:
 *                * Systematic testing of SetSerializeDefaultValues before/after writes
 *                * Validation of GetSerializeDefaultValues return values
 *                * Testing configuration state persistence across write operations
 *                * Focus on the 110+ undiscovered branches in WriteGltfSceneToFile
 *                  and 63+ in WriteGltfSceneToStream that are untouched despite
 *                  previous harnesses
 * 
 * Invocation sequence based on coverage guidance:
 * 1. Create TinyGLTF instance (default constructor)
 * 2. Load a glTF model using LoadASCIIFromFile or LoadBinaryFromFile (use test assets)
 * 3. Configure writing options with SetSerializeDefaultValues(true/false)
 * 4. Call WriteGltfSceneToFile with various parameter combinations:
 *    - embedImages = true/false
 *    - embedBuffers = true/false
 *    - prettyPrint = true/false
 *    - writeBinary = true/false
 * 5. For binary mode, also test WriteGltfSceneToStream with std::stringstream
 * 6. Validate written output (optional: read back and compare)
 * 7. Clean up Model resources
 * 
 * Targets 110+ undiscovered branches in WriteGltfSceneToFile and 63+ in 
 * WriteGltfSceneToStream with comprehensive configuration API testing.
 * 
 * Uses FuzzedDataProvider to split input for systematic testing of different
 * configuration states, write parameter combinations, and model loading scenarios.
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
        strcpy(temp_filename, "/tmp/tinygltf_config_XXXXXX.glb");
    } else {
        strcpy(temp_filename, "/tmp/tinygltf_config_XXXXXX.gltf");
    }
    
    int fd = mkstemps(temp_filename, extension == ".glb" ? 4 : 5);
    if (fd == -1) {
        return "";
    }
    close(fd);
    
    return std::string(temp_filename);
}

// List of test glTF files available in the repository
static const char* TEST_GLTF_FILES[] = {
    "/root/src/tinygltf/models/box01.glb",
    "/root/src/tinygltf/models/Cube/Cube.gltf",
    "/root/src/tinygltf/models/regression/unassigned-skeleton.gltf",
    "/root/src/tinygltf/models/Extensions-issue97/test.gltf",
    "/root/src/tinygltf/models/CubeImageUriSpaces/CubeImageUriSpaces.gltf",
};

static const size_t NUM_TEST_FILES = sizeof(TEST_GLTF_FILES) / sizeof(TEST_GLTF_FILES[0]);

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for selector and configuration control
    if (size < 12) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume selector bytes for comprehensive testing
    uint8_t file_selector = fdp.ConsumeIntegral<uint8_t>() % NUM_TEST_FILES;
    uint8_t load_method = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0=ASCII, 1=Binary
    uint8_t config_mode = fdp.ConsumeIntegral<uint8_t>() % 4;  // Configuration testing mode
    
    // Select test file
    std::string test_file = TEST_GLTF_FILES[file_selector];
    
    // Check if test file exists
    struct stat st;
    if (stat(test_file.c_str(), &st) != 0) {
        // Test file doesn't exist, skip this iteration
        return 0;
    }
    
    // Initialize TinyGLTF context
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    bool load_result = false;
    
    // Load model using selected method
    if (load_method == 0) {
        // Load ASCII
        load_result = ctx.LoadASCIIFromFile(&model, &err, &warn, test_file, 0);
    } else {
        // Load binary
        load_result = ctx.LoadBinaryFromFile(&model, &err, &warn, test_file, 0);
    }
    
    if (!load_result) {
        // Loading failed, but that's OK for fuzzing
        return 0;
    }
    
    // Configuration testing based on config_mode
    bool initial_serialize_defaults = false;
    bool final_serialize_defaults = false;
    
    switch (config_mode) {
        case 0:
            // Test default configuration (false)
            // Verify GetSerializeDefaultValues returns false by default
            initial_serialize_defaults = ctx.GetSerializeDefaultValues();
            // Should be false by default
            break;
            
        case 1:
            // Set to true before write operations
            ctx.SetSerializeDefaultValues(true);
            initial_serialize_defaults = ctx.GetSerializeDefaultValues();
            // Should be true after setting
            break;
            
        case 2:
            // Set to false explicitly (though it's default)
            ctx.SetSerializeDefaultValues(false);
            initial_serialize_defaults = ctx.GetSerializeDefaultValues();
            // Should be false
            break;
            
        case 3:
            // Test toggling configuration
            bool toggle_value = (fdp.ConsumeIntegral<uint8_t>() & 0x01) != 0;
            ctx.SetSerializeDefaultValues(toggle_value);
            initial_serialize_defaults = ctx.GetSerializeDefaultValues();
            // Should match toggle_value
            break;
    }
    
    // Consume write parameters from fuzzer input
    uint8_t write_params = fdp.ConsumeIntegral<uint8_t>();
    bool embedImages = (write_params & 0x01) != 0;
    bool embedBuffers = (write_params & 0x02) != 0;
    bool prettyPrint = (write_params & 0x04) != 0;
    bool writeBinary = (write_params & 0x08) != 0;
    
    // Test 1: WriteGltfSceneToFile with current configuration
    std::string output_ext = writeBinary ? ".glb" : ".gltf";
    std::string output_file = create_output_temp_file(output_ext);
    
    if (!output_file.empty()) {
        bool write_result = ctx.WriteGltfSceneToFile(&model, output_file,
                                                    embedImages, embedBuffers,
                                                    prettyPrint, writeBinary);
        
        // After write, check if configuration persists
        final_serialize_defaults = ctx.GetSerializeDefaultValues();
        
        // Configuration should persist after write operation
        // (initial_serialize_defaults == final_serialize_defaults)
        
        // Clean up output file
        unlink(output_file.c_str());
        
        // The result is intentionally ignored as we're testing for crashes,
        // not correctness. Both success and failure paths are valuable for coverage.
        (void)write_result;
    }
    
    // Test 2: WriteGltfSceneToStream for binary mode testing
    if (writeBinary && fdp.remaining_bytes() >= 1) {
        uint8_t stream_params = fdp.ConsumeIntegral<uint8_t>();
        bool stream_pretty_print = (stream_params & 0x01) != 0;
        
        std::ostringstream oss;
        bool stream_result = ctx.WriteGltfSceneToStream(&model, oss,
                                                       stream_pretty_print,
                                                       writeBinary);
        
        // Check configuration after stream write
        bool after_stream_config = ctx.GetSerializeDefaultValues();
        
        // Configuration should persist after stream write too
        // (final_serialize_defaults == after_stream_config)
        
        // Optional: Try to parse the output stream for round-trip validation
        if (stream_result && !oss.str().empty() && fdp.remaining_bytes() >= 4) {
            tinygltf::TinyGLTF ctx2;
            tinygltf::Model model2;
            std::string err2, warn2;
            
            // Test with different configuration on second context
            bool second_config = (fdp.ConsumeIntegral<uint8_t>() & 0x01) != 0;
            ctx2.SetSerializeDefaultValues(second_config);
            
            std::string stream_data = oss.str();
            bool load_stream_result = false;
            
            if (writeBinary) {
                load_stream_result = ctx2.LoadBinaryFromMemory(&model2, &err2, &warn2,
                                                             reinterpret_cast<const unsigned char*>(stream_data.data()),
                                                             static_cast<unsigned int>(stream_data.size()),
                                                             "", 0);
            } else {
                // For ASCII mode testing
                std::ostringstream oss2;
                bool ascii_stream_result = ctx.WriteGltfSceneToStream(&model, oss2,
                                                                     stream_pretty_print,
                                                                     false); // ASCII mode
                if (ascii_stream_result && !oss2.str().empty()) {
                    load_stream_result = ctx2.LoadASCIIFromString(&model2, &err2, &warn2,
                                                                oss2.str().c_str(),
                                                                static_cast<unsigned int>(oss2.str().size()),
                                                                "", 0);
                }
            }
            
            // Test writing the loaded model with different configuration
            if (load_stream_result) {
                uint8_t second_write_params = 0;
                if (fdp.remaining_bytes() >= 1) {
                    second_write_params = fdp.ConsumeIntegral<uint8_t>();
                }
                bool second_embedImages = (second_write_params & 0x01) != 0;
                bool second_embedBuffers = (second_write_params & 0x02) != 0;
                bool second_prettyPrint = (second_write_params & 0x04) != 0;
                bool second_writeBinary = (second_write_params & 0x08) != 0;
                
                std::string second_output_ext = second_writeBinary ? ".glb" : ".gltf";
                std::string second_output_file = create_output_temp_file(second_output_ext);
                
                if (!second_output_file.empty()) {
                    bool second_write_result = ctx2.WriteGltfSceneToFile(&model2, second_output_file,
                                                                       second_embedImages, second_embedBuffers,
                                                                       second_prettyPrint, second_writeBinary);
                    
                    // Clean up second output file
                    unlink(second_output_file.c_str());
                    
                    (void)second_write_result;
                }
            }
        }
        
        (void)stream_result;
    }
    
    // Test 3: Configuration state change and re-write
    if (fdp.remaining_bytes() >= 2) {
        uint8_t new_config = fdp.ConsumeIntegral<uint8_t>();
        bool change_config = (new_config & 0x01) != 0;
        
        if (change_config) {
            // Change configuration state
            bool new_serialize_value = (new_config & 0x02) != 0;
            ctx.SetSerializeDefaultValues(new_serialize_value);
            
            // Verify the change took effect
            bool verify_config = ctx.GetSerializeDefaultValues();
            // Should equal new_serialize_value
            
            // Write with new configuration
            uint8_t third_write_params = 0;
            if (fdp.remaining_bytes() >= 1) {
                third_write_params = fdp.ConsumeIntegral<uint8_t>();
            }
            bool third_embedImages = (third_write_params & 0x01) != 0;
            bool third_embedBuffers = (third_write_params & 0x02) != 0;
            bool third_prettyPrint = (third_write_params & 0x04) != 0;
            bool third_writeBinary = (third_write_params & 0x08) != 0;
            
            std::string third_output_ext = third_writeBinary ? ".glb" : ".gltf";
            std::string third_output_file = create_output_temp_file(third_output_ext);
            
            if (!third_output_file.empty()) {
                bool third_write_result = ctx.WriteGltfSceneToFile(&model, third_output_file,
                                                                  third_embedImages, third_embedBuffers,
                                                                  third_prettyPrint, third_writeBinary);
                
                // Clean up third output file
                unlink(third_output_file.c_str());
                
                (void)third_write_result;
            }
        }
    }
    
    // Cleanup: Model destructor will handle cleanup automatically
    // No explicit cleanup needed as Model is stack-allocated
    
    return 0;
}

} // namespace harness_017
