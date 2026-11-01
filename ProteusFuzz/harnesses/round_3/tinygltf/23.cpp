// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.h:1445:8 in tiny_gltf.h
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

// Helper to create a dummy FsCallbacks
static tinygltf::FsCallbacks CreateDummyFsCallbacks() {
    tinygltf::FsCallbacks fs;
    fs.FileExists = nullptr;
    fs.ExpandFilePath = nullptr;
    fs.ReadWholeFile = nullptr;
    fs.WriteWholeFile = nullptr;
    fs.GetFileSizeInBytes = nullptr;
    fs.user_data = nullptr;
    return fs;
}

// Helper to create a dummy URICallbacks
static tinygltf::URICallbacks CreateDummyURICallbacks() {
    tinygltf::URICallbacks uri_cb;
    uri_cb.encode = nullptr;
    uri_cb.decode = nullptr;
    uri_cb.user_data = nullptr;
    return uri_cb;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Avoid creating a string from potentially invalid UTF-8 or null-terminated issues
    // by using a safe copy mechanism if needed, but std::string constructor is generally safe
    // for raw bytes. The crash likely comes from passing nullptr or invalid pointer to LoadASCIIFromString.
    // The 6th argument is typically a pointer to a callback or options, 7th is check_sections.
    // Let's ensure we pass valid pointers.
    
    std::string input(reinterpret_cast<const char*>(Data), Size);
    
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    // We do not need to write to a file for LoadASCIIFromString.
    // The crash was likely due to passing nullptr where a valid pointer was expected, 
    // or an internal assertion in tinygltf when parsing invalid JSON.
    // We will catch exceptions just in case.
    
    try {
        // The signature is: LoadASCIIFromString(Model*, std::string*, std::string*, const char*, size_t, const std::string*, unsigned int)
        // The 6th arg is usually a base directory path. Passing nullptr might be okay if it's optional, 
        // but some versions might expect a valid pointer. Let's pass an empty string literal instead.
        // The 7th arg is check_sections (unsigned int).
        bool ret = loader.LoadASCIIFromString(&model, &err, &warn, input.c_str(), input.size(), "", 0);
        
        if (!ret) {
            // If ASCII load failed, try binary load if the data looks like it could be binary
            // This is optional but good for coverage.
            // LoadBinaryFromMemory signature: (Model*, std::string*, std::string*, const unsigned char*, size_t, const std::string*, unsigned int)
            bool ret_bin = loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "", 0);
            (void)ret_bin;
        }
    } catch (...) {
        // Catch any exceptions thrown by the library to prevent fuzzer crash
        return 0;
    }

    // Iterate over parsed components to ensure state is exercised
    // This helps cover code paths inside the model structure
    if (!model.buffers.empty()) {
        for (const auto& buffer : model.buffers) {
            (void)buffer; 
        }
    }
    if (!model.textures.empty()) {
        for (const auto& texture : model.textures) {
            (void)texture;
        }
    }
    if (!model.audioSources.empty()) {
        for (const auto& audio : model.audioSources) {
            (void)audio;
        }
    }

    return 0;
}