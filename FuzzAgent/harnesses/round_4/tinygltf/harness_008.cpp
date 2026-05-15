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
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_008 {
/*
 * Fuzzing harness for tinygltf library - harness_008.cpp
 * 
 * This harness targets configuration and callback APIs with 46+ undiscovered branches:
 * - SetImageWriter: Configure custom image writing callback
 * - SetImageLoader: Configure custom image loading callback  
 * - SetFsCallbacks: Configure custom filesystem callbacks
 * - SetURICallbacks: Configure custom URI encoding/decoding callbacks
 * - Custom callback implementations for file I/O, image loading/saving, and URI resolution
 * - Testing callback integration with loading/writing operations
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Memory-based operations (LoadASCIIFromString, LoadBinaryFromMemory)
 * - harness_001: Basic file I/O operations (load/write without callbacks)
 * - harness_002: Round-trip operations with validation
 * - harness_003: Error paths and boundary conditions
 * - harness_004: Model creation and serialization
 * - harness_005: Image writing APIs
 * - harness_006: WriteGltfSceneToFile with embedded images
 * - harness_007: Direct stb_image_write.h testing
 * - harness_008: Configuration and callback API testing with custom implementations
 * 
 * Targets configuration/callback APIs which represent a new semantic area
 * with 46+ undiscovered branches in custom file I/O and URI handling callbacks.
 * 
 * Uses FuzzedDataProvider to split input for comprehensive testing of different
 * callback configurations, behaviors, and integration scenarios.
 */


// Include FuzzedDataProvider for structured input splitting

// TinyGLTF headers - using standard implementation
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// ============================================================================
// Custom callback implementations using fuzzer data
// ============================================================================

// Custom image loader callback
static bool custom_image_loader(tinygltf::Image* image, const int image_idx, 
                                std::string* err, std::string* warn,
                                int req_width, int req_height,
                                const unsigned char* bytes, int size, 
                                void* user_data) {
    // Use user_data to pass fuzzer context
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(user_data);
    if (!fdp || fdp->remaining_bytes() == 0) {
        return false;
    }
    
    // Consume some fuzzer data to control loader behavior
    uint8_t behavior = fdp->ConsumeIntegral<uint8_t>();
    
    // Simple implementation that creates a small test image
    // This tests the callback integration path
    image->width = 16;
    image->height = 16;
    image->component = 4;  // RGBA
    image->bits = 8;
    image->pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
    
    // Generate pixel data based on fuzzer input
    size_t pixel_count = 16 * 16 * 4;  // width * height * components
    if (fdp->remaining_bytes() >= pixel_count) {
        image->image = fdp->ConsumeBytes<unsigned char>(pixel_count);
    } else {
        // Fallback: generate simple pattern
        image->image.resize(pixel_count);
        for (size_t i = 0; i < pixel_count; i++) {
            image->image[i] = static_cast<unsigned char>(i % 256);
        }
    }
    
    // Simulate success/failure based on fuzzer input
    return (behavior & 0x01) != 0;  // 50% chance of success
}

// Custom image writer callback  
static bool custom_image_writer(const std::string* basepath, 
                                const std::string* filename,
                                const tinygltf::Image* image, bool embedImages,
                                const tinygltf::FsCallbacks* fs_cb, 
                                const tinygltf::URICallbacks* uri_cb,
                                std::string* out_uri, void* user_pointer) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(user_pointer);
    if (!fdp) {
        return false;
    }
    
    // Consume fuzzer data to determine behavior
    uint8_t behavior = fdp->ConsumeIntegral<uint8_t>();
    
    // Simple implementation that returns a fake URI
    // This tests the callback integration path
    if (embedImages) {
        // Create data URI
        *out_uri = "data:image/png;base64,";
        if (fdp->remaining_bytes() > 10) {
            *out_uri += fdp->ConsumeRandomLengthString(50);
        }
    } else {
        // Create file URI
        *out_uri = *filename;
        if (fdp->remaining_bytes() > 5) {
            *out_uri += "_modified";
        }
    }
    
    // Simulate success/failure
    return (behavior & 0x02) != 0;  // Different bit to vary from loader
}

// Custom file exists callback
static bool custom_file_exists(const std::string& abs_filename, void* user_data) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(user_data);
    if (!fdp || fdp->remaining_bytes() == 0) {
        // Default behavior: check if file exists using stat
        struct stat buffer;
        return (stat(abs_filename.c_str(), &buffer) == 0);
    }
    
    uint8_t behavior = fdp->ConsumeIntegral<uint8_t>();
    // Return true/false based on fuzzer input
    return (behavior & 0x04) != 0;
}

// Custom expand file path callback
static std::string custom_expand_filepath(const std::string& filepath, void* user_data) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(user_data);
    if (!fdp || fdp->remaining_bytes() == 0) {
        return filepath;  // Default: return as-is
    }
    
    // Consume string to modify path
    if (fdp->remaining_bytes() > 10) {
        std::string prefix = fdp->ConsumeRandomLengthString(20);
        return prefix + "/" + filepath;
    }
    
    return filepath;
}

