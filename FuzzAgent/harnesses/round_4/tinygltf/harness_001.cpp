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
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_001 {
/*
 * Fuzzing harness for tinygltf library - harness_001.cpp
 * 
 * This harness targets file-based GLTF operations with 0% coverage:
 * - LoadBinaryFromFile: Loads glTF binary asset from a file
 * - LoadASCIIFromFile: Loads glTF ASCII asset from a file  
 * - WriteGltfSceneToFile: Write glTF to file
 * - WriteGltfSceneToStream: Write glTF to stream
 * 
 * Semantic differentiation from harness_000:
 * - harness_000 targets memory-based operations (LoadASCIIFromString, LoadBinaryFromMemory)
 * - harness_001 targets file-based operations with file I/O, path handling, and filesystem interactions
 * 
 * Uses FuzzedDataProvider to split input for comprehensive testing
 * of different file operation paths and API variations.
 */


// Include FuzzedDataProvider for structured input splitting

// TinyGLTF headers - using standard implementation
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// Helper function to create a temporary file with fuzzer data
static std::string create_temp_file(const uint8_t* data, size_t size) {
    char temp_filename[] = "/tmp/tinygltf_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
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

// Helper function to create a temporary file with specific extension
static std::string create_temp_file_with_ext(const uint8_t* data, size_t size, const std::string& extension) {
    // Buffer needs to be large enough for template + extension + null terminator
    // Template: "/tmp/tinygltf_fuzz_XXXXXX" (25 characters + null = 26 bytes)
    // Extension: up to, say, 10 characters (e.g., ".gltf" is 5, ".glb" is 4)
    // Total: 26 + 10 = 36 bytes, plus some margin
    char temp_filename[64] = "/tmp/tinygltf_fuzz_XXXXXX";
    
    // Append extension before calling mkstemp
    strcat(temp_filename, extension.c_str());
    
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        return "";
    }
    
    ssize_t written = write(fd, data, size);
    close(fd);
    
    if (written != static_cast<ssize_t>(size)) {
        unlink(temp_filename);
        return "";
    }
    
    return std::string(temp_filename);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need at least 4 bytes for selector and some data
    if (size < 4) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume a selector byte to choose different operation paths
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Use different bits of selector to control different aspects:
    // Bit 0: ASCII vs Binary for loading
    // Bit 1: Use base_dir or empty for loading
    // Bit 2: embedImages for writing
    // Bit 3: embedBuffers for writing  
    // Bit 4: prettyPrint for writing
    // Bit 5: writeBinary for writing
    // Bit 6: Test WriteGltfSceneToStream vs WriteGltfSceneToFile
    bool load_ascii = (selector & 0x01) == 0;
    bool use_base_dir = (selector & 0x02) != 0;
    bool embed_images = (selector & 0x04) != 0;
    bool embed_buffers = (selector & 0x08) != 0;
    bool pretty_print = (selector & 0x10) != 0;
    bool write_binary = (selector & 0x20) != 0;
    bool test_stream = (selector & 0x40) != 0;
    
    // Split remaining input: first part for file content, second part for filename
    size_t filename_size = fdp.ConsumeIntegralInRange<size_t>(1, 32);
    std::string filename_part = fdp.ConsumeRandomLengthString(filename_size);
    
    // Get remaining data for file content
    std::vector<uint8_t> file_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    if (file_data.empty()) {
        return 0;
    }
    
    // Initialize TinyGLTF context
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Create temporary file with fuzzer data
    std::string temp_filename = create_temp_file(file_data.data(), file_data.size());
    if (temp_filename.empty()) {
        return 0;
    }
    
    // Clean up temp file at the end
    struct TempFileCleanup {
        std::string filename;
        ~TempFileCleanup() {
            if (!filename.empty()) {
                unlink(filename.c_str());
            }
        }
    } cleanup{temp_filename};
    
    bool load_result = false;
    
    // Test file loading based on selector
    if (load_ascii) {
        // ASCII/JSON file loading path
        std::string base_dir = use_base_dir ? "/tmp" : "";
        
        load_result = ctx.LoadASCIIFromFile(&model, &err, &warn,
                                           temp_filename,
                                           tinygltf::REQUIRE_VERSION);
    } else {
        // Binary/GLB file loading path  
        std::string base_dir = use_base_dir ? "/tmp" : "";
        
        load_result = ctx.LoadBinaryFromFile(&model, &err, &warn,
                                            temp_filename,
                                            tinygltf::REQUIRE_VERSION);
    }
    
    // The load result is intentionally ignored as we're testing for crashes,
    // not correctness. Both success and failure paths are valuable for coverage.
    (void)load_result;
    
    // If loading succeeded and we have a model, test writing operations
    if (load_result && !model.meshes.empty()) {
        // Create output filename for writing tests
        std::string output_filename = "/tmp/tinygltf_output_" + filename_part + 
                                     (write_binary ? ".glb" : ".gltf");
        
        // Clean up output file at the end if created
        struct OutputFileCleanup {
            std::string filename;
            ~OutputFileCleanup() {
                if (!filename.empty()) {
                    unlink(filename.c_str());
                }
            }
        } output_cleanup{output_filename};
        
        if (test_stream) {
            // Test WriteGltfSceneToStream
            std::ostringstream oss;
            bool write_result = ctx.WriteGltfSceneToStream(&model, oss, 
                                                          pretty_print, 
                                                          write_binary);
            (void)write_result;
            
            // Also test with stringstream
            std::stringstream ss;
            bool write_result2 = ctx.WriteGltfSceneToStream(&model, ss,
                                                           pretty_print,
                                                           write_binary);
            (void)write_result2;
        } else {
            // Test WriteGltfSceneToFile
            bool write_result = ctx.WriteGltfSceneToFile(&model, output_filename,
                                                        embed_images,
                                                        embed_buffers,
                                                        pretty_print,
                                                        write_binary);
            (void)write_result;
            
            // Try reading back the written file (if it was created)
            if (write_result) {
                tinygltf::Model model2;
                std::string err2, warn2;
                
                // Try to load what we just wrote
                bool readback_result = false;
                if (write_binary) {
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
    }
    
    // Test additional variations with a small probability
    if ((selector & 0x80) != 0 && file_data.size() > 100) {
        // Create a second temporary file with different data
        std::vector<uint8_t> small_data(file_data.begin(), 
                                       file_data.begin() + std::min(file_data.size(), size_t(1000)));
        
        std::string temp_filename2 = create_temp_file_with_ext(small_data.data(), 
                                                              small_data.size(),
                                                              load_ascii ? ".gltf" : ".glb");
        if (!temp_filename2.empty()) {
            TempFileCleanup cleanup2{temp_filename2};
            
            // Try parsing with different strictness settings
            tinygltf::ParseStrictness strictness = 
                (selector & 0x01) ? tinygltf::Strict : tinygltf::Permissive;
            ctx.SetParseStrictness(strictness);
            
            // Create a new model for this test
            tinygltf::Model model2;
            std::string err2, warn2;
            
            bool result2 = false;
            if (load_ascii) {
                result2 = ctx.LoadASCIIFromFile(&model2, &err2, &warn2,
                                               temp_filename2,
                                               tinygltf::REQUIRE_VERSION);
            } else {
                result2 = ctx.LoadBinaryFromFile(&model2, &err2, &warn2,
                                                temp_filename2,
                                                tinygltf::REQUIRE_VERSION);
            }
            (void)result2;
        }
    }
    
    // Test file operations with different filename patterns
    if (filename_part.size() > 3) {
        // Create another temp file with the filename part in the name
        std::string patterned_filename = "/tmp/gltf_" + filename_part + 
                                        (load_ascii ? "_test.gltf" : "_test.glb");
        
        // Write data to this patterned file
        std::ofstream ofs(patterned_filename, std::ios::binary);
        if (ofs) {
            ofs.write(reinterpret_cast<const char*>(file_data.data()), file_data.size());
            ofs.close();
            
            // Try loading from this patterned file
            tinygltf::Model model3;
            std::string err3, warn3;
            
            bool result3 = false;
            if (load_ascii) {
                result3 = ctx.LoadASCIIFromFile(&model3, &err3, &warn3,
                                               patterned_filename,
                                               tinygltf::REQUIRE_VERSION);
            } else {
                result3 = ctx.LoadBinaryFromFile(&model3, &err3, &warn3,
                                                patterned_filename,
                                                tinygltf::REQUIRE_VERSION);
            }
            (void)result3;
            
            // Clean up
            unlink(patterned_filename.c_str());
        }
    }
    
    return 0;
}

} // namespace harness_001
