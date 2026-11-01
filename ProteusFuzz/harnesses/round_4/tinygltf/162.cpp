// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::LoadASCIIFromString at tiny_gltf.h:1423:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetFsCallbacks at tiny_gltf.h:1497:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadBinaryFromFile at tiny_gltf.h:1433:8 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetURICallbacks at tiny_gltf.h:1490:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImageLoader at tiny_gltf.h:1473:8 in tiny_gltf.h
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

// Dummy Filesystem Callbacks
// Note: tinygltf expects ReadWholeFile to have signature:
// bool ReadWholeFile(std::string *out_err, std::vector<unsigned char> *out_buf, const std::string &filename, void *user_data)
// But the error suggests the expected signature might be slightly different or the function pointer type is strict.
// Based on the error message: function<bool (vector<unsigned char> *, basic_string<char> *, const basic_string<char> &, void *)>
// This implies the expected signature is: bool ReadWholeFile(std::vector<unsigned char> *out_buf, std::string *out_err, const std::string &filename, void *user_data)
// Let's adjust the dummy function to match the expected signature from the error.
static bool DummyReadWholeFile(std::vector<unsigned char> *out_buf, std::string *out_err,
                               const std::string &filename, void *) {
    (void)out_buf;
    (void)out_err;
    (void)filename;
    return false;
}

// Note: tinygltf expects GetFileSizeInBytes to have signature:
// int GetFileSizeInBytes(const std::string &filename, void *user_data)
// But the error suggests: function<bool (unsigned long *, basic_string<char> *, const basic_string<char> &, void *)>
// This implies the expected signature is: bool GetFileSizeInBytes(unsigned long *out_size, std::string *out_err, const std::string &filename, void *user_data)
// Let's adjust the dummy function to match the expected signature from the error.
static bool DummyGetFileSizeInBytes(unsigned long *out_size, std::string *out_err,
                                    const std::string &filename, void *) {
    (void)out_size;
    (void)out_err;
    (void)filename;
    return false;
}

// Dummy URI Callbacks
// Error suggests: function<bool (const basic_string<char> &, const basic_string<char> &, basic_string<char> *, void *)> for encode
// Expected: bool URIEncode(const std::string &uri_to_encode, const std::string &base_dir, std::string *out_uri, void *user_data)
static bool DummyURIEncode(const std::string &uri_to_encode, const std::string &base_dir, std::string *out_uri, void *) {
    (void)uri_to_encode;
    (void)base_dir;
    (void)out_uri;
    return false;
}

// Error suggests: function<bool (const basic_string<char> &, basic_string<char> *, void *)> for decode
// Expected: bool URIDecode(const std::string &uri_to_decode, std::string *out_uri, void *user_data)
static bool DummyURIDecode(const std::string &uri_to_decode, std::string *out_uri, void *) {
    (void)uri_to_decode;
    (void)out_uri;
    return false;
}

// Dummy Image Loader
// Error suggests: function<bool (Image *, const int, basic_string<char> *, basic_string<char> *, int, int, const unsigned char *, int, void *)>
// Expected: bool LoadImageData(tinygltf::Image *image, int index, std::string *uri, std::string *base_dir, int width, int height, const unsigned char *bytes, int size, void *user_data)
static bool DummyLoadImageData(tinygltf::Image *image, int index, std::string *uri, std::string *base_dir,
                               int width, int height, const unsigned char *bytes, int size, void *user_data) {
    (void)image;
    (void)index;
    (void)uri;
    (void)base_dir;
    (void)width;
    (void)height;
    (void)bytes;
    (void)size;
    (void)user_data;
    return false;
}

// Dummy FileExists
static bool DummyFileExists(const std::string &filename, void *) {
    (void)filename;
    return false;
}

// Dummy ExpandFilePath
static std::string DummyExpandFilePath(const std::string &filepath, void *) {
    (void)filepath;
    return filepath;
}

// Dummy WriteWholeFile
static bool DummyWriteWholeFile(std::string *out_err, const std::string &filename,
                                const std::vector<unsigned char> &contents, void *) {
    (void)out_err;
    (void)filename;
    (void)contents;
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // Prepare dummy file for file-based loaders
    const char *dummy_filename = "./dummy_file";
    {
        std::ofstream dummy_file(dummy_filename, std::ios::binary);
        if (dummy_file.is_open()) {
            dummy_file.write(reinterpret_cast<const char*>(Data), Size);
            dummy_file.close();
        }
    }

    // 1. Test LoadASCIIFromString
    // Treat input as ASCII JSON string
    loader.LoadASCIIFromString(&model, &err, &warn,
                               reinterpret_cast<const char*>(Data), Size,
                               "", tinygltf::REQUIRE_VERSION);
    // Reset model by reassigning a new instance since Clear() might not exist in all versions
    model = tinygltf::Model();

    // 2. Test SetFsCallbacks
    tinygltf::FsCallbacks fs_callbacks;
    fs_callbacks.FileExists = DummyFileExists;
    fs_callbacks.ExpandFilePath = DummyExpandFilePath;
    fs_callbacks.ReadWholeFile = DummyReadWholeFile;
    fs_callbacks.WriteWholeFile = DummyWriteWholeFile;
    fs_callbacks.GetFileSizeInBytes = DummyGetFileSizeInBytes;
    fs_callbacks.user_data = nullptr;
    loader.SetFsCallbacks(fs_callbacks, &err);

    // 3. Test LoadBinaryFromFile (using dummy file)
    loader.LoadBinaryFromFile(&model, &err, &warn, dummy_filename, tinygltf::REQUIRE_VERSION);
    model = tinygltf::Model();

    // 4. Test LoadASCIIFromFile (using dummy file)
    loader.LoadASCIIFromFile(&model, &err, &warn, dummy_filename, tinygltf::REQUIRE_VERSION);
    model = tinygltf::Model();

    // 5. Test SetURICallbacks
    tinygltf::URICallbacks uri_callbacks;
    uri_callbacks.encode = DummyURIEncode;
    uri_callbacks.decode = DummyURIDecode;
    uri_callbacks.user_data = nullptr;
    loader.SetURICallbacks(uri_callbacks, &err);

    // 6. Test SetImageLoader
    loader.SetImageLoader(DummyLoadImageData, nullptr);

    // Cleanup
    std::remove(dummy_filename);

    return 0;
}