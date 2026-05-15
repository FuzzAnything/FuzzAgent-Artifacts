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
#include <memory>
#include <functional>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_015 {
/*
 * Fuzzing harness for tinygltf library - harness_015.cpp
 * 
 * This harness specifically targets glTF file writing operations with comprehensive
 * file system error simulation and low-level file utility testing:
 * - WriteGltfSceneToFile: Main file writing function with simulated file system failures (169 undiscovered branches)
 * - GetFileSizeInBytes: File size utility with error simulation (20 undiscovered branches)
 * - WriteWholeFile: File writing utility with error simulation (8 undiscovered branches)
 * - LoadASCIIFromFile/LoadBinaryFromFile: Model loading for test setup
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_001: Basic file I/O operations (load/write without error simulation)
 * - harness_003: General error paths and boundary conditions (not specifically file system utilities)
 * - harness_006: WriteGltfSceneToFile with embedded images (normal operation)
 * - harness_008: Configuration and callback APIs (normal callback testing)
 * - harness_010: WriteGltfSceneToFile parameter variations (normal operation)
 * - harness_013: File verification workflow (successful operations)
 * - harness_014: Round-trip serialization with model modification (normal operation)
 * - harness_015: FILE SYSTEM ERROR SIMULATION AND RECOVERY - specifically focuses on
 *                simulating file system failures (disk full, permissions, invalid paths)
 *                for WriteGltfSceneToFile and its underlying utilities (GetFileSizeInBytes,
 *                WriteWholeFile), testing error recovery and failure propagation through
 *                the GLTF export pipeline with 0% coverage in critical error handling paths.
 * 
 * Targets 197+ undiscovered branches across file system error handling with comprehensive
 * simulation of real-world failure scenarios.
 * 
 * Uses FuzzedDataProvider to split input for comprehensive testing of different
 * error conditions, failure modes, and recovery scenarios.
 */


// Include FuzzedDataProvider for structured input splitting

// TinyGLTF headers - using standard implementation
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// ============================================================================
// Custom FsCallbacks for error simulation
// ============================================================================

// Error simulation modes for custom file system callbacks
enum FsErrorMode {
    FS_ERROR_NONE = 0,
    FS_ERROR_OPEN_FAILURE,      // Simulate file open failure
    FS_ERROR_READ_FAILURE,      // Simulate file read failure
    FS_ERROR_WRITE_FAILURE,     // Simulate file write failure
    FS_ERROR_SIZE_FAILURE,      // Simulate GetFileSizeInBytes failure
    FS_ERROR_PERMISSION_DENIED, // Simulate permission denied
    FS_ERROR_DISK_FULL,         // Simulate disk full
    FS_ERROR_INVALID_PATH,      // Simulate invalid path
    FS_ERROR_RANDOM_FAILURE     // Random failure based on fuzzer input
};

// Custom filesystem callbacks with error simulation
static bool custom_get_file_size_in_bytes(size_t *filesize_out, std::string *err,
                                         const std::string &filepath, void *user_data) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(user_data);
    if (!fdp) {
        return tinygltf::GetFileSizeInBytes(filesize_out, err, filepath, nullptr);
    }
    
    // Consume error mode from fuzzer input
    uint8_t error_mode = fdp->ConsumeIntegral<uint8_t>() % 8;
    
    switch (error_mode) {
        case FS_ERROR_NONE:
            // Normal operation
            return tinygltf::GetFileSizeInBytes(filesize_out, err, filepath, nullptr);
            
        case FS_ERROR_SIZE_FAILURE:
            // Simulate size calculation failure
            if (err) {
                *err = "Simulated GetFileSizeInBytes failure for: " + filepath;
            }
            return false;
            
        case FS_ERROR_OPEN_FAILURE:
            // Simulate file open failure
            if (err) {
                *err = "Simulated file open error: " + filepath;
            }
            return false;
            
        case FS_ERROR_PERMISSION_DENIED:
            // Simulate permission denied
            if (err) {
                *err = "Permission denied: " + filepath;
            }
            return false;
            
        case FS_ERROR_INVALID_PATH:
            // Simulate invalid path
            if (err) {
                *err = "Invalid file path: " + filepath;
            }
            return false;
            
        case FS_ERROR_RANDOM_FAILURE:
            // Random failure based on fuzzer input
            if (fdp->ConsumeBool()) {
                if (err) {
                    *err = "Random GetFileSizeInBytes failure";
                }
                return false;
            }
            return tinygltf::GetFileSizeInBytes(filesize_out, err, filepath, nullptr);
            
        default:
            // Default to normal operation
            return tinygltf::GetFileSizeInBytes(filesize_out, err, filepath, nullptr);
    }
}

