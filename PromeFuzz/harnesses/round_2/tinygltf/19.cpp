// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::GetComponentSizeInBytes at tiny_gltf.h:199:23 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::GetNumComponentsInType at tiny_gltf.h:222:23 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

#include "tiny_gltf.h"

static void fuzz_URIDecode(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    std::string in_uri(reinterpret_cast<const char*>(data), size);
    std::string out_uri;
    bool result = tinygltf::URIDecode(in_uri, &out_uri, nullptr);
    (void)result;
}

static void fuzz_GetComponentSizeInBytes(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    uint32_t componentType;
    memcpy(&componentType, data, sizeof(uint32_t));
    int32_t result = tinygltf::GetComponentSizeInBytes(componentType);
    (void)result;
}

static void fuzz_DecodeDataURI(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    std::vector<unsigned char> out;
    std::string mime_type;
    std::string in(reinterpret_cast<const char*>(data), size);
    
    // Try with different combinations of parameters
    bool result1 = tinygltf::DecodeDataURI(&out, mime_type, in, 0, false);
    bool result2 = tinygltf::DecodeDataURI(&out, mime_type, in, 100, true);
    bool result3 = tinygltf::DecodeDataURI(&out, mime_type, in, size, false);
    
    (void)result1;
    (void)result2;
    (void)result3;
}

static void fuzz_LoadImageData(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    tinygltf::Image image;
    std::string err;
    std::string warn;
    int image_idx = 0;
    
    // Test with different parameter combinations
    bool result1 = tinygltf::LoadImageData(&image, image_idx, &err, &warn, 
                                           -1, -1, data, size, nullptr);
    
    bool result2 = tinygltf::LoadImageData(&image, image_idx, &err, &warn,
                                           100, 100, data, size, nullptr);
    
    bool result3 = tinygltf::LoadImageData(&image, image_idx, nullptr, nullptr,
                                           -1, -1, data, size, nullptr);
    
    (void)result1;
    (void)result2;
    (void)result3;
}

static void fuzz_GetNumComponentsInType(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    uint32_t ty;
    memcpy(&ty, data, sizeof(uint32_t));
    int32_t result = tinygltf::GetNumComponentsInType(ty);
    (void)result;
}

static void fuzz_IsDataURI(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    std::string in(reinterpret_cast<const char*>(data), size);
    bool result = tinygltf::IsDataURI(in);
    (void)result;
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *data, size_t size) {
    // Fuzz each API function with the input data
    fuzz_URIDecode(data, size);
    fuzz_GetComponentSizeInBytes(data, size);
    fuzz_DecodeDataURI(data, size);
    fuzz_LoadImageData(data, size);
    fuzz_GetNumComponentsInType(data, size);
    fuzz_IsDataURI(data, size);
    
    return 0;
}