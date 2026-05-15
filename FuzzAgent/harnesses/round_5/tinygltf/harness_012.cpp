#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_012 {
/*
 * Fuzzing harness for tiny_gltf.h module targeting lowest coverage APIs (26.35% line coverage).
 * Specifically targets LoadASCIIFromString (62 undiscovered branches, 1% coverage) 
 * and WriteGltfSceneToStream (55 undiscovered branches, 50% coverage) APIs.
 * 
 * This harness implements a complete ASCII GLTF loading and stream writing workflow:
 * 1) Create TinyGLTF instance
 * 2) Create Model instance  
 * 3) Call LoadASCIIFromString with fuzzed input data (ASCII GLTF content)
 * 4) If load succeeds, create std::stringstream output stream
 * 5) Call WriteGltfSceneToStream to write loaded model
 * 6) Include error handling
 * 7) Clean up
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp tests LoadASCIIFromString but only as part of a switch statement
 * - harness_001.cpp tests WriteGltfSceneToStream but focuses on file operations
 * - This harness focuses specifically on the ASCII string loading → stream writing workflow
 *   as a continuous sequence, maximizing coverage of these two low-coverage APIs
 * 
 * Uses FuzzedDataProvider for structured input splitting to generate:
 * - ASCII GLTF content (main payload)
 * - Loader options (check_sections flags)
 * - Writer options (prettyPrint, writeBinary)
 * - Additional test variations
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size: need at least enough for flags and a small ASCII string
    if (size < 8) {
        return 0;  // Insufficient input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Consume loader configuration from fuzzed input
    uint8_t config_byte = fdp.ConsumeIntegral<uint8_t>();
    
    // Parse check_sections flags from consumed data
    unsigned int check_sections = 0;
    bool check_version = (config_byte & 0x01) != 0;
    bool check_scene = (config_byte & 0x02) != 0;
    bool check_all = (config_byte & 0x04) != 0;
    
    if (check_version) check_sections |= tinygltf::REQUIRE_VERSION;
    if (check_scene) check_sections |= tinygltf::REQUIRE_SCENE;
    if (check_all) check_sections = tinygltf::REQUIRE_ALL;
    
    // Consume loader options
    bool preserve_channels = (config_byte & 0x08) != 0;
    bool images_as_is = (config_byte & 0x10) != 0;
    bool serialize_defaults = (config_byte & 0x20) != 0;
    bool store_original_json = (config_byte & 0x40) != 0;
    
    // Consume writer options
    bool pretty_print = (config_byte & 0x80) != 0;
    uint8_t writer_config = fdp.ConsumeIntegral<uint8_t>();
    bool write_binary = (writer_config & 0x01) != 0;
    
    // Choose base_dir strategy (0 = empty, 1 = simple path, 2 = fuzzed path)
    uint8_t base_dir_type = writer_config % 3;
    std::string base_dir;
    
    switch (base_dir_type) {
        case 0:
            base_dir = "";  // Empty base_dir
            break;
        case 1:
            base_dir = "/tmp";  // Simple path
            break;
        case 2:
            // Consume a random length string for base_dir
            if (fdp.remaining_bytes() > 0) {
                base_dir = fdp.ConsumeRandomLengthString(64);
            } else {
                base_dir = "";
            }
            break;
    }
    
    // Step 2: Get the remaining data as ASCII GLTF content
    // Use all remaining bytes as the ASCII GLTF string to parse
    std::string ascii_content = fdp.ConsumeRemainingBytesAsString();
    if (ascii_content.empty()) {
        return 0;  // No content to parse
    }
    
    // Step 3: Create TinyGLTF and Model instances
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    // Step 4: Configure loader based on consumed options
    loader.SetPreserveImageChannels(preserve_channels);
    loader.SetImagesAsIs(images_as_is);
    loader.SetSerializeDefaultValues(serialize_defaults);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
    
    // Step 5: Call LoadASCIIFromString with fuzzed input data
    bool load_success = loader.LoadASCIIFromString(
        &model, 
        &err, 
        &warn,
        ascii_content.c_str(),
        static_cast<unsigned int>(ascii_content.size()),
        base_dir,
        check_sections
    );
    
    // Step 6: If load succeeded, test WriteGltfSceneToStream
    if (load_success) {
        // Create output stream for writing
        std::stringstream output_stream;
        
        // Call WriteGltfSceneToStream to write the loaded model
        bool write_success = loader.WriteGltfSceneToStream(
            &model,
            output_stream,
            pretty_print,
            write_binary
        );
        
        // If writing succeeded, we can optionally verify the output
        if (write_success) {
            std::string output_content = output_stream.str();
            // Basic validation: output shouldn't be empty if writing succeeded
            if (output_content.empty()) {
                // This is unexpected but not an error - just note it
            }
        }
        
        // Test additional write variations with different parameters
        // Try all combinations of prettyPrint and writeBinary
        for (int i = 0; i < 4; i++) {
            bool test_pretty = (i & 0x01) != 0;
            bool test_binary = (i & 0x02) != 0;
            
            std::stringstream test_stream;
            (void)loader.WriteGltfSceneToStream(
                &model,
                test_stream,
                test_pretty,
                test_binary
            );
        }
    } else {
        // Load failed - test error paths
        // Try loading with different check_sections to explore error handling
        if (check_sections != tinygltf::REQUIRE_ALL) {
            // Try with REQUIRE_ALL to see if it produces different errors
            (void)loader.LoadASCIIFromString(
                &model,
                &err,
                &warn,
                ascii_content.c_str(),
                static_cast<unsigned int>(ascii_content.size()),
                base_dir,
                tinygltf::REQUIRE_ALL
            );
        }
        
        // Also test with empty base_dir if we used a non-empty one
        if (!base_dir.empty()) {
            (void)loader.LoadASCIIFromString(
                &model,
                &err,
                &warn,
                ascii_content.c_str(),
                static_cast<unsigned int>(ascii_content.size()),
                "",
                check_sections
            );
        }
    }
    
    // Step 7: Test edge cases with modified input
    
    // Test with extremely short strings (edge cases for parser)
    if (ascii_content.size() > 0) {
        tinygltf::Model edge_model;
        std::string edge_err;
        std::string edge_warn;
        
        // Test single character
        (void)loader.LoadASCIIFromString(
            &edge_model,
            &edge_err,
            &edge_warn,
            "x",
            1,
            "",
            check_sections
        );
        
        // Test with just braces (minimal JSON)
        (void)loader.LoadASCIIFromString(
            &edge_model,
            &edge_err,
            &edge_warn,
            "{}",
            2,
            "",
            check_sections
        );
        
        // Test with valid JSON but not GLTF
        (void)loader.LoadASCIIFromString(
            &edge_model,
            &edge_err,
            &edge_warn,
            "{\"test\": 123}",
            13,
            "",
            check_sections
        );
    }
    
    // Step 8: Test WriteGltfSceneToStream with empty model (edge case)
    if (!load_success) {
        tinygltf::Model empty_model;
        std::stringstream empty_stream;
        
        // This should fail or produce minimal output
        (void)loader.WriteGltfSceneToStream(
            &empty_model,
            empty_stream,
            pretty_print,
            write_binary
        );
    }
    
    // Step 9: Test with null model pointer (should crash but we want to test error paths)
    // Note: We don't actually pass null as it would crash - instead test edge cases
    
    return 0;  // Always return 0
}

} // namespace harness_012
