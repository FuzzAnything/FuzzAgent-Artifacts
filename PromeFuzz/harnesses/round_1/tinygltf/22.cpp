// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include "tiny_gltf.h"

static bool dummyFileExists(const std::string& filename, void* user_data) {
    return false;
}

static std::string dummyExpandFilePath(const std::string& filename, void* user_data) {
    return filename;
}

static bool dummyReadWholeFile(std::vector<unsigned char>* out,
                               std::string* err,
                               const std::string& filename,
                               void* user_data) {
    return false;
}

static bool dummyWriteWholeFile(const std::string* filename,
                                const std::string& basepath,
                                const std::vector<unsigned char>& contents,
                                void* user_data) {
    return true;
}

static bool dummyGetFileSizeInBytes(size_t* size,
                                    const std::string* filename,
                                    const std::string& basepath,
                                    void* user_data) {
    return false;
}

static bool dummyURIDecode(const std::string& uri,
                           std::string* out,
                           void* user_data) {
    if (out) {
        *out = uri;
    }
    return true;
}

static bool dummyURIEncode(const std::string& uri,
                           const std::string& basepath,
                           std::string* out,
                           void* user_data) {
    if (out) {
        *out = uri;
    }
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Initialize TinyGLTF object
    tinygltf::TinyGLTF loader;
    
    // Test GetImagesAsIs and GetPreserveImageChannels on default object
    bool defaultAsIs = loader.GetImagesAsIs();
    bool defaultPreserve = loader.GetPreserveImageChannels();
    
    // Use first byte to control settings
    if (Size > 0) {
        uint8_t settings = Data[0];
        
        // Set flags based on input data
        loader.SetImagesAsIs((settings & 0x01) != 0);
        loader.SetPreserveImageChannels((settings & 0x02) != 0);
        
        // Verify settings were applied
        bool newAsIs = loader.GetImagesAsIs();
        bool newPreserve = loader.GetPreserveImageChannels();
    }
    
    // Test LoadImageData with remaining input as image data
    if (Size > 1) {
        tinygltf::Image image;
        std::string err;
        std::string warn;
        
        // Use remaining bytes as image data
        const unsigned char* image_data = Data + 1;
        size_t image_size = Size - 1;
        
        // Try loading with various parameters
        for (int i = 0; i < 4; i++) {
            tinygltf::LoadImageData(&image, i, &err, &warn, 
                                   (i & 1) ? 64 : 0,  // req_width
                                   (i & 2) ? 64 : 0,  // req_height
                                   image_data, 
                                   static_cast<int>(image_size), 
                                   nullptr);
        }
    }
    
    // Test WriteImageData with dummy image
    if (Size > 0) {
        tinygltf::Image image;
        image.width = 16;
        image.height = 16;
        image.as_is = (Data[0] & 0x01) != 0;
        
        // Create some dummy image data (RGBA, 16x16)
        image.image.resize(16 * 16 * 4, 128);
        
        // Set up dummy callbacks
        tinygltf::FsCallbacks fs_callbacks;
        fs_callbacks.FileExists = &dummyFileExists;
        fs_callbacks.ExpandFilePath = &dummyExpandFilePath;
        fs_callbacks.ReadWholeFile = &dummyReadWholeFile;
        fs_callbacks.WriteWholeFile = &dummyWriteWholeFile;
        fs_callbacks.GetFileSizeInBytes = &dummyGetFileSizeInBytes;
        fs_callbacks.user_data = nullptr;
        
        tinygltf::URICallbacks uri_callbacks;
        uri_callbacks.decode = &dummyURIDecode;
        uri_callbacks.encode = &dummyURIEncode;
        uri_callbacks.user_data = nullptr;
        
        std::string basepath = ".";
        std::string filename = "./dummy_file";
        std::string out_uri;
        
        // Try writing with different embed settings
        for (int i = 0; i < 2; i++) {
            tinygltf::WriteImageData(&basepath, &filename, &image, 
                                    i == 0,  // embedImages
                                    &fs_callbacks, 
                                    &uri_callbacks, 
                                    &out_uri, 
                                    nullptr);
        }
    }
    
    return 0;
}