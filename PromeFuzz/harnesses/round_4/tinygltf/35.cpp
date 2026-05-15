// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::GetComponentSizeInBytes at tiny_gltf.h:199:23 in tiny_gltf.h
// tinygltf::GetNumComponentsInType at tiny_gltf.h:222:23 in tiny_gltf.h
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
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

static bool dummyURIDecode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

static bool dummyURIEncode(const std::string &in_uri, const std::string &base_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Function 2: GetComponentSizeInBytes
    if (Size >= sizeof(uint32_t)) {
        uint32_t componentType;
        memcpy(&componentType, Data, sizeof(uint32_t));
        tinygltf::GetComponentSizeInBytes(componentType);
    }

    // Function 3: GetNumComponentsInType
    if (Size >= sizeof(uint32_t)) {
        uint32_t ty;
        memcpy(&ty, Data, sizeof(uint32_t));
        tinygltf::GetNumComponentsInType(ty);
    }

    // Function 5: URIDecode
    if (Size > 0) {
        std::string in_uri(reinterpret_cast<const char*>(Data), Size % 1024);
        std::string out_uri;
        tinygltf::URIDecode(in_uri, &out_uri, nullptr);
    }

    // Function 6: DecodeDataURI
    if (Size > 0) {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string in(reinterpret_cast<const char*>(Data), Size % 1024);
        bool checkSize = (Size % 2) == 0;
        size_t reqBytes = Size % 256;
        tinygltf::DecodeDataURI(&out, mime_type, in, reqBytes, checkSize);
    }

    // Function 1: SetURICallbacks
    tinygltf::TinyGLTF loader;
    tinygltf::URICallbacks callbacks;
    callbacks.encode = dummyURIEncode;
    callbacks.decode = dummyURIDecode;
    callbacks.user_data = nullptr;
    std::string err;
    loader.SetURICallbacks(callbacks, &err);

    // Function 4: LoadImageData
    if (Size > 0) {
        tinygltf::Image image;
        std::string warn;
        int req_width = Size % 1024;
        int req_height = Size % 1024;
        bool as_is = (Size % 3) == 0;
        image.as_is = as_is;
        
        tinygltf::LoadImageData(&image, 0, &err, &warn, req_width, req_height, 
                               Data, Size, nullptr);
    }

    return 0;
}