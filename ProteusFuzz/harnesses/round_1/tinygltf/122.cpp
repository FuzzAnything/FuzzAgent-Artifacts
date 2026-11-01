// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
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

static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char *>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Determine which API to fuzz based on the first byte
    uint8_t mode = Data[0] % 6;

    // Prepare data for file-based operations if needed
    std::string dummy_filename = "./dummy_file";
    WriteDummyFile(dummy_filename, Data, Size);

    // Helper to safely get check_sections value
    auto get_check_sections = [&](size_t idx) -> unsigned int {
        if (idx < Size) {
            return Data[idx] % 3;
        }
        return 0;
    };

    switch (mode) {
        case 0: {
            // Fuzz LoadASCIIFromFile
            unsigned int check_sections = get_check_sections(1);
            ctx.LoadASCIIFromFile(&model, &err, &warn, dummy_filename, check_sections);
            break;
        }
        case 1: {
            // Fuzz LoadBinaryFromFile
            unsigned int check_sections = get_check_sections(1);
            ctx.LoadBinaryFromFile(&model, &err, &warn, dummy_filename, check_sections);
            break;
        }
        case 2: {
            // Fuzz LoadBinaryFromMemory
            // Use the input data directly as binary buffer
            unsigned int check_sections = get_check_sections(1);
            std::string base_dir = "./";
            ctx.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, base_dir, check_sections);
            break;
        }
        case 3: {
            // Fuzz LoadASCIIFromString
            // Interpret input as ASCII string (cast to char*)
            // Ensure null termination for safety if used as C-string, though API uses length
            unsigned int check_sections = get_check_sections(1);
            std::string base_dir = "./";
            ctx.LoadASCIIFromString(&model, &err, &warn, reinterpret_cast<const char *>(Data), Size, base_dir, check_sections);
            break;
        }
        case 4: {
            // Fuzz SetURICallbacks
            tinygltf::URICallbacks callbacks;
            // Set to null to test error handling, or valid functions if we wanted to test success
            callbacks.encode = nullptr;
            callbacks.decode = nullptr;
            callbacks.user_data = nullptr;
            ctx.SetURICallbacks(callbacks, &err);
            
            // Also try with a dummy valid-looking callback structure if we had functions, 
            // but for fuzzing, triggering the null check is valuable.
            break;
        }
        case 5: {
            // Fuzz SetFsCallbacks
            tinygltf::FsCallbacks callbacks;
            callbacks.FileExists = nullptr;
            callbacks.ExpandFilePath = nullptr;
            callbacks.ReadWholeFile = nullptr;
            callbacks.WriteWholeFile = nullptr;
            callbacks.GetFileSizeInBytes = nullptr;
            callbacks.user_data = nullptr;
            ctx.SetFsCallbacks(callbacks, &err);
            break;
        }
        default:
            break;
    }

    // Cleanup is implicit as model is a local variable.
    // Errors are stored in 'err' string which is also local.
    return 0;
}