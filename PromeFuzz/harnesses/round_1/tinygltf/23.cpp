// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
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

static bool dummyFileExists(const std::string&, void*) { return false; }
static std::string dummyExpandFilePath(const std::string& path, void*) { return path; }
static bool dummyReadWholeFile(std::vector<unsigned char>*, std::string*, const std::string&, void*) { return false; }
static bool dummyWriteWholeFile(std::string*, const std::string&, const std::vector<unsigned char>&, void*) { return false; }
static bool dummyGetFileSizeInBytes(size_t*, std::string*, const std::string&, void*) { return false; }

static bool dummyURIEncode(const std::string&, const std::string&, std::string*, void*) { return false; }
static bool dummyURIDecode(const std::string&, std::string*, void*) { return false; }

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Initialize TinyGLTF object
    tinygltf::TinyGLTF loader;
    
    // Test getters
    bool imagesAsIs = loader.GetImagesAsIs();
    bool preserveChannels = loader.GetPreserveImageChannels();
    
    // Test setters using data from input
    if (Size > 0) {
        loader.SetImagesAsIs(Data[0] & 1);
        if (Size > 1) {
            loader.SetPreserveImageChannels(Data[1] & 1);
        }
    }
    
    // Test WriteImageData
    if (Size > 2) {
        tinygltf::Image image;
        image.name = "test_image";
        image.width = 1;
        
        // Use input data for image content (limit to reasonable size)
        size_t imageDataSize = Size > 1024 ? 1024 : Size;
        image.image.resize(imageDataSize);
        if (Size > 2) {
            memcpy(image.image.data(), Data, imageDataSize);
        }
        
        // Set as_is flag based on input
        if (Size > 3) {
            image.as_is = Data[3] & 1;
        }
        
        std::string basepath = "./";
        std::string filename = "dummy_file";
        std::string out_uri;
        
        // Create dummy file for testing
        std::ofstream dummy_file("./dummy_file", std::ios::binary);
        if (dummy_file.is_open()) {
            dummy_file.write(reinterpret_cast<const char*>(Data), 
                           Size > 256 ? 256 : Size);
            dummy_file.close();
        }
        
        // Setup dummy callbacks
        tinygltf::FsCallbacks fs_callbacks;
        fs_callbacks.FileExists = dummyFileExists;
        fs_callbacks.ExpandFilePath = dummyExpandFilePath;
        fs_callbacks.ReadWholeFile = dummyReadWholeFile;
        fs_callbacks.WriteWholeFile = dummyWriteWholeFile;
        fs_callbacks.GetFileSizeInBytes = dummyGetFileSizeInBytes;
        fs_callbacks.user_data = nullptr;
        
        tinygltf::URICallbacks uri_callbacks;
        uri_callbacks.encode = dummyURIEncode;
        uri_callbacks.decode = dummyURIDecode;
        uri_callbacks.user_data = nullptr;
        
        // Call WriteImageData with various embed settings
        bool embedImages = Size > 4 ? (Data[4] & 1) : false;
        tinygltf::WriteImageData(&basepath, &filename, &image, embedImages,
                                &fs_callbacks, &uri_callbacks, &out_uri, nullptr);
    }
    
    // Test LoadImageData
    if (Size > 5) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        
        // Use input data as image bytes
        const unsigned char* bytes = Data;
        int size = static_cast<int>(Size > 1024 ? 1024 : Size);
        
        // Set requested dimensions based on input
        int req_width = -1;
        int req_height = -1;
        if (Size > 6) {
            req_width = static_cast<int>(Data[5]) % 64;  // Limit to reasonable size
            req_height = static_cast<int>(Data[6]) % 64;
        }
        
        // Call LoadImageData
        tinygltf::LoadImageData(&image, 0, &err, &warn, req_width, req_height,
                               bytes, size, nullptr);
    }
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}