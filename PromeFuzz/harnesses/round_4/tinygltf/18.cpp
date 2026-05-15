// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.cc:944:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.cc:944:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
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
#include <fstream>
#include <string>

static std::string dummy_file_path = "./dummy_file";

static tinygltf::URICallbacks create_uri_callbacks(const uint8_t *data, size_t size) {
    tinygltf::URICallbacks callbacks;
    
    // Simple encode callback that returns the input string
    callbacks.encode = [](const std::string &in, const std::string &baseuri, 
                          std::string *out, void *user_data) -> bool {
        if (!out) return false;
        *out = in;
        return true;
    };
    
    // Simple decode callback that returns the input string
    callbacks.decode = [](const std::string &in, std::string *out, 
                          void *user_data) -> bool {
        if (!out) return false;
        *out = in;
        return true;
    };
    
    // Use fuzz data as user data
    callbacks.user_data = const_cast<void*>(static_cast<const void*>(data));
    return callbacks;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // 1. Test SetURICallbacks
    {
        tinygltf::URICallbacks callbacks = create_uri_callbacks(Data, Size);
        bool result = loader.SetURICallbacks(callbacks, &err);
        (void)result; // Use result to avoid unused variable warning
    }
    
    // 2. Test SetParseStrictness
    {
        // Use first byte to decide strictness
        if (Data[0] & 0x01) {
            loader.SetParseStrictness(tinygltf::ParseStrictness::Strict);
        } else {
            loader.SetParseStrictness(tinygltf::ParseStrictness::Permissive);
        }
    }
    
    // 3. Test LoadASCIIFromString
    {
        // Use Data as the string content
        std::string gltf_str(reinterpret_cast<const char*>(Data), Size);
        bool result = loader.LoadASCIIFromString(&model, &err, &warn, 
            gltf_str.c_str(), static_cast<unsigned int>(gltf_str.size()), 
            "./", tinygltf::REQUIRE_VERSION);
        (void)result;
    }
    
    // 4. Test LoadBinaryFromMemory
    {
        // Reset model for clean test
        model = tinygltf::Model();
        bool result = loader.LoadBinaryFromMemory(&model, &err, &warn, 
            Data, static_cast<unsigned int>(Size), 
            "./", tinygltf::REQUIRE_VERSION);
        (void)result;
    }
    
    // 5. Test LoadASCIIFromFile and LoadBinaryFromFile
    {
        // Write fuzz data to dummy file
        std::ofstream outfile(dummy_file_path, std::ios::binary);
        if (outfile.is_open()) {
            outfile.write(reinterpret_cast<const char*>(Data), Size);
            outfile.close();
            
            // Test ASCII file loading
            model = tinygltf::Model();
            bool ascii_result = loader.LoadASCIIFromFile(&model, &err, &warn, 
                dummy_file_path, tinygltf::REQUIRE_VERSION);
            (void)ascii_result;
            
            // Test binary file loading
            model = tinygltf::Model();
            bool binary_result = loader.LoadBinaryFromFile(&model, &err, &warn, 
                dummy_file_path, tinygltf::REQUIRE_VERSION);
            (void)binary_result;
            
            // Clean up dummy file
            std::remove(dummy_file_path.c_str());
        }
    }
    
    return 0;
}