// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

// Helper to write data to a file
static bool WriteFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    file.write(reinterpret_cast<const char *>(data), size);
    file.close();
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // 1. Test GetStoreOriginalJSONForExtrasAndExtensions (initial state)
    bool original_json_enabled = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)original_json_enabled;

    // 2. Test SetStoreOriginalJSONForExtrasAndExtensions
    // Use first byte to decide whether to enable or disable
    bool enable_flag = (Data[0] % 2 == 0);
    loader.SetStoreOriginalJSONForExtrasAndExtensions(enable_flag);

    // Verify the setting was applied
    bool current_json_enabled = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    if (current_json_enabled != enable_flag) {
        // This is unexpected but not a crash, just a logic check
    }

    // 3. Test GetImagesAsIs (initial state)
    bool images_as_is = loader.GetImagesAsIs();
    (void)images_as_is;

    // Prepare file content
    std::string filename = "./dummy_file";
    
    // Write data to file to test file-based loaders
    if (!WriteFile(filename, Data, Size)) {
        return 0;
    }

    // 4. Test LoadBinaryFromFile
    // We pass the raw fuzzed data as a potential .glb file
    // check_sections can be varied based on input
    // Ensure we have enough data before accessing Data[1]
    unsigned int check_sections = 0;
    if (Size > 1) {
        check_sections = (Data[1] % 4); 
    }
    
    bool binary_result = loader.LoadBinaryFromFile(&model, &err, &warn, filename, check_sections);
    (void)binary_result;

    // Reset model for next test
    model = tinygltf::Model();

    // 5. Test LoadASCIIFromFile
    // Same file content treated as ASCII glTF
    bool ascii_file_result = loader.LoadASCIIFromFile(&model, &err, &warn, filename, check_sections);
    (void)ascii_file_result;

    // Reset model for next test
    model = tinygltf::Model();

    // 6. Test LoadASCIIFromString
    // Load directly from the fuzzed buffer as a string
    // Ensure we don't pass null pointer if size is 0 (handled at start)
    const char *str_data = reinterpret_cast<const char *>(Data);
    unsigned int str_length = static_cast<unsigned int>(Size);
    std::string base_dir = "";
    
    bool ascii_str_result = loader.LoadASCIIFromString(&model, &err, &warn, str_data, str_length, base_dir, check_sections);
    (void)ascii_str_result;

    // 7. Final check of GetImagesAsIs after loading attempts
    bool final_images_as_is = loader.GetImagesAsIs();
    (void)final_images_as_is;

    // Cleanup
    // tinygltf::Model destructor handles cleanup of internal resources
    return 0;
}