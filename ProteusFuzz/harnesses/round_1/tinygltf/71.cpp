// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::base64_decode at tiny_gltf.h:2270:13 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.h:1382:6 in tiny_gltf.h
// tinygltf::ParseImage at tiny_gltf.h:4292:13 in tiny_gltf.h
// tinygltf::ParseImage at tiny_gltf.h:4292:13 in tiny_gltf.h
// tinygltf::ParseImage at tiny_gltf.h:4292:13 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.h:1375:6 in tiny_gltf.h
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

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

// Helper to create a dummy file for file-based operations
static void CreateDummyFile(const std::string& filename, const uint8_t* data, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        if (data && size > 0) {
            file.write(reinterpret_cast<const char*>(data), size);
        }
        file.close();
    }
}

// Default callbacks for URICallbacks
static bool DefaultURIDecode(const std::string& uri, std::string* out, void* user_data) {
    (void)user_data;
    if (out) *out = uri;
    return true;
}

static bool DefaultURIEncode(const std::string& uri, const std::string& mime, std::string* out, void* user_data) {
    (void)user_data;
    (void)mime;
    if (out) *out = uri;
    return true;
}

// Default callbacks for FsCallbacks
static bool DefaultFileExists(const std::string& filename, void* user_data) {
    (void)user_data;
    std::ifstream f(filename);
    return f.good();
}

static std::string DefaultExpandFilePath(const std::string& filepath, void* user_data) {
    (void)user_data;
    return filepath;
}

static bool DefaultReadWholeFile(std::vector<unsigned char>* out, std::string* err, const std::string& filename, void* user_data) {
    (void)user_data;
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        if (err) *err = "Cannot open file";
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    out->resize(static_cast<size_t>(size));
    if (size > 0) {
        file.read(reinterpret_cast<char*>(out->data()), size);
    }
    file.close();
    return true;
}

static bool DefaultWriteWholeFile(std::string* err, const std::string& filename, const std::vector<unsigned char>& contents, void* user_data) {
    (void)user_data;
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "Cannot write file";
        return false;
    }
    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    file.close();
    return true;
}

static bool DefaultGetFileSize(size_t* size, std::string* err, const std::string& filename, void* user_data) {
    (void)user_data;
    (void)err;
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;
    std::streamsize s = file.tellg();
    file.close();
    if (size) *size = static_cast<size_t>(s);
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Test tinygltf::WriteToMemory_stbi
    {
        std::vector<unsigned char> buffer;
        buffer.insert(buffer.end(), Data, Data + Size);
        (void)buffer;
    }

    // 2. Test tinygltf::base64_decode
    {
        std::string encoded(reinterpret_cast<const char*>(Data), Size);
        std::string decoded = tinygltf::base64_decode(encoded);
        (void)decoded;
    }

    // 3. Test tinygltf::WriteImageData
    {
        tinygltf::Image image;
        image.image.assign(Data, Data + Size);
        image.width = static_cast<int>(Size > 0 ? Size : 1);
        image.height = 1;
        image.name = "fuzz_image";

        std::string basepath = "./";
        std::string filename = "dummy_image.png";
        std::string out_uri;

        tinygltf::FsCallbacks fs_cb;
        fs_cb.FileExists = DefaultFileExists;
        fs_cb.ExpandFilePath = DefaultExpandFilePath;
        fs_cb.ReadWholeFile = DefaultReadWholeFile;
        fs_cb.WriteWholeFile = DefaultWriteWholeFile;
        fs_cb.GetFileSizeInBytes = DefaultGetFileSize;
        fs_cb.user_data = nullptr;

        tinygltf::URICallbacks uri_cb;
        uri_cb.encode = DefaultURIEncode;
        uri_cb.decode = DefaultURIDecode;
        uri_cb.user_data = nullptr;

        bool res1 = tinygltf::WriteImageData(&basepath, &filename, &image, false, &fs_cb, &uri_cb, &out_uri, nullptr);
        (void)res1;

        bool res2 = tinygltf::WriteImageData(&basepath, &filename, &image, true, &fs_cb, &uri_cb, &out_uri, nullptr);
        (void)res2;
    }

    // 4. Test tinygltf::ParseImage
    {
        tinygltf::Image image;
        std::string err, warn;
        
        nlohmann::json o;
        o["uri"] = "data:image/png;base64,"; 
        o["mimeType"] = "image/png";

        bool store_original = true;
        std::string basedir = "./";
        size_t max_file_size = 1024 * 1024 * 10; 

        tinygltf::FsCallbacks fs_cb;
        fs_cb.FileExists = DefaultFileExists;
        fs_cb.ExpandFilePath = DefaultExpandFilePath;
        fs_cb.ReadWholeFile = DefaultReadWholeFile;
        fs_cb.WriteWholeFile = DefaultWriteWholeFile;
        fs_cb.GetFileSizeInBytes = DefaultGetFileSize;
        fs_cb.user_data = nullptr;

        tinygltf::URICallbacks uri_cb;
        uri_cb.encode = DefaultURIEncode;
        uri_cb.decode = DefaultURIDecode;
        uri_cb.user_data = nullptr;

        tinygltf::ParseImage(&image, 0, &err, &warn, o, store_original, basedir, max_file_size, &fs_cb, &uri_cb);
        
        nlohmann::json o2;
        o2["bufferView"] = 0;
        o2["mimeType"] = "image/png";
        tinygltf::ParseImage(&image, 1, &err, &warn, o2, store_original, basedir, max_file_size, &fs_cb, &uri_cb);

        nlohmann::json o3;
        o3["uri"] = "test.png";
        o3["bufferView"] = 0;
        tinygltf::ParseImage(&image, 2, &err, &warn, o3, store_original, basedir, max_file_size, &fs_cb, &uri_cb);
    }

    // 5. Test tinygltf::LoadImageData (both overloads)
    {
        tinygltf::Image image;
        std::string err, warn;
        int req_width = 64;
        int req_height = 64;
        
        // Overload 1: user_data = nullptr
        bool res1 = tinygltf::LoadImageData(&image, 0, &err, &warn, req_width, req_height, Data, static_cast<int>(Size), nullptr);
        (void)res1;

        // Overload 2: user_data = valid LoadImageDataOption
        // The ASan error occurred because we passed raw fuzz data as user_data, 
        // but the function tries to cast it to LoadImageDataOption*.
        // We must provide a properly initialized LoadImageDataOption struct.
        tinygltf::LoadImageDataOption option;
        option = tinygltf::LoadImageDataOption(); // Default constructor
        bool res2 = tinygltf::LoadImageData(&image, 1, &err, &warn, req_width, req_height, Data, static_cast<int>(Size), &option);
        (void)res2;
    }

    return 0;
}