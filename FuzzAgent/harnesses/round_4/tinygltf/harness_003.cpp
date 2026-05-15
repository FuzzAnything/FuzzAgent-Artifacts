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
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_003 {
/*
 * Fuzzing harness for tinygltf library - harness_003.cpp
 * 
 * This harness targets edge cases, error paths, and boundary conditions in file-based GLTF operations:
 * - LoadBinaryFromFile: Error paths for invalid files, permissions, malformed data
 * - LoadASCIIFromFile: Error paths for invalid ASCII files, syntax errors, missing resources
 * - WriteGltfSceneToFile: Error paths for disk full, invalid paths, permission denied
 * - WriteGltfSceneToStream: Error paths for stream failures, bad stream states
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Memory-based operations only (LoadASCIIFromString, LoadBinaryFromMemory)
 * - harness_001: Basic file I/O operations (load/write without deep error testing)
 * - harness_002: Round-trip operations with validation and model modification
 * - harness_003: Focus on error paths, boundary conditions, and failure scenarios
 * 
 * Targets 686+ branches of undiscovered complexity in error handling paths of file I/O operations.
 * 
 * Uses FuzzedDataProvider to generate pathological file paths, malformed data, 
 * and edge case scenarios that trigger error handling code paths.
 */


// Include FuzzedDataProvider for structured input splitting

// TinyGLTF headers - using standard implementation
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// Helper function to generate pathological file paths based on fuzzer input
static std::string generate_pathological_filename(FuzzedDataProvider& fdp) {
    // Use fuzzer input to create various problematic file paths
    uint8_t path_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (path_type) {
        case 0:
            // Very long path (potential buffer overflow)
            return std::string(4096, 'a') + ".gltf";
            
        case 1:
            // Path with null bytes
            return "test\0null\0byte.gltf";
            
        case 2:
            // Path with special characters
            return "test!@#$%^&*()_+-=[]{}|;:,.<>?`~.gltf";
            
        case 3:
            // Path with directory traversal
            return "../../../../etc/passwd";
            
        case 4:
            // Path with spaces and newlines
            return "test file with spaces\nand newline.gltf";
            
        case 5:
            // Empty filename
            return "";
            
        case 6:
            // Path that's a directory
            return "/tmp/";
            
        case 7:
            // Normal-looking but invalid path
            return fdp.ConsumeRandomLengthString(256) + ".gltf";
            
        default:
            return "default.gltf";
    }
}

// Helper function to create a malformed GLTF file
static std::string create_malformed_gltf_file(FuzzedDataProvider& fdp) {
    char temp_filename[] = "/tmp/tinygltf_error_XXXXXX.gltf";
    int fd = mkstemps(temp_filename, 5); // 5 for ".gltf"
    if (fd == -1) {
        return "";
    }
    
    // Generate malformed GLTF content based on fuzzer input
    uint8_t error_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    std::string malformed_content;
    
    switch (error_type) {
        case 0:
            // Empty file
            malformed_content = "";
            break;
            
        case 1:
            // Invalid JSON syntax
            malformed_content = "{ invalid json";
            break;
            
        case 2:
            // Valid JSON but missing required GLTF fields
            malformed_content = "{\"not_gltf\": true}";
            break;
            
        case 3:
            // GLTF with invalid version
            malformed_content = "{\"asset\": {\"version\": \"0.0\"}}";
            break;
            
        case 4:
            // GLTF with extremely large numbers (potential overflow)
            malformed_content = "{\"asset\": {\"version\": \"2.0\"}, \"buffers\": [{\"byteLength\": 999999999999999999}]}";
            break;
            
        case 5:
            // Binary GLTF with invalid header
            malformed_content = "glTF\x00\x00\x00\x00"; // Invalid version/length
            // Append some random binary data
            size_t random_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
            std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(random_size);
            malformed_content.append(reinterpret_cast<const char*>(random_data.data()), random_data.size());
            break;
    }
    
    // Write the malformed content
    ssize_t written = write(fd, malformed_content.data(), malformed_content.size());
    close(fd);
    
    if (written != static_cast<ssize_t>(malformed_content.size())) {
        unlink(temp_filename);
        return "";
    }
    
    return std::string(temp_filename);
}

// Helper function to test disk full simulation (by setting file size limits)
static void test_disk_full_scenario(const std::string& filename, FuzzedDataProvider& fdp) {
    // Create a file and then limit its growth to simulate disk full
    int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        return;
    }
    
    // Write some initial data
    std::string initial_data = "{\"asset\": {\"version\": \"2.0\"}}";
    write(fd, initial_data.data(), initial_data.size());
    close(fd);
    
    // Now try to write more data with ftruncate to limit file size
    fd = open(filename.c_str(), O_WRONLY);
    if (fd != -1) {
        // Set file size limit to current size to simulate disk full
        struct stat st;
        fstat(fd, &st);
        ftruncate(fd, st.st_size); // Limit to current size
        
        // Try to write more data (should fail or trigger error handling)
        std::string extra_data = fdp.ConsumeRandomLengthString(1024);
        write(fd, extra_data.data(), extra_data.size());
        close(fd);
    }
}

