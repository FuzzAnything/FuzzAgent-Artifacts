#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_014 {
// Harness 014: tinygltf filesystem and URI callback fuzzer
// Targets: SetFsCallbacks, SetURICallbacks, and related callback functions
// Focus: Custom callback infrastructure for filesystem and URI operations with 18+ undiscovered branches
// Differentiates from harness_013 (stb_image callbacks) by targeting filesystem/URI infrastructure
// Exercises setup-registration-usage lifecycle with various error conditions and edge cases
// Uses FuzzedDataProvider for structured input splitting


// Include FuzzedDataProvider for structured input splitting

// Include tinygltf header - DO NOT define TINYGLTF_IMPLEMENTATION
// since we're linking against libtinygltf.a which already has implementations

// Custom user data structure for callback context
struct CallbackUserData {
    FuzzedDataProvider* fdp;
    int error_mode; // 0: normal, 1: fail operations, 2: return invalid data, 3: partial success
    bool simulate_file_exists;
    bool simulate_large_file;
    size_t file_size_limit;
    std::vector<uint8_t> file_content;
};

// Custom FileExists callback
static bool CustomFileExists(const std::string& abs_filename, void* user_data) {
    CallbackUserData* ctx = reinterpret_cast<CallbackUserData*>(user_data);
    if (!ctx) return false;
    
    // Simulate different behaviors based on error_mode
    switch (ctx->error_mode) {
        case 1: // Fail operations
            return false;
        case 2: // Return invalid data (always true even for non-existent files)
            return true;
        case 3: // Partial success
            if (ctx->fdp && ctx->fdp->ConsumeBool()) {
                return false;
            }
            // Fall through
        default: // Normal mode
            return ctx->simulate_file_exists;
    }
}

// Custom ExpandFilePath callback
static std::string CustomExpandFilePath(const std::string& filepath, void* user_data) {
    CallbackUserData* ctx = reinterpret_cast<CallbackUserData*>(user_data);
    if (!ctx) return filepath;
    
    // Simulate different behaviors based on error_mode
    switch (ctx->error_mode) {
        case 1: // Fail operations - return empty string
            return "";
        case 2: // Return invalid data
            return "invalid://path/" + filepath;
        case 3: // Partial success
            if (ctx->fdp && ctx->fdp->ConsumeBool()) {
                return "";
            }
            // Fall through
        default: // Normal mode
            // Simple path expansion simulation
            if (filepath.find("~/") == 0) {
                return "/home/user/" + filepath.substr(2);
            }
            if (filepath.find("%TEMP%") != std::string::npos) {
                std::string result = filepath;
                size_t pos = result.find("%TEMP%");
                result.replace(pos, 6, "/tmp");
                return result;
            }
            return filepath;
    }
}

// Custom ReadWholeFile callback
static bool CustomReadWholeFile(std::vector<unsigned char>* out, std::string* err,
                                const std::string& filepath, void* user_data) {
    CallbackUserData* ctx = reinterpret_cast<CallbackUserData*>(user_data);
    if (!ctx || !out || !err) return false;
    
    // Simulate different behaviors based on error_mode
    switch (ctx->error_mode) {
        case 1: // Fail operations
            *err = "Simulated read failure";
            return false;
        case 2: // Return invalid data
            out->clear();
            if (ctx->fdp) {
                size_t invalid_size = ctx->fdp->ConsumeIntegralInRange<size_t>(0, 1000);
                out->resize(invalid_size, 0xFF); // Fill with 0xFF
            }
            return true; // Report success but with invalid data
        case 3: // Partial success
            if (ctx->fdp && ctx->fdp->ConsumeBool()) {
                *err = "Partial failure";
                return false;
            }
            // Fall through
        default: // Normal mode
            if (ctx->simulate_large_file && ctx->file_size_limit > 0) {
                // Simulate reading a large file
                size_t read_size = std::min(ctx->file_size_limit, ctx->file_content.size());
                out->assign(ctx->file_content.begin(), ctx->file_content.begin() + read_size);
            } else {
                // Use pre-loaded content
                *out = ctx->file_content;
            }
            return true;
    }
}

