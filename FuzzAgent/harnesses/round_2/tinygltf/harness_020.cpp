#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <algorithm>
#include <iostream>
#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"

namespace harness_020 {
// Harness 020: Advanced tinygltf callback infrastructure fuzzer
// Targets: SetFsCallbacks, SetURICallbacks, SetImageLoader, SetImageWriter
// Focus: Deep callback infrastructure coverage with 27+ undiscovered branches
// Differentiates from harness_014 by:
//   - Adding image loader/writer callbacks (SetImageLoader, SetImageWriter)
//   - More complex stateful callback behavior with shared context
//   - Nested callback scenarios and callback chaining
//   - Advanced error recovery and callback reconfiguration
//   - Interaction between all three callback types (fs, uri, image)
// Uses FuzzedDataProvider for structured input splitting and dynamic behavior control


// Include FuzzedDataProvider for structured input splitting

// Include tinygltf header - DO NOT define TINYGLTF_IMPLEMENTATION
// since we're linking against libtinygltf.a which already has implementations

// Advanced shared callback context with stateful behavior
struct AdvancedCallbackContext {
    FuzzedDataProvider* fdp;
    
    // State tracking for callbacks
    struct {
        int fs_call_count = 0;
        int uri_call_count = 0;
        int image_load_count = 0;
        int image_write_count = 0;
        std::map<std::string, std::vector<uint8_t>> virtual_filesystem;
        std::map<std::string, std::string> uri_mappings;
        std::map<std::string, std::vector<uint8_t>> image_cache;
    } state;
    
    // Configuration from fuzzer input
    struct {
        int error_mode; // 0: normal, 1: fail ops, 2: return invalid, 3: partial, 4: stateful
        bool simulate_large_files;
        bool enable_image_caching;
        bool shared_user_data; // Use same user_data for all callbacks
        bool nested_callbacks; // Callbacks that call other callbacks
        size_t max_file_size;
        size_t max_image_size;
        int max_recursion_depth;
    } config;
    
    // Derived data from fuzzer input
    std::vector<uint8_t> default_file_content;
    std::vector<uint8_t> default_image_data;
    std::string base_path;
    
    // Recursion tracking
    int current_recursion_depth = 0;
    
    AdvancedCallbackContext(FuzzedDataProvider* f) : fdp(f) {
        // Initialize configuration from fuzzer input
        config.error_mode = fdp->ConsumeIntegralInRange<int>(0, 4);
        config.simulate_large_files = fdp->ConsumeBool();
        config.enable_image_caching = fdp->ConsumeBool();
        config.shared_user_data = fdp->ConsumeBool();
        config.nested_callbacks = fdp->ConsumeBool();
        config.max_file_size = fdp->ConsumeIntegralInRange<size_t>(0, 10000);
        config.max_image_size = fdp->ConsumeIntegralInRange<size_t>(0, 50000);
        config.max_recursion_depth = fdp->ConsumeIntegralInRange<int>(0, 3);
        
        // Consume some content for testing
        size_t file_content_size = fdp->ConsumeIntegralInRange<size_t>(0, 1000);
        default_file_content = fdp->ConsumeBytes<uint8_t>(file_content_size);
        
        size_t image_content_size = fdp->ConsumeIntegralInRange<size_t>(0, 2000);
        default_image_data = fdp->ConsumeBytes<uint8_t>(image_content_size);
        
        base_path = fdp->ConsumeRandomLengthString(32);
    }
    
    // Helper to check recursion limits
    bool enter_recursion() {
        if (current_recursion_depth >= config.max_recursion_depth) {
            return false;
        }
        current_recursion_depth++;
        return true;
    }
    
