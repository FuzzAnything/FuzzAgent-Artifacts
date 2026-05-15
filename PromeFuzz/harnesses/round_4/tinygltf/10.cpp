// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
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
#include <string>
#include <vector>

// Dummy filesystem callbacks for testing with correct signatures
static bool dummy_file_exists(const std::string& filename, void* user_data) {
    return false;
}

static std::string dummy_expand_file_path(const std::string& filepath, void* user_data) {
    return filepath;
}

static bool dummy_read_whole_file(std::vector<unsigned char>* out, std::string* err,
                                  const std::string& filepath, void* user_data) {
    return false;
}

static bool dummy_write_whole_file(std::string* err, const std::string& filename,
                                   const std::vector<unsigned char>& contents,
                                   void* user_data) {
    return true;
}

static bool dummy_get_file_size(unsigned long* size, std::string* err,
                                const std::string& filename, void* user_data) {
    if (size) *size = 0;
    return true;
}

// Dummy URI callbacks for testing with correct signatures
static bool dummy_uri_encode(const std::string& basepath, const std::string& filename,
                             std::string* out, void* user_data) {
    if (out) *out = basepath + filename;
    return true;
}

static bool dummy_uri_decode(const std::string& in, std::string* out,
                             void* user_data) {
    if (out) *out = in;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Initialize tinygltf objects
    tinygltf::TinyGLTF loader;
    tinygltf::Image image;
    std::string err;
    std::string warn;
    std::string out_uri;
    
    // Set up filesystem callbacks
    tinygltf::FsCallbacks fs_cb;
    fs_cb.FileExists = dummy_file_exists;
    fs_cb.ExpandFilePath = dummy_expand_file_path;
    fs_cb.ReadWholeFile = dummy_read_whole_file;
    fs_cb.WriteWholeFile = dummy_write_whole_file;
    fs_cb.GetFileSizeInBytes = dummy_get_file_size;
    fs_cb.user_data = nullptr;
    
    // Set up URI callbacks
    tinygltf::URICallbacks uri_cb;
    uri_cb.encode = dummy_uri_encode;
    uri_cb.decode = dummy_uri_decode;
    uri_cb.user_data = nullptr;
    
    // Use first byte to determine various flags
    uint8_t flags = Size > 0 ? Data[0] : 0;
    bool preserve_channels = (flags & 0x01) != 0;
    bool images_as_is = (flags & 0x02) != 0;
    bool embed_images = (flags & 0x04) != 0;
    
    // Adjust data pointer and size if we used the first byte
    const unsigned char* image_data = Size > 0 ? Data + 1 : Data;
    int image_data_size = Size > 0 ? static_cast<int>(Size - 1) : 0;
    
    // Test 1: Set and get preserve image channels
    loader.SetPreserveImageChannels(preserve_channels);
    bool got_preserve = loader.GetPreserveImageChannels();
    
    // Test 2: Set and get images as-is
    loader.SetImagesAsIs(images_as_is);
    bool got_as_is = loader.GetImagesAsIs();
    
    // Test 3: Load image data with various parameters
    // Use different width/height values based on input data
    int req_width = 0;
    int req_height = 0;
    
    if (image_data_size >= 2) {
        req_width = static_cast<int>(image_data[0]) + 1;
        req_height = static_cast<int>(image_data[1]) + 1;
    }
    
    // Set image as_is flag based on input
    image.as_is = (flags & 0x08) != 0;
    
    // Try loading image data
    bool load_result = tinygltf::LoadImageData(&image, 0, &err, &warn, 
                                               req_width, req_height,
                                               image_data, image_data_size,
                                               nullptr);
    
    // Test 4: Write image data
    std::string basepath = "./";
    std::string filename = "dummy_file";
    
    // Only attempt write if we have a valid image
    if (load_result || image.as_is) {
        bool write_result = tinygltf::WriteImageData(&basepath, &filename, &image,
                                                     embed_images, &fs_cb, &uri_cb,
                                                     &out_uri, nullptr);
    }
    
    // Test 5: Load image data with different combinations
    // Try with as_is flag set differently
    tinygltf::Image image2;
    image2.as_is = !image.as_is;
    
    bool load_result2 = tinygltf::LoadImageData(&image2, 1, &err, &warn,
                                                0, 0, image_data, image_data_size,
                                                nullptr);
    
    // Test 6: Write with different parameters
    if (load_result2 || image2.as_is) {
        bool write_result2 = tinygltf::WriteImageData(nullptr, &filename, &image2,
                                                      !embed_images, &fs_cb, &uri_cb,
                                                      &out_uri, nullptr);
    }
    
    return 0;
}