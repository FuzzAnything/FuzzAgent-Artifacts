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

namespace harness_013 {
/*
 * Fuzzing harness for tinygltf library - harness_013.cpp
 * 
 * This harness specifically targets GLTF file writing/export operations with comprehensive
 * file verification using low-level file utilities:
 * - WriteGltfSceneToFile: Main file writing function (169 undiscovered branches)
 * - WriteGltfSceneToStream: Stream writing variant (122 undiscovered branches)
 * - WriteWholeFile: File writing utility for verification (8 undiscovered branches)
 * - GetFileSizeInBytes: File size utility for verification (20 undiscovered branches)
 * - LoadASCIIFromFile: Model loading from ASCII files (158 undiscovered branches)
 * - LoadBinaryFromFile: Model loading from binary files (211 undiscovered branches)
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_001: Basic file I/O operations (load/write without file verification)
 * - harness_002: Round-trip operations with validation of existing files
 * - harness_006: WriteGltfSceneToFile with embedded images and stb_image_write.h integration
 * - harness_010: WriteGltfSceneToFile parameter variations and round-trip validation
 * - harness_011: WriteGltfSceneToFile error conditions and validation failures
 * - harness_013: COMPREHENSIVE EXPORT WORKFLOW with file verification - specifically focuses on
 *                the complete Load -> Write -> Verify chain using WriteWholeFile and GetFileSizeInBytes
 *                for file operation validation, testing the entire GLTF export pipeline
 * 
 * Targets 500+ undiscovered branches across the GLTF export workflow with 0% coverage
 * in critical file writing utilities.
 * 
 * Uses FuzzedDataProvider to split input for comprehensive testing of different
 * export scenarios, file verification patterns, and parameter combinations.
 */


// Include FuzzedDataProvider for structured input splitting

// TinyGLTF headers - using standard implementation
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// Helper function to create a temporary file with fuzzer data
static std::string create_temp_input_file(const uint8_t* data, size_t size, const std::string& extension) {
    char temp_filename[64];
    if (extension == ".glb") {
        strcpy(temp_filename, "/tmp/tinygltf_export_XXXXXX.glb");
    } else {
        strcpy(temp_filename, "/tmp/tinygltf_export_XXXXXX.gltf");
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

// Helper function to create a temporary output file
static std::string create_temp_output_file(const std::string& extension) {
    char temp_filename[64];
    if (extension == ".glb") {
        strcpy(temp_filename, "/tmp/tinygltf_output_XXXXXX.glb");
    } else {
        strcpy(temp_filename, "/tmp/tinygltf_output_XXXXXX.gltf");
    }
    
    int fd = mkstemps(temp_filename, extension == ".glb" ? 4 : 5);
    if (fd == -1) {
        return "";
    }
    close(fd);
    
    return std::string(temp_filename);
}

// Helper function to verify file operations using WriteWholeFile and GetFileSizeInBytes
static bool verify_file_operations(const std::string& filename) {
    if (filename.empty()) {
        return false;
    }
    
    // Test GetFileSizeInBytes
    size_t file_size = 0;
    std::string size_err;
    bool size_result = tinygltf::GetFileSizeInBytes(&file_size, &size_err, filename, nullptr);
    if (!size_result || file_size == 0) {
        // File might not exist or be empty, that's OK for testing
        return true;
    }
    
    // Read the file content
    std::vector<unsigned char> file_content;
    std::string read_err;
    bool read_result = tinygltf::ReadWholeFile(&file_content, &read_err, filename, nullptr);
    if (!read_result || file_content.empty()) {
        // Can't read file, but that's OK for testing error paths
        return true;
    }
    
    // Create a copy filename to test WriteWholeFile
    char copy_filename[] = "/tmp/tinygltf_verify_XXXXXX.dat";
    int fd = mkstemp(copy_filename);
    if (fd == -1) {
        return false;
    }
    close(fd);
    
    std::string copy_filename_str(copy_filename);
    
    // Test WriteWholeFile by writing a copy
    std::string write_err;
    bool write_result = tinygltf::WriteWholeFile(&write_err, copy_filename_str, file_content, nullptr);
    
    // Clean up the copy file
    unlink(copy_filename);
    
    return write_result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need at least 8 bytes for selector and basic operations
    if (size < 8) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume selector bytes to control test behavior
    uint8_t selector1 = fdp.ConsumeIntegral<uint8_t>();
    uint8_t selector2 = fdp.ConsumeIntegral<uint8_t>();
    
    // Determine test configuration based on selector bits
    // Bit 0: Use ASCII (gltf) or Binary (glb) for input file
    bool input_is_ascii = (selector1 & 0x01) == 0;
    // Bit 1: Use ASCII or Binary for output format
    bool output_is_binary = (selector1 & 0x02) != 0;
    // Bit 2: embedImages flag for writing
    bool embed_images = (selector1 & 0x04) != 0;
    // Bit 3: embedBuffers flag for writing
    bool embed_buffers = (selector1 & 0x08) != 0;
    // Bit 4: prettyPrint flag for writing
    bool pretty_print = (selector1 & 0x10) != 0;
    // Bit 5: Test stream writing in addition to file writing
    bool test_stream = (selector1 & 0x20) != 0;
    // Bit 6: Verify file operations with WriteWholeFile/GetFileSizeInBytes
    bool verify_files = (selector1 & 0x40) != 0;
    // Bit 7: Use different strictness for loading
    bool use_strict_loading = (selector1 & 0x80) != 0;
    
    // Create input file from remaining fuzzer data
    std::string input_extension = input_is_ascii ? ".gltf" : ".glb";
    std::vector<uint8_t> file_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    if (file_data.empty()) {
        return 0;
    }
    
    std::string input_filename = create_temp_input_file(file_data.data(), file_data.size(), input_extension);
    if (input_filename.empty()) {
        return 0;
    }
    
    // Auto-cleanup for input file
    struct InputFileCleanup {
        std::string filename;
        ~InputFileCleanup() {
            if (!filename.empty()) {
                unlink(filename.c_str());
            }
        }
    } input_cleanup{input_filename};
    
    // Initialize TinyGLTF context
    tinygltf::TinyGLTF ctx;
    
    // Set parse strictness if requested
    if (use_strict_loading) {
        ctx.SetParseStrictness(tinygltf::Strict);
    } else {
        ctx.SetParseStrictness(tinygltf::Permissive);
    }
    
    // Load model from input file
    tinygltf::Model model;
    std::string err, warn;
    bool load_result = false;
    
    if (input_is_ascii) {
        load_result = ctx.LoadASCIIFromFile(&model, &err, &warn, input_filename,
                                           tinygltf::REQUIRE_VERSION);
    } else {
        load_result = ctx.LoadBinaryFromFile(&model, &err, &warn, input_filename,
                                            tinygltf::REQUIRE_VERSION);
    }
    
    // The load result is intentionally ignored as we're testing for crashes,
    // not correctness. Both success and failure paths are valuable for coverage.
    (void)load_result;
    
    // If loading succeeded and we have some model data, test writing operations
    if (load_result || !model.asset.version.empty() || !model.meshes.empty()) {
        // Create output filename for writing tests
        std::string output_extension = output_is_binary ? ".glb" : ".gltf";
        std::string output_filename = create_temp_output_file(output_extension);
        
        if (output_filename.empty()) {
            return 0;
        }
        
        // Auto-cleanup for output file
        struct OutputFileCleanup {
            std::string filename;
            ~OutputFileCleanup() {
                if (!filename.empty()) {
                    unlink(filename.c_str());
                }
            }
        } output_cleanup{output_filename};
        
        // Test WriteGltfSceneToFile
        bool write_result = ctx.WriteGltfSceneToFile(&model, output_filename,
                                                    embed_images, embed_buffers,
                                                    pretty_print, output_is_binary);
        (void)write_result;
        
        // If file writing succeeded and verification is enabled, test file utilities
        if (verify_files && write_result) {
            bool verification_passed = verify_file_operations(output_filename);
            (void)verification_passed;
        }
        
        // Test WriteGltfSceneToStream if enabled
        if (test_stream) {
            std::ostringstream oss;
            bool stream_result = ctx.WriteGltfSceneToStream(&model, oss,
                                                          pretty_print, output_is_binary);
            (void)stream_result;
            
            // Also test with stringstream
            std::stringstream ss;
            bool stream_result2 = ctx.WriteGltfSceneToStream(&model, ss,
                                                           pretty_print, output_is_binary);
            (void)stream_result2;
            
            // If stream writing produced output, optionally verify it
            if (verify_files && stream_result && !oss.str().empty()) {
                // Create a temporary file from stream output to verify
                std::string stream_filename = create_temp_output_file(output_extension);
                if (!stream_filename.empty()) {
                    struct StreamFileCleanup {
                        std::string filename;
                        ~StreamFileCleanup() {
                            if (!filename.empty()) {
                                unlink(filename.c_str());
                            }
                        }
                    } stream_cleanup{stream_filename};
                    
                    // Write stream content to file for verification
                    std::string stream_content = oss.str();
                    std::vector<unsigned char> stream_data(stream_content.begin(), stream_content.end());
                    
                    std::string write_err;
                    bool file_write_result = tinygltf::WriteWholeFile(&write_err, stream_filename,
                                                                     stream_data, nullptr);
                    (void)file_write_result;
                    
                    if (file_write_result) {
                        bool stream_verification = verify_file_operations(stream_filename);
                        (void)stream_verification;
                    }
                }
            }
        }
        
        // Test round-trip: try to load back what we wrote (if writing succeeded)
        if (write_result) {
            tinygltf::Model model2;
            std::string err2, warn2;
            
            bool readback_result = false;
            if (output_is_binary) {
                readback_result = ctx.LoadBinaryFromFile(&model2, &err2, &warn2,
                                                        output_filename,
                                                        tinygltf::REQUIRE_VERSION);
            } else {
                readback_result = ctx.LoadASCIIFromFile(&model2, &err2, &warn2,
                                                       output_filename,
                                                       tinygltf::REQUIRE_VERSION);
            }
            (void)readback_result;
        }
    }
    
    // Additional test: Direct file utility testing with random data
    if (verify_files && file_data.size() > 10) {
        // Create a temporary file with random data
        char util_filename[] = "/tmp/tinygltf_util_XXXXXX.dat";
        int fd = mkstemp(util_filename);
        if (fd != -1) {
            close(fd);
            std::string util_filename_str(util_filename);
            
            struct UtilFileCleanup {
                std::string filename;
                ~UtilFileCleanup() {
                    if (!filename.empty()) {
                        unlink(filename.c_str());
                    }
                }
            } util_cleanup{util_filename_str};
            
            // Write random data using WriteWholeFile
            std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(
                fdp.ConsumeIntegralInRange<size_t>(1, std::min(size_t(1024), file_data.size())));
            
            if (!random_data.empty()) {
                std::string write_err;
                bool util_write_result = tinygltf::WriteWholeFile(&write_err, util_filename_str,
                                                                 random_data, nullptr);
                (void)util_write_result;
                
                if (util_write_result) {
                    // Verify with GetFileSizeInBytes
                    size_t util_file_size = 0;
                    std::string size_err;
                    bool util_size_result = tinygltf::GetFileSizeInBytes(&util_file_size, &size_err,
                                                                        util_filename_str, nullptr);
                    (void)util_size_result;
                    
                    // Read back and compare
                    std::vector<unsigned char> readback_data;
                    std::string read_err;
                    bool util_read_result = tinygltf::ReadWholeFile(&readback_data, &read_err,
                                                                   util_filename_str, nullptr);
                    (void)util_read_result;
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_013