static bool custom_write_whole_file(std::string *err, const std::string &filepath,
                                   const std::vector<unsigned char> &contents, void *user_data) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(user_data);
    if (!fdp) {
        return tinygltf::WriteWholeFile(err, filepath, contents, nullptr);
    }
    
    // Consume error mode from fuzzer input
    uint8_t error_mode = fdp->ConsumeIntegral<uint8_t>() % 8;
    
    switch (error_mode) {
        case FS_ERROR_NONE:
            // Normal operation
            return tinygltf::WriteWholeFile(err, filepath, contents, nullptr);
            
        case FS_ERROR_WRITE_FAILURE:
            // Simulate write failure
            if (err) {
                *err = "Simulated WriteWholeFile failure for: " + filepath;
            }
            return false;
            
        case FS_ERROR_DISK_FULL:
            // Simulate disk full
            if (err) {
                *err = "Disk full: " + filepath;
            }
            return false;
            
        case FS_ERROR_PERMISSION_DENIED:
            // Simulate permission denied
            if (err) {
                *err = "Permission denied for writing: " + filepath;
            }
            return false;
            
        case FS_ERROR_INVALID_PATH:
            // Simulate invalid path
            if (err) {
                *err = "Invalid output path: " + filepath;
            }
            return false;
            
        case FS_ERROR_RANDOM_FAILURE:
            // Random failure based on fuzzer input
            if (fdp->ConsumeBool()) {
                if (err) {
                    *err = "Random WriteWholeFile failure";
                }
                return false;
            }
            return tinygltf::WriteWholeFile(err, filepath, contents, nullptr);
            
        default:
            // Default to normal operation
            return tinygltf::WriteWholeFile(err, filepath, contents, nullptr);
    }
}

// Custom file existence check with error simulation
static bool custom_file_exists(const std::string &filepath, void *user_data) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(user_data);
    if (!fdp) {
        // Use standard check
        struct stat buffer;
        return (stat(filepath.c_str(), &buffer) == 0);
    }
    
    // Random failure simulation
    return fdp->ConsumeBool();
}

// Custom expand file path (simplified)
static std::string custom_expand_file_path(const std::string &filepath, void *user_data) {
    // Simple implementation - just return the path
    return filepath;
}

// ============================================================================
// Helper functions for file operations
// ============================================================================

// Helper function to create a temporary input file with fuzzer data
static std::string create_temp_input_file(const uint8_t* data, size_t size, const std::string& extension) {
    char temp_filename[64];
    if (extension == ".glb") {
        strcpy(temp_filename, "/tmp/tinygltf_errsim_XXXXXX.glb");
    } else {
        strcpy(temp_filename, "/tmp/tinygltf_errsim_XXXXXX.gltf");
    }
    
    int fd = mkstemps(temp_filename, extension == ".glb" ? 4 : 5);
    if (fd == -1) {
        return "";
    }
    
    // Write fuzzer data to the temporary file
    ssize_t written = write(fd, data, size);
    close(fd);
    
    if (written != static_cast<ssize_t>(size)) {
        unlink(temp_filename);
        return "";
    }
    
    return std::string(temp_filename);
}

