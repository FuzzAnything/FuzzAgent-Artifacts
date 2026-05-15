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
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_014 {
/*
 * Fuzzing harness for tiny_gltf.h targeting configuration APIs and LoadASCIIFromString.
 * Specifically targets:
 * - LoadASCIIFromString (0% covered, 62 undiscovered branches)
 * - SetImageLoader (0% covered)
 * - SetParseStrictness (0% covered)
 * - SetFsCallbacks (41.67% covered, 7 undiscovered branches)
 * - SetURICallbacks (33.33% covered, 4 undiscovered branches)
 * 
 * This harness implements a comprehensive configuration and loading workflow:
 * 1) Create TinyGLTF instance
 * 2) Set parse strictness (Permissive/Strict)
 * 3) Set custom filesystem callbacks with various implementations
 * 4) Set custom URI callbacks with encoding/decoding functions
 * 5) Set custom image loader callback
 * 6) Create Model instance
 * 7) Call LoadASCIIFromString with fuzzed GLTF JSON data
 * 8) Model destructor cleanup
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp tests LoadASCIIFromString but without configuration callbacks
 * - harness_012.cpp tests LoadASCIIFromString with WriteGltfSceneToStream
 * - This harness focuses specifically on configuration callback setup and ASCII loading
 *   with custom filesystem, URI, and image loader implementations to maximize coverage
 *   of low-coverage configuration APIs
 * 
 * Uses FuzzedDataProvider for structured input splitting to generate:
 * - Parse strictness mode (Permissive/Strict)
 * - Filesystem callback implementations and behavior
 * - URI callback implementations and behavior
 * - Image loader callback behavior
 * - ASCII GLTF content variations
 * - Loader options and base_dir variations
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


// Custom filesystem callback implementations
static bool CustomFileExists(const std::string &abs_filename, void *user_data) {
    // Return different results based on fuzzed input pattern
    uint8_t* pattern = static_cast<uint8_t*>(user_data);
    if (pattern == nullptr) {
        return false;
    }
    
    // Check for specific patterns in filename to test different code paths
    if (abs_filename.find(".gltf") != std::string::npos ||
        abs_filename.find(".json") != std::string::npos) {
        return (*pattern & 0x01) != 0;  // Return true/false based on pattern
    }
    
    if (abs_filename.find("texture") != std::string::npos ||
        abs_filename.find("image") != std::string::npos) {
        return (*pattern & 0x02) != 0;  // Return true/false based on pattern
    }
    
    return (*pattern & 0x04) != 0;  // Default behavior
}

static std::string CustomExpandFilePath(const std::string &filepath, void *user_data) {
    // Simple expansion - just return the input for testing
    // In real implementation, this would expand ~, environment variables, etc.
    return filepath;
}

static bool CustomReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &filepath, void *user_data) {
    // Mock implementation for testing - always fail to read files
    // This tests error handling paths in the loader
    if (err != nullptr) {
        *err = "Custom filesystem: File read not implemented for fuzzing";
    }
    return false;
}

static bool CustomWriteWholeFile(std::string *err, const std::string &filepath,
                                const std::vector<unsigned char> &contents, void *user_data) {
    // Mock implementation - always succeed for testing
    return true;
}

static bool CustomGetFileSizeInBytes(size_t *filesize_out, std::string *err,
                                    const std::string &filepath, void *user_data) {
    // Mock implementation - return random size based on filename pattern
    if (filesize_out == nullptr) {
        if (err != nullptr) {
            *err = "Null filesize_out pointer";
        }
        return false;
    }
    
    // Generate deterministic size based on filename hash
    size_t hash = 0;
    for (char c : filepath) {
        hash = hash * 31 + static_cast<size_t>(c);
    }
    *filesize_out = (hash % 1024) + 1;  // 1-1024 bytes
    
    return true;
}

// Custom URI callback implementations
static bool CustomURIEncode(const std::string &in_uri, const std::string &object_type,
                           std::string *out_uri, void *user_data) {
    // Simple encoding - just return input with prefix
    if (out_uri != nullptr) {
        *out_uri = "custom_encoded:" + in_uri;
        return true;
    }
    return false;
}

static bool CustomURIDecode(const std::string &in_uri, std::string *out_uri,
                           void *user_data) {
    // Simple decoding - strip prefix if present
    if (out_uri != nullptr) {
        const std::string prefix = "custom_encoded:";
        if (in_uri.find(prefix) == 0) {
            *out_uri = in_uri.substr(prefix.length());
        } else {
            *out_uri = in_uri;
        }
        return true;
    }
    return false;
}

// Custom image loader callback
static bool CustomLoadImageData(tinygltf::Image *image, const int image_idx,
                               std::string *err, std::string *warn,
                               int req_width, int req_height,
                               const unsigned char *bytes, int size,
                               void *user_data) {
    // Mock implementation for testing
    // Create a small test image (1x1 pixel) with RGBA data
    image->width = 1;
    image->height = 1;
    image->component = 4;  // RGBA
    image->bits = 8;
    image->pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
    
    // Create simple RGBA pixel (red)
    image->image = std::vector<unsigned char>(4);
    image->image[0] = 255;  // R
    image->image[1] = 0;    // G
    image->image[2] = 0;    // B
    image->image[3] = 255;  // A
    
    image->name = "custom_loaded_image_" + std::to_string(image_idx);
    image->uri = "data:image/png;base64,mock";
    
    return true;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size: need enough for configuration bytes and some ASCII content
    if (size < 16) {
        return 0;  // Insufficient input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Consume configuration data from fuzzed input
    uint8_t config_byte1 = fdp.ConsumeIntegral<uint8_t>();
    uint8_t config_byte2 = fdp.ConsumeIntegral<uint8_t>();
    
    // Parse strictness configuration
    tinygltf::ParseStrictness strictness = tinygltf::ParseStrictness::Strict;
    if ((config_byte1 & 0x01) != 0) {
        strictness = tinygltf::ParseStrictness::Permissive;
    }
    
    // Filesystem callback configuration
    bool use_custom_fs = (config_byte1 & 0x02) != 0;
    uint8_t fs_behavior = config_byte2 & 0x07;  // 3 bits for FS behavior
    
    // URI callback configuration
    bool use_custom_uri = (config_byte1 & 0x04) != 0;
    bool encode_enabled = (config_byte1 & 0x08) != 0;
    
    // Image loader configuration
    bool use_custom_image_loader = (config_byte1 & 0x10) != 0;
    
    // Loader options
    bool preserve_channels = (config_byte1 & 0x20) != 0;
    bool images_as_is = (config_byte1 & 0x40) != 0;
    bool store_original_json = (config_byte1 & 0x80) != 0;
    
    // Check sections configuration
    unsigned int check_sections = 0;
    bool check_version = (config_byte2 & 0x08) != 0;
    bool check_scene = (config_byte2 & 0x10) != 0;
    bool check_all = (config_byte2 & 0x20) != 0;
    
    if (check_version) check_sections |= tinygltf::REQUIRE_VERSION;
    if (check_scene) check_sections |= tinygltf::REQUIRE_SCENE;
    if (check_all) check_sections = tinygltf::REQUIRE_ALL;
    
    // Base directory configuration
    uint8_t base_dir_type = config_byte2 >> 6;  // Use top 2 bits
    
    std::string base_dir;
    switch (base_dir_type) {
        case 0:
            base_dir = "";  // Empty
            break;
        case 1:
            base_dir = "/tmp/fuzz_test";  // Simple path
            break;
        case 2:
            // Consume a random length string for base_dir
            if (fdp.remaining_bytes() > 10) {
                base_dir = fdp.ConsumeRandomLengthString(64);
            } else {
                base_dir = "";
            }
            break;
        case 3:
            base_dir = ".";  // Current directory
            break;
    }
    
    // Step 2: Create TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Step 3: Set parse strictness
    loader.SetParseStrictness(strictness);
    
    // Step 4: Set filesystem callbacks (if configured)
    if (use_custom_fs) {
        tinygltf::FsCallbacks fs_callbacks;
        
        // Use static storage for user_data to avoid use-after-free
        // The library needs user_data to persist while callbacks are in use
        static uint8_t fs_user_data_storage = fs_behavior;
        uint8_t* fs_user_data = &fs_user_data_storage;
        fs_callbacks.FileExists = CustomFileExists;
        fs_callbacks.ExpandFilePath = CustomExpandFilePath;
        fs_callbacks.ReadWholeFile = CustomReadWholeFile;
        fs_callbacks.WriteWholeFile = CustomWriteWholeFile;
        fs_callbacks.GetFileSizeInBytes = CustomGetFileSizeInBytes;
        fs_callbacks.user_data = fs_user_data;
        
        std::string fs_err;
        bool fs_set = loader.SetFsCallbacks(fs_callbacks, &fs_err);
        
        // Note: user_data must persist while callbacks are in use
        // For fuzzing, we accept memory leak rather than use-after-free
    }
    
    // Step 5: Set URI callbacks (if configured)
    if (use_custom_uri) {
        tinygltf::URICallbacks uri_callbacks;
        
        // User data not needed for our simple implementation
        uri_callbacks.user_data = nullptr;
        
        if (encode_enabled) {
            uri_callbacks.encode = CustomURIEncode;
        } else {
            uri_callbacks.encode = nullptr;  // No encoding callback
        }
        uri_callbacks.decode = CustomURIDecode;
        
        std::string uri_err;
        bool uri_set = loader.SetURICallbacks(uri_callbacks, &uri_err);
    }
    
    // Step 6: Set custom image loader callback (if configured)
    if (use_custom_image_loader) {
        loader.SetImageLoader(CustomLoadImageData, nullptr);
    }
    
    // Set loader options
    loader.SetPreserveImageChannels(preserve_channels);
    loader.SetImagesAsIs(images_as_is);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(store_original_json);
    
    // Step 7: Create Model instance
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Step 8: Get the remaining data as ASCII GLTF content
    // Use all remaining bytes as the ASCII GLTF string to parse
    std::string ascii_content = fdp.ConsumeRemainingBytesAsString();
    if (ascii_content.empty()) {
        // If no content, create a minimal valid GLTF JSON for testing
        ascii_content = "{\"asset\": {\"version\": \"2.0\"}}";
    }
    
    // Step 9: Call LoadASCIIFromString with the consumed data
    bool ret = loader.LoadASCIIFromString(&model, &err, &warn,
                                         ascii_content.c_str(),
                                         static_cast<unsigned int>(ascii_content.size()),
                                         base_dir,
                                         check_sections);
    
    // Step 10: Test additional error conditions by calling again with invalid data
    // This helps explore more code paths in the loader
    if (!ret && fdp.remaining_bytes() == 0) {
        // Try with empty string to test different error path
        (void)loader.LoadASCIIFromString(&model, &err, &warn,
                                        "",
                                        0,
                                        base_dir,
                                        check_sections);
    }
    
    // Step 11: Test with null error/warn pointers to explore those code paths
    if ((config_byte2 & 0x40) != 0) {  // Use another bit for this test
        tinygltf::Model model2;
        (void)loader.LoadASCIIFromString(&model2,
                                        nullptr,  // null err pointer
                                        nullptr,  // null warn pointer
                                        ascii_content.c_str(),
                                        static_cast<unsigned int>(ascii_content.size()),
                                        base_dir,
                                        check_sections);
    }
    
    // Step 12: Cleanup happens automatically when objects go out of scope
    
    return 0;
}

} // namespace harness_014
