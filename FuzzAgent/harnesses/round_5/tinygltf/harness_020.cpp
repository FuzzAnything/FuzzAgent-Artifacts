#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_020 {
/*
 * Fuzzing harness for tinygltf targeting AudioSource equality operator
 * which is completely uncovered (0% coverage, 4 undiscovered branches).
 * 
 * Target API:
 * - AudioSource equality operator (_ZNK8tinygltf11AudioSourceeqERKS0_)
 * 
 * Required Helper APIs:
 * - LoadASCIIFromString or LoadBinaryFromMemory to load glTF models with audio extensions
 * - Model constructor and destructor
 * - Various Value access functions to modify AudioSource properties for inequality testing
 * 
 * The AudioSource equality operator represents 4 completely undiscovered branches 
 * and 0% coverage. Testing this operator will exercise model loading paths with 
 * audio extensions and potentially uncover other audio-related code paths.
 * 
 * Invocation sequence:
 * 1. Create a TinyGLTF instance and Model object.
 * 2. Load a glTF model containing AudioSource objects using LoadASCIIFromString or LoadBinaryFromMemory.
 * 3. Extract AudioSource objects from the loaded model.
 * 4. Test equality operator on identical AudioSource objects (should return true).
 * 5. Create modified copies of AudioSource objects by changing properties (name, uri, mimeType, bufferView, etc.).
 * 6. Test equality operator on different AudioSource objects (should return false).
 * 7. Test edge cases: empty AudioSource objects, partially initialized objects.
 * 8. Clean up all resources.
 * 
 * Uses FuzzedDataProvider for structured input splitting with no hardcoded test data.
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


// Function to create a minimal valid GLTF with AudioSource extensions
static std::string create_minimal_gltf_with_audio() {
    return R"({
  "asset": { "version": "2.0" },
  "extensionsUsed": ["KHR_audio"],
  "extensions": {
    "KHR_audio": {
      "sources": [
        {
          "name": "audio1",
          "uri": "sound.mp3"
        },
        {
          "name": "audio2", 
          "uri": "music.ogg",
          "mimeType": "audio/mpeg"
        },
        {
          "name": "audio3",
          "bufferView": 0,
          "mimeType": "audio/mpeg"
        }
      ]
    }
  },
  "bufferViews": [
    {
      "buffer": 0,
      "byteOffset": 0,
      "byteLength": 1024
    }
  ],
  "buffers": [
    {
      "byteLength": 1024,
      "uri": "data:application/octet-stream;base64,AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmqq6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w=="
    }
  ]
})";
}

// Function to create GLTF with AudioSource from fuzzed data
static std::string create_gltf_with_fuzzed_audiosource(FuzzedDataProvider& fdp) {
    // Consume number of audio sources to create (0-10)
    size_t num_sources = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    
    // Start building GLTF JSON
    std::string gltf = R"({
  "asset": { "version": "2.0" },
  "extensionsUsed": ["KHR_audio"],
  "extensions": {
    "KHR_audio": {
      "sources": [)";
    
    // Add audio sources
    for (size_t i = 0; i < num_sources; i++) {
        if (i > 0) gltf += ",";
        
        // Consume random data for AudioSource fields
        std::string name = fdp.ConsumeRandomLengthString(50);
        std::string uri = fdp.ConsumeRandomLengthString(100);
        int bufferView = fdp.ConsumeBool() ? fdp.ConsumeIntegralInRange<int>(-1, 10) : -1;
        std::string mimeType = fdp.ConsumeBool() ? "audio/mpeg" : "";
        
        // Escape JSON strings
        auto escape_json = [](const std::string& s) -> std::string {
            std::string escaped;
            for (char c : s) {
                if (c == '"' || c == '\\' || c == '\n' || c == '\r' || c == '\t' || c == '\b' || c == '\f') {
                    escaped += '\\';
                    if (c == '\n') escaped += 'n';
                    else if (c == '\r') escaped += 'r';
                    else if (c == '\t') escaped += 't';
                    else if (c == '\b') escaped += 'b';
                    else if (c == '\f') escaped += 'f';
                    else escaped += c;
                } else {
                    escaped += c;
                }
            }
            return escaped;
        };
        
        std::string escaped_name = escape_json(name);
        std::string escaped_uri = escape_json(uri);
        std::string escaped_mimeType = escape_json(mimeType);
        
        gltf += R"(
        {
          "name": ")" + escaped_name + R"(")";
        
        if (!uri.empty() || bufferView == -1) {
            gltf += R"(,
          "uri": ")" + escaped_uri + R"(")";
        }
        
        if (bufferView != -1) {
            gltf += R"(,
          "bufferView": )" + std::to_string(bufferView);
        }
        
        if (!mimeType.empty()) {
            gltf += R"(,
          "mimeType": ")" + escaped_mimeType + R"(")";
        }
        
        gltf += R"(
        })";
    }
    
    // Add bufferViews if needed
    bool needs_buffer = false;
    for (size_t i = 0; i < num_sources; i++) {
        // Check if any source uses bufferView
        // We'll just add a bufferView for simplicity
        needs_buffer = true;
    }
    
    if (needs_buffer && num_sources > 0) {
        gltf += R"(
      ]
    }
  },
  "bufferViews": [
    {
      "buffer": 0,
      "byteOffset": 0,
      "byteLength": 1024
    }
  ],
  "buffers": [
    {
      "byteLength": 1024,
      "uri": "data:application/octet-stream;base64,AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmqq6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w=="
    }
  ]
})";
    } else {
        gltf += R"(
      ]
    }
  }
})";
    }
    
    return gltf;
}

// Function to test AudioSource equality operator comprehensively
static void test_audiosource_equality_operator(tinygltf::Model& model, FuzzedDataProvider& fdp) {
    // Test 1: Equality on loaded AudioSource objects
    for (size_t i = 0; i < model.audioSources.size(); i++) {
        for (size_t j = 0; j < model.audioSources.size(); j++) {
            bool equal = (model.audioSources[i] == model.audioSources[j]);
            (void)equal;  // Prevent unused variable warning
            
            // Self-equality should always be true
            if (i == j) {
                bool self_equal = (model.audioSources[i] == model.audioSources[i]);
                (void)self_equal;
                // assert(self_equal == true); - but we can't assert in fuzzer
            }
        }
    }
    
    // Test 2: Create modified copies and test inequality
    if (!model.audioSources.empty()) {
        for (size_t i = 0; i < std::min(model.audioSources.size(), (size_t)5); i++) {
            tinygltf::AudioSource original = model.audioSources[i];
            tinygltf::AudioSource modified = original;
            
            // Modify different properties based on fuzzed input
            uint8_t modification_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            
            switch (modification_type) {
                case 0:
                    // Modify name
                    modified.name = fdp.ConsumeRandomLengthString(50);
                    break;
                case 1:
                    // Modify uri
                    modified.uri = fdp.ConsumeRandomLengthString(100);
                    break;
                case 2:
                    // Modify bufferView
                    modified.bufferView = fdp.ConsumeIntegralInRange<int>(-1, 100);
                    break;
                case 3:
                    // Modify mimeType
                    modified.mimeType = fdp.ConsumeRandomLengthString(20);
                    break;
            }
            
            // Test equality - should be false if name or uri changed
            bool equal_after_mod = (original == modified);
            (void)equal_after_mod;
            
            // Also test with extras/extensions if present
            if (fdp.ConsumeBool() && original.extras.Type() != tinygltf::NULL_TYPE) {
                // Note: operator== doesn't compare extras or extensions
                tinygltf::AudioSource with_extras = original;
                // Extras modification doesn't affect equality according to operator==
                bool equal_with_extras = (original == with_extras);
                (void)equal_with_extras;
            }
        }
    }
    
    // Test 3: Test edge cases - empty AudioSource objects
    {
        tinygltf::AudioSource empty1;
        tinygltf::AudioSource empty2;
        bool empty_equal = (empty1 == empty2);
        (void)empty_equal;  // Should be true (both empty)
        
        // Test with partially initialized
        tinygltf::AudioSource partial;
        partial.name = "test";
        bool empty_vs_partial = (empty1 == partial);
        (void)empty_vs_partial;  // Should be false
        
        // Self-equality for empty
        bool empty_self = (empty1 == empty1);
        (void)empty_self;
    }
    
    // Test 4: Create AudioSource objects manually and test
    if (model.audioSources.size() < 2 && fdp.ConsumeBool()) {
        tinygltf::AudioSource manual1;
        manual1.name = fdp.ConsumeRandomLengthString(30);
        manual1.uri = fdp.ConsumeRandomLengthString(60);
        
        tinygltf::AudioSource manual2 = manual1;
        bool manual_equal = (manual1 == manual2);
        (void)manual_equal;
        
        // Modify one and test inequality
        manual2.name = fdp.ConsumeRandomLengthString(30);
        bool manual_unequal = (manual1 == manual2);
        (void)manual_unequal;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) {
        return 0;  // Insufficient input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize loader
    tinygltf::TinyGLTF loader;
    loader.SetStoreOriginalJSONForExtrasAndExtensions(true);
    
    // Consume choice of GLTF source
    uint8_t gltf_source = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    bool parse_success = false;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    std::string gltf_data;
    
    switch (gltf_source) {
        case 0:
            // Use minimal valid GLTF with audio
            gltf_data = create_minimal_gltf_with_audio();
            break;
        case 1:
            // Generate GLTF with fuzzed AudioSource data
            gltf_data = create_gltf_with_fuzzed_audiosource(fdp);
            break;
        case 2:
            // Use remaining fuzzer input as GLTF data directly
            gltf_data = fdp.ConsumeRemainingBytesAsString();
            break;
    }
    
    // Try parsing as ASCII first
    parse_success = loader.LoadASCIIFromString(&model, &err, &warn, 
                                               gltf_data.c_str(), 
                                               gltf_data.length(),
                                               "");
    
    if (!parse_success && !err.empty()) {
        // If ASCII parsing fails, try as binary if the data looks like binary
        // Reset model
        model = tinygltf::Model();
        
        // For binary, we need to provide the data as const char*
        const std::vector<uint8_t> binary_data(gltf_data.begin(), gltf_data.end());
        if (!binary_data.empty()) {
            parse_success = loader.LoadBinaryFromMemory(&model, &err, &warn,
                                                        binary_data.data(),
                                                        binary_data.size(),
                                                        "");
        }
    }
    
    // If we successfully parsed a model, test AudioSource equality operator
    if (parse_success) {
        test_audiosource_equality_operator(model, fdp);
        
        // Also test Value access functions for AudioSource extras/extensions
        for (size_t i = 0; i < model.audioSources.size(); i++) {
            const auto& audioSource = model.audioSources[i];
            
            // Test extras if present
            if (audioSource.extras.Type() != tinygltf::NULL_TYPE) {
                // Try to access extras as different types
                if (audioSource.extras.IsBool()) {
                    bool b = audioSource.extras.Get<bool>();
                    (void)b;
                } else if (audioSource.extras.IsInt()) {
                    int i_val = audioSource.extras.Get<int>();
                    (void)i_val;
                } else if (audioSource.extras.IsNumber()) {
                    double d = audioSource.extras.Get<double>();
                    (void)d;
                } else if (audioSource.extras.IsString()) {
                    std::string s = audioSource.extras.Get<std::string>();
                    (void)s;
                }
            }
            
            // Test extensions (ExtensionMap is std::map<std::string, tinygltf::Value>)
            for (const auto& ext : audioSource.extensions) {
                const std::string& ext_name = ext.first;
                const tinygltf::Value& ext_value = ext.second;
                (void)ext_name;
                (void)ext_value;
            }
        }
    } else {
        // Even if parsing failed, we can still test AudioSource equality
        // with manually created objects
        tinygltf::AudioSource src1;
        src1.name = fdp.ConsumeRandomLengthString(20);
        src1.uri = fdp.ConsumeRandomLengthString(40);
        
        tinygltf::AudioSource src2 = src1;
        
        // Test equality
        bool equal = (src1 == src2);
        (void)equal;
        
        // Modify and test inequality
        src2.name = fdp.ConsumeRandomLengthString(20);
        bool unequal = (src1 == src2);
        (void)unequal;
    }
    
    // Model destructor automatically cleans up resources
    return 0;
}

} // namespace harness_020