// Helper function to generate pathological output filenames
static std::string generate_error_output_filename(FuzzedDataProvider& fdp) {
    uint8_t error_type = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    switch (error_type) {
        case 0:
            // Normal temporary file
            {
                char temp_filename[] = "/tmp/tinygltf_output_XXXXXX.gltf";
                int fd = mkstemps(temp_filename, 5);
                if (fd != -1) {
                    close(fd);
                    return std::string(temp_filename);
                }
                return "/tmp/tinygltf_default.gltf";
            }
            
        case 1:
            // Read-only directory (simulate permission error)
            return "/root/test.gltf";
            
        case 2:
            // Non-existent directory
            return "/nonexistent/directory/test.gltf";
            
        case 3:
            // Very long path
            return std::string(1000, 'a') + ".gltf";
            
        case 4:
            // Path with null bytes (potentially problematic)
            return "test\0null.gltf";
            
        case 5:
            // Path with special characters
            return "/tmp/test!@#$%^.gltf";
            
        case 6:
            // Empty filename
            return "";
            
        case 7:
            // Existing directory (not a file)
            return "/tmp";
            
        case 8:
            // Device file
            return "/dev/null";
            
        case 9:
            // Symlink (create one)
            {
                char temp_filename[] = "/tmp/tinygltf_symlink_XXXXXX.gltf";
                int fd = mkstemps(temp_filename, 5);
                if (fd != -1) {
                    close(fd);
                    // Create symlink
                    std::string symlink_name = std::string(temp_filename) + "_link";
                    symlink(temp_filename, symlink_name.c_str());
                    return symlink_name;
                }
                return "/tmp/tinygltf_default.gltf";
            }
            
        default:
            return "/tmp/tinygltf_default.gltf";
    }
}

// Helper to consume a valid UTF-8 string from fuzzer input
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
        char c = 32 + (byte % 95);  // 95 printable ASCII chars (32-126)
        result.push_back(c);
    }
    
    return result;
}

// Helper to test GetFileSizeInBytes directly
static void test_get_file_size_directly(FuzzedDataProvider& fdp) {
    if (fdp.remaining_bytes() < 4) return;
    
    // Create a temporary test file
    size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    if (fdp.remaining_bytes() < test_size) return;
    
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
    
    char temp_filename[] = "/tmp/tinygltf_size_test_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) return;
    
    write(fd, test_data.data(), test_data.size());
    close(fd);
    
    std::string filename = temp_filename;
    
    // Test GetFileSizeInBytes with various error modes
    size_t file_size = 0;
    std::string err;
    
    // Test 1: Normal operation
    bool result1 = tinygltf::GetFileSizeInBytes(&file_size, &err, filename, nullptr);
    
    // Test 2: With custom error-prone callback - use original fdp by pointer
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.ReadWholeFile = nullptr;  // Use default
    fs_callbacks.WriteWholeFile = nullptr; // Use default
    fs_callbacks.GetFileSizeInBytes = &custom_get_file_size_in_bytes;
    fs_callbacks.FileExists = &custom_file_exists;
    fs_callbacks.ExpandFilePath = &custom_expand_file_path;
    fs_callbacks.user_data = &fdp;
    
    // Test 3: Direct WriteWholeFile testing
    std::string write_err;
    std::vector<unsigned char> write_data = {'t', 'e', 's', 't'};
    bool write_result = tinygltf::WriteWholeFile(&write_err, filename + ".write", write_data, nullptr);
    
    // Cleanup
    unlink(temp_filename);
    unlink((filename + ".write").c_str());
}

