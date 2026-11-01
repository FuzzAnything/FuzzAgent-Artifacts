// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
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
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// Fuzz driver for tinygltf API functions
extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to simulate glTF loading scenarios if needed
    const char *dummy_file = "./dummy_file";
    std::ofstream ofs(dummy_file, std::ios::binary);
    if (ofs.is_open()) {
        ofs.write(reinterpret_cast<const char*>(Data), Size);
        ofs.close();
    }

    // Initialize tinygltf Model and other required structures
    tinygltf::Model model;
    tinygltf::Value value;
    tinygltf::AnimationChannel channel;
    std::string err;

    // Attempt to parse the input as JSON using tinygltf's internal JSON parser
    // We use a Model loader to parse JSON, which internally uses the target functions
    tinygltf::TinyGLTF loader;
    std::string warn, err_msg;
    
    // Try loading from the dummy file
    bool loaded = loader.LoadASCIIFromString(&model, &warn, &err_msg, 
                                              reinterpret_cast<const char*>(Data), Size, dummy_file);
    
    // If loading failed, we can still try to test the specific API functions directly
    // by constructing JSON objects manually if possible, or by using the parsed data
    // if the load was partially successful.

    // Test ParseJsonAsValue indirectly through the model loading process
    // The function is called internally when parsing JSON members.
    
    // Test IsEmpty, IsArray, FindMember, ForEachInArray, ParseAnimationChannel
    // These are internal functions, so we test them indirectly through the model loading
    // or by accessing the model's data structures if they were populated.

    // If the model was loaded (even partially), we can iterate over its animations
    // to trigger ParseAnimationChannel logic
    if (!model.animations.empty()) {
        for (const auto &anim : model.animations) {
            for (const auto &channel_data : anim.channels) {
                // Accessing channel_data triggers the parsing logic if it was done lazily
                (void)channel_data.sampler;
                (void)channel_data.target_path;
            }
        }
    }

    // Clean up: The model and other objects will be destructed automatically
    // when they go out of scope.

    // Return 0 to indicate successful execution (no crash)
    return 0;
}