// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1519:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetStoreOriginalJSONForExtrasAndExtensions at tiny_gltf.h:1523:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

#include "tiny_gltf.h"

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static bool FileExists(const std::string &filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;

    // Test SetStoreOriginalJSONForExtrasAndExtensions and GetStoreOriginalJSONForExtrasAndExtensions
    // Toggle state to explore different code paths
    bool toggle = (Data[0] % 2) == 0;
    ctx.SetStoreOriginalJSONForExtrasAndExtensions(toggle);
    bool current_state = ctx.GetStoreOriginalJSONForExtrasAndExtensions();
    (void)current_state; // Suppress unused variable warning

    // Split input data to test different loading scenarios
    const char *str_data = reinterpret_cast<const char*>(Data);
    size_t str_len = Size;

    // 1. Test LoadASCIIFromString (overload with length)
    // Use the raw input as a potential JSON string
    {
        tinygltf::Model model;
        ctx.LoadASCIIFromString(&model, &err, &warn, str_data, str_len, "", tinygltf::REQUIRE_VERSION);
        err.clear();
        warn.clear();
    }

    // 2. Test LoadASCIIFromString (overload without length, using strlen)
    // Ensure null termination for safety if we treat it as a C-string
    {
        tinygltf::Model model;
        std::string safe_str(reinterpret_cast<const char*>(Data), Size);
        safe_str += '\0';
        const char *c_str = safe_str.c_str();
        ctx.LoadASCIIFromString(&model, &err, &warn, c_str, safe_str.length(), "", tinygltf::REQUIRE_VERSION);
        err.clear();
        warn.clear();
    }

    // 3. Test LoadASCIIFromFile
    // Write data to a dummy file with .gltf extension
    const std::string ascii_filename = "./dummy_file.gltf";
    WriteDummyFile(ascii_filename, Data, Size);
    
    {
        tinygltf::Model model;
        // Try with different check_sections flags based on input data
        unsigned int check_flag_ascii = (Size > 0) ? (Data[0] % 2) : 0;
        ctx.LoadASCIIFromFile(&model, &err, &warn, ascii_filename, check_flag_ascii);
        err.clear();
        warn.clear();
    }

    // 4. Test LoadBinaryFromFile
    // Write data to a dummy file with .glb extension
    const std::string binary_filename = "./dummy_file.glb";
    WriteDummyFile(binary_filename, Data, Size);

    {
        tinygltf::Model model;
        unsigned int check_flag_binary = (Size > 0) ? (Data[0] % 2) : 0;
        ctx.LoadBinaryFromFile(&model, &err, &warn, binary_filename, check_flag_binary);
        err.clear();
        warn.clear();
    }

    // Cleanup dummy files
    std::remove(ascii_filename.c_str());
    std::remove(binary_filename.c_str());

    return 0;
}