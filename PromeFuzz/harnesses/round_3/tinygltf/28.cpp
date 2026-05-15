// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::GetComponentSizeInBytes at tiny_gltf.h:199:23 in tiny_gltf.h
// tinygltf::GetNumComponentsInType at tiny_gltf.h:222:23 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
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
#include <string>
#include <vector>
#include <fstream>

static uint32_t get_uint32_t(const uint8_t* data, size_t size, size_t* pos) {
    if (*pos + 4 > size) {
        *pos = size;
        return 0;
    }
    uint32_t val = 0;
    for (int i = 0; i < 4; i++) {
        val = (val << 8) | data[*pos];
        (*pos)++;
    }
    return val;
}

static std::string get_string(const uint8_t* data, size_t size, size_t* pos, size_t max_len) {
    if (*pos >= size) return "";
    size_t remaining = size - *pos;
    size_t len = std::min(remaining, max_len);
    std::string str(reinterpret_cast<const char*>(data + *pos), len);
    *pos += len;
    return str;
}

static bool dummy_encode(const std::string& in, const std::string&, std::string* out, void* user_data) {
    *out = in;
    return true;
}

static bool dummy_decode(const std::string& in, std::string* out, void* user_data) {
    *out = in;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;

    size_t pos = 0;
    uint8_t selector = Data[pos] % 6;
    pos++;

    switch (selector) {
        case 0: {
            // Test GetNumComponentsInType
            uint32_t type = get_uint32_t(Data, Size, &pos);
            tinygltf::GetNumComponentsInType(type);
            break;
        }
        case 1: {
            // Test SetURICallbacks
            tinygltf::TinyGLTF loader;
            tinygltf::URICallbacks callbacks;
            std::string err;
            
            uint8_t callback_selector = (pos < Size) ? Data[pos] % 3 : 0;
            pos++;
            
            if (callback_selector == 0) {
                callbacks.encode = dummy_encode;
                callbacks.decode = dummy_decode;
                callbacks.user_data = nullptr;
            } else if (callback_selector == 1) {
                callbacks.encode = dummy_encode;
                callbacks.decode = nullptr;
                callbacks.user_data = nullptr;
            } else {
                callbacks.encode = nullptr;
                callbacks.decode = dummy_decode;
                callbacks.user_data = nullptr;
            }
            
            loader.SetURICallbacks(callbacks, &err);
            break;
        }
        case 2: {
            // Test DecodeDataURI
            std::vector<unsigned char> out;
            std::string mime_type;
            std::string in_str = get_string(Data, Size, &pos, 1024);
            
            size_t reqBytes = get_uint32_t(Data, Size, &pos);
            bool checkSize = (pos < Size) ? (Data[pos] % 2 == 0) : false;
            pos++;
            
            tinygltf::DecodeDataURI(&out, mime_type, in_str, reqBytes, checkSize);
            break;
        }
        case 3: {
            // Test LoadImageData
            tinygltf::Image image;
            std::string err, warn;
            int image_idx = static_cast<int>(get_uint32_t(Data, Size, &pos));
            
            int req_width = static_cast<int>(get_uint32_t(Data, Size, &pos));
            int req_height = static_cast<int>(get_uint32_t(Data, Size, &pos));
            
            size_t bytes_size = (Size - pos > 0) ? Size - pos : 0;
            const unsigned char* bytes = (bytes_size > 0) ? Data + pos : nullptr;
            
            image.as_is = (pos < Size) ? (Data[pos] % 2 == 0) : false;
            pos++;
            
            if (bytes && bytes_size > 0) {
                tinygltf::LoadImageData(&image, image_idx, &err, &warn, 
                                       req_width, req_height, bytes, 
                                       static_cast<int>(bytes_size), nullptr);
            }
            break;
        }
        case 4: {
            // Test URIDecode
            std::string in_uri = get_string(Data, Size, &pos, 1024);
            std::string out_uri;
            tinygltf::URIDecode(in_uri, &out_uri, nullptr);
            break;
        }
        case 5: {
            // Test GetComponentSizeInBytes
            uint32_t componentType = get_uint32_t(Data, Size, &pos);
            tinygltf::GetComponentSizeInBytes(componentType);
            break;
        }
    }

    return 0;
}