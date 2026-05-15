// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
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

static bool dummyFileExists(const std::string&, void*) { return true; }
static std::string dummyExpandFilePath(const std::string& path, void*) { return path; }
static bool dummyReadWholeFile(std::vector<unsigned char>*, std::string*, const std::string&, void*) { return true; }
static bool dummyWriteWholeFile(std::string*, const std::string&, const std::vector<unsigned char>&, void*) { return true; }
static bool dummyGetFileSizeInBytes(size_t*, std::string*, const std::string&, void*) { return true; }

static bool dummyURIEncode(const std::string& in_uri, const std::string&, std::string* out_uri, void*) { 
    *out_uri = in_uri; 
    return true; 
}
static bool dummyURIDecode(const std::string& in_uri, std::string* out_uri, void*) { 
    *out_uri = in_uri; 
    return true; 
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Initialize tinygltf instance
    tinygltf::TinyGLTF loader;
    
    // Test SetPreserveImageChannels and GetPreserveImageChannels
    bool preserve_channels = Size > 0 ? (Data[0] % 2 == 0) : false;
    loader.SetPreserveImageChannels(preserve_channels);
    if (loader.GetPreserveImageChannels() != preserve_channels) {
        // Mismatch detected
        return 0;
    }
    
    // Test SetImagesAsIs and GetImagesAsIs
    bool images_as_is = Size > 1 ? (Data[1] % 2 == 0) : false;
    loader.SetImagesAsIs(images_as_is);
    if (loader.GetImagesAsIs() != images_as_is) {
        // Mismatch detected
        return 0;
    }
    
    // Test LoadImageData
    if (Size > 0) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        int image_idx = 0;
        
        // Use first 2 bytes for width/height if available
        int req_width = Size > 2 ? static_cast<int>(Data[2]) : 0;
        int req_height = Size > 3 ? static_cast<int>(Data[3]) : 0;
        
        // Use remaining data as image bytes
        const unsigned char* image_bytes = Size > 4 ? Data + 4 : Data;
        int image_size = Size > 4 ? static_cast<int>(Size - 4) : static_cast<int>(Size);
        
        // Try loading image data
        bool load_result = tinygltf::LoadImageData(&image, image_idx, &err, &warn, 
                                                  req_width, req_height, 
                                                  image_bytes, image_size, nullptr);
        
        // Test WriteImageData with the loaded image (or empty image if load failed)
        if (Size > 0) {
            std::string basepath = "./";
            std::string filename = "dummy_image";
            std::string out_uri;
            bool embed_images = Size > 5 ? (Data[5] % 2 == 0) : false;
            
            // Setup dummy filesystem callbacks
            tinygltf::FsCallbacks fs_callbacks;
            fs_callbacks.FileExists = dummyFileExists;
            fs_callbacks.ExpandFilePath = dummyExpandFilePath;
            fs_callbacks.ReadWholeFile = dummyReadWholeFile;
            fs_callbacks.WriteWholeFile = dummyWriteWholeFile;
            fs_callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
            fs_callbacks.user_data = nullptr;
            
            // Setup dummy URI callbacks
            tinygltf::URICallbacks uri_callbacks;
            uri_callbacks.encode = dummyURIEncode;
            uri_callbacks.decode = dummyURIDecode;
            uri_callbacks.user_data = nullptr;
            
            // Try writing image data
            bool write_result = tinygltf::WriteImageData(&basepath, &filename, &image, 
                                                        embed_images, &fs_callbacks, 
                                                        &uri_callbacks, &out_uri, nullptr);
            
            // Clean up dummy file if created
            std::string dummy_file = "./dummy_file";
            std::remove(dummy_file.c_str());
        }
    }
    
    return 0;
}