// Custom read whole file callback
static bool custom_read_whole_file(std::vector<unsigned char>* out, 
                                   std::string* err, 
                                   const std::string& filepath, 
                                   void* user_data) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(user_data);
    if (!fdp) {
        return false;
    }
    
    // Try to read actual file first
    std::ifstream file(filepath, std::ios::binary);
    if (file) {
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        out->resize(size);
        file.read(reinterpret_cast<char*>(out->data()), size);
        return true;
    }
    
    // If file doesn't exist, generate data from fuzzer input
    if (fdp->remaining_bytes() > 0) {
        size_t data_size = fdp->ConsumeIntegralInRange<size_t>(1, 
            std::min<size_t>(1024, fdp->remaining_bytes()));
        *out = fdp->ConsumeBytes<unsigned char>(data_size);
        return true;
    }
    
    return false;
}

// Custom write whole file callback
static bool custom_write_whole_file(std::string* err, 
                                    const std::string& filepath,
                                    const std::vector<unsigned char>& contents, 
                                    void* user_data) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(user_data);
    if (!fdp) {
        return false;
    }
    
    // Try to write to file
    std::ofstream file(filepath, std::ios::binary);
    if (!file) {
        if (err) *err = "Failed to open file for writing";
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    
    // Simulate success/failure based on fuzzer input
    uint8_t behavior = fdp->ConsumeIntegral<uint8_t>();
    return (behavior & 0x08) != 0;
}

// Custom get file size callback
static bool custom_get_file_size(size_t* filesize_out, std::string* err,
                                 const std::string& abs_filename, void* user_data) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(user_data);
    if (!fdp) {
        return false;
    }
    
    // Try to get actual file size
    struct stat buffer;
    if (stat(abs_filename.c_str(), &buffer) == 0) {
        *filesize_out = buffer.st_size;
        return true;
    }
    
    // If file doesn't exist, generate size from fuzzer input
    if (fdp->remaining_bytes() > 0) {
        *filesize_out = fdp->ConsumeIntegralInRange<size_t>(0, 65536);
        return true;
    }
    
    return false;
}

// Custom URI decode callback
static bool custom_uri_decode(const std::string& in_uri, 
                              std::string* out_uri, 
                              void* user_data) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(user_data);
    if (!fdp) {
        // Default: copy as-is
        *out_uri = in_uri;
        return true;
    }
    
    // Simple decoding: remove "custom://" prefix if present
    if (in_uri.find("custom://") == 0) {
        *out_uri = in_uri.substr(9);
    } else if (in_uri.find("file://") == 0) {
        *out_uri = in_uri.substr(7);
    } else {
        *out_uri = in_uri;
    }
    
    // Potentially modify based on fuzzer input
    if (fdp->remaining_bytes() > 5) {
        uint8_t modify = fdp->ConsumeIntegral<uint8_t>();
        if (modify & 0x01) {
            *out_uri += "_decoded";
        }
    }
    
    return true;
}

// Custom URI encode callback
static bool custom_uri_encode(const std::string& in_uri, 
                              const std::string& object_type,
                              std::string* out_uri, 
                              void* user_data) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(user_data);
    if (!fdp) {
        // Default: copy as-is
        *out_uri = in_uri;
        return true;
    }
    
    // Simple encoding: add prefix based on object type
    if (object_type == "image") {
        *out_uri = "custom://images/" + in_uri;
    } else if (object_type == "buffer") {
        *out_uri = "custom://buffers/" + in_uri;
    } else {
        *out_uri = "custom://" + in_uri;
    }
    
    // Potentially modify based on fuzzer input
    if (fdp->remaining_bytes() > 5) {
        uint8_t modify = fdp->ConsumeIntegral<uint8_t>();
        if (modify & 0x02) {
            *out_uri += "_encoded";
        }
    }
    
    return true;
}

