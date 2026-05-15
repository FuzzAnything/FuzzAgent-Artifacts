#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <algorithm>
#include "tiny_gltf.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_022 {
/*
 * Fuzzing harness for tinygltf targeting comprehensive GLTF loading lifecycle:
 * Configuration → Loading → Data Access → Cleanup
 * 
 * Target APIs with coverage gaps:
 * - LoadASCIIFromString (176 branches, 62 undiscovered)
 * - LoadBinaryFromFile (252 branches, 107 undiscovered) 
 * - LoadBinaryFromMemory (244 branches, 101 undiscovered)
 * - SetImageLoader, SetImageWriter, SetFsCallbacks, SetURICallbacks (Configuration APIs, 0% covered)
 * - Parameter::TextureScale, Parameter::ColorFactor (Data access, completely uncovered)
 * - Value::Get<object> (Value access, completely uncovered)
 *
 * These functions represent 600+ points of undiscovered branch complexity.
 * The loading APIs are core to tinygltf functionality with massive uncovered branch complexity.
 * Configuration APIs are 0% covered. Value and Parameter accessors are completely untouched
 * despite being essential for accessing parsed GLTF data.
 *
 * Invocation sequence:
 * 1. Create TinyGLTF instance
 * 2. Configure with SetImageLoader, SetImageWriter, SetFsCallbacks, SetURICallbacks
 * 3. Call LoadASCIIFromString with test GLTF data
 * 4. Call LoadBinaryFromMemory with binary GLTF data
 * 5. Access parsed model data using Parameter accessors (TextureScale, ColorFactor, etc.)
 * 6. Access Value data using Value::Get methods
 * 7. Call LoadBinaryFromFile if file system callbacks are properly configured
 * 8. Clean up Model objects
 *
 * Focus on comprehensive GLTF loading lifecycle: Configure → Load → Access → Cleanup.
 * Target the lowest covered modules (tiny_gltf.h has only 28.5% line coverage).
 *
 * Uses FuzzedDataProvider for structured input splitting with no hardcoded test data.
 */


// Include tinygltf with all implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION


// Custom image loader callback for SetImageLoader
static bool custom_image_loader(tinygltf::Image* image, const int image_idx,
                               std::string* err, std::string* warn,
                               int req_width, int req_height,
                               const unsigned char* bytes, int size,
                               void* user_data) {
    // Simple image loader that always succeeds
    if (size > 0 && bytes != nullptr) {
        image->width = req_width > 0 ? req_width : 64;
        image->height = req_height > 0 ? req_height : 64;
        image->component = 4; // RGBA
        image->bits = 8;
        image->pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
        
        // Create some dummy pixel data
        size_t pixel_count = image->width * image->height * image->component;
        image->image.resize(pixel_count, 255); // White image
        
        return true;
    }
    return false;
}

// Custom image writer callback for SetImageWriter
static bool custom_image_writer(const std::string* basepath,
                               const std::string* filename,
                               const tinygltf::Image* image,
                               bool embedImages,
                               const tinygltf::FsCallbacks* fs_cb,
                               const tinygltf::URICallbacks* uri_cb,
                               std::string* out_uri,
                               void* user_pointer) {
    // Simple image writer that always succeeds
    if (out_uri != nullptr) {
        *out_uri = "data:image/png;base64,test";
        return true;
    }
    return false;
}

// Custom filesystem callbacks for SetFsCallbacks
static bool custom_file_exists(const std::string& abs_filename, void* userdata) {
    return true; // Always say file exists
}

static std::string custom_expand_filepath(const std::string& filepath, void* userdata) {
    return filepath; // No expansion
}

static bool custom_read_whole_file(std::vector<unsigned char>* out,
                                   std::string* err,
                                   const std::string& filepath,
                                   void* userdata) {
    // Read from a global buffer if available
    static std::vector<unsigned char> dummy_data = {1, 2, 3, 4, 5};
    if (out != nullptr) {
        *out = dummy_data;
        return true;
    }
    return false;
}

