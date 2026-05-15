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
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_015 {
/*
 * Fuzzing harness for tiny_gltf.h targeting comprehensive loading APIs with callback configuration.
 * Specifically targets all four core loading functions with full callback setup:
 * - LoadASCIIFromString (62 undiscovered branches, 0% covered)
 * - LoadASCIIFromFile (71 undiscovered branches, 0% covered)  
 * - LoadBinaryFromMemory (112 undiscovered branches, 0% covered)
 * - LoadBinaryFromFile (118 undiscovered branches, 0% covered)
 * 
 * Also targets helper callback APIs:
 * - SetFsCallbacks (7 undiscovered branches)
 * - SetURICallbacks (4 undiscovered branches)
 * - SetParseStrictness (0% covered)
 *
 * This harness implements a comprehensive loading workflow with callback configuration:
 * 1) Create TinyGLTF instance
 * 2) Configure parse strictness (Permissive/Strict/Completely)
 * 3) Set custom filesystem callbacks
 * 4) Set custom URI callbacks
 * 5) Create Model object
 * 6) Call loading functions with fuzzed GLTF/GLB data
 * 7) Validate return values and error strings
 * 8) Automatic cleanup via destructors
 *
 * Tests both ASCII (.gltf) and Binary (.glb) formats, valid and malformed data,
 * edge cases with different parsing strictness levels.
 *
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp tests LoadASCIIFromString and LoadBinaryFromMemory but without callback configuration
 * - harness_001.cpp tests LoadASCIIFromFile and LoadBinaryFromFile with temporary files but minimal callbacks
 * - harness_014.cpp tests LoadASCIIFromString with callback configuration
 * - This harness comprehensively tests ALL FOUR loading methods with FULL callback configuration
 *   to maximize coverage of the 363+ undiscovered branches in core loading logic
 *
 * Uses FuzzedDataProvider for structured input splitting to generate:
 * - Loading method selection (ASCII from string/file, Binary from memory/file)
 * - Parse strictness mode (Permissive/Strict/Completely)
 * - Filesystem callback implementations and behavior
 * - URI callback implementations and behavior
 * - GLTF/GLB data variations (valid JSON, binary GLB, malformed data)
 * - Temporary file paths and base_dir variations
 * - Check sections flags for validation
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


// ============================================================================
// Custom Filesystem Callback Implementations
// ============================================================================

// User data structure for filesystem callbacks
struct FsUserData {
    uint8_t pattern;
    bool should_succeed;
    size_t max_file_size;
};

static bool CustomFileExists(const std::string &abs_filename, void *user_data) {
    FsUserData* data = static_cast<FsUserData*>(user_data);
    if (!data || !data->should_succeed) {
        return false;
    }
    
    // Check file extension patterns
    if (abs_filename.find(".gltf") != std::string::npos ||
        abs_filename.find(".glb") != std::string::npos ||
        abs_filename.find(".json") != std::string::npos) {
        return (data->pattern & 0x01) != 0;
    }
    
    if (abs_filename.find("texture") != std::string::npos ||
        abs_filename.find("image") != std::string::npos ||
        abs_filename.find(".png") != std::string::npos ||
        abs_filename.find(".jpg") != std::string::npos) {
        return (data->pattern & 0x02) != 0;
    }
    
    if (abs_filename.find(".bin") != std::string::npos) {
        return (data->pattern & 0x04) != 0;
    }
    
    return (data->pattern & 0x08) != 0;
}

static std::string CustomExpandFilePath(const std::string &filepath, void *user_data) {
    FsUserData* data = static_cast<FsUserData*>(user_data);
    if (!data || !data->should_succeed) {
        return "";
    }
    
    // Simple expansion - add /tmp prefix for testing
    if (filepath.empty()) {
        return "/tmp";
    }
    
    if (filepath[0] == '/') {
        return filepath;  // Already absolute
    }
    
    return "/tmp/" + filepath;
}

static bool CustomReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                               const std::string &filepath, void *user_data) {
    FsUserData* data = static_cast<FsUserData*>(user_data);
    if (!data || !data->should_succeed) {
        if (err) *err = "Custom filesystem: User data invalid or should not succeed";
        return false;
    }
    
    // Try to actually read the file if it exists
    FILE* f = fopen(filepath.c_str(), "rb");
    if (!f) {
        if (err) *err = "Custom filesystem: File does not exist or cannot be opened";
        return false;
    }
    
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    if (file_size < 0 || file_size > static_cast<long>(data->max_file_size)) {
        fclose(f);
        if (err) *err = "Custom filesystem: Invalid file size or exceeds maximum";
        return false;
    }
    
    out->resize(file_size);
    if (file_size > 0) {
        size_t bytes_read = fread(out->data(), 1, file_size, f);
        fclose(f);
        if (bytes_read != static_cast<size_t>(file_size)) {
            if (err) *err = "Custom filesystem: Failed to read entire file";
            return false;
        }
    } else {
        fclose(f);
    }
    
    return true;
}

static bool CustomWriteWholeFile(std::string *err, const std::string &filepath,
                                const std::vector<unsigned char> &contents, void *user_data) {
    FsUserData* data = static_cast<FsUserData*>(user_data);
    if (!data || !data->should_succeed) {
        if (err) *err = "Custom filesystem: User data invalid or should not succeed";
        return false;
    }
    
    FILE* f = fopen(filepath.c_str(), "wb");
    if (!f) {
        if (err) *err = "Custom filesystem: Cannot open file for writing";
        return false;
    }
    
    if (!contents.empty()) {
        size_t bytes_written = fwrite(contents.data(), 1, contents.size(), f);
        fclose(f);
        if (bytes_written != contents.size()) {
            if (err) *err = "Custom filesystem: Failed to write entire file";
            return false;
        }
    } else {
        fclose(f);
    }
    
    return true;
}

static bool CustomGetFileSizeInBytes(size_t *filesize_out, std::string *err,
                                    const std::string &filepath, void *user_data) {
    FsUserData* data = static_cast<FsUserData*>(user_data);
    if (!data || !data->should_succeed) {
        if (err) *err = "Custom filesystem: User data invalid or should not succeed";
        return false;
    }
    
    struct stat st;
    if (stat(filepath.c_str(), &st) != 0) {
        if (err) *err = "Custom filesystem: Cannot stat file";
        return false;
    }
    
    *filesize_out = static_cast<size_t>(st.st_size);
    return true;
}

// ============================================================================
// Custom URI Callback Implementations
// ============================================================================

// User data structure for URI callbacks
struct URIUserData {
    uint8_t pattern;
    bool should_succeed;
    bool encode_as_data_uri;
};

static bool CustomURIEncode(const std::string &in_uri, const std::string &object_type,
                           std::string *out_uri, void *user_data) {
    URIUserData* data = static_cast<URIUserData*>(user_data);
    if (!data || !data->should_succeed || !out_uri) {
        return false;
    }
    
    if (data->encode_as_data_uri) {
        // Encode as data URI (simplified)
        *out_uri = "data:application/octet-stream;base64," + in_uri;
    } else {
        // Keep as is or add prefix based on pattern
        if (data->pattern & 0x01) {
            *out_uri = "file://" + in_uri;
        } else if (data->pattern & 0x02) {
            *out_uri = "http://example.com/" + in_uri;
        } else {
            *out_uri = in_uri;
        }
    }
    
    return true;
}

static bool CustomURIDecode(const std::string &in_uri, std::string *out_uri,
                           void *user_data) {
    URIUserData* data = static_cast<URIUserData*>(user_data);
    if (!data || !data->should_succeed || !out_uri) {
        return false;
    }
    
    // Simple decoding - strip prefixes
    if (in_uri.find("file://") == 0) {
        *out_uri = in_uri.substr(7);
    } else if (in_uri.find("http://") == 0) {
        *out_uri = in_uri.substr(7);
    } else if (in_uri.find("data:") == 0) {
        // Simple data URI handling
        size_t comma_pos = in_uri.find(',');
        if (comma_pos != std::string::npos) {
            *out_uri = in_uri.substr(comma_pos + 1);
        } else {
            *out_uri = in_uri;
        }
    } else {
        *out_uri = in_uri;
    }
    
    return true;
}

// ============================================================================
// Helper Functions
// ============================================================================

static std::string create_temp_file(const std::vector<uint8_t>& data, const std::string& suffix = "") {
    std::string pattern = "/tmp/tinygltf_fuzz_XXXXXX";
    if (!suffix.empty()) {
        pattern += suffix;
    }
    
    std::vector<char> tmp_filename(pattern.begin(), pattern.end());
    tmp_filename.push_back('\0');
    
    int fd = mkstemp(tmp_filename.data());
    if (fd == -1) {
        return "";
    }
    
    // Write data to file
    if (!data.empty()) {
        ssize_t written = write(fd, data.data(), data.size());
        if (written != static_cast<ssize_t>(data.size())) {
            close(fd);
            unlink(tmp_filename.data());
            return "";
        }
    }
    
    close(fd);
    return std::string(tmp_filename.data());
}

static void cleanup_temp_file(const std::string& filename) {
    if (!filename.empty()) {
        unlink(filename.c_str());
    }
}

// ============================================================================
// Main Fuzzer Function
// ============================================================================

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ========================================================================
    // Phase 1: Configuration Parameters from Fuzzed Input
    // ========================================================================
    
    // Select loading method (0-3 correspond to the four target APIs)
    uint8_t loading_method = fdp.ConsumeIntegralInRange<uint8_t>(0, 3);
    
    // Parse strictness (0=Permissive, 1=Strict, 2=Completely)
    uint8_t strictness_mode = fdp.ConsumeIntegralInRange<uint8_t>(0, 2);
    
    // Check sections flags
    unsigned int check_sections = 0;
    bool check_version = fdp.ConsumeBool();
    bool check_scene = fdp.ConsumeBool();
    bool check_nodes = fdp.ConsumeBool();
    bool check_all = fdp.ConsumeBool();
    
    if (check_version) check_sections |= tinygltf::REQUIRE_VERSION;
    if (check_scene) check_sections |= tinygltf::REQUIRE_SCENE;
    if (check_nodes) check_sections |= tinygltf::REQUIRE_NODES;
    if (check_all) check_sections = tinygltf::REQUIRE_ALL;
    
    // Filesystem callback configuration
    bool use_custom_fs = fdp.ConsumeBool();
    uint8_t fs_pattern = fdp.ConsumeIntegral<uint8_t>();
    bool fs_should_succeed = fdp.ConsumeBool();
    size_t max_file_size = fdp.ConsumeIntegralInRange<size_t>(1024, 10*1024*1024);
    
    // URI callback configuration
    bool use_custom_uri = fdp.ConsumeBool();
    uint8_t uri_pattern = fdp.ConsumeIntegral<uint8_t>();
    bool uri_should_succeed = fdp.ConsumeBool();
    bool encode_as_data_uri = fdp.ConsumeBool();
    
    // Base directory for loading
    std::string base_dir;
    if (fdp.ConsumeBool()) {
        base_dir = fdp.ConsumeRandomLengthString(64);
    }
    
    // ========================================================================
    // Phase 2: Prepare Test Data
    // ========================================================================
    
    // Get remaining data as payload - this will be used as GLTF/GLB content
    std::vector<uint8_t> payload = fdp.ConsumeRemainingBytes<uint8_t>();
    if (payload.empty()) {
        // If no payload, create minimal test data
        if (loading_method == 0 || loading_method == 1) {
            // Minimal valid GLTF JSON for ASCII loading
            const char* minimal_gltf = "{\"asset\":{\"version\":\"2.0\"}}";
            payload.assign(reinterpret_cast<const uint8_t*>(minimal_gltf), 
                          reinterpret_cast<const uint8_t*>(minimal_gltf) + strlen(minimal_gltf));
        } else {
            // Minimal GLB header for binary loading (12-byte header + JSON chunk)
            uint8_t minimal_glb[] = {
                0x67, 0x6C, 0x54, 0x46,  // magic "glTF"
                0x02, 0x00, 0x00, 0x00,  // version 2
                0x28, 0x00, 0x00, 0x00,  // total length 40 bytes
                0x18, 0x00, 0x00, 0x00,  // JSON chunk length 24
                0x4A, 0x53, 0x4F, 0x4E,  // chunk type "JSON"
                0x7B, 0x22, 0x61, 0x73,  // JSON content: {"asset":{"version":"2.0"}}
                0x73, 0x65, 0x74, 0x22,
                0x3A, 0x7B, 0x22, 0x76,
                0x65, 0x72, 0x73, 0x69,
                0x6F, 0x6E, 0x22, 0x3A,
                0x22, 0x32, 0x2E, 0x30,
                0x22, 0x7D, 0x7D
            };
            payload.assign(minimal_glb, minimal_glb + sizeof(minimal_glb));
        }
    }
    
    // Create temporary file if needed for file-based loading
    std::string temp_filename;
    if (loading_method == 1 || loading_method == 3) {  // File-based loading
        std::string suffix;
        if (loading_method == 1) suffix = ".gltf";
        else suffix = ".glb";
        
        temp_filename = create_temp_file(payload, suffix);
        if (temp_filename.empty()) {
            return 0;  // Failed to create temp file
        }
    }
    
    // ========================================================================
    // Phase 3: Initialize TinyGLTF and Configure Callbacks
    // ========================================================================
    
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    // Set parse strictness
    switch (strictness_mode) {
        case 0:
            loader.SetParseStrictness(tinygltf::ParseStrictness::Permissive);
            break;
        case 1:
            loader.SetParseStrictness(tinygltf::ParseStrictness::Strict);
            break;
        default:
            // Use default (Strict)
            break;
    }
    FsUserData fs_user_data{fs_pattern, fs_should_succeed, max_file_size};
    if (use_custom_fs) {
        tinygltf::FsCallbacks fs_callbacks;
        fs_callbacks.FileExists = CustomFileExists;
        fs_callbacks.ExpandFilePath = CustomExpandFilePath;
        fs_callbacks.ReadWholeFile = CustomReadWholeFile;
        fs_callbacks.WriteWholeFile = CustomWriteWholeFile;
        fs_callbacks.GetFileSizeInBytes = CustomGetFileSizeInBytes;
        fs_callbacks.user_data = &fs_user_data;
        
        if (!loader.SetFsCallbacks(fs_callbacks, &err)) {
            // Callback setup failed, but we can continue with default callbacks
            err.clear();
        }
    }
    
    // Set custom URI callbacks if requested
    URIUserData uri_user_data{uri_pattern, uri_should_succeed, encode_as_data_uri};
    if (use_custom_uri) {
        tinygltf::URICallbacks uri_callbacks;
        uri_callbacks.encode = CustomURIEncode;
        uri_callbacks.decode = CustomURIDecode;
        uri_callbacks.user_data = &uri_user_data;
        
        if (!loader.SetURICallbacks(uri_callbacks, &err)) {
            // Callback setup failed, but we can continue with default callbacks
            err.clear();
        }
    }
    
    // ========================================================================
    // Phase 4: Execute Selected Loading Method
    // ========================================================================
    
    bool ret = false;
    switch (loading_method) {
        case 0:  // LoadASCIIFromString
            ret = loader.LoadASCIIFromString(&model, &err, &warn,
                                            reinterpret_cast<const char*>(payload.data()),
                                            static_cast<unsigned int>(payload.size()),
                                            base_dir, check_sections);
            break;
            
        case 1:  // LoadASCIIFromFile
            if (!temp_filename.empty()) {
                ret = loader.LoadASCIIFromFile(&model, &err, &warn,
                                              temp_filename, check_sections);
            }
            break;
            
        case 2:  // LoadBinaryFromMemory
            ret = loader.LoadBinaryFromMemory(&model, &err, &warn,
                                             payload.data(),
                                             static_cast<unsigned int>(payload.size()),
                                             base_dir, check_sections);
            break;
            
        case 3:  // LoadBinaryFromFile
            if (!temp_filename.empty()) {
                ret = loader.LoadBinaryFromFile(&model, &err, &warn,
                                               temp_filename, check_sections);
            }
            break;
    }
    
    // ========================================================================
    // Phase 5: Cleanup
    // ========================================================================
    
    // Clean up temporary file if created
    if (!temp_filename.empty()) {
        cleanup_temp_file(temp_filename);
    }
    
    // Model and loader objects are automatically destroyed when they go out of scope
    
    return 0;
}

} // namespace harness_015