// Custom WriteWholeFile callback
static bool CustomWriteWholeFile(std::string* err, const std::string& filepath,
                                 const std::vector<unsigned char>& contents, void* user_data) {
    CallbackUserData* ctx = reinterpret_cast<CallbackUserData*>(user_data);
    if (!ctx || !err) return false;
    
    // Simulate different behaviors based on error_mode
    switch (ctx->error_mode) {
        case 1: // Fail operations
            *err = "Simulated write failure";
            return false;
        case 2: // Return invalid data - always succeed but don't actually write
            return true;
        case 3: // Partial success
            if (ctx->fdp && ctx->fdp->ConsumeBool()) {
                *err = "Partial write failure";
                return false;
            }
            // Fall through
        default: // Normal mode
            // Store written content for verification (simulated)
            ctx->file_content = contents;
            return true;
    }
}

// Custom GetFileSizeInBytes callback
static bool CustomGetFileSizeInBytes(size_t* filesize_out, std::string* err,
                                     const std::string& abs_filename, void* user_data) {
    CallbackUserData* ctx = reinterpret_cast<CallbackUserData*>(user_data);
    if (!ctx || !filesize_out || !err) return false;
    
    // Simulate different behaviors based on error_mode
    switch (ctx->error_mode) {
        case 1: // Fail operations
            *err = "Simulated size query failure";
            return false;
        case 2: // Return invalid data
            *filesize_out = ctx->fdp ? ctx->fdp->ConsumeIntegral<size_t>() : 0;
            return true;
        case 3: // Partial success
            if (ctx->fdp && ctx->fdp->ConsumeBool()) {
                *err = "Partial size query failure";
                return false;
            }
            // Fall through
        default: // Normal mode
            if (ctx->simulate_large_file) {
                *filesize_out = ctx->file_size_limit;
            } else {
                *filesize_out = ctx->file_content.size();
            }
            return true;
    }
}

// Custom URI encode callback
static bool CustomURIEncode(const std::string& in_uri, const std::string& object_type,
                            std::string* out_uri, void* user_data) {
    CallbackUserData* ctx = reinterpret_cast<CallbackUserData*>(user_data);
    if (!ctx || !out_uri) return false;
    
    // Simulate different behaviors based on error_mode
    switch (ctx->error_mode) {
        case 1: // Fail operations
            return false;
        case 2: // Return invalid data
            *out_uri = "invalid://" + in_uri;
            return true;
        case 3: // Partial success
            if (ctx->fdp && ctx->fdp->ConsumeBool()) {
                return false;
            }
            // Fall through
        default: // Normal mode
            // Simple encoding simulation - just prefix with "encoded://"
            *out_uri = "encoded://" + object_type + "/" + in_uri;
            return true;
    }
}

