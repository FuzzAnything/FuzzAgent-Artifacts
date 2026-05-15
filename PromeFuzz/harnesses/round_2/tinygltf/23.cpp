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
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

#include "code/tiny_gltf.h"

static void WriteDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare dummy file for any potential file operations
    WriteDummyFile(Data, Size);

    // 1. Test URIDecode
    {
        std::string in_uri(reinterpret_cast<const char*>(Data), Size);
        std::string out_uri;
        bool result = tinygltf::URIDecode(in_uri, &out_uri, nullptr);
        (void)result; // Ignore return value
    }

    // 2. Test GetComponentSizeInBytes
    {
        // Use first 4 bytes as component type, or entire input if smaller
        uint32_t componentType = 0;
        size_t bytesToCopy = std::min(Size, sizeof(uint32_t));
        std::memcpy(&componentType, Data, bytesToCopy);
        int32_t size = tinygltf::GetComponentSizeInBytes(componentType);
        (void)size; // Ignore return value
    }

    // 3. Test DecodeDataURI
    {
        std::vector<unsigned char> out;
        std::string mime_type;
        std::string in(reinterpret_cast<const char*>(Data), Size);
        
        // Test with different combinations of parameters
        bool result1 = tinygltf::DecodeDataURI(&out, mime_type, in, 0, false);
        bool result2 = tinygltf::DecodeDataURI(&out, mime_type, in, Size, true);
        bool result3 = tinygltf::DecodeDataURI(&out, mime_type, in, Size/2, false);
        (void)result1; (void)result2; (void)result3;
    }

    // 4. Test LoadImageData
    {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        
        // Test with as_is = false (default)
        bool result1 = tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0, Data, Size, nullptr);
        
        // Test with as_is = true
        image.as_is = true;
        bool result2 = tinygltf::LoadImageData(&image, 1, &err, &warn, 100, 100, Data, Size, nullptr);
        
        // Test with negative dimensions
        bool result3 = tinygltf::LoadImageData(&image, 2, &err, &warn, -1, -1, Data, Size, nullptr);
        (void)result1; (void)result2; (void)result3;
    }

    // 5. Test GetNumComponentsInType
    {
        // Use first 4 bytes as type, or entire input if smaller
        uint32_t type = 0;
        size_t bytesToCopy = std::min(Size, sizeof(uint32_t));
        std::memcpy(&type, Data, bytesToCopy);
        int32_t count = tinygltf::GetNumComponentsInType(type);
        (void)count; // Ignore return value
    }

    // 6. Test IsDataURI
    {
        std::string in(reinterpret_cast<const char*>(Data), Size);
        bool is_data_uri = tinygltf::IsDataURI(in);
        (void)is_data_uri; // Ignore return value
    }

    // Additional test: Test Value constructor with binary data
    {
        tinygltf::Value value(Data, Size);
    }

    return 0;
}