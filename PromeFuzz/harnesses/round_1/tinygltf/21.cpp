// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
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
#include <cstring>
#include <string>
#include <vector>

static bool dummyFileExists(const std::string& filename, void* user_data) {
    return true;
}

static std::string dummyExpandFilePath(const std::string& filepath, void* user_data) {
    return filepath;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out,
                               std::string* err,
                               const std::string& filepath,
                               void* user_data) {
    return false;
}

static bool dummyWriteWholeFile(std::string* err,
                                const std::string& filename,
                                const std::vector<unsigned char>& contents,
                                void* user_data) {
    return true;
}

static bool dummyGetFileSizeInBytes(size_t* size,
                                    std::string* err,
                                    const std::string& filename,
                                    void* user_data) {
    *size = 0;
    return true;
}

static bool dummyURIEncode(const std::string& in_uri,
                           const std::string& basepath,
                           std::string* out_uri,
                           void* user_data) {
    *out_uri = in_uri;
    return true;
}

static bool dummyURIDecode(const std::string& in_uri,
                           std::string* out_uri,
                           void* user_data) {
    *out_uri = in_uri;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize TinyGLTF object
    tinygltf::TinyGLTF loader;
    
    // Use first byte to determine boolean values
    uint8_t flags = Data[0];
    bool as_is = (flags & 0x01) != 0;
    bool preserve_channels = (flags & 0x02) != 0;
    
    // Test setters and getters
    loader.SetImagesAsIs(as_is);
    loader.SetPreserveImageChannels(preserve_channels);
    
    bool get_as_is = loader.GetImagesAsIs();
    bool get_preserve = loader.GetPreserveImageChannels();
    
    // Initialize Image object
    tinygltf::Image image;
    image.name = "test_image";
    image.width = 1;
    image.height = 1;
    image.as_is = as_is;
    
    // Use remaining data for image content
    if (Size > 1) {
        size_t image_data_size = Size - 1;
        const uint8_t* image_data = Data + 1;
        image.image.assign(image_data, image_data + image_data_size);
    }
    
    // Initialize callbacks
    tinygltf::FsCallbacks fs_cb;
    fs_cb.FileExists = dummyFileExists;
    fs_cb.ExpandFilePath = dummyExpandFilePath;
    fs_cb.ReadWholeFile = dummyReadWholeFile;
    fs_cb.WriteWholeFile = dummyWriteWholeFile;
    fs_cb.GetFileSizeInBytes = dummyGetFileSizeInBytes;
    fs_cb.user_data = nullptr;
    
    tinygltf::URICallbacks uri_cb;
    uri_cb.encode = dummyURIEncode;
    uri_cb.decode = dummyURIDecode;
    uri_cb.user_data = nullptr;
    
    // Test WriteImageData with different embed modes
    std::string out_uri;
    std::string basepath = "./";
    std::string filename = "dummy_file";
    
    // Try with embedImages = true
    bool write_result1 = tinygltf::WriteImageData(&basepath, &filename, &image, 
                                                  true, &fs_cb, &uri_cb, &out_uri, nullptr);
    
    // Try with embedImages = false
    bool write_result2 = tinygltf::WriteImageData(&basepath, &filename, &image, 
                                                  false, &fs_cb, &uri_cb, &out_uri, nullptr);
    
    // Test LoadImageData
    std::string err;
    std::string warn;
    int req_width = 0;
    int req_height = 0;
    
    // Create a new image for loading
    tinygltf::Image loaded_image;
    loaded_image.as_is = as_is;
    
    // Use the same image data for loading
    const unsigned char* bytes = nullptr;
    int bytes_size = 0;
    if (Size > 1) {
        bytes = Data + 1;
        bytes_size = static_cast<int>(Size - 1);
    }
    
    bool load_result = tinygltf::LoadImageData(&loaded_image, 0, &err, &warn,
                                               req_width, req_height, bytes, bytes_size, nullptr);
    
    return 0;
}