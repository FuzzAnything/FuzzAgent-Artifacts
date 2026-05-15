// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::GetComponentSizeInBytes at tiny_gltf.h:199:23 in tiny_gltf.h
// tinygltf::GetNumComponentsInType at tiny_gltf.h:222:23 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
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
#include <vector>
#include <string>
#include <fstream>

static bool dummy_uri_encode(const std::string &in_uri, const std::string &base_uri, std::string *out_uri, void *user_data) {
    *out_uri = in_uri;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Function 1: GetNumComponentsInType
    if (Size >= sizeof(uint32_t)) {
        uint32_t ty;
        memcpy(&ty, Data, sizeof(uint32_t));
        tinygltf::GetNumComponentsInType(ty);
    }

    // Function 2: SetURICallbacks
    {
        tinygltf::TinyGLTF loader;
        tinygltf::URICallbacks callbacks;
        std::string err;
        
        // Use fuzzer data to decide callback configuration
        if (Size > 0) {
            if (Data[0] & 0x01) {
                callbacks.encode = dummy_uri_encode;
                callbacks.decode = tinygltf::URIDecode;
            } else {
                callbacks.encode = nullptr;
                callbacks.decode = nullptr;
            }
            callbacks.user_data = nullptr;
            loader.SetURICallbacks(callbacks, &err);
        }
    }

    // Function 3: DecodeDataURI
    if (Size > 0) {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string in_str(reinterpret_cast<const char*>(Data), 
                          Size > 1024 ? 1024 : Size); // Limit size
        
        // Use fuzzer data to determine parameters
        bool checkSize = (Size > 1 && (Data[1] & 0x01));
        size_t reqBytes = 0;
        if (Size > sizeof(size_t) + 2) {
            memcpy(&reqBytes, Data + 2, sizeof(size_t));
        }
        
        tinygltf::DecodeDataURI(&out, mime_type, in_str, reqBytes, checkSize);
    }

    // Function 4: LoadImageData
    if (Size > 0) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        
        // Use fuzzer data to set parameters
        int req_width = 0;
        int req_height = 0;
        if (Size >= 2 * sizeof(int) + 2) {
            memcpy(&req_width, Data, sizeof(int));
            memcpy(&req_height, Data + sizeof(int), sizeof(int));
        }
        
        // Limit image data size to prevent excessive memory usage
        size_t image_data_size = Size > 65536 ? 65536 : Size;
        const unsigned char* image_data = Data;
        
        tinygltf::LoadImageData(&image, 0, &err, &warn, 
                               req_width, req_height, 
                               image_data, static_cast<int>(image_data_size), 
                               nullptr);
    }

    // Function 5: URIDecode
    if (Size > 0) {
        std::string out_uri;
        std::string in_uri(reinterpret_cast<const char*>(Data), 
                          Size > 1024 ? 1024 : Size); // Limit size
        tinygltf::URIDecode(in_uri, &out_uri, nullptr);
    }

    // Function 6: GetComponentSizeInBytes
    if (Size >= sizeof(uint32_t)) {
        uint32_t componentType;
        memcpy(&componentType, Data, sizeof(uint32_t));
        tinygltf::GetComponentSizeInBytes(componentType);
    }

    return 0;
}