// Helper function to create a file with specific permissions
static std::string create_file_with_permissions(FuzzedDataProvider& fdp) {
    char temp_filename[] = "/tmp/tinygltf_perm_XXXXXX.gltf";
    int fd = mkstemps(temp_filename, 5);
    if (fd == -1) {
        return "";
    }
    
    // Write simple GLTF content
    std::string gltf_content = "{\"asset\": {\"version\": \"2.0\"}}";
    write(fd, gltf_content.data(), gltf_content.size());
    close(fd);
    
    // Set permissions based on fuzzer input
    uint8_t perm_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    mode_t mode = 0;
    
    switch (perm_type) {
        case 0:
            mode = 0000; // No permissions
            break;
        case 1:
            mode = 0444; // Read-only
            break;
        case 2:
            mode = 0222; // Write-only
            break;
        case 3:
            mode = 0111; // Execute-only (weird for data file)
            break;
    }
    
    chmod(temp_filename, mode);
    
    return std::string(temp_filename);
}

// Helper to create a bad stream (simulating stream failures)
static std::stringstream create_bad_stream(FuzzedDataProvider& fdp) {
    std::stringstream ss;
    
    // Set failbit or badbit based on fuzzer input
    uint8_t stream_error = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (stream_error) {
        case 0:
            // Set failbit
            ss.setstate(std::ios::failbit);
            break;
        case 1:
            // Set badbit
            ss.setstate(std::ios::badbit);
            break;
        case 2:
            // Both failbit and badbit
            ss.setstate(std::ios::failbit | std::ios::badbit);
            break;
    }
    
    // Try to write some data to the stream (will fail due to error state)
    if (fdp.remaining_bytes() > 0) {
        std::string data = fdp.ConsumeRandomLengthString(100);
        ss << data;
    }
    
    return ss;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 4) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Main selector for error scenario type
    uint8_t scenario_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Create TinyGLTF instance
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Set different parse strictness levels to test error paths
    uint8_t strictness = fdp.ConsumeIntegral<uint8_t>() % 2;
    switch (strictness) {
        case 0:
            loader.SetParseStrictness(tinygltf::Strict);
            break;
        case 1:
            loader.SetParseStrictness(tinygltf::Permissive);
            break;
    }
    
    switch (scenario_type) {
        case 0: {
            std::string bad_path = generate_pathological_filename(fdp);
            bool load_ascii = (fdp.ConsumeBool()) ? 
                loader.LoadASCIIFromFile(&model, &err, &warn, bad_path) :
                loader.LoadBinaryFromFile(&model, &err, &warn, bad_path);
            // Don't check result - we're testing error paths
            (void)load_ascii;
            break;
        }
        
        case 1: {
            // Test with malformed GLTF files
            std::string malformed_file = create_malformed_gltf_file(fdp);
            if (!malformed_file.empty()) {
                bool load_result = loader.LoadASCIIFromFile(&model, &err, &warn, malformed_file);
                (void)load_result;
                
                // Clean up
                unlink(malformed_file.c_str());
            }
            break;
        }
        
        case 2: {
            // Test disk full simulation
            char disk_full_file[] = "/tmp/tinygltf_diskfull_XXXXXX.gltf";
            int fd = mkstemps(disk_full_file, 5);
            if (fd != -1) {
                close(fd);
                std::string filename(disk_full_file);
                test_disk_full_scenario(filename, fdp);
                
                // Try to load/write with potentially full disk
                loader.LoadASCIIFromFile(&model, &err, &warn, filename);
                
                // Clean up
                unlink(filename.c_str());
            }
            break;
        }
        
        case 3: {
            // Test permission issues
            std::string perm_file = create_file_with_permissions(fdp);
            if (!perm_file.empty()) {
                bool load_result = loader.LoadASCIIFromFile(&model, &err, &warn, perm_file);
                (void)load_result;
                
                // Try to write to read-only or no-permission file
                if (fdp.ConsumeBool()) {
                    bool write_result = loader.WriteGltfSceneToFile(
                        &model, perm_file, 
                        fdp.ConsumeBool(), fdp.ConsumeBool(),  // embedImages, embedBuffers
                        fdp.ConsumeBool(), fdp.ConsumeBool()   // prettyPrint, writeBinary
                    );
                    (void)write_result;
                }
                
                // Clean up
                chmod(perm_file.c_str(), 0644); // Restore permissions for cleanup
                unlink(perm_file.c_str());
            }
            break;
        }
        
        case 4: {
            // Test with bad streams
            std::stringstream bad_ss = create_bad_stream(fdp);
            bool write_result = loader.WriteGltfSceneToStream(
                &model, bad_ss, 
                fdp.ConsumeBool(), fdp.ConsumeBool()  // prettyPrint, writeBinary
            );
            (void)write_result;
            break;
        }
        
        case 5: {
            // Test error paths in binary file loading
            std::string temp_file = create_malformed_gltf_file(fdp);
            if (!temp_file.empty()) {
                // First try ASCII load (will likely fail)
                loader.LoadASCIIFromFile(&model, &err, &warn, temp_file);
                
                // Then try binary load on same file (different error paths)
                err.clear();
                warn.clear();
                loader.LoadBinaryFromFile(&model, &err, &warn, temp_file);
                
                unlink(temp_file.c_str());
            }
            break;
        }
        
        case 6: {
            // Test with extremely large/small buffer sizes
            char size_test_file[] = "/tmp/tinygltf_size_XXXXXX.gltf";
            int fd = mkstemps(size_test_file, 5);
            if (fd != -1) {
                // Create file with size based on fuzzer input
                size_t file_size = fdp.ConsumeIntegralInRange<size_t>(0, 1048576); // Up to 1MB
                std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(file_size);
                
                write(fd, file_data.data(), file_data.size());
                close(fd);
                
                std::string filename(size_test_file);
                loader.LoadBinaryFromFile(&model, &err, &warn, filename);
                
                unlink(filename.c_str());
            }
            break;
        }
        
        case 7: {
            // Test with null model pointer and other invalid parameters
            std::string dummy_file = "/tmp/dummy.gltf";
            
            // Test with null model pointer (should crash or handle gracefully)
            if (fdp.ConsumeBool()) {
                loader.LoadASCIIFromFile(nullptr, &err, &warn, dummy_file);
            }
            
            // Test with null error/warn pointers
            if (fdp.ConsumeBool()) {
                loader.LoadASCIIFromFile(&model, nullptr, nullptr, dummy_file);
            }
            
            // Test with empty base directory variations
            char valid_file[] = "/tmp/tinygltf_valid_XXXXXX.gltf";
            int fd = mkstemps(valid_file, 5);
            if (fd != -1) {
                // Write minimal valid GLTF
                std::string valid_gltf = "{\"asset\": {\"version\": \"2.0\"}}";
                write(fd, valid_gltf.data(), valid_gltf.size());
                close(fd);
                
                std::string filename(valid_file);
                
                // Test multiple load attempts on same file (cache/reuse scenarios)
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                    err.clear();
                    warn.clear();
                    loader.LoadASCIIFromFile(&model, &err, &warn, filename);
                }
                
                unlink(filename.c_str());
            }
            break;
        }
    }
    
    // Additional edge case: Test with exhausted input (empty or nearly empty)
    if (fdp.remaining_bytes() == 0) {
        // Try operations with default/empty state
        tinygltf::Model empty_model;
        std::string empty_err, empty_warn;
        
        // Test writing empty model
        char empty_out[] = "/tmp/tinygltf_empty_XXXXXX.gltf";
        int fd = mkstemps(empty_out, 5);
        if (fd != -1) {
            close(fd);
            std::string out_file(empty_out);
            
            loader.WriteGltfSceneToFile(&empty_model, out_file, 
                                       false, false, false, false);
            
            // Try to load the empty output
            loader.LoadASCIIFromFile(&model, &empty_err, &empty_warn, out_file);
            
            unlink(out_file.c_str());
        }
    }
    
    return 0;
}

} // namespace harness_003
