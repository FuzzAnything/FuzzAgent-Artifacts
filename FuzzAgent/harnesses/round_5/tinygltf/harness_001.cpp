#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_001 {
/*
 * Fuzzing harness for tinygltf library - targeting file I/O operations
 * This harness tests file-based loading and writing APIs:
 * - LoadBinaryFromFile
 * - LoadASCIIFromFile  
 * - WriteGltfSceneToFile
 * - WriteGltfSceneToStream
 * Uses FuzzedDataProvider for structured input splitting and temporary files
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


// Helper function to create a temporary file with given content
static std::string create_temp_file(const std::vector<uint8_t>& data) {
    char tmp_filename[] = "/tmp/tinygltf_fuzz_XXXXXX";
    int fd = mkstemp(tmp_filename);
    if (fd == -1) {
        return "";
    }
    
    // Write data to file
    ssize_t written = write(fd, data.data(), data.size());
    close(fd);
    
    if (written != static_cast<ssize_t>(data.size())) {
        unlink(tmp_filename);
        return "";
    }
    
    return std::string(tmp_filename);
}

// Helper function to cleanup temporary file
static void cleanup_temp_file(const std::string& filename) {
    if (!filename.empty()) {
        unlink(filename.c_str());
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Need minimum data for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume selector byte to choose operation
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume file I/O operation type
    uint8_t operation = selector % 4;  // 0: LoadBinaryFromFile, 1: LoadASCIIFromFile, 2: WriteGltfSceneToFile, 3: WriteGltfSceneToStream
    
    // Consume boolean options for writing operations
    bool embed_images = (selector & 0x01) != 0;
    bool embed_buffers = (selector & 0x02) != 0;
    bool pretty_print = (selector & 0x04) != 0;
    bool write_binary = (selector & 0x08) != 0;
    
    // Consume check_sections flags for loading operations
    unsigned int check_sections = 0;
    bool check_version = (selector & 0x10) != 0;
    bool check_scene = (selector & 0x20) != 0;
    bool check_all = (selector & 0x40) != 0;
    
    if (check_version) check_sections |= tinygltf::REQUIRE_VERSION;
    if (check_scene) check_sections |= tinygltf::REQUIRE_SCENE;
    if (check_all) check_sections = tinygltf::REQUIRE_ALL;
    
    // Get the remaining data as payload
    std::vector<uint8_t> payload = fdp.ConsumeRemainingBytes<uint8_t>();
    if (payload.empty()) {
        return 0;
    }
    
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    // Create a temporary file with the payload data
    std::string temp_filename = create_temp_file(payload);
    if (temp_filename.empty()) {
        return 0;  // Failed to create temp file
    }
    
    bool ret = false;
    
    switch (operation) {
        case 0: { // LoadBinaryFromFile
            ret = loader.LoadBinaryFromFile(&model, &err, &warn, temp_filename, check_sections);
            break;
        }
            
        case 1: { // LoadASCIIFromFile
            ret = loader.LoadASCIIFromFile(&model, &err, &warn, temp_filename, check_sections);
            break;
        }
            
        case 2: { // WriteGltfSceneToFile
            // First try to load some data to have a model to write
            // Try loading as ASCII first, fall back to binary
            if (!loader.LoadASCIIFromFile(&model, &err, &warn, temp_filename, check_sections)) {
                // Clear model and try binary
                model = tinygltf::Model();
                (void)loader.LoadBinaryFromFile(&model, &err, &warn, temp_filename, check_sections);
            }
            
            // Create output filename
            char output_filename[] = "/tmp/tinygltf_output_XXXXXX";
            int out_fd = mkstemp(output_filename);
            if (out_fd != -1) {
                close(out_fd);
                std::string output_path(output_filename);
                
                // Write the model to file (even if loading failed, we might have empty model)
                ret = loader.WriteGltfSceneToFile(&model, output_path, 
                                                 embed_images, embed_buffers,
                                                 pretty_print, write_binary);
                
                // Cleanup output file
                unlink(output_filename);
            }
            break;
        }
            
        case 3: { // WriteGltfSceneToStream
            // First try to load some data to have a model to write
            if (!loader.LoadASCIIFromFile(&model, &err, &warn, temp_filename, check_sections)) {
                // Clear model and try binary
                model = tinygltf::Model();
                (void)loader.LoadBinaryFromFile(&model, &err, &warn, temp_filename, check_sections);
            }
            
            // Write to string stream
            std::ostringstream oss;
            ret = loader.WriteGltfSceneToStream(&model, oss, pretty_print, write_binary);
            break;
        }
    }
    
    // Cleanup temporary input file
    cleanup_temp_file(temp_filename);
    
    // The return value is ignored - we're testing for crashes, not correctness
    (void)ret;
    
    return 0;
}

} // namespace harness_001