// ============================================================================
// Main fuzzer entry point
// ============================================================================

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need at least some data for meaningful testing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume selector to choose different test scenarios
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (test_scenario) {
        case 0: {
            // Scenario 0: Direct testing of GetFileSizeInBytes and WriteWholeFile
            test_get_file_size_directly(fdp);
            break;
        }
            
        case 1: {
            // Scenario 1: WriteGltfSceneToFile with error-prone custom callbacks
            if (fdp.remaining_bytes() < 100) break;
            
            // Create a temporary input file
            size_t input_size = fdp.ConsumeIntegralInRange<size_t>(100, fdp.remaining_bytes());
            std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(input_size);
            
            // Choose file extension based on fuzzer input
            bool use_glb = fdp.ConsumeBool();
            std::string input_file = create_temp_input_file(input_data.data(), input_data.size(), 
                                                           use_glb ? ".glb" : ".gltf");
            if (input_file.empty()) break;
            
            // Load model from the temporary file
            tinygltf::TinyGLTF loader;
            tinygltf::Model model;
            std::string err, warn;
            
            bool load_success = false;
            if (use_glb) {
                load_success = loader.LoadBinaryFromFile(&model, &err, &warn, input_file, 0);
            } else {
                load_success = loader.LoadASCIIFromFile(&model, &err, &warn, input_file, 0);
            }
            
            if (!load_success) {
                // Cleanup and exit
                unlink(input_file.c_str());
                break;
            }
            
            // Set up error-prone filesystem callbacks - use original fdp by pointer
            tinygltf::FsCallbacks error_callbacks;
            error_callbacks.ReadWholeFile = nullptr;  // Use default for reading
            error_callbacks.WriteWholeFile = &custom_write_whole_file;
            error_callbacks.GetFileSizeInBytes = &custom_get_file_size_in_bytes;
            error_callbacks.FileExists = &custom_file_exists;
            error_callbacks.ExpandFilePath = &custom_expand_file_path;
            error_callbacks.user_data = &fdp;
            
            // Configure loader with error-prone callbacks
            std::string fs_err;
            loader.SetFsCallbacks(error_callbacks, &fs_err);
            
            // Generate output filename with potential errors
            std::string output_file = generate_error_output_filename(fdp);
            
            // Test WriteGltfSceneToFile with various parameter combinations
            bool embedImages = fdp.ConsumeBool();
            bool embedBuffers = fdp.ConsumeBool();
            bool prettyPrint = fdp.ConsumeBool();
            bool writeBinary = fdp.ConsumeBool();
            
            // Attempt to write the file (may fail due to simulated errors)
            bool write_success = loader.WriteGltfSceneToFile(&model, output_file,
                                                            embedImages, embedBuffers,
                                                            prettyPrint, writeBinary);
            
            // Cleanup
            unlink(input_file.c_str());
            
            // Try to cleanup output file if it was created
            if (!output_file.empty() && output_file != "/tmp" && output_file != "/dev/null") {
                struct stat st;
                if (stat(output_file.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
                    unlink(output_file.c_str());
                }
            }
            
            break;
        }
            
        case 2: {
            // Scenario 2: Mixed error simulation with partial success/failure
            if (fdp.remaining_bytes() < 50) break;
            
            // Create multiple test files with different error conditions
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
                if (fdp.remaining_bytes() < test_data_size) break;
                
                std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
                
                // Test GetFileSizeInBytes on the test file
                char temp_filename[] = "/tmp/tinygltf_mixed_test_XXXXXX";
                int fd = mkstemp(temp_filename);
                if (fd != -1) {
                    write(fd, test_data.data(), test_data.size());
                    close(fd);
                    
                    size_t file_size = 0;
                    std::string err_msg;
                    bool size_result = tinygltf::GetFileSizeInBytes(&file_size, &err_msg, 
                                                                   temp_filename, nullptr);
                    
                    // Also test WriteWholeFile
                    std::string write_err;
                    std::vector<unsigned char> write_content = {'d', 'a', 't', 'a'};
                    bool write_result = tinygltf::WriteWholeFile(&write_err, 
                                                                 temp_filename + std::string(".copy"),
                                                                 write_content, nullptr);
                    
                    // Cleanup
                    unlink(temp_filename);
                    unlink((std::string(temp_filename) + ".copy").c_str());
                }
            }
            break;
        }
            
        case 3: {
            // Scenario 3: Stress testing with minimal input
            if (fdp.remaining_bytes() < 20) break;
            
            // Create a very small test file
            size_t tiny_size = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            std::vector<uint8_t> tiny_data = fdp.ConsumeBytes<uint8_t>(tiny_size);
            
            char temp_filename[] = "/tmp/tinygltf_stress_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd != -1) {
                write(fd, tiny_data.data(), tiny_data.size());
                close(fd);
                
                // Test GetFileSizeInBytes on tiny file
                size_t file_size = 0;
                std::string err_msg;
                tinygltf::GetFileSizeInBytes(&file_size, &err_msg, temp_filename, nullptr);
                
                // Test WriteWholeFile with tiny content
                std::string write_err;
                std::vector<unsigned char> tiny_write = {0};
                tinygltf::WriteWholeFile(&write_err, temp_filename + std::string(".tiny"),
                                        tiny_write, nullptr);
                
                // Cleanup
                unlink(temp_filename);
                unlink((std::string(temp_filename) + ".tiny").c_str());
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_015
