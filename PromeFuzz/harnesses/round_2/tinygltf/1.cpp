// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.cc:944:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetParseStrictness at tiny_gltf.cc:944:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.cc:5053:16 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>

#include "tiny_gltf.h"

static std::string dummy_file = "./dummy_file";

// Dummy URI callbacks for testing - must match the expected function signatures
static bool dummy_uri_encode(const std::string &in_uri, const std::string &base_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

static bool dummy_uri_decode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Initialize objects
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Use first byte to select which function to test
    if (Size == 0) return 0;
    uint8_t selector = Data[0] % 6;
    const uint8_t* actual_data = Data + 1;
    size_t actual_size = Size - 1;
    
    switch (selector) {
        case 0: {
            // Test SetURICallbacks
            tinygltf::URICallbacks callbacks;
            // Use fuzz data to decide whether to set valid or null callbacks
            if (actual_size > 0 && actual_data[0] % 2 == 0) {
                callbacks.encode = dummy_uri_encode;
                callbacks.decode = dummy_uri_decode;
                callbacks.user_data = nullptr;
                loader.SetURICallbacks(callbacks, &err);
            } else {
                // Test with null decode (should fail)
                callbacks.encode = dummy_uri_encode;
                callbacks.decode = nullptr;
                callbacks.user_data = nullptr;
                loader.SetURICallbacks(callbacks, &err);
            }
            break;
        }
        
        case 1: {
            // Test SetParseStrictness
            if (actual_size > 0) {
                if (actual_data[0] % 2 == 0) {
                    loader.SetParseStrictness(tinygltf::ParseStrictness::Permissive);
                } else {
                    loader.SetParseStrictness(tinygltf::ParseStrictness::Strict);
                }
            }
            break;
        }
        
        case 2: {
            // Test LoadBinaryFromFile
            // Write fuzz data to dummy file
            std::ofstream file(dummy_file, std::ios::binary);
            if (file.is_open() && actual_size > 0) {
                file.write(reinterpret_cast<const char*>(actual_data), actual_size);
                file.close();
                loader.LoadBinaryFromFile(&model, &err, &warn, dummy_file);
                // Clean up
                std::remove(dummy_file.c_str());
            }
            break;
        }
        
        case 3: {
            // Test LoadASCIIFromString
            if (actual_size > 0) {
                std::string gltf_str(reinterpret_cast<const char*>(actual_data), actual_size);
                loader.LoadASCIIFromString(&model, &err, &warn, 
                                         gltf_str.c_str(), 
                                         static_cast<unsigned int>(gltf_str.length()),
                                         "");
            }
            break;
        }
        
        case 4: {
            // Test LoadASCIIFromFile
            // Write fuzz data to dummy file
            std::ofstream file(dummy_file, std::ios::binary);
            if (file.is_open() && actual_size > 0) {
                file.write(reinterpret_cast<const char*>(actual_data), actual_size);
                file.close();
                loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file);
                // Clean up
                std::remove(dummy_file.c_str());
            }
            break;
        }
        
        case 5: {
            // Test LoadBinaryFromMemory
            if (actual_size > 0) {
                loader.LoadBinaryFromMemory(&model, &err, &warn,
                                          actual_data,
                                          static_cast<unsigned int>(actual_size),
                                          "");
            }
            break;
        }
    }
    
    return 0;
}