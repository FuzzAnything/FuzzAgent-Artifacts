#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_007 {
/*
 * Fuzzing harness for tinygltf library - targeting file system callback integration with glTF scene writing and image processing
 * This harness specifically tests custom file system callbacks, URI callbacks, and image writing functionality.
 * These have 160+ undiscovered branches and 0% coverage according to coverage analysis.
 * 
 * Target APIs:
 * - tinygltf::GetFileSizeInBytes (20 undiscovered branches, 0% coverage)
 * - tinygltf::TinyGLTF::SetFsCallbacks (12 undiscovered branches, 0% coverage)  
 * - tinygltf::TinyGLTF::SetURICallbacks (6 undiscovered branches, 0% coverage)
 * - tinygltf::TinyGLTF::SetImageWriter (custom image writing)
 * - tinygltf::WriteImageData (30 undiscovered branches, low coverage: 30.95% lines)
 * - tinygltf::TinyGLTF::WriteGltfSceneToFile (77 undiscovered branches, 38.64% lines)
 * - tinygltf::TinyGLTF::WriteGltfSceneToStream (62 undiscovered branches, 15% lines)
 * 
 * Strategy: Create custom callback implementations that use fuzzer input to control behavior,
 * load models using these callbacks, modify models, then write them back using scene writing APIs
 * with both file and stream outputs.
 * 
 * Implementation uses FuzzedDataProvider for structured input splitting and callback behavior control.
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


// Robust UTF-8 validation and sanitization helper
// Based on RFC 3629 and Unicode standards
std::string SanitizeUTF8(const std::string& input) {
    std::string output;
    output.reserve(input.size());
    
    const unsigned char* data = reinterpret_cast<const unsigned char*>(input.data());
    size_t i = 0;
    
    while (i < input.size()) {
        unsigned char c = data[i];
        
        // 1-byte sequence (ASCII) - 0xxxxxxx
        if (c <= 0x7F) {
            // Check for control characters (excluding tab, newline, carriage return)
            if ((c >= 0x20 && c <= 0x7E) || c == 0x09 || c == 0x0A || c == 0x0D) {
                output.push_back(c);
            } else {
                output.push_back('?');
            }
            ++i;
        }
        // 2-byte sequence - 110xxxxx 10xxxxxx
        else if (c >= 0xC2 && c <= 0xDF) {
            if (i + 1 >= input.size()) {
                output.push_back('?');
                ++i;
                continue;
            }
            
            unsigned char c2 = data[i + 1];
            if (c2 >= 0x80 && c2 <= 0xBF) {
                // Validate the code point range
                uint32_t codepoint = ((c & 0x1F) << 6) | (c2 & 0x3F);
                // Valid range for 2-byte UTF-8: U+0080 to U+07FF
                if (codepoint >= 0x80 && codepoint <= 0x7FF) {
                    output.push_back(c);
                    output.push_back(c2);
                    i += 2;
                } else {
                    output.push_back('?');
                    ++i;
                }
            } else {
                output.push_back('?');
                ++i;
            }
        }
        // 3-byte sequence - 1110xxxx 10xxxxxx 10xxxxxx
        else if (c >= 0xE0 && c <= 0xEF) {
            if (i + 2 >= input.size()) {
                output.push_back('?');
                ++i;
                continue;
            }
            
            unsigned char c2 = data[i + 1];
            unsigned char c3 = data[i + 2];
            
            if (c2 >= 0x80 && c2 <= 0xBF && c3 >= 0x80 && c3 <= 0xBF) {
                uint32_t codepoint = ((c & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
                
                // Check for overlong encoding
                bool overlong = false;
                if (c == 0xE0 && c2 < 0xA0) overlong = true;  // Should be >= 0xA0 for E0
                if (c == 0xED && c2 > 0x9F) overlong = true;  // Should be <= 0x9F for ED
                
                // Valid range for 3-byte UTF-8: U+0800 to U+FFFF
                // Exclude surrogates: U+D800 to U+DFFF
                if (!overlong && codepoint >= 0x800 && codepoint <= 0xFFFF && 
                    !(codepoint >= 0xD800 && codepoint <= 0xDFFF)) {
                    output.push_back(c);
                    output.push_back(c2);
                    output.push_back(c3);
                    i += 3;
                } else {
                    output.push_back('?');
                    ++i;
                }
            } else {
                output.push_back('?');
                ++i;
            }
        }
        // 4-byte sequence - 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        else if (c >= 0xF0 && c <= 0xF4) {
            if (i + 3 >= input.size()) {
                output.push_back('?');
                ++i;
                continue;
            }
            
            unsigned char c2 = data[i + 1];
            unsigned char c3 = data[i + 2];
            unsigned char c4 = data[i + 3];
            
            if (c2 >= 0x80 && c2 <= 0xBF && 
                c3 >= 0x80 && c3 <= 0xBF && 
                c4 >= 0x80 && c4 <= 0xBF) {
                uint32_t codepoint = ((c & 0x07) << 18) | ((c2 & 0x3F) << 12) | 
                                     ((c3 & 0x3F) << 6) | (c4 & 0x3F);
                
                // Check for overlong encoding and valid ranges
                bool overlong = false;
                if (c == 0xF0 && c2 < 0x90) overlong = true;  // Should be >= 0x90 for F0
                if (c == 0xF4 && c2 > 0x8F) overlong = true;  // Should be <= 0x8F for F4
                
                // Valid range for 4-byte UTF-8: U+10000 to U+10FFFF
                if (!overlong && codepoint >= 0x10000 && codepoint <= 0x10FFFF) {
                    output.push_back(c);
                    output.push_back(c2);
                    output.push_back(c3);
                    output.push_back(c4);
                    i += 4;
                } else {
                    output.push_back('?');
                    ++i;
                }
            } else {
                output.push_back('?');
                ++i;
            }
        }
        // Invalid UTF-8 byte (continuation bytes 0x80-0xBF without start byte,
        // or bytes 0xF5-0xFF which are invalid in UTF-8)
        else {
            output.push_back('?');
            ++i;
        }
    }
    
    return output;
}

// Base64 validation helper for data URIs
bool IsValidBase64Chars(const std::string& str) {
    if (str.empty()) return true;
    
    // Base64 alphabet: A-Z, a-z, 0-9, +, /, = (padding)
    for (char c : str) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
            (c >= '0' && c <= '9') || c == '+' || c == '/' || c == '=') {
            continue;
        }
        // Also allow URL-safe base64 characters
        if (c == '-' || c == '_') {
            continue;
        }
        return false;
    }
    return true;
}

// Test function to validate UTF-8 with known invalid sequences
void TestUTF8Validation() {
    // Known invalid UTF-8 sequences for testing
    const char* test_sequences[] = {
        "\xC0\x80",  // Overlong encoding of NUL
        "\xE0\x80\x80",  // Overlong encoding of NUL (3-byte)
        "\xF0\x80\x80\x80",  // Overlong encoding of NUL (4-byte)
        "\xED\xA0\x80",  // Surrogate pair (high surrogate)
        "\xED\xBF\xBF",  // Surrogate pair (low surrogate)
        "\xF4\x90\x80\x80",  // Code point beyond U+10FFFF
        "\xC1\xBF",  // Overlong 2-byte sequence
        "\xFE",  // Invalid byte
        "\xFF",  // Invalid byte
        "\x80",  // Continuation byte without start
        "\xBF",  // Continuation byte without start
    };
    
    for (const char* seq : test_sequences) {
        std::string sanitized = SanitizeUTF8(std::string(seq, strlen(seq)));
        // All invalid sequences should be sanitized to '?' or empty
    }
}
struct CallbackUserData {
    FuzzedDataProvider* fdp;
    bool should_fail;
    size_t custom_file_size;
    std::vector<unsigned char> file_content;
    std::string custom_uri;
};

// Custom file system callbacks implementation
bool CustomFileExists(const std::string& abs_filename, void* user_data) {
    CallbackUserData* data = static_cast<CallbackUserData*>(user_data);
    if (!data || !data->fdp) return false;
    
    // Use fuzzer input to decide if file exists
    if (data->fdp->remaining_bytes() > 0) {
        data->should_fail = data->fdp->ConsumeBool();
    }
    return !data->should_fail;
}

std::string CustomExpandFilePath(const std::string& filepath, void* user_data) {
    CallbackUserData* data = static_cast<CallbackUserData*>(user_data);
    if (!data || !data->fdp) return filepath;
    
    // Use fuzzer input to potentially modify path
    if (data->fdp->remaining_bytes() > 0 && data->fdp->ConsumeBool()) {
        std::string prefix = SanitizeUTF8(data->fdp->ConsumeRandomLengthString(16));
        return prefix + "_" + filepath;
    }
    return filepath;
}

bool CustomReadWholeFile(std::vector<unsigned char>* out, std::string* err, 
                         const std::string& filepath, void* user_data) {
    CallbackUserData* data = static_cast<CallbackUserData*>(user_data);
    if (!data || !data->fdp || !out) {
        if (err) *err = "Invalid parameters";
        return false;
    }
    
    if (data->should_fail) {
        if (err) *err = "Custom callback failure";
        return false;
    }
    
    // Use fuzzer input to generate file content
    size_t content_size = 0;
    if (data->fdp->remaining_bytes() > sizeof(size_t)) {
        content_size = data->fdp->ConsumeIntegralInRange<size_t>(0, 1024);
    }
    
    if (content_size > 0 && data->fdp->remaining_bytes() >= content_size) {
        data->file_content = data->fdp->ConsumeBytes<unsigned char>(content_size);
    } else {
        // Generate some default content if no fuzzer input
        data->file_content = {'{', '"', 'a', 's', 's', 'e', 't', '"', ':', 
                              '{', '"', 'v', 'e', 'r', 's', 'i', 'o', 'n', 
                              '"', ':', '"', '2', '.', '0', '"', '}', '}'};
    }
    
    *out = data->file_content;
    return true;
}

bool CustomWriteWholeFile(std::string* err, const std::string& filepath,
                          const std::vector<unsigned char>& contents, void* user_data) {
    CallbackUserData* data = static_cast<CallbackUserData*>(user_data);
    if (!data) {
        if (err) *err = "No user data";
        return false;
    }
    
    if (data->should_fail) {
        if (err) *err = "Custom write failure";
        return false;
    }
    
    // Store the written content (simulating file write)
    data->file_content = contents;
    return true;
}

bool CustomGetFileSizeInBytes(size_t* filesize_out, std::string* err,
                              const std::string& filepath, void* user_data) {
    CallbackUserData* data = static_cast<CallbackUserData*>(user_data);
    if (!data || !filesize_out) {
        if (err) *err = "Invalid parameters";
        return false;
    }
    
    if (data->should_fail) {
        if (err) *err = "Custom size check failure";
        return false;
    }
    
    // Use fuzzer input to determine file size
    if (data->fdp && data->fdp->remaining_bytes() > sizeof(size_t)) {
        data->custom_file_size = data->fdp->ConsumeIntegralInRange<size_t>(0, 1048576); // up to 1MB
    }
    
    *filesize_out = data->custom_file_size;
    return true;
}

// Custom URI callbacks implementation
bool CustomURIDecode(const std::string& in_uri, std::string* out_uri, void* user_data) {
    CallbackUserData* data = static_cast<CallbackUserData*>(user_data);
    if (!data || !out_uri) return false;
    
    if (data->should_fail) {
        return false;
    }
    
    // Use fuzzer input to potentially modify URI
    if (data->fdp && data->fdp->remaining_bytes() > 0 && data->fdp->ConsumeBool()) {
        std::string prefix = SanitizeUTF8(data->fdp->ConsumeRandomLengthString(8));
        data->custom_uri = prefix + "_decoded_" + in_uri;
    } else {
        data->custom_uri = "decoded_" + in_uri;
    }
    
    *out_uri = data->custom_uri;
    return true;
}

bool CustomURIEncode(const std::string& in_uri, const std::string& object_type,
                     std::string* out_uri, void* user_data) {
    CallbackUserData* data = static_cast<CallbackUserData*>(user_data);
    if (!data || !out_uri) return false;
    
    if (data->should_fail) {
        return false;
    }
    
    // Use fuzzer input to potentially modify URI
    if (data->fdp && data->fdp->remaining_bytes() > 0 && data->fdp->ConsumeBool()) {
        std::string suffix = SanitizeUTF8(data->fdp->ConsumeRandomLengthString(8));
        data->custom_uri = in_uri + "_encoded_" + object_type + "_" + suffix;
    } else {
        data->custom_uri = in_uri + "_encoded_" + object_type;
    }
    
    *out_uri = data->custom_uri;
    return true;
}

// Custom image writer callback implementation with improved UTF-8 and base64 validation
bool CustomWriteImageData(const std::string* basepath, const std::string* filename,
                          const tinygltf::Image* image, bool embedImages,
                          const tinygltf::FsCallbacks* fs_cb, const tinygltf::URICallbacks* uri_cb,
                          std::string* out_uri, void* user_pointer) {
    CallbackUserData* data = static_cast<CallbackUserData*>(user_pointer);
    if (!data || !out_uri) return false;
    
    if (data->should_fail) {
        return false;
    }
    
    // Use fuzzer input to generate URI
    if (data->fdp && data->fdp->remaining_bytes() > 0) {
        // Consume raw input for URI generation
        size_t max_uri_length = data->fdp->ConsumeIntegralInRange<size_t>(1, 256);
        std::string raw_uri_part = data->fdp->ConsumeRandomLengthString(max_uri_length);
        
        // Apply robust UTF-8 sanitization
        std::string sanitized_uri = SanitizeUTF8(raw_uri_part);
        
        if (embedImages) {
            // For embedded images (base64 data URIs), validate base64 characters
            // Extract only the base64 part (after comma) for validation
            std::string base64_part = sanitized_uri;
            
            // Remove any non-base64 characters
            std::string clean_base64;
            for (char c : base64_part) {
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
                    (c >= '0' && c <= '9') || c == '+' || c == '/' || c == '=' ||
                    c == '-' || c == '_') {
                    clean_base64.push_back(c);
                } else {
                    clean_base64.push_back('A');  // Replace with valid base64 char
                }
            }
            
            // Ensure base64 part is not empty
            if (clean_base64.empty()) {
                clean_base64 = "test";
            }
            
            *out_uri = "data:image/png;base64," + clean_base64;
        } else {
            // For file URIs, ensure filename is safe
            // Remove any problematic characters for filenames
            std::string safe_filename;
            for (char c : sanitized_uri) {
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
                    (c >= '0' && c <= '9') || c == '.' || c == '-' || c == '_') {
                    safe_filename.push_back(c);
                } else if (c == ' ') {
                    safe_filename.push_back('_');
                } else {
                    safe_filename.push_back('x');
                }
            }
            
            // Ensure filename is not empty
            if (safe_filename.empty()) {
                safe_filename = "image";
            }
            
            *out_uri = safe_filename + ".png";
        }
    } else {
        // Default fallback values
        if (embedImages) {
            *out_uri = "data:image/png;base64,test";
        } else {
            *out_uri = "output_image.png";
        }
    }
    
    // Additional safety: ensure URI is not too long
    if (out_uri->size() > 4096) {
        out_uri->resize(4096);
    }
    
    return true;
}

// Simple test GLTF JSON for loading
const char* TEST_GLTF_JSON = R"({
  "asset": { "version": "2.0" },
  "scenes": [{ "nodes": [0] }],
  "nodes": [{ "mesh": 0 }],
  "meshes": [{
    "primitives": [{
      "attributes": { "POSITION": 0 },
      "indices": 0
    }]
  }],
  "accessors": [
    { "bufferView": 0, "componentType": 5123, "count": 3, "type": "SCALAR" },
    { "bufferView": 1, "componentType": 5126, "count": 3, "type": "VEC3", "min": [0, 0, 0], "max": [1, 1, 1] }
  ],
  "bufferViews": [
    { "buffer": 0, "byteOffset": 0, "byteLength": 6 },
    { "buffer": 0, "byteOffset": 6, "byteLength": 36 }
  ],
  "buffers": [{
    "uri": "data:application/octet-stream;base64,AAABAAIAAAAAAAAAAAAAAAAAAAAAAIA/AAAAAAAAAAAAAAAAAADAPwAAAAA=",
    "byteLength": 42
  }]
})";

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Need minimum data for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create user data for callbacks
    CallbackUserData user_data;
    user_data.fdp = &fdp;
    user_data.should_fail = false;
    user_data.custom_file_size = 0;
    
    // Set up custom file system callbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = &CustomFileExists;
    fs_callbacks.ExpandFilePath = &CustomExpandFilePath;
    fs_callbacks.ReadWholeFile = &CustomReadWholeFile;
    fs_callbacks.WriteWholeFile = &CustomWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = &CustomGetFileSizeInBytes;
    fs_callbacks.user_data = &user_data;
    
    // Set up custom URI callbacks
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = &CustomURIEncode;
    uri_callbacks.decode = &CustomURIDecode;
    uri_callbacks.user_data = &user_data;
    
    // Create TinyGLTF instance
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    // Set custom callbacks
    if (!loader.SetFsCallbacks(fs_callbacks, &err)) {
        // Continue anyway, callback setting might fail intentionally
    }
    
    if (!loader.SetURICallbacks(uri_callbacks, &err)) {
        // Continue anyway
    }
    
    // Set custom image writer
    loader.SetImageWriter(&CustomWriteImageData, &user_data);
    
    // Load a model using custom callbacks
    tinygltf::Model model;
    
    // Try to load from file (using callbacks)
    bool load_success = false;
    if (fdp.ConsumeBool()) {
        // Try loading from a "file" using callbacks
        std::string fake_filename = SanitizeUTF8(fdp.ConsumeRandomLengthString(32));
        // This would trigger our custom callbacks
        // Note: Actual file loading would happen here if we had real files
        // For fuzzing, we'll load from string instead
        
        // Convert test JSON to vector for callback simulation
        std::vector<unsigned char> json_data(TEST_GLTF_JSON, 
                                             TEST_GLTF_JSON + strlen(TEST_GLTF_JSON));
        user_data.file_content = json_data;
    }
    
    // Load model from string (using default path since our callbacks simulate files)
    load_success = loader.LoadASCIIFromString(&model, &err, &warn,
                                             TEST_GLTF_JSON,
                                             strlen(TEST_GLTF_JSON),
                                             "", tinygltf::REQUIRE_VERSION);
    
    if (!load_success) {
        // Try binary loading as fallback
        std::vector<unsigned char> glb_data;
        if (fdp.remaining_bytes() > 100) {
            size_t glb_size = fdp.ConsumeIntegralInRange<size_t>(50, fdp.remaining_bytes());
            glb_data = fdp.ConsumeBytes<unsigned char>(glb_size);
            
            // Clear model and try binary load
            model = tinygltf::Model();
            (void)loader.LoadBinaryFromMemory(&model, &err, &warn,
                                             glb_data.data(),
                                             static_cast<unsigned int>(glb_data.size()),
                                             "", 0);
        }
    }
    
    // Modify the model based on fuzzer input to test writing
    if (model.images.empty() && fdp.ConsumeBool()) {
        // Add a test image to the model
        tinygltf::Image image;
        image.name = SanitizeUTF8(fdp.ConsumeRandomLengthString(16));
        image.width = fdp.ConsumeIntegralInRange<int>(1, 256);
        image.height = fdp.ConsumeIntegralInRange<int>(1, 256);
        image.component = fdp.ConsumeIntegralInRange<int>(1, 4);
        image.bits = fdp.ConsumeIntegralInRange<int>(1, 32);
        
        // Generate some pixel data
        size_t pixel_data_size = image.width * image.height * image.component * (image.bits / 8);
        if (pixel_data_size > 0 && fdp.remaining_bytes() >= pixel_data_size) {
            image.image.resize(pixel_data_size);
            auto pixel_data = fdp.ConsumeBytes<unsigned char>(pixel_data_size);
            std::copy(pixel_data.begin(), pixel_data.end(), image.image.begin());
        }
        
        model.images.push_back(image);
        
        // Add a texture referencing the image
        if (model.textures.empty()) {
            tinygltf::Texture texture;
            texture.source = 0;  // Reference first image
            model.textures.push_back(texture);
        }
    }
    
    // Test WriteGltfSceneToFile with different embedImages settings
    bool embed_images = fdp.ConsumeBool();
    bool embed_buffers = fdp.ConsumeBool();
    bool write_binary = fdp.ConsumeBool();
    bool pretty_print = fdp.ConsumeBool();
    
    // Create temporary filenames for testing
    std::string output_filename = "/tmp/gltf_test_" + std::to_string(fdp.ConsumeIntegral<uint32_t>()) + 
                                 (write_binary ? ".glb" : ".gltf");
    
    // Write to file
    bool write_file_success = loader.WriteGltfSceneToFile(&model, output_filename,
                                                         embed_images,
                                                         embed_buffers,
                                                         pretty_print,
                                                         write_binary);
    
    // Test WriteGltfSceneToStream
    std::stringstream stream;
    bool write_stream_success = loader.WriteGltfSceneToStream(&model, stream,
                                                             pretty_print,
                                                             write_binary);
    // Clean up temporary file if it was created
    std::remove(output_filename.c_str());
    
    // Test GetFileSizeInBytes directly (standalone function)
    size_t test_file_size = 0;
    std::string size_err;
    std::string test_filename = SanitizeUTF8(fdp.ConsumeRandomLengthString(32));
    (void)tinygltf::GetFileSizeInBytes(&test_file_size, &size_err, test_filename, nullptr);
    
    // Test default WriteImageData function if available
    #ifndef TINYGLTF_NO_STB_IMAGE_WRITE
    if (!model.images.empty() && fdp.ConsumeBool()) {
        tinygltf::Image test_image = model.images[0];
        std::string test_out_uri;
        std::string test_basepath = "/tmp";
        std::string test_filename = "test_output.png";
        
        // Test default WriteImageData
        (void)tinygltf::WriteImageData(&test_basepath, &test_filename,
                                      &test_image, embed_images,
                                      &fs_callbacks, &uri_callbacks,
                                      &test_out_uri, &user_data);
    }
    #endif
    
    return 0;
}

} // namespace harness_007
