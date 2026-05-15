// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromMemory at tiny_gltf.cc:5109:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.cc:5291:16 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.cc:5066:16 in tiny_gltf.h
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include "tiny_gltf.h"

static std::string dummy_file = "./dummy_file";

// URI encode/decode callbacks with correct signatures
static bool dummy_encode(const std::string &in_uri, const std::string &base_path, 
                        std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

static bool dummy_decode(const std::string &in_uri, std::string *out_uri, 
                        void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Initialize tinygltf object
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    
    // Use first byte to determine which functions to test
    uint8_t selector = 0;
    if (Size > 0) {
        selector = Data[0];
        Data++;
        Size--;
    }
    
    // Test SetURICallbacks with different configurations
    tinygltf::URICallbacks callbacks;
    callbacks.encode = dummy_encode;
    callbacks.decode = dummy_decode;
    callbacks.user_data = nullptr;
    
    // Test with valid callbacks
    if (!loader.SetURICallbacks(callbacks, &err)) {
        // Handle error if needed
    }
    
    // Test with null decode (should fail)
    tinygltf::URICallbacks bad_callbacks;
    bad_callbacks.encode = dummy_encode;
    bad_callbacks.decode = nullptr;
    bad_callbacks.user_data = nullptr;
    loader.SetURICallbacks(bad_callbacks, &err);
    
    // Test GetPreserveImageChannels (simple getter)
    bool preserve = loader.GetPreserveImageChannels();
    
    // Test SetImagesAsIs with different values
    loader.SetImagesAsIs((selector & 0x01) != 0);
    
    // Test LoadBinaryFromMemory if we have enough data
    if (Size > 20) {  // Minimum required by documentation
        // Use the remaining data as binary glTF
        loader.LoadBinaryFromMemory(&model, &err, &warn, Data, Size, "");
        
        // Clear model for next test
        model = tinygltf::Model();
    }
    
    // Test file-based loading functions
    if (Size > 0) {
        // Write fuzzer data to dummy file
        std::ofstream outfile(dummy_file, std::ios::binary);
        if (outfile.is_open()) {
            outfile.write(reinterpret_cast<const char*>(Data), Size);
            outfile.close();
            
            // Test LoadBinaryFromFile
            loader.LoadBinaryFromFile(&model, &err, &warn, dummy_file);
            
            // Clear model for next test
            model = tinygltf::Model();
            
            // Test LoadASCIIFromFile
            loader.LoadASCIIFromFile(&model, &err, &warn, dummy_file);
            
            // Remove dummy file
            std::remove(dummy_file.c_str());
        }
    }
    
    // Test GetPreserveImageChannels again after operations
    preserve = loader.GetPreserveImageChannels();
    
    // Reset callbacks to default
    tinygltf::URICallbacks default_callbacks;
    loader.SetURICallbacks(default_callbacks, nullptr);
    
    return 0;
}