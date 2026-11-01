// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::IsDataURI at tiny_gltf.h:244:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
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
#include <fstream>
#include <functional>

#include "tiny_gltf.h"

// Helper function to write data to a dummy file for filesystem callbacks
static bool WriteDummyFile(const std::string &filename, const std::string &contents) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;
    file.write(contents.c_str(), contents.size());
    file.close();
    return true;
}

// Helper function to read data from a dummy file
static bool ReadDummyFile(const std::string &filename, std::vector<unsigned char> *out, std::string *err) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "File not found";
        return false;
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    out->resize(size);
    file.read(reinterpret_cast<char*>(out->data()), size);
    file.close();
    return true;
}

// Dummy callbacks for FsCallbacks
static bool FileExistsFunc(const std::string &filename, void *user_data) {
    std::ifstream f(filename);
    return f.good();
}

static std::string ExpandFilePathFunc(const std::string &filepath, void *user_data) {
    return filepath;
}

static bool ReadWholeFileFunc(std::vector<unsigned char> *out, std::string *err, const std::string &filename, void *user_data) {
    return ReadDummyFile(filename, out, err);
}

static bool WriteWholeFileFunc(std::string *filename, const std::string &contents, const std::vector<unsigned char> &data, void *user_data) {
    if (!filename) return false;
    if (!data.empty()) {
        std::ofstream file(*filename, std::ios::binary);
        if (!file.is_open()) return false;
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();
        return true;
    }
    return WriteDummyFile(*filename, contents);
}

static bool GetFileSizeInBytesFunc(unsigned long *size, std::string *filename, const std::string &filepath, void *user_data) {
    if (!size || !filename) return false;
    std::ifstream f(filepath, std::ios::binary | std::ios::ate);
    if (!f.is_open()) return false;
    *size = static_cast<unsigned long>(f.tellg());
    *filename = filepath;
    return true;
}

// Dummy callbacks for URICallbacks
static bool URIDecodeCallback(const std::string &in_uri, std::string *out_uri, void *user_data) {
    return tinygltf::URIDecode(in_uri, out_uri, user_data);
}

static bool URIEncodeCallback(const std::string &in_uri, const std::string &filename, std::string *out_uri, void *user_data) {
    if (out_uri) *out_uri = in_uri;
    return true;
}

// Dummy LoadImageDataFunction
static bool LoadImageDataCallback(std::vector<unsigned char> *out, std::string *ext,
                                  const int image_idx, const std::string &uri,
                                  size_t *out_size, std::string *err,
                                  void *user_data) {
    std::vector<unsigned char> data;
    std::string e;
    if (ReadDummyFile(uri, &data, &e)) {
        *out = data;
        *out_size = data.size();
        size_t dot = uri.rfind('.');
        if (dot != std::string::npos) *ext = uri.substr(dot + 1);
        return true;
    }
    return false;
}

// Dummy WriteImageDataFunction wrapper
static bool WriteImageDataCallback(const std::string *basepath, const std::string *filename,
                                   const tinygltf::Image *image, bool embedImages,
                                   const tinygltf::FsCallbacks *fs_cb, const tinygltf::URICallbacks *uri_cb,
                                   std::string *out_uri, void *user_data) {
    return tinygltf::WriteImageData(basepath, filename, image, embedImages, fs_cb, uri_cb, out_uri, user_data);
}

// UpdateImageObject is a static function inside the detail namespace in tiny_gltf.h
// We cannot call it directly from outside without accessing the detail namespace.
// Since the task requires fuzzing the API, and UpdateImageObject is not part of the public API,
// we will skip calling it directly to avoid compilation errors.
// Instead, we rely on the other public functions to exercise the logic indirectly.

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::string input(reinterpret_cast<const char*>(Data), Size);

    // 1. Test tinygltf::IsDataURI
    {
        tinygltf::IsDataURI(input);
        tinygltf::IsDataURI("");
        std::string data_uri = "data:image/png;base64," + input;
        tinygltf::IsDataURI(data_uri);
    }

    // 2. Test tinygltf::URIDecode
    {
        std::string decoded;
        tinygltf::URIDecode(input, &decoded, nullptr);
        tinygltf::URIDecode("", &decoded, nullptr);
        std::string malformed = "%GG";
        tinygltf::URIDecode(malformed, &decoded, nullptr);
    }

    // 3. Test tinygltf::DecodeDataURI
    {
        std::vector<unsigned char> out;
        std::string mime_type;
        tinygltf::DecodeDataURI(&out, mime_type, input, 0, false);
        tinygltf::DecodeDataURI(&out, mime_type, input, 1024, true);
        std::string valid_data = "data:image/png;base64," + input;
        tinygltf::DecodeDataURI(&out, mime_type, valid_data, 0, false);
    }

    // 4. Test tinygltf::WriteImageData
    {
        tinygltf::Image image;
        image.image.assign(Size, 0);
        image.width = static_cast<int>(Size > 0 ? Size : 1);
        image.height = static_cast<int>(Size > 0 ? Size : 1);
        
        std::string basepath = "./";
        std::string filename = "fuzz_image.png";
        std::string out_uri;
        
        tinygltf::FsCallbacks fs_cb = {
            FileExistsFunc, 
            ExpandFilePathFunc, 
            ReadWholeFileFunc, 
            WriteWholeFileFunc, 
            GetFileSizeInBytesFunc, 
            nullptr
        };
        tinygltf::URICallbacks uri_cb = {
            URIEncodeCallback, 
            URIDecodeCallback, 
            nullptr
        };

        tinygltf::WriteImageData(&basepath, &filename, &image, true, &fs_cb, &uri_cb, &out_uri, nullptr);
        tinygltf::WriteImageData(&basepath, &filename, &image, false, &fs_cb, &uri_cb, &out_uri, nullptr);
        tinygltf::WriteImageData(&basepath, &filename, &image, true, &fs_cb, &uri_cb, nullptr, nullptr);
    }

    // 5. Skip tinygltf::UpdateImageObject as it is a static function in the detail namespace
    // and not part of the public API.

    return 0;
}