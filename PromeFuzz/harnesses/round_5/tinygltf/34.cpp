// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::GetNumComponentsInType at tiny_gltf.h:222:23 in tiny_gltf.h
// tinygltf::GetComponentSizeInBytes at tiny_gltf.h:199:23 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
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

static bool custom_uri_decode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

static bool custom_uri_encode(const std::string &in_uri1, const std::string &in_uri2, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri1;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // Test GetNumComponentsInType
    if (Size >= sizeof(uint32_t)) {
        uint32_t type;
        memcpy(&type, Data, sizeof(uint32_t));
        tinygltf::GetNumComponentsInType(type);
    }

    // Test GetComponentSizeInBytes
    if (Size >= sizeof(uint32_t)) {
        uint32_t component_type;
        memcpy(&component_type, Data, sizeof(uint32_t));
        tinygltf::GetComponentSizeInBytes(component_type);
    }

    // Test DecodeDataURI
    if (Size > 0) {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string in_str(reinterpret_cast<const char*>(Data), Size);
        size_t req_bytes = Size % 1024;
        bool check_size = (Size % 2) == 0;
        tinygltf::DecodeDataURI(&out, mime_type, in_str, req_bytes, check_size);
    }

    // Test URIDecode
    if (Size > 0) {
        std::string in_uri(reinterpret_cast<const char*>(Data), Size);
        std::string out_uri;
        tinygltf::URIDecode(in_uri, &out_uri, nullptr);
    }

    // Test SetURICallbacks
    {
        tinygltf::TinyGLTF loader;
        tinygltf::URICallbacks callbacks;
        callbacks.encode = custom_uri_encode;
        callbacks.decode = custom_uri_decode;
        callbacks.user_data = nullptr;
        std::string err;
        loader.SetURICallbacks(callbacks, &err);
    }

    // Test LoadImageData
    if (Size > 0) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        int req_width = static_cast<int>(Size % 1024);
        int req_height = static_cast<int>((Size / 1024) % 1024);
        bool as_is = (Size % 3) == 0;
        image.as_is = as_is;
        
        tinygltf::LoadImageData(&image, 0, &err, &warn, req_width, req_height, Data, static_cast<int>(Size), nullptr);
    }

    return 0;
}