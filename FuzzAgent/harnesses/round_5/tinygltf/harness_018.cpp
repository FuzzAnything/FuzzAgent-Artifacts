#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <unistd.h>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_018 {
/*
 * Fuzzing harness for tinygltf targeting core GLTF loading functions with maximum coverage.
 * Specifically targets the 4 core loading functions with massive undiscovered branch complexity:
 * - LoadASCIIFromString: 176 branches, 62 undiscovered (0% coverage)
 * - LoadASCIIFromFile: 188 branches, 71 undiscovered  
 * - LoadBinaryFromMemory: 244 branches, 110 undiscovered
 * - LoadBinaryFromFile: 252 branches, 116 undiscovered
 * 
 * Total: 860+ branches, 359+ undiscovered - representing the largest coverage gap in tinygltf.
 * 
 * This harness implements comprehensive GLTF file loading testing with various inputs:
 * 1) Create TinyGLTF instance
 * 2) Create Model instance to store loaded data  
 * 3) Create error/warning strings
 * 4) Call loading functions with fuzzed test data:
 *    - For LoadASCIIFromString: Provide ASCII GLTF string from fuzzed input
 *    - For LoadASCIIFromFile: Create temp file with ASCII GLTF content
 *    - For LoadBinaryFromMemory: Provide binary GLTF data buffer from fuzzed input
 *    - For LoadBinaryFromFile: Create temp file with binary GLTF content
 * 5) Check return status and error strings
 * 6) Model object destructor handles cleanup automatically
 * 
 * Focus on comprehensive GLTF parsing infrastructure testing with various inputs
 * to maximize coverage of the core loading logic.
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: Tests ASCII and binary loading but only as switch statement
 * - harness_001.cpp: Tests file-based loading with temporary files
 * - harness_012.cpp: Tests ASCII loading → stream writing workflow  
 * - harness_015.cpp: Tests loading with callback configuration but uses hardcoded minimal GLTF
 * - This harness: Focuses PURELY on the 4 core loading functions with MAXIMUM input variation,
 *   NO hardcoded test data, comprehensive testing of parsing edge cases
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


// Helper function to create a temporary file with given content and suffix
static std::string create_temp_file(const std::vector<uint8_t>& data, const std::string& suffix = "") {
    char tmp_filename[] = "/tmp/tinygltf_fuzz_XXXXXX";
    if (!suffix.empty()) {
        snprintf(tmp_filename, sizeof(tmp_filename), "/tmp/tinygltf_fuzz_XXXXXX%s", suffix.c_str());
    }
    
    int fd = mkstemps(tmp_filename, suffix.length());
    if (fd == -1) {
        // Fallback to simple mkstemp if mkstemps fails
        if (suffix.empty()) {
            fd = mkstemp(tmp_filename);
        }
        if (fd == -1) {
            return "";
        }
    }
    
    // Write data to file
    if (!data.empty()) {
        ssize_t written = write(fd, data.data(), data.size());
        if (written != static_cast<ssize_t>(data.size())) {
            close(fd);
            unlink(tmp_filename);
            return "";
        }
    }
    
    close(fd);
    return std::string(tmp_filename);
}

// Helper function to cleanup temporary file
static void cleanup_temp_file(const std::string& filename) {
    if (!filename.empty()) {
        unlink(filename.c_str());
    }
}

// Helper to generate GLB header for binary testing (uses fuzzed data, not hardcoded)
static void augment_with_glb_header(std::vector<uint8_t>& data, FuzzedDataProvider& fdp) {
    // Only add GLB header if we have enough space and want to test GLB format
    if (data.size() < 12) {
        return;
    }
    
    bool add_glb_header = fdp.ConsumeBool();
    if (!add_glb_header) {
        return;
    }
    
    // Create GLB header (12 bytes) and JSON chunk header (8 bytes)
    std::vector<uint8_t> glb_data;
    glb_data.reserve(20 + data.size());
    
    // GLB magic and version
    glb_data.push_back(0x67); // 'g'
    glb_data.push_back(0x6C); // 'l'
    glb_data.push_back(0x54); // 'T'
    glb_data.push_back(0x46); // 'F'
    glb_data.push_back(0x02); // version 2
    glb_data.push_back(0x00);
    glb_data.push_back(0x00);
    glb_data.push_back(0x00);
    
    // Total length will be updated later
    uint32_t total_length = 12 + 8 + static_cast<uint32_t>(data.size());
    glb_data.push_back(static_cast<uint8_t>(total_length & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((total_length >> 8) & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((total_length >> 16) & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((total_length >> 24) & 0xFF));
    
    // JSON chunk length and type
    uint32_t chunk_length = static_cast<uint32_t>(data.size());
    glb_data.push_back(static_cast<uint8_t>(chunk_length & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((chunk_length >> 8) & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((chunk_length >> 16) & 0xFF));
    glb_data.push_back(static_cast<uint8_t>((chunk_length >> 24) & 0xFF));
    
    glb_data.push_back(0x4A); // 'J'
    glb_data.push_back(0x53); // 'S'
    glb_data.push_back(0x4F); // 'O'
    glb_data.push_back(0x4E); // 'N'
    
    // Add the actual JSON data
    glb_data.insert(glb_data.end(), data.begin(), data.end());
    
    // Replace original data with GLB-wrapped data
    data = std::move(glb_data);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful testing
    // We need: loading method (1 byte), check_sections (4 bytes), base_dir length (1 byte)
    // plus actual GLTF data
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ========================================================================
    // Phase 1: Configuration from Fuzzed Input
    // ========================================================================
    
    // Select which loading function to test (0-3)
    uint8_t loading_method = fdp.ConsumeIntegralInRange<uint8_t>(0, 3);
    
    // Parse check_sections flags
    unsigned int check_sections = 0;
    bool check_version = fdp.ConsumeBool();
    bool check_scene = fdp.ConsumeBool();
    bool check_nodes = fdp.ConsumeBool();
    bool check_accessors = fdp.ConsumeBool();
    bool check_buffers = fdp.ConsumeBool();
    bool check_buffer_views = fdp.ConsumeBool();
    bool check_all = fdp.ConsumeBool();
    
    if (check_version) check_sections |= tinygltf::REQUIRE_VERSION;
    if (check_scene) check_sections |= tinygltf::REQUIRE_SCENE;
    if (check_nodes) check_sections |= tinygltf::REQUIRE_NODES;
    if (check_accessors) check_sections |= tinygltf::REQUIRE_ACCESSORS;
    if (check_buffers) check_sections |= tinygltf::REQUIRE_BUFFERS;
    if (check_buffer_views) check_sections |= tinygltf::REQUIRE_BUFFER_VIEWS;
    if (check_all) check_sections = tinygltf::REQUIRE_ALL;
    
    // Base directory for loading (if applicable)
    std::string base_dir;
    uint8_t base_dir_type = fdp.ConsumeIntegralInRange<uint8_t>(0, 3);
    switch (base_dir_type) {
        case 0:
            base_dir = "";  // Empty
            break;
        case 1:
            base_dir = "/tmp";  // Simple path
            break;
        case 2:
            base_dir = ".";  // Current directory
            break;
        case 3:
            // Consume a random string for base_dir
            if (fdp.remaining_bytes() > 10) {
                base_dir = fdp.ConsumeRandomLengthString(64);
            }
            break;
    }
    
    // ========================================================================
    // Phase 2: Prepare Test Data (NO HARCODED TEST CASES)
    // ========================================================================
    
    // Get remaining data as GLTF content - this is our fuzzed input
    std::vector<uint8_t> payload = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // For binary loading methods, optionally wrap with GLB header
    if (loading_method == 2 || loading_method == 3) {
        // Create a copy of fdp for augmentation (we can't use original fdp after ConsumeRemainingBytes)
        // Instead, we'll use a simple probability based on payload characteristics
        if (payload.size() > 20) {
            // Use first byte of payload to decide if we add GLB header
            bool add_glb = (payload[0] & 0x01) != 0;
            if (add_glb) {
                // Extract "JSON" part from payload (skip first few bytes to avoid using header bytes)
                size_t json_start = std::min<size_t>(10, payload.size());
                std::vector<uint8_t> json_data(payload.begin() + json_start, payload.end());
                
                // Create new payload with GLB header
                std::vector<uint8_t> glb_payload;
                glb_payload.reserve(20 + json_data.size());
                
                // GLB magic "glTF" and version 2
                uint8_t glb_header[] = {
                    0x67, 0x6C, 0x54, 0x46,  // "glTF"
                    0x02, 0x00, 0x00, 0x00,  // version 2
                };
                glb_payload.insert(glb_payload.end(), glb_header, glb_header + 8);
                
                // Total length
                uint32_t total_len = 12 + 8 + static_cast<uint32_t>(json_data.size());
                glb_payload.push_back(static_cast<uint8_t>(total_len & 0xFF));
                glb_payload.push_back(static_cast<uint8_t>((total_len >> 8) & 0xFF));
                glb_payload.push_back(static_cast<uint8_t>((total_len >> 16) & 0xFF));
                glb_payload.push_back(static_cast<uint8_t>((total_len >> 24) & 0xFF));
                
                // JSON chunk length
                uint32_t json_len = static_cast<uint32_t>(json_data.size());
                glb_payload.push_back(static_cast<uint8_t>(json_len & 0xFF));
                glb_payload.push_back(static_cast<uint8_t>((json_len >> 8) & 0xFF));
                glb_payload.push_back(static_cast<uint8_t>((json_len >> 16) & 0xFF));
                glb_payload.push_back(static_cast<uint8_t>((json_len >> 24) & 0xFF));
                
                // JSON chunk type
                uint8_t json_type[] = {0x4A, 0x53, 0x4F, 0x4E}; // "JSON"
                glb_payload.insert(glb_payload.end(), json_type, json_type + 4);
                
                // JSON data
                glb_payload.insert(glb_payload.end(), json_data.begin(), json_data.end());
                
                payload = std::move(glb_payload);
            }
        }
    }
    
    // Create temporary file if needed for file-based loading
    std::string temp_filename;
    if (loading_method == 1 || loading_method == 3) {  // File-based loading
        std::string suffix;
        if (loading_method == 1) {
            suffix = ".gltf";
        } else {
            suffix = ".glb";
        }
        
        temp_filename = create_temp_file(payload, suffix);
        if (temp_filename.empty()) {
            return 0;  // Failed to create temp file
        }
    }
    
    // ========================================================================
    // Phase 3: Initialize TinyGLTF and Execute Loading
    // ========================================================================
    
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    bool ret = false;
    
    // Execute the selected loading method
    switch (loading_method) {
        case 0:  // LoadASCIIFromString
            if (!payload.empty()) {
                ret = loader.LoadASCIIFromString(&model, &err, &warn,
                                                reinterpret_cast<const char*>(payload.data()),
                                                static_cast<unsigned int>(payload.size()),
                                                base_dir, check_sections);
            }
            break;
            
        case 1:  // LoadASCIIFromFile
            if (!temp_filename.empty()) {
                ret = loader.LoadASCIIFromFile(&model, &err, &warn,
                                              temp_filename, check_sections);
            }
            break;
            
        case 2:  // LoadBinaryFromMemory
            if (!payload.empty()) {
                ret = loader.LoadBinaryFromMemory(&model, &err, &warn,
                                                 payload.data(),
                                                 static_cast<unsigned int>(payload.size()),
                                                 base_dir, check_sections);
            }
            break;
            
        case 3:  // LoadBinaryFromFile
            if (!temp_filename.empty()) {
                ret = loader.LoadBinaryFromFile(&model, &err, &warn,
                                               temp_filename, check_sections);
            }
            break;
    }
    
    // ========================================================================
    // Phase 4: Cleanup
    // ========================================================================
    
    // Cleanup temporary file if created
    if (!temp_filename.empty()) {
        cleanup_temp_file(temp_filename);
    }
    
    // Model destructor automatically cleans up
    // We don't care about the return value - fuzzer tracks coverage
    return 0;
}

} // namespace harness_018
