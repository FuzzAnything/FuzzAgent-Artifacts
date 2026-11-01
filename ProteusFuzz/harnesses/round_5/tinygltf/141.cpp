// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::URIDecode at tiny_gltf.h:1270:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.h:1483:8 in tiny_gltf.h
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
#include <functional>
#include <fstream>
#include "tiny_gltf.h"

// Helper to create a dummy file with fuzz data
static void WriteDummyFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

// Dummy callbacks for FsCallbacks matching tinygltf signatures
static bool FileExistsFunc(const std::string &abs_path, void *) {
    return true;
}

static std::string ExpandFilePathFunc(const std::string &filepath, void *) {
    return filepath;
}

static bool ReadWholeFileFunc(std::vector<unsigned char> *out, std::string *err, const std::string &filepath, void *) {
    if (filepath == "./dummy_file") {
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            if (err) *err = "File not found";
            return false;
        }
        size_t size = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);
        out->resize(size);
        file.read(reinterpret_cast<char*>(out->data()), size);
        return true;
    }
    return false;
}

static bool WriteWholeFileFunc(std::string *err, const std::string &filepath, const std::vector<unsigned char> &contents, void *) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        if (err) *err = "Cannot write file";
        return false;
    }
    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    return true;
}

static bool GetFileSizeInBytesFunc(size_t *size, std::string *err, const std::string &filepath, void *) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        if (err) *err = "File not found";
        return false;
    }
    *size = static_cast<size_t>(file.tellg());
    return true;
}

// Dummy callbacks for URICallbacks
static bool URIEncodeFunc(const std::string &in_uri, const std::string &dummy, std::string *out_uri, void *) {
    if (out_uri) {
        *out_uri = in_uri;
        return true;
    }
    return false;
}

static bool URIDecodeFunc(const std::string &in_uri, std::string *out_uri, void *) {
    if (out_uri) {
        *out_uri = in_uri; 
        return true;
    }
    return false;
}

// Dummy LoadImageDataFunction
static bool LoadImageDataFunc(std::string *out, std::string *ext,
                              int *width, int *height, int *component,
                              int *bit_depth, int *pixel_type,
                              size_t bytes_read, const unsigned char *bytes,
                              int image_index, const std::string &mime_type,
                              tinygltf::URICallbacks *uri_cb,
                              tinygltf::FsCallbacks *fs_cb,
                              void *user_data) {
    if (out) *out = "";
    if (ext) *ext = mime_type;
    if (width) *width = 1;
    if (height) *height = 1;
    if (component) *component = 3;
    if (bit_depth) *bit_depth = 8;
    if (pixel_type) *pixel_type = 0;
    return true;
}

// Dummy WriteImageDataFunction matching the expected signature with const FsCallbacks* and const URICallbacks*
static bool WriteImageDataFunc(const std::string *basepath, const std::string *filename,
                               const tinygltf::Image *image, bool embedImages,
                               const tinygltf::FsCallbacks *fs_cb,
                               const tinygltf::URICallbacks *uri_cb,
                               std::string *out_uri, void *user_data) {
    if (embedImages) {
        if (out_uri) *out_uri = "data:image/png;base64,dummy";
        return true;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize TinyGLTF instance
    tinygltf::TinyGLTF model;
    tinygltf::Image image;
    std::string err, warn;
    std::string baseDir = "./";
    std::string out_uri;

    // Setup callbacks
    tinygltf::FsCallbacks fs_cb;
    fs_cb.FileExists = FileExistsFunc;
    fs_cb.ExpandFilePath = ExpandFilePathFunc;
    fs_cb.ReadWholeFile = ReadWholeFileFunc;
    fs_cb.WriteWholeFile = WriteWholeFileFunc;
    fs_cb.GetFileSizeInBytes = GetFileSizeInBytesFunc;
    fs_cb.user_data = nullptr;

    tinygltf::URICallbacks uri_cb;
    uri_cb.encode = URIEncodeFunc;
    uri_cb.decode = URIDecodeFunc;
    uri_cb.user_data = nullptr;

    // Write dummy file for file-based operations
    WriteDummyFile(Data, Size);

    // 1. Fuzz URIDecode
    std::string uri_input(reinterpret_cast<const char*>(Data), Size);
    std::string uri_output;
    tinygltf::URIDecode(uri_input, &uri_output, nullptr);

    // 2. Fuzz SetImageWriter
    // Use the corrected function pointer type
    model.SetImageWriter(WriteImageDataFunc, nullptr);

    // 3. Fuzz UpdateImageObject
    // Note: UpdateImageObject is a static function in tinygltf namespace in some versions, 
    // but the error says it's not found. It might be a private detail function or removed.
    // We will skip calling it directly if it's not in the public API.
    // Instead, we rely on the model's internal logic or other public functions.
    // If it's truly not available, we skip this part.
    // For now, we assume it's not directly callable and skip.
    // If the library version has it, the compiler would have accepted it.
    // Since the error says "no member named 'UpdateImageObject' in namespace 'tinygltf'", we skip it.

    // 4. Fuzz WriteImageData
    const std::string *basepath_ptr = &baseDir;
    const std::string *filename_ptr = &image.name;
    const tinygltf::Image *image_ptr = &image;
    std::string out_uri_write;
    tinygltf::WriteImageData(basepath_ptr, filename_ptr, image_ptr, false, &fs_cb, &uri_cb, &out_uri_write, nullptr);
    tinygltf::WriteImageData(basepath_ptr, filename_ptr, image_ptr, true, &fs_cb, &uri_cb, &out_uri_write, nullptr);

    return 0;
}