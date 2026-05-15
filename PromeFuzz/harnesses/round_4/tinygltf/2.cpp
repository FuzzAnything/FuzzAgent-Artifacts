// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.cc:1211:16 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::URIDecode at tiny_gltf.cc:842:6 in tiny_gltf.h
// tinygltf::DecodeDataURI at tiny_gltf.cc:1732:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
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
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

static void dummyFileWrite(const std::string& content) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(content.data(), content.size());
        file.close();
    }
}

static bool dummyFileExists(const std::string& filename, void* user_data) {
    return true;
}

static std::string dummyExpandPath(const std::string& path, void* user_data) {
    return path;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out,
                               std::string* err,
                               const std::string& filename,
                               void* user_data) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "Failed to open file";
        return false;
    }
    file.seekg(0, std::ios::end);
    std::streampos size = file.tellg();
    file.seekg(0, std::ios::beg);
    out->resize(static_cast<size_t>(size));
    if (size > 0) {
        file.read(reinterpret_cast<char*>(out->data()), size);
    }
    return true;
}

static bool dummyWriteWholeFile(std::string* err,
                                const std::string& filename,
                                const std::vector<unsigned char>& data,
                                void* user_data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "Failed to open file";
        return false;
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return true;
}

static bool dummyGetFileSize(size_t* size,
                             std::string* err,
                             const std::string& filename,
                             void* user_data) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        if (err) *err = "Failed to open file";
        return false;
    }
    std::streampos file_size = file.tellg();
    if (file_size >= 0) {
        *size = static_cast<size_t>(file_size);
        return true;
    }
    if (err) *err = "Failed to get file size";
    return false;
}

static bool dummyURIDecode(const std::string& in_uri,
                           std::string* out_uri,
                           void* user_data) {
    if (!out_uri) return false;
    *out_uri = in_uri;
    return true;
}

static bool dummyURIEncode(const std::string& in_uri,
                           const std::string& base_uri,
                           std::string* out_uri,
                           void* user_data) {
    if (!out_uri) return false;
    *out_uri = in_uri;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare dummy file with fuzzing data
    std::string dummy_content(reinterpret_cast<const char*>(Data), Size);
    dummyFileWrite(dummy_content);

    // Initialize structures
    tinygltf::TinyGLTF loader;
    tinygltf::URICallbacks uri_callbacks;
    tinygltf::FsCallbacks fs_callbacks;
    std::string err;
    std::string warn;

    // Use first byte to select different code paths
    uint8_t selector = Data[0];

    // 1. Test SetURICallbacks
    uri_callbacks.decode = dummyURIDecode;
    uri_callbacks.encode = dummyURIEncode;
    uri_callbacks.user_data = nullptr;
    loader.SetURICallbacks(uri_callbacks, &err);

    // 2. Test LoadImageData
    tinygltf::Image image;
    image.name = "fuzz_image";
    image.width = static_cast<int>(Size % 1024);
    image.as_is = (selector & 0x02) != 0;
    if (Size > 1) {
        const unsigned char* bytes = Data + 1;
        int byte_size = static_cast<int>(Size - 1);
        tinygltf::LoadImageData(&image, 0, &err, &warn, 0, 0, bytes, byte_size, nullptr);
    }

    // 3. Test URIDecode
    std::string in_uri = dummy_content.substr(0, std::min(Size, (size_t)100));
    std::string out_uri;
    tinygltf::URIDecode(in_uri, &out_uri, nullptr);

    // 4. Test DecodeDataURI
    std::vector<unsigned char> decoded;
    std::string mime_type;
    std::string data_uri = "data:application/octet-stream;base64,";
    if (Size > 10) {
        data_uri.append(dummy_content.substr(0, std::min(Size, (size_t)100)));
    }
    bool checkSize = (selector & 0x04) != 0;
    size_t reqBytes = (selector & 0x08) ? Size : 0;
    tinygltf::DecodeDataURI(&decoded, mime_type, data_uri, reqBytes, checkSize);

    // 5. Test WriteImageData
    fs_callbacks.FileExists = dummyFileExists;
    fs_callbacks.ExpandFilePath = dummyExpandPath;
    fs_callbacks.ReadWholeFile = dummyReadWholeFile;
    fs_callbacks.WriteWholeFile = dummyWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = dummyGetFileSize;
    fs_callbacks.user_data = nullptr;

    std::string out_uri_result;
    bool embedImages = (selector & 0x10) != 0;
    std::string basepath = ".";
    std::string filename = "./dummy_file";
    tinygltf::WriteImageData(&basepath, &filename, &image, embedImages,
                            &fs_callbacks, &uri_callbacks, &out_uri_result, nullptr);

    // 6. Test IsDataURI
    std::string test_uri = (selector & 0x20) ? 
                          "data:image/png;base64,abc123" : 
                          dummy_content.substr(0, std::min(Size, (size_t)50));
    tinygltf::IsDataURI(test_uri);

    return 0;
}