    void exit_recursion() {
        if (current_recursion_depth > 0) {
            current_recursion_depth--;
        }
    }
};

// Advanced FileExists callback with virtual filesystem
static bool AdvancedFileExists(const std::string& abs_filename, void* user_data) {
    AdvancedCallbackContext* ctx = reinterpret_cast<AdvancedCallbackContext*>(user_data);
    if (!ctx) return false;
    
    ctx->state.fs_call_count++;
    
    // Handle error modes
    switch (ctx->config.error_mode) {
        case 1: // Fail operations
            return false;
        case 2: // Return invalid data (always true)
            return true;
        case 3: // Partial success
            if (ctx->fdp->ConsumeBool()) {
                return false;
            }
            break;
        case 4: // Stateful behavior
            if (ctx->state.fs_call_count % 3 == 0) {
                return false; // Fail every 3rd call
            }
            break;
    }
    
    // Check virtual filesystem
    return ctx->state.virtual_filesystem.find(abs_filename) != ctx->state.virtual_filesystem.end();
}

// Advanced ExpandFilePath callback with environment variables
static std::string AdvancedExpandFilePath(const std::string& filepath, void* user_data) {
    AdvancedCallbackContext* ctx = reinterpret_cast<AdvancedCallbackContext*>(user_data);
    if (!ctx) return filepath;
    
    ctx->state.fs_call_count++;
    
    // Handle error modes
    if (ctx->config.error_mode == 1) {
        return ""; // Fail
    }
    
    std::string result = filepath;
    
    // Expand various path patterns
    if (result.find("${BASE}") != std::string::npos) {
        size_t pos = result.find("${BASE}");
        result.replace(pos, 7, ctx->base_path);
    }
    
    if (result.find("~/") == 0) {
        result = "/home/user/" + result.substr(2);
    }
    
    if (result.find("%TEMP%") != std::string::npos) {
        size_t pos = result.find("%TEMP%");
        result.replace(pos, 6, "/tmp");
    }
    
    // Add random suffix for variability
    if (ctx->fdp->ConsumeBool()) {
        result += "_" + ctx->fdp->ConsumeRandomLengthString(4);
    }
    
    return result;
}

// Advanced ReadWholeFile callback with virtual filesystem
static bool AdvancedReadWholeFile(std::vector<unsigned char>* out, std::string* err,
                                 const std::string& filepath, void* user_data) {
    AdvancedCallbackContext* ctx = reinterpret_cast<AdvancedCallbackContext*>(user_data);
    if (!ctx || !out || !err) return false;
    
    ctx->state.fs_call_count++;
    
    // Handle error modes
    switch (ctx->config.error_mode) {
        case 1: // Fail operations
            *err = "Simulated read failure";
            return false;
        case 2: { // Return invalid data
            out->clear();
            size_t invalid_size = ctx->fdp->ConsumeIntegralInRange<size_t>(0, 1000);
            out->resize(invalid_size, 0xFF);
            return true;
        }
        case 3: // Partial success
            if (ctx->fdp->ConsumeBool()) {
                *err = "Partial read failure";
                return false;
            }
            break;
    }
    
    // Check virtual filesystem
    auto it = ctx->state.virtual_filesystem.find(filepath);
    if (it != ctx->state.virtual_filesystem.end()) {
        *out = std::vector<unsigned char>(it->second.begin(), it->second.end());
        return true;
    }
    
    // Return default content for new files
    if (!ctx->default_file_content.empty()) {
        size_t read_size = std::min(ctx->default_file_content.size(), 
                                   ctx->config.simulate_large_files ? ctx->config.max_file_size : 
                                   ctx->default_file_content.size());
        out->assign(ctx->default_file_content.begin(), 
                   ctx->default_file_content.begin() + read_size);
        return true;
    }
    
    *err = "File not found in virtual filesystem";
    return false;
}

// Advanced WriteWholeFile callback with virtual filesystem
static bool AdvancedWriteWholeFile(std::string* err, const std::string& filepath,
                                  const std::vector<unsigned char>& contents, void* user_data) {
    AdvancedCallbackContext* ctx = reinterpret_cast<AdvancedCallbackContext*>(user_data);
    if (!ctx || !err) return false;
    
    ctx->state.fs_call_count++;
    
    // Handle error modes
    switch (ctx->config.error_mode) {
        case 1: // Fail operations
            *err = "Simulated write failure";
            return false;
        case 2: // Return invalid data but succeed
            return true;
        case 3: // Partial success
            if (ctx->fdp->ConsumeBool()) {
                *err = "Partial write failure";
                return false;
            }
            break;
        case 4: // Stateful - fail on large files
            if (contents.size() > ctx->config.max_file_size) {
                *err = "File too large";
                return false;
            }
            break;
    }
    
    // Store in virtual filesystem
    ctx->state.virtual_filesystem[filepath] = 
        std::vector<uint8_t>(contents.begin(), contents.end());
    
    return true;
}

// Advanced GetFileSizeInBytes callback
static bool AdvancedGetFileSizeInBytes(size_t* filesize_out, std::string* err,
                                      const std::string& abs_filename, void* user_data) {
    AdvancedCallbackContext* ctx = reinterpret_cast<AdvancedCallbackContext*>(user_data);
    if (!ctx || !filesize_out || !err) return false;
    
    ctx->state.fs_call_count++;
    
    // Handle error modes
    if (ctx->config.error_mode == 1) {
        *err = "Simulated size query failure";
        return false;
    }
    
    // Check virtual filesystem
    auto it = ctx->state.virtual_filesystem.find(abs_filename);
    if (it != ctx->state.virtual_filesystem.end()) {
        *filesize_out = it->second.size();
        return true;
    }
    
    // Return default size
    *filesize_out = ctx->default_file_content.size();
    return true;
}

// Advanced URI encode callback with stateful mappings
static bool AdvancedURIEncode(const std::string& in_uri, const std::string& object_type,
                             std::string* out_uri, void* user_data) {
    AdvancedCallbackContext* ctx = reinterpret_cast<AdvancedCallbackContext*>(user_data);
    if (!ctx || !out_uri) return false;
    
    ctx->state.uri_call_count++;
    
    // Handle error modes
    if (ctx->config.error_mode == 1) {
        return false;
    }
    
    // Check if we should use nested callbacks
    if (ctx->config.nested_callbacks && ctx->enter_recursion()) {
        // Simulate a callback that calls ExpandFilePath
        std::string expanded = AdvancedExpandFilePath(in_uri, user_data);
        *out_uri = "nested://" + object_type + "/" + expanded;
        ctx->exit_recursion();
    } else {
        // Create encoded URI
        std::string encoded = "encoded://" + object_type + "/" + in_uri;
        
        // Store mapping for stateful behavior
        if (ctx->config.error_mode == 4) {
            ctx->state.uri_mappings[encoded] = in_uri;
        }
        
        *out_uri = encoded;
    }
    
    return true;
}

// Advanced URI decode callback with stateful mappings
static bool AdvancedURIDecode(const std::string& in_uri, std::string* out_uri, void* user_data) {
    AdvancedCallbackContext* ctx = reinterpret_cast<AdvancedCallbackContext*>(user_data);
    if (!ctx || !out_uri) return false;
    
    ctx->state.uri_call_count++;
    
    // Handle error modes
    if (ctx->config.error_mode == 1) {
        return false;
    }
    
    // Check stateful mappings first
    if (ctx->config.error_mode == 4) {
        auto it = ctx->state.uri_mappings.find(in_uri);
        if (it != ctx->state.uri_mappings.end()) {
            *out_uri = it->second;
            return true;
        }
    }
    
    // Decode based on URI pattern
    if (in_uri.find("encoded://") == 0) {
        size_t start = 10; // Skip "encoded://"
        size_t slash = in_uri.find('/', start);
        if (slash != std::string::npos) {
            *out_uri = in_uri.substr(slash + 1);
            return true;
        }
    } else if (in_uri.find("data:") == 0) {
        // Simple data URI handling
        size_t comma = in_uri.find(',');
        if (comma != std::string::npos) {
            *out_uri = "data_content_" + in_uri.substr(comma + 1, 10);
            return true;
        }
    } else if (in_uri.find("nested://") == 0) {
        // Handle nested URIs
        *out_uri = in_uri.substr(9); // Remove "nested://"
        return true;
    }
    
    // Default: return as-is
    *out_uri = in_uri;
    return true;
}

// Advanced image loader callback
static bool AdvancedLoadImageData(tinygltf::Image* image, const int image_idx, std::string* err,
                                 std::string* warn, int req_width, int req_height,
                                 const unsigned char* bytes, int size, void* user_data) {
    AdvancedCallbackContext* ctx = reinterpret_cast<AdvancedCallbackContext*>(user_data);
    if (!ctx || !image || !err) return false;
    
    ctx->state.image_load_count++;
    
    // Handle error modes
    switch (ctx->config.error_mode) {
        case 1: // Fail operations
            *err = "Simulated image load failure";
            return false;
        case 2: // Return invalid image data
            image->width = image->height = 1;
            image->component = 4;
            image->image.resize(4, 0xFF); // Single white pixel
            return true;
        case 3: // Partial success
            if (ctx->fdp->ConsumeBool()) {
                *err = "Partial image load failure";
                return false;
            }
            break;
    }
    
    // Check image cache
    std::string cache_key = std::to_string(image_idx) + "_" + std::to_string(size);
    if (ctx->config.enable_image_caching) {
        auto it = ctx->state.image_cache.find(cache_key);
        if (it != ctx->state.image_cache.end()) {
            image->image.assign(it->second.begin(), it->second.end());
            image->width = 64;
            image->height = 64;
            image->component = 4;
            return true;
        }
    }
    
    // Simulate image loading
    if (size > 0) {
        // Create a simple image from input data
        size_t image_size = std::min(static_cast<size_t>(size), 
                                    ctx->config.max_image_size);
        image->image.resize(image_size);
        
        // Use input bytes for image data
        size_t copy_size = std::min(image_size, static_cast<size_t>(size));
        if (bytes && copy_size > 0) {
            std::copy(bytes, bytes + copy_size, image->image.begin());
        }
        
        // Set image properties
        image->width = req_width > 0 ? req_width : 64;
        image->height = req_height > 0 ? req_height : 64;
        image->component = 4;
        
        // Cache if enabled
        if (ctx->config.enable_image_caching) {
            ctx->state.image_cache[cache_key] = image->image;
        }
        
        return true;
    }
    
    // Fallback to default image
    if (!ctx->default_image_data.empty()) {
        size_t image_size = std::min(ctx->default_image_data.size(),
                                    ctx->config.max_image_size);
        image->image.assign(ctx->default_image_data.begin(),
                          ctx->default_image_data.begin() + image_size);
        image->width = 64;
        image->height = 64;
        image->component = 4;
        return true;
    }
    
    *err = "No image data available";
    return false;
}

// Advanced image writer callback
static bool AdvancedWriteImageData(const std::string* basepath, const std::string* filename,
                                  const tinygltf::Image* image, bool embedImages,
                                  const tinygltf::FsCallbacks* fs_cb,
                                  const tinygltf::URICallbacks* uri_cb,
                                  std::string* out_uri, void* user_data) {
    AdvancedCallbackContext* ctx = reinterpret_cast<AdvancedCallbackContext*>(user_data);
    if (!ctx || !out_uri) return false;
    
    ctx->state.image_write_count++;
    
    // Handle error modes
    if (ctx->config.error_mode == 1) {
        return false;
    }
    
    // Check if we should use nested callbacks
    if (ctx->config.nested_callbacks && uri_cb && uri_cb->decode && ctx->enter_recursion()) {
        // Create a URI and then decode it
        std::string test_uri = "image://" + (filename ? *filename : "unknown");
        std::string decoded;
        if (uri_cb->decode(test_uri, &decoded, user_data)) {
            *out_uri = "nested_written://" + decoded;
        } else {
            *out_uri = "written://" + (filename ? *filename : "image");
        }
        ctx->exit_recursion();
    } else {
        // Generate output URI
        if (embedImages) {
            // Create data URI
            *out_uri = "data:image/png;base64," + 
                      ctx->fdp->ConsumeRandomLengthString(20);
        } else {
            // Create file URI
            std::string fname = filename ? *filename : "output.png";
            if (basepath && !basepath->empty()) {
                *out_uri = *basepath + "/" + fname;
            } else {
                *out_uri = fname;
            }
        }
    }
    
    return true;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for advanced configuration
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create TinyGLTF context
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err, warn;
    
    // Create advanced callback context
    AdvancedCallbackContext callback_ctx(&fdp);
    
    // Consume operation selector for different test scenarios
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    bool setup_success = false;
    bool use_image_callbacks = false;
    bool use_shared_context = callback_ctx.config.shared_user_data;
    
    // Setup callback structures based on operation
    tinygltf::FsCallbacks fs_callbacks;
    tinygltf::URICallbacks uri_callbacks;
    
    // Always set up FS callbacks
    fs_callbacks.FileExists = AdvancedFileExists;
    fs_callbacks.ExpandFilePath = AdvancedExpandFilePath;
    fs_callbacks.ReadWholeFile = AdvancedReadWholeFile;
    fs_callbacks.WriteWholeFile = AdvancedWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = AdvancedGetFileSizeInBytes;
    fs_callbacks.user_data = &callback_ctx;
    
    // Always set up URI callbacks  
    uri_callbacks.encode = AdvancedURIEncode;
    uri_callbacks.decode = AdvancedURIDecode;
    uri_callbacks.user_data = &callback_ctx;
    
    // Test different callback configuration scenarios
    switch (operation) {
        case 0: // Basic: FS + URI callbacks only
            setup_success = ctx.SetFsCallbacks(fs_callbacks, &err) &&
                          ctx.SetURICallbacks(uri_callbacks, &err);
            break;
            
        case 1: // With image loader only
            ctx.SetImageLoader(AdvancedLoadImageData, &callback_ctx);
            setup_success = ctx.SetFsCallbacks(fs_callbacks, &err) &&
                          ctx.SetURICallbacks(uri_callbacks, &err);
            use_image_callbacks = true;
            break;
            
        case 2: // With image writer only
            ctx.SetImageWriter(AdvancedWriteImageData, &callback_ctx);
            setup_success = ctx.SetFsCallbacks(fs_callbacks, &err) &&
                          ctx.SetURICallbacks(uri_callbacks, &err);
            use_image_callbacks = true;
            break;
            
        case 3: // With both image loader and writer
            ctx.SetImageLoader(AdvancedLoadImageData, &callback_ctx);
            ctx.SetImageWriter(AdvancedWriteImageData, &callback_ctx);
            setup_success = ctx.SetFsCallbacks(fs_callbacks, &err) &&
                          ctx.SetURICallbacks(uri_callbacks, &err);
            use_image_callbacks = true;
            break;
            
        case 4: // Test error conditions with partial callbacks
            // Test invalid FS callbacks (missing ReadWholeFile)
            {
                tinygltf::FsCallbacks invalid_fs = fs_callbacks;
                invalid_fs.ReadWholeFile = nullptr;
                bool should_fail = !ctx.SetFsCallbacks(invalid_fs, &err);
                (void)should_fail;
                
                // Then set valid ones
                setup_success = ctx.SetFsCallbacks(fs_callbacks, &err) &&
                              ctx.SetURICallbacks(uri_callbacks, &err);
            }
            break;
            
        case 5: // Test callback reconfiguration
            // Set, then remove, then set again
            if (ctx.SetFsCallbacks(fs_callbacks, &err) &&
                ctx.SetURICallbacks(uri_callbacks, &err)) {
                
                // Remove image loader if it was set
                ctx.RemoveImageLoader();
                
                // Reconfigure with image callbacks
                if (fdp.ConsumeBool()) {
                    ctx.SetImageLoader(AdvancedLoadImageData, &callback_ctx);
                    use_image_callbacks = true;
                }
                
                setup_success = true;
            }
            break;
            
        case 6: // Test with shared/null user_data scenarios
            if (use_shared_context) {
                // All callbacks use same context
                setup_success = ctx.SetFsCallbacks(fs_callbacks, &err) &&
                              ctx.SetURICallbacks(uri_callbacks, &err);
                
                if (fdp.ConsumeBool()) {
                    ctx.SetImageLoader(AdvancedLoadImageData, &callback_ctx);
                    use_image_callbacks = true;
                }
            } else {
                // Test with null user_data for some callbacks
                tinygltf::FsCallbacks fs_no_user = fs_callbacks;
                fs_no_user.user_data = nullptr;
                
                setup_success = ctx.SetFsCallbacks(fs_no_user, &err) &&
                              ctx.SetURICallbacks(uri_callbacks, &err);
            }
            break;
            
        case 7: // Complex: All callbacks with stateful operations
            ctx.SetImageLoader(AdvancedLoadImageData, &callback_ctx);
            ctx.SetImageWriter(AdvancedWriteImageData, &callback_ctx);
            setup_success = ctx.SetFsCallbacks(fs_callbacks, &err) &&
                          ctx.SetURICallbacks(uri_callbacks, &err);
            use_image_callbacks = true;
            
            // Pre-populate virtual filesystem
            if (setup_success) {
                std::string test_file = "test.gltf";
                std::vector<uint8_t> gltf_content = fdp.ConsumeBytes<uint8_t>(
                    fdp.ConsumeIntegralInRange<size_t>(0, 500));
                callback_ctx.state.virtual_filesystem[test_file] = gltf_content;
            }
            break;
    }
    
    // If callbacks were successfully set, exercise them
    if (setup_success && fdp.remaining_bytes() > 50) {
        // Exercise FS callbacks
        std::string test_filename = fdp.ConsumeRandomLengthString(64);
        bool exists = fs_callbacks.FileExists(test_filename, &callback_ctx);
        (void)exists;
        
        std::string expanded = fs_callbacks.ExpandFilePath(test_filename, &callback_ctx);
        (void)expanded;
        
        // Exercise URI callbacks
        std::string test_uri = fdp.ConsumeRandomLengthString(128);
        std::string object_type = fdp.ConsumeRandomLengthString(32);
        std::string encoded_uri, decoded_uri;
        
        if (uri_callbacks.encode) {
            bool encode_ok = uri_callbacks.encode(test_uri, object_type, &encoded_uri, &callback_ctx);
            (void)encode_ok;
        }
        
        if (uri_callbacks.decode) {
            bool decode_ok = uri_callbacks.decode(test_uri, &decoded_uri, &callback_ctx);
            (void)decode_ok;
        }
        
        // Try to load a model to exercise integrated callback usage
        if (fdp.remaining_bytes() > 100) {
            std::string json_input = fdp.ConsumeRandomLengthString(
                fdp.ConsumeIntegralInRange<size_t>(50, 1000));
            
            bool load_result = ctx.LoadASCIIFromString(&model, &err, &warn,
                                                      json_input.c_str(),
                                                      static_cast<unsigned int>(json_input.size()),
                                                      "");
            (void)load_result;
            
            // Also try binary loading with remaining data
            if (fdp.remaining_bytes() > 50) {
                std::vector<uint8_t> binary_data = fdp.ConsumeRemainingBytes<uint8_t>();
                if (!binary_data.empty()) {
                    bool binary_result = ctx.LoadBinaryFromMemory(&model, &err, &warn,
                                                                 binary_data.data(),
                                                                 static_cast<unsigned int>(binary_data.size()),
                                                                 "");
                    (void)binary_result;
                }
            }
        }
        
        // Exercise image callbacks if they were set
        if (use_image_callbacks && fdp.remaining_bytes() > 20) {
            // Create a simple image and try to write it
            tinygltf::Image test_image;
            test_image.width = 64;
            test_image.height = 64;
            test_image.component = 4;
            test_image.image.resize(64 * 64 * 4, 0x80);
            
            std::string out_uri;
            bool write_ok = AdvancedWriteImageData(&callback_ctx.base_path, 
                                                  &test_filename, &test_image, 
                                                  fdp.ConsumeBool(),
                                                  &fs_callbacks, &uri_callbacks,
                                                  &out_uri, &callback_ctx);
            (void)write_ok;
        }
    }
    
    // Test error reporting and state inspection
    if (!err.empty()) {
        // Error was reported - this is OK for fuzzing
        (void)err.c_str();
    }
    
    if (!warn.empty()) {
        // Warning was reported - this is OK for fuzzing
        (void)warn.c_str();
    }
    
    return 0;
}

} // namespace harness_020