// Custom URI decode callback
static bool CustomURIDecode(const std::string& in_uri, std::string* out_uri, void* user_data) {
    CallbackUserData* ctx = reinterpret_cast<CallbackUserData*>(user_data);
    if (!ctx || !out_uri) return false;
    
    // Simulate different behaviors based on error_mode
    switch (ctx->error_mode) {
        case 1: // Fail operations
            return false;
        case 2: // Return invalid data
            *out_uri = "malformed/path/" + in_uri;
            return true;
        case 3: // Partial success
            if (ctx->fdp && ctx->fdp->ConsumeBool()) {
                return false;
            }
            // Fall through
        default: // Normal mode
            // Simple decoding simulation
            if (in_uri.find("encoded://") == 0) {
                *out_uri = in_uri.substr(10); // Remove "encoded://" prefix
                size_t slash_pos = out_uri->find('/');
                if (slash_pos != std::string::npos) {
                    *out_uri = out_uri->substr(slash_pos + 1);
                }
            } else if (in_uri.find("data:") == 0) {
                // Handle data URIs - just extract a simple representation
                size_t comma_pos = in_uri.find(',');
                if (comma_pos != std::string::npos) {
                    *out_uri = "data_content_" + in_uri.substr(comma_pos + 1, 10);
                } else {
                    *out_uri = in_uri;
                }
            } else {
                *out_uri = in_uri;
            }
            return true;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for callback configuration
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create TinyGLTF context
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err, warn;
    
    // Setup callback user data
    CallbackUserData user_data;
    user_data.fdp = &fdp;
    user_data.error_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
    user_data.simulate_file_exists = fdp.ConsumeBool();
    user_data.simulate_large_file = fdp.ConsumeBool();
    user_data.file_size_limit = fdp.ConsumeIntegralInRange<size_t>(0, 10000);
    
    // Consume some file content from input
    size_t file_content_size = fdp.ConsumeIntegralInRange<size_t>(0, 1000);
    user_data.file_content = fdp.ConsumeBytes<uint8_t>(file_content_size);
    
    // Consume operation selector (0-3 for different callback configuration paths)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Test different callback configuration scenarios
    bool setup_success = false;
    
    switch (operation) {
        case 0: // Test FsCallbacks only
        {
            tinygltf::FsCallbacks fs_callbacks;
            fs_callbacks.FileExists = CustomFileExists;
            fs_callbacks.ExpandFilePath = CustomExpandFilePath;
            fs_callbacks.ReadWholeFile = CustomReadWholeFile;
            fs_callbacks.WriteWholeFile = CustomWriteWholeFile;
            fs_callbacks.GetFileSizeInBytes = CustomGetFileSizeInBytes;
            fs_callbacks.user_data = &user_data;
            
            setup_success = ctx.SetFsCallbacks(fs_callbacks, &err);
            
            // Try to use the callbacks with a simple file operation simulation
            if (setup_success) {
                // Test file existence check
                std::string test_filename = fdp.ConsumeRandomLengthString(64);
                bool file_exists = fs_callbacks.FileExists(test_filename, &user_data);
                (void)file_exists;
                
                // Test path expansion
                std::string expanded = fs_callbacks.ExpandFilePath(test_filename, &user_data);
                (void)expanded;
            }
            break;
        }
        
        case 1: // Test URICallbacks only
        {
            tinygltf::URICallbacks uri_callbacks;
            uri_callbacks.encode = CustomURIEncode;
            uri_callbacks.decode = CustomURIDecode;
            uri_callbacks.user_data = &user_data;
            
            setup_success = ctx.SetURICallbacks(uri_callbacks, &err);
            
            // Try to use the URI callbacks
            if (setup_success) {
                std::string test_uri = fdp.ConsumeRandomLengthString(128);
                std::string object_type = fdp.ConsumeRandomLengthString(32);
                std::string encoded_uri, decoded_uri;
                
                // Test encoding
                if (uri_callbacks.encode) {
                    bool encode_success = uri_callbacks.encode(test_uri, object_type, &encoded_uri, &user_data);
                    (void)encode_success;
                }
                
                // Test decoding
                if (uri_callbacks.decode) {
                    bool decode_success = uri_callbacks.decode(test_uri, &decoded_uri, &user_data);
                    (void)decode_success;
                }
            }
            break;
        }
        
        case 2: // Test both FsCallbacks and URICallbacks
        {
            tinygltf::FsCallbacks fs_callbacks;
            fs_callbacks.FileExists = CustomFileExists;
            fs_callbacks.ExpandFilePath = CustomExpandFilePath;
            fs_callbacks.ReadWholeFile = CustomReadWholeFile;
            fs_callbacks.WriteWholeFile = CustomWriteWholeFile;
            fs_callbacks.GetFileSizeInBytes = CustomGetFileSizeInBytes;
            fs_callbacks.user_data = &user_data;
            
            tinygltf::URICallbacks uri_callbacks;
            uri_callbacks.encode = CustomURIEncode;
            uri_callbacks.decode = CustomURIDecode;
            uri_callbacks.user_data = &user_data;
            
            bool fs_success = ctx.SetFsCallbacks(fs_callbacks, &err);
            bool uri_success = ctx.SetURICallbacks(uri_callbacks, &err);
            setup_success = fs_success && uri_success;
            
            // Test combined usage
            if (setup_success) {
                // Create a simple test scenario that uses both callback types
                std::string filename = fdp.ConsumeRandomLengthString(64);
                std::string uri = fdp.ConsumeRandomLengthString(128);
                
                // Check if file exists
                bool exists = fs_callbacks.FileExists(filename, &user_data);
                (void)exists;
                
                // Try to decode URI
                std::string decoded;
                if (uri_callbacks.decode) {
                    bool decode_ok = uri_callbacks.decode(uri, &decoded, &user_data);
                    (void)decode_ok;
                }
            }
            break;
        }
        
        case 3: // Test error conditions - invalid callback configurations
        {
            // Test 1: FsCallbacks with some null callbacks (should fail)
            tinygltf::FsCallbacks invalid_fs_callbacks;
            invalid_fs_callbacks.FileExists = CustomFileExists;
            invalid_fs_callbacks.ExpandFilePath = CustomExpandFilePath;
            invalid_fs_callbacks.ReadWholeFile = nullptr; // Invalid - should cause failure
            invalid_fs_callbacks.WriteWholeFile = CustomWriteWholeFile;
            invalid_fs_callbacks.GetFileSizeInBytes = CustomGetFileSizeInBytes;
            invalid_fs_callbacks.user_data = &user_data;
            
            bool fs_failure = !ctx.SetFsCallbacks(invalid_fs_callbacks, &err);
            (void)fs_failure;
            
            // Test 2: URICallbacks with null decode (should fail)
            tinygltf::URICallbacks invalid_uri_callbacks;
            invalid_uri_callbacks.encode = CustomURIEncode;
            invalid_uri_callbacks.decode = nullptr; // Invalid - decode is required
            invalid_uri_callbacks.user_data = &user_data;
            
            bool uri_failure = !ctx.SetURICallbacks(invalid_uri_callbacks, &err);
            (void)uri_failure;
            
            // Test 3: Valid callbacks after invalid ones should work
            if (fdp.ConsumeBool()) {
                tinygltf::FsCallbacks valid_fs_callbacks;
                valid_fs_callbacks.FileExists = CustomFileExists;
                valid_fs_callbacks.ExpandFilePath = CustomExpandFilePath;
                valid_fs_callbacks.ReadWholeFile = CustomReadWholeFile;
                valid_fs_callbacks.WriteWholeFile = CustomWriteWholeFile;
                valid_fs_callbacks.GetFileSizeInBytes = CustomGetFileSizeInBytes;
                valid_fs_callbacks.user_data = &user_data;
                
                setup_success = ctx.SetFsCallbacks(valid_fs_callbacks, &err);
            }
            break;
        }
    }
    
    // If callbacks were successfully set, try to load a simple model
    // to exercise the callback integration
    if (setup_success && fdp.remaining_bytes() > 100) {
        // Create a minimal GLTF JSON that might trigger callback usage
        std::string json_test = "{\"asset\":{\"version\":\"2.0\"}}";
        std::string remaining_input = fdp.ConsumeRemainingBytesAsString();
        
        if (!remaining_input.empty() && remaining_input.size() < 10000) {
            // Try loading with our custom callbacks
            bool load_result = ctx.LoadASCIIFromString(&model, &err, &warn,
                                                      remaining_input.c_str(),
                                                      static_cast<unsigned int>(remaining_input.size()),
                                                      "");
            (void)load_result;
            
            // Also try binary loading if we have enough data
            if (fdp.remaining_bytes() > 50) {
                std::vector<uint8_t> binary_test = fdp.ConsumeRemainingBytes<uint8_t>();
                if (!binary_test.empty()) {
                    bool binary_result = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                                 binary_test.data(),
                                                                 static_cast<unsigned int>(binary_test.size()),
                                                                 "");
                    (void)binary_result;
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_014
