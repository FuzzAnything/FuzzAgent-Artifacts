// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.cc:1693:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
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
#include <vector>
#include <string>
#include <fstream>

static bool customURIDecode(const std::string &in_uri, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
        return true;
    }
    return false;
}

static bool customURIEncode(const std::string &in_uri, const std::string &base_path, std::string *out_uri, void *user_data) {
    if (out_uri) {
        *out_uri = in_uri;
        return true;
    }
    return false;
}

static bool customFileExists(const std::string &filename, void *user_data) {
    return true;
}

static std::string customExpandFilePath(const std::string &path, void *user_data) {
    return path;
}

static bool customReadWholeFile(std::vector<unsigned char> *out, std::string *err,
                                const std::string &filename, void *user_data) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        if (err) *err = "Cannot open file";
        return false;
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    out->resize(size);
    file.read(reinterpret_cast<char*>(out->data()), size);
    return true;
}

static bool customWriteWholeFile(std::string *err, const std::string &filename,
                                 const std::vector<unsigned char> &contents, void *user_data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        if (err) *err = "Cannot open file";
        return false;
    }
    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    return true;
}

static bool customGetFileSizeInBytes(size_t *size, std::string *err,
                                     const std::string &filename, void *user_data) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        if (err) *err = "Cannot open file";
        return false;
    }
    *size = file.tellg();
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Limit input size to avoid excessive memory usage
    if (Size > 1024 * 1024) {
        return 0;
    }

    // Convert input to string for various functions
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    // 1. Test IsDataURI
    bool is_data_uri = tinygltf::IsDataURI(input_str);
    
    // 2. Test DecodeDataURI
    std::vector<unsigned char> out_data;
    std::string mime_type;
    size_t reqBytes = Size % 100;  // Use modulo to get a reasonable required bytes
    bool checkSize = (Size % 2 == 0);
    bool decode_result = tinygltf::DecodeDataURI(&out_data, mime_type, input_str, reqBytes, checkSize);
    
    // 3. Test URIDecode
    std::string decoded_uri;
    bool uri_decode_result = tinygltf::URIDecode(input_str, &decoded_uri, nullptr);
    
    // 4. Test LoadImageData
    tinygltf::Image image;
    std::string err;
    std::string warn;
    int req_width = static_cast<int>(Size % 256);
    int req_height = static_cast<int>(Size % 256);
    bool as_is = (Size % 3 == 0);
    image.as_is = as_is;
    
    // Use the raw data as image bytes
    const unsigned char* image_bytes = Data;
    int image_size = static_cast<int>(Size);
    
    bool load_image_result = tinygltf::LoadImageData(&image, 0, &err, &warn, 
                                                     req_width, req_height, 
                                                     image_bytes, image_size, nullptr);
    
    // 5. Test SetURICallbacks
    tinygltf::TinyGLTF loader;
    tinygltf::URICallbacks uri_callbacks;
    
    uri_callbacks.encode = customURIEncode;
    uri_callbacks.decode = customURIDecode;
    uri_callbacks.user_data = nullptr;
    
    std::string seturi_err;
    bool seturi_result = loader.SetURICallbacks(uri_callbacks, &seturi_err);
    
    // 6. Test WriteImageData
    // Create dummy file for writing
    const std::string dummy_file = "./dummy_file";
    
    // Create a simple filesystem callback structure
    tinygltf::FsCallbacks fs_callbacks;
    
    fs_callbacks.FileExists = customFileExists;
    fs_callbacks.ExpandFilePath = customExpandFilePath;
    fs_callbacks.ReadWholeFile = customReadWholeFile;
    fs_callbacks.WriteWholeFile = customWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = customGetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;
    
    // Prepare image for writing
    if (image.image.empty() && Size > 0) {
        // If image is empty, add some dummy data
        image.image.resize(std::min(static_cast<size_t>(100), Size));
        for (size_t i = 0; i < image.image.size(); i++) {
            image.image[i] = Data[i % Size];
        }
        image.width = 10;
    }
    
    std::string out_uri;
    bool embedImages = (Size % 5 == 0);
    bool write_image_result = tinygltf::WriteImageData(nullptr, &dummy_file, &image, 
                                                       embedImages, &fs_callbacks, 
                                                       &uri_callbacks, &out_uri, nullptr);
    
    // Clean up dummy file
    std::remove(dummy_file.c_str());
    
    return 0;
}