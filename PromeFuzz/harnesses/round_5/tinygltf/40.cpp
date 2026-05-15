// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::GetNumComponentsInType at tiny_gltf.h:222:23 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::GetComponentSizeInBytes at tiny_gltf.h:199:23 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
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
#include <vector>
#include <string>
#include <cstring>

static bool dummyURIDecode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

static bool dummyURIDecodeFail(const std::string &in_uri, std::string *out_uri, void *user_data) {
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Function 1: GetNumComponentsInType
    if (Size > 0) {
        uint32_t type = Data[0];
        tinygltf::GetNumComponentsInType(type);
    }

    // Function 2: DecodeDataURI
    if (Size > 0) {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string in_str(reinterpret_cast<const char*>(Data), Size);
        bool checkSize = (Size % 2 == 0);
        size_t reqBytes = (Size > 4) ? (Data[0] * 256 + Data[1]) : 0;
        tinygltf::DecodeDataURI(&out, mime_type, in_str, reqBytes, checkSize);
    }

    // Function 3: SetURICallbacks
    {
        tinygltf::TinyGLTF loader;
        std::string err;
        tinygltf::URICallbacks callbacks;
        
        // Test with valid decode callback
        callbacks.decode = dummyURIDecode;
        callbacks.encode = nullptr;
        callbacks.user_data = nullptr;
        loader.SetURICallbacks(callbacks, &err);
        
        // Test with null decode callback
        callbacks.decode = nullptr;
        loader.SetURICallbacks(callbacks, &err);
        
        // Test with failing decode callback
        callbacks.decode = dummyURIDecodeFail;
        loader.SetURICallbacks(callbacks, nullptr);
    }

    // Function 4: GetComponentSizeInBytes
    if (Size > 0) {
        uint32_t componentType = Data[0];
        tinygltf::GetComponentSizeInBytes(componentType);
    }

    // Function 5: URIDecode
    if (Size > 0) {
        std::string out_uri;
        std::string in_uri(reinterpret_cast<const char*>(Data), Size);
        tinygltf::URIDecode(in_uri, &out_uri, nullptr);
    }

    // Function 6: LoadImageData
    if (Size > 0) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        int image_idx = (Size > 1) ? Data[0] : 0;
        int req_width = (Size > 2) ? Data[1] : 0;
        int req_height = (Size > 3) ? Data[2] : 0;
        bool as_is = (Size > 4) ? (Data[3] % 2 == 0) : false;
        image.as_is = as_is;
        
        // Test with different image data
        const unsigned char* bytes = Data;
        int size = static_cast<int>(Size);
        tinygltf::LoadImageData(&image, image_idx, &err, &warn, req_width, req_height, bytes, size, nullptr);
        
        // Test with empty data
        if (Size > 10) {
            tinygltf::LoadImageData(&image, image_idx, &err, &warn, req_width, req_height, nullptr, 0, nullptr);
        }
    }

    return 0;
}