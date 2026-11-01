// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <functional>
#include <fstream>
#include "tiny_gltf.h"

// Forward declarations to match tinygltf types
using FileExistsFunction = std::function<bool(const std::string&, void*)>;
using ExpandFilePathFunction = std::function<std::string(const std::string&, void*)>;
using ReadWholeFileFunction = std::function<bool(std::vector<unsigned char>*, std::string*, const std::string&, void*)>;
using WriteWholeFileFunction = std::function<bool(std::string*, const std::string&, const std::vector<unsigned char>&, void*)>;
// Corrected signature for GetFileSizeFunction based on error message
using GetFileSizeFunction = std::function<bool(size_t*, std::string*, const std::string&, void*)>;
// Corrected signature for URIEncodeFunction based on error message
using URIEncodeFunction = std::function<bool(const std::string&, const std::string&, std::string*, void*)>;
using URIDecodeFunction = std::function<bool(const std::string&, std::string*, void*)>;

// Dummy callbacks for FsCallbacks
static bool FileExistsImpl(const std::string &filename, void *user_data) {
    (void)filename;
    (void)user_data;
    return false;
}

static std::string ExpandFilePathImpl(const std::string &filepath, void *user_data) {
    (void)filepath;
    (void)user_data;
    return filepath;
}

static bool ReadWholeFileImpl(std::vector<unsigned char> *out, std::string *err,
                          const std::string &filename, void *user_data) {
    (void)out;
    (void)err;
    (void)filename;
    (void)user_data;
    return false;
}

static bool WriteWholeFileImpl(std::string *err, const std::string &filename,
                           const std::vector<unsigned char> &contents,
                           void *user_data) {
    if (!err) return false;
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) {
        *err = "Failed to open file for writing";
        return false;
    }
    ofs.write(reinterpret_cast<const char *>(contents.data()), contents.size());
    ofs.close();
    return true;
}

// Corrected signature for GetFileSizeInBytesImpl
static bool GetFileSizeInBytesImpl(size_t *size, std::string *err, const std::string &filename,
                               void *user_data) {
    (void)size;
    (void)err;
    (void)filename;
    (void)user_data;
    return false;
}

// Dummy callbacks for URICallbacks
static bool URIDecodeImpl(const std::string &in_uri, std::string *out_uri,
                          void *user_data) {
    return tinygltf::URIDecode(in_uri, out_uri, user_data);
}

// Corrected signature for URIEncodeImpl
static bool URIEncodeImpl(const std::string &in_uri, const std::string &base_path, std::string *out_uri,
                          void *user_data) {
    (void)in_uri;
    (void)base_path;
    (void)out_uri;
    (void)user_data;
    return true;
}

// Dummy LoadImageDataFunction
static bool LoadImageDataImpl(std::vector<unsigned char> *out, std::string *ext,
                              std::string *mime_type, int width, int height,
                              const std::string &uri, int image_index,
                              void *user_data) {
    (void)out;
    (void)ext;
    (void)mime_type;
    (void)width;
    (void)height;
    (void)uri;
    (void)image_index;
    (void)user_data;
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Parse input to determine which function to test
    uint8_t mode = Data[0] % 5; 
    std::string input_data(reinterpret_cast<const char*>(Data + 1), Size - 1);

    tinygltf::FsCallbacks fs_cb = {
        FileExistsImpl,
        ExpandFilePathImpl,
        ReadWholeFileImpl,
        WriteWholeFileImpl,
        GetFileSizeInBytesImpl,
        nullptr
    };

    tinygltf::URICallbacks uri_cb = {
        URIEncodeImpl,
        URIDecodeImpl,
        nullptr
    };

    std::string err, warn;
    std::string out_uri;
    std::string mime_type;
    std::vector<unsigned char> out_data;

    switch (mode) {
        case 0: {
            // Test URIDecode
            std::string decoded;
            tinygltf::URIDecode(input_data, &decoded, nullptr);
            break;
        }
        case 1: {
            // Test IsDataURI
            bool is_data = tinygltf::IsDataURI(input_data);
            (void)is_data;
            break;
        }
        case 2: {
            // Test DecodeDataURI
            tinygltf::DecodeDataURI(&out_data, mime_type, input_data, 0, false);
            break;
        }
        case 3: {
            // Test WriteImageData
            tinygltf::Image image;
            image.image = std::vector<unsigned char>(input_data.begin(), input_data.end());
            image.name = "test_image";
            std::string basepath = "./dummy_file";
            std::string filename = "test.png";
            tinygltf::WriteImageData(&basepath, &filename, &image, false,
                                     &fs_cb, &uri_cb, &out_uri, nullptr);
            break;
        }
        case 4: {
            // Test internal logic via URIDecode and IsDataURI combination
            std::string decoded_uri;
            tinygltf::URIDecode(input_data, &decoded_uri, nullptr);
            bool is_data_uri = tinygltf::IsDataURI(decoded_uri);
            (void)is_data_uri;
            break;
        }
        default:
            break;
    }

    return 0;
}