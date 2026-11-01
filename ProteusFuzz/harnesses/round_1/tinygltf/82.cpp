// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::DecodeDataURI at tiny_gltf.h:245:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageWriter at tiny_gltf.h:1483:8 in tiny_gltf.h
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
#include <algorithm>
#include "tiny_gltf.h"

// Dummy callbacks for FsCallbacks
// ReadWholeFileFunction signature: bool (std::vector<unsigned char> *buf, std::string *err, const std::string &filename, void *user_data)
static bool DummyReadWholeFile(std::vector<unsigned char> *buf, std::string *err,
                               const std::string &filename, void *user_data) {
    return false;
}

// WriteWholeFileFunction signature: bool (std::string *err, const std::string &filename, const std::vector<unsigned char> &contents, void *user_data)
static bool DummyWriteWholeFile(std::string *err, const std::string &filename,
                                const std::vector<unsigned char> &contents, void *user_data) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(contents.data(), 1, contents.size(), f);
        fclose(f);
        return true;
    }
    return false;
}

// FileExistsFunction signature: bool (const std::string &filename, void *user_data)
static bool DummyFileExists(const std::string &filename, void *user_data) { return false; }

// ExpandFilePathFunction signature: std::string (const std::string &filepath, void *user_data)
static std::string DummyExpandFilePath(const std::string &filepath, void *user_data) { return filepath; }

// GetFileSizeFunction signature: bool (size_t *size, std::string *err, const std::string &filename, void *user_data)
static bool DummyGetFileSizeInBytes(size_t *size, std::string *err, const std::string &filename, void *user_data) {
    if (size) *size = 0;
    return false;
}

// Dummy callbacks for URICallbacks
// URIEncodeFunction signature: bool (const std::string &uri, const std::string &base, std::string *out, void *user_data)
static bool DummyURIEncode(const std::string &uri, const std::string &base, std::string *out, void *user_data) {
    if (out) *out = uri;
    return true;
}

// URIDecodeFunction signature: bool (const std::string &uri, std::string *out, void *user_data)
static bool DummyURIDecode(const std::string &uri, std::string *out, void *user_data) {
    if (out) *out = uri;
    return true;
}

// Custom image writer for SetImageWriter
// WriteImageDataFunction signature: bool (const std::string *basepath, const std::string *filename, const Image *image, bool embedImages, const FsCallbacks *fs_cb, const URICallbacks *uri_cb, std::string *out_uri, void *user_data)
static bool CustomWriteImageData(const std::string *basepath, const std::string *filename,
                                 const tinygltf::Image *image, bool embedImages,
                                 const tinygltf::FsCallbacks *fs_cb, const tinygltf::URICallbacks *uri_cb,
                                 std::string *out_uri, void *user_data) {
    if (image && out_uri) {
        out_uri->clear();
        if (embedImages) {
            out_uri->assign("data:image/png;base64,");
        } else {
            out_uri->assign("dummy_image.png");
        }
    }
    return true;
}

// Custom image loader for LoadImageData
// LoadImageDataFunction signature: bool (Image *image, const int image_idx, std::string *err, std::string *warn, int req_width, int req_height, const unsigned char *bytes, int size, void *user_data)
static bool CustomLoadImageData(tinygltf::Image *image, const int image_idx, std::string *err,
                                std::string *warn, int req_width, int req_height,
                                const unsigned char *bytes, int size, void *user_data) {
    if (!image || !bytes || size <= 0) return false;
    image->image.assign(bytes, bytes + size);
    image->width = req_width > 0 ? req_width : 1;
    image->height = req_height > 0 ? req_height : 1;
    image->name = "fuzz_image";
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    tinygltf::Image image;
    std::string baseDir = "./";
    std::string out_uri;
    std::string err, warn;
    
    tinygltf::FsCallbacks fs_cb = {
        DummyFileExists,
        DummyExpandFilePath,
        DummyReadWholeFile,
        DummyWriteWholeFile,
        DummyGetFileSizeInBytes,
        nullptr
    };

    tinygltf::URICallbacks uri_cb = {
        DummyURIEncode,
        DummyURIDecode,
        nullptr
    };

    // Initialize image with fuzzed data if possible
    int img_size = static_cast<int>(Size);
    if (img_size > 0) {
        image.image.assign(Data, Data + img_size);
        
        // Safe access to Data[0] and Data[1]
        uint8_t w_val = (Size > 0) ? Data[0] : 0;
        uint8_t h_val = (Size > 1) ? Data[1] : 0;
        
        image.width = (w_val % 1024) + 1;
        image.height = (h_val % 1024) + 1;
        
        // Ensure we don't read past bounds and types match
        size_t name_len = std::min(Size, static_cast<size_t>(100));
        image.name = std::string(reinterpret_cast<const char*>(Data), name_len);
    }

    // 2. Fuzz LoadImageData
    std::vector<unsigned char> dummy_bytes(Size);
    std::memcpy(dummy_bytes.data(), Data, Size);
    tinygltf::Image img_load;
    
    uint8_t req_w = (Size > 0) ? Data[0] : 10;
    uint8_t req_h = (Size > 1) ? Data[1] : 10;
    
    CustomLoadImageData(&img_load, 0, &err, &warn, 
                        (req_w % 100) + 1, (req_h % 100) + 1,
                        dummy_bytes.data(), static_cast<int>(Size), nullptr);

    // 3. Fuzz DecodeDataURI
    std::vector<unsigned char> out_data;
    std::string mime_type;
    std::string uri_str(reinterpret_cast<const char*>(Data), Size);
    tinygltf::DecodeDataURI(&out_data, mime_type, uri_str, 0, false);

    // 4. Fuzz SetImageWriter and TinyGLTF instance
    tinygltf::TinyGLTF model;
    model.SetImageWriter(CustomWriteImageData, nullptr);

    // 5. Fuzz WriteImageData (default implementation)
    std::string basepath = "./";
    std::string filename = "test.png";
    
    uint8_t embed_flag = (Size > 3) ? Data[3] : 0;
    bool write_res = tinygltf::WriteImageData(
        &basepath, &filename, &image, (embed_flag % 2 == 0),
        &fs_cb, &uri_cb, &out_uri, nullptr
    );

    return 0;
}