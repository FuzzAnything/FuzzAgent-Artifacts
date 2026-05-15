// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetImagesAsIs at tiny_gltf.h:1540:8 in tiny_gltf.h
// tinygltf::TinyGLTF::SetPreserveImageChannels at tiny_gltf.h:1531:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetImagesAsIs at tiny_gltf.h:1544:8 in tiny_gltf.h
// tinygltf::TinyGLTF::GetPreserveImageChannels at tiny_gltf.h:1535:8 in tiny_gltf.h
// tinygltf::WriteImageData at tiny_gltf.cc:1120:6 in tiny_gltf.h
// tinygltf::LoadImageData at tiny_gltf.cc:971:6 in tiny_gltf.h
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

static bool dummyFileExists(const std::string&, void*) { return true; }
static std::string dummyExpandFilePath(const std::string&, void*) { return ""; }
static bool dummyReadWholeFile(std::vector<unsigned char>*, std::string*, const std::string&, void*) { return true; }
static bool dummyWriteWholeFile(std::string*, const std::string&, const std::vector<unsigned char>&, void*) { return true; }
static bool dummyGetFileSizeInBytes(unsigned long*, std::string*, const std::string&, void*) { return true; }

static bool dummyURIEncode(const std::string&, const std::string&, std::string*, void*) { return true; }
static bool dummyURIDecode(const std::string&, std::string*, void*) { return true; }

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize TinyGLTF object
    tinygltf::TinyGLTF loader;

    // Test getters before setting
    bool imagesAsIs = loader.GetImagesAsIs();
    bool preserveChannels = loader.GetPreserveImageChannels();
    (void)imagesAsIs;
    (void)preserveChannels;

    // Set values based on fuzz data
    bool setAsIs = Data[0] & 1;
    bool setPreserve = Data[0] & 2;
    loader.SetImagesAsIs(setAsIs);
    loader.SetPreserveImageChannels(setPreserve);

    // Test getters after setting
    imagesAsIs = loader.GetImagesAsIs();
    preserveChannels = loader.GetPreserveImageChannels();
    (void)imagesAsIs;
    (void)preserveChannels;

    // Prepare for WriteImageData
    tinygltf::Image image;
    image.name = "fuzz_image";
    image.width = static_cast<int>(Size % 1024);
    image.as_is = setAsIs;
    
    // Fill image data with fuzz data (avoid empty vector)
    if (Size > 1) {
        image.image.assign(Data + 1, Data + Size);
    } else {
        image.image.push_back(0);
    }

    std::string basepath = "./";
    std::string filename = "dummy_file";
    bool embedImages = Data[0] & 4;
    std::string out_uri;

    // Setup dummy callbacks
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

    // Call WriteImageData
    bool writeResult = tinygltf::WriteImageData(
        &basepath, &filename, &image, embedImages,
        &fs_cb, &uri_cb, &out_uri, nullptr);
    (void)writeResult;

    // Prepare for LoadImageData
    std::string err;
    std::string warn;
    int req_width = (Data[0] & 8) ? 64 : 0;
    int req_height = (Data[0] & 16) ? 64 : 0;
    
    // Use fuzz data as image bytes
    const unsigned char* bytes = (Size > 1) ? Data + 1 : Data;
    int bytes_size = (Size > 1) ? static_cast<int>(Size - 1) : 1;

    // Call LoadImageData with different image indices
    for (int idx = 0; idx < 2; ++idx) {
        tinygltf::Image loadImage;
        loadImage.as_is = setAsIs;
        
        bool loadResult = tinygltf::LoadImageData(
            &loadImage, idx, &err, &warn,
            req_width, req_height, bytes, bytes_size, nullptr);
        (void)loadResult;
    }

    return 0;
}