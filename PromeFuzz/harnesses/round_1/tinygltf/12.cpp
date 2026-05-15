// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::GetNumComponentsInType at tiny_gltf.h:222:23 in tiny_gltf.h
// tinygltf::GetComponentSizeInBytes at tiny_gltf.h:199:23 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
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

static bool dummyURIDecode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

static bool dummyURIEncode(const std::string &in_uri, const std::string &base_path, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
    }
    return true;
}

static bool dummyLoadImageData(tinygltf::Image *image, const int image_idx, std::string *err,
                               std::string *warn, int req_width, int req_height,
                               const unsigned char *bytes, int size, void *user_data) {
    if (image) {
        image->width = req_width;
        image->height = req_height;
        if (bytes && size > 0) {
            image->image.assign(bytes, bytes + size);
        }
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Use first 8 bytes for various integer parameters
    uint32_t type_val = 0;
    uint32_t component_type_val = 0;
    if (Size >= 8) {
        memcpy(&type_val, Data, 4);
        memcpy(&component_type_val, Data + 4, 4);
    } else if (Size >= 4) {
        memcpy(&type_val, Data, 4);
        component_type_val = type_val;
    } else if (Size > 0) {
        type_val = Data[0];
        component_type_val = Size > 1 ? Data[1] : Data[0];
    }

    // Test GetNumComponentsInType
    int32_t components = tinygltf::GetNumComponentsInType(type_val);

    // Test GetComponentSizeInBytes
    int32_t component_size = tinygltf::GetComponentSizeInBytes(component_type_val);

    // Test SetURICallbacks
    tinygltf::TinyGLTF loader;
    tinygltf::URICallbacks callbacks;
    callbacks.decode = dummyURIDecode;
    callbacks.encode = dummyURIEncode;
    callbacks.user_data = nullptr;
    
    std::string err_msg;
    bool set_result = loader.SetURICallbacks(callbacks, &err_msg);
    
    // Test with null decode (should fail)
    tinygltf::URICallbacks bad_callbacks;
    bad_callbacks.decode = nullptr;
    bad_callbacks.encode = dummyURIEncode;
    bad_callbacks.user_data = nullptr;
    bool bad_result = loader.SetURICallbacks(bad_callbacks, &err_msg);

    // Test DecodeDataURI
    std::vector<unsigned char> out_data;
    std::string mime_type;
    std::string input_str;
    
    if (Size > 0) {
        input_str.assign(reinterpret_cast<const char*>(Data), Size);
    }
    
    // Test with various combinations
    bool check_size = (Size % 2 == 0);
    size_t req_bytes = Size;
    
    bool decode_result = tinygltf::DecodeDataURI(&out_data, mime_type, input_str, req_bytes, check_size);
    
    // Test with zero required bytes
    bool decode_result2 = tinygltf::DecodeDataURI(&out_data, mime_type, input_str, 0, false);
    
    // Test with very large required bytes
    bool decode_result3 = tinygltf::DecodeDataURI(&out_data, mime_type, input_str, SIZE_MAX, true);

    // Test URIDecode
    std::string out_uri;
    bool uri_decode_result = tinygltf::URIDecode(input_str, &out_uri, nullptr);
    
    // Test with empty string
    std::string empty_input;
    std::string empty_out;
    bool empty_uri_result = tinygltf::URIDecode(empty_input, &empty_out, nullptr);

    // Test LoadImageData
    tinygltf::Image image;
    std::string load_err;
    std::string load_warn;
    
    int req_width = 0;
    int req_height = 0;
    if (Size >= 12) {
        memcpy(&req_width, Data, 4);
        memcpy(&req_height, Data + 4, 4);
    }
    
    // Test with various parameters
    bool load_result = tinygltf::LoadImageData(&image, 0, &load_err, &load_warn, 
                                               req_width, req_height, Data, Size, nullptr);
    
    // Test with as_is flag set
    image.as_is = true;
    bool load_result_as_is = tinygltf::LoadImageData(&image, 1, &load_err, &load_warn,
                                                     req_width, req_height, Data, Size, nullptr);
    
    // Test with null bytes
    bool load_result_null = tinygltf::LoadImageData(&image, 2, &load_err, &load_warn,
                                                    req_width, req_height, nullptr, 0, nullptr);

    // Test with negative size (should be handled gracefully)
    bool load_result_neg = tinygltf::LoadImageData(&image, 3, &load_err, &load_warn,
                                                   req_width, req_height, Data, -1, nullptr);

    // Test with dummy file if needed
    if (Size > 0) {
        std::ofstream file("./dummy_file", std::ios::binary);
        if (file) {
            file.write(reinterpret_cast<const char*>(Data), Size);
            file.close();
        }
    }

    return 0;
}