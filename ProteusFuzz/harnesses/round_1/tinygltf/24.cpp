// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
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
#include <fstream>

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char *>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Safe access to Data with bounds checking
    uint8_t val0 = (Size > 0) ? Data[0] : 0;
    uint8_t val1 = (Size > 1) ? Data[1] : 0;
    uint8_t val2 = (Size > 2) ? Data[2] : 0;

    // Fuzz configuration flags
    // Toggle StoreOriginalJSONForExtrasAndExtensions
    bool json_flag = (val0 % 2) != 0;
    loader.SetStoreOriginalJSONForExtrasAndExtensions(json_flag);
    
    // Verify the flag was set
    bool current_json_flag = loader.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)current_json_flag; // Use to avoid unused variable warning

    // Toggle ImagesAsIs
    bool images_as_is_flag = (val1 % 2) != 0;
    loader.SetImagesAsIs(images_as_is_flag);
    
    // Verify the flag was set
    bool current_images_flag = loader.GetImagesAsIs();
    (void)current_images_flag;

    // Determine operation type based on data
    int op_type = val0 % 4;

    // Prepare check_sections flag
    unsigned int check_sections = (val2 % 4);

    if (op_type == 0) {
        // Test LoadASCIIFromString
        loader.LoadASCIIFromString(&model, &err, &warn, 
                                   reinterpret_cast<const char *>(Data), 
                                   Size, 
                                   "./dummy_file", 
                                   check_sections);
    } 
    else if (op_type == 1) {
        // Test LoadASCIIFromFile
        const std::string filename = "./dummy_file";
        WriteDummyFile(filename, Data, Size);
        loader.LoadASCIIFromFile(&model, &err, &warn, filename, check_sections);
    } 
    else if (op_type == 2) {
        // Test LoadBinaryFromFile
        const std::string filename = "./dummy_file.glb";
        WriteDummyFile(filename, Data, Size);
        loader.LoadBinaryFromFile(&model, &err, &warn, filename, check_sections);
    }
    else {
        // Mixed or redundant calls to exercise state
        // Re-check flags after potential loading
        loader.GetStoreOriginalJSONForExtrasAndExtensions();
        loader.GetImagesAsIs();
        
        // Try loading from string again with different parameters
        if (Size > 10) {
             loader.LoadASCIIFromString(&model, &err, &warn, 
                                        reinterpret_cast<const char *>(Data + 10), 
                                        Size - 10, 
                                        "./dummy_file", 
                                        check_sections);
        }
    }

    // Cleanup: Reconstruct the model to clear previous state
    model = tinygltf::Model();

    return 0;
}