// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
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
#include <cstddef>
#include <string>
#include <vector>

static bool dummy_FileExists(const std::string& filename, void* user_data) {
    return true;
}

static std::string dummy_ExpandFilePath(const std::string& path, void* user_data) {
    return path;
}

static bool dummy_ReadWholeFile(std::vector<unsigned char>* out, std::string* err,
                                const std::string& filename, void* user_data) {
    out->push_back('d');
    out->push_back('u');
    out->push_back('m');
    out->push_back('m');
    out->push_back('y');
    return true;
}

static bool dummy_WriteWholeFile(std::string* filename, const std::string& basepath,
                                 const std::vector<unsigned char>& contents,
                                 void* user_data) {
    return true;
}

static bool dummy_GetFileSizeInBytes(unsigned long* filesize, std::string* err,
                                     const std::string& filename, void* user_data) {
    *filesize = 5;
    return true;
}

static bool dummy_URIEncode(const std::string& in_uri, const std::string& basepath,
                            std::string* out_uri, void* user_data) {
    *out_uri = in_uri;
    return true;
}

static bool dummy_URIDecode(const std::string& in_uri, std::string* out_uri,
                            void* user_data) {
    *out_uri = in_uri;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Initialize TinyGLTF instance
    tinygltf::TinyGLTF loader;
    
    // Test SetImagesAsIs and GetImagesAsIs
    if (Size > 0) {
        bool as_is = Data[0] & 1;
        loader.SetImagesAsIs(as_is);
        bool get_as_is = loader.GetImagesAsIs();
        (void)get_as_is; // Use variable to avoid unused warning
    }
    
    // Test SetPreserveImageChannels and GetPreserveImageChannels
    if (Size > 1) {
        bool preserve_channels = Data[1] & 1;
        loader.SetPreserveImageChannels(preserve_channels);
        bool get_preserve = loader.GetPreserveImageChannels();
        (void)get_preserve; // Use variable to avoid unused warning
    }
    
    // Test WriteImageData
    if (Size > 10) {
        tinygltf::Image image;
        image.name = "test_image";
        image.width = 2;
        image.height = 2;
        image.as_is = (Size > 20) ? (Data[2] & 1) : false;
        
        // Fill image data with fuzz input
        size_t data_to_copy = std::min(Size - 3, (size_t)16);
        for (size_t i = 0; i < data_to_copy; i++) {
            image.image.push_back(Data[i + 3]);
        }
        
        // Set up callbacks
        tinygltf::FsCallbacks fs_cb;
        fs_cb.FileExists = dummy_FileExists;
        fs_cb.ExpandFilePath = dummy_ExpandFilePath;
        fs_cb.ReadWholeFile = dummy_ReadWholeFile;
        fs_cb.WriteWholeFile = dummy_WriteWholeFile;
        fs_cb.GetFileSizeInBytes = dummy_GetFileSizeInBytes;
        fs_cb.user_data = nullptr;
        
        tinygltf::URICallbacks uri_cb;
        uri_cb.encode = dummy_URIEncode;
        uri_cb.decode = dummy_URIDecode;
        uri_cb.user_data = nullptr;
        
        std::string basepath = "./";
        std::string filename = "dummy_file";
        bool embedImages = (Size > 30) ? (Data[3] & 1) : false;
        std::string out_uri;
        
        // Call WriteImageData
        bool write_result = tinygltf::WriteImageData(
            &basepath, &filename, &image, embedImages, 
            &fs_cb, &uri_cb, &out_uri, nullptr);
        (void)write_result; // Use variable to avoid unused warning
    }
    
    // Test LoadImageData
    if (Size > 5) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        
        int image_idx = (Size > 6) ? (Data[4] % 10) : 0;
        int req_width = (Size > 7) ? (Data[5] % 100) : -1;
        int req_height = (Size > 8) ? (Data[6] % 100) : -1;
        
        // Use remaining data as image bytes
        const unsigned char* bytes = nullptr;
        int bytes_size = 0;
        if (Size > 9) {
            bytes = Data + 9;
            bytes_size = static_cast<int>(Size - 9);
        }
        
        // Call LoadImageData
        bool load_result = tinygltf::LoadImageData(
            &image, image_idx, &err, &warn, 
            req_width, req_height, bytes, bytes_size, nullptr);
        (void)load_result; // Use variable to avoid unused warning
    }
    
    return 0;
}