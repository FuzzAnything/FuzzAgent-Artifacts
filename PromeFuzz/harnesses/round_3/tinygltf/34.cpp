// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::GetNumComponentsInType at tiny_gltf.h:222:23 in tiny_gltf.h
// tinygltf::GetComponentSizeInBytes at tiny_gltf.h:199:23 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
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

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // 1. GetNumComponentsInType
    if (Size >= sizeof(uint32_t)) {
        uint32_t type_val;
        memcpy(&type_val, Data, sizeof(uint32_t));
        tinygltf::GetNumComponentsInType(type_val);
    }

    // 2. GetComponentSizeInBytes
    if (Size >= sizeof(uint32_t)) {
        uint32_t component_type;
        memcpy(&component_type, Data, sizeof(uint32_t));
        tinygltf::GetComponentSizeInBytes(component_type);
    }

    // 3. URIDecode
    if (Size > 0) {
        std::string in_uri(reinterpret_cast<const char*>(Data), Size);
        std::string out_uri;
        tinygltf::URIDecode(in_uri, &out_uri, nullptr);
    }

    // 4. DecodeDataURI
    if (Size > 0) {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string in_string(reinterpret_cast<const char*>(Data), Size);
        bool checkSize = (Size % 2 == 0);
        size_t reqBytes = Size;
        tinygltf::DecodeDataURI(&out, mime_type, in_string, reqBytes, checkSize);
    }

    // 5. SetURICallbacks
    {
        tinygltf::TinyGLTF loader;
        tinygltf::URICallbacks callbacks;
        callbacks.decode = tinygltf::URIDecode;
        callbacks.encode = nullptr;
        callbacks.user_data = nullptr;
        std::string err;
        loader.SetURICallbacks(callbacks, &err);
    }

    // 6. LoadImageData
    if (Size > 0) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        int req_width = 0;
        int req_height = 0;
        void *user_data = nullptr;
        
        // Set some image properties based on input data
        if (Size >= 2) {
            req_width = Data[0];
            req_height = Data[1];
        }
        
        // Use the remaining data as image bytes
        const unsigned char* image_data = (Size > 2) ? Data + 2 : Data;
        int image_size = (Size > 2) ? Size - 2 : Size;
        
        tinygltf::LoadImageData(&image, 0, &err, &warn, req_width, req_height, 
                               image_data, image_size, user_data);
    }

    return 0;
}