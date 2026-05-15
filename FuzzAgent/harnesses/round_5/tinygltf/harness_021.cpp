#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <algorithm>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_021 {
/*
 * Fuzzing harness for tinygltf targeting ASCII GLTF loading operations
 * LoadASCIIFromString is completely uncovered (0% coverage) with 176 branches and 62 undiscovered branches.
 * 
 * Target APIs:
 * - LoadASCIIFromString (0% coverage, 176 branches, 62 undiscovered)
 * - LoadASCIIFromFile (71 undiscovered branches)
 * - Value::Get<string>, Value::Get<int>, Value::Get<double>, Value::Get<bool> (Helper APIs)
 * 
 * Invocation sequence:
 * 1. Create TinyGLTF instance (constructor)
 * 2. Call LoadASCIIFromString with various GLTF JSON strings
 * 3. On successful load, use Value.Get<Type> methods to access parsed model data
 * 4. Test error paths with malformed GLTF strings
 * 5. Optional: Test LoadASCIIFromFile for file-based loading
 * 6. Let Model destructor handle cleanup (RAII)
 * 
 * These functions represent 250+ points of undiscovered complexity. LoadASCIIFromString is currently 0% covered 
 * with 176 branches, while related LoadASCIIFromFile has 71 undiscovered branches. The Value getter APIs are also 
 * completely uncovered (0% hit). They form a logical lifecycle (Parse -> Access data) for ASCII GLTF files and 
 * are critical for testing GLTF parsing functionality.
 * 
 * Uses FuzzedDataProvider for structured input splitting with no hardcoded test data.
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

// Function to create a minimal valid GLTF JSON string from fuzzed data
static std::string create_gltf_json_from_fuzzed_data(FuzzedDataProvider& fdp) {
    // Consume configuration options
    bool include_extras = fdp.ConsumeBool();
    bool include_extensions = fdp.ConsumeBool();
    bool include_assets = fdp.ConsumeBool();
    bool include_scenes = fdp.ConsumeBool();
    bool include_nodes = fdp.ConsumeBool();
    bool include_meshes = fdp.ConsumeBool();
    bool include_materials = fdp.ConsumeBool();
    bool include_textures = fdp.ConsumeBool();
    bool include_images = fdp.ConsumeBool();
    bool include_buffers = fdp.ConsumeBool();
    bool include_buffer_views = fdp.ConsumeBool();
    bool include_accessors = fdp.ConsumeBool();
    
    // Consume string data for various fields
    std::string version = fdp.ConsumeRandomLengthString(10);
    std::string generator = fdp.ConsumeRandomLengthString(50);
    std::string copyright = fdp.ConsumeRandomLengthString(100);
    std::string minVersion = fdp.ConsumeRandomLengthString(10);
    
    // Start building GLTF JSON
    std::string gltf = "{\n";
    
    // Add asset section (required)
    if (include_assets || true) { // Always include at least minimal asset
        gltf += "  \"asset\": {\n";
        gltf += "    \"version\": \"" + (version.empty() ? "2.0" : version) + "\"";
        if (!generator.empty()) {
            gltf += ",\n    \"generator\": \"" + generator + "\"";
        }
        if (!copyright.empty()) {
            gltf += ",\n    \"copyright\": \"" + copyright + "\"";
        }
        if (!minVersion.empty()) {
            gltf += ",\n    \"minVersion\": \"" + minVersion + "\"";
        }
        gltf += "\n  }";
    }
    
    // Add scenes if requested
    if (include_scenes) {
        int num_scenes = fdp.ConsumeIntegralInRange<int>(0, 5);
        if (num_scenes > 0) {
            gltf += ",\n  \"scenes\": [\n";
            for (int i = 0; i < num_scenes; i++) {
                if (i > 0) gltf += ",\n";
                gltf += "    {\n";
                gltf += "      \"nodes\": [" + std::to_string(i) + "]";
                
                // Add extras if requested
                if (include_extras) {
                    std::string extra_name = fdp.ConsumeRandomLengthString(20);
                    if (!extra_name.empty()) {
                        gltf += ",\n      \"extras\": {\n";
                        gltf += "        \"name\": \"" + extra_name + "\"";
                        
                        // Add some typed extras to test Value::Get<T>
                        if (fdp.ConsumeBool()) {
                            int extra_int = fdp.ConsumeIntegral<int>();
                            gltf += ",\n        \"count\": " + std::to_string(extra_int);
                        }
                        if (fdp.ConsumeBool()) {
                            double extra_double = fdp.ConsumeFloatingPoint<double>();
                            gltf += ",\n        \"scale\": " + std::to_string(extra_double);
                        }
                        if (fdp.ConsumeBool()) {
                            bool extra_bool = fdp.ConsumeBool();
                            gltf += ",\n        \"visible\": " + std::string(extra_bool ? "true" : "false");
                        }
                        if (fdp.ConsumeBool()) {
                            std::string extra_string = fdp.ConsumeRandomLengthString(30);
                            gltf += ",\n        \"description\": \"" + extra_string + "\"";
                        }
                        gltf += "\n      }";
                    }
                }
                gltf += "\n    }";
            }
            gltf += "\n  ]";
            
            // Set default scene
            gltf += ",\n  \"defaultScene\": 0";
        }
    }
    
    // Add nodes if requested
    if (include_nodes) {
        int num_nodes = fdp.ConsumeIntegralInRange<int>(0, 10);
        if (num_nodes > 0) {
            gltf += ",\n  \"nodes\": [\n";
            for (int i = 0; i < num_nodes; i++) {
                if (i > 0) gltf += ",\n";
                gltf += "    {\n";
                gltf += "      \"name\": \"node_" + std::to_string(i) + "\"";
                
                // Add mesh reference sometimes
                if (include_meshes && fdp.ConsumeBool()) {
                    gltf += ",\n      \"mesh\": " + std::to_string(fdp.ConsumeIntegralInRange<int>(0, 5));
                }
                gltf += "\n    }";
            }
            gltf += "\n  ]";
        }
    }
    
    // Add extensionsUsed if requested
    if (include_extensions) {
        int num_extensions = fdp.ConsumeIntegralInRange<int>(0, 5);
        if (num_extensions > 0) {
            gltf += ",\n  \"extensionsUsed\": [\n";
            for (int i = 0; i < num_extensions; i++) {
                if (i > 0) gltf += ",\n";
                std::string ext_name = fdp.ConsumeRandomLengthString(30);
                gltf += "    \"" + ext_name + "\"";
            }
            gltf += "\n  ]";
        }
    }
    
    // Add extensionsRequired if requested
    if (include_extensions) {
        int num_extensions_req = fdp.ConsumeIntegralInRange<int>(0, 3);
        if (num_extensions_req > 0) {
            gltf += ",\n  \"extensionsRequired\": [\n";
            for (int i = 0; i < num_extensions_req; i++) {
                if (i > 0) gltf += ",\n";
                std::string ext_name = fdp.ConsumeRandomLengthString(30);
                gltf += "    \"" + ext_name + "\"";
            }
            gltf += "\n  ]";
        }
    }
    
    // Close the JSON
    gltf += "\n}";
    
    return gltf;
}

// Function to test Value::Get<T> methods on model extras
static void test_value_getters(const tinygltf::Model& model, FuzzedDataProvider& fdp) {
    // Test extras on model if present
    if (model.extras.IsObject()) {
        const tinygltf::Value::Object& extras_obj = model.extras.Get<tinygltf::Value::Object>();
        
        // Try to access various types
        for (const auto& kv : extras_obj) {
            const std::string& key = kv.first;
            const tinygltf::Value& value = kv.second;
            
            // Based on value type, try appropriate Get<T> method
            if (value.IsBool()) {
                try {
                    bool bool_val = value.Get<bool>();
                    (void)bool_val; // Use to avoid unused variable warning
                } catch (...) {
                    // Expected for malformed data
                }
            } else if (value.IsInt()) {
                try {
                    int int_val = value.Get<int>();
                    (void)int_val;
                } catch (...) {
                    // Expected for malformed data
                }
            } else if (value.IsReal()) {
                try {
                    double double_val = value.Get<double>();
                    (void)double_val;
                } catch (...) {
                    // Expected for malformed data
                }
            } else if (value.IsString()) {
                try {
                    std::string string_val = value.Get<std::string>();
                    (void)string_val;
                } catch (...) {
                    // Expected for malformed data
                }
            } else if (value.IsArray()) {
                try {
                    const tinygltf::Value::Array& array_val = value.Get<tinygltf::Value::Array>();
                    (void)array_val;
                } catch (...) {
                    // Expected for malformed data
                }
            } else if (value.IsObject()) {
                try {
                    const tinygltf::Value::Object& obj_val = value.Get<tinygltf::Value::Object>();
                    (void)obj_val;
                } catch (...) {
                    // Expected for malformed data
                }
            }
        }
    }
    
    // Test extras on scenes
    for (const auto& scene : model.scenes) {
        if (scene.extras.IsObject()) {
            const tinygltf::Value::Object& extras_obj = scene.extras.Get<tinygltf::Value::Object>();
            (void)extras_obj; // Use to avoid unused variable warning
        }
    }
    
    // Test extras on nodes
    for (const auto& node : model.nodes) {
        if (node.extras.IsObject()) {
            const tinygltf::Value::Object& extras_obj = node.extras.Get<tinygltf::Value::Object>();
            (void)extras_obj;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Need minimum data for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume selector byte to choose operation
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Determine which APIs to test
    bool test_load_ascii_string = (selector & 0x01) != 0;
    bool test_load_ascii_file = (selector & 0x02) != 0;
    bool test_value_getters_flag = (selector & 0x04) != 0;
    bool test_error_paths = (selector & 0x08) != 0;
    
    // Consume check_sections flags
    unsigned int check_sections = 0;
    bool check_version = (selector & 0x10) != 0;
    bool check_scene = (selector & 0x20) != 0;
    bool check_all = (selector & 0x40) != 0;
    
    if (check_version) check_sections |= tinygltf::REQUIRE_VERSION;
    if (check_scene) check_sections |= tinygltf::REQUIRE_SCENE;
    if (check_all) check_sections = tinygltf::REQUIRE_ALL;
    
    // Consume base_dir string
    std::string base_dir = fdp.ConsumeRandomLengthString(100);
    
    // Get remaining bytes for GLTF data
    std::vector<uint8_t> remaining_bytes = fdp.ConsumeRemainingBytes<uint8_t>();
    
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    
    bool ret = false;
    
    // Test 1: LoadASCIIFromString with generated GLTF JSON
    if (test_load_ascii_string && !remaining_bytes.empty()) {
        // Create GLTF JSON from fuzzed data
        std::string gltf_json = create_gltf_json_from_fuzzed_data(fdp);
        
        if (!gltf_json.empty()) {
            ret = loader.LoadASCIIFromString(&model, &err, &warn,
                                            gltf_json.c_str(),
                                            static_cast<unsigned int>(gltf_json.size()),
                                            base_dir,
                                            check_sections);
            
            // If load succeeded, test Value getters
            if (ret && test_value_getters_flag) {
                test_value_getters(model, fdp);
            }
            
            // Clear model for next test
            model = tinygltf::Model();
        }
    }
    
    // Test 2: LoadASCIIFromString with raw fuzzed data (error paths)
    if (test_error_paths && !remaining_bytes.empty()) {
        // Use raw fuzzed data as GLTF string (likely malformed)
        std::string raw_data(reinterpret_cast<const char*>(remaining_bytes.data()), 
                            std::min(remaining_bytes.size(), static_cast<size_t>(10000)));
        
        (void)loader.LoadASCIIFromString(&model, &err, &warn,
                                        raw_data.c_str(),
                                        static_cast<unsigned int>(raw_data.size()),
                                        base_dir,
                                        check_sections);
        
        // Clear model
        model = tinygltf::Model();
    }
    
    // Test 3: LoadASCIIFromFile if we have enough data
    if (test_load_ascii_file && !remaining_bytes.empty()) {
        // Create a temporary file with the remaining bytes
        std::string temp_filename = create_temp_file(remaining_bytes);
        if (!temp_filename.empty()) {
            // Try to load as GLTF file
            (void)loader.LoadASCIIFromFile(&model, &err, &warn,
                                          temp_filename,
                                          check_sections);
            
            // Cleanup temporary file
            cleanup_temp_file(temp_filename);
            
            // If load succeeded, test Value getters
            if (ret && test_value_getters_flag) {
                test_value_getters(model, fdp);
            }
            
            // Clear model
            model = tinygltf::Model();
        }
    }
    
    // Test 4: Direct Value::Get<T> testing with constructed Values
    if (test_value_getters_flag) {
        // Create some Value objects and test Get<T> methods
        try {
            // Test bool Value
            tinygltf::Value bool_value(true);
            bool bool_val = bool_value.Get<bool>();
            (void)bool_val;
            
            // Test int Value
            int int_test_val = fdp.ConsumeIntegral<int>();
            tinygltf::Value int_value(int_test_val);
            int int_val = int_value.Get<int>();
            (void)int_val;
            
            // Test double Value
            double double_test_val = fdp.ConsumeFloatingPoint<double>();
            tinygltf::Value double_value(double_test_val);
            double double_val = double_value.Get<double>();
            (void)double_val;
            
            // Test string Value
            std::string string_test_val = fdp.ConsumeRandomLengthString(100);
            tinygltf::Value string_value(string_test_val);
            std::string string_val = string_value.Get<std::string>();
            (void)string_val;
            
            // Test array Value
            tinygltf::Value::Array array_test;
            array_test.push_back(tinygltf::Value(42));
            array_test.push_back(tinygltf::Value("test"));
            tinygltf::Value array_value(array_test);
            const tinygltf::Value::Array& array_val = array_value.Get<tinygltf::Value::Array>();
            (void)array_val;
            
            // Test object Value
            tinygltf::Value::Object object_test;
            object_test["int_field"] = tinygltf::Value(123);
            object_test["string_field"] = tinygltf::Value("hello");
            tinygltf::Value object_value(object_test);
            const tinygltf::Value::Object& object_val = object_value.Get<tinygltf::Value::Object>();
            (void)object_val;
            
        } catch (...) {
            // Expected for malformed data or type mismatches
        }
    }
    
    return 0;
}

} // namespace harness_021
