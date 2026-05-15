// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::GetNumComponentsInType at tiny_gltf.h:222:23 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::GetComponentSizeInBytes at tiny_gltf.h:199:23 in tiny_gltf.h
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
#include <fstream>

static bool dummyURIDecode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) *out_uri = in_uri;
    return true;
}

static bool dummyURIEncode(const std::string &in_uri, const std::string &, std::string *out_uri, void *user_data) {
    if (out_uri) *out_uri = in_uri;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Function 1: GetNumComponentsInType
    if (Size > 0) {
        uint32_t type = Data[0];
        tinygltf::GetNumComponentsInType(type);
    }

    // Function 2: SetURICallbacks
    static tinygltf::TinyGLTF loader;
    tinygltf::URICallbacks callbacks;
    callbacks.encode = dummyURIEncode;
    callbacks.decode = dummyURIDecode;
    callbacks.user_data = nullptr;
    std::string err;
    loader.SetURICallbacks(callbacks, &err);

    // Function 3: DecodeDataURI
    if (Size > 0) {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string in_str(reinterpret_cast<const char*>(Data), Size % 256);
        bool checkSize = (Data[0] % 2) == 0;
        size_t reqBytes = Size;
        tinygltf::DecodeDataURI(&out, mime_type, in_str, reqBytes, checkSize);
    }

    // Function 4: LoadImageData
    if (Size > 0) {
        tinygltf::Image image;
        std::string err_str, warn_str;
        int req_width = (Data[0] % 64) + 1;
        int req_height = (Size > 1 ? (Data[Size-1] % 64) + 1 : 1);
        tinygltf::LoadImageData(&image, 0, &err_str, &warn_str, req_width, req_height, Data, Size, nullptr);
    }

    // Function 5: URIDecode
    if (Size > 0) {
        std::string out_uri;
        std::string in_uri(reinterpret_cast<const char*>(Data), Size % 128);
        tinygltf::URIDecode(in_uri, &out_uri, nullptr);
    }

    // Function 6: GetComponentSizeInBytes
    if (Size > 0) {
        uint32_t componentType = Size > 1 ? Data[Size-1] : Data[0];
        tinygltf::GetComponentSizeInBytes(componentType);
    }

    return 0;
}