static bool custom_write_whole_file(std::string* err,
                                    const std::string& filepath,
                                    const std::vector<unsigned char>& contents,
                                    void* userdata) {
    return true; // Always succeed
}

static bool custom_get_file_size(size_t* filesize_out,
                                 std::string* err,
                                 const std::string& abs_filename,
                                 void* userdata) {
    if (filesize_out != nullptr) {
        *filesize_out = 1024; // Dummy size
        return true;
    }
    return false;
}

// Custom URI callbacks for SetURICallbacks
static bool custom_encode_uri(const std::string& in_uri, 
                             const std::string& object_type,
                             std::string* out_uri,
                             void* user_data) {
    if (out_uri != nullptr) {
        *out_uri = in_uri; // Simple encoding: return input as-is
        return true;
    }
    return false;
}

static bool custom_decode_uri(const std::string& in_uri,
                             std::string* out_uri,
                             void* user_data) {
    if (out_uri != nullptr) {
        *out_uri = in_uri; // Simple decoding: return input as-is
        return true;
    }
    return false;
}

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

// Function to create a simple GLTF JSON for testing
static std::string create_simple_gltf_json(FuzzedDataProvider& fdp) {
    // Consume some configuration options
    bool include_material = fdp.ConsumeBool();
    bool include_texture = fdp.ConsumeBool();
    bool include_image = fdp.ConsumeBool();
    
    // Consume some string data
    std::string material_name = fdp.ConsumeRandomLengthString(20);
    std::string texture_name = fdp.ConsumeRandomLengthString(20);
    
    // Consume numeric values for material properties
    double color_r = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double color_g = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double color_b = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double color_a = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double metallic_factor = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double roughness_factor = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double texture_scale = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    double texture_strength = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    double emissive_r = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double emissive_g = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double emissive_b = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    
    // Build minimal GLTF JSON
    std::string gltf = R"({
  "asset": {
    "version": "2.0",
    "generator": "tinygltf-fuzzer"
  })";
    
    if (include_material) {
        gltf += R"(,
  "materials": [
    {
      "name": ")" + material_name + R"(",
      "pbrMetallicRoughness": {
        "baseColorFactor": [)" + 
        std::to_string(color_r) + ", " + 
        std::to_string(color_g) + ", " + 
        std::to_string(color_b) + ", " + 
        std::to_string(color_a) + R"(],
        "metallicFactor": )" + std::to_string(metallic_factor) + R"(,
        "roughnessFactor": )" + std::to_string(roughness_factor) + R"(
      },
      "normalTexture": {
        "index": 0,
        "scale": )" + std::to_string(texture_scale) + R"(
      },
      "occlusionTexture": {
        "index": 0,
        "strength": )" + std::to_string(texture_strength) + R"(
      },
      "emissiveFactor": [)" + 
        std::to_string(emissive_r) + ", " + 
        std::to_string(emissive_g) + ", " + 
        std::to_string(emissive_b) + R"(]
    }
  ])";
    }
    
    if (include_texture) {
        gltf += R"(,
  "textures": [
    {
      "name": ")" + texture_name + R"(",
      "sampler": 0,
      "source": 0
    }
  ],
  "samplers": [
    {
      "magFilter": 9729,
      "minFilter": 9729
    }
  ])";
    }
    
    if (include_image) {
        gltf += R"(,
  "images": [
    {
      "uri": "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mNkYPhfDwAChwGA60e6kgAAAABJRU5ErkJggg=="
    }
  ])";
    }
    
    gltf += "\n}";
    
    return gltf;
}

// Function to test Parameter accessors on loaded model
static void test_parameter_accessors(const tinygltf::Model& model) {
    // Try to access Parameter data from materials
    for (const auto& material : model.materials) {
        // Create a Parameter-like structure to test TextureScale and ColorFactor
        tinygltf::Parameter param;
        
        // Test TextureScale - this should work if material has normalTexture with scale
        if (material.normalTexture.scale != 0.0) {
            // Note: TextureScale() is a method on Parameter, not on Material
            // We need to create a Parameter with appropriate json_double_value
            tinygltf::Parameter texture_param;
            texture_param.json_double_value["scale"] = material.normalTexture.scale;
            double scale = texture_param.TextureScale();
            (void)scale; // Use result
        }
        
        // Test ColorFactor - this should work if material has pbrMetallicRoughness
        if (material.pbrMetallicRoughness.baseColorFactor.size() >= 3) {
            tinygltf::Parameter color_param;
            color_param.number_array = material.pbrMetallicRoughness.baseColorFactor;
            tinygltf::ColorValue color = color_param.ColorFactor();
            (void)color; // Use result
        }
    }
}

// Function to test Value::Get methods using fuzzed data
static void test_value_get_methods(FuzzedDataProvider& fdp) {
    // Check if we have enough data for testing
    if (fdp.remaining_bytes() < 20) {
        return;
    }
    
    // Create various Value objects and test Get<> methods using fuzzed data
    bool bool_val_data = fdp.ConsumeBool();
    tinygltf::Value bool_val(bool_val_data);
    if (bool_val.IsBool()) {
        bool b = bool_val.Get<bool>();
        (void)b;
    }
    
    int int_val_data = fdp.ConsumeIntegral<int>();
    tinygltf::Value int_val(int_val_data);
    if (int_val.IsInt()) {
        int i = int_val.Get<int>();
        (void)i;
    }
    
    double real_val_data = fdp.ConsumeFloatingPoint<double>();
    tinygltf::Value real_val(real_val_data);
    if (int_val.IsReal()) {
        double d = real_val.Get<double>();
        (void)d;
    }
    
    std::string string_val_data = fdp.ConsumeRandomLengthString(50);
    tinygltf::Value string_val(string_val_data);
    if (string_val.IsString()) {
        std::string s = string_val.Get<std::string>();
        (void)s;
    }
    
    // Create binary data
    size_t binary_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
    std::vector<unsigned char> binary_data = fdp.ConsumeBytes<unsigned char>(binary_size);
    tinygltf::Value binary_val(std::move(binary_data));
    if (binary_val.IsBinary()) {
        std::vector<unsigned char> b = binary_val.Get<std::vector<unsigned char>>();
        (void)b;
    }
    
    // Create array value
    size_t array_size = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    tinygltf::Value::Array array_val;
    for (size_t i = 0; i < array_size; i++) {
        int array_elem = fdp.ConsumeIntegral<int>();
        array_val.push_back(tinygltf::Value(array_elem));
    }
    tinygltf::Value array_obj(array_val);
    if (array_obj.IsArray()) {
        tinygltf::Value::Array a = array_obj.Get<tinygltf::Value::Array>();
        (void)a;
    }
    
    // Create object value
    size_t object_size = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    tinygltf::Value::Object object_val;
    for (size_t i = 0; i < object_size; i++) {
        std::string key = "key" + std::to_string(i);
        std::string value_str = fdp.ConsumeRandomLengthString(20);
        object_val[key] = tinygltf::Value(value_str);
    }
    tinygltf::Value object_obj(object_val);
    if (object_obj.IsObject()) {
        tinygltf::Value::Object o = object_obj.Get<tinygltf::Value::Object>();
        (void)o;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0; // Need minimum data for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume selector to determine which operations to perform
    uint8_t selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Determine which configuration APIs to test
    bool test_image_loader = (selector & 0x01) != 0;
    bool test_image_writer = (selector & 0x02) != 0;
    bool test_fs_callbacks = (selector & 0x04) != 0;
    bool test_uri_callbacks = (selector & 0x08) != 0;
    
    // Determine which loading APIs to test
    bool test_ascii_load = (selector & 0x10) != 0;
    bool test_binary_memory_load = (selector & 0x20) != 0;
    bool test_binary_file_load = (selector & 0x40) != 0;
    
    // Determine which data access APIs to test
    bool test_parameter_access = (selector & 0x80) != 0;
    
    // Consume check_sections flags
    unsigned int check_sections = 0;
    bool check_version = fdp.ConsumeBool();
    bool check_scene = fdp.ConsumeBool();
    bool check_all = fdp.ConsumeBool();
    
    if (check_version) check_sections |= tinygltf::REQUIRE_VERSION;
    if (check_scene) check_sections |= tinygltf::REQUIRE_SCENE;
    if (check_all) check_sections = tinygltf::REQUIRE_ALL;
    
    // Create TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Configure with custom callbacks based on selector
    if (test_image_loader) {
        loader.SetImageLoader(custom_image_loader, nullptr);
    }
    
    if (test_image_writer) {
        loader.SetImageWriter(custom_image_writer, nullptr);
    }
    
    if (test_fs_callbacks) {
        tinygltf::FsCallbacks fs_callbacks;
        fs_callbacks.FileExists = custom_file_exists;
        fs_callbacks.ExpandFilePath = custom_expand_filepath;
        fs_callbacks.ReadWholeFile = custom_read_whole_file;
        fs_callbacks.WriteWholeFile = custom_write_whole_file;
        fs_callbacks.GetFileSizeInBytes = custom_get_file_size;
        fs_callbacks.user_data = nullptr;
        
        std::string fs_err;
        loader.SetFsCallbacks(fs_callbacks, &fs_err);
    }
    
    if (test_uri_callbacks) {
        tinygltf::URICallbacks uri_callbacks;
        uri_callbacks.encode = custom_encode_uri;
        uri_callbacks.decode = custom_decode_uri;
        uri_callbacks.user_data = nullptr;
        
        std::string uri_err;
        loader.SetURICallbacks(uri_callbacks, &uri_err);
    }
    
    // Test Value::Get methods (always test these as they're completely uncovered)
    test_value_get_methods(fdp);
    
    // Test LoadASCIIFromString if selected
    if (test_ascii_load) {
        tinygltf::Model model1;
        std::string err1, warn1;
        
        // Create GLTF JSON from fuzzed data
        std::string gltf_json = create_simple_gltf_json(fdp);
        
        bool ret1 = loader.LoadASCIIFromString(&model1, &err1, &warn1,
                                              gltf_json.c_str(),
                                              static_cast<unsigned int>(gltf_json.size()),
                                              "", check_sections);
                                              
        // Test Parameter accessors on loaded model if successful
        if (ret1 && test_parameter_access) {
            test_parameter_accessors(model1);
        }
    }
    
    // Get remaining data for GLTF payload (for binary loading tests)
    std::vector<uint8_t> payload = fdp.ConsumeRemainingBytes<uint8_t>();
    if (payload.empty()) {
        return 0;
    }

    
    // Test LoadBinaryFromMemory if selected
    if (test_binary_memory_load && payload.size() > 0) {
        tinygltf::Model model2;
        std::string err2, warn2;
        
        bool ret2 = loader.LoadBinaryFromMemory(&model2, &err2, &warn2,
                                               payload.data(),
                                               static_cast<unsigned int>(payload.size()),
                                               "", check_sections);
                                               
        // Test Parameter accessors on loaded model if successful
        if (ret2 && test_parameter_access) {
            test_parameter_accessors(model2);
        }
    }
    
    // Test LoadBinaryFromFile if selected (requires file system callbacks)
    if (test_binary_file_load && test_fs_callbacks) {
        tinygltf::Model model3;
        std::string err3, warn3;
        
        // Create temporary file with payload
        std::string temp_filename = create_temp_file(payload);
        if (!temp_filename.empty()) {
            bool ret3 = loader.LoadBinaryFromFile(&model3, &err3, &warn3,
                                                 temp_filename, check_sections);
            
            // Test Parameter accessors on loaded model if successful
            if (ret3 && test_parameter_access) {
                test_parameter_accessors(model3);
            }
            
            // Cleanup temporary file
            cleanup_temp_file(temp_filename);
        }
    }
    
    return 0;
}

} // namespace harness_022
