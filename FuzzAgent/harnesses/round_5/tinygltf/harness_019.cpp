#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_019 {
/*
 * Fuzzing harness for tinygltf targeting tinygltf::Value type-accessor functions
 * which are completely uncovered (0 hits across all 19 fuzzers).
 * 
 * Target APIs:
 * 1. Value::Get<int> - Value::Get<int> templated method
 * 2. Value::Get<double> - Value::Get<double> templated method  
 * 3. Value::Get<bool> - Value::Get<bool> templated method
 * 4. Value::Get<string> - Value::Get<std::string> templated method
 * 5. Value::Get<array> - Value::Get<std::vector<Value>> templated method
 * 6. Value::Get<object> - Value::Get<std::map<std::string, Value>> templated method
 * 7. AudioSource equality operator - AudioSource::operator== with 4 undiscovered branches
 * 
 * These Value::Get<T> functions represent a cohesive cluster of 6+ completely 
 * uncovered APIs (0 hits) essential for accessing typed data from parsed GLTF.
 * They handle type conversion logic with branching. The AudioSource equality 
 * operator adds 4 more undiscovered branches.
 * 
 * Invocation sequence:
 * 1. Call either LoadBinaryFromMemory or LoadASCIIFromString to parse a GLTF input into a Model object.
 * 2. Traverse the parsed Model to find Value objects in extensions, extras, or other GLTF properties.
 * 3. For each Value found, attempt to call the appropriate Get<T> method based on the Value's type.
 * 4. Specifically create GLTF inputs containing AudioSource objects to exercise the equality operator.
 * 5. Let Model destructor clean up resources automatically.
 * 
 * Uses FuzzedDataProvider for structured input splitting with no hardcoded test data.
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


// Function to test all Value::Get<T> methods based on value type
static void test_value_get_methods(const tinygltf::Value& value) {
    // Check type and call appropriate Get<T> method
    switch (value.Type()) {
        case tinygltf::NULL_TYPE:
            // Nothing to get from null type
            break;
            
        case tinygltf::BOOL_TYPE:
            // Test Value::Get<bool>
            try {
                bool b = value.Get<bool>();
                (void)b;  // Prevent unused variable warning
            } catch (...) {
                // Handle any exceptions from Get()
            }
            break;
            
        case tinygltf::INT_TYPE:
            // Test Value::Get<int>
            try {
                int i = value.Get<int>();
                (void)i;
            } catch (...) {
                // Handle any exceptions from Get()
            }
            break;
            
        case tinygltf::REAL_TYPE:
            // Test Value::Get<double>
            try {
                double d = value.Get<double>();
                (void)d;
            } catch (...) {
                // Handle any exceptions from Get()
            }
            break;
            
        case tinygltf::STRING_TYPE:
            // Test Value::Get<std::string>
            try {
                std::string s = value.Get<std::string>();
                (void)s;
            } catch (...) {
                // Handle any exceptions from Get()
            }
            break;
            
        case tinygltf::BINARY_TYPE:
            // Test Value::Get<std::vector<unsigned char>>
            try {
                std::vector<unsigned char> b = value.Get<std::vector<unsigned char>>();
                (void)b;
            } catch (...) {
                // Handle any exceptions from Get()
            }
            break;
            
        case tinygltf::ARRAY_TYPE:
            // Test Value::Get<std::vector<tinygltf::Value>>
            try {
                tinygltf::Value::Array arr = value.Get<tinygltf::Value::Array>();
                (void)arr;
                
                // Also test Get() with index for array access
                if (value.Size() > 0) {
                    const tinygltf::Value& elem = value.Get(0);
                    (void)elem;
                }
            } catch (...) {
                // Handle any exceptions from Get()
            }
            break;
            
        case tinygltf::OBJECT_TYPE:
            // Test Value::Get<std::map<std::string, tinygltf::Value>>
            try {
                tinygltf::Value::Object obj = value.Get<tinygltf::Value::Object>();
                (void)obj;
                
                // Also test Get() with key for object access
                std::vector<std::string> keys = value.Keys();
                if (!keys.empty()) {
                    const tinygltf::Value& elem = value.Get(keys[0]);
                    (void)elem;
                }
            } catch (...) {
                // Handle any exceptions from Get()
            }
            break;
    }
}

// Function to recursively traverse and test Value objects
static void traverse_and_test_values(const tinygltf::Value& value) {
    // Test Get<T> methods for this value
    test_value_get_methods(value);
    
    // Recursively traverse arrays
    if (value.IsArray()) {
        for (size_t i = 0; i < value.Size(); i++) {
            const tinygltf::Value& elem = value.Get(i);
            traverse_and_test_values(elem);
        }
    }
    
    // Recursively traverse objects
    if (value.IsObject()) {
        std::vector<std::string> keys = value.Keys();
        for (const auto& key : keys) {
            const tinygltf::Value& elem = value.Get(key);
            traverse_and_test_values(elem);
        }
    }
}

// Function to test AudioSource equality operator
static void test_audiosource_equality(const tinygltf::Model& model) {
    // Test equality for all AudioSource objects in the model
    for (size_t i = 0; i < model.audioSources.size(); i++) {
        for (size_t j = 0; j < model.audioSources.size(); j++) {
            // Test operator==
            bool equal = (model.audioSources[i] == model.audioSources[j]);
            (void)equal;  // Prevent unused variable warning
            
            // Also test self-equality
            if (i == j) {
                bool self_equal = (model.audioSources[i] == model.audioSources[i]);
                (void)self_equal;
            }
        }
    }
}

// Function to create a GLTF string with AudioSource elements from fuzzed data
static std::string create_gltf_with_audiosource(FuzzedDataProvider& fdp) {
    // Consume number of audio sources to create (0-5)
    size_t num_sources = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    
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
        
        // Consume random strings for name and uri
        std::string name = fdp.ConsumeRandomLengthString(32);
        std::string uri = fdp.ConsumeRandomLengthString(64);
        
        // Escape JSON strings
        std::string escaped_name;
        for (char c : name) {
            if (c == '"' || c == '\\') escaped_name += '\\';
            escaped_name += c;
        }
        
        std::string escaped_uri;
        for (char c : uri) {
            if (c == '"' || c == '\\') escaped_uri += '\\';
            escaped_uri += c;
        }
        
        gltf += R"(
        {
          "name": ")" + escaped_name + R"(",
          "uri": ")" + escaped_uri + R"("
        })";
    }
    
    gltf += R"(
      ]
    }
  }
})";
    
    return gltf;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Insufficient input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize loader
    tinygltf::TinyGLTF loader;
    loader.SetStoreOriginalJSONForExtrasAndExtensions(true);
    
    // Consume choice: 0 = use provided GLTF data, 1 = generate GLTF with AudioSource
    uint8_t choice = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    bool parse_success = false;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    if (choice == 0) {
        // Use fuzzed data directly as GLTF input
        std::vector<uint8_t> gltf_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (gltf_data.size() < 20) {
            return 0;  // Too small for GLTF
        }
        
        // Try parsing as ASCII first
        parse_success = loader.LoadASCIIFromString(
            &model, &err, &warn,
            reinterpret_cast<const char*>(gltf_data.data()),
            static_cast<unsigned int>(gltf_data.size()),
            "", 0);
            
        if (!parse_success) {
            // Try parsing as binary
            parse_success = loader.LoadBinaryFromMemory(
                &model, &err, &warn,
                gltf_data.data(),
                static_cast<unsigned int>(gltf_data.size()),
                "", 0);
        }
    } else {
        // Generate GLTF with AudioSource from fuzzed data
        std::string gltf_string = create_gltf_with_audiosource(fdp);
        
        parse_success = loader.LoadASCIIFromString(
            &model, &err, &warn,
            gltf_string.c_str(),
            static_cast<unsigned int>(gltf_string.size()),
            "", 0);
    }
    
    if (!parse_success) {
        // Even if parsing fails, we can still test with empty model
        // Some Value objects might be created during failed parsing
    }
    
    // Test Value objects in the model
    
    // 1. Test extras Value
    traverse_and_test_values(model.extras);
    
    // 2. Test extensions Value
    traverse_and_test_values(tinygltf::Value(model.extensions));
    
    // 3. Test Value objects in asset
    traverse_and_test_values(model.asset.extras);
    traverse_and_test_values(tinygltf::Value(model.asset.extensions));
    
    // 4. Test Value objects in all components that have extras/extensions
    for (const auto& accessor : model.accessors) {
        traverse_and_test_values(accessor.extras);
        traverse_and_test_values(tinygltf::Value(accessor.extensions));
    }
    
    for (const auto& material : model.materials) {
        traverse_and_test_values(material.extras);
        traverse_and_test_values(tinygltf::Value(material.extensions));
    }
    
    for (const auto& mesh : model.meshes) {
        traverse_and_test_values(mesh.extras);
        traverse_and_test_values(tinygltf::Value(mesh.extensions));
    }
    
    for (const auto& node : model.nodes) {
        traverse_and_test_values(node.extras);
        traverse_and_test_values(tinygltf::Value(node.extensions));
    }
    
    for (const auto& texture : model.textures) {
        traverse_and_test_values(texture.extras);
        traverse_and_test_values(tinygltf::Value(texture.extensions));
    }
    
    // 5. Test AudioSource equality operator if we have audio sources
    if (!model.audioSources.empty()) {
        test_audiosource_equality(model);
    }
    
    // 6. Also test Value objects in audio emitters and sources
    for (const auto& emitter : model.audioEmitters) {
        traverse_and_test_values(emitter.extras);
        traverse_and_test_values(tinygltf::Value(emitter.extensions));
    }
    
    for (const auto& source : model.audioSources) {
        traverse_and_test_values(source.extras);
        traverse_and_test_values(tinygltf::Value(source.extensions));
    }
    
    // 7. Create and test synthetic Value objects with different types
    // to ensure we cover all Get<T> specializations
    
    // Test bool Value
    tinygltf::Value bool_val(true);
    test_value_get_methods(bool_val);
    
    // Test int Value
    tinygltf::Value int_val(fdp.ConsumeIntegral<int>());
    test_value_get_methods(int_val);
    
    // Test double Value  
    tinygltf::Value double_val(fdp.ConsumeFloatingPoint<double>());
    test_value_get_methods(double_val);
    
    // Test string Value
    std::string str_val = fdp.ConsumeRandomLengthString(128);
    tinygltf::Value string_val(str_val);
    test_value_get_methods(string_val);
    
    // Test array Value
    tinygltf::Value::Array arr;
    arr.push_back(tinygltf::Value(fdp.ConsumeIntegral<int>()));
    arr.push_back(tinygltf::Value(fdp.ConsumeRandomLengthString(64)));
    tinygltf::Value array_val(arr);
    test_value_get_methods(array_val);
    
    // Test object Value
    tinygltf::Value::Object obj;
    obj["int_key"] = tinygltf::Value(fdp.ConsumeIntegral<int>());
    obj["str_key"] = tinygltf::Value(fdp.ConsumeRandomLengthString(64));
    tinygltf::Value object_val(obj);
    test_value_get_methods(object_val);
    
    // Model destructor will clean up automatically
    return 0;
}

} // namespace harness_019
