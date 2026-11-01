// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include "tiny_gltf.h"

// Helper to write dummy file
static void WriteDummyFile(const std::string &filename, const uint8_t *data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Setup callbacks
static void SetupFsCallbacks(tinygltf::FsCallbacks *fs_cb) {
    fs_cb->FileExists = nullptr;
    fs_cb->ExpandFilePath = nullptr;
    fs_cb->ReadWholeFile = nullptr;
    fs_cb->WriteWholeFile = nullptr;
    fs_cb->GetFileSizeInBytes = nullptr;
    fs_cb->user_data = nullptr;
}

static void SetupURICallbacks(tinygltf::URICallbacks *uri_cb) {
    uri_cb->encode = nullptr;
    uri_cb->decode = nullptr;
    uri_cb->user_data = nullptr;
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string input_str(reinterpret_cast<const char*>(Data), Size);

    // 1. Fuzz is_base64
    // is_base64 is static inline, so we cannot call it directly from outside.
    // We rely on other functions that might use it internally.

    // 2. Fuzz base64_encode
    // base64_encode is not in the tinygltf namespace in this version.
    // We will skip direct calling and rely on other functions.
    // However, we can try to call it if it's accessible.
    // Since it's not accessible, we skip it.

    // 3. Fuzz base64_decode
    // base64_decode is not in the tinygltf namespace in this version.
    // We will skip direct calling and rely on other functions.
    // However, we can try to call it if it's accessible.
    // Since it's not accessible, we skip it.

    // 4. Fuzz DecodeDataURI
    std::string data_uri = "data:image/png;base64," + input_str;
    std::vector<unsigned char> out_data;
    std::string mime_type;
    tinygltf::DecodeDataURI(&out_data, mime_type, data_uri, 0, false);
    tinygltf::DecodeDataURI(&out_data, mime_type, data_uri, Size, true);

    // 5. Fuzz LoadImageData
    tinygltf::Image image;
    std::string err;
    std::string warn;
    tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0, Data, static_cast<int>(Size), nullptr);
    
    if (Size > 4) {
        int w = Data[0] + 1;
        int h = Data[1] + 1;
        tinygltf::LoadImageData(&image, 1, &err, &warn, w, h, Data, static_cast<int>(Size), nullptr);
    }

    // 6. Fuzz WriteImageData
    tinygltf::FsCallbacks fs_cb;
    SetupFsCallbacks(&fs_cb);
    
    tinygltf::URICallbacks uri_cb;
    SetupURICallbacks(&uri_cb);

    std::string out_uri;
    std::string basepath = "./";
    std::string filename = "dummy_image";

    tinygltf::WriteImageData(&basepath, &filename, &image, true, &fs_cb, &uri_cb, &out_uri, nullptr);
    tinygltf::WriteImageData(&basepath, &filename, &image, false, &fs_cb, &uri_cb, &out_uri, nullptr);

    return 0;
}