// Helper function to create a temporary file with fuzzer data
static std::string create_temp_file(const uint8_t* data, size_t size) {
    char temp_filename[] = "/tmp/tinygltf_callback_XXXXXX";
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

// ============================================================================
// Main fuzzer entry point
// ============================================================================

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for configuration testing
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume selector to choose different callback configuration scenarios
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Create TinyGLTF context
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // ========================================================================
    // Test 1: Configure custom callbacks
    // ========================================================================
    
    // Configure image loader callback
    if (selector & 0x01) {
        ctx.SetImageLoader(custom_image_loader, &fdp);
    }
    
    // Configure image writer callback
    if (selector & 0x02) {
        ctx.SetImageWriter(custom_image_writer, &fdp);
    }
    
    // Configure filesystem callbacks
    if (selector & 0x04) {
        tinygltf::FsCallbacks fs_callbacks;
        fs_callbacks.FileExists = custom_file_exists;
        fs_callbacks.ExpandFilePath = custom_expand_filepath;
        fs_callbacks.ReadWholeFile = custom_read_whole_file;
        fs_callbacks.WriteWholeFile = custom_write_whole_file;
        fs_callbacks.GetFileSizeInBytes = custom_get_file_size;
        fs_callbacks.user_data = &fdp;
        
        ctx.SetFsCallbacks(fs_callbacks, &err);
    }
    
    // Configure URI callbacks
    if (selector & 0x08) {
        tinygltf::URICallbacks uri_callbacks;
        uri_callbacks.encode = custom_uri_encode;
        uri_callbacks.decode = custom_uri_decode;
        uri_callbacks.user_data = &fdp;
        
        ctx.SetURICallbacks(uri_callbacks, &err);
    }
    
    // ========================================================================
    // Test 2: Create test data for loading
    // ========================================================================
    
    // Generate simple glTF JSON data for testing
    std::string test_json;
    if (fdp.remaining_bytes() > 100) {
        // Create minimal valid glTF JSON
        test_json = R"({
            "asset": { "version": "2.0", "generator": "fuzzer" },
            "scenes": [{ "nodes": [] }],
            "scene": 0,
            "nodes": [],
            "meshes": [],
            "buffers": [],
            "bufferViews": [],
            "accessors": [],
            "materials": [],
            "textures": [],
            "images": []
        })";
    } else {
        // Use fuzzer data as JSON
        test_json = fdp.ConsumeRemainingBytesAsString();
    }
    
    // ========================================================================
    // Test 3: Load with configured callbacks
    // ========================================================================
    
    bool load_result = false;
    
    // Choose loading method based on selector
    if ((selector & 0x10) != 0) {
        // Load from memory (ASCII)
        load_result = ctx.LoadASCIIFromString(&model, &err, &warn,
                                              test_json.c_str(),
                                              static_cast<unsigned int>(test_json.size()),
                                              "/tmp",
                                              tinygltf::REQUIRE_VERSION);
    } else {
        // Load from file
        std::string temp_file = create_temp_file(
            reinterpret_cast<const uint8_t*>(test_json.data()), 
            test_json.size());
        
        if (!temp_file.empty()) {
            load_result = ctx.LoadASCIIFromFile(&model, &err, &warn,
                                                temp_file,
                                                tinygltf::REQUIRE_VERSION);
            
            // Clean up temp file
            unlink(temp_file.c_str());
        }
    }
    
    // ========================================================================
    // Test 4: Write with configured callbacks
    // ========================================================================
    
    if (load_result && !model.meshes.empty()) {
        // Create output file
        char output_file[] = "/tmp/tinygltf_callback_out_XXXXXX.gltf";
        int fd = mkstemps(output_file, 5);  // 5 chars for ".gltf"
        if (fd != -1) {
            close(fd);
            
            // Test writing with configured callbacks
            bool write_result = ctx.WriteGltfSceneToFile(&model, output_file,
                                                        (selector & 0x20) != 0,  // embedImages
                                                        (selector & 0x40) != 0,  // embedBuffers
                                                        (selector & 0x80) != 0,  // prettyPrint
                                                        false);                  // writeBinary
            
            // Clean up output file
            if (write_result) {
                unlink(output_file);
            }
        }
    }
    
    // ========================================================================
    // Test 5: Round-trip with different callback combinations
    // ========================================================================
    
    // Create a second context with different callback configuration
    if (fdp.remaining_bytes() > 50) {
        tinygltf::TinyGLTF ctx2;
        
        // Configure with opposite settings from first context
        if (!(selector & 0x01)) {
            ctx2.SetImageLoader(custom_image_loader, &fdp);
        }
        
        if (!(selector & 0x04)) {
            tinygltf::FsCallbacks fs_callbacks2;
            fs_callbacks2.FileExists = custom_file_exists;
            fs_callbacks2.ExpandFilePath = custom_expand_filepath;
            fs_callbacks2.ReadWholeFile = custom_read_whole_file;
            fs_callbacks2.WriteWholeFile = custom_write_whole_file;
            fs_callbacks2.GetFileSizeInBytes = custom_get_file_size;
            fs_callbacks2.user_data = &fdp;
            
            ctx2.SetFsCallbacks(fs_callbacks2, &err);
        }
        
        // Test loading with second context
        tinygltf::Model model2;
        std::string err2, warn2;
        
        if (fdp.remaining_bytes() > 100) {
            std::string json2 = fdp.ConsumeRandomLengthString(200);
            ctx2.LoadASCIIFromString(&model2, &err2, &warn2,
                                     json2.c_str(),
                                     static_cast<unsigned int>(json2.size()),
                                     "",
                                     tinygltf::REQUIRE_VERSION);
        }
    }
    
    // ========================================================================
    // Test 6: Callback-specific error handling
    // ========================================================================
    
    // Test callback configuration error cases
    if (fdp.remaining_bytes() > 10) {
        // Test with null callbacks
        tinygltf::TinyGLTF ctx3;
        
        // Test removing image loader (by setting to nullptr)
        ctx3.SetImageLoader(nullptr, nullptr);
        
        // Test with partially configured FsCallbacks
        tinygltf::FsCallbacks partial_fs;
        partial_fs.FileExists = custom_file_exists;
        partial_fs.ExpandFilePath = custom_expand_filepath;
        // Leave other callbacks null
        partial_fs.user_data = &fdp;
        
        // This should work (null callbacks are allowed)
        ctx3.SetFsCallbacks(partial_fs, &err);
    }
    
    return 0;
}

} // namespace harness_008
