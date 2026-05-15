// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::GetNumComponentsInType at tiny_gltf.h:222:23 in tiny_gltf.h
// tinygltf::GetComponentSizeInBytes at tiny_gltf.h:199:23 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
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
#include <cstddef>
#include <vector>
#include <string>

static bool dummy_encode(const std::string &base_uri, const std::string &relative_uri, std::string *out_uri, void *user_data) {
    if (out_uri) *out_uri = base_uri + relative_uri;
    return true;
}

static bool dummy_decode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) *out_uri = in_uri;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // 1. GetNumComponentsInType
    if (Size > 0) {
        uint32_t type = Data[0];
        tinygltf::GetNumComponentsInType(type);
    }

    // 2. GetComponentSizeInBytes
    if (Size > 1) {
        uint32_t componentType = Data[1];
        tinygltf::GetComponentSizeInBytes(componentType);
    }

    // 3. URIDecode
    if (Size > 2) {
        std::string in_uri(reinterpret_cast<const char*>(Data), Size % 256);
        std::string out_uri;
        tinygltf::URIDecode(in_uri, &out_uri, nullptr);
    }

    // 4. SetURICallbacks
    {
        tinygltf::TinyGLTF loader;
        tinygltf::URICallbacks callbacks;
        std::string err;
        
        // Test with null decode
        callbacks.encode = dummy_encode;
        callbacks.decode = nullptr;
        callbacks.user_data = nullptr;
        loader.SetURICallbacks(callbacks, &err);
        
        // Test with valid callbacks
        callbacks.decode = dummy_decode;
        loader.SetURICallbacks(callbacks, &err);
        
        // Test without error string
        loader.SetURICallbacks(callbacks, nullptr);
    }

    // 5. DecodeDataURI
    if (Size > 3) {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string in_string(reinterpret_cast<const char*>(Data), Size % 512);
        
        // Test with checkSize = false
        tinygltf::DecodeDataURI(&out, mime_type, in_string, 0, false);
        
        // Test with checkSize = true and various required bytes
        tinygltf::DecodeDataURI(&out, mime_type, in_string, Size % 128, true);
        tinygltf::DecodeDataURI(&out, mime_type, in_string, Size, true);
    }

    // 6. LoadImageData
    if (Size > 4) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        
        // Test with as_is = false (default)
        image.as_is = false;
        tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0, Data, Size, nullptr);
        
        // Test with as_is = true
        image.as_is = true;
        tinygltf::LoadImageData(&image, 1, &err, &warn, 100, 100, Data, Size % 1024, nullptr);
        
        // Test without error/warning strings
        tinygltf::LoadImageData(&image, 2, nullptr, nullptr, -1, -1, Data, Size, nullptr);
    }

    return